#include <iostream>
#include <fstream>
#include <array>
#include <cmath>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <list>
#include <time.h>

#include "TFile.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TMinuit.h"
#include "TEnv.h"
#include "TTree.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TPolyLine3D.h"
#include "TTreeReader.h"
#include "TRandom3.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH3F.h"

#include "../../commonDependencies/cPhysicalHit.h"
#include "../../commonDependencies/cPhysicalEvent.h"
#include "../../commonDependencies/cTrackerFine.h"
#include "../../commonDependencies/cFittedEvent.h"
#include "../../commonDependencies/cVertexFinder.h"
#include "../../commonDependencies/cDrawEvents.h"
#include "../../commonDependencies/cUtils.h"

#include "cTrackerRansacSpeed.h"

#ifdef __CLING__
#pragma link C++ class cTrackerRansac < cPhysicalHit> + ;
#endif

using namespace std;

int fit(string inputFileName = "input_parameters.txt")
{

    TString dataFileName;
    map<string, double> parMap;
    getInputMap(inputFileName, parMap, dataFileName);

    // TString dataFileName; = dataFileName;

    // Setting Hyperparameters
    gROOT->SetBatch(kFALSE);

    double threshold = parMap["tracksEnergyThreshold"];             // minimum energy requested for a single cluster.
    double fenergy = parMap["beamEnergyThreshold"];                 // energy threshold for a track in order to be considered a beam track.
    double width = parMap["tracksWidth"];                           // maximum distance from model accepted in clustering
    double fwidth = parMap["beamWidth"];                            // maximum distance from model accepted in clustering for beam tracks
    double vertexWidthAcceptance = parMap["vertexWidthAcceptance"]; // maximum distance accepted between two different cluster
    double loops = parMap["numberLoops"];                           // number of loops, i.e. number of random couples chosen.
    double trsize = parMap["trackMinSize"];                         // min number of pads required in order to consider a cluster a real track
    double besize = parMap["beamMinSize"];                          // min number of pads required in order to consider a cluster a real track FOR BEAM

    bool oneEventOnly = parMap["oneEventOnly"];
    int toAnalyse = parMap["toAnalyse"];
    int startFrom = parMap["startFrom"];

    double zRescaling = parMap["zRescaling"];

    // Drawing parameters
    int binX = parMap["binX"];
    int binY = parMap["binY"];
    int binZ = parMap["binZ"];
    float maxX = parMap["maxX"];
    float maxY = parMap["maxY"];
    float maxZ = parMap["maxZ"];

    // Opening the input file.
    TFile *ifile = new TFile(dataFileName.Data(), "READ");
    if (ifile->IsZombie())
    {
        printf("Unable to open inputfile: %s", dataFileName.Data());
        return -1;
    }

    gROOT->SetBatch(kFALSE);

    TTree *physicalEventTree = (TTree *)ifile->Get("physicalEventTree");
    if (physicalEventTree->IsZombie())
    {
        printf("Unable to open inputtree: ");
        return -1;
    }

    cFittedEvent<cPhysicalHit> *fitEvt = new cFittedEvent<cPhysicalHit>();

    TTreeReader rdr(physicalEventTree);
    TTreeReaderValue<cPhysicalEvent> event(rdr, "event"); // reading input file

    int nent = rdr.GetEntries(false);

    std::vector<double> timesIterations;
    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();

    std::vector<std::vector<std::vector<double>>> iterationCounter;
    std::vector<std::vector<std::vector<double>>> iterationEnergy;

    while (rdr.Next())
    {

        // auto start = std::chrono::steady_clock::now();

        // cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;

        // if ((rdr.GetCurrentEntry() - 1) % 100 == 0)
        // {
        //     cout << "Starting at entry:" << rdr.GetCurrentEntry() << " of " << nent << endl;
        //     start = std::chrono::steady_clock::now();
        // }

        // if (event->getEventNumber() > startFrom + 10)
        // {
        //     break;
        // }

        if ((!oneEventOnly || event->getEventNumber() == toAnalyse) || event->getEventNumber() < startFrom)
        {
            delete fitEvt;
            fitEvt = new cFittedEvent<cPhysicalHit>();
            fitEvt->setEventNumber(event->getEventNumber());
            fitEvt->setTimestamp(event->getTimestamp());
            fitEvt->getLines().clear();

            list<cPhysicalHit> hitslist = event->getHits(); // picking up the list from the event and converting it into a vector.

            cTrackerRansacSpeed<cPhysicalHit> traC;

            traC.setTracksEnergyThreshold(threshold);
            traC.setBeamEnergyThreshold(fenergy);
            traC.setTracksWidth(width);
            traC.setBeamWidth(fwidth);
            traC.setLoopsNumber(vertexWidthAcceptance);
            traC.setTrackMinSize(trsize);
            traC.setBeamMinSize(besize);

            for (auto &h : hitslist)
            {
                // Take only the pad hits that are set as trackable
                if (!h.isTrackable() || h.GetEnergy() < 0)
                {
                    fitEvt->getUnfittedPoints().push_back(h);
                    continue;
                }

                // The remaining points are good to be tracked
                traC.addPoint(h);
            }

            bool beamFittable = false;
            bool beamOneSide = false;
            bool beamTracks = true;
            traC.Ransac(fenergy, besize, fwidth, beamTracks, beamOneSide, beamFittable);

            bool trackFittable = true;
            bool trackOneSide = false;
            beamTracks = false;
            traC.Ransac(threshold, trsize, width, beamTracks, trackOneSide, trackFittable);
            // End Clustering

            iterationCounter.push_back(traC.iterSize);
            iterationEnergy.push_back(traC.iterEnergy);
            // Fit the lines
            traC.fitLines();

            for (auto &it_line : traC.lines)
            {
                fitEvt->getLines().push_back(it_line);
            }

            for (auto &it_point : traC.points)
            {
                fitEvt->getUnfittedPoints().push_back(it_point);
            }

            // Save the fitted lines and the unfitted points in the final event
            cVertexFinder<cPhysicalHit> vrt;
            vrt.setMinZ(0);
            std::vector<double> pStart = {64., 64., 500. / (zRescaling * 2.)};
            vrt.setParStart(pStart);
            vrt.setMaxZ(maxZ);
            vrt.setMaxDist(sqrt(55.));
            vrt.findVertex(fitEvt);

            cDrawEvents<cFittedEvent<cPhysicalHit>> *drawEvt =
                new cDrawEvents<cFittedEvent<cPhysicalHit>>(binX, binY, binZ, maxX, maxY, maxZ);

            drawEvt->setEvent(fitEvt);
            drawEvt->drawColors2D(false);
            drawEvt->drawTracks3D(true);

            delete drawEvt;
        }
        else
        {
            continue;
        }
    }

    std::ofstream outFileCount("iterCounter.txt");
    std::ofstream outFileEnergy("iterEnergy.txt");

    for (const auto &event : iterationCounter)
    {
        for (const auto &linee : event)
        {
            for (const auto &data : linee)
            {
                outFileCount << data << "\n";
            }
        }
        outFileCount << 100000 << "\n";
    }

    for (const auto &event : iterationEnergy)
    {
        for (const auto &linee : event)
        {
            for (const auto &data : linee)
            {
                outFileEnergy << data << "\n";
            }
        }
        outFileEnergy << 100000 << "\n";
    }

    outFileCount.close();
    outFileEnergy.close();

    return 0;
}

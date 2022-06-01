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
#include <numeric>
#include <functional>

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

#include "cPhysicalHit.h"
#include "cPhysicalEvent.h"
#include "cTrackerFine.h"
#include "cFittedEvent.h"
#include "cVertexFinder.h"
#include "cDrawEvents.h"
#include "cTrackerRansac.h"
#include "cUtils.h"
#include "cVertex.h"

#ifdef __CLING__
#pragma link C++ class cTrackerRansac < cPhysicalHit> + ;
#endif

using namespace std;

int fit(string inputFileName = "inputNMIHough.txt")
{

    TString dataFileName;
    map<string, double> parMap;
    getInputMap(inputFileName, parMap, dataFileName);

    // TString dataFileName; = dataFileName;

    // Setting Hyperparameters
    gROOT->SetBatch(kFALSE);

    getInputMap(inputFileName, parMap, dataFileName);

    double angularSteps = parMap["AngularSteps"];
    double distanceSteps = parMap["DistanceSteps"];
    double minimumEnergy = parMap["MinimumEnergy"];

    double pointDistance = parMap["PointDistance"];
    double minPoints = parMap["MinPoints"];
    double vertexWidthAcceptance = parMap["vertexWidthAcceptance"];

    float_t xRescaling = parMap["xRescaling"];
    float_t yRescaling = parMap["yRescaling"];
    double zRescaling = parMap["zRescaling"];
    double zRescalingBeam = parMap["zRescalingBeam"];

    double maxDistance = parMap["MaxDistance"];
    maxDistance = sqrt(128 * 128 + 128 * 128 + (500 * zRescaling) * (500 * zRescaling));

    double beamMinEnergy = parMap["beamEnergyThreshold"]; // energy threshold for a track in order to be considered a beam track.
    double beamPointDistance = parMap["beamWidth"];       // maximum distance from model accepted in clustering for beam tracks
    double beamMinSize = parMap["beamMinSize"];

    // Drawing parameters
    int binX = parMap["binX"];
    int binY = parMap["binY"];
    int binZ = parMap["binZ"];
    float maxX = parMap["maxX"];
    float maxY = parMap["maxY"];
    float maxZ = parMap["maxZ"];

    std::vector<Float_t> multFactor = {xRescaling, yRescaling, static_cast<float>(zRescaling)};
    std::vector<Float_t> multFactorBeam = {xRescaling, yRescaling, static_cast<float>(zRescalingBeam)};

    int skip = parMap["skip"];

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

    cFittedEvent<cPhysicalHit> *fitEvt = 0;
    cFittedEvent<cPhysicalHit> *referenceFitEvt = 0;

    // Open output file
    TString filename = "100eventsHough.root";
    TFile fout(filename.Data(), "CREATE");
    if (fout.IsZombie())
    {
        cout << "Output file creation failed" << endl;
        return 0;
    }
    fout.cd();

    map<int, int> identificationMap;
    double evtNum = 0, execTime = 0;

    TTree *fOutTree = new TTree("trackTree", "trackTree");
    // auto evtBranch = fOutTree->Branch("fullEvent", &fitEvt);
    auto executionTime = fOutTree->Branch("executionTime", &execTime);
    auto evtNumber = fOutTree->Branch("eventNum", &evtNum);
    auto parBranch = fOutTree->Branch("parameters", &parMap);
    auto fNmBranch = fOutTree->Branch("inputFile", &dataFileName);
    auto identBranch = fOutTree->Branch("identificationMap", &identificationMap);

    TTreeReader rdr(physicalEventTree);
    TTreeReaderValue<cPhysicalEvent> event(rdr, "event"); // reading input file

    int nent = rdr.GetEntries(false);

    vector<double> numLoops = {5, 10, 15, 30, 50, 80, 100, 200, 500};

    vector<double> vecAngSteps = {10, 20, 50, 100, 200, 300, 500, 1000};
    vector<double> vecDisSteps = {10, 20, 50, 100, 200, 300, 500, 1000};

    while (rdr.Next())
    {

        std::cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << std::flush;
        list<cPhysicalHit> hitslist = event->getHits(); // picking up the list from the event and converting it into a vector.

        cout << "hitslist.size()  " << hitslist.size() << endl;

        if (rdr.GetCurrentEntry() < parMap["stopAt"])
        {
            for (auto &itAng : vecAngSteps)
            {
                for (auto &itDis : vecDisSteps)
                {
                    auto startTime = chrono::steady_clock::now();
                    delete fitEvt;
                    fitEvt = new cFittedEvent<cPhysicalHit>();
                    fitEvt->setEventNumber(event->getEventNumber());
                    fitEvt->setTimestamp(event->getTimestamp());
                    fitEvt->getLines().clear();

                    cTrackerFine<cPhysicalHit> trcC;

                    trcC.setAngularSteps(itAng);
                    trcC.setDistanceSteps(itDis);
                    trcC.setMinimumEnergy(minimumEnergy);
                    trcC.setMaxDistance(maxDistance);
                    trcC.setPointDistance(pointDistance);
                    trcC.setMinPoints(minPoints);
                    trcC.points.clear();
                    trcC.accumulator.clear();
                    // trcC.lines.clear();
                    trcC.fittedLines.clear();

                    for (auto &h : hitslist)
                    {
                        // Take only the pad hits that are set as trackable
                        if (!h.isTrackable() || h.GetEnergy() < 0)
                        {
                            fitEvt->getUnfittedPoints().push_back(h);
                            continue;
                        }
                        // The remaining points are good to be tracked
                        trcC.addPoint(h);
                    }

                    //  Start the BEAM tracking on the x,z plane
                    trcC.setMinimumEnergy(beamMinEnergy);
                    trcC.setPointDistance(beamPointDistance);
                    trcC.setMinPoints(beamMinSize);
                    trcC.setLinesFittable(true);
                    trcC.setMultFactor(multFactorBeam);
                    trcC.track(cTrackerFine<cPhysicalHit>::direction::x, cTrackerFine<cPhysicalHit>::direction::y);

                    // Start the BEAM tracking on the x,z plane
                    trcC.setMinimumEnergy(minimumEnergy);
                    trcC.setPointDistance(pointDistance);
                    trcC.setMinPoints(minPoints);
                    trcC.setLinesFittable(true);
                    trcC.setMultFactor(multFactor);
                    trcC.track(cTrackerFine<cPhysicalHit>::direction::x, cTrackerFine<cPhysicalHit>::direction::z);

                    // Cluster and FitLiness
                    trcC.fitLines();

                    // Save the fitted lines and the unfitted points in the final event
                    for (auto &l : trcC.fittedLines)
                    {
                        l.setFittable(true);
                        fitEvt->getLines().push_back(l);
                    }
                    for (auto &p : trcC.points)
                    {
                        fitEvt->getUnfittedPoints().push_back(p);
                    }

                    // Save the fitted lines and the unfitted points in the final event
                    cVertexFinder<cPhysicalHit> vrt;
                    vrt.setMinZ(0);
                    std::vector<double> pStart = {64., 64., 512. / (zRescaling * 2.)};
                    vrt.setParStart(pStart);
                    vrt.setMaxZ(maxZ);
                    vrt.setMaxDist(vertexWidthAcceptance);
                    vrt.findVertex(fitEvt);

                    auto endTime = chrono::steady_clock::now();
                    execTime = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();

                    evtNum = event->getEventNumber();
                    parMap["angularSteps"] = itAng;
                    parMap["distanceSteps"] = itDis;

                    // fill identificationMap for tracked points;
                    double trackCount = 0;

                    for (auto &itLines : fitEvt->getLines())
                    {
                        trackCount += 1;
                        for (auto &itPoint : itLines.getPoints())
                        {
                            identificationMap[itPoint.getX() + 128 * itPoint.getY() + 128 * 128 * itPoint.getZ()] = trackCount;
                        }
                    }

                    // fill identificationMap for tracked points,
                    for (auto &itUnf : fitEvt->getUnfittedPoints())
                    {
                        identificationMap[itUnf.getX() + 128 * itUnf.getY() + 128 * 128 * itUnf.getZ()] = 0;
                    }

                    fOutTree->Fill();

                    identificationMap.clear();
                    evtNum = -1;
                }
            }
        }
    }

    fOutTree->Write();
    fout.Close();

    return 0;
}

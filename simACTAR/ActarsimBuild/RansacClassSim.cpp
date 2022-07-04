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

#include "/home/lorenzo/Desktop/ACTAR_git/commonDependencies/cPhysicalHit.h"
#include "/home/lorenzo/Desktop/ACTAR_git/commonDependencies/cPhysicalEvent.h"
#include "/home/lorenzo/Desktop/ACTAR_git/commonDependencies/cTrackerFine.h"
#include "/home/lorenzo/Desktop/ACTAR_git/commonDependencies/cFittedEvent.h"
#include "/home/lorenzo/Desktop/ACTAR_git/commonDependencies/cVertexFinder.h"
#include "/home/lorenzo/Desktop/ACTAR_git/commonDependencies/cDrawEvents.h"
#include "/home/lorenzo/Desktop/ACTAR_git/commonDependencies/cTrackerRansac.h"
#include "/home/lorenzo/Desktop/ACTAR_git/commonDependencies/cUtils.h"
#include "/home/lorenzo/Desktop/ACTAR_git/commonDependencies/cVertex.h"

#ifdef __CLING__
#pragma link C++ class cTrackerRansac < cPhysicalHit> + ;
#endif

using namespace std;

int fit(string inputFileName = "inputParametersSim.txt")
{

    TString dataFileName;
    map<string, double> parMap;
    getInputMap(inputFileName, parMap, dataFileName);

    // TString dataFileName; = dataFileName;

    // Setting Hyperparameters
    gROOT->SetBatch(kFALSE);

    double threshold = parMap["trackEnergyThreshold"];              // minimum energy requested for a single cluster.
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

    bool DrawMultiplicity = parMap["drawMultiplicity"];
    TH1F *hMult = new TH1F("hMult", "hMult", 10, 0, 10);

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
    std::cout << nent << std::endl;

    cout << startFrom << " startFrom" << endl;

    while (rdr.Next())
    {

        cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;

        if ((!oneEventOnly || event->getEventNumber() == toAnalyse) && rdr.GetCurrentEntry() > startFrom)
        {
            delete fitEvt;
            fitEvt = new cFittedEvent<cPhysicalHit>();
            fitEvt->setEventNumber(event->getEventNumber());
            fitEvt->setTimestamp(event->getTimestamp());
            fitEvt->getLines().clear();

            list<cPhysicalHit> hitslist = event->getHits(); // picking up the list from the event and converting it into a vector.

            cTrackerRansac<cPhysicalHit> traC;

            traC.setTracksEnergyThreshold(threshold);
            traC.setBeamEnergyThreshold(fenergy);
            traC.setTracksWidth(width);
            traC.setBeamWidth(fwidth);
            traC.setLoopsNumber(loops);
            traC.setTrackMinSize(trsize);
            traC.setBeamMinSize(besize);
            traC.zRescaling = zRescaling;

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

            // bool beamFittable = false;
            // bool beamOneSide = false;
            // bool beamTracks = true;
            // traC.Ransac(fenergy, besize, fwidth, beamTracks, beamOneSide, beamFittable);

            bool trackFittable = true;
            bool trackOneSide = true;
            bool beamTracks = false;

            traC.Ransac(beamTracks, trackOneSide, trackFittable);

            // traC.Ransac(threshold, trsize, width, beamTracks, trackOneSide, trackFittable);
            // End Clustering

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
            std::vector<double> pStart = {64., 64., 100};
            vrt.setParStart(pStart);
            vrt.setMaxZ(maxZ);
            vrt.setMaxDist(vertexWidthAcceptance);
            vrt.findVertex(fitEvt);


            cout << "\nxVer: " << fitEvt->getVertex().front().getX() << endl; 
            cout << "\nMult: " << fitEvt->getVertex().front().getTracks().size() << endl;
            hMult->Fill(fitEvt->getVertex().front().getTracks().size());
            auto listTracks = fitEvt->getLines();

            bool verbose = false;

            // cout << "verbose:" << verbose << endl;
            if (verbose)
            {

                cout << "verbose inside loop:" << verbose << endl;

                int itTracks = 0;
                for (auto &it : listTracks)
                {
                    double minZ = DBL_MAX;
                    double maxZ = -100;
                    double minX = DBL_MAX;
                    double max = -100;

                    double totalEnergy = 0;
                    for (auto &pts : it.getPoints())
                    {
                        totalEnergy += pts.getEnergy();
                        if (pts.getZ() < minZ)
                            minZ = pts.getZ();
                        if (pts.getZ() > maxZ)
                            maxZ = pts.getZ();
                        if (pts.getX() < minX)
                            minX = pts.getX();
                        if (pts.getX() > maxX)
                            maxX = pts.getX();
                    }

                    cout << "Track " << itTracks << " Energy: " << totalEnergy << "  Size: " << it.getPoints().size() << endl;
                    // cout << "Track " << itTracks << "  " << minX << "  " << maxX << "  " << minZ << "  " << maxZ << endl;
                    itTracks++;
                }
            }

            // Draw the analysed event;
            cDrawEvents<cFittedEvent<cPhysicalHit>> *drawEvt =
                new cDrawEvents<cFittedEvent<cPhysicalHit>>(binX, binY, binZ, maxX, maxY, maxZ);

            drawEvt->setEvent(fitEvt);
            // drawEvt->drawAll3D(false);
            // drawEvt->drawAll2D(false);
            drawEvt->drawComponents2D(false, 0, 0, 800, 500, " ");
            drawEvt->drawColors2D(true, 800, 0, 800, 500, "  ");
            // drawEvt->drawTracks3D(false, 800, 600, 1115, 500);
            //  drawEvt->drawVertex(true, 800, 0, 1115, 500);

            delete drawEvt;
            // }

            // Coding for finding informations on tracks;
        }
        else
        {
            continue;
        }
    }

    if (DrawMultiplicity)
    {
        auto *c = new TCanvas();
        c->cd();
        hMult->Draw();
        c->WaitPrimitive();
    }

    return 0;
}

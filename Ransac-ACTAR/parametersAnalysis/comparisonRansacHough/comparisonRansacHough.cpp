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

int fit(string inputFileName = "inputParametersFill.txt")
{

    TString dataFileName;
    map<string, double> parMap;
    getInputMap(inputFileName, parMap, dataFileName);

    // TString dataFileName; = dataFileName;

    // Setting Hyperparameters
    gROOT->SetBatch(kFALSE);

    double referenceThreshold = parMap["trackEnergyThreshold"];              // minimum energy requested for a single cluster.
    double referenceFenergy = parMap["beamEnergyThreshold"];                 // energy threshold for a track in order to be considered a beam track.
    double referenceWidth = parMap["tracksWidth"];                           // maximum distance from model accepted in clustering
    double referenceFwidth = parMap["beamWidth"];                            // maximum distance from model accepted in clustering for beam tracks
    double referenceVertexWidthAcceptance = parMap["vertexWidthAcceptance"]; // maximum distance accepted between two different cluster
    double referenceTrsize = parMap["trackMinSize"];                         // min number of pads required in order to consider a cluster a real track
    double referenceBesize = parMap["beamMinSize"];                          // min number of pads required in order to consider a cluster a real track FOR BEAM
    double referenceLoops = parMap["referenceLoops"];                        // number of loops, i.e. number of random couples chosen.
    double referenceZRescaling = parMap["referenceZRescaling"];

    double threshold = parMap["trackEnergyThreshold"];              // minimum energy requested for a single cluster.
    double fenergy = parMap["beamEnergyThreshold"];                 // energy threshold for a track in order to be considered a beam track.
    double width = parMap["tracksWidth"];                           // maximum distance from model accepted in clustering
    double fwidth = parMap["beamWidth"];                            // maximum distance from model accepted in clustering for beam tracks
    double vertexWidthAcceptance = parMap["vertexWidthAcceptance"]; // maximum distance accepted between two different cluster
    double trsize = parMap["trackMinSize"];                         // min number of pads required in order to consider a cluster a real track
    double besize = parMap["beamMinSize"];                          // min number of pads required in order to consider a cluster a real track FOR BEAM
    double loops = parMap["ransacLoops"];                           // number of loops, i.e. number of random couples chosen.

    bool oneEventOnly = parMap["oneEventOnly"];
    int toAnalyse = parMap["toAnalyse"];
    int startFrom = parMap["startFrom"];
    double zRescaling = parMap["zRescaling"];

    double numberMaxLoops = parMap["numberMaximumLoops"]; // number of loops, i.e. number of random couples chosen.
    int loopsSteps = parMap["loopsSteps"];

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

    cFittedEvent<cPhysicalHit> *fitEvt = 0;
    cFittedEvent<cPhysicalHit> *referenceFitEvt = 0;

    TTreeReader rdr(physicalEventTree);
    TTreeReaderValue<cPhysicalEvent> event(rdr, "event"); // reading input file

    int nent = rdr.GetEntries(false);

    cout << "tutok?" << endl;
    while (rdr.Next())
    {

        if (rdr.GetCurrentEntry() == 8)
        {
            std::cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << std::endl;

            list<cPhysicalHit> hitslist = event->getHits(); // picking up the list from the event and converting it into a vector.

            // FIT reference - with reference parameters.
            cTrackerRansac<cPhysicalHit> referenceTraC;

            delete referenceFitEvt;
            referenceFitEvt = new cFittedEvent<cPhysicalHit>();
            referenceFitEvt->setEventNumber(event->getEventNumber());
            referenceFitEvt->setTimestamp(event->getTimestamp());
            referenceFitEvt->getLines().clear();

            referenceTraC.setTracksEnergyThreshold(referenceThreshold);
            referenceTraC.setBeamEnergyThreshold(referenceFenergy);
            referenceTraC.setTracksWidth(referenceWidth);
            referenceTraC.setBeamWidth(referenceFwidth);
            referenceTraC.setLoopsNumber(referenceLoops);
            referenceTraC.setTrackMinSize(referenceTrsize);
            referenceTraC.setBeamMinSize(referenceBesize);
            referenceTraC.zRescaling = referenceZRescaling;

            for (auto &h : hitslist)
            {
                // Take only the pad hits that are set as trackable
                if (!h.isTrackable() || h.GetEnergy() < 0)
                {
                    fitEvt->getUnfittedPoints().push_back(h);
                    continue;
                }
                // The remaining points are good to be tracked
                referenceTraC.addPoint(h);
            }

            bool referenceTrackFittable = true;
            bool referenceTrackOneSide = true;
            bool referenceBeamTracks = false;

            referenceTraC.Ransac(referenceThreshold, referenceTrsize, referenceWidth, referenceBeamTracks, referenceTrackOneSide, referenceTrackFittable);

            cout << "loops:" << referenceLoops << endl;
            cout << "energy: " << referenceThreshold << endl;
            // Cluster and FitLiness
            referenceTraC.fitLines();

            for (auto &it_line : referenceTraC.lines)
            {
                referenceFitEvt->getLines().push_back(it_line);
            }

            for (auto &it_point : referenceTraC.points)
            {
                referenceFitEvt->getUnfittedPoints().push_back(it_point);
            }

            // Save the fitted lines and the unfitted points in the final event
            cVertexFinder<cPhysicalHit> referernceVrt;
            referernceVrt.setMinZ(0);
            std::vector<double> referencepStart = {64., 64., 500. / (referenceZRescaling * 2.)};
            referernceVrt.setParStart(referencepStart);
            referernceVrt.setMaxZ(maxZ);
            referernceVrt.setMaxDist(vertexWidthAcceptance);
            referernceVrt.findVertex(referenceFitEvt);
            // END reference fit

            // START comparison fit
            auto start = chrono::steady_clock::now();

            // std::cout << "\rloops: " << loops << std::flush;
            delete fitEvt;
            fitEvt = new cFittedEvent<cPhysicalHit>();
            fitEvt->setEventNumber(event->getEventNumber());
            fitEvt->setTimestamp(event->getTimestamp());
            fitEvt->getLines().clear();

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

            bool trackFittable = true;
            bool trackOneSide = true;
            bool beamTracks = false;
            traC.Ransac(threshold, trsize, width, beamTracks, trackOneSide, trackFittable);

            // Cluster and FitLiness
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
            vrt.setMaxDist(vertexWidthAcceptance);
            vrt.findVertex(fitEvt);

            cDrawEvents<cFittedEvent<cPhysicalHit>> *referenceDrawEvt = new cDrawEvents<cFittedEvent<cPhysicalHit>>(binX, binY, binZ, maxX, maxY, maxZ);
            referenceDrawEvt->setEvent(*referenceFitEvt);
            referenceDrawEvt->drawColors2D(false, 0, 600, 800, 500, "referenceCanvasColors");
            referenceDrawEvt->drawComponents2D(false, 800, 600, 800, 500, "referenceComponents");

            cDrawEvents<cFittedEvent<cPhysicalHit>> *drawEvt = new cDrawEvents<cFittedEvent<cPhysicalHit>>(binX, binY, binZ, maxX, maxY, maxZ);
            drawEvt->setEvent(*fitEvt);
            drawEvt->drawColors2D(false, 0, 0, 800, 500, "CanvasColors");
            drawEvt->drawComponents2D(true, 800, 0, 800, 500, " components2D");
        }
    }

    // fOutTree->Write();
    // fout.Write();
    // fout.Close();
    // Coding for finding informations on tracks;
    return 0;
}

// std::vector<double> vecLoops;

// for (int i = 1; i < numberMaxLoops; i += loopsSteps)
// {
//     vecLoops.push_back(i);
// }

// vecLoops.push_back(1000);

// Open output file
// TString filename = "prova100Events.root";
// TFile fout(filename.Data(), "CREATE");
// if (fout.IsZombie())
// {
//     cout << "Output file creation failed" << endl;
//     return 0;
// }
// fout.cd();

// double numberLoops = 0.;
// double durationTime = 0.;
// double durationTimeTotal = 0.;
// double durationTimeClustering = 0.;
// double durationTimeFitting = 0.;

// TTree *fOutTree = new TTree("trackTree", "trackTree");
// fOutTree->Branch("numberLoops", &numberLoops);
// fOutTree->Branch("Full_event", &fitEvt);
// fOutTree->Branch("durationTimeTotal", &durationTimeTotal);
// fOutTree->Branch("durationTimeClustering", &durationTimeClustering);
// fOutTree->Branch("durationTimeFitting", &durationTimeFitting);
// fOutTree->SetDirectory(&fout);
// fOutTree->AutoSave();
// fOutTree->Branch("distanceSteps", &distanceSteps);

// measure total time
// auto endTimeTotal = chrono::steady_clock::now();
// durationTimeTotal = chrono::duration_cast<chrono::microseconds>(endTimeTotal - start).count();

// fOutTree->Fill();
// // measure clustering time
// auto endTimeClustering = chrono::steady_clock::now();
// durationTimeClustering = chrono::duration_cast<chrono::microseconds>(endTimeClustering - start).count();

// std::vector<double> numPtsLoops;
// std::vector<double> energiesLoops;
// std::vector<std::vector<double>> lengthTracksLoops;
// std::vector<std::vector<double>> energyTracksLoops;
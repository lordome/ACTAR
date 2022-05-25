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

int fit(string inputFileName = "inputParametersFillDurationAnalysis.txt")
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
    double trsize = parMap["trackMinSize"];                         // min number of pads required in order to consider a cluster a real track
    double besize = parMap["beamMinSize"];                          // min number of pads required in order to consider a cluster a real track FOR BEAM

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

    // Open output file
    TString filename = "correlationNumTracksDurationTime.root";
    TFile fout(filename.Data(), "CREATE");
    if (fout.IsZombie())
    {
        cout << "Output file creation failed" << endl;
        return 0;
    }
    fout.cd();

    cFittedEvent<cPhysicalHit> *fitEvt = 0;
    double numberLoops = 0.;
    double durationTime = 0.;
    double durationTimeTotal = 0.;
    double durationTimeClustering = 0.;
    double durationTimeFitting = 0.;
    unsigned int numTracks = 0;
    unsigned int numVertexTracks = 0;

    TTree *fOutTree = new TTree("trackTree", "trackTree");
    fOutTree->Branch("numberLoops", &numberLoops);
    fOutTree->Branch("numTracks", &numTracks);
        fOutTree->Branch("numVertexTracks", &numVertexTracks);

    fOutTree->Branch("durationTimeTotal", &durationTimeTotal);
    fOutTree->Branch("durationTimeClustering", &durationTimeClustering);
    fOutTree->Branch("durationTimeFitting", &durationTimeFitting);

    fOutTree->SetDirectory(&fout);
    fOutTree->AutoSave();
    // fOutTree->Branch("distanceSteps", &distanceSteps);

    TTreeReader rdr(physicalEventTree);
    TTreeReaderValue<cPhysicalEvent> event(rdr, "event"); // reading input file

    int nent = rdr.GetEntries(false);
    std::cout << nent << std::endl;

    std::vector<std::vector<double>> energiesResultsLoops;
    std::vector<std::vector<double>> numPtsResultsLoops;
    std::vector<std::vector<std::vector<double>>> lengthTracksResultsLoops;
    std::vector<std::vector<std::vector<double>>> energyTracksResultsLoops;
    std::vector<std::vector<double>> vecDurations;

    std::vector<double> vecLoops;
    vecLoops.push_back(1000);

    while (rdr.Next())
    {
        std::vector<double> numPtsLoops;
        std::vector<double> energiesLoops;
        std::vector<std::vector<double>> lengthTracksLoops;
        std::vector<std::vector<double>> energyTracksLoops;

        std::cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << std::endl;

        numberLoops = 100;
        auto start = chrono::steady_clock::now();

        // std::cout << "\rloops: " << loops << std::flush;
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
        traC.setLoopsNumber(numberLoops);
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

        // measure clustering time
        auto endTimeClustering = chrono::steady_clock::now();
        durationTimeClustering = chrono::duration_cast<chrono::microseconds>(endTimeClustering - start).count();

        // Cluster and FitLiness
        traC.fitLines();

        auto endTimeFitting = chrono::steady_clock::now();
        durationTimeFitting = chrono::duration_cast<chrono::microseconds>(endTimeFitting - start).count();

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

        // measure total time
        auto endTimeTotal = chrono::steady_clock::now();
        durationTimeTotal = chrono::duration_cast<chrono::microseconds>(endTimeTotal - start).count();

        numTracks = fitEvt->getLines().size();

        numVertexTracks = fitEvt->getVertex().front().getTracks().size();

        fOutTree->Fill();
    }

    fOutTree->Write();
    fout.Write();
    fout.Close();
    // Coding for finding informations on tracks;
    return 0;
}

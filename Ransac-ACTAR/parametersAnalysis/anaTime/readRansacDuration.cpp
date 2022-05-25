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
#include "TMultiGraph.h"

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

int fit(string inputFileName = "inputParametersDurationAnalysis.txt")
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

    TTree *trackTree = (TTree *)ifile->Get("trackTree");
    if (trackTree->IsZombie())
    {
        printf("Unable to open inputtree: ");
        return -1;
    }

    TTreeReader rdr(trackTree);
    TTreeReaderValue<cFittedEvent<cPhysicalHit>> fitEvt(rdr, "Full_event");         // reading input file
    TTreeReaderValue<double> durationTimeTotal(rdr, "durationTimeTotal");           // reading input file
    TTreeReaderValue<double> durationTimeClustering(rdr, "durationTimeClustering"); // reading input file
    TTreeReaderValue<double> durationTimeFitting(rdr, "durationTimeFitting");       // reading input file
    TTreeReaderValue<double> numberLoops(rdr, "numberLoops");

    int nent = rdr.GetEntries(false);
    std::cout << nent << std::endl;

    std::vector<std::vector<double>> energiesResultsLoops;
    std::vector<std::vector<double>> numPtsResultsLoops;
    std::vector<std::vector<std::vector<double>>> lengthTracksResultsLoops;
    std::vector<std::vector<std::vector<double>>> energyTracksResultsLoops;
    std::vector<std::vector<double>> vecDurations;
    std::vector<double> vecLoops;

    unsigned int durationGraphCount = 0;
    unsigned int correlationPointsCount = 0;
    TMultiGraph *mg = new TMultiGraph();
    TMultiGraph *mgRescaled = new TMultiGraph();

    TGraph *gDurationTotal = new TGraph();
    gDurationTotal->SetMarkerColor(1);
    gDurationTotal->SetTitle("Total time");

    TGraph *gDurationFitting = new TGraph();
    gDurationFitting->SetMarkerColor(2);
    gDurationFitting->SetTitle("Fitting time");

    TGraph *gDurationClustering = new TGraph();
    gDurationClustering->SetMarkerColor(3);
    gDurationClustering->SetTitle("Clustering time");

    TGraph *gDurationFittingRescaled = new TGraph();
    gDurationFittingRescaled->SetMarkerColor(2);
    gDurationFittingRescaled->SetTitle("Fitting Rescaled time");

    TGraph *gDurationClusteringRescaled = new TGraph();
    gDurationClusteringRescaled->SetMarkerColor(3);
    gDurationClusteringRescaled->SetTitle("Clustering Rescaled time");

    TCanvas *cTemp = new TCanvas("cT", "cT", 100, 100, 500, 500);
    TCanvas *cRescaled = new TCanvas("cRescaled", "cRescaled", 600, 100, 500, 500);

    unsigned int maxTrackNum = 0;
    unsigned int maxTrackInVertex = 0;

    std::vector<std::vector<double>> vecCorrelationNumTracks(8, std::vector<double>());
    std::vector<std::vector<double>> vecCorrelationVertexTracks(4, std::vector<double>());

    TGraph *gCorrNum = new TGraph();
    TGraph *gCorrVerNum = new TGraph();

    while (rdr.Next())
    {
        if (0 && rdr.GetCurrentEntry() % 26 == 0 && rdr.GetCurrentEntry() != 0)
        {
            delete cTemp;
            cTemp = new TCanvas("cT", "cT", 100, 100, 500, 500);
            cTemp->cd();
            // gDurationTotal->Draw("AP*");
            // gDurationClustering->Draw("AP*");
            // gDurationFitting->Draw("AP*");

            mg->Add(gDurationTotal);
            mg->Add(gDurationClustering);
            mg->Add(gDurationFitting);
            mg->Draw("AP*");
            cTemp->BuildLegend();
            // cTemp->WaitPrimitive();

            delete cRescaled;
            cRescaled = new TCanvas("cRescaled", "cRescaled", 600, 100, 500, 500);
            cRescaled->cd();
            // gDurationTotal->Draw("AP*");
            // gDurationClustering->Draw("AP*");
            // gDurationFitting->Draw("AP*");

            mgRescaled->Add(gDurationClusteringRescaled);
            mgRescaled->Add(gDurationFittingRescaled);
            mgRescaled->Draw("AP*");
            cRescaled->BuildLegend();
            cRescaled->WaitPrimitive();

            delete gDurationTotal;
            delete gDurationClustering;
            delete gDurationFitting;
            delete gDurationClusteringRescaled;
            delete gDurationFittingRescaled;

            delete mg;

            gDurationTotal = new TGraph();
            gDurationClustering = new TGraph();
            gDurationFitting = new TGraph();
            gDurationClusteringRescaled = new TGraph();
            gDurationFittingRescaled = new TGraph();

            gDurationTotal->SetMarkerColor(1);
            gDurationFitting->SetMarkerColor(2);
            gDurationClustering->SetMarkerColor(3);
            gDurationFittingRescaled->SetMarkerColor(2);
            gDurationClusteringRescaled->SetMarkerColor(3);

            gDurationTotal->SetTitle("Total time");
            gDurationFitting->SetTitle("Fitting time");
            gDurationClustering->SetTitle("Clustering time");
            gDurationFittingRescaled->SetTitle("Fitting rescaled time");
            gDurationClusteringRescaled->SetTitle("Clustering rescaled time");

            mg = new TMultiGraph();

            durationGraphCount = 0;
        }

        // fill duration graph
        gDurationTotal->SetPoint(durationGraphCount, *numberLoops, *durationTimeTotal);
        gDurationFitting->SetPoint(durationGraphCount, *numberLoops, *durationTimeFitting);
        gDurationClustering->SetPoint(durationGraphCount, *numberLoops, *durationTimeClustering);

        gDurationFittingRescaled->SetPoint(durationGraphCount, *numberLoops, *durationTimeFitting / *durationTimeTotal);
        gDurationClusteringRescaled->SetPoint(durationGraphCount, *numberLoops, *durationTimeClustering / *durationTimeTotal);

        ++durationGraphCount;

        if (rdr.GetCurrentEntry() % 26 == 0)
        {
            gCorrNum->SetPoint(durationGraphCount, fitEvt->getLines().size(), 1.0 - *durationTimeFitting / *durationTimeTotal);

            if (fitEvt->getVertex().front().getTracks().size() < 200)
            {
                gCorrVerNum->SetPoint(durationGraphCount, fitEvt->getVertex().front().getTracks().size(), 1.0 - *durationTimeFitting / *durationTimeTotal);
            }

            correlationPointsCount++;
        }
        fitEvt->getLines().size() > maxTrackNum ? maxTrackNum = fitEvt->getLines().size() : maxTrackNum = maxTrackNum;
        fitEvt->getVertex().front().getTracks().size() > maxTrackInVertex ? maxTrackInVertex = fitEvt->getVertex().front().getTracks().size() : maxTrackInVertex = maxTrackInVertex;

        // std::cout << maxTrackNum << "   " << fitEvt->getLines().size() << "\n";
        // std::cout << maxTrackInVertex << "   " << fitEvt->getVertex().front().getTracks().size() << "\n";

        // vecCorrelationNumTracks[fitEvt->getLines().size() - 1].push_back(1. - *durationTimeFitting / *durationTimeTotal);
        // vecCorrelationNumTracks[fitEvt->getVertex().front().getTracks().size() - 1].push_back(1. - *durationTimeFitting / *durationTimeTotal);
    }

    auto cDuration = new TCanvas();
    cDuration->cd();
    gDurationTotal->Draw("AP*");

    auto cCorr1 = new TCanvas("cCorr1", "cCorr1");
    cCorr1->cd();
    gCorrNum->Draw("AP*");

    auto cCorr2 = new TCanvas("cCorr2", "cCorr2");
    cCorr2->cd();
    gCorrVerNum->Draw("AP*");
    return 1000;
}

// std::vector<double> numPtsLoops;
// std::vector<double> energiesLoops;
// std::vector<std::vector<double>> lengthTracksLoops;
// std::vector<std::vector<double>> energyTracksLoops;

// std::cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << std::endl;

// double enEnd = energiesLoops.back();
// for (auto &itEne : energiesLoops)
// {
//     itEne /= enEnd;
// }
// double ptsEnd = numPtsLoops.back();
// for (auto &itPts : numPtsLoops)
// {
//     itPts /= ptsEnd;
// }

// energiesResultsLoops.push_back(energiesLoops);
// numPtsResultsLoops.push_back(numPtsLoops);
// lengthTracksResultsLoops.push_back(lengthTracksLoops);
// energyTracksResultsLoops.push_back(energyTracksLoops);

// std::vector<double> energiesGraph;
// std::vector<double> numPtsGraph;
// std::vector<double> mseLenghtsGraph;
// std::vector<double> mseEnergiesGraph;

// vecLoops.pop_back();

// for (unsigned int i = 0; i < vecLoops.size(); ++i)
// {
//     double tmpSumEnergy = 0;
//     double tmpSumNumPts = 0;

//     for (unsigned int j = 0; j < energiesResultsLoops.size() - 1; ++j)
//     {
//         tmpSumEnergy += energiesResultsLoops[j][i];
//         tmpSumNumPts += numPtsResultsLoops[j][i];
//     }

//     energiesGraph.push_back(tmpSumEnergy);
//     numPtsGraph.push_back(tmpSumNumPts);
// }

// for (auto &itEne : energiesGraph)
// {
//     itEne /= energiesResultsLoops.size();
// }
// for (auto &itPts : numPtsGraph)
// {
//     itPts /= numPtsGraph.size();
// }

// for (auto &itResults : lengthTracksResultsLoops)
// {
//     std::vector<double> lengthLastLoop = itResults.back();

//     double mseCumul = 0.0;
//     int mseCount = 0;

//     for (auto &itLoops : itResults)
//     {

//         for (auto &itSingle : itLoops)
//         {
//             cout << itSingle << " ";
//         }
//         std::cout << "\n";

//         int totalPts = std::accumulate(lengthLastLoop.begin(), lengthLastLoop.end(), 0.);
//         for (unsigned int i = 0; i < itLoops.size() && i < lengthLastLoop.size(); ++i)
//         {
//             mseCumul += pow((itLoops[i] - lengthLastLoop[i]), 2);
//             ++mseCount;
//         }
//         mseLenghtsGraph.push_back(sqrt(mseCumul / (1.0 * mseCount)) / totalPts);
//     }
// }

// for (auto &itResults : energyTracksResultsLoops)
// {
//     std::vector<double> energyLastLoop = itResults.back();

//     double mseCumul = 0.0;
//     int mseCount = 0;

//     for (auto &itLoops : itResults)
//     {

//         for (auto &itSingle : itLoops)
//         {
//             cout << itSingle << " ";
//         }
//         std::cout << "\n";

//         double totalEnergy = std::accumulate(energyLastLoop.begin(), energyLastLoop.end(), 0);
//         for (unsigned int i = 0; i < itLoops.size() && i < energyLastLoop.size(); ++i)
//         {
//             mseCumul += pow((itLoops[i] - energyLastLoop[i]), 2);
//             ++mseCount;
//         }
//         mseEnergiesGraph.push_back(sqrt(mseCumul / (1.0 * mseCount)) / totalEnergy);
//     }
// }

// // DRAW AND PRINT RESULTS
// TCanvas *c1 = new TCanvas("c1", "c1", 200, 10, 500, 300);
// TGraph *gr = new TGraph();
// for (unsigned int i = 0; i < vecLoops.size(); i++)
// {
//     gr->SetPoint(i, vecLoops[i], energiesGraph[i]);
// }
// gr->Draw("AC*");

// TCanvas *c2 = new TCanvas("c2", "c2", 200, 10, 500, 300);
// TGraph *gr2 = new TGraph();
// gr2->GetXaxis()->SetTitle("Number of loops");
// gr2->GetYaxis()->SetTitle("");

// for (unsigned int i = 0; i < vecLoops.size(); i++)
// {
//     gr2->SetPoint(i, vecLoops[i], numPtsGraph[i]);
// }
// gr2->Draw("AC*");

// TCanvas *c3 = new TCanvas("c3", "c3", 200, 10, 500, 300);
// TGraph *gr3 = new TGraph();
// for (unsigned int i = 0; i < vecLoops.size(); i++)
// {
//     cout << vecLoops[i] << "  " << mseLenghtsGraph[i] << "\n";
//     gr3->SetPoint(i, vecLoops[i], mseLenghtsGraph[i]);
// }
// gr3->Draw("AC*");

// TCanvas *c4 = new TCanvas("c4", "c4", 200, 10, 500, 300);
// TGraph *gr4 = new TGraph();
// for (unsigned int i = 0; i < vecLoops.size(); i++)
// {
//     cout << vecLoops[i] << "  " << mseEnergiesGraph[i] << "\n";
//     gr4->SetPoint(i, vecLoops[i], mseEnergiesGraph[i]);
// }
// gr4->Draw("AC*");

// auto *gr5 = new TGraph();
// TCanvas *c5 = new TCanvas("c5", "c5", 200, 10, 500, 300);
// c5->cd();
// unsigned int counter = 0;
// for (auto &itDur : vecDurations)
// {
//     gr5->SetPoint(counter, itDur[0], itDur[1]);
//     ++counter;
// }
// gr5->Draw("AC*");

// return 0;
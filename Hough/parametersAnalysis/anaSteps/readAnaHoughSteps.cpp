#include <TROOT.h>
#include <functional>
#include <numeric>
#include <unordered_map>
#include <TChain.h>
#include <TFile.h>
#include <fstream>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TH1.h>
#include "TProofOutputFile.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH3F.h"
#include "TGraph.h"

// Headers needed by this particular selector
// #include "../root_e796/LinkDef.h"
// #include "../commonDependencies/cPhysicalHit.h"
// #include "../commonDependencies/cPhysicalEvent.h"
// #include "cFittedEvent.h"
#include "cTrackerFine.h"
#include "cVertexFinder.h"
#include "cDrawEvents.h"
#include "cUtils.h"

using namespace std;

int fit(TString inputFileName = "durationTreeWithParameters.root")
{

    TString dataFileName;
    map<string, double> parMap;

    std::string inName = "input_parameters_hough.txt";
    getInputMap(inName, parMap, dataFileName);

    double maxAngularSteps = parMap["maxAngularSteps"];
    double maxDistanceSteps = parMap["maxDistanceSteps"];
    double stepsAngularSteps = parMap["stepsAngularSteps"];
    double stepsDistanceSteps = parMap["stepsDistanceSteps"];

    // Opening the input file.
    TFile *ifile = new TFile(inputFileName.Data(), "READ");
    if (ifile->IsZombie())
    {
        printf("Unable to open inputfile: %s", inputFileName.Data());
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
    TTreeReaderValue<cFittedEvent<cPhysicalHit>> fitEvt(rdr, "Full_event"); // reading input file
    TTreeReaderValue<double> durationTime(rdr, "durationTime");             // reading input file

    TTreeReaderValue<double> readAngularSteps(rdr, "angularSteps");   // reading input file
    TTreeReaderValue<double> readDistanceSteps(rdr, "distanceSteps"); // reading input file

    // DEFINITION OF GRID OF PARAMETERS
    std::vector<std::pair<double, double>> vecParams;
    double tmpParAngle = 0.;
    double tmpParDistance = 0.;

    for (unsigned int i = 1; tmpParAngle < maxAngularSteps; ++i)
    {
        tmpParAngle += stepsAngularSteps;

        for (unsigned int j = 0; tmpParDistance < maxDistanceSteps; ++j)
        {
            tmpParDistance += stepsDistanceSteps;
            vecParams.push_back(std::make_pair(tmpParAngle, tmpParDistance));
        }
        tmpParDistance = 0;
    }

    vecParams.push_back(std::make_pair(200, 200));

    double angularSteps = 0.;
    double distanceSteps = 0.;

    // Vectors for storing results
    std::vector<std::vector<double>> energiesResultsLoops;
    std::vector<std::vector<double>> numPtsResultsLoops;

    int nent = rdr.GetEntries();

    unsigned int iterCount = 0;

    int iteratorIndex1 = 0;
    int iteratorIndex2 = 0;

    std::vector<std::vector<float>> maybeEnergies(10, std::vector<float>(10, 0));
    std::vector<std::vector<float>> sumAvgTrackLength(10, std::vector<float>(10, 0));

    std::vector<std::vector<std::vector<float>>> vecAvgTrackLength(10, std::vector<std::vector<float>>(10, std::vector<float>()));

    std::vector<std::vector<std::vector<float>>> vecIterationTimes(10, std::vector<std::vector<float>>(10, std::vector<float>()));

    unsigned int itUnico = 0;
    vecParams.pop_back();

    TH1 *h = new TH1F("h1", "Execution Time [us]", 500, 0, 6000000);

    TH2 *h2DTime = new TH2F("h2DTime", "ExecutionTime vs Steps [us]", 10, 0, 500, 10, 0, 500);
    h2DTime->GetXaxis()->SetTitle("angularSteps");
    h2DTime->GetYaxis()->SetTitle("distanceSteps");

    while (rdr.Next())
    {
        // sum energies for every track and store it in the right spot in the vector.
        double tmpSumEnergy = 0;
        vector<double> countPts = {};
        for (auto &itLines : fitEvt->getLines())
        {
            for (auto &itPoints : itLines.getPoints())
            {
                tmpSumEnergy += itPoints.getEnergy();
            }
            countPts.push_back(itLines.getPoints().size());
        }
        maybeEnergies[vecParams[itUnico].first / 50 - 1][vecParams[itUnico].second / 50 - 1] += tmpSumEnergy;

        sumAvgTrackLength[vecParams[itUnico].first / 50 - 1][vecParams[itUnico].second / 50 - 1] += std::accumulate(countPts.begin(), countPts.end(), 0.0) / countPts.size() * 1.0;
        vecAvgTrackLength[vecParams[itUnico].first / 50 - 1][vecParams[itUnico].second / 50 - 1].insert(vecAvgTrackLength[vecParams[itUnico].first / 50 - 1][vecParams[itUnico].second / 50 - 1].end(), countPts.begin(), countPts.end());

        itUnico += 1;
        if (itUnico >= vecParams.size())
        {
            itUnico = 0;
        }

        h->Fill(*durationTime);
        h2DTime->Fill(*readAngularSteps, *readDistanceSteps, *durationTime);
    }

    cout << "start analysing" << endl;
    // ANALYSE TRACKS AND VECTORS FOUND
    // std::vector<double> energiesGraph;
    std::vector<double> numPtsGraph;
    std::vector<double> mseLenghtsGraph;
    std::vector<double> mseEnergiesGraph;

    std::vector<std::vector<float>> energiesGraph(10, std::vector<float>(10, 0));

    // DRAW AND PRINT RESULTS
    TCanvas *c1 = new TCanvas("c1", "c1", 200, 10, 500, 300);
    TH2D *h2D_1 = new TH2D("h2d_1", "Total reconstructed energy vs steps", 10, 0, 500, 10, 0, 500);
    h2D_1->GetXaxis()->SetTitle("angularSteps");
    h2D_1->GetYaxis()->SetTitle("distanceSteps");
    for (unsigned int i = 0; i < vecParams.size(); i++)
    {
        // cout << i << "  " << maybeEnergies[inde][j] << endl;
        unsigned int inde = int(vecParams[i].first / 50 - 1);
        unsigned int j = int(vecParams[i].second / 50 - 1);
        h2D_1->Fill(vecParams[i].first, vecParams[i].second, maybeEnergies[inde][j]);
    }
    h2D_1->Draw("COLZ");

    TCanvas *c2 = new TCanvas("c2", "c2", 700, 10, 500, 300);
    c2->cd();

    TH2D *h2d_2 = new TH2D("h2d_2", "Number of clustered points vs steps", 10, 0, 500, 10, 0, 500);
    h2d_2->GetXaxis()->SetTitle("angularSteps");
    h2d_2->GetYaxis()->SetTitle("distanceSteps");
    for (unsigned int i = 0; i < vecParams.size(); i++)
    {
        unsigned int inde = int(vecParams[i].first / 50 - 1);
        unsigned int j = int(vecParams[i].second / 50 - 1);
        cout << i << vecParams[i].first << "  " << vecParams[i].second << "  " << sumAvgTrackLength[inde][j] << endl;
        h2d_2->Fill(vecParams[i].first, vecParams[i].second, sumAvgTrackLength[inde][j]);
    }
    h2d_2->Draw("colz");

    std::unordered_map<double, double> unOrdMap;
    std::unordered_map<double, int> mapCounts;
    for (unsigned int i = 0; i < vecParams.size(); i++)
    {
        unsigned int inde = int(vecParams[i].first / 50 - 1);
        unsigned int j = int(vecParams[i].second / 50 - 1);
        // cout << i << vecParams[i].first << "  " << vecParams[i].second << "  " << sumAvgTrackLength[inde][j] << endl;
        if (unOrdMap.find(vecParams[i].first * vecParams[i].second) != unOrdMap.end())
        {
            unOrdMap[vecParams[i].first * vecParams[i].second] += sumAvgTrackLength[inde][j];
            mapCounts[vecParams[i].first * vecParams[i].second] += 1;
        }
        else
        {
            unOrdMap.insert(make_pair(vecParams[i].first * vecParams[i].second, sumAvgTrackLength[inde][j]));
            mapCounts.insert(make_pair(vecParams[i].first * vecParams[i].second, 1));
        }
    }

    // TCanvas *c3 = new TCanvas("c3", "c3", 200, 310, 500, 300);
    // c3->cd();
    // TGraph *g1 = new TGraph();
    // g1->SetTitle("Average track length - xAxis: nSteps x nDistance");
    // int fakeCounter = 0;
    // for (auto &itMap : unOrdMap)
    // {

    //     // itMap.second /= mapCounts[itMap.first];
    //     // cout << itMap.first << "  " << itMap.second << endl;
    //     g1->SetPoint(fakeCounter, itMap.first, itMap.second / mapCounts[itMap.first]);
    //     fakeCounter++;
    //     if ((itMap.second / mapCounts[itMap.first]) > 1500)
    //     {
    //         std::cout << itMap.first << "  " << itMap.second << "  " << mapCounts[itMap.first] << endl;
    //         // std::cout << itMap.second / mapCounts[itMap.first] << std::endl;
    //     }
    // }
    // cout << fakeCounter << endl;
    // g1->Draw("AP*");

    TCanvas *c4 = new TCanvas("c4", "Track Length analysis", 700, 310, 500, 300);
    c4->cd();
    TGraph *g2 = new TGraph();
    g2->SetTitle("Average track length - vectorMethod");
    g2->GetXaxis()->SetTitle("Product Angular x Distance steps");
    g2->GetYaxis()->SetTitle("Average track size (increasing trend)");

    int secondCounter = 0;
    for (auto &itMap : vecParams)
    {
        unsigned int ind1 = int(itMap.first / 50 - 1);
        unsigned int ind2 = int(itMap.second / 50 - 1);
        auto itBegin = vecAvgTrackLength[ind1][ind2].begin();
        auto itEnd = vecAvgTrackLength[ind1][ind2].end();
        auto vecSize = vecAvgTrackLength[ind1][ind2].size();

        g2->SetPoint(secondCounter, itMap.first * itMap.second,
                     std::accumulate(itBegin, itEnd, 0.0) / vecSize);
        secondCounter++;
    }
    cout << secondCounter << endl;
    g2->Draw("AP*");

    TCanvas *c5 = new TCanvas("c5", "Canvas Time analysis", 200, 310, 500, 300);
    h2DTime->Draw("colz");

    return 0;
}

// auto end = std::chrono::steady_clock::now();
// cout << "Time FOR ONE RUN = " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << endl;

// std::vector<double> energiesLoops;
// std::vector<double> numPtsLoops;
// cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;

// // for (unsigned int i = 0; i < vecParams.size(); ++i)
// // {
// double tmpEnergy = 0;
// double tmpNumPts = 0;
// std::vector<double> tmpLength;
// std::vector<double> tmpTracksEnergy;

// for (auto &itLines : fitEvt->getLines())
// {
//     double trackEnergy = 0;
//     for (auto &itPts : itLines.getPoints())
//     {
//         trackEnergy += itPts.getEnergy();
//     }
//     tmpEnergy += trackEnergy;
//     tmpNumPts += itLines.getPoints().size();

//     tmpTracksEnergy.push_back(trackEnergy);
//     tmpLength.push_back(itLines.getPoints().size());
//     // }

//     energiesLoops.push_back(tmpEnergy);
//     numPtsLoops.push_back(tmpNumPts);
// }

// for (unsigned int i = 0; i < energiesLoops.size(); ++i)
// {
//     std::cout << energiesLoops[i] << "Energies:  NumPts: " << numPtsLoops[i] << "\n";
// }
// energiesResultsLoops.push_back(energiesLoops);
// numPtsResultsLoops.push_back(numPtsLoops);

// iterCount += 1;
// if (iterCount > vecParams.size())
// {
//     iterCount = 0;
// }

// vector<double> prova;
// for (auto &it : vecParams)
// {
//     prova.push_back(it.first * it.second);
// }
// std::sort(prova.begin(), prova.end());
// int uniqueCount = std::unique(prova.begin(), prova.end()) - prova.begin();

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

int fit(string inputFileName = "inputParametersLoopsAnalysis.txt")
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

    cFittedEvent<cPhysicalHit> *fitEvt = new cFittedEvent<cPhysicalHit>();

    TTreeReader rdr(physicalEventTree);
    TTreeReaderValue<cPhysicalEvent> event(rdr, "event"); // reading input file

    int nent = rdr.GetEntries(false);
    std::cout << nent << std::endl;

    std::vector<std::vector<double>> energiesResultsLoops;
    std::vector<std::vector<double>> numPtsResultsLoops;
    std::vector<std::vector<std::vector<double>>> lengthTracksResultsLoops;
    std::vector<std::vector<std::vector<double>>> energyTracksResultsLoops;

    std::vector<double> vecLoops;

    for (int i = 1; i < numberMaxLoops; i += loopsSteps)
    {
        vecLoops.push_back(i);
    }

    vecLoops.push_back(1000);

    while (rdr.Next())
    {
        std::vector<double> numPtsLoops;
        std::vector<double> energiesLoops;
        std::vector<std::vector<double>> lengthTracksLoops;
        std::vector<std::vector<double>> energyTracksLoops;

        if (rdr.GetCurrentEntry() > 10)
        {
            break;
        }

        std::cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << std::endl;

        for (auto &loops : vecLoops)
        {

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

            // Cluster and FitLiness
            traC.Ransac(threshold, trsize, width, beamTracks, trackOneSide, trackFittable);
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

            double tmpEnergy = 0;
            double tmpNumPts = 0;
            std::vector<double> tmpLength;
            std::vector<double> tmpTracksEnergy;

            for (auto &itLines : traC.lines)
            {
                double trackEnergy = 0;
                for (auto &itPts : itLines.getPoints())
                {
                    trackEnergy += itPts.getEnergy();
                }
                tmpEnergy += trackEnergy;
                tmpNumPts += itLines.getPoints().size();

                tmpTracksEnergy.push_back(trackEnergy);
                tmpLength.push_back(itLines.getPoints().size());
            }
            energiesLoops.push_back(tmpEnergy);
            numPtsLoops.push_back(tmpNumPts);

            std::sort(tmpLength.begin(), tmpLength.end(), std::greater<double>());
            lengthTracksLoops.push_back(tmpLength);
            std::sort(tmpTracksEnergy.begin(), tmpTracksEnergy.end(), std::greater<double>());
            energyTracksLoops.push_back(tmpTracksEnergy);
        }

        // Coding for finding informations on tracks;

        double enEnd = energiesLoops.back();
        for (auto &itEne : energiesLoops)
        {
            itEne /= enEnd;
        }
        double ptsEnd = numPtsLoops.back();
        for (auto &itPts : numPtsLoops)
        {
            itPts /= ptsEnd;
        }

        energiesResultsLoops.push_back(energiesLoops);
        numPtsResultsLoops.push_back(numPtsLoops);
        lengthTracksResultsLoops.push_back(lengthTracksLoops);
        energyTracksResultsLoops.push_back(energyTracksLoops);
    }

    // ANALYSE TRACKS AND VECTORS FOUND
    std::vector<double> energiesGraph;
    std::vector<double> numPtsGraph;
    std::vector<double> mseLenghtsGraph;
    std::vector<double> mseEnergiesGraph;

    vecLoops.pop_back();

    for (unsigned int i = 0; i < vecLoops.size(); ++i)
    {
        double tmpSumEnergy = 0;
        double tmpSumNumPts = 0;

        for (unsigned int j = 0; j < energiesResultsLoops.size() - 1; ++j)
        {
            tmpSumEnergy += energiesResultsLoops[j][i];
            tmpSumNumPts += numPtsResultsLoops[j][i];
        }

        energiesGraph.push_back(tmpSumEnergy);
        numPtsGraph.push_back(tmpSumNumPts);
    }

    for (auto &itEne : energiesGraph)
    {
        itEne /= (energiesResultsLoops.size() - 1);
    }
    for (auto &itPts : numPtsGraph)
    {
        itPts /= (energiesResultsLoops.size() - 1);
    }

    for (auto &itResults : lengthTracksResultsLoops)
    {
        std::vector<double> lengthLastLoop = itResults.back();

        double mseCumul = 0.0;
        int mseCount = 0;

        for (auto &itLoops : itResults)
        {

            for (auto &itSingle : itLoops)
            {
                cout << itSingle << " ";
            }
            std::cout << "\n";

            int totalPts = std::accumulate(lengthLastLoop.begin(), lengthLastLoop.end(), 0.);
            for (unsigned int i = 0; i < itLoops.size() && i < lengthLastLoop.size(); ++i)
            {
                mseCumul += pow((itLoops[i] - lengthLastLoop[i]), 2);
                ++mseCount;
            }
            mseLenghtsGraph.push_back(sqrt(mseCumul / (1.0 * mseCount)) / totalPts);
        }
    }

    for (auto &itResults : energyTracksResultsLoops)
    {
        std::vector<double> energyLastLoop = itResults.back();

        double mseCumul = 0.0;
        int mseCount = 0;

        for (auto &itLoops : itResults)
        {

            for (auto &itSingle : itLoops)
            {
                cout << itSingle << " ";
            }
            std::cout << "\n";

            double totalEnergy = std::accumulate(energyLastLoop.begin(), energyLastLoop.end(), 0);
            for (unsigned int i = 0; i < itLoops.size() && i < energyLastLoop.size(); ++i)
            {
                mseCumul += pow((itLoops[i] - energyLastLoop[i]), 2);
                ++mseCount;
            }
            mseEnergiesGraph.push_back(sqrt(mseCumul / (1.0 * mseCount)) / totalEnergy);
        }
    }

    // DRAW AND PRINT RESULTS
    TCanvas *c1 = new TCanvas("c1_numberLoops", "c1_numberLoops", 200, 10, 500, 300);
    TGraph *gr = new TGraph();
    gr->GetXaxis()->SetTitle("number of Loops");
    gr->GetYaxis()->SetTitle("Total energy reconstructed");

    for (unsigned int i = 0; i < vecLoops.size(); i++)
    {
        gr->SetPoint(i, vecLoops[i], energiesGraph[i]);
    }
    gr->Draw("AP*");

    TCanvas *c2 = new TCanvas("c2_numberLoops", "c2_numberLoops", 200, 10, 500, 300);
    TGraph *gr2 = new TGraph();
    gr2->GetXaxis()->SetTitle("Number of Loops");
    gr2->GetYaxis()->SetTitle("Number of points");

    for (unsigned int i = 0; i < vecLoops.size(); i++)
    {
        gr2->SetPoint(i, vecLoops[i], numPtsGraph[i]);
    }
    gr2->Draw("AP*");

    TCanvas *c3 = new TCanvas("c3_numberLoops", "c3_numberLoops", 200, 10, 500, 300);
    TGraph *gr3 = new TGraph();
    gr3->GetXaxis()->SetTitle("Number of Loops");
    gr3->GetYaxis()->SetTitle("MSE in number of points");

    for (unsigned int i = 0; i < vecLoops.size(); i++)
    {
        cout << vecLoops[i] << "  " << mseLenghtsGraph[i] << "\n";
        gr3->SetPoint(i, vecLoops[i], mseLenghtsGraph[i]);
    }
    gr3->Draw("AP*");

    TCanvas *c4 = new TCanvas("c4_numberLoops", "c4_numberLoops", 200, 10, 500, 300);
    TGraph *gr4 = new TGraph();
    gr4->GetXaxis()->SetTitle("Number of Loops");
    gr4->GetYaxis()->SetTitle("MSE in energy of single trakcsts");

    for (unsigned int i = 0; i < vecLoops.size(); i++)
    {
        cout << vecLoops[i] << "  " << mseEnergiesGraph[i] << "\n";
        gr4->SetPoint(i, vecLoops[i], mseEnergiesGraph[i]);
    }
    gr4->Draw("AP*");
    return 0;
}

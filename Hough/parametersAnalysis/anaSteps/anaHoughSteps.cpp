#include <TROOT.h>
#include <functional>
#include <numeric>
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

int fit(string inputFileName = "input_parameters_hough.txt")
{

    TString dataFileName;
    map<string, double> parMap;

    getInputMap(inputFileName, parMap, dataFileName);

    // double angularSteps = parMap["AngularSteps"];
    // double distanceSteps = parMap["DistanceSteps"];

    double maxAngularSteps = parMap["maxAngularSteps"];
    double maxDistanceSteps = parMap["maxDistanceSteps"];
    double stepsAngularSteps = parMap["stepsAngularSteps"];
    double stepsDistanceSteps = parMap["stepsDistanceSteps"];

    double minimumEnergy = parMap["MinimumEnergy"];
    double pointDistance = parMap["PointDistance"];
    double minPoints = parMap["MinPoints"];

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

    TTree *physicalEventTree = (TTree *)ifile->Get("physicalEventTree");
    if (physicalEventTree->IsZombie())
    {
        printf("Unable to open inputtree:");
        return -1;
    }

    // Open output file
    TString filename = "outTree.root";
    TFile fout(filename.Data(), "RECREATE");
    if (fout.IsZombie())
    {
        cout << "Output file creation failed" << endl;
        return 0;
    }
    fout.cd();

    cFittedEvent<cPhysicalHit> *fitEvt = 0;

    cTrackerFine<cPhysicalHit> *trcC = 0;

    TTree *fOutTree = new TTree("trackTree", "trackTree");
    fOutTree->Branch("Full_event", &fitEvt);
    fOutTree->SetDirectory(&fout);
    fOutTree->AutoSave();

    TTreeReader fReader(physicalEventTree);
    TTreeReaderValue<cPhysicalEvent> event(fReader, "event"); // reading input file

    int it_count = 0;

    auto nent = fReader.GetEntries();

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

    while (fReader.Next())
    {
        std::vector<double> energiesLoops;
        std::vector<double> numPtsLoops;
        cout << "\rConverting entry " << fReader.GetCurrentEntry() << " of " << nent << flush;

        if (fReader.GetCurrentEntry() > 10)
        {
            break;
        }

        for (unsigned int i = 0; i < vecParams.size(); ++i)
        {

            delete trcC;
            trcC = new cTrackerFine<cPhysicalHit>;
            angularSteps = vecParams[i].first;
            distanceSteps = vecParams[i].second;

            //  Delete the previous fitted event and create a new one
            delete fitEvt;
            fitEvt = new cFittedEvent<cPhysicalHit>;
            fitEvt->setRunNumber(event->getEventNumber());
            fitEvt->setTimestamp(event->getTimestamp());

            // Set the tracking variables and clean all the tracker's
            // accumulators
            /***** !!! TRACKING PARAMETERS !!! *****/
            trcC->setAngularSteps(angularSteps);   // 50Ti 44
            trcC->setDistanceSteps(distanceSteps); // 50Ti 44
            trcC->setMinimumEnergy(minimumEnergy); // 50Ti 1000
            trcC->setMaxDistance(maxDistance);     // 50Ti 260
            trcC->setPointDistance(pointDistance);
            trcC->setMinPoints(minPoints);

            trcC->points.clear();
            trcC->accumulator.clear();
            // trcC->lines.clear();
            trcC->fittedLines.clear();

            double rangeLow = 0.;
            double rangeHigh = 700.;

            for (cPhysicalHit &h : event->getHits())
            {
                // Take only the pad hits that are set as trackable
                if (!h.isTrackable() || h.GetEnergy() < 0)
                {
                    fitEvt->getUnfittedPoints().push_back(h);
                    continue;
                }

                trcC->addPoint(h);
            }

            // Start the BEAM tracking on the x,z plane
            trcC->setMinimumEnergy(beamMinEnergy); // 50Ti 1000
            trcC->setPointDistance(beamPointDistance);
            trcC->setMinPoints(beamMinSize);
            trcC->setLinesFittable(false);
            trcC->setMultFactor(multFactorBeam);
            trcC->track(cTrackerFine<cPhysicalHit>::direction::x, cTrackerFine<cPhysicalHit>::direction::y);

            // Start the BEAM tracking on the x,z plane
            trcC->setMinimumEnergy(minimumEnergy); // 50Ti 1000
            trcC->setPointDistance(pointDistance);
            trcC->setMinPoints(minPoints);
            trcC->setLinesFittable(true);
            trcC->setMultFactor(multFactor);
            trcC->track(cTrackerFine<cPhysicalHit>::direction::x, cTrackerFine<cPhysicalHit>::direction::z);

            // Fit the lines
            trcC->fitLines();

            // Save the fitted lines and the unfitted points in the final event
            for (auto &l : trcC->fittedLines)
            {
                // Beam removal has to be placed here
                l.setFittable(true);

                fitEvt->getLines().push_back(l);
            }
            for (auto &p : trcC->points)
            {
                p[2] += rangeLow;
                fitEvt->getUnfittedPoints().push_back(p);
            }

            // Look for vertices
            cVertexFinder<cPhysicalHit> vrt;
            vrt.setMinZ(rangeLow);
            std::vector<double> pStart = {64., 64., 500. / (1. / zRescaling * 2.)};
            vrt.setParStart(pStart);
            vrt.setMaxZ(rangeHigh);
            vrt.setMaxDist(sqrt(55.));
            vrt.findVertex(fitEvt);

            double tmpEnergy = 0;
            double tmpNumPts = 0;
            std::vector<double> tmpLength;
            std::vector<double> tmpTracksEnergy;

            for (auto &itLines : trcC->fittedLines)
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
        }

        energiesResultsLoops.push_back(energiesLoops);
        numPtsResultsLoops.push_back(numPtsLoops);
    }

    // ANALYSE TRACKS AND VECTORS FOUND
    std::vector<double> energiesGraph;
    std::vector<double> numPtsGraph;
    std::vector<double> mseLenghtsGraph;
    std::vector<double> mseEnergiesGraph;

    vecParams.pop_back();

    for (unsigned int i = 0; i < vecParams.size(); ++i)
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
        itEne /= energiesResultsLoops.size();
    }
    for (auto &itPts : numPtsGraph)
    {
        itPts /= numPtsGraph.size();
    }

    // DRAW AND PRINT RESULTS
    TCanvas *c1 = new TCanvas("c1", "c1", 200, 10, 500, 300);
    TH2D *h2D_1 = new TH2D("h2d_1", "h2d_1", 10, 0, 500, 10, 0, 500);
    for (unsigned int i = 0; i < vecParams.size(); i++)
    {
        cout << vecParams[i].first << "  " << vecParams[i].second << "  " << energiesGraph[i] << endl;
        h2D_1->Fill(vecParams[i].first, vecParams[i].second, energiesGraph[i]);
    }
    h2D_1->Draw("COLZ");

    c1->WaitPrimitive();
    c1->Close();
    

    TCanvas *c2 = new TCanvas("c2", "c2", 200, 10, 500, 300);
    c2->cd();
    TH2D *h2d_2 = new TH2D("h2d_2", "h2d_2", 10, 0, 500, 10, 0, 500);
    for (unsigned int i = 0; i < vecParams.size(); i++)
    {
        h2d_2->Fill(vecParams[i].first, vecParams[i].second, numPtsGraph[i]);
    }
    h2d_2->Draw("colz");

    gSystem->ProcessEvents();
    c2->WaitPrimitive();
    return 0;
}

// auto end = std::chrono::steady_clock::now();
// cout << "Time FOR ONE RUN = " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << endl;

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
    TString filename = "durationSeparatedTreeWithParameters.root";
    TFile fout(filename.Data(), "CREATE");
    if (fout.IsZombie())
    {
        cout << "Output file creation failed" << endl;
        return 0;
    }
    fout.cd();

    cFittedEvent<cPhysicalHit> *fitEvt = 0;
    cTrackerFine<cPhysicalHit> *trcC = 0;
    double durationTimeFitting = 0.;
    double durationTimeClustering = 0.;
    double durationTimeTotal = 0.;

    double angularSteps = 0.;
    double distanceSteps = 0.;

    // Output TTree definition
    TTree *fOutTree = new TTree("trackTree", "trackTree");

    // Branch initialization
    fOutTree->Branch("durationTimeFitting", &durationTimeFitting);
    fOutTree->Branch("durationTimeClustering", &durationTimeClustering);
    fOutTree->Branch("durationTimeTotal", &durationTimeTotal);

    fOutTree->Branch("angularSteps", &angularSteps);
    fOutTree->Branch("distanceSteps", &distanceSteps);

    fOutTree->Branch("Full_event", &fitEvt);

    // Directory set
    fOutTree->SetDirectory(&fout);
    fOutTree->AutoSave();

    // Readers definitions
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
            auto end = chrono::steady_clock::now();
            auto start = chrono::steady_clock::now();

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

            end = chrono::steady_clock::now();

            durationTimeClustering = chrono::duration_cast<chrono::microseconds>(end - start).count();

            // Fit the linesf
            trcC->fitLines();

            end = chrono::steady_clock::now();

            durationTimeClustering = chrono::duration_cast<chrono::microseconds>(end - start).count();

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
            vrt.setMaxDist(maxDistance);
            vrt.findVertex(fitEvt);

            end = chrono::steady_clock::now();
            durationTimeTotal = chrono::duration_cast<chrono::microseconds>(end - start).count();

            double totalEn = 0.;
            double countPts = 0.;
            for (auto &itLines : fitEvt->getLines())
            {

                for (auto &itPts : itLines.getPoints())
                {
                    totalEn += itPts.getEnergy();
                    countPts += 1;
                }
            }

            // cout << "Eneergi" << totalEn << "  pts: " << countPts << "  durationTime" << durationTime << "\n";

            fOutTree->Fill();
        }
    }
    fOutTree->Write();
    fout.Write();
    fout.Close();

    return 0;
}

// auto end = std::chrono::steady_clock::now();
// cout << "Time FOR ONE RUN = " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << endl;

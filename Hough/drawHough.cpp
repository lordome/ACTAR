#include <TROOT.h>
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

// Headers needed by this particular selector
//#include "../root_e796/LinkDef.h"
#include "../commonDependencies/cPhysicalHit.h"
#include "../commonDependencies/cPhysicalEvent.h"
#include "../commonDependencies/cFittedEvent.h"
#include "../commonDependencies/cTrackerFine.h"
#include "../commonDependencies/cVertexFinder.h"
#include "../commonDependencies/cDrawEvents.h"
#include "../commonDependencies/cUtils.h"

using namespace std;

int fit(string inputFileName = "input_parameters_hough.txt")
{

    TString dataFileName;
    map<string, double> parMap;

    getInputMap(inputFileName, parMap, dataFileName);

    double angularSteps = parMap["AngularSteps"];
    double distanceSteps = parMap["DistanceSteps"];
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

    cTrackerFine<cPhysicalHit> trcC;

    TTree *fOutTree = new TTree("trackTree", "trackTree");
    fOutTree->Branch("Full_event", &fitEvt);
    fOutTree->SetDirectory(&fout);
    fOutTree->AutoSave();

    TTreeReader fReader(physicalEventTree);
    TTreeReaderValue<cPhysicalEvent> event(fReader, "event"); // reading input file

    int it_count = 0;

    auto nent = fReader.GetEntries();

    while (fReader.Next())
    {
        cout << "\nConverting entry " << fReader.GetCurrentEntry() << " of " << nent << flush;

        gSystem->ProcessEvents();

        if (event->getEventNumber() > skip)
        {

            // auto start = std::chrono::steady_clock::now();
            //  Delete the previous fitted event and create a new one
            delete fitEvt;
            fitEvt = new cFittedEvent<cPhysicalHit>;
            fitEvt->setRunNumber(event->getEventNumber());
            fitEvt->setTimestamp(event->getTimestamp());

            // Set the tracking variables and clean all the tracker's
            // accumulators
            /***** !!! TRACKING PARAMETERS !!! *****/
            trcC.setAngularSteps(angularSteps);   // 50Ti 44
            trcC.setDistanceSteps(distanceSteps); // 50Ti 44
            trcC.setMinimumEnergy(minimumEnergy); // 50Ti 1000
            trcC.setMaxDistance(maxDistance);     // 50Ti 260
            trcC.setPointDistance(pointDistance);
            trcC.setMinPoints(minPoints);

            trcC.points.clear();
            trcC.accumulator.clear();
            // trcC.lines.clear();
            trcC.fittedLines.clear();

            // In this cycle we want to select all the hits in a given
            // range of z position. This is done because the trigger
            // logic makes so that the "good tracks" are in a given
            // interval. To find this simply make an histogram of the z
            // position of the hits, this good region will present a peak
            /***** !!! REGION SELECTION !!! *****/
            double rangeLow = 0.;
            double rangeHigh = 700.;
            for (cPhysicalHit &h : event->getHits())
            {
                // Save the hits from the ancillaries
                // if (h.getGlobalChannelId() >= 20000)
                // {
                //     fitEvt->getAncillaryHit().push_back(h);
                //     continue;
                // }

                // Take only the pad hits that are set as trackable
                if (!h.isTrackable() || h.GetEnergy() < 0)
                {
                    fitEvt->getUnfittedPoints().push_back(h);
                    continue;
                }

                // Remove points in the wrong region
                // if (h.getZ() < rangeLow || h.getZ() > rangeHigh)
                // {
                //     fitEvt->getUnfittedPoints().push_back(h);
                //     continue;
                // }

                // The remaining points are good to be tracked
                // h[2] -= rangeLow;
                // h[2] /= zRescaling;

                trcC.addPoint(h);
            }

            // Start the BEAM tracking on the x,z plane
            trcC.setMinimumEnergy(beamMinEnergy); // 50Ti 1000
            trcC.setPointDistance(beamPointDistance);
            trcC.setMinPoints(beamMinSize);
            trcC.setLinesFittable(false);
            trcC.setMultFactor(multFactorBeam);
            trcC.track(cTrackerFine<cPhysicalHit>::direction::x, cTrackerFine<cPhysicalHit>::direction::y);

            // Start the BEAM tracking on the x,z plane
            trcC.setMinimumEnergy(minimumEnergy); // 50Ti 1000
            trcC.setPointDistance(pointDistance);
            trcC.setMinPoints(minPoints);
            trcC.setLinesFittable(true);
            trcC.setMultFactor(multFactor);
            trcC.track(cTrackerFine<cPhysicalHit>::direction::x, cTrackerFine<cPhysicalHit>::direction::z);

            // Fit the lines
            trcC.fitLines();

            // Save the fitted lines and the unfitted points in the final event
            for (auto &l : trcC.fittedLines)
            {
                for (auto &p : l.getPoints())
                {
                    p[2] += rangeLow;
                }

                // Correct the z traslation in the basepoint
                l.setBasepoint(l.getBasepoint() + TVector3(0., 0., rangeLow));

                // Beam removal has to be placed here
                l.setFittable(true);

                fitEvt->getLines().push_back(l);
            }
            for (auto &p : trcC.points)
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

            // Draw Single Event
            float energy = 0;
            for (auto &it_lines : fitEvt->getLines())
            {
                energy = 0;
                for (auto &it_points : it_lines.getPoints())
                {
                    energy += it_points.getEnergy();
                }
                // cout << "Size: " << it_lines.getPoints().size() << "  Energy " << energy << endl;
            }

            cDrawEvents<cFittedEvent<cPhysicalHit>> *drawEvt =
                new cDrawEvents<cFittedEvent<cPhysicalHit>>(binX, binY, binZ, maxX, maxY, maxZ);

            drawEvt->setEvent(fitEvt);
            drawEvt->drawColors2D(false);
            // drawEvt->drawTracks3DandUnfitted(false);
            drawEvt->drawTracks3D(true);

            delete drawEvt;
        }
        else
        {
            continue;
        }
    }

    // fOutTree->Write();
    // fout.Write();
    // fout.Close();

    return 1;
}

// auto end = std::chrono::steady_clock::now();
// cout << "Time FOR ONE RUN = " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << endl;

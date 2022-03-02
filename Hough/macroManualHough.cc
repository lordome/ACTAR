#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TH1.h>
#include "TProofOutputFile.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"

// Headers needed by this particular selector
//#include "../root_e796/LinkDef.h"
#include "../root_e796/ARDA_extraClasses/cPhysicalHit.h"
#include "../root_e796/ARDA_extraClasses/cPhysicalEvent.h"
#include "cTrackerFine.h"
#include "cFittedEvent.h"
#include "cVertexFinder.h"

using namespace std;

int fit()
{

    TString ifname = "outputPrecalibrator.root";

    // Opening the input file.
    TFile *ifile = new TFile(ifname.Data(), "READ");
    if (ifile->IsZombie())
    {
        printf("Unable to open inputfile: %s", ifname.Data());
        return -1;
    }

    TTree *physicalEventTree = (TTree *)ifile->Get("physicalEventTree");
    if (physicalEventTree->IsZombie())
    {
        printf("Unable to open inputtree: ");
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

    while (fReader.Next())
    {

        // Delete the previous fitted event and create a new one
        delete fitEvt;
        fitEvt = new cFittedEvent<cPhysicalHit>;
        fitEvt->setRunNumber(event->getEventNumber());
        fitEvt->setTimestamp(event->getTimestamp());

        // Set the tracking variables and clean all the tracker's
        // accumulators
        /***** !!! TRACKING PARAMETERS !!! *****/
        trcC.setAngularSteps(200);   // 50Ti 44
        trcC.setDistanceSteps(400.); // 50Ti 44
        trcC.setMinimumEnergy(800);  // 50Ti 1000
        trcC.setMaxDistance(800.);   // 50Ti 260
        trcC.setPointDistance(7.);
        trcC.setMinPoints(10);
        trcC.points.clear();
        trcC.accumulator.clear();
        trcC.lines.clear();
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
            if (h.getGlobalChannelId() >= 20000)
            {
                fitEvt->getAncillaryHit().push_back(h);
                continue;
            }

            // Take only the pad hits that are set as trackable
            if (!h.isTrackable() || h.GetEnergy() < 0)
            {
                fitEvt->getUnfittedPoints().push_back(h);
                continue;
            }

            // Remove points in the wrong region
            if (h.getZ() < rangeLow || h.getZ() > rangeHigh)
            {
                fitEvt->getUnfittedPoints().push_back(h);
                continue;
            }

            // The remaining points are good to be tracked
            h[2] -= rangeLow;
            trcC.addPoint(h);
        }

        // Start the tracking on the x,y plane
        trcC.track(cTrackerFine<cPhysicalHit>::direction::x, cTrackerFine<cPhysicalHit>::direction::y);

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
        vrt.setMaxZ(rangeHigh);
        vrt.setMaxDist(sqrt(55.));
        vrt.findVertex(fitEvt);

        

        cout << "Filling" << endl;
        fOutTree->Fill();

        cout << "Writing" << endl;
        fOutTree->Write();

        break;
    }

    // fOutTree->Write();
    fout.Write();
    fout.Close();

    return 1;
}
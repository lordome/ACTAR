#define trackingSelector_cxx
// The class definition in trackingSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("trackingSelector.C")
// root> T->Process("trackingSelector.C","some options")
// root> T->Process("trackingSelector.C+")
//

#include "trackingSelector.h"
#include <TH2.h>
#include <TStyle.h>
#include <TMath.h>
#include <TVector3.h>
#include <TSystem.h>

trackingSelector::~trackingSelector()
{
    if (fFile)
    {
        SafeDelete(fOutTree);
        SafeDelete(fFile);
    }
}

void trackingSelector::Begin(TTree * /*tree*/)
{
    // The Begin() function is called at the start of the query.
    // When running with PROOF Begin() is only called on the client.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();
}

void trackingSelector::SlaveBegin(TTree * /*tree*/)
{
    // The SlaveBegin() function is called after the Begin() function.
    // When running with PROOF SlaveBegin() is called on each slave server.
    // The tree argument is deprecated (on PROOF 0 is passed).

    TString option = GetOption();

    fProofFile = new TProofOutputFile("outTrack.root", "M");
    fProofFile->SetTitle("outTrack.root"); // File name

    fFile = fProofFile->OpenFile("RECREATE");
    if (fFile && fFile->IsZombie())
        SafeDelete(fFile);

    // Cannot continue
    if (!fFile)
    {
        Info("SlaveBegin", "could not create '%s': instance is invalid!", fProofFile->GetName());
        return;
    }

    fOutTree = new TTree("trackTree", "trackTree");
    fOutTree->Branch("event", &fitEvt);
    fOutTree->SetDirectory(fFile);
    fOutTree->AutoSave();

    // fOutTreeMerge = new TTree("trackTreeMerge", "trackTreeMerge");
    // fOutTreeMerge->Branch("event", &fitEvt);
    // fOutTreeMerge->SetDirectory(fFile);
    // fOutTreeMerge->AutoSave();
}

Bool_t trackingSelector::Process(Long64_t entry)
{
    // The Process() function is called for each entry in the tree (or possibly
    // keyed object in the case of PROOF) to be processed. The entry argument
    // specifies which entry in the currently loaded tree is to be processed.
    // When processing keyed objects with PROOF, the object is already loaded
    // and is available via the fObject pointer.
    //
    // This function should contain the \"body\" of the analysis. It can contain
    // simple or elaborate selection criteria, run algorithms on the data
    // of the event and typically fill histograms.
    //
    // The processing can be stopped by calling Abort().
    //
    // Use fStatus to set the return value of TTree::Process().
    //
    // The return value is currently not used.

    // Load the event to analyze
    fReader.SetEntry(entry);

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

    fOutTree->Fill();

    return kTRUE;
}

void trackingSelector::SlaveTerminate()
{
    // The SlaveTerminate() function is called after all entries or objects
    // have been processed. When running with PROOF SlaveTerminate() is called
    // on each slave server.

    if (fFile)
    {
        Bool_t cleanup = kFALSE;
        TDirectory *savedir = gDirectory;
        if (fOutTree->GetEntries() > 0)
        {
            fFile->cd();
            fOutTree->Write();
            // fOutTreeMerge->Write();
            fProofFile->Print();
            fOutput->Add(fProofFile);
        }
        else
        {
            cleanup = kTRUE;
        }
        fOutTree->SetDirectory(0);
        gDirectory = savedir;
        fFile->Close();
        // Cleanup, if needed
        if (cleanup)
        {
            TUrl uf(*(fFile->GetEndpointUrl()));
            SafeDelete(fFile);
            gSystem->Unlink(uf.GetFile());
            SafeDelete(fProofFile);
        }
    }
}

void trackingSelector::Terminate()
{
    // The Terminate() function is the last function to be called during
    // a query. It always runs on the client, it can be used to present
    // the results graphically or save the results to file.
}

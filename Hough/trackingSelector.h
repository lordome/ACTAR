//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr 26 15:31:11 2019 by ROOT version 6.16/00
// from TTree rawEventTree/rawEventTree
// found on file: /home/luca/bigdata/multiPeakTest/ACTAR_375.root
//////////////////////////////////////////////////////////

#ifndef trackingSelector_h
#define trackingSelector_h

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

// Headers needed by this particular selector
//#include "../root_e796/LinkDef.h"
#include "../root_e796/ARDA_extraClasses/cPhysicalHit.h"
#include "../root_e796/ARDA_extraClasses/cPhysicalEvent.h"
#include "cTrackerFine.h"
#include "cFittedEvent.h"
#include "cVertexFinder.h"

/*
TProof::Open("workers=3")
gSystem->AddIncludePath("/home/luca/uni/houghTrack/")
gSystem->Load("/home/luca/uni/tesi/bin/libARDAlib.so");
*/

class trackingSelector : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   TProofOutputFile *fProofFile = 0;
   TTree *fOutTree = 0;
   TTree *fOutTreeMerge = 0;
   TFile *fFile = 0;
   cFittedEvent<cPhysicalHit> *fitEvt = 0;

   cTrackerFine<cPhysicalHit> trcC;

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<cPhysicalEvent> event = {fReader, "event"};

   trackingSelector(TTree * /*tree*/ =0) { }
   virtual ~trackingSelector();
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(trackingSelector,0);

};

#endif

#ifdef trackingSelector_cxx
void trackingSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t trackingSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef trackingTest_cxx

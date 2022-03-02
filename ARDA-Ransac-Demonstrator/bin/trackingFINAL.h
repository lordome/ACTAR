//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr 26 15:31:11 2019 by ROOT version 6.16/00
// from TTree rawEventTree/rawEventTree
//////////////////////////////////////////////////////////

#ifndef trackingFINAL_h
#define trackingFINAL_h

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
#include "/home/lorenzo/Desktop/ARDAC/bin/LinkDef.h"
#include "/home/lorenzo/Desktop/ARDAC/bin/cPhysicalEvent.h"
#include "/home/lorenzo/Desktop/ARDAC/bin/cPhysicalHit.h"
#include "/home/lorenzo/Desktop/ARDAC/bin/cFittedEvent.h"
#include "TH2F.h"

class trackingFINAL : public TSelector
{
public:
   TTreeReader fReader; //!the tree reader
   TTree *fChain = 0;   //!pointer to the analyzed TTree or TChain

   TProofOutputFile *fProofFile = 0;
   TTree *fOutTree = 0;
   TTree *fOutTreeMerge = 0;
   TFile *fFile = 0;
   cFittedEvent<int> *fitEvt = 0;
   cFittedEvent<cPhysicalHit> *fitEvtPhysical = 0;

   cFittedEvent<int> *tracker = nullptr;
   cFittedEvent<cPhysicalHit> *tracker_withHits = nullptr;

   //TH2F *histodEE = 0; //new TH2F ( ("histo " + filename ).Data() , ("histo " + filename ).Data() , 200, 0, 20000, 500, 0, 500);


   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<cPhysicalEvent> event = {fReader, "event"};

   trackingFINAL(TTree * /*tree*/ = 0) {}
   virtual ~trackingFINAL();
   virtual Int_t Version() const { return 2; }
   virtual void Begin(TTree *tree);
   virtual void SlaveBegin(TTree *tree);
   virtual void Init(TTree *tree);
   virtual Bool_t Notify();
   virtual Bool_t Process(Long64_t entry);
   virtual Int_t GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void SetOption(const char *option) { fOption = option; }
   virtual void SetObject(TObject *obj) { fObject = obj; }
   virtual void SetInputList(TList *input) { fInput = input; }
   virtual TList *GetOutputList() const { return fOutput; }
   virtual void SlaveTerminate();
   virtual void Terminate();

   ClassDef(trackingFINAL, 0);
};

#endif

#ifdef trackingFINAL_cxx
void trackingFINAL::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t trackingFINAL::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef trackingFINAL_cxx

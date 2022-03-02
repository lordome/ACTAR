#include "TProof.h"
#include "TSelector.h"
#include "TChain.h"
#include "TProofChain.h"


TSelector* runProof() {
  TProof *prf = TProof::Open("workers=3");
  prf->AddDynamicPath("./");
  prf->AddIncludePath("./");
  prf->AddIncludePath("/home/lorenzo/Desktop/ARDAC/bin/");

  prf->Load("./libMFM.so");
  prf->Load("/home/lorenzo/Desktop/fromNuclex1/ARDADOWNLOAD/bin/libpClasses.so");
  prf->Load("./libARDAlib.so");
  prf->AddDynamicPath("./");

  prf->Load("./fast_trackingFINAL.cpp++");
  TSelector *sel = TSelector::GetSelector("./fast_trackingFINAL.cpp+");
  TChain *c = new TChain("physicalEventTree");
  c->Add("Raw_ROOTFiles/410Cal.root");
  c->SetProof();
  c->Process(sel);

  return sel;
}

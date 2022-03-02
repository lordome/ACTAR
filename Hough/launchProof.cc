#include "TProof.h"
#include "TSelector.h"
#include "TChain.h"
#include "TProofChain.h"

TSelector *runProof()
{
  TProof *prf = TProof::Open("workers=3");

  gSystem->Load("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06/libMFM.so");
  gSystem->Load("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06/libpClasses.so");
  gSystem->Load("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06/libARDAlib.so");
  prf->AddDynamicPath("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06");


  // prf->AddDynamicPath("/home/lorenzo/Desktop/fromNuclex1/ARDADOWNLOAD/bin");
  // prf->Load("/home/lorenzo/Desktop/fromNuclex1/ARDADOWNLOAD/bin/libMFM.so");
  // //  prf->Load("/home/lorenzo/Desktop/fromNuclex1/ARDADOWNLOAD/bin/libpClasses.so");
  // prf->Load("/home/lorenzo/Desktop/fromNuclex1/ARDADOWNLOAD/bin/libARDAlib.so");
  prf->AddDynamicPath("/home/lorenzo/Desktop/fromNuclex1/dimostratore/proofFINAL/");
  prf->AddIncludePath("/home/lorenzo/Desktop/fromNuclex1/dimostratore/proofFINAL/");
  prf->Load("/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/trackingHough/trackingSelector.cc++");
  TSelector *sel = TSelector::GetSelector("/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/trackingHough/trackingSelector.cc+");
  TChain *c = new TChain("physicalEventTree");
  c->Add("/home/lorenzo/Desktop/fromNuclex1/CALfiles/410Cal.root");
  c->SetProof();
  c->Process(sel);

  return sel;
}

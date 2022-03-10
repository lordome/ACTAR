#include "TProof.h"
#include "TSelector.h"
#include "TChain.h"
#include "TProofChain.h"

TSelector *runProof()
{
  TProof *prf = TProof::Open("workers=3");



  gSystem->Load("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06/libMFM.so+");
  // gSystem->Load("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06/libpClasses.so");
  gSystem->Load("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06/libARDAlib.so");

  prf->AddDynamicPath("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06");

  prf->Load("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06/libMFM.so");
  //prf->AddDynamicPath("/home/lorenzo/Desktop/fromNuclex1/ARDADOWNLOAD/bin");
  // prf->Load("/home/lorenzo/Desktop/fromNuclex1/ARDADOWNLOAD/bin/libpClasses.so");
  prf->Load("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06/libMFM.so");
  prf->Load("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06/libARDAlib.so");
  //  prf->AddDynamicPath("/home/lorenzo/Desktop/fromNuclex1/dimostratore/proofFINAL/");
  //  prf->AddIncludePath("/home/lorenzo/Desktop/fromNuclex1/dimostratore/proofFINAL/");

  prf->AddIncludePath("-I\"/home/lorenzo/Desktop/ACTAR_git/commonDependencies/\"");
  gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/ACTAR_git/commonDependencies/\"");
  prf->Load("/home/lorenzo/Desktop/ACTAR_git/Hough/trackingSelector.cc++");
  TSelector *sel = TSelector::GetSelector("/home/lorenzo/Desktop/ACTAR_git/Hough/trackingSelector.cc+");
  TChain *c = new TChain("physicalEventTree");
  c->Add("/home/lorenzo/Desktop/ROOTFiles/outputPrecalibrator.root");
  c->SetProof();
  c->Process(sel);
  return sel;
}

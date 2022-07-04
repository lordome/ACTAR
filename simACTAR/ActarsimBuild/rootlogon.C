#include <iostream>
#include <TString.h>
#include "TSystem.h"
#include "TROOT.h"
#include "TColor.h"
#include "TStyle.h"

using namespace std;

int rootlogon()
{
  printf("Loading ACTARsim logon script (rootlogon.C)\n");

  gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/inc\"");
  gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/pClasses/inc\"");
  gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/libMFM\"");

  gSystem->Load("/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/bin_root6.24.06/libMFM.so");
  gSystem->Load("/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/bin_root6.24.06/libpClasses.so");
  gSystem->Load("/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/bin_root6.24.06/libARDAlib.so");


  // gSystem->Load("/usr/local/geant4/10.06.03/lib/libG4Tree.so");

  // automatically load scripts
  gSystem->SetFlagsOpt("-O2 -std=c++11");

  gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/ACTAR_git/commonDependencies/\"");
  gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/simACTAR/Actarsim/\"");
  // gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/inc/\"");
  // gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/Ganil_Actar/MFMlib-JDF/install/include/mfmlib/\"");
  // gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/utils/reduction/include/\"");
  // gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/utils/GFFT/src/\"");

  // automatically load scripts
  // printf("-Loading libactar.sl \n");


  // gSystem->Load("/usr/local/geant4/lib/libG4Tree.so");
  // gSystem->Load("libactar.sl");

  //  gROOT->ProcessLine(".L ./plotta_maschere.C");

  // nicer style
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat(1111111);
  gStyle->SetTitleW(0.9);
  gStyle->SetPadGridX(kTRUE);
  gStyle->SetPadGridY(kTRUE);
  TColor *orange = gROOT->GetColor(5);
  orange->SetRGB(1.0, 0.612, 0.002);
  TColor *green = gROOT->GetColor(3);
  green->SetRGB(0.15, 0.7, 0.15);

  const Int_t NRGBs = 5;
  const Int_t NCont = 500;
  Double_t stops[NRGBs] = {0.00, 0.34, 0.61, 0.84, 1.00};
  Double_t r[NRGBs] = {0.00, 0.00, 0.87, 1.00, 0.51};
  Double_t g[NRGBs] = {0.00, 0.81, 1.00, 0.20, 0.00};
  Double_t b[NRGBs] = {0.51, 1.00, 0.12, 0.00, 0.00};
  TColor::CreateGradientColorTable(NRGBs, stops, r, g, b, NCont);
  gStyle->SetNumberContours(NCont);

  printf("-Done. ------ \n");

  return 1;
}

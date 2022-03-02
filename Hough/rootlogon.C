#include <iostream>
#include <TString.h>
#include "TSystem.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TColor.h"

using namespace std;

void rootlogon()
{
  // this is necessary for the codes to work
  gSystem->Load("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06/libMFM.so");
  gSystem->Load("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06/libpClasses.so");
  gSystem->Load("/home/lorenzo/Desktop/ARDAC/bin_root6.24.06/libARDAlib.so");

  gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/ARDAC/bin_root6.24.06/\"");

  // automatically load scripts
  gSystem->SetFlagsOpt("-O2 -std=c++11");

  gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/ACTAR_git/commonDependencies/\"");
  // gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/inc/\"");
  // gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/Ganil_Actar/MFMlib-JDF/install/include/mfmlib/\"");
  // gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/utils/reduction/include/\"");
  // gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/utils/GFFT/src/\"");

  // gROOT->ProcessLine(".L /home/lorenzo/Desktop/Ganil_Actar/MFMlib-JDF/install/lib/libmfm.so");
  // gROOT->ProcessLine(".L /home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/libACTARshared.so");
  // gROOT->ProcessLine(".L MTrack.cpp+g");
  // gROOT->ProcessLine(".L Utils.cpp+g");
  // gROOT->ProcessLine(".L TreeRead.C+g");

  // nicer style
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0000000);
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

  cout << "ARDA ENVIRONMENT LOADED" << endl;
}

#include <iostream>
#include <TString.h>

using namespace std;

void rootlogon()
{
  //this is necessary for the codes to work
  gSystem->Load("./libMFM.so");
  gSystem->Load("./libpClasses.so");
  gSystem->Load("./libARDAlib.so");

  //automatically load scripts

//  gROOT->ProcessLine(".L ./converter.C");
//  gROOT->ProcessLine(".L ./plotta_maschere.C");

  //nicer style
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
  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t r[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t g[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t b[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, r, g, b, NCont);
  gStyle->SetNumberContours(NCont);


  cout<<"ARDA ENVIRONMENT LOADED"<<endl;
}

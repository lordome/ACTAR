#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "cRawEvent.h"
#include "cLookupTable.h"
#include "TH2I.h"
#include "TH1I.h"
#include "TList.h"
#include "TPolyMarker.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TFitResult.h"
#include "TEnv.h"
#include <iostream>
#include <map>
#include <string>
#include <list>
#include <utility>
#include <algorithm>

std::vector<Double_t> histFit(TH1*, Int_t, Double_t, Double_t&);

struct fitResult {
  Double_t m;
  Double_t q;
};

void padAlignment(TString fname, Int_t expectedMaximums = 5, Double_t sigma = 5., Int_t refPad = 18) {
  // Open ROOT file and find the TTree
  TFile* f = new TFile(fname);
  if (!f->IsOpen()) return;

  TTree *rawEventTree = (TTree*) f->Get("rawEventTree");

  // Open a TTreeReader
  TTreeReader rdr(rawEventTree);
  TTreeReaderValue<cRawEvent> event(rdr, "event");

  // Pad histogram before alignment
  TH2* hbefore = new TH2I("beforespec", "Uncorrected spectrum", 2176, -0.5, 2175.5, 4096, -0.5, 4095.5);
  // Pad histogram after alignment
  TH2* hafter  = new TH2I("afterspec", "Corrected spectrum", 2176, -0.5, 2175.5, 4096, -0.5, 4095.5);

  // Chi square histogram of fitted peaks
  TH1 *chiHist = new TH1I("chihist", "Chi square of peak fit", 200, 0, 1000);

  // Map containing pointers to the histogram of each channel
  std::map<Int_t, TH1*> histos;

  // Fill histogram, new channel id created to avoid holes
  while (rdr.Next()) {
    for (auto &hit: event->getHits()) {
      int gid = hit.getGlobalChannelId();

      // Take only pad channels
      if (gid < 20000) {
        double padnum = cLookupTable::getChanFromGlobalChannelId(gid) +
                        cLookupTable::getAgetFromGlobalChannelId(gid) * 68 +
                        cLookupTable::getAsadFromGlobalChannelId(gid) * 68 * 4 +
                        cLookupTable::getCoboFromGlobalChannelId(gid) * 68 * 4 * 4;
        hbefore->Fill(padnum, hit.getPeakHeight());

        // Check if the histogram is in the map, if it is not create it, the fill it
        if (histos.count(gid) == 0) histos[gid] = new TH1I(("p_" + to_string(gid)).c_str(), "", 4096, -0.5, 4095.5);
        histos[gid]->Fill(hit.getPeakHeight());
      }
    }
  }

  // Select the region of interest
  for (auto &p: histos) {
    p.second->GetXaxis()->SetRangeUser(150, 1400);
  }

  // Take reference points from the pad
  Double_t chi = 0.;
  histos[refPad]->ShowPeaks(2, "nodraw", 0.05);
  std::vector<Double_t> refPoints = histFit(histos[refPad], expectedMaximums, sigma, chi);
  chiHist->Fill(chi);

  // Delete all the single channel histograms, they are not needed anymore
  // for (auto p: histos) delete p.second;
  // histos.clear();

  std::map<Int_t, fitResult> calibration;

  for (auto &h: histos) {
    // Skip the reference pad
    if (h.first == refPad) continue;

    // Find peaks in each histogram (default TSpectrum parameters)
    h.second->ShowPeaks(2, "nodraw", 0.05);

    std::vector<Double_t> thisPoints = histFit(h.second, expectedMaximums, sigma, chi);
    chiHist->Fill(chi);

    // Check if the correct maximas are found
    if (thisPoints.size() != expectedMaximums) {
      cout << "WRONG " << h.first << endl;
      continue;
    }

    // Perform linear fit
    TGraph g(expectedMaximums, thisPoints.data(), refPoints.data());
    TF1 lin("myFitFunc", "[0]+[1]*x", 0, 5000);
    g.Fit(&lin, "Q");

    fitResult fres;
    fres.q = lin.GetParameter(0);
    fres.m = lin.GetParameter(1);

    calibration.emplace(h.first, fres);
  }

  // Store the data of each pad. If the angular coeffiecient of the interpolating
  // line is too big (or small) probably there was something wrong with that
  // channel, so I discard it
  TEnv outfile("padcal.cal");
  for (auto &p: calibration) {
    outfile.SetValue((to_string(p.first) + "q").data(), p.second.q);
    outfile.SetValue((to_string(p.first) + "m").data(), p.second.m);
  }
  outfile.SaveLevel(kEnvLocal);

  // Restart the TTreeReader from the beginning, and proceed to apply the correction
  // to the hits in order to check if the correction is performed properly
  rdr.Restart();

  while (rdr.Next()) {
    for (auto &hit: event->getHits()) {
      int gid = hit.getGlobalChannelId();

      // Take only pad channels
      if (gid < 20000) {
        double padnum = cLookupTable::getChanFromGlobalChannelId(gid) +
                        cLookupTable::getAgetFromGlobalChannelId(gid) * 68 +
                        cLookupTable::getAsadFromGlobalChannelId(gid) * 68 * 4 +
                        cLookupTable::getCoboFromGlobalChannelId(gid) * 68 * 4 * 4;
        hafter->Fill(padnum, hit.getPeakHeight()*calibration[gid].m+calibration[gid].q);
      }
    }
  }
}

std::vector<Double_t> histFit(TH1* h, Int_t expectedMaximums, Double_t sigma, Double_t &chiVal) {
  std::vector<Double_t> res;

  std::list<pair<Double_t, Double_t>> l;

  // Save the points found by TSpectrum in a list and order them
  TList *functions = h->GetListOfFunctions();
  TPolyMarker *pm = (TPolyMarker*)functions->FindObject("TPolyMarker");

  if (pm == nullptr) return res;

  // Check if the correct number of maximas are found
  if (pm->GetN() != expectedMaximums) return res;

  for (Int_t i = 0; i < pm->GetN(); i++) l.push_back(make_pair(pm->GetX()[i], pm->GetY()[i]));

  // Prepare a function to fit the peaks
  TString func = "gaus(0)";
  for (Int_t i = 3; i < 3 * expectedMaximums; i+=3) {
    func += "+gaus(";
    func += to_string(i).c_str();
    func += ")";
  }

  // Set the parameters of the TF1
  TF1 f("fitFunc", func.Data(), 0, 4096);
  Int_t par = 0;
  for (auto p: l) {
    f.SetParameter(par, p.second);
    f.SetParameter(par+1, p.first);
    f.SetParLimits(par+1, p.first-sigma, p.first+sigma);
    f.SetParameter(par+2, sigma);

    par += 3;
  }

  // FIT
  TFitResultPtr r = h->Fit(&f, "QS");

  chiVal = f.GetChisquare();

  // Get position of maximas
  for (Int_t i = 0; i < 3 * expectedMaximums; i+=3) {
    res.push_back(f.GetParameter(i+1));
  }
  std::sort(res.begin(), res.end());

  return res;
}

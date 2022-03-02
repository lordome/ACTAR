{
  ofstream caltab("maya_cal_table.dat");
  caltab << "pad	cobo	asad	aget	chan	row	col	a[keV/ch]	b[keV]" << endl;

  TTreeReader rdr(rawEventTree);
  TTreeReaderValue<cRawEvent> event(rdr, "event");


  TH1* curspec;
  new TH1I("h23", "", 4096, -0.5, 4095.5);
  new TH1I("h27", "", 4096, -0.5, 4095.5);
  new TH1I("h31", "", 4096, -0.5, 4095.5);
  new TH1I("h35", "", 4096, -0.5, 4095.5);
  new TH1I("h39", "", 4096, -0.5, 4095.5);
  new TH1I("h43", "", 4096, -0.5, 4095.5);

  while (rdr.Next()) {
    for (auto &hit: event->getHits()) {
      int glob = hit.getGlobalChannelId();
      if (cLookupTable::getCoboFromGlobalChannelId(glob) != 3) continue;
      curspec = (TH1*) gDirectory->Get(Form("h%02d", cLookupTable::getChanFromGlobalChannelId(glob)));
      /*if (hit.getPeakHeight() > 500)*/ curspec->Fill(hit.getPeakHeight());
    }
  }

  int aget = 0;
  for (int chan = 0; chan < 68; chan++) {
    if (!(chan == 23 | chan == 27 | chan == 31 | chan == 35 | chan == 39 | chan == 43)) continue;
    TSpectrum sp;

    curspec = (TH1*) gDirectory->Get(Form("h%02d", chan));

    bool firstIter = true;

    do {
      if(!firstIter) curspec->Rebin(2);
      sp.Search(curspec, 2, "nodraw", 0.3);
      firstIter = false;
    } while(sp.GetNPeaks() > 3);
    cout << Form("h%02d", chan) << "\t" << curspec->GetEntries();

    auto &cdt = lookupTable->getTable()[cLookupTable::getGlobalChannelId(chan, aget, 0, 3)];

    caltab << -1           << "\t"
           <<  3           << "\t"
           <<  0           << "\t"
           << aget         << "\t"
           << chan         << "\t"
           << cdt.getRow() << "\t"
           << cdt.getCol() << "\t";

    if(sp.GetNPeaks() == 3) {
      vector<double> vals = {sp.GetPositionX()[0], sp.GetPositionX()[1], sp.GetPositionX()[2]};
      std::sort(vals.begin(), vals.end());

      TF1 specfit("sf", "gaus(0)+gaus(3)+gaus(6)", vals[0] - 20., vals[2] + 20.);
      specfit.SetParameter(0, curspec->GetBinContent(curspec->GetXaxis()->FindBin(vals[0])));
      specfit.SetParameter(3, curspec->GetBinContent(curspec->GetXaxis()->FindBin(vals[1])));
      specfit.SetParameter(6, curspec->GetBinContent(curspec->GetXaxis()->FindBin(vals[2])));
      specfit.SetParameter(1, vals[0]);
      specfit.SetParameter(4, vals[1]);
      specfit.SetParameter(7, vals[2]);
      specfit.SetParameter(2, 15.);
      specfit.SetParameter(5, 15.);
      specfit.SetParameter(8, 15.);

      curspec->Fit(&specfit, "Q S B R");

      TGraph *g = new TGraph(3);
      TF1 fitfunc("ff", "pol1", 0, 5000);
      fitfunc.SetParameter(0, 0.);
      fitfunc.SetParameter(1, 5.5);
      // fitfunc.SetParLimits(1, 5., 6.);
      g->SetPoint(0, specfit.GetParameter(1), 5156.59);
      g->SetPoint(1, specfit.GetParameter(4), 5485.56);
      g->SetPoint(2, specfit.GetParameter(7), 5804.77);
      auto fitres = g->Fit(&fitfunc, "Q S B");

      cout << "\t" << sp.GetPositionX()[0] << "\t"
                   << sp.GetPositionX()[1] << "\t"
                   << sp.GetPositionX()[2] << "\t"
                   << fitres->Value(0)     << "\t"
                   << fitres->Value(1)     << endl;

      caltab << fitres->Value(1) << "\t"
             << fitres->Value(0) << endl;

      delete g;
    }
    else {
      cout << "\t" << "NOGOOD" << endl;
      caltab << 0 << "\t" << 0 << "\t" << endl;
    }
  }
}

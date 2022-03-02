{
  ofstream caltab("dssd_cal_table.dat");
  caltab << "pad	cobo	asad	aget	chan	row	col	a[keV/ch]	b[keV]" << endl;

  TTreeReader rdr("rawEventTree");
  TTreeReaderValue<cRawEvent> event(rdr, "event");

  int gnum = 0;

  TH1* curspec;
  for (int aget = 0; aget < 4; aget++) {
    for (int chan = 0; chan < 68; chan++) {
      new TH1I(Form("h%02d%02d", aget, chan), "", 4096, -0.5, 4095.5);
      gnum++;
    }
  }

  while (rdr.Next()) {
    for (auto &hit: event->getHits()) {
      int glob = hit.getGlobalChannelId();
      if (cLookupTable::getCoboFromGlobalChannelId(glob) != 2) continue;
      curspec = (TH1*) gDirectory->Get(Form("h%02d%02d", cLookupTable::getAgetFromGlobalChannelId(glob),
                                            cLookupTable::getChanFromGlobalChannelId(glob)));
      if (hit.getPeakHeight() > 500) curspec->Fill(hit.getPeakHeight());
    }
  }

  for (int aget = 0; aget < 4; aget++) {
    for (int chan = 0; chan < 68; chan++) {
      TSpectrum sp;

      curspec = (TH1*) gDirectory->Get(Form("h%02d%02d", aget, chan));

      bool firstIter = true;

      do {
        if(!firstIter) curspec->Rebin(2);
        sp.Search(curspec, 2, "nodraw", 0.3);
        firstIter = false;
      } while(sp.GetNPeaks() > 3);
      cout << Form("h%02d%02d", aget, chan) << "\t" << curspec->GetEntries();

      auto &cdt = lookupTable->getTable()[cLookupTable::getGlobalChannelId(chan, aget, 0, 2)];

      caltab << -1           << "\t"
             <<  2           << "\t"
             <<  0           << "\t"
             << aget         << "\t"
             << chan         << "\t"
             << cdt.getRow() << "\t"
             << cdt.getCol() << "\t";

      if(sp.GetNPeaks() == 3) {
        vector<double> vals = {sp.GetPositionX()[0], sp.GetPositionX()[1], sp.GetPositionX()[2]};
        std::sort(vals.begin(), vals.end());

        TGraph *g = new TGraph(3);
        TF1 fitfunc("ff", "pol1", 0, 5000);
        fitfunc.SetParameter(0, 0.);
        fitfunc.SetParameter(1, 5.5);
        fitfunc.SetParLimits(1, 5., 6.);
        g->SetPoint(0, vals[0], 5156.59);
        g->SetPoint(1, vals[1], 5485.56);
        g->SetPoint(2, vals[2], 5804.77);
        auto fitres = g->Fit(&fitfunc, "Q S B");

        cout << "\t" << sp.GetPositionX()[0] << "\t"
                     << sp.GetPositionX()[1] << "\t"
                     << sp.GetPositionX()[2] << "\t"
                     << fitres->Value(0)     << "\t"
                     << fitres->Value(1)     << endl;

        if (fitres->Value(1) < 5.8) {
          caltab << fitres->Value(1) << "\t"
                << fitres->Value(0) << endl;
        } else {
          caltab << 0. << "\t"
                 << 0. << endl;
        }

        delete g;
      }
      else {
        cout << "\t" << "NOGOOD" << endl;
        caltab << 0 << "\t" << 0 << "\t" << endl;
      }
    }
  }
}

{
  ofstream caltab("pad_cal_table.dat");
  caltab << "pad	cobo	asad	aget	chan	row	col	a[keV/ch]	b[keV]" << endl;

  TTreeReader rdr(rawEventTree);
  TTreeReaderValue<cRawEvent> event(rdr, "event");

  TH2* hbefore = new TH2I("padspec", "", 2176, -0.5, 2175.5, 4096, -0.5, 4095.5);
  TH2* hafter  = new TH2I("padspeccorr", "", 2176, -0.5, 2175.5, 4096, -0.5, 4095.5);
  TH1* peakmult = new TH1I("peakmult", "", 21, -0.5, 20.5);

  TH1* firstpeak = new TH1I("firstpeak", "", 500, 250, 750);
  TH1* secondpeak = new TH1I("secondpeak", "", 500, 750 - 250, 750 + 250);
  TH1* thirdpeak = new TH1I("thirdpeak", "", 500, 1100 - 250, 1100 + 250);
  TH1* fourthpeak = new TH1I("fourthpeak", "", 500, 1600 - 250, 1600 + 250);
  TH1* fifthpeak = new TH1I("fifthpeak", "", 500, 1950 - 350, 1950 + 350);
  TH1* sixthpeak = new TH1I("sixthpeak", "", 500, 3400 - 400, 3400 + 400);

  // pad 00218 chose as reference
  vector<double> calibpoi;

  for (int cobo = 0; cobo < 2; cobo++) {
    for (int asad = 0; asad < 4; asad++) {
      for (int aget = 0; aget < 4; aget++) {
        for (int chan = 0; chan < 68; chan++) {
          new TH1I(Form("pad%d%d%d%02d", cobo, asad, aget, chan), "", 4096, -0.5, 4095.5);
        }
      }
    }
  }

  while (rdr.Next()) {
    for (auto &hit: event->getHits()) {
      int gid = hit.getGlobalChannelId();
      if (gid < 20000) {
        double padnum = cLookupTable::getChanFromGlobalChannelId(gid) +
                        cLookupTable::getAgetFromGlobalChannelId(gid) * 68 +
                        cLookupTable::getAsadFromGlobalChannelId(gid) * 68 * 4 +
                        cLookupTable::getCoboFromGlobalChannelId(gid) * 68 * 4 * 4;
        hbefore->Fill(padnum, hit.getPeakHeight());

        TH1* curspec = (TH1*) gDirectory->Get(Form("pad%05d", gid));
        curspec->Fill(hit.getPeakHeight());
      }
    }
  }

  for (int cobo = 0; cobo < 2; cobo++) {
    for (int asad = 0; asad < 4; asad++) {
      for (int aget = 0; aget < 4; aget++) {
        for (int chan = 0; chan < 68; chan++) {
          TH1* curspec = (TH1*) gDirectory->Get(Form("pad%d%d%d%02d", cobo, asad, aget, chan));
          TSpectrum sp;
          bool firstIter = true;
          do {
            if(!firstIter) curspec->Rebin(2);
            sp.Search(curspec, 5, "nodraw", 0.2);
            firstIter = false;
          } while(sp.GetNPeaks() > 6);
          if (sp.GetNPeaks() == 5) {
            sp.Search(curspec, 5, "nodraw", 0.15);
            do {
              curspec->Rebin(2);
              sp.Search(curspec, 5, "nodraw", 0.15);
              firstIter = false;
            } while(sp.GetNPeaks() > 6);
          }

          if (sp.GetNPeaks() == 6) {
            vector<double> vals;
            for (int i = 0; i < sp.GetNPeaks(); i++) {
              vals.push_back(sp.GetPositionX()[i]);
            }

            std::sort(vals.begin(), vals.end());
            firstpeak->Fill(vals[0]);
            secondpeak->Fill(vals[1]);
            thirdpeak->Fill(vals[2]);
            fourthpeak->Fill(vals[3]);
            fifthpeak->Fill(vals[4]);
            sixthpeak->Fill(vals[5]);
            if (cobo == 0 && asad == 0 && aget == 2 && chan == 18) {
              calibpoi = vals;
            }
          }

          peakmult->Fill(sp.GetNPeaks());
        }
      }
    }
  }

  struct fitData {
    double a;
    double b;
  };

  map<int, fitData> calMap;

  for (int cobo = 0; cobo < 2; cobo++) {
    for (int asad = 0; asad < 4; asad++) {
      for (int aget = 0; aget < 4; aget++) {
        for (int chan = 0; chan < 68; chan++) {
          TH1* curspec = (TH1*) gDirectory->Get(Form("pad%d%d%d%02d", cobo, asad, aget, chan));

          int gid = cLookupTable::getGlobalChannelId(chan, aget, asad, cobo);

          caltab <<  gid << "\t"
                 << cobo << "\t"
                 << asad << "\t"
                 << aget << "\t"
                 << chan << "\t"
                 << lookupTable->getTable()[gid].getRow() << "\t"
                 << lookupTable->getTable()[gid].getCol() << "\t";

          TList *functions = curspec->GetListOfFunctions();
          TPolyMarker *pm = (TPolyMarker*)functions->FindObject("TPolyMarker");
          if (!pm || pm->GetN() != 6) {
            caltab << 1 << "\t" << 0 << endl;
            continue;
          }

          vector<double> vals;
          for (int i = 0; i < pm->GetN(); i++) vals.push_back(pm->GetX()[i]);
          std::sort(vals.begin(), vals.end());

          TGraph g(pm->GetN());
          TF1 fitfunc("ff", "pol1", 0, 6000);
          for (int i = 0; i < pm->GetN(); i++) g.SetPoint(i, vals[i], calibpoi[i]);

          auto fitres = g.Fit(&fitfunc, "Q S B");

          caltab << fitres->Value(1) << "\t"
                 << fitres->Value(0) << endl;

          fitData d;
          d.a = fitres->Value(1);
          d.b = fitres->Value(0);
          calMap[gid] = d;
        }
      }
    }
  }


  rdr.Restart();
  while (rdr.Next()) {
    for (auto &hit: event->getHits()) {
      int gid = hit.getGlobalChannelId();
      if (gid < 20000) {
        double padnum = cLookupTable::getChanFromGlobalChannelId(gid) +
                        cLookupTable::getAgetFromGlobalChannelId(gid) * 68 +
                        cLookupTable::getAsadFromGlobalChannelId(gid) * 68 * 4 +
                        cLookupTable::getCoboFromGlobalChannelId(gid) * 68 * 4 * 4;
        hafter->Fill(padnum, calMap[gid].a * hit.getPeakHeight() + calMap[gid].b);
      }
    }
  }
}

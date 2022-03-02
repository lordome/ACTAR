{
  map<int, double> velmap;
  TH3 *h = new TH3I("posdssd", "", 201, -0.5, 200.5, 201, -70.5, 130.5, 201, -0.5, 200.5);
  DetectorSet d;
  d.SetType("demo");
  for (double v = 8.; v < 50.5; v += 1.) {
    double score = 0.;
    int vint = v;
    TFile f(Form("outvel/Track_320_%d.root", vint));
    TTreeReader rdr("Track1", &f);
    TTreeReaderValue<TrackData> track(rdr, "track");
    TTreeReaderValue<DSSDData>  dssd(rdr, "dssd");

    while (rdr.Next()) {
      for (int i = 0; i < dssd->Size(); i++) {
        TVector3 hitpos = dssd->GetPosition(i);
        // If it is false count at 0
        if (hitpos.Mag() < 1) continue;
        if (vint == 8) h->Fill(hitpos.X(), hitpos.Y(), hitpos.Z());

        double pardist = 0.;
        for (int j = 0; j < track->Size(); j++) {
          double vv = (hitpos - track->GetReachingPoint(j)).Mag();
          if ((pardist < 1e-4 || pardist > vv) && vv < d.SizeOfDSSD().Mag()) pardist = vv;
        }

        score += pardist;
      }
    }

    velmap[vint] = score;
  }

  TGraph g(velmap.size());
  Int_t i = 0;
  for (auto &p: velmap) {
    g.SetPoint(i, p.first, p.second);
    i++;
  }
}

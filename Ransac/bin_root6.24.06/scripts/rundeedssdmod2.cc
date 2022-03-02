{

  TChain *c = new TChain("track");
  c->Add("outallcalib/Track_287.root/Track1");
  c->Add("outallcalib/Track_287.root/Track2");
  c->Add("outallcalib/Track_288.root/Track1");
  c->Add("outallcalib/Track_288.root/Track2");
  c->Add("outallcalib/Track_289.root/Track1");
  c->Add("outallcalib/Track_289.root/Track2");
  c->Add("outallcalib/Track_290.root/Track1");
  c->Add("outallcalib/Track_290.root/Track2");
  c->Add("outallcalib/Track_291.root/Track1");
  c->Add("outallcalib/Track_291.root/Track2");
  c->Add("outallcalib/Track_292.root/Track1");
  c->Add("outallcalib/Track_292.root/Track2");
  c->Add("outallcalib/Track_293.root/Track1");
  c->Add("outallcalib/Track_293.root/Track2");
  c->Add("outallcalib/Track_294.root/Track1");
  c->Add("outallcalib/Track_294.root/Track2");
  c->Add("outallcalib/Track_295.root/Track1");
  c->Add("outallcalib/Track_295.root/Track2");
  c->Add("outallcalib/Track_296.root/Track1");
  c->Add("outallcalib/Track_296.root/Track2");
  c->Add("outallcalib/Track_297.root/Track1");
  c->Add("outallcalib/Track_297.root/Track2");
  // c->Add("outcalibdssd/Track_290.root/Track2");
  // c->Add("outcalibdssd/Track_290.root/Track99");
  // Add others

  TTreeReader rdr(c);
  TTreeReaderValue<DSSDData>  dssd(rdr, "dssd");
  TTreeReaderValue<PadData>   pad(rdr,"pad");
  TTreeReaderValue<TrackData> track(rdr, "track");

  // Width specified in RANSAC
  double width = 5;

  TH2 *h = new TH2I("dee", "DE-E", 2000, 0, 40000, 1000, 0, 5000);
  TH2 *hcorr = new TH2I("deecorr", "DE-E", 2000, 0, 40000, 1000, 0, 5000);
  TH1 *tl = new TH1I("acctl", "Length of accepted tracks", 200, 0, 200);
  TH1 *hpad = new TH1I("trackmult", "", 2048, 0, 2048);

  while (rdr.Next()) {
    for (int i = 0; i < dssd->Size(); i++) {
      TVector3 bmb = dssd->GetPosition(i);
      for (int j = 0; j < track->Size(); j++) {
        TVector3 etr = track->GetReachingPoint(j);
        TVector3 bs  = track->GetPosition(j);
        TVector3 dir = track->GetDirection(j);

        if ((bmb-etr).Mag() < 2.5){
          TVector3 fpoint = pad->GetXYZ(pad->FindFpoint(j+1));
          TVector3 epoint = pad->GetXYZ(pad->FindEpoint(j+1));

          // Calculates the number of pads
          int npads = 0;
          vector<int> acceptedPads;
          for (int k = 0; k < pad->Size(); k++) {
            TVector3 p = pad->GetXYZ(k);
            // Checks if the pad is inside of the track area
            if ((p - epoint) * dir > 0) continue;
            if ((p - fpoint) * dir < 0) continue;
            double dist = dir.Cross(p - bs).Mag() / dir.Mag();
            if (dist < width) {
              acceptedPads.push_back(k);
              npads++;
            }
          }

          double eneCorr = 0.;
          for (auto k: acceptedPads) {
            eneCorr += pad->GetEnergy(k);

            TVector3 p(pad->GetX(k), pad->GetY(k), 0.);
            TVector3 b(bs.X(), bs.Y(), 0.);
            TVector3 d(dir.X(), dir.Y(), 0.);

            TVector3 simpos = 2.*(b+(dir*(p-b))/dir.Mag()/dir.Mag()*dir)-p;
            if (simpos.Y() < 39 && simpos.Y() > 25) eneCorr += pad->GetEnergy(k);
          }

          hpad->Fill(npads);
          hcorr->Fill(dssd->GetEnergy(i), eneCorr/((fpoint-epoint).Mag()));
          h->Fill(dssd->GetEnergy(i), track->GetEnergy(j)/((fpoint-epoint).Mag()));
          tl->Fill(track->GetLength(j));
          break;
        }
      }
    }
  }
}

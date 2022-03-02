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

  TH2 *h = new TH2I("dee", "DE-E", 2000, 0, 40000, 1000, 0, 5000);
  TH1 *tl = new TH1I("acctl", "Length of accepted tracks", 200, 0, 200);

  while (rdr.Next()) {
    for (int i = 0; i < dssd->Size(); i++) {
      TVector3 bmb = dssd->GetPosition(i);
      for (int j = 0; j < track->Size(); j++) {
        TVector3 etr = track->GetReachingPoint(j);

        if ((bmb-etr).Mag() < 2.5){
          TVector3 fpoint = pad->GetXYZ(pad->FindFpoint(j+1));
          TVector3 epoint = pad->GetXYZ(pad->FindEpoint(j+1));
          h->Fill(dssd->GetEnergy(i), track->GetEnergy(j)/((epoint-fpoint).Mag()));
          tl->Fill(track->GetLength(j));
          break;
        }
      }
    }
  }
}

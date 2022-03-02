{
  // Merge the file with contours with the one with the ranges, then load it in the root shell
  // before calling this script
  // hadd comp.root ionloss1001mbar.root deecnt.root
  TChain *c = new TChain("track");
  //c->Add("outallcalib/Track_287.root/Track1");
  c->Add("outallcalib/Track_287.root/Track2");
  //c->Add("outallcalib/Track_288.root/Track1");
  c->Add("outallcalib/Track_288.root/Track2");
  //c->Add("outallcalib/Track_289.root/Track1");
  c->Add("outallcalib/Track_289.root/Track2");
  //c->Add("outallcalib/Track_290.root/Track1");
  c->Add("outallcalib/Track_290.root/Track2");
  //c->Add("outallcalib/Track_291.root/Track1");
  c->Add("outallcalib/Track_291.root/Track2");
  //c->Add("outallcalib/Track_292.root/Track1");
  c->Add("outallcalib/Track_292.root/Track2");
  //c->Add("outallcalib/Track_293.root/Track1");
  c->Add("outallcalib/Track_293.root/Track2");
  //c->Add("outallcalib/Track_294.root/Track1");
  c->Add("outallcalib/Track_294.root/Track2");
  //c->Add("outallcalib/Track_295.root/Track1");
  c->Add("outallcalib/Track_295.root/Track2");
  //c->Add("outallcalib/Track_296.root/Track1");
  c->Add("outallcalib/Track_296.root/Track2");
  //c->Add("outallcalib/Track_297.root/Track1");
  c->Add("outallcalib/Track_297.root/Track2");
  // c->Add("outallcalib/Track_290.root/Track2");
  // c->Add("outallcalib/Track_290.root/Track99");
  // Add others

  TTreeReader rdr(c);
  TTreeReaderValue<DSSDData>   dssd   (rdr, "dssd");
  TTreeReaderValue<PadData>    pad    (rdr, "pad");
  TTreeReaderValue<MayaData>   maya   (rdr, "maya");
  TTreeReaderValue<TrackData>  track  (rdr, "track");
  TTreeReaderValue<VertexData> vertex (rdr, "vertex");

  TH2 *hLi = new TH2I("elhistLi", "", 181, -0.5, 45.5, 2000, 0, 20000);
  TH2 *hLicorr = new TH2I("elhistLicorr", "", 181, -0.5, 45.5, 2000, 0, 20000);
  //TH3 *hLicorr3d = new TH3I("elhistLicorr3d", "", 200, 0, 200, 181, -0.5, 45.5, 2000, 0, 20000);
  TH2 *hHe = new TH2I("elhistHe", "", 181, -0.5, 180.5, 2000, 0, 20000);
  TH2 *hHecorr = new TH2I("elhistHecorr", "", 181, -0.5, 180.5, 2000, 0, 20000);
  TH2 *ag = new TH2I("ag", "", 181, -0.5, 180.5, 181, -0.5, 180.5);
  TH1 *v = new TH1I("vrt", "", 200, 0, 200);

  while (rdr.Next()) {
    double angHe = -1;
    double angLi = -1;
    double angIg = -1;

    for (int i = 0; i < maya->Size(); i++) {
      TVector3 bmb = maya->GetPosition(i);
      for (int j = 0; j < track->Size(); j++) {
        TVector3 etr = track->GetReachingPoint(j);

        if (abs((bmb-etr).X()) < 25 && abs((bmb-etr).Y()) < 25){
          TVector3 fpoint = pad->GetXYZ(pad->FindFpoint(j+1));
          TVector3 epoint = pad->GetXYZ(pad->FindEpoint(j+1));

          double len = (epoint-fpoint).Mag();
          double ang = track->GetDirection(j).Angle(TVector3(1,0,0));
          ang *= 180. / M_PI;

          if (He4deeMaya->IsInside(maya->GetEnergy(i), track->GetEnergy(j)/((epoint-fpoint).Mag()))) {
            angHe = ang;
            hHe->Fill(ang, maya->GetEnergy(i));
            double gaslen = (vertex->GetVertex() - etr).Mag();
            double dist = EvsRangeHe->Eval(maya->GetEnergy(i) * 1e-3) + gaslen;
            double ereco = RangevsEHe->Eval(dist) * 1e3;
            hHecorr->Fill(ang, ereco);
          }

          break;
        } else {
          angIg = track->GetDirection(j).Angle(TVector3(1,0,0));
          angIg *= 180. / M_PI;
        }
      }
    }

    for (int i = 0; i < dssd->Size(); i++) {
      if (angIg > 0 && angHe > 0) break;
      TVector3 bmb = dssd->GetPosition(i);
      for (int j = 0; j < track->Size(); j++) {
        TVector3 etr = track->GetReachingPoint(j);
        double vertpos = vertex->GetVertexX();
        v->Fill(vertpos);

        if ((bmb-etr).Mag() < 2.5){
          TVector3 fpoint = pad->GetXYZ(pad->FindFpoint(j+1));
          TVector3 epoint = pad->GetXYZ(pad->FindEpoint(j+1));
          double len = (epoint-fpoint).Mag();
          double ang = track->GetDirection(j).Angle(TVector3(1,0,0));
          ang *= 180. / M_PI;

          if (He4dee1->IsInside(dssd->GetEnergy(i), track->GetEnergy(j)/len) || He4dee2->IsInside(dssd->GetEnergy(i), track->GetEnergy(j)/len)) {
            angHe = ang;
            hHe->Fill(ang, dssd->GetEnergy(i));
            double gaslen = (vertex->GetVertex() - etr).Mag();
            double dist = EvsRangeHe->Eval(dssd->GetEnergy(i) * 1e-3) + gaslen;
            double ereco = RangevsEHe->Eval(dist) * 1e3;
            hHecorr->Fill(ang, ereco);
          }

          else if (Li6dee->IsInside(dssd->GetEnergy(i), track->GetEnergy(j)/len)) {
            angLi = ang;
            hLi->Fill(ang, dssd->GetEnergy(i));
            double gaslen = (vertex->GetVertex() - etr).Mag();
            double dist = EvsRangeLi->Eval(dssd->GetEnergy(i) * 1e-3) + gaslen;
            double ereco = RangevsELi->Eval(dist) * 1e3;
            //hLicorr3d->Fill(vertpos, ang, ereco);
            hLicorr->Fill(ang, ereco);
          }

          break;
        } else {
          angIg = track->GetDirection(j).Angle(TVector3(1,0,0));
          angIg *= 180. / M_PI;
        }
      }
    }

    if (angHe >= 0 && angLi >= 0) ag->Fill(angLi, angHe);
    else if (track->Size() == 2 && angIg >= 0 && angHe >= 0) ag->Fill(angIg, angHe);
    else if (track->Size() == 2 && angIg >= 0 && angLi >= 0) ag->Fill(angLi, angIg);
  }
}

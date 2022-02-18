TTreeReader rdr(conv);
TTreeReaderValue<DSSDData> dssd(rdr, "dssd");

TH2* dssdplane = new TH2I("dssdplane", "", 139, -37.5, 101.5, 139, 35.5, 174.5);

while (rdr.Next()) {
  for (Int_t i = 0; i < dssd->Size(); i++) {
    if (dssd->GetX(i) > 100.) {
      dssdplane->Fill(dssd->GetY(i), dssd->GetZ(i));
    }
  }
}

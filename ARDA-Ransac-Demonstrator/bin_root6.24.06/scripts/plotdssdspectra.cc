TTreeReader rdr(conv);
TTreeReaderValue<DSSDData> dssd(rdr, "dssd");

TH1* dssdspectra = new TH1I("dssdspectra", "", 5000, 0, 5000);

while (rdr.Next()) {
  for (Int_t i = 0; i < dssd->Size(); i++) {
    if (dssd->GetX(i) > 100.) {
      if (int(dssd->GetY(i)) == -16 && int(dssd->GetZ(i)) == 63) dssdspectra->Fill(dssd->GetEnergy(i));
    }
  }
}

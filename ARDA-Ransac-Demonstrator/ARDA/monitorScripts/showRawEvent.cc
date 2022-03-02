/*
 *    Usage:
 *    .L monitorScripts/showRawEvent.cc
 *
 *    showRawEvent("filename", eventNumber)
 *
 */

void showRawEvent(TString fname, Int_t eventNumber) {
  TH2 *pad = new TH2F(Form("pad_%d", eventNumber), "Pad hit projection", 64, -0.5, 63.5, 32, -0.5, 31.5);

  TFile f(fname);

  TTree *rawEventTree = (TTree*) f.Get("rawEventTree");
  cLookupTable *lookupTable = (cLookupTable*) f.Get("lookupTable");

  TTreeReader rdr(rawEventTree);
  TTreeReaderValue<cRawEvent> event(rdr, "event");

  if (rdr.SetEntry(eventNumber) != TTreeReader::EEntryStatus::kEntryValid) {
    cout << "Error in retrieving the requested event" << endl;
    return;
  }

  for (auto &hit: event->getHits()) {
    cLookupTable::chanData hitInfo = lookupTable->getTable()[hit.getGlobalChannelId()];

    // Selects only hits from the pad
    if (hitInfo.getDet() != 0) continue;

    pad->Fill(hitInfo.getCol(), hitInfo.getRow(), hit.getPeakHeight());
  }

  pad->Draw("colz");
}

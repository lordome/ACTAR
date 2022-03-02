/*
 *    Usage:
 *    .L monitorScripts/showRawEventSig.cc
 *
 *    showRawEventSig("filename", eventNumber)
 *
 */

static cLookupTable* lookupTable  = nullptr;
static cRawEvent*    eventPtr     = nullptr;
static TCanvas*      commonCanvas = nullptr;

void showRawEventSig(TString fname, Int_t eventNumber) {
  TH2 *pad = new TH2F(Form("pad_%d", eventNumber), "Pad hit projection", 64, -0.5, 63.5, 32, -0.5, 31.5);

  TFile *f = new TFile(fname);

  TTree *rawEventTree = (TTree*) f->Get("rawEventTree");
  lookupTable = (cLookupTable*) f->Get("lookupTable");

  TTreeReader rdr(rawEventTree);
  TTreeReaderValue<cRawEvent> event(rdr, "event");

  if (rdr.SetEntry(eventNumber) != TTreeReader::EEntryStatus::kEntryValid) {
    cout << "Error in retrieving the requested event" << endl;
    return;
  }

  eventPtr = event.Get();

  for (auto &hit: event->getHits()) {
    cLookupTable::chanData hitInfo = lookupTable->getTable()[hit.getGlobalChannelId()];

    // Selects only hits from the pad
    if (hitInfo.getDet() != 0) continue;

    pad->Fill(hitInfo.getCol(), hitInfo.getRow(), hit.getPeakHeight());
  }

  pad->Draw("colz");
  gPad->AddExec("myexec", "myexec()");
}

void myexec() {
  // get event information
  int event = gPad->GetEvent();
  int px = gPad->GetEventX();
  int py = gPad->GetEventY();
  // some magic to get the coordinates...
  double xd = gPad->AbsPixeltoX(px);
  double yd = gPad->AbsPixeltoY(py);
  float x = gPad->PadtoX(xd);
  float y = gPad->PadtoY(yd);

  int posx = x;
  int posy = y;

  if (event!=kButton1Down) {
    return;
  }

  auto ib = lookupTable->getTable().begin();
  auto ie = lookupTable->getTable().end();

  int gid = -1;

  while (ib != ie) {
    if (ib->second.getCol() == posx && ib->second.getRow() == posy) {
      gid = ib->first;
    }

    ib++;
  }

  if (gid == -1) return;

  for (auto &sig: eventPtr->getSignal()) {
    if (sig.getGlobalChannelId() == gid) {
      if (!commonCanvas) commonCanvas = new TCanvas;

      commonCanvas->cd();
      pSignal p = sig;
      p.Draw("");
      commonCanvas->Update();
    }
  }
}

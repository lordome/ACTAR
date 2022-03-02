{
  TH1 *h = new TH1I("trig", "", 513, -0.5, 512.5);

  TTreeReader rdr(rawEventTree);
  TTreeReaderValue<cRawEvent> event(rdr, "event");

  while (rdr.Next()) {
    for (auto &hit: event->getHits()) {
      if (hit.getGlobalChannelId() < 20000) h->Fill(hit.getPeakTime());
    }
  }
}

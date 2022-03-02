/*
 *    Usage:
 *    .L monitorScripts/checkSignals.cc
 *
 *    checkSignals("filename", eventNumber)
 *
 */

 void checkSignals(TString fname, Int_t eventNumber) {
   cout << "checkSignals script" << endl;
   cout << "input the globalchannelid to display the corresponding signal" << endl;
   cout << "input ls to view the available signals" << endl << endl;


   TFile f(fname);

   TTree *rawEventTree = (TTree*) f.Get("rawEventTree");
   cLookupTable *lookupTable = (cLookupTable*) f.Get("lookupTable");

   TTreeReader rdr(rawEventTree);
   TTreeReaderValue<cRawEvent> event(rdr, "event");

   if (rdr.SetEntry(eventNumber) != TTreeReader::EEntryStatus::kEntryValid) {
     cout << "Error in retrieving the requested event" << endl;
     return;
   }

   bool exitCond = false;
   while (!exitCond) {
     string cmd;

     cout << "Command: ";
     cin >> cmd;

     if (cmd == "ls") {
       // Print list of signals
       cout << "ID\tCOBO\tASAD\tAGET\tCHAN\tROW\tCOL" << endl;
       for (Int_t i = 0; i < event->getSignal().size(); i++) {
         cRawSignal &sig = event->getSignal()[i];

         Int_t gid = sig.getGlobalChannelId();
         cLookupTable::chanData hitInfo = lookupTable->getTable()[gid];

         cout << i                                             << "\t"
              << cLookupTable::getCoboFromGlobalChannelId(gid) << "\t"
              << cLookupTable::getAsadFromGlobalChannelId(gid) << "\t"
              << cLookupTable::getAgetFromGlobalChannelId(gid) << "\t"
              << cLookupTable::getChanFromGlobalChannelId(gid) << "\t"
              << hitInfo.getRow()                              << "\t"
              << hitInfo.getCol() << endl;
       }
     }
     else if (cmd == "q") {
       // Exit
       exitCond = true;
     }
     else {
       // Draw a signal
       Int_t i = -1;
       try {
         i = stoi(cmd);
       }
       catch (...) {
         continue;
       }

       if (i >= event->getSignal().size()) continue;

       pSignal p = event->getSignal()[i];
       p.Draw("");

       exitCond = true;
     }
   }
 }

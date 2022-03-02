#include <iostream>
#include <fstream>
#include <string>
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "cPhysicalEvent.h"

using namespace std;

int main(int argc, char const *argv[]) {
  if (argc < 3 || argc > 4) {
    cout << "Usage: convert2txt inFile.root outFile.txt [numberOfEvents]" << endl;
  }

  cout << "Opening " << argv[1] << endl;
  TFile fin(argv[1]);

  cout << "Opening " << argv[2] << endl;
  ofstream fout(argv[2]);

  int nevt = 10000;
  if (argc == 4) nevt = stoi(argv[3]);
  cout << "Saving " << nevt << " events" << endl;

  TTree *tree = (TTree*) fin.Get("physicalEventTree");
  if (!tree) {
    cout << "Error while retrieving event TTree" << endl;
    return 0;
  }
  else cout << "TTree successfully retrieved" << endl;

  TTreeReader rdr(tree);
  TTreeReaderValue<cPhysicalEvent> event(rdr, "event");

  while (rdr.Next() && nevt > 0) {
    fout << event->getEventNumber() << endl;

    for (auto &h: event->getHits()) {
      if (h.getGlobalChannelId() >= 20000) continue;

      fout << h[0] << " "
           << h[1] << " "
           << h[2] << " "
           << h.GetEnergy() << endl;
    }

    fout << "@" << endl << endl;
    fout << flush;

    nevt--;
  }

  return 0;
}

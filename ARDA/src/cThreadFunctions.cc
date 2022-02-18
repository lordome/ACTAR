#include "cThreadFunctions.h"
#include "TFile.h"
#include "TObject.h"
#include "TChain.h"
#include "TROOT.h"
#include "cRunController.h"
#include "cLookupTable.h"
#include "cRawEvent.h"
#include "TTreeIndex.h"
#include "cInfoPrinter.h"

#include <map>
#include <string>

using namespace std;

void cThreadFunctions::mergeFiles(cRunFinder* finder, vector<string> fn, Int_t runNum, atomic<bool>* st) {
  cInfoPrinter::updateFileInfo(to_string(runNum), fileStatus::merging);

  // Avoid race conditions while reading
  finder->readControlDirs.lock();
  string tempdir = finder->tempdir;
  string outdir  = finder->outdir;
  finder->readControlDirs.unlock();

  // Creates a TFile where the merged tree will be stored
  TFile *f = new TFile(Form("%s/ACTAR_%d.root", outdir.c_str(), runNum), "recreate");
  f->cd();

  // Output TTree : Information
  TTree *infotree = new TTree("Information","Information");
  infotree->Branch("Run", &runNum, "Run/I");
  infotree->Fill();
  infotree->Write("", TObject::kOverwrite);

  // TTree that will store the events
  TTree *outTree = new TTree("rawEventTree", "rawEventTree");
  cRawEvent *outEvt = new cRawEvent;
  outTree->Branch("event", &outEvt);

  // TChain to access the TTrees in the various files
  TChain *c = new TChain("rawEventTree");

  // Adds the temporary files to the TChain
  for (auto &s: fn) {
    c->Add(Form("%s/%s.root", tempdir.c_str(), s.c_str()));
  }

  // Sets TChain's branch
  cRawEvent *inEvt = new cRawEvent;
  c->SetBranchAddress("event", &inEvt);

  // Build an ordered index for the TChain
  vector<pair<int, Long64_t>> index;
  for (Long64_t i = 0; i < c->GetEntries(); i++) {
    c->GetEntry(i);
    index.push_back(make_pair(inEvt->getEventNumber(), i));
  }

  sort(index.begin(), index.end(), [](const pair<int, Long64_t>& a, const pair<int, Long64_t>& b) {return a.first < b.first;});

  Int_t prevEvtNum =  0;
  Int_t curEvtNum  = -1;

  // Gets the first entry and prepares it for saving
  c->GetEntry(index.begin()->second);
  prevEvtNum = inEvt->getEventNumber();
  *outEvt = *inEvt;

  // Cycles through all the entries
  for (auto indexIt = index.begin(); indexIt != index.end(); indexIt++) {
    c->GetEntry(indexIt->second);

    curEvtNum = inEvt->getEventNumber();

    // If the current event number is equal to the previous one merge the events
    // If not store the event
    if (curEvtNum == prevEvtNum) {
      outEvt->append(*inEvt);
    }
    else {
      outTree->Fill();
      *outEvt = *inEvt;
    }

    prevEvtNum = curEvtNum;
  }

  outTree->Write("", TObject::kOverwrite);

  // Writes the lookup table, thread safe way
  finder->readControlDirs.lock();
  //f->WriteTObject(finder->lt, "lookupTable");
  finder->lt->Write("lookupTable", TObject::kOverwrite);
  finder->readControlDirs.unlock();

  // Closes the file
  f->Close();
  f->Delete();

  delete outEvt;
  delete inEvt;

  // Deletes the temporary files
  finder->readControlDirs.lock();
  finder->deleteTempFilesForRun(runNum);
  finder->readControlDirs.unlock();

  cInfoPrinter::updateFileInfo(to_string(runNum), fileStatus::finishedMerging);

  // Signals that the thread ended
  st->store(true);
}

void cThreadFunctions::analyseFile(cRunFinder *finder, string fnameu, atomic<bool> *st, Int_t nFrameToRead) {
  cInfoPrinter::updateFileInfo(fnameu, fileStatus::processing);

  // Avoid race conditions while reading, creates analysis object
  finder->readControlDirs.lock();
  cRunController *rn = new cRunController(*(finder->lt), fnameu, finder->rundir, finder->tempdir, nFrameToRead);
  finder->readControlDirs.unlock();

  // Runs analysis on the file
  rn->openfile();
  delete rn;

  cInfoPrinter::updateFileInfo(fnameu, fileStatus::finished);

  st->store(true);
}

void cThreadFunctions::prepareSystemForThread() {
  ROOT::EnableThreadSafety();
}

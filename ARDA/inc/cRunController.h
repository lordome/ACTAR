#include <fcntl.h>
#include <string>
#include <map>

#include "TObject.h"
#include "MFMCommonFrame.h"
#include "MFMCoboFrame.h"
#include "MFMMergeFrame.h"

#include "cRawSignal.h"
#include "cLookupTable.h"
#include "cAnalysisController.h"
#include "cHit.h"
#include "cRawEvent.h"

#ifndef cRunController_h
#define cRunController_h

class TTree;
class TFile;

/**
This class reads a file and calls the PSA for each channel by looking the cLookupTable object
*/
class cRunController {
private:
  map<Int_t, cRawSignal> *sigs;

  cLookupTable        *lookup;  /**< Copy of the lookup table*/
  cAnalysisController *PSAs;    /**< PSA factory*/

  TTree *tree;
  TFile *rootfile;
  cRawEvent *revt;
  int curEventNum;
  unsigned long int curTimestamp;
  Int_t run;
  Int_t nFrameToRead;

  std::string fname;
  std::string rundir;
  std::string tempdir;

  void unpack(MFMCommonFrame *frame);
  void unpackMergeFrame(MFMCommonFrame *frame);
  void unpackCoboFrame(MFMCommonFrame* frame);
  void doPSAonSigs();
  void InitROOT();
  void preprocessSignals();

public:
  cRunController(cLookupTable &lt, std::string fnameu, std::string rundiru, std::string tempdiru, Int_t nFrameToReadu);
  virtual ~cRunController();

  void openfile();

  Int_t getRun() const {return run;}
  void setRun(Int_t v) {run = v;}
};

#endif

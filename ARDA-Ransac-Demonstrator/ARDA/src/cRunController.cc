#include "cRunController.h"
#include "cLookupTable.h"
#include "cSignalPreprocessorManager.h"
#include "cSignalPreprocessor.h"

#include <stdlib.h>
#include "TTree.h"
#include "TFile.h"
#include "TThread.h"

using namespace std;

/// Default constructor
cRunController::cRunController(cLookupTable &lt, string fnameu, string rundiru, string tempdiru, Int_t nFrameToReadu) {
  // Sets filename and directory names
  fname        = fnameu;
  rundir       = rundiru;
  tempdir      = tempdiru;
  nFrameToRead = nFrameToReadu;


  // Opens the TFile
  InitROOT();

  // Create the map where various signals are stored
  sigs = new map<Int_t, cRawSignal>;

  // Makes copies of lookup table
  lookup = new cLookupTable(lt);

  // PSA factory
  PSAs = new cAnalysisController();
}

void cRunController::InitROOT() {
  rootfile = new TFile(Form("%s/%s.root", tempdir.c_str(), fname.c_str()), "create");
  rootfile->cd();

  // Initialize empty object
  revt = new cRawEvent;

  tree = new TTree("rawEventTree", "rawEventTree");
  tree->Branch("event", &revt);
}

/// Default destructor
cRunController::~cRunController() {
  tree->Write("", TObject::kOverwrite);

  rootfile->Close();
  rootfile->Delete();

  delete sigs;
  delete lookup;
  delete revt;
}

/**
\brief This function is used to start the conversion of a file
*/
void cRunController::openfile() {
  // Connection between file and file descriptor
  int fLun = open((rundir + "/" + fname).c_str(), O_RDONLY);
  if (fLun <= 0) cout << "Error open file" << endl;

  // Set size of header
  int headersize = MFM_BLOB_HEADER_SIZE;

  // Creates new frame object
  MFMCommonFrame * frame = new MFMCommonFrame();

  // Allocate memory for a frame header
  char *v = (char*) (malloc(headersize));

  // Based on how MVisu works this will unpack one event
  while(nFrameToRead) {
    // Reads data and fills frame
		int framesize = frame->ReadInFile(&fLun, &v, &headersize);
		if (framesize <= 0) break;

    // Sets the current event number to unreal (negative value)
    curEventNum = -1;
    curTimestamp = -1;

		unpack(frame);

    // Preprocess signals
    preprocessSignals();

    // Makes PSA on signals
    doPSAonSigs();

    // Clear the signals after analysis
    sigs->clear();

    // Decrease the number of frames left to read (greater than zero do avoid underflows)
    if (nFrameToRead > 0) --nFrameToRead;
	}

  // Frees used memory
  free(v);
  delete frame;
}

/**
This function takes a frame, check it's type, and calls the correct function to treat it
*/
void cRunController::unpack(MFMCommonFrame *frame) {
  // Gets frame type
  int frametype = frame->GetFrameType();

  // Insert here new hardware frames
  switch (frametype) {
    case MFM_MERGE_EN_FRAME_TYPE:
      unpackMergeFrame(frame);
      break;
    case MFM_MERGE_TS_FRAME_TYPE:
      unpackMergeFrame(frame);
      break;
    case MFM_COBO_FRAME_TYPE:
      unpackCoboFrame(frame);
      break;
    case MFM_COBOF_FRAME_TYPE:
      unpackCoboFrame(frame);
      break;
  }
}

/**
Function to treat merge frames and to start the unpacking of the contained frames
*/
void cRunController::unpackMergeFrame(MFMCommonFrame *frame) {
  // Creates new mergeframe
  MFMMergeFrame *mergeframe = new MFMMergeFrame();

  // Sets header and loads frame
  mergeframe->SetAttributs(frame->GetPointHeader());

  // Number of other frames contained
  int nFrames = mergeframe->GetNbItemsAttribut();

  // Sets read pointer to the begin of the first frame contained
  mergeframe->ResetReadInMem();

  // Pointer that will point to the contained frame
  MFMCommonFrame *cFrame = new MFMCommonFrame;

  // Cycles amongst all the frames contained
  for (int i = 0; i < nFrames; i++) {
    // Reads the frame
    mergeframe->ReadInFrame(cFrame);

    // Starts unpacking the needed frame
    unpack(cFrame);
  }

  // Frees memory
  delete cFrame;
  delete mergeframe;
}

/**
Function to treat the CoBo frames. Will load the files in the map pointed by cRunController::sigs
*/
void cRunController::unpackCoboFrame(MFMCommonFrame *frame) {
  // Creates new cobo frame
  MFMCoboFrame *coboframe = new MFMCoboFrame();

  // Sets header and loads frame
  coboframe->SetAttributs(frame->GetPointHeader());

  if (curEventNum >= 0 && curEventNum != coboframe->GetEventNumber())
  cout << "WARNING EVENT NUMBER VARYING " << curEventNum << " -> " << coboframe->GetEventNumber() << endl;

  // Sets the current event number
  curEventNum = coboframe->GetEventNumber();

  // Get Timestamp
  curTimestamp = coboframe->GetTimeStamp();

  // Get number of items stored in the frame
  int nitems = coboframe->GetNbItems();

  // sampleid is the index of the sample in the samples array
  uint32_t sample, agetid, chanid, sampleid;

  // gets infos from frame
  int coboid = coboframe->CoboGetCoboIdx();
  int asadid = coboframe->CoboGetAsaIdx ();
  int event  = coboframe->GetEventNumber();

  for (int i = 0; i < nitems; i++) {
    // Gets datas from the cobo frame
    coboframe->CoboGetParameters(i, &sample, &sampleid, &chanid, &agetid);

    // Gets the pointer to the cRawSignal corresponding to the right channel
    cRawSignal &curSignal = (*sigs)[cLookupTable::getGlobalChannelId(chanid, agetid, asadid, coboid)];

    // Checks if the corresponding signal has already been created, if not, put the global informations
    if (curSignal.getEvent() < 0) {
      curSignal.setEvent  (event);
      curSignal.setCobo   (coboid);
      curSignal.setAsad   (asadid);
      curSignal.setAget   (agetid);
      curSignal.setChannel(chanid);
    }

    // Load the sample into the array
    curSignal.getSamples()[sampleid] = sample;
  }

  delete coboframe;
}

void cRunController::preprocessSignals() {
  for (string prepName: lookup->getPreprocessorRequested()) {
    for (auto pGen: cSignalPreprocessorManager::getPreprocessorFactories()) {
      if (pGen->getName() == prepName) {
        cSignalPreprocessor *ps = pGen->createPreprocessor();

        ps->preprocess(sigs);

        delete ps;
      }
    }
  }
}

/**
Runs the proper analysis
*/
void cRunController::doPSAonSigs() {
  revt->clear();

  revt->setEventNumber(curEventNum);

  revt->setTimestamp(curTimestamp);

  if (sigs->size() == 0) return;

  // PSA created and needed in this analysis
  map<string, cPSA*> psaDisp;

  for (auto &keyAndSig: *sigs) {
    #ifdef INCRAWSIG
    revt->getSignal().push_back(keyAndSig.second);
    #endif

    // Gets the psa id and creates a psa object
    string psaString = lookup->getPSAIdFromGlobalChannelId(keyAndSig.first);

    // PSA string support parameters in the format psaName(arg1,arg2)
    // We have to read the psaName correctly in order to set the arguments
    bool hasArguments = psaString.find("(") != string::npos;

    string psaId = psaString;
    if (hasArguments) {
      psaId = psaString.substr(0, psaString.find("("));
    }

    // If in the analysis of this event the PSA needed has not yet been created
    // it gets it
    if (psaDisp.count(psaId) == 0) psaDisp[psaId] = PSAs->getPSA(psaId);

    cPSA *psa = psaDisp[psaId];

    // If there is no psa go to the next signal
    if (!psa) continue;

    if (hasArguments) {
      string arglist = psaString.substr(psaString.find("(")+1, psaString.find(")")- psaString.find("(")-1);

      string::size_type pos = -1;
      do {
        pos++;
        string::size_type acc = arglist.find(",", pos);

        psa->addParameter(stoi(arglist.substr(pos, acc-pos)));

        pos = acc;
      } while (pos != string::npos);
    }

    // Run analysis, generates hits
    psa->analyse(sigs, keyAndSig.first);

    // Gets processed hits and add them to the event
    vector<cHit>& sighits = psa->getHits();
    revt->append(sighits);

    // Clears the hits
    psa->clearHits();
  }

  // Save the signals the PSA decided to save and then delete the PSA
  for (auto keyAndPSA: psaDisp) {
    #ifndef INCRAWSIG
    if (!keyAndPSA.second) continue;
    vector<cRawSignal> &sigsav = keyAndPSA.second->getSavedSignals();
    revt->getSignal().insert(revt->getSignal().end(), sigsav.begin(), sigsav.end());
    #endif
    
    delete keyAndPSA.second;
  }

  tree->Fill();
}

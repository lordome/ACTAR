#include <iostream>
#include "cRunFinder.h"
#include "cUserInput.h"
#include "cLookupTable.h"
#include "cAnalysisController.h"
#include "cSignalPreprocessorManager.h"
#include <experimental/filesystem>
#include <climits>
#include <string>
#include <stdexcept>

using namespace std;
namespace fs = std::experimental::filesystem;

int main(int argc, char const *argv[]) {
  cUserInput::setUserInput(argc, argv);

  if (!cUserInput::isSet("nobanner")){
    #include "banner.h"
  }

  if (cUserInput::getArgs("tempdir") != "" &&
      cUserInput::getArgs("rundir")  != "" &&
      cUserInput::getArgs("outdir")  != "") {

    if (!fs::exists(fs::path(cUserInput::getArgs("tempdir")))) {
      cout << "Temporary directory " << cUserInput::getArgs("tempdir") << " not found!" << endl;
      return 0;
    }

    if (!fs::exists(fs::path(cUserInput::getArgs("outdir")))) {
      cout << "Output directory " << cUserInput::getArgs("outdir") << " not found!" << endl;
      return 0;
    }

    if (!fs::exists(fs::path(cUserInput::getArgs("rundir")))) {
      cout << "Run directory " << cUserInput::getArgs("rundir") << " not found!" << endl;
      return 0;
    }

    int nstart;
    int nstop;

    if (cUserInput::getArgs("start") != "" && cUserInput::getArgs("stop") != "") {
      try {
        nstart = stoi(cUserInput::getArgs("start"));
        nstop  = stoi(cUserInput::getArgs("stop"));
      }
      catch (...) {
        cout << "Invalid start stop numbers" << endl;
        return 0;
      }
    } else {
      cout << "No start stop numbers" << endl;
      return 0;
    }

    cAnalysisController pcnt;
    cout << "Found PSAs";
    for (auto n: pcnt.getPSANames()) cout << " " << n;
    cout << endl;

    cout << "Found preprocessors";
    for (auto preproc: cSignalPreprocessorManager::getPreprocessorFactories())
      cout << " "<< preproc->getName();
    cout << endl;

    cout << "RUNNING ANALYSIS" << endl;

    Int_t nFrameToRead;
    try {
      nFrameToRead = stoi(cUserInput::getArgs("limit"));
    } catch (invalid_argument e) {
      nFrameToRead = -1;
    }

    cRunFinder rf(nstart, nstop, cUserInput::getArgs("rundir"), cUserInput::getArgs("tempdir"), cUserInput::getArgs("outdir"), nFrameToRead);

    // Build lookup LookupTable
    cLookupTable *lt;

    if (cUserInput::isSet("gecotable") && cUserInput::isSet("psatable")) {
      lt = new cLookupTable(cUserInput::getArgs("gecotable"), cUserInput::getArgs("psatable"));
    } else {
      lt = new cLookupTable("LookupTable.dat");
    }

    // Checks if preprocessor flag is set
    if (cUserInput::isSet("preproc")) {
      if (lt) lt->loadPreprocessorTable(cUserInput::getArgs("preproc"));
    }

    rf.initializeLookupTable(*lt);

    rf.findRuns();
    rf.threadAnalyse();
  }

  return 0;
}

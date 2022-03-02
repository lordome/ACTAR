#include "cRunFinder.h"
#include <experimental/filesystem>
#include <iostream>
#include <chrono>
#include <algorithm>
#include "cRunController.h"
#include "Parameters.h"
#include "cThreadFunctions.h"
#include "cInfoPrinter.h"

using namespace std;
namespace fs = std::experimental::filesystem;

/**
Constructor
\param Int_t nstartu: run number to start from (included)
\param Int_t nstopu:  run number to stop to (included)
\param string rundiru: directory where to search for runs
\param string tempdiru: directory where to save temporary files
\param string outdiru: directory where to save analysed files
Default is "." directory
*/
cRunFinder::cRunFinder(Int_t nstartu, Int_t nstopu, string rundiru, string tempdiru, string outdiru, Int_t nFrameToReadu) {
  nstart  = nstartu;
  nstop   = nstopu;
  rundir  = rundiru;
  tempdir = tempdiru;
  outdir  = outdiru;
  lt      = nullptr;
  nFrameToRead = nFrameToReadu;
}

/**
Looks for all the filenames in the given directory, returns the number of runs found
*/
Int_t cRunFinder::findRuns() {
  for (auto &f: fs::directory_iterator(rundir)) {
    string fpath = f.path().string();
    string fname = f.path().filename().string();
    // cout << "Found "
    //      << fpath << endl;

    // If file starts with name tries to extract the run number
    if (fname.find("run_") == 0) {
      // Looks for the first non alphanumeric character after run_
      string::size_type numend = fname.find_first_not_of("0123456789", 4);

      // Converts string containing the number to int
      Int_t runnum = stoi(fname.substr(4, numend - 4));

      if (runnum >= nstart && runnum <= nstop) {
        runFiles[runnum].push_back(fname);
        // cout << "Run " << runnum << " filename " << fname << endl;
      }
    }
  }

  // Orders the file names based on their final number run_[runNumber]---------.[fileNumber]
  for (auto &p: runFiles) {
    vector<string> &vec = p.second;

    // This function compares strings by length to find the length of the shortes
    auto minfunc = [](const string& a, const string& b) {
      return a.size() < b.size();
    };

    // Length of the shortest string (the one without .[number])
    int minstrlen = min_element(vec.begin(), vec.end(), minfunc)->size();

    // Sorts the string by looking at the file number
    auto sortfunc = [minstrlen](const string& a, const string& b){
      if (a.size() == minstrlen) return true;
      if (b.size() == minstrlen) return false;

      return stoi(a.substr(minstrlen + 1)) < stoi(b.substr(minstrlen + 1));
    };

    sort(vec.begin(), vec.end(), sortfunc);
  }
}

/**
If the run is in the list analyses it, no multithread
*/
void cRunFinder::analyseRun(Int_t run) {
  if (runFiles.count(run) == 0) return;

  vector<string> &runFileNames = runFiles[run];

  cRunController *rn;
  for (string &frn: runFileNames) {
    rn = new cRunController(*lt, frn, rundir, tempdir, nFrameToRead);
    rn->openfile();
    delete rn;
  }
}

void cRunFinder::initializeLookupTable(string fname) {
  lt = new cLookupTable(fname);
}

void cRunFinder::initializeLookupTable(cLookupTable &ltu) {
  lt = new cLookupTable(ltu);
}

void cRunFinder::initializeLookupTable(string gecoFile, string psaFile) {
  lt = new cLookupTable(gecoFile, psaFile);
}

void cRunFinder::threadAnalyse() {
  // Enables ROOT thread safety
  cThreadFunctions::prepareSystemForThread();

  // Creates allStatus with all the statuses
  for (auto &elem: runFiles) {
    // Creates an array with the number of files
    filesAn[elem.first] = elem.second.size();

    for (string &fn: elem.second) {
      allStatus.emplace_back();
      allStatus.back().runNumber = elem.first;
      allStatus.back().filename  = fn;
      allStatus.back().status    = stateType::unprocessed;
    }
  }

  // Creates the atomic needed to control the threads and save a tuple that
  // contains the id of the file in the allStatus, the atomic and the thread
  // and the pointer to the thread
  atomic<bool> *b;
  for (int i = 0; i < NTHREAD; i++) {
    b = new atomic<bool>(true);
    threadState.push_back(make_tuple(-1, b, nullptr));
  }

  // Sets current run number to lower run number in cRunFinder::runFiles
  Int_t currun  = allStatus.front().runNumber;
  Int_t curelem = 0;

  while (true) {
    // Number of threads that finished running
    Int_t endedt  = 0;

    // Cycle through all threads
    for (auto &state: threadState) {
      // If it finished elaborating
      if (get<1>(state)->load()) {
        // If thread pointer is not nullptr
        if (get<2>(state)) {
          // Join thread and delete it
          get<2>(state)->join();
          delete get<2>(state);

          get<2>(state) = nullptr;

          // Checks if it was analysis run, not a merge
          if (get<0>(state) >= 0) {
            allStatus[get<0>(state)].status = stateType::done;
            int thisElemRunNum = allStatus[get<0>(state)].runNumber;
            filesAn[thisElemRunNum]--;

            if (filesAn[thisElemRunNum] == 0) {
              get<0>(state) = -1;
              get<1>(state)->store(false);
              get<2>(state) = new thread(cThreadFunctions::mergeFiles, this, runFiles[thisElemRunNum], thisElemRunNum, get<1>(state));
              break;
            }
          }
        }

        // If not read all the files analyze another one, else increment
        // the ended thread counter
        if (curelem < allStatus.size()) {
          // Set current element, end state and make the thread
          get<0>(state) = curelem;
          get<1>(state)->store(false);
          get<2>(state) = new thread(cThreadFunctions::analyseFile, this, allStatus[curelem].filename, get<1>(state), nFrameToRead);
          allStatus[curelem].status = stateType::processing;

          currun = allStatus[curelem].runNumber;
          curelem++;
        }
        else {
          endedt++;
        }
      }
    }

    // If all threads ended end the cycle
    if (endedt == NTHREAD) break;

    // Wait 10 ms, to avoid asking continuosly the state
    this_thread::sleep_for(chrono::milliseconds(1000));

    // Print status
    cInfoPrinter::printStatus();
  }

  // Free memory used by atomic
  for (auto &state: threadState) {
    delete get<1>(state);
  }
}

void cRunFinder::deleteTempFilesForRun(Int_t run) {
  vector<string> &filesToDelete = runFiles[run];

  // For every file in the corresponding run, generates a path object and then
  // delete the corresponding file
  for (string &fn: filesToDelete) {
    fs::path pathToFile = tempdir + "/" + fn + ".root";

    fs::remove(pathToFile);
  }
}

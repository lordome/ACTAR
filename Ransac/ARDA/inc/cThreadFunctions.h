#ifndef threadFunctions_h
#define threadFunctions_h

#include <vector>
#include <string>
#include <atomic>
#include "TObject.h"
#include "cRunFinder.h"

/**
\brief Contains functions that must have static linkage in order to be executed
by threads.
They are responsible for calling the analysis objects, gathering
the informations needed, and signal, through an std::atomi<bool> when they
completed their task
*/
class cThreadFunctions {
  public:
    /// Merges the temporary files of a run and merges separated events
    static void mergeFiles (cRunFinder*, std::vector<std::string>, Int_t, std::atomic<bool>*);

    /// Analyses a file and store the result in a ROOT file to be merged afterwards
    static void analyseFile(cRunFinder*, std::string, std::atomic<bool>*, Int_t nFrameToRead);

    /// Sets ROOT::EnableThreadSafety()
    static void prepareSystemForThread();
};

#endif

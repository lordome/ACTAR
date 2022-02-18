#include "TObject.h"
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <tuple>
#include "cLookupTable.h"
#include "cThreadFunctions.h"

#ifndef cRunFinder_h
#define cRunFinder_h

/**
\brief This class finds the runs and organizes the files
It searches the directory given by the user for files with names starting with

run_[runnumber]*****

with runnumber in the range of cRunFinder::nstart and cRunFinder::nstop
*/

class cRunFinder {
  friend class cThreadFunctions;

  private:
    Int_t nstart;        /**<Start run numer */
    Int_t nstop;         /**<Stop run number*/
    Int_t nFrameToRead;  /**<Number of frames to read*/
    std::string rundir;  /**<Run files' directory */
    std::string tempdir; /**<Temporary files directory*/
    std::string outdir;  /**<Output files directory*/
    std::mutex readControlDirs;

    cLookupTable *lt;

    enum class stateType {
      unprocessed,
      processing,
      done,
    };

    class packedJobStatus {
      public:
        Int_t runNumber;
        std::string filename;
        stateType status;
    };

    std::vector<std::tuple<Int_t, std::atomic<bool>*, std::thread*>> threadState;
    std::vector<packedJobStatus> allStatus;
    std::map<Int_t, Int_t> filesAn;

    /**
    This map contains a collection of vectors, ordered by run number, each containing
    a vector with all the file names of files associated with that run
    */
    std::map<Int_t, std::vector<std::string>> runFiles;

    /// Deletes all the temporary files of a run
    void deleteTempFilesForRun(Int_t);

  public:
    cRunFinder(Int_t nstartu, Int_t nstopu, std::string rundiru = ".", std::string tempdiru = ".", std::string outdiru = ".", Int_t nFrameToReadu = -1);
    virtual ~cRunFinder() {}

    /// Creates an analysis object and runs it for all the files in a run
    void analyseRun(Int_t run);

    /// Sets the lookup table that will be passed to the analysis objects
    void initializeLookupTable(std::string fname = "LookupTable.dat");
    void initializeLookupTable(std::string, std::string);
    /// Sets the lookup table that will be passed to the analysis objects
    void initializeLookupTable(cLookupTable &ltu);

    void threadAnalyse();

    Int_t findRuns();
};

#endif

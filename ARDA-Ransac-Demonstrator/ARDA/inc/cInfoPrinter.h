#ifndef cInfoPrinter_h
#define cInfoPrinter_h

#include <map>
#include <string>
#include <mutex>

class cInfoPrinter {
  public:
    static cInfoPrinter& getInfoPrinter();

    enum class fileStatus {
      unknown,
      processing,
      broken,
      finished,
      merging,
      finishedMerging
    };

    static void updateFileInfo(std::string, fileStatus);
    static void printStatus();

  private:
    cInfoPrinter() : firstPrint(true) {}
    cInfoPrinter  (cInfoPrinter const&) = delete;
    void operator=(cInfoPrinter const&) = delete;

    std::mutex fileListMutex;
    std::map<std::string, fileStatus> fileList;
    bool firstPrint;
};

typedef cInfoPrinter::fileStatus fileStatus;

#endif

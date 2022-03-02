#include "cInfoPrinter.h"
#include "Parameters.h"
#include <iostream>

using namespace std;

cInfoPrinter& cInfoPrinter::getInfoPrinter() {
  static cInfoPrinter info;
  return info;
}

void cInfoPrinter::updateFileInfo(string fName, fileStatus s) {
  cInfoPrinter &ip = cInfoPrinter::getInfoPrinter();

  ip.fileListMutex.lock();
  ip.fileList[fName] = s;
  ip.fileListMutex.unlock();
}

void cInfoPrinter::printStatus() {
  cInfoPrinter &ip = cInfoPrinter::getInfoPrinter();

  ip.firstPrint = false;

  ip.fileListMutex.lock();

  for (auto &f: ip.fileList) {
    bool deleteFlag = false;

    switch (f.second) {
      case fileStatus::finished:
          cout << "Finished analysing file " << f.first << "\n";
          deleteFlag = true;
          break;
      case fileStatus::finishedMerging:
          cout << "Finished merging run " << f.first << "\n";
          deleteFlag = true;
          break;
      case fileStatus::broken:
          cout << "BROKEN FILE " << f.first << "\n";
          deleteFlag = true;
          break;
      case fileStatus::processing:
          cout << "Processing file " << f.first << "\n";
          deleteFlag = true;
          break;
      case fileStatus::merging:
          cout << "Merging run " << f.first << "\n";
          deleteFlag = true;
          break;
    }

    if (deleteFlag) ip.fileList.erase(f.first);
  }

  ip.fileListMutex.unlock();

  cout << flush;
}

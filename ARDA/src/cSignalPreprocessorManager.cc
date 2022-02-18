#include "cSignalPreprocessorManager.h"

using namespace std;

void cSignalPreprocessorManager::registerPreprocessor(abstractFactory* v) {
  getPreprocList().push_back(v);
}

list<cSignalPreprocessorManager::abstractFactory*> cSignalPreprocessorManager::getPreprocessorFactories() {
  return getPreprocList();
}

list<cSignalPreprocessorManager::abstractFactory*>& cSignalPreprocessorManager::getPreprocList() {
  static list<abstractFactory*> preprocList;

  return preprocList;
}

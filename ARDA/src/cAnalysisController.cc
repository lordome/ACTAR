#include "cAnalysisController.h"

#include <map>
#include <string>

using namespace std;

typedef cAnalysisController::abstractFactory abstractFactory;

cAnalysisController::cAnalysisController() {
  // Copying the map
  properPSAList = getPSAlist();
}

cAnalysisController::~cAnalysisController() {}

/**
\brief Returns a reference to the static list of PSAs factories
*/
map<string, abstractFactory*>& cAnalysisController::getPSAlist() {
  static map<string, abstractFactory*> listPSAs;
  return listPSAs;
}

/**
\brief Registers the PSA class factory to the list of possible PSAs
\param abstractFactory *psaFac: a prototype of the PSA
\param string name: the name to uniquely identify the PSA
*/
void cAnalysisController::registerPSA(string name, abstractFactory *psaFac) {
  static map<string, abstractFactory*> &listPSAs = getPSAlist();

  // Checks if an object with the given identifier already exists
  if (listPSAs.count(name) > 0) return;

  // Adds the PSA to the list
  listPSAs[name] = psaFac;
}

/**
\brief Return an istance of the PSA object corresponding to the given name
*/
cPSA* cAnalysisController::getPSA(string name) {
  /// If the psa name is "none" do not analyse the event (to use with FPN)
  if (name == "none") return nullptr;
  if (properPSAList.count(name) == 0) return nullptr;

  abstractFactory* absFac = properPSAList[name];

  return absFac->createPSA();
}

list<string> cAnalysisController::getPSANames() const {
  list<string> names;

  for (auto &p: properPSAList) {
    names.push_back(p.first);
  }

  return names;
}

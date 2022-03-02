#include "cHit.h"
#include "cRawSignal.h"
#include <vector>
#include <map>

#ifndef cPSA_h
#define cPSA_h

/**
\brief This class is the prototype for the general pulse shape analysis
Every acquired channel (not necessarily in order) is passed to the analyse function,
then getHits and clearHits are called
*/

class cPSA {
  protected:
    std::vector<cHit> hits;
    std::vector<cRawSignal> savedSignals;

  public:
    cPSA() {}
    virtual ~cPSA() {}

    /**
    \brief Analyse the cRawSignal corresponding to globalchannelid
    */
    virtual void analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid) = 0;

    /**
    \brief Returns a vector of cHit containing all the hits found in the signal
    */
    virtual std::vector<cHit>& getHits() {return hits;}
    virtual std::vector<cRawSignal>& getSavedSignals() {return savedSignals;}

    /**
    \brief Called after the hits have been saved
    */
    virtual void clearHits() {hits.clear();}
    virtual void clearSavedSignals() {savedSignals.clear();}

    virtual void addParameter(Double_t) {}
};

#endif

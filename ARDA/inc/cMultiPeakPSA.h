#include "cPSA.h"
#include "Parameters.h"

#ifndef cMultiPeakPSA_h
#define cMultiPeakPSA_h

/**
\brief PSA capable of separating various peaks
*/
class cMultiPeakPSA : public cPSA {
  public:
    cMultiPeakPSA() {}          ///< Constructor
    virtual ~cMultiPeakPSA() {} ///< Destructor

    void analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid);

  private:
    void calculateDerivative();
    void findMaximums();

    Float_t derivative[NSAMPLES];
    Float_t *signal;
    Int_t curGlobalChannelId;
};

#endif

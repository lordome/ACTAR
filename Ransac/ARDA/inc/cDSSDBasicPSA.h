#include "cPSA.h"

#ifndef cDSSDBasicPSA_h
#define cDSSDBasicPSA_h

/**
\brief Simplest possible PSA
*/
class cDSSDBasicPSA : public cPSA {
  public:
    cDSSDBasicPSA() {}          ///< Constructor
    virtual ~cDSSDBasicPSA() {} ///< Destructor

    void analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid);
};

#endif

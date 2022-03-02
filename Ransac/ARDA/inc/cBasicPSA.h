#include "cPSA.h"

#ifndef cBasicPSA_h
#define cBasicPSA_h

/**
\brief Simplest possible PSA
*/
class cBasicPSA : public cPSA {
  public:
    cBasicPSA() {}          ///< Constructor
    virtual ~cBasicPSA() {} ///< Destructor

    void analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid);
};

#endif

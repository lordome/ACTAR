#include "cPSA.h"

#ifndef cMayaBasicPSA_h
#define cMayaBasicPSA_h

/**
\brief Simplest possible PSA
*/
class cMayaBasicPSA : public cPSA {
  public:
    cMayaBasicPSA() {}          ///< Constructor
    virtual ~cMayaBasicPSA() {} ///< Destructor

    void analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid);
};

#endif

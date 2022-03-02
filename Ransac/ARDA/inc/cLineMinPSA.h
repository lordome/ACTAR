#include "cPSA.h"
#include "TF1.h"
#include "TObject.h"

#ifndef cLineMinPSA_h
#define cLineMinPSA_h

// Name of the PSA class
#define psaClassName cLineMinPSA

// Id of the PSA (the one used by the lookup table)
#define psaIdName "lineMin"

class cLineMinPSA : public cPSA {
  public:
    cLineMinPSA() {}          ///< Constructor
    virtual ~cLineMinPSA() {} ///< Destructor

    void analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid);
};

#endif

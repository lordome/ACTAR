#include "cPSA.h"
#include "TF1.h"
#include "TObject.h"

#ifndef cLinePSA_h
#define cLinePSA_h

// Name of the PSA class
#define psaClassName cLinePSA

// Id of the PSA (the one used by the lookup table)
#define psaIdName "line"

class cLinePSA : public cPSA {
  public:
    cLinePSA() {}          ///< Constructor
    virtual ~cLinePSA() {} ///< Destructor
    
    void analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid);
};

#endif

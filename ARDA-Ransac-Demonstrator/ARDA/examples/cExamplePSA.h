#include "cPSA.h"
#include "cRawSignal.h"
#include <iostream>

#pragma once

// Name of the PSA class
#define psaClassName cExamplePSA

// Id of the PSA (the one used by the lookup table)
#define psaIdName "examplePSA"

class psaClassName : public cPSA {
  public:
    psaClassName() {}
    virtual ~psaClassName() {}

    void analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid);
};

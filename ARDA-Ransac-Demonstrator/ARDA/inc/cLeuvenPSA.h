#include "cPSA.h"

#ifndef cLeuvenPSA_h
#define cLeuvenPSA_h

/**
\brief Simplest possible PSA
*/
class cLeuvenPSA : public cPSA {
  public:
    cLeuvenPSA();        ///< Constructor
    virtual ~cLeuvenPSA(); ///< Destructor

    void analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid);

    void addParameter(Double_t);

  private:
    int parameters[3];
    int curpar;

    short int* convSig;
    short int* baseline;
};

#endif

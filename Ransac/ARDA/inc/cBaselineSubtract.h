#ifndef cBaselineSubctract_h
#define cBaselineSubctract_h

#include "cSignalPreprocessor.h"
#include "cRawSignal.h"
#include "Parameters.h"
#include <map>

class cBaselineSubctract : public cSignalPreprocessor {
  public:
    void preprocess(std::map<Int_t, cRawSignal> *);
};

#endif

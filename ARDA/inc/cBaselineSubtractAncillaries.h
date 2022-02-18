#ifndef cBaselineSubctractAncillaries_h
#define cBaselineSubctractAncillaries_h

#include "cSignalPreprocessor.h"
#include "cRawSignal.h"
#include "Parameters.h"
#include <map>

class cBaselineSubctractAncillaries : public cSignalPreprocessor {
  public:
    void preprocess(std::map<Int_t, cRawSignal> *);
};

#endif

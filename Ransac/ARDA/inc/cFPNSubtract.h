#ifndef cFPNSubtract_h
#define cFPNSubtract_h

#include "cSignalPreprocessor.h"
#include "cRawSignal.h"
#include "Parameters.h"
#include <map>

class cFPNSubtract : public cSignalPreprocessor {
  public:
    void preprocess(std::map<Int_t, cRawSignal> *);

  private:
    void computeAverage(std::map<Int_t, cRawSignal>*, Int_t, Int_t, Int_t);

    Float_t calcAvg[NSAMPLES];
};

#endif

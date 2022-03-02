#ifndef cSignalPreprocessor_h
#define cSignalPreprocessor_h

#include <map>
#include "cRawSignal.h"

/**
Base class for the operations to be applied to the signals before PSA
*/

class cSignalPreprocessor {
  public:
    cSignalPreprocessor() {}          /**< Default constructor*/
    virtual ~cSignalPreprocessor() {} /**< Default destructor*/

    virtual void preprocess(std::map<Int_t, cRawSignal> *) = 0;

};

#endif

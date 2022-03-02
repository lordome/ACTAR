#include "cFPNSubtract.h"
#include "cLookupTable.h"
#include "cSignalPreprocessorManager.h"

class cFPNSubtractFactory : public cSignalPreprocessorManager::abstractFactory {
  public:
    cFPNSubtractFactory() : abstractFactory("FPNsubtract") {}
    ~cFPNSubtractFactory() {}

    cSignalPreprocessor* createPreprocessor() {return new cFPNSubtract;}
};

static cFPNSubtractFactory FPNfac;

void cFPNSubtract::preprocess(std::map<Int_t, cRawSignal> *sigs) {
  Int_t curCoBo = -1;
  Int_t curAsAd = -1;
  Int_t curAGET = -1;

  // Look for non FPN channels in order to compute the average only
  // if signals for that AGET have been stored
  for (auto &s: *sigs) {
    cRawSignal &sig = s.second;

    // Check if the signal is not an FPN
    if (sig.getChannel() != 11 &&
        sig.getChannel() != 22 &&
        sig.getChannel() != 45 &&
        sig.getChannel() != 56)  {

      // Check if this is a new AGET, if it is, compute average
      if (sig.getCobo() > curCoBo ||
          sig.getAsad() > curAsAd ||
          sig.getAget() > curAGET) {

        curCoBo = sig.getCobo();
        curAsAd = sig.getAsad();
        curAGET = sig.getAget();

        computeAverage(sigs, curCoBo, curAsAd, curAGET);
      }

      // Subtract average from the signal
      for (Int_t i = 0; i < NSAMPLES; i++) sig.getSamples()[i] -= calcAvg[i];
    }
  }
}

void cFPNSubtract::computeAverage(std::map<Int_t, cRawSignal>* sigs, Int_t cobo, Int_t asad, Int_t aget) {
  // Number of FPN found
  Float_t FPNfound = 0.;

  // Global channel id without the channel number for this AGET
  Int_t globChanId = cLookupTable::getGlobalChannelId(0, aget, asad, cobo);

  // Zero out the array that will contain the average
  for (Int_t i = 0; i < NSAMPLES; i++) calcAvg[i] = 0.;

  // If the FPNs are in the signals compute their average
  if (sigs->find(globChanId + 11) != sigs->end()) {
    Float_t *curFPNsig = sigs->find(globChanId + 11)->second.getSamples();
    for (Int_t i = 0; i < NSAMPLES; i++) calcAvg[i] += curFPNsig[i];
    FPNfound++;
  }
  if (sigs->find(globChanId + 22) != sigs->end()) {
    Float_t *curFPNsig = sigs->find(globChanId + 22)->second.getSamples();
    for (Int_t i = 0; i < NSAMPLES; i++) calcAvg[i] += curFPNsig[i];
    FPNfound++;
  }
  if (sigs->find(globChanId + 45) != sigs->end()) {
    Float_t *curFPNsig = sigs->find(globChanId + 45)->second.getSamples();
    for (Int_t i = 0; i < NSAMPLES; i++) calcAvg[i] += curFPNsig[i];
    FPNfound++;
  }
  if (sigs->find(globChanId + 56) != sigs->end()) {
    Float_t *curFPNsig = sigs->find(globChanId + 56)->second.getSamples();
    for (Int_t i = 0; i < NSAMPLES; i++) calcAvg[i] += curFPNsig[i];
    FPNfound++;
  }

  for (Int_t i = 0; i < NSAMPLES; i++) calcAvg[i] /= FPNfound;
}

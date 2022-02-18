#include "cBaselineSubtractAncillaries.h"
#include "cLookupTable.h"
#include "cSignalPreprocessorManager.h"
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>

class cBaselineSubctractAncillariesFactory : public cSignalPreprocessorManager::abstractFactory {
  public:
    cBaselineSubctractAncillariesFactory() : abstractFactory("baselineSubtractAncillaries") {}
    ~cBaselineSubctractAncillariesFactory() {}

    cSignalPreprocessor* createPreprocessor() {return new cBaselineSubctractAncillaries;}
};

static cBaselineSubctractAncillariesFactory FPNfac;

// Samples before the minimum to exclude
#define NDW 100

void cBaselineSubctractAncillaries::preprocess(std::map<Int_t, cRawSignal> *sigs) {
  // Main loop
  for (auto &signal: *sigs) {
    // Check if it is a pad plane signal, if it is not, discard it
    if (cLookupTable::getCoboFromGlobalChannelId(signal.first) < 2) continue;

    // Discard FPN signals
    Int_t channel = cLookupTable::getChanFromGlobalChannelId(signal.first);
    if (channel == 11 || channel == 22 || channel == 45 || channel == 56) continue;

    // Get the signal
    Float_t *s = signal.second.getSamples();

    // Find minimum and maximum value of the signal to make an histogram of the
    // correct size
    Float_t minVal = s[0];
    Float_t maxVal = s[0];

    for (Int_t i = 0; i < NSAMPLES; i++) {
      if (s[i] > maxVal) maxVal = s[i];
      if (s[i] < minVal) minVal = s[i];
    }

    // Calculate histogram lower, upper bins and bin number
    Int_t lowbin  = minVal;
    Int_t highbin = maxVal + 1.;
    Int_t nbins   =  highbin - lowbin + 1;

    // Create histogram
    Int_t *histo = new Int_t[nbins];
    for (Int_t i = 0; i < nbins; i++) histo[i] = 0;

    // Fill the histogram
    for (Int_t i = 0; i < NSAMPLES; i++) {
      Int_t index = s[i] - lowbin;

      // If outside the bound exit to avoid segfault
      if (index < 0 || index >= nbins) continue;

      histo[index]++;
    }

    // Find the maximum element in the histogram
    Int_t maxel = std::max_element(histo, histo + nbins) - histo;
    maxel += lowbin;

    // Calculate the standard deviation on the points **above** the maximum,
    // taking as mean the maximum position
    Float_t sigma = 0.;
    Float_t okpts = 0.;
    for (Int_t i = 0; i < NSAMPLES; i++) {
      if (s[i] > maxel) {
        sigma += (s[i] - maxel) * (s[i] - maxel);
        okpts++;
      }
    }
    sigma /= okpts;
    sigma = sqrt(sigma);

    // Calculate the mean of the points that are +- sigma around the maximum
    Float_t mean = 0.;
    okpts = 0.;
    for (Int_t i = 0; i < NSAMPLES; i++) {
      if (abs(s[i] - maxel) < sigma) {
        mean += s[i];
        okpts++;
      }
    }
    mean /= okpts;

    // Subtract the mean
    for (Int_t i = 0; i < NSAMPLES; i++) s[i] -= mean;

    // Delete histogram
    delete[] histo;
  }
}

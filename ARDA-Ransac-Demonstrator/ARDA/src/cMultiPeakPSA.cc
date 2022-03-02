#include "cMultiPeakPSA.h"
#include "cAnalysisController.h"
#include "TObject.h"

class cMultiPeakPSAFactory : public cAnalysisController::abstractFactory {
  public:
    cMultiPeakPSAFactory() : abstractFactory("multiPeak") {}
    ~cMultiPeakPSAFactory() {}

    cPSA* createPSA() {return new cMultiPeakPSA;}
};

static cMultiPeakPSAFactory cMultiPeakFacObj;

void cMultiPeakPSA::analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid) {
  // Access the raw signal at position globalchannelid
  cRawSignal &sig = (*sigs)[globalchannelid];
  signal = sig.getSamples();
  curGlobalChannelId = globalchannelid;

  // Calculate the discrete derivative of the signal
  calculateDerivative();

  // Find maximums and fill their hits into the hit array
  findMaximums();
}

// Calculate the discrete derivative
void cMultiPeakPSA::calculateDerivative() {
  derivative[0] = signal[1] - signal[0];
  derivative[NSAMPLES - 1] = signal[NSAMPLES - 1] - signal[NSAMPLES - 2];

  for (int i = 1; i < NSAMPLES - 1; i++) {
    derivative[i] = signal[i + 1] - signal[i - 1];
  }
}

// Find maximums and create hits
void cMultiPeakPSA::findMaximums() {
  // Try to find a point where the derivative goes to zero
  for (int i = 5; i < NSAMPLES - 6; i++) {
    // It must be positive and then negative, in order
    // for the point to be a local maximum
    if (derivative[i] > 0 && derivative[i + 1] < 0) {
      // If it is a maximum the derivative must hold the sign for about +- 5 samples
      bool badMaximumFlag = false;

      for (int j = i - 5; j < i; j++) {
        if (derivative[j] < 0) {
          badMaximumFlag = true;
          break;
        }
      }
      for (int j = i + 1; j <= i + 5; j++) {
        if (derivative[j] > 0) {
          badMaximumFlag = true;
          break;
        }
      }

      if (!badMaximumFlag) {
        // Interpolate to find the best position for the maximum
        double iFine = i - derivative[i] / (derivative[i + 1] - derivative[i]);

        double maxVal = 0.;
        if (signal[i] > signal[i + 1]) maxVal = signal[i];
        else maxVal = signal[i + 1];

        hits.emplace_back(curGlobalChannelId, maxVal, iFine);
      }
    }
  }
}

#include "cDSSDBasicPSA.h"
#include "cAnalysisController.h"
#include "TObject.h"
#include "Parameters.h"
#include <cmath>

using namespace std;

class cDSSDBasicPSAFactory : public cAnalysisController::abstractFactory {
  public:
    cDSSDBasicPSAFactory() : abstractFactory("dssdbasic") {}
    ~cDSSDBasicPSAFactory() {}

    cPSA* createPSA() {return new cDSSDBasicPSA;}
};

static cDSSDBasicPSAFactory cMayaBasicFacObj;

void cDSSDBasicPSA::analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid) {
  // Set min to a value greater than 2^12 in order to look for minimum
  Int_t min     = 5000;
  Int_t max     =    0;
  Int_t minchan =   -1;
  Int_t maxchan =   -1;

  // Access the raw signal at position globalchannelid
  cRawSignal &sig = (*sigs)[globalchannelid];

  Float_t *samples = sig.getSamples();

  // Compute average of the signal to tell if it is a positive or negative pulse
  Double_t average = 0.;
  for (int i = 0; i < NSAMPLES; i++) average += samples[i] / NSAMPLES;

  for (int i = 0; i < NSAMPLES; i++) {
    if (samples[i] < min) {
      min = samples[i];
      minchan = i;
    }

    if (samples[i] > max) {
      max = samples[i];
      maxchan = i;
    }
  }

  Int_t val     = -1;
  Int_t valchan = -1;
  if (max - average > average - min) {
    val     = max;
    valchan = maxchan;
  }
  else {
    val     = min;
    valchan = minchan;
  }

  // Samples to average for baseline subtraction
  Int_t smplavg = 40;
  Double_t baseline = 0.;

  // Sum smplavg before peak (approximate width is 40)
  for (Int_t i = valchan - 20 - smplavg; i < valchan - 20; i++) {
    baseline += samples[i] / smplavg;
  }

  hits.emplace_back(sig.getGlobalChannelId(), abs(baseline - val), valchan);
}

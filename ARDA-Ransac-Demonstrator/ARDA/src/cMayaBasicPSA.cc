#include "cMayaBasicPSA.h"
#include "cAnalysisController.h"
#include "TObject.h"
#include "Parameters.h"

class cMayaBasicPSAFactory : public cAnalysisController::abstractFactory {
  public:
    cMayaBasicPSAFactory() : abstractFactory("mayabasic") {}
    ~cMayaBasicPSAFactory() {}

    cPSA* createPSA() {return new cMayaBasicPSA;}
};

static cMayaBasicPSAFactory cMayaBasicFacObj;

void cMayaBasicPSA::analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid) {
  // Set min to a value greater than 2^12 in order to look for minimum
  Int_t min     = 5000;
  Int_t minchan =   -1;

  // Access the raw signal at position globalchannelid
  cRawSignal &sig = (*sigs)[globalchannelid];

  Float_t *samples = sig.getSamples();

  for (int i = 0; i < NSAMPLES; i++) {
    if (samples[i] < min) {
      min = samples[i];
      minchan = i;
    }
  }

  // Samples to average for baseline subtraction
  Int_t smplavg = 40;
  Double_t baseline = 0.;

  // Sum smplavg after and before peak (approximate width is 40)
  for (Int_t i = minchan + 20; i < minchan + 20 + smplavg; i++) {
    baseline += samples[i] / (2. * smplavg);
  }
  for (Int_t i = minchan - 20 - smplavg; i < minchan - 20; i++) {
    baseline += samples[i] / (2. * smplavg);
  }

  hits.emplace_back(sig.getGlobalChannelId(), baseline - min, minchan);
}

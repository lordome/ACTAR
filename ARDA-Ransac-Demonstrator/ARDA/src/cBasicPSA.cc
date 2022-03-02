#include "cBasicPSA.h"
#include "cAnalysisController.h"
#include "TObject.h"
#include "Parameters.h"

class cBasicPSAFactory : public cAnalysisController::abstractFactory {
  public:
    cBasicPSAFactory() : abstractFactory("basic") {}
    ~cBasicPSAFactory() {}

    cPSA* createPSA() {return new cBasicPSA;}
};

static cBasicPSAFactory cBasicFacObj;

void cBasicPSA::analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid) {
  Int_t max     = -1;
  Int_t maxchan =  0;

  // Access the raw signal at position globalchannelid
  cRawSignal &sig = (*sigs)[globalchannelid];

  Float_t *samples = sig.getSamples();

  for (int i = 0; i < NSAMPLES; i++) {
    if (samples[i] > max) {
      max = samples[i];
      maxchan = i;
    }
  }

  hits.emplace_back(sig.getGlobalChannelId(), max, maxchan);
}

#include "cLeuvenPSA.h"
#include "cAnalysisController.h"
#include "TObject.h"
#include "Parameters.h"
#include "TrapezoidAnalyser.h"

#include <algorithm>
using namespace std;

class cLeuvenPSAFactory : public cAnalysisController::abstractFactory {
  public:
    cLeuvenPSAFactory() : abstractFactory("leuvenPSA") {}
    ~cLeuvenPSAFactory() {}

    cPSA* createPSA() {return new cLeuvenPSA;}
};

static cLeuvenPSAFactory cLeuvenFacObj;

cLeuvenPSA::cLeuvenPSA() {
  convSig  = new short int[NSAMPLES];
  baseline = new short int[NSAMPLES];

  for (int i = 0; i < NSAMPLES; i++) baseline[i] = 0;
}

cLeuvenPSA::~cLeuvenPSA() {
  delete[] convSig;
  delete[] baseline;
}


void cLeuvenPSA::analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid) {
  // Access the raw signal at position globalchannelid
  cRawSignal &sig = (*sigs)[globalchannelid];

  Float_t *samples = sig.getSamples();

  TrapezoidAnalyser trAn(parameters[0], parameters[1], parameters[2]);

  for (int i = 0; i < NSAMPLES; i++) convSig[i] = samples[i];

  if (trAn.Analyse(convSig, baseline, NSAMPLES)) {
    vector<double> &shapedPulse = trAn.GetShapedPulse();

    auto maxPos = max_element(shapedPulse.begin(), shapedPulse.end());

    hits.emplace_back(sig.getGlobalChannelId(), *maxPos, maxPos - shapedPulse.begin());
  }
}

void cLeuvenPSA::addParameter(Double_t v) {
  if (curpar > 2) return;
  parameters[curpar] = v;
}

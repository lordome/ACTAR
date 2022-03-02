#include "cExamplePSA.h"
#include "TObject.h"
#include "Parameters.h"

// Mandatory to have the PSA working
#include "psaUtil.h"

// Add your needed includes here

void psaClassName::analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid) {
  // Modify this
  Int_t max     = -1;
  Int_t maxchan =  0;

  // Access the raw signal at position globalchannelid
  cRawSignal &sig = (*sigs)[globalchannelid];

  Int_t *samples = sig.getSamples();

  for (int i = 0; i < NSAMPLES; i++) {
    if (samples[i] > max) {
      max = samples[i];
      maxchan = i;
    }
  }

  hits.emplace_back(sig.getGlobalChannelId(), max, maxchan);
}

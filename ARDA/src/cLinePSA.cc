#include "cLinePSA.h"
#include "cAnalysisController.h"
#include "Parameters.h"

#include "psaUtil.h"

#define INTMAXLOW   30
#define INTMAXHIGH 100
#define STEP 200

class cLinePSAFactory : public cAnalysisController::abstractFactory {
  public:
    cLinePSAFactory() : abstractFactory("line") {}
    ~cLinePSAFactory() {}

    cPSA* createPSA() {return new cLinePSA;}
};

static cLinePSAFactory cLineFacObj;

void cLinePSA::analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid)
{
  Double_t max = 0;
  Int_t maxchan =  -1;

  Bool_t STEP_FLAG = kFALSE;

  // Access the raw signal at position globalchannelid
  cRawSignal &sig = (*sigs)[globalchannelid];

  Float_t *samples = sig.getSamples();

  for(int kk=1; kk<NSAMPLES; kk++)
  {
    if(TMath::Abs(samples[kk]-samples[kk-1])>STEP)
    {
     STEP_FLAG = kTRUE;
     break;
    }
  }

 if(STEP_FLAG)
 {
   hits.emplace_back(sig.getGlobalChannelId(), max, maxchan);
   return;
 }

 max = samples[0];
 maxchan = 0;
 for (Int_t i = 1; i < NSAMPLES; i++) {
   if (max < samples[i]) {
     maxchan = i;
     max = samples[i];
   }
 }

 /* starting baseline subtraction modelling it as a generic linear function*/

 int Nbl = 0;
 double A = 0;
 double B = 0;
 double C = 0;
 double D = 0;

 int lower = maxchan-INTMAXLOW;
 int upper = maxchan+INTMAXHIGH;

 for(int kk=0; kk<NSAMPLES; kk++)
 {
    if(kk<lower || kk>upper)
    	 {
   	  Nbl++;
   	  double val = samples[kk];
   	  A += kk;
   	  B += kk*kk;
   	  C += val;
   	  D += val*kk;
 	 }
 }

 double det = Nbl*B-A*A;

 double q = (B*C-A*D)/det;
 double m = (Nbl*D-A*C)/det;

 /* correction of the maximum -> "baseline" subtraction*/

 max -= m*maxchan+q;

 hits.emplace_back(sig.getGlobalChannelId(), max, maxchan);

}

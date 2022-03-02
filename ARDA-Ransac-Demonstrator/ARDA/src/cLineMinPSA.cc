#include "cLineMinPSA.h"
#include "cAnalysisController.h"
#include "Parameters.h"

#include <cmath>

#include "psaUtil.h"

#define INTMAXLOW   50
#define STEP 10e10
#define STARTVAL 5
#define NMINPOI  10

class cLineMinPSAFactory : public cAnalysisController::abstractFactory {
  public:
    cLineMinPSAFactory() : abstractFactory("lineMin") {}
    ~cLineMinPSAFactory() {}

    cPSA* createPSA() {return new cLineMinPSA;}
};

static cLineMinPSAFactory cLineFacObj;

void cLineMinPSA::analyse(std::map<Int_t, cRawSignal>* sigs, Int_t globalchannelid)
{
  Double_t minval = 0;
  Int_t minchan =  -1;

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
   hits.emplace_back(sig.getGlobalChannelId(), minval, minchan);
   return;
 }

 minval = samples[STARTVAL];
 minchan = STARTVAL;
 for (Int_t i = STARTVAL+1; i < NSAMPLES; i++) {
   if (samples[i] < minval) {
     minchan = i;
     minval = samples[i];
   }
 }

 /* starting baseline subtraction modelling it as a generic linear function*/

 int Nbl = 0;
 double A = 0;
 double B = 0;
 double C = 0;
 double D = 0;

 int lower = minchan-INTMAXLOW;

 if (lower <= NMINPOI + STARTVAL) return;

 for(int kk=STARTVAL; kk<lower; kk++) {
   Nbl++;
   double val = samples[kk];
   A += kk;
   B += kk*kk;
   C += val;
   D += val*kk;
 }

 double det = Nbl*B-A*A;

 double q = (B*C-A*D)/det;
 double m = (Nbl*D-A*C)/det;

 /* correction of the maximum -> "baseline" subtraction*/

 minval -= m*minchan+q;
 minval *= -1.;

 double ookpts = 0.;
 double scarto = 0.;

 for (int i = STARTVAL; i < lower; i++) {
   scarto += (samples[i] - m*i - q)*(samples[i] - m*i - q);
   ookpts++;
 }

 scarto /= ookpts;
 scarto = sqrt(scarto);

 if (m > 1.) savedSignals.push_back(sig);

 cHit hh(sig.getGlobalChannelId(), minval, minchan);
 hh.getpsaValues().push_back(q);
 hh.getpsaValues().push_back(m);
 hh.getpsaValues().push_back(scarto);
 hits.push_back(hh);
}

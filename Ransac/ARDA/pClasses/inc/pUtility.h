#ifndef p_Utility
#define p_Utility

#include <TMath.h>
#include <TF1.h>
#include <TRandom.h>


#define DEBUG






class pUtility {

 public: 
  //-----Added ottanelli 22/12/2016
//Segnale di corrente con tempo di salita e di discesa ampiezza, t0, parametro di scaling temporale ...
static double GenerateCurrentSignal(double *x,double* pars){
  double t1=pars[0];
  double t2=pars[1];
  double A=pars[2];
  double t0=pars[3];
  double g=pars[4];
  double X=(x[0]-t0);
  if (X>=0) {
    double tbar=1/(1/t1+1/t2);
    double norm=t2-tbar-tbar*tbar/t1;
    return g*A*(1-(1+g*X/t1)*exp(-g*X/t1))*exp(-g*X/t2)/norm;
  }
  else return 0;  
}

//Segnale di carica generato da integrazione del segnale di corrente su preamplificatore con costante di decadimento
static double GenerateChargeSignal(double *x,double* pars){
  double t1=pars[0];
  double t2=pars[1];
  double t3=pars[2];
  double A=pars[3];
  double B=pars[4];
  double t0=pars[5];
  double g=pars[6];
  double X=(x[0]-t0);
  if (X>=0) {
    double tbar=1/(1/t1+1/t2);
    double norm=t2-tbar-tbar*tbar/t1;
    double a1=1/t3-g/t2;
    double a2=g*(1/t1+1/t2);
    double a3=1/t3-a2;
    double p1= exp(-X/t3)*(1/a3-1/a1-g/(t1*a3*a3));
    double p2= exp(-X*a2)*(g/(t1*a3*a3)-1/a3-g*X/(t1*a3));
    double p3=exp(-g*X/t2)/a1;
    return g*A*(p1+p2+p3)/norm+B;
  }
  else return B;  
}



  
   pUtility(){}
   virtual ~pUtility(){}

   /*! <p>Returns the pointer to a TF1 (ROOT function) which has more or less the shape of the output signal from
    * a charge sensitive preamplifier (CSA or CSPA). The shape includes the exponential decay due to the
    * RC parallel in the feedback loop. Polarity is positive for positive amplitude. The function is:</p>
    * <p> f(t) = baseline + amplitude*(1 - (1 + (t-tzero)/risetime)*exp(-(t-tzero)/risetime) ) * exp(-(t-tzero)/tau)   for t &gt; tzero</p>
    * <p> f(t) = 0   for t <= tzero</p>
    *<ul>
    * <li> amplitude: asymptotic amplitude of the step (from baseline to maximum amplitude when tau->infinity)
    * <li> tzero: time interval from t=0 to the beginning of the step (baseline length)
    * <li> risetime: time constant of the leading edge
    * <li> tau: exponential decay constant
    * <li> baseline: DC level of the baseline
    * <li> tmax: function domain will be (0, tmax)
    * </ul>
    * <p>Example:
    * \code
    * root
    * .x init_pClasses.C
    * TF1 *cspa = pUtility::Preamp2TF1(1000,500,10,10000,200,2000)
    * cspa->Draw()
    * \endcode
    * </p>

    */
   static TF1 *Preamp2TF1(double amplitude, double tzero, double risetime, double tau, double baseline, double tmax){
// funzione simile a segnale PA (gli fa bene anche essere un po' filtrata, successivamente)
      TF1 *f = new TF1("f","[4]+(x>[0])*[3]*((1-(1+(x-[0])/[1])*exp(-(x>[0])*(x-[0])/[1])))*exp(-(x>[0])*(x-[0])/[2])",0,tmax);
      f->SetParameter(0,tzero);
      f->SetParameter(1,risetime);
      f->SetParameter(2,tau);
      f->SetParameter(3,amplitude);
      f->SetParameter(4,baseline);
       f->SetNpx(10000);

     return f;
   }

/*! <p>Returns the pointer to a TF1 (ROOT function) which has more or less the shape of the output current signal from a
silicon detector. Polarity is positive for positive amplitude. The function is:</p>
    * <p> f(t) =charge*(1 - (1 + (t-tzer0/trise)*exp(-(t-tzero)/trise) ) * exp(-(t-tzero)/tfall)/N   for t &gt; tzero</p>
    * <p> f(t) = 0   for t <= tzero</p>
    *<ul>
    * <li> charge: integrated charge of the current signal
    * <li> tzero: time interval from t=0 to the beginning of the signal (baseline length)
    * <li> trise: time constant of the leading edge
    * <li> tfall: time constant of the falling edge
    * <li> tmax: function domain will be (0, tmax)
    * <li> N: normalization factor
    * </ul>
    * <p>Example:
    * \code
    * root
    * .x init_pClasses.C
    * TF1 *current = pUtility::Current2TF1(1000,10,10,200,2000)
    * current->Draw()
    * \endcode
    * </p>

    */
   static TF1 *Current2TF1(double charge, double trise,double tfall,double tzero,double tmax){
     // Funzione simile a segnale di corrente con tempo di salita e discesa
     TF1 *f=new TF1("current",&GenerateCurrentSignal,0,tmax,5);
      f->SetParameter(0,trise);
      f->SetParameter(1,tfall);
      f->SetParameter(2,charge);
      f->SetParameter(3,tzero);
      f->SetParameter(4,1);
      f->SetNpx(10000);
      return f;
   }

   /*! <p>Returns the pointer to a TF1 (ROOT function) which has more or less the shape of the output current signal from a
silicon detector after a charge pre-amplifier. The shape includes the exponential decay due to the
    * RC parallel in the feedback loop. Polarity is positive for positive amplitude.
    * <li> charge: integrated charge of the current signal
    * <li> tzero: time interval from t=0 to the beginning of the signal (baseline length)
    * <li> trise: time constant of the leading edge
    * <li> tfall: time constant of the falling edge
    * <li> tau: exponential decay constant
    * <li> baseline: DC level of the baseline
    * <li> tmax: function domain will be (0, tmax)
    * <li> N: normalization factor
    * </ul>
    * <p>Example:
    * \code
    * root
    * .x init_pClasses.C
    * TF1 *charge = pUtility::Charge2TF1(1000,10,10,10000,200,2000)
    * charge->Draw()
    * \endcode
    * </p>

    */
   static TF1 *Charge2TF1(double charge, double trise,double tfall,double tau,double baseline,double tzero,double tmax){
     // Funzione simile a segnale di corrente con tempo di salita e discesa
     TF1 *f=new TF1("charge",&GenerateChargeSignal,0,tmax,7);
      f->SetParameter(0,trise);
      f->SetParameter(1,tfall);
      f->SetParameter(2,tau);
      f->SetParameter(3,charge);
      f->SetParameter(4,baseline);
      f->SetParameter(5,tzero);
      f->SetParameter(6,1);
      f->SetNpx(10000);
      return f;
   }


   

   
   /*! <p>Returns the pointer to a TF1 (ROOT function) which is a sine wave of the form</p>
    * <p>ampl * sin(2* PI *freq*(t-toff) )</p>
    * <p>on the domain (0, tmax).</p>
    * 
   */
   static TF1 *Sine2TF1(double ampl, double freq, double toff, double tmax){
          TF1 *f = new TF1("sine",Form("%f*sin(2.*TMath::Pi()*%f*(x-%f))",ampl,freq,toff),0, tmax);
          f->SetNpx(10000);
	  return f;
   }
   
   /*! <p>Returns the pointer to a TF1 (ROOT function) which has more or less the shape of the output
    * of the anode of a PhotoMultiplier Tube (PMT). Polarity is negative for positive amplitude. </p>
    * <p>f(t) = ampl * (exp(-(t-tzero)/taur) - exp(-(t-tzero)/tauf) )  for t &gt; zero</p>
    * <p>f(t) =0   for t <= tzero </p>
    * <p>on the domain (0, tmax).</p>
    * <ul>
    * <li> ampl: signal scale factor (maximum amplitude will be different from ampl and it will also depend on taur and taur)
    * <li> taur: leading edge time constant (must be < tauf)
    * <li> tauf: falling edge time constant (must be > taur)
    * <li> tzero: time interval from t=0 to the beginning of the step (baseline length)
    * <li> tmax: function domain will be (0, tmax)
    * </ul>
    * <p>Example:
    * \code
    * root
    * .x init_pClasses.C
    * TF1 *pmt = pUtility::PMT2TF1(1000,10,20,100,500)
    * pmt->Draw()
    * \endcode
    * </p>
   */  
   static TF1 *PMT2TF1(double ampl, double taur, double tauf, double tzero, double tmax){
        TF1 *f = new TF1("pmt","[0]*(exp(-(x-[3])/[1])-exp(-(x-[3])/[2]))*(x>[3])",0,tmax);
        f->SetParameter(0,ampl);
        f->SetParameter(1,taur);
        f->SetParameter(2,tauf);
        f->SetParameter(3,tzero);
        f->SetNpx(10000);
        return f;
   }
   
 
 
   /*! Converts risetime value of a step response to system bandwidth (if trise in ns, fc will be in GHz).
    * Exact for one-pole low pass filters, good approx for usual multipole filters (Sallen-Key).
    */
   static Double_t trise2fc(Double_t trise){return 0.35/trise;}

   /*! Converts  system bandwidth to risetime value of the step response (if fc in GHz,  trise will be in ns).
    */
   static Double_t fc2trise(Double_t fc){return 0.35/fc;}

   /*! Converts filter time constant (tau=1/omega where omega angular frequency, omega=2*PI*fc) to system bandwidth.
    */
   static Double_t tau2fc(Double_t tau){return 1/(2.0*TMath::Pi()*tau);}

   /*! Converts system bandwidth  to  filter time constant (tau=1/omega where omega angular frequency).
    */
   static Double_t fc2tau(Double_t fc){return 1/(2.0*TMath::Pi()*fc);}

      /*! Converts rms noise value (in LSB) to ENOB, given the resolution N (approx formula!)
    */
   static Double_t rms2ENOB(Double_t rms,int N){return log10(pow(2.,N)/sqrt(12.)/rms)/log10(2.);}

      /*! Converts ENOB to rms noise value (in LSB), given the resolution N  (approx formula!)
    */
   static Double_t ENOB2rms(Double_t ENOB,int N){return pow(2.,N-ENOB)/sqrt(12.);}

    /*! Converts ENOB to SINAD in dB
    */
   static Double_t ENOB2SINAD(Double_t ENOB){return ENOB*6.02+1.76;}

   /*! Converts SINAD in dB  to ENOB
    */
   static Double_t SINAD2ENOB(Double_t SINAD){return (SINAD-1.76)/6.02;}

ClassDef(pUtility,1);

};

#endif

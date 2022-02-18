
// to be implemented: negative frequency, to use Oppenheim-Schafer
// convention

#ifndef p_FT

#define p_FT

#include <TObject.h>
#include <TGraph.h>
#include <TF1.h>
#include <TH1F.h>
#include <TMath.h>
#include <TArrayD.h>
#include <TAxis.h>

#include "pSignal.h"

// Basic classes:
// pSignal: samples storage and simple operations on samples, signal generation
//             (sinusoidal, preamp, PMT signals...)
// pADC: A/D conversion simulation
// pFT: contains a Fourier Tranform
// pDSP: handles Fourier transforms, Z-Tranforms, Filters, various LTI system operations, convolution, recursive difference
//                    equations....

#define DEBUG
class pFT: public TObject{

 public: 
// class constructors
     pFT(int N);
     pFT(const pFT &a);
     virtual ~pFT(){}     
     // signal generation
   pSignal *Magnitude();
   pSignal *Phase();
   
   /*! 
    *  Return a signal containing the real part of the DFT (be careful: in the
    * previous version a TArrayD * was returned like for GetArrayReal()!)
    * */
   pSignal *Real();
   /*! 
    *  Return a signal containing the imaginary part of the DFT (be careful: in the
    * previous version a TArrayD * was returned like for GetArrayImag()!)
    * */
   pSignal *Imag();

   /*! Returns a TH1F histogram containing the DFT Maginutude using the units set with SetUnits() */
   TH1F    * FT2MagHisto();
   // graphical utilities
   
   TObject * DrawReal(Option_t *option);
   TObject * DrawImag(Option_t *option);
   TObject * DrawMagnitude(Option_t *option);
   TObject * DrawPhase(Option_t *option);
   
   // getters
   TArrayD *GetArrayReal() {return re;}
   TArrayD *GetArrayImag() {return im;}
   int GetN() {return nsamples;}
   double GetTs() {return Ts;}
   TString *GetUnits(){return units;}

  // setters
   int SetN(int value) {nsamples = value; re->Set(value);im->Set(value); return nsamples;}
  double SetTs(double value) {Ts=value; Fs=1./Ts; return Ts;}
  double SetRange(double value) {range=value; return range;}
  int SetLineColor(int val){lcol=val; return lcol;} 
  int SetMarkerColor(int val){mcol=val; return mcol;}
  int SetMarkerStyle(int val){mstyle=val; return mstyle;}
  TString *SetUnits(const char *val){units->Clear(); units->Append(val);return units;}

  // operators
    pFT operator  = (const pFT &);

protected:

 double Ts;   // sampling period in ns
 double Fs;   // sampling frequency in GHz
 double range; // peak-to-peak range, default +- 1V
 int Nbits;  // number of bits for quantization
 int fractional; // set to 1 for fractional (i.e. abs(sample)<1) output 
 int nsamples;  // number of samples in pFT
 // graphic parameters
 int mstyle;
 int mcol;
 int lcol;
 TString *units;
 TArrayD *re;
 TArrayD *im;
 
 ClassDef(pFT,2);
};

#endif

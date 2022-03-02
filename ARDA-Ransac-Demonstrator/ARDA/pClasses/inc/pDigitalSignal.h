
#ifndef p_DigitalSignal
#define p_DigitalSignal 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <TObject.h>
#include <TGraph.h>
#include <TF1.h>
#include <TH1F.h>
#include <TMath.h>
#include <TArrayD.h>
#include <TArrayL64.h>
#include <TAxis.h>
#include <TMatrixF.h>
#include <TMatrixD.h>
#include "pSignal.h"

/*
// CFD type defines:
#define LINEAR_CFD  0
#define CUBIC_CFD  1
#define CUBICCONV_CFD 2
#define CUBICSPLINE_CFD 3
#define PENTA_CFD  4
#define SMOOTHINGSPLINE_CFD 5
*/
using namespace std;
// Basic classes:
// pSignal: samples storage and simple operations on samples, signal generation
//             (sinusoidal, preamp, PMT signals...)
// pADC: A/D conversion simulation
// pFT: contains a Fourier Tranform
// pDSP: handles Fourier transforms, Z-Tranforms, Filters, various LTI system operations, convolution, recursive difference
//                    equations....

#define DEBUG
/*! \brief Class modelling a digitized signal with integer samples*/
/* Inherits from TArrayL64(), samples are stored as Long64_t (64 bit integers)*/
 /* Automatically manages saturation effects etc...
 */
class pDigitalSignal: public TObject, public TArrayL64{

 public: 
// class constructors
//! creates a zero length pDigitalSignal with standard values of parameters (as fixed by Init())
     pDigitalSignal();
     //! creates a pDigitalSignal of length N samples, with standard values of other parameters (see Init())
     pDigitalSignal(int N,int nbits=14,int Signed=0);
     //! creates a pDigitalSignal which is a copy of pSignal a with integer samples
     pDigitalSignal(pSignal &a);
      //! creates a pDigitalSignal which is a copy of pDigitalSignal a
     pDigitalSignal(const pDigitalSignal &a);
     //! creates a pSignal from a TArrayD
     pDigitalSignal(const TArrayL64 &a);
     //! inits internal variables to standard values
     void Init();     
     //! class destructor
     virtual ~pDigitalSignal(){}
     

     //Build signal
      void BuildSine(double amplitude, double freq, double toff);
 
   
   // graphical utilities

   //! returns a TGraph object which is a graphical representation of the signal
    //! useful to do some more graphic tricks than allowed by pDigitalSignal class
    //! tshift allows for shifting the samples in time (useful to get negative values
    //!  of times for plots). Takes into account the saturation of the digital signal.
    TGraph  *Sig2Graph(double tshift);

   //! returns a TGraph object which is a graphical representation of the signal
    //! useful to do some more graphic tricks than allowed by pDigitalSignal class.
    //!Takes into account the saturation of the digital signal.
    TGraph  *Sig2Graph();
    
    //! returns a TH1F histogram which is a graphical representation of the signal.
    //!Takes into account the saturation of the digital signal.
    TH1F  *Sig2Histo();
    
    //! draw the pSignal either as TGraph or as TH1F depending on option.
    
    /*! <ul>
     * <li> if option contains "histo" a TH1F will be created and  plotted
     * <li> otherwise a TGraph will be created and  plotted
     * </ul>
     * e.g. :
     \code
       s->Draw("ALP");
       \endcode
       will plot the pDigitalSignal as a TGraph with markers, lines between markers, also drawing axis.
       * Takes into account saturation effects of pDigitalSignal.
     */  
    void Draw(Option_t *option);

    
    //! gets  resolution in bits (useful when signal obtained from ADC)
     int GetNbits() {return Nbits;}
     
     //! get  length in samples
     int GetN() {return nsamples;}
     
     //! gets  sampling period in ns
     double GetTs() {return Ts;}
     
     //! get sampling frequency in GHz
    double GetFs() {return Fs;}
    
    //! get range in Volt
     double GetRange() {return range;}

     //! return 1 if the signal uses signed integer notation, 0 if not
     int GetSigned(){ return Signed;}

     //! returns the Bit mask used to simulate saturation effects
     Long64_t GetMask(){ return Mask;}
     
     //! get time-axis units ("samples" or "ns")
     TString *GetUnits(){return units;}
     
     //! get line color
     int GetLineColor(){return lcol;} 

     //! get marker color
     int GetMarkerColor(){return mcol;}

    //! get marker style
     int GetMarkerStyle(){return mstyle;}

     
        
  // setters
  //! set signal length in samples
    int SetN(int value) {nsamples = value; this->Set(value); return nsamples;}

    //! set signal resolution in bits (useful when signal obtained from ADC)
    int SetNbits(int value) {Nbits = value;BuildMask(); return Nbits;}

    //! sets signal sampling period in ns (used, e.g., for X axis calculations in physical units).
    //! Sampling frequency is updated accordingly
    double SetTs(double value) {Ts=value; Fs=1.0/Ts; return Ts;}

    //! sets signal notation type: 1 for signed notation, 0 for unsigned notation.
    int SetSigned(int signed_st) {
      if(signed_st<=0) Signed=0;
      else Signed=1;
      BuildMask();
      return Signed;
    }
    
    //! Set time axis units: if "samples" the abscissa reports the sample index;
    //! if "ns" the abscissa is the sample index times the sampling period (physical time)
        TString *SetUnits(char *val){units->Clear(); units->Append(val); return units; }

     //! sets signal frequency in GHz,   
    //! sampling period is updated accordingly
    double SetFs(double value) {Fs=value; Ts=1.0/Fs; return Fs;}
    
    //! sets range of signal (useful when signal obtained from ADC)
    double SetRange(double value) {range=value; return range;}
    
    //! sets line color for plots
    int SetLineColor(int val){lcol=val; return lcol;} 
    //! sets marker color for plots
    int SetMarkerColor(int val){mcol=val; return mcol;}
    //! sets marker style for plots
    int SetMarkerStyle(int val){mstyle=val; return mstyle;}
    
 //! Fits a TF1 function to the signal using Minuit (see ROOT manual for options)
    TFitResultPtr Fit(TF1* f1, Option_t* option = "", Option_t* goption = "", Axis_t xmin = 0, Axis_t xmax = 0);
  
    
/*!  \brief  Returns the time value (with origin t=0 at the first sample) for which
 * signal = fmax (threshold crossing), reconstructing the continuous signal via linear interpolation.
 * <ul>
 * <li>x2: index of sample immediately before threshold crossing (must be determined in advance, before calling this method)
 *<li> fmax: threshold value
 * </ul>
 */     
     double LinearInterpolation(int x2, double fmax);
    
//! Returns Area between start and stop. Takes baseline average, start, stop and signal polarity

     double Integrate(double, int, int, int);
        
/*!  \brief  Returns the time value (with origin t=0 at the first sample) for which
 * signal = fmax (threshold crossing), reconstructing the continuous signal via cubic interpolation (Bardelli's
 * kernel) and solving the 3rd order equation using Newton method
 * of successive approximations (the crossing obtained via linear interpolation is used as first guess). 
 * This particular interpolation is equivalent to finding the threshold crossing of 
 * the unique cubic passing through four samples, two before and two after the crossing.
 *  These four samples are used to determine the coefficients of the polynomial. 
 * <ul>
 * <li>x2: index of sample immediately before threshold crossing (must be determined in advance, before calling this method)
 *<li> fmax: threshold value
 * <li> Nrecurr: number of iterations for recursive solution
 * </ul>
 */     
     double CubicInterpolation(int x2, double fmax, int Nrecurr);
     
/*!  \brief  Returns the time value (with origin t=0 at the first sample) for which
 * signal = fmax (threshold crossing), reconstructing the continuous signal via cubic interpolation (Key's
 * kernel, see: R.G.Keys, Cubic Convolution Interpolation for Digital Image Processing, IEEE Trans. Acoust., Speech,
 * Signal Processing, vol. ASSP-29, Dec. 1981) and solving the 3rd order equation using Newton method
 * of successive approximations (the crossing obtained via linear interpolation is used as first guess).  Key's interpolation is implemented via the equivalent cubic polynomial
 * which is used to interpolate between the two samples, one before and one after the crossing.
 * Four samples are used to determine the coefficients of the polynomial. 
 * <ul>
 * <li>x2: index of sample immediately before threshold crossing (must be determined in advance, before calling this method)
 *<li> fmax: threshold value
 * <li> Nrecurr: number of iterations for recursive solution
 * </ul>
 */     
    double CubicConvInterpolation(int x2, double fmax, int Nrecurr);
    
/*!  \brief  Returns the time value (with origin t=0 at the first sample) for which
 * signal = fmax (threshold crossing), reconstructing the continuous signal via cubic Spline interpolation 
 * (see: Hou and Andrews, Cubic Splines for image Interpolation and Filtering,  IEEE Trans. Acoust., Speech,
 * Signal Processing, vol.  ASSP-26, N 0 . 6 , Dec. 1978)  and solving the 3rd order equation using Newton method
 * of successive approximations. See article for details.
 * <ul>
 * <li>x2: index of sample immediately before threshold crossing (must be determined in advance, before calling this method)
 *<li> fmax: threshold value
 * <li> Nrecurr: number of iterations for recursive solution
 * </ul>
 */     
    // double CubicSplineInterpolation(int x2, double fmax, int Nrecurr);

 /*!  \brief  Returns the time value (with origin t=0 at the first sample) for which
 * signal = fmax (threshold crossing), reconstructing the continuous signal from cubic Spline coefficients,
 * obtained from the original signal using an LTI filter described by Unser
 * (see: Unser et al., B-Spline Signal Processing: Part I-Theory,  IEEE Trans. Signal Processing,
 * vol.  TSP-41, N 2 , Feb. 1993)  and solving the 3rd order equation using Newton method
 * of successive approximations. See article for details.
 * The coefficients can be obtained from the signal using either pDSP::BuildCubicSplineCoeffSignal() or pDSP::BuildSmoothingSplineCoeffSignal().
 * <ul>
 * <li>x2: index of sample immediately before threshold crossing (must be determined in advance, before calling this method)
 *<li> fmax: threshold value
 * <li> Nrecurr: number of iterations for recursive solution
 * </ul>
 */     
    double CubicSplineLTIInterpolation(int x2, double fmax, int Nrecurr);

    
/*!  \brief  Returns the time value (with origin t=0 at the first sample) for which
 * signal = fmax (threshold crossing), reconstructing the continuous signal via 5th order interpolation
 * and solving the 5th order equation using Newton method
 * of successive approximations   (the crossing obtained via linear interpolation is used as first guess). 
 * This particular interpolation is equivalent to finding the threshold crossing of 
 * the unique 5th order polynomial passing through six samples, three before and three after the crossing.
 *  These six samples are used to determine the coefficients of the polynomial. 
 * <ul>
 * <li>x3: index of sample immediately before threshold crossing (must be determined in advance, before calling this method)
 *<li> fmax: threshold value
 * <li> Nrecurr: number of iterations for recursive solution
 * </ul>
 */
     double PentaInterpolation(int x3, double fmax, int Nrecurr);

/*!  \brief  General method to calculate Constant Fraction Discriminator (CFD) timing.
 * The desired fraction and the signal maximum amplitude must be passed to the method.
 * <ul>
 * <li> fraction: CFD fraction (0&lt;fraction&lt;1) 
 * <li> Max: signal maximum (threshold=fraction*Max)
 * <li> polarity: if >= 0 signal assumed of positive polarity, if <0 negative polarity is assumed 
 * <li> type: interpolation  type (0=linear, 1=Cubic Bardelli, 2=Cubic Key, 3=Cubic Spline, 4=5th order, 5=Cubic Spline LTI)
 * <li> Nrecurr: number of iterations (if =0 it is equivalent to linear interpolation)
 * <li> reverse: if =0 the first crossing from the beginning of the signal is calculated, if =1 search goes
 *        backward in time starting at start)
 * <li> start: search start time when reverse is used
 * </ul>
 */
 double CFD(double fraction, Long64_t Max, int polarity, int type, int Nrecurr, int reverse=0, double start=0);

 /*! \brief calculates integral between tstart and tstop */
Long64_t  TimeIntegral(double tstart, double tstop);

 
 /*!  \brief  standard forward-in-time Cubic CFD with 3 iterations, positive polarity */
 double PosCubicCFD(double fraction, Long64_t Max){
   return CFD(fraction, Max, 1, CUBIC_CFD,  3, 0, 0);
 }
 
 /*! \brief standard forward-in-time Cubic CFD with 3 iterations, negative polarity */
 double NegCubicCFD(double fraction, Long64_t Max){
   return CFD(fraction, Max, -1, CUBIC_CFD,  3, 0, 0);
 }

 
    /*! \brief produces a pSignal which is the difference (sample by sample) between the  original pDigitalSignal and the TF1 function pointed by f.
     * Useful when "f" points to
    * the fitted function and you want to calculate fit residuals (nomen omen!)*/   
    pSignal *Residuals(TF1 *f);

    
    //  variable calculations
   
  //! \brief looks for maximum or minimum value (according to maxmin flag), writes value into val, returns sample number
  int MaxMin(Long64_t *val, int maxmin){
    int i, index=-1;
    Long64_t val_here;
    val_here=this->At(0);
    for(i=1; i<  GetN(); i++){
      if(this->At(i)> val_here && maxmin==1) { val_here = this->At(i); index = i; } 
      if(this->At(i)< val_here && maxmin==0) { val_here = this->At(i); index = i;} 
    }
    *val = val_here;
    return index;
  }

  //_____________________________________________________________________________
  //!  \brief  returns root mean squared value of samples between ifirst and ilast indexes (NOT standard deviation!).
double RMS(int ifirst, int ilast){
     
     if(ifirst>ilast) {int dummy=ifirst; ifirst=ilast; ilast=dummy;} // put extremes in right order
     if(ifirst<0) ifirst=0;   // no samples with negative indexes
     if(ilast>=GetN()) ilast=GetN()-1;   // no samples beyond end of signal
     
     double sum2=0; 
     for(int i=ifirst; i<=ilast;i++) sum2 += (this->At(i)*this->At(i));
     sum2 /= (ilast-ifirst+1);
     return sqrt(sum2);
}
  //_____________________________________________________________________________
 //!  \brief returns average value of samples between ifirst and ilast indexes
double Average(int ifirst, int ilast){
     double sum=0;
     
     if(ifirst>ilast) {int dummy=ifirst; ifirst=ilast; ilast=dummy;} // put extremes in right order
     if(ifirst<0) ifirst=0;   // no samples with negative indexes
     if(ilast>=GetN()) ilast=GetN()-1;   // no samples beyond end of signal
     
     for(int i=ifirst; i<=ilast;i++) sum += this->At(i);
     sum /= (ilast-ifirst+1);
     return sum;
}
  //_____________________________________________________________________________
  //!  \brief returns standard deviation of samples between ifirst and ilast indexes, around their average value
double StdDev(int ifirst, int ilast){
     double sum2=0;
     
     if(ifirst>ilast) {int dummy=ifirst; ifirst=ilast; ilast=dummy;} // put extremes in right order
     if(ifirst<0) ifirst=0;   // no samples with negative indexes
     if(ilast>=GetN()) ilast=GetN()-1;   // no samples beyond end of signal
     
     double average = Average(ifirst, ilast);
     for(int i=ifirst; i<=ilast;i++) sum2 +=( (this->At(i)-average)* (this->At(i)-average));
     sum2 /= (ilast-ifirst+1-1); // average degree of freedom already used
     return sqrt(sum2);
}

    
   
  //!  \brief  add  a constant to all samples (use negative values for subtraction)
 void AddConstant(Long64_t value,int ResNbits=-1){
   if(ResNbits>0) SetNbits(ResNbits);
   for(int i=0; i<nsamples;i++) {
     this->SetAt(this->At(i)+value, i);  	
   }
   SaturateSignal();
 }
 

  //!  \brief  add two signals sample by sample
  //! ResBits sets the number of bits of the resulting signal
  pDigitalSignal *Add(pDigitalSignal &add,int ResBits=-1);
 
  //!  \brief  subtract two signals sample by sample
   //! ResBits sets the number of bits of the resulting signal
  pDigitalSignal *Subtract(pDigitalSignal &sub,int ResBits=-1);  

  //! \brief Multiply Signal by Long64_t constant
  //! ResBits sets the number of bits of the resulting signal
  pDigitalSignal *Mult(Long64_t factor,int ResBits=-1);

  //Protected SetAt and At
  Long64_t AT(int index,int padding){
    if(index<0) return padding*At(0);
    if(index>=GetN()) return padding*At(GetN()-1);
    return At(index);
  }

  void SETAT(Long64_t value,int index){
    if(index<0) return;
    if(index>=GetN()) return;
    SetAt(value,index);
    return;
  }


  void BuildMask(){
    int i;
    Mask=0;
    for(i=0;i<Nbits;i++) Mask|=(Long64_t)1<<i;
  }
  
    Long64_t Saturate(Long64_t input){
      Long64_t temp=input&Mask;
      Long64_t sign=temp&((Long64_t)1<<(Nbits-1));
      int i;
      if((Signed)&&(sign)) for(i=Nbits;i<64;i++) temp|=((Long64_t)1<<i);
    return temp;
  }

    Long64_t Saturate(Int_t index){
      Long64_t temp=this->At(index)&Mask;
      Long64_t sign=temp&((Long64_t)1<<(Nbits-1));
      int i;
      if((Signed)&&(sign)) for(i=Nbits;i<64;i++) temp|=((Long64_t)1<<i);
    return temp;
  }

    

    void SaturateSignal(){
      BuildMask();
      int i;
      for(i=0;i<nsamples;i++) this->SetAt(Saturate(i),i);
    }
    
  void Lshift(Int_t amount){
    if(amount<=0) return;
    else{
      int i;
      // Nbits+=amount;
      //BuildMask();
      for(i=0;i<nsamples;i++) this->SetAt(this->At(i)<<amount,i);
      SaturateSignal();
      return;
    }
  }

  void Rshift(Int_t amount,Int_t Round=-1){
    if(amount<=0) return;
    else{
      // Nbits-=amount;
      //BuildMask();
      int i;
      Long64_t round;
      for(i=0;i<nsamples;i++){
	round=0;
	if(Round>0) round=(this->At(i)>>(amount-1))&1;
	this->SetAt((this->At(i)>>amount)+round,i);
      }
      //    SaturateSignal();
      return;
    }
  }

  //============= Convert to pSignal==============
  //! \brief generates a standard PSignal from a pDigitalSignal
  pSignal ToPSignal(){
    pSignal res(this->GetN());
    res.SetTs(this->GetTs());
    res.SetRange(this->GetRange());
    //res.SetFractional(0);
    res.SetLineColor(this->GetLineColor());
    res.SetMarkerColor(this->GetMarkerColor());
    res.SetMarkerStyle(this->GetMarkerStyle());
    //res.SetUnits(this->GetUnits()->Data());
    int i;
    for(i=0;i<this->GetN();i++) res.SetAt((Double_t)this->At(i),i);
    return res;
  }
    
  // ============  operators ==================
  //!  \brief  to copy one signal to another using = sign
    pDigitalSignal operator  = (const pDigitalSignal &);

    
    
    //============= Input/Output ================

    //!  \brief  save in ASCII format, suitable for gnuplot etc.
void  SaveAsASCII(char *filename);

protected:

 double Ts;   // sampling period in ns
 double Fs;   // sampling frequency in GHz
 double range; // peak-to-peak range, default +- 1V
 int Nbits;  // number of bits for quantization
 int nsamples;  // number of samples in pDigitalSignal
 int Signed;
 Long64_t Mask;
 // graphic parameters
 int mstyle;
 int mcol;
 int lcol;
 TString *units;
 // TArrayD *s; //->
 
 ClassDef(pDigitalSignal,3);
};
#endif

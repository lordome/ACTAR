


#ifndef p_DSP
#define p_DSP


#include <TObject.h>

#include <TMath.h>
// #include <TArrayD.h>
#include <TArrayD.h>

#include <TRandom.h>
#include <TVirtualFFT.h>

#include "pUtility.h"

#include "pFT.h"
#include "pSignal.h"
#include "pDigitalSignal.h"

// Basic classes:
// pSignal: samples storage and simple operations on samples, signal generation
//             (sinusoidal, preamp, PMT signals...)
// pADC: A/D conversion simulation
// pFT: contains a Fourier Tranform
// pDSP: handles Fourier transforms, Z-Tranforms, Filters, various LTI system operations, convolution, recursive difference
//                    equations....

#define DEBUG
/*! \brief class performing digital signal processing
The idea is to group in this class all digital processing operations:
<ul>
<li> resampling using interpolation
<li> filtering using Linear Time Invariant systems
<li> Fourier Transform (interface to call TVirtualFFT ROOT class)
<li> etc etc
</ul>
*/
class pDSP: public TObject{

 public: 
// class constructors
     pDSP();
     virtual ~pDSP(){}
     // signal generation
 

/*! Apply windowing to finite length signal in order to reduce artifacts in
 * calculating FFT. Four window types available:
 * <ul>
 * <li> BARLETT (window_type=0)
 * <li> HANNING (window_type=1)
 * <li> HAmmING (window_type=2)
 * <li> BLACKMAN (window_type=3)
 * </ul>
 * Taken from FClasses by L.Bardelli.
 */
     pSignal *ApplyWindowing(pSignal &s, int window_type, double beta=0);
     
/*!
 *  From root TVirtualFFT page:<br>
 *-option : consists of 3 parts - flag option and an option to create a new TVirtualFFT
 <ul>
        <li> 1) transform type option:<br>
           Available transform types are:<br>
           C2CForward, C2CBackward, C2R, R2C, R2HC, HC2R, DHT<br>
           see class description for details<br>
        <li> 2) flag option: choosing how much time should be spent in planning the transform:<br>
           Possible options:<br>
           "ES" (from "estimate") - no time in preparing the transform,<br>
                                  but probably sub-optimal  performance<br>
           "M"  (from "measure")  - some time spend in finding the optimal way<br>
                                  to do the transform<br>
           "P" (from "patient")   - more time spend in finding the optimal way<br>
                                  to do the transform<br>
           "EX" (from "exhaustive") - the most optimal way is found<br>
           This option should be chosen depending on how many transforms of the<br>
           same size and type are going to be done.<br>
           Planning is only done once, for the first transform of this size and type.<br>
         <li>3) option allowing to choose between the global fgFFT and a new TVirtualFFT object<br>
           ""  - default, changes and returns the global fgFFT variable<br>
           "K" (from "keep")- without touching the global fgFFT,<br>
           creates and returns a new TVirtualFFT*. User is then responsible for deleting it.<br>
           </ul>
 Examples of valid options: "R2C ES K", "C2CF M", "DHT P K", etc.
 * NOTE FROM ROOT REF. MANUAL: FFTW computes unnormalized transforms, so doing a transform, followed by its
 *      inverse will give the original array, multiplied by normalization constant
 *      (transform size(N) for FFT, 2*(N-1) for DCT-I, 2*(N+1) for DST-I, 2*N for
 *      other sine/cosine transforms)
 *
 *
 *
 *
 **/     
     pFT  *FFT(pSignal &s,const char *opt=(char*)"R2C ES K",int window=3);

/*! Calculate a Discrete Fourier Tranform (slow method! no FFT!).
 * Taken from FClasses by L.Bardelli.
 * No windowing is applied.
 * Time interval to tranform can be passed using t0 and t1 (default is entire signal).
 */

     pFT  *DFT(pSignal &s,float t0=-1, float t1=-1); 


      /*! returns a pointer to a new pSignal, obtained by delaying
      * pSignal s by an integer number of samples. 
       *   <ul>
       *  <li>  s: input pSignal object
       * <li> ndelay:   desired delay in samples: positive value=> proprt delay;  negative value==>signal is anticipated
       * </ul>
       */
     pSignal *SampleDelay(pSignal &s, int ndelay);

       /*! returns a pointer to a new pDigitalSignal, obtained by delaying
      * pDigitalSignal s by an integer number of samples. 
       *   <ul>
       *  <li>  s: input pDigitalSignal object
       * <li> ndelay:   desired delay in samples: positive value=> proprt delay;  negative value==>signal is anticipated
       * </ul>
       */
     pDigitalSignal * SampleDelay(pDigitalSignal &s, int ndelay);
     
     /*! returns a pointer to a new pSignal, obtained by decimation
      * of pSignal s. Decimation factor is: m.*/
    pSignal * Decimation(pSignal &s, int m, int do_filter);

     /*! returns a pointer to a new pDigitalSignal, obtained by decimation
      * of pDigitalSignal s. Decimation factor is: m.*/
    pDigitalSignal * Decimation(pDigitalSignal &s, int m, int do_filter);
     
/*! Returns the interpolated value of pSignal <strong>s</strong>, calculated at time <strong>t</strong>
 * using linear interpolation. 
 */
     static double GetDataInterLinear(pSignal &s, double t);
 
/*! Returns the interpolated value of pSignal <strong>s</strong>, calculated at time <strong>t</strong>
 * using Cubic interpolation and Bardelli's kernel. 
 */  
     static double GetDataInterCubic(pSignal &s, double t);

     /*! Returns the interpolated value of pSignal <strong>s</strong>, calculated at time <strong>t</strong>
 * using Cubic interpolation and Key's kernel. 
 */  
     static double GetDataCubicConvolution(pSignal &c, double t);
     
 /*!  Returns a new pSignal, obtained from <strong>s</strong> via interpolation.
  * New samples are spaced by new_Ts (new sampling period).
  * Linear  interpolation kernel is used.
  */    
     pSignal *ResampleLinear(pSignal &s, Double_t new_Ts);

 /*!  Returns a new pSignal, obtained from <strong>s</strong> via interpolation.
  * New samples are spaced by new_Ts (new sampling period).
  * Bardelli's interpolation kernel is used.
  */    
     pSignal *ResampleCubicBardelli(pSignal &s, Double_t new_Ts);
     
/*!  Returns a new pSignal, obtained from <strong>s</strong> via interpolation.
  * New samples are spaced by new_Ts (new sampling period).
  * Keys's interpolation kernel is used.
  */    
     pSignal *ResampleCubicConvolution(pSignal &s, Double_t new_Ts);
 
/*! Utility function used by ResampleSincConvolution()
  */    
     TGraph* BuildWindowedSincTGraph(double tauclk_orig, int npoints_orig_LR, int noversampling, int window_type);

/*! Utility function used by ResampleSincConvolution()
  */         
     pSignal BuildWindowedSinc(double tauclk_orig, int npoints_orig_LR, int noversampling, int window_type);
   
/*!  Returns a new pSignal, obtained from <strong>s</strong> via sinc interpolation.
  * New samples are spaced by taufinal (new sampling period).
  * Sinc interpolation kernel is used.
  * <ul>
  * <li> s: original pSignal object to be interpolated
  * <li> taufinal: new sampling period (Ts of newly created signal)
  * <li> npoints_LR: number of points in sinc to be considered on each side before truncation
  * <li> window_type: bell shaped function to be used for sinc truncation (0=Barlett, 1=Hanning, 2=Hamming, 3=Blackman)
  * </ul>
  */    
     pSignal * ResampleSincConvolution(pSignal &s, double taufinal, int npoints_LR, int window_type);

/*!  Calculates difference equation coefficients to obtain a RC Low Pass Filter LTI system
 * <ul>
 * <li>f_c: cut off (-3dB) frequency in GHz
 * <li>Ts: sampling period in nanoseconds
 * <li>b: points to the pointer of an array of doubles, used for x[] (input signal) coefficients
 * <li>a: points to the pointer of an array of doubles, used for y[] (output signal) coefficients
 * </ul>
 * a and b can be passed to LTIDifferenceEquation() to implement the system (i.e. to filter a signal).
 * Memory for a[] and b[] will be allocated by the method (risk of memory leaking!!!).
 */ 
      void LTICalcRCLowPassFilter(double f_c, double Ts, double **b, double **a);
      
/*!  Calculates difference equation coefficients to obtain a RC High Pass Filter LTI system
 * <ul>
 * <li>f_c: cut off (-3dB) frequency in GHz
 * <li>Ts: sampling period in nanoseconds
 * <li>b: points to the pointer of an array of doubles, used for x[] (input signal) coefficients
 * <li>a: points to the pointer of an array of doubles, used for y[] (output signal) coefficients
 * </ul>
 * a and b can be passed to LTIDifferenceEquation() to implement the system (i.e. to filter a signal).
 * Memory for a[] and b[] will be allocated by the method (risk of memory leaking!!!).
 */ 
      void LTICalcRCHighPassFilter(double f_c, double Ts, double **b, double **a);
     
     //! allocates and fills coefficients to obtain a CRRC shaper with tau (in ns) from a difference equation
     //! coefficients can then be used with LTIDifferenceEquation()
     void LTICalcCRRCCoeff(double tau, double Ts, double **b, double **a);
     

      /*! allocates and fills coefficients to obtain a pole zero correction filter
     *  with decay constant tau in ns
     * coefficients can then be used with LTIDifferenceEquation(). N.B. the processing
     * will cancel the exponential decay WITHOUT applying integration. If you
     * need a step signal, you have to cascade it with LTIAccumulator().
     */
    void LTICalcPZCorrection(double tau, double Ts, double **b, double **a);

    
    void LTICalcPZCorrection(Long64_t c_pzc, int scaling,Long64_t **b,Long64_t **a);
    
    void LTICalcPZCorrection(double tau, double Ts, int scaling,Long64_t **b,Long64_t **a);
    
     //! allocates and fills coefficients to obtain a CRRC shaper with pole zero correction
     //! with CR=RC=tau (in ns) and decay constant taupzc (in ns) from a difference equation
     //! coefficients can then be used with LTIDifferenceEquation()
     void LTICalcCRRCwPZC(double tau, double taupzc, double Ts, double **b, double **a);

//! implements y[n]=a1*y[n-1]+a2*y[n-2]+...+aN*y[n-N]+ b0*x[n] +b1*x[n-1]+...+bM*x[n-M]
//! allocates a new pSignal(), returns pointer to pSignal() 
     pSignal *LTIDifferenceEquation(pSignal *s, int orderX, int orderY, double *b, double *a);

//! implements y[n]=a1*y[n-1]+a2*y[n-2]+...+aN*y[n-N]+ b0*x[n] +b1*x[n-1]+...+bM*x[n-M]
//! allocates a new pDigitalSignal(), returns pointer to pDigitalSignal()      
     pDigitalSignal *LTIDifferenceEquation(pDigitalSignal *s, int orderX, int orderY, Long64_t *b, Long64_t *a,int ResNbits=-1);
     
//! implements y[n]=y[n-1]+x[n], that is a so called "accumulator",
//!  in other words, the output pSignal is the discrete integral of the input
     pSignal *LTIAccumulator(pSignal *s);

//! implements y[n]=y[n-1]+x[n], that is a so called "accumulator",
//!  in other words, the output pDigitalSignal is the discrete integral of the input
     pDigitalSignal *LTIAccumulator(pDigitalSignal *s,int ResNbits=-1);
     
//! implements y[n]=y[n-1]+x[n], that is a so called "accumulator",
//!  in other words, the output signal is the discrete integral of the input
//!  This version accepts a start parameter, i.e. the index of the sample where integration
//!  should start (to avoid instabilities when integrating long baselines).     
     pSignal *LTIAccumulator(pSignal *s, int start);
     
//! implements y[n]=y[n-1]+1/N(x[n]-x[n-N]-x[n-N-M]+x[n-2N-M]) for pSignal, a system which has
//! a trapezoidal step response with rise time of N samples and flat top of M samples.
//! No pole zero correction is applied.     
    pSignal *LTITrapezoidalShaper(pSignal *s, int Nrise, int Mflat);

//! implements y[n]=y[n-1]+1/N(x[n]-x[n-N]-x[n-N-M]+x[n-2N-M]) for pDigitalSignal, a system which has
//! a trapezoidal step response with rise time of N samples and flat top of M samples.
//! No pole zero correction is applied.         
    pDigitalSignal *LTITrapezoidalShaper(pDigitalSignal *s, int Nrise, int Mflat,int ResNbits=-1);

//! implements y[n]=y[n-1]+1/N(x[n]-x[n-N]-x[n-N-M]+x[n-2N-M])-1/N*p*(x[n-1]-x[n-N-1]-x[n-N-M-1]+x[n-2N-M-1]), 
//! a system which has a trapezoidal  response with rise time of N samples and flat top of M samples
//! when the input is a decaying exponential with time constant of taupzc nanoseconds (p=exp(-Ts/taupzc))
//! In other words, pole zero correction is included.
    pSignal *LTITrapezoidalShaperPZC(pSignal *s, int Nrise, int Mflat, double taupzc, double Ts);

    //check//
//! implements y[n]=y[n-1]+1/N(x[n]-x[n-N]-x[n-N-M]+x[n-2N-M])-1/N*p*(x[n-1]-x[n-N-1]-x[n-N-M-1]+x[n-2N-M-1]), 
//! a system which has a trapezoidal  response with rise time of N samples and flat top of M samples
//! when the input is a decaying exponential with time constant of taupzc nanoseconds (p=exp(-Ts/taupzc))
//! In other words, pole zero correction is included. 
    pDigitalSignal *LTITrapezoidalShaperPZC(pDigitalSignal *s, int Nrise, int Mflat, Long64_t c_pz,int scaling, int ResNbits=-1);

    pDigitalSignal *LTITrapezoidalShaperPZC(pDigitalSignal *s, int Nrise, int Mflat, double taupzc, double Ts,int scaling, int ResNbits=-1);
    
    /*! Implements the convolution sum of two pSignal's pointed at by s1 and s2.
     *  Allocates a new pSignal object and returns a pointer to it.
     */
    pSignal  *LTIConvolutionSum(pSignal *s1, pSignal *s2)
    {
     double y;
     if(s1->GetTs() != s2->GetTs()) printf("pDSP::LTIConvolutionSum():  WARNING!!!! pSignal's have different sampling periods!! Are you sure it's ok??\n");
     
     pSignal *csum = new pSignal(1);
     csum->SetN(s1->GetN()+s2->GetN()-1);
     csum->SetTs(s1->GetTs());
     for(int i=0; i < csum->GetN(); i++){
           y=0;
           for(int j=0; j< s2->GetN();j++)  {
	       if((i-j) >= 0 && (i-j)<s1->GetN())    y+= s2->At(j)*s1->At(i-j);
	   }
           csum->SetAt(y,i);
                
    }
    return csum;
  }

   /*! Implements the convolution sum of two pDigitalSignal's pointed at by s1 and s2.
     *  Allocates a new pDigitalSignal object and returns a pointer to it.
     * ResNbits is the number of bits of the resulting pDigitalSignal, if negative the value is choosen in order to avoid saturation.
     */  
    pDigitalSignal  *LTIConvolutionSum(pDigitalSignal *s1, pDigitalSignal *s2, int ResNbits=-1)
    {
     Long64_t y;
     if(s1->GetTs() != s2->GetTs()) printf("pDSP::LTIConvolutionSum():  WARNING!!!! pSignal's have different sampling periods!! Are you sure it's ok??\n");
     
     pDigitalSignal *csum = new pDigitalSignal(1,s1->GetNbits()+s2->GetNbits());
     if(ResNbits>0) csum->SetNbits(ResNbits);
     csum->SetN(s1->GetN()+s2->GetN()-1);
     csum->SetTs(s1->GetTs());
     for(int i=0; i < csum->GetN(); i++){
           y=0;
           for(int j=0; j< s2->GetN();j++)  {
	       if((i-j) >= 0 && (i-j)<s1->GetN())    y+= s2->At(j)*s1->At(i-j);
	   }
           csum->SetAt(y,i);                
    }
     csum->SaturateSignal();
    return csum;
  }
    
/*! Add to the pSignal s an inverted version of itself, shifted in time by an integer multiple
 * of the sampling period. The delay parameter is the delay value in samples.
 * For delay>0 we get proper delay, for delay<0 subtracted copy is anticipated in time.
 */
    pSignal  *LTIDelayDiff(pSignal &s, int delay)
    {
          pSignal *diff = new pSignal(s);
	  
          for(int i=0; i< diff->GetN(); i++){ 
	if(i-delay>=0 && i-delay < s.GetN()) 
	  diff ->SetAt(s.At(i)-s.At(i-delay),i);
	else if(i-delay<0)
	  diff ->SetAt(s.At(i)-s.At(0),i);
	else if(i-delay>= s.GetN())
	  diff ->SetAt(s.At(i)-s.At(s.GetN()-1),i);
	  
          }
          return diff;
          
    }

  /*! Add to the pDigitalSignal s an inverted version of itself, shifted in time by delay.
 * For delay>0 we get proper delay, for delay<0 subtracted copy is anticipated in time.
 */  
    pDigitalSignal  *LTIDelayDiff(pDigitalSignal &s, int delay)
    {
      pDigitalSignal *diff = new pDigitalSignal(s);
      diff->SetNbits(s.GetNbits()+1);
      diff->SetSigned(1);
      for(int i=0; i< diff->GetN(); i++){ 
	if(i-delay>=0 && i-delay < s.GetN()) 
	  diff ->SetAt(s.At(i)-s.At(i-delay),i);
	else if(i-delay<0)
	  diff ->SetAt(s.At(i)-s.At(0),i);
	else if(i-delay>= s.GetN())
	  diff ->SetAt(s.At(i)-s.At(s.GetN()-1),i);
	
      }
      diff->SaturateSignal();
      return diff;
      
    }
    
    /*! A causal (for M>0) moving average is performed on the input signal.
     * A pointer to the resulting pSignal is returned. Difference equation: 
     * y [n] = y [n − 1] + 1/M* (x[n] − x[n − M])
 
 */
    pSignal  *LTIMovingAverage(pSignal &s, int M)
    {
      pSignal *ma = new pSignal(s);
      
      for(int n=0; n< ma->GetN(); n++){
	int j = n - M;
	double y_nm1, x_n, x_j;
	x_n = s.At(n);
	if(j<0) x_j = s.At(0);
	else if(j>=s.GetN()) x_j = s.At(s.GetN()-1);
	else x_j = s.At(j);
	
	if(n<1) y_nm1 = 0;
	else y_nm1 = ma->At(n-1);
	
	ma->SetAt( y_nm1+(x_n-x_j)/M , n);
	
      }
      return ma;
      
    }

  
     /*! A causal (for M>0) moving average is performed on the input signal.
     * A pointer to the resulting pDigitalSignal is returned. Difference equation: 
     * y [n] = y [n − 1] + 1/M* (x[n] − x[n − M])
 
 */
    
    pDigitalSignal  *LTIMovingSum(pDigitalSignal &s, int M,int ResNbits=-1)
    {
      pDigitalSignal *ma = new pDigitalSignal(s);
      int newbits=s.GetNbits()+(int)ceil(log(M)/log(2));
      ma->SetNbits(newbits);
      if(ResNbits>0)  ma->SetNbits(ResNbits);
      
      for(int n=0; n< ma->GetN(); n++){
	int j = n - M;
	Long64_t y_nm1, x_n, x_j;
	x_n = s.At(n);
	if(j<0) x_j = 0;
	else if(j>=s.GetN()) x_j = s.At(s.GetN()-1);
	else x_j = s.At(j);
	
	if(n<1) y_nm1 = 0;
	else y_nm1 = ma->At(n-1);
	
	ma->SetAt( y_nm1+(x_n-x_j) , n);
	
      }
      ma->SaturateSignal();
      return ma;
      
    }
    
    /*! Produces a pSignal of length nsamples, containing the autocorrelation 
     * sequence of the given pSignal. The autocorrelation is calculated as a mean
     * in time domain (thus assuming a stationary process).
     */
    pSignal *AutoCorrelation(pSignal &s, int nsamples);
    
    
    /*! Produces a pSignal of length nsamples, containing the autocorrelation 
     * sequence of the given pSignal. The autocorrelation is calculated exploiting
     * the FFT, i.e. as the inverse FFT of the Magnitude of the FFT of the signal.
     */
    pSignal  *FFTAutocorr(pSignal &s);
    //Cubic Spline interpolation using LTI filter[Unser93]
    
    //! Allocates and fills coefficients to obtain a LTI filter to perform cubic spline interpolation on a pSignal.
    //! If nbits>2 the coefficients are quantized using a bit for the sign, one for the integer part and the remainder for the fractional part.
    //! The method returns N, the number of calculated coefficients is 2N+1. The coefficients can then be used with LTIDifferenceEquation()
    int LTICalcCubicSplineFilter(int nbits,double **a);
    
    /*! Utility function used by ResampleCubicSplines() and ResampleCubicSplinesDerivative().
  */   
    pSignal *BuildCubicSplineCoeffSignal(int nbits,pSignal *in);

     /*! Returns the interpolated value at time <strong>t</strong>, obtained by the cubic spline coefficient signal <strong>coeff</strong>.
      * To get interpolated value of a pSignal s, coefficients must be calculated in advance using  BuildCubicSplineCoeffSignal() or BuildSmoothingSplineCoeffSignal().
      * Uses standard cubic spline kernel for reconstruction.
      */  
    static double GetDataInterCubicSplines(pSignal &coeff, double t);

     /*! Returns the interpolated value at time <strong>t</strong>, obtained by the cubic spline coefficient signal <strong>coeff</strong>.
      * To get interpolated value of a pSignal s, coefficients must be calculated in advance using BuildCubicSplineCoeffSignal() or BuildSmoothingSplineCoeffSignal().
      * Uses derivative cubic spline kernel for reconstruction.
      */  
     static double GetDataInterCubicSplinesDerivative(pSignal &coeff, double t);

     /*!  Returns a new pSignal, obtained from <strong>s</strong> via interpolation.
      * New samples are spaced by new_Ts (new sampling period).
      * Cubic Spline interpolation is used.
      */    
    pSignal *ResampleCubicSplines(pSignal &s,Double_t new_Ts, int nbits=-1);

     /*!  Returns a new pSignal, obtained from <strong>s</strong> via interpolation and differentiation.
      * New samples are spaced by new_Ts (new sampling period).
      * Cubic Spline interpolation is used.
      * Can also include a PoleZeroCorrection if <strong>tauPZC</strong>>0.
      */ 
    pSignal *ResampleCubicSplinesDerivative(pSignal &s,Double_t new_Ts, Double_t tauPZC, int nbits=-1);
    //Smoothing Spline Interpolation using LTI filter[Unser93];

    //! Allocates and fills coefficients to obtain a LTI filter to perform smoothing spline interpolation with smoothing parameter l.
    //! If nbits>2 the coefficients are quantized using a bit for the sign, one for the integer part and the remainder for the fractional part.
    //! The method returns N, the number of calculated coefficients is 2N+1. The coefficients can then be used with LTIDifferenceEquation() (pSignal only).
    //! If the determination of the coefficients fails, the method returns -1 to signal the error.
    int LTICalcSmoothingSplineFilter(int nbits,double l,double **a);

     /*! Utility function used by ResampleSmoothingSplines() and ResampleSmoothingSplinesDerivative().
  */   
    pSignal *BuildSmoothingSplineCoeffSignal(int nbits,double l,pSignal *in);

     /*!  Returns a new pSignal, obtained from <strong>s</strong> via interpolation.
      * New samples are spaced by new_Ts (new sampling period).
      * Smoothing Spline interpolation is used.
      */    
    pSignal *ResampleSmoothingSplines(pSignal &s,Double_t new_Ts,double l,int nbits=-1);

     /*!  Returns a new pSignal, obtained from <strong>s</strong> via interpolation and differentiation.
      * New samples are spaced by new_Ts (new sampling period).
      * Smoothing Spline interpolation is used.
      * Can also include a PoleZeroCorrection if <strong>tauPZC</strong>>0.
      */ 
    pSignal *ResampleSmoothingSplinesDerivative(pSignal &s,Double_t new_Ts, Double_t tauPZC,double l,int nbits=-1);

    //******************************************************************************************************//
    //******************************************************************************************************//
    //******************************************************************************************************//
    
    ///Splines interpolation->pDigitalSignal/
      //! Allocates and fills coefficients to obtain a LTI filter to perform cubic spline interpolation on a pDigitalSignal.
    //! If nbits>2 the coefficients are quantized using a bit for the sign, one for the integer part and the remainder for the fractional part and expressed in Integer notation.
    //! The method returns N, the number of calculated coefficients is 2N+1. The coefficients can then be used with LTIDifferenceEquation() on a pDigitalSignal object.
    int LTICalcCubicSplineFilter(int nbits,Long64_t **a);
    
    /*! Utility function used by ResampleCubicSplines() and ResampleCubicSplinesDerivative() with pDigitalSignal.
  */   
    pDigitalSignal *BuildCubicSplineCoeffSignal(int nbits,pDigitalSignal *in, int ResNbits=-1);

 /*! Returns the interpolated value at time <strong>t</strong>, obtained by the cubic spline coefficient signal <strong>coeff</strong>.
      * To get interpolated value of a pDigitalSignal s, coefficients must be calculated in advance using  BuildCubicSplineCoeffSignal() or BuildSmoothingSplineCoeffSignal().
      * Uses standard cubic spline kernel for reconstruction.
      * nbits_rec is the number of bits used to represent the coefficients.
      */  
     static Long64_t GetDataInterCubicSplines(pDigitalSignal &coeff, int nbits_rec, double t);

     /*! Returns the interpolated value at time <strong>t</strong>, obtained by the cubic spline coefficient signal <strong>coeff</strong>.
      * To get interpolated value of a pDigitalSignal s, coefficients must be calculated in advance using  BuildCubicSplineCoeffSignal() or BuildSmoothingSplineCoeffSignal().
      * Uses derivative cubic spline kernel for reconstruction.
      * nbits_rec is the number of bits used to represent the coefficients.
      */  
     static Long64_t GetDataInterCubicSplinesDerivative(pDigitalSignal &coeff, int nbits_rec, double t);
     /*! Returns the interpolated value at time <strong>t</strong>, obtained by the cubic spline coefficient signal <strong>coeff</strong>.
      * To get interpolated value of a pDigitalSignal s, coefficients must be calculated in advance using  BuildCubicSplineCoeffSignal() or BuildSmoothingSplineCoeffSignal().
      * Uses derivative cubic spline kernel with PZC for reconstruction.
      * nbits_rec is the number of bits used to represent the coefficients.
      */  
     
    static Long64_t GetDataInterCubicSplinesDerivativePZC(pDigitalSignal &coeff, int nbits_rec, Double_t tauPZC, double t);

     /*!  Returns a new pDigitalSignal, obtained from <strong>s</strong> via interpolation.
      * New samples are spaced by new_Ts (new sampling period)
      * Cubic Spline interpolation is used.
      */    
    pDigitalSignal *ResampleCubicSplines(pDigitalSignal &s,Double_t new_Ts, int nbits_filter, int nbits_coeff, int nbits_rec, int ResNbits=-1);

     /*!  Returns a new pDigitalSignal, obtained from <strong>s</strong> via interpolation and differentiation.
      * New samples are spaced by new_Ts (new sampling period).
      * Cubic Spline interpolation is used.
      * Can also include a Pole Zero Correction if <strong>tauPZC</strong>>0.
      */ 
    pDigitalSignal *ResampleCubicSplinesDerivative(pDigitalSignal &s,Double_t new_Ts, Double_t tauPZC, int nbits_filter, int nbits_coeff, int nbits_rec, int ResNbits=-1);
    //Smoothing Spline Interpolation using LTI filter[Unser93];

    //! Allocates and fills coefficients to obtain a LTI filter to perform smoothing spline interpolation with smoothing parameter l on a pDigitalSignal.
    //! If nbits>2 the coefficients are quantized using a bit for the sign, one for the integer part and the remainder for the fractional part.
    //! The method returns N, the number of calculated coefficients is 2N+1. The coefficients can then be used with LTIDifferenceEquation().
    //! If the determination of the coefficients fails, the method returns -1 to signal the error.
    int LTICalcSmoothingSplineFilter(int nbits,double l,Long64_t **a);

     /*! Utility function used by ResampleSmoothingSplines() and ResampleSmoothingSplinesDerivative().
  */   
    pDigitalSignal *BuildSmoothingSplineCoeffSignal(int nbits,double l,pDigitalSignal *in,int ResNbits=-1);

     /*!  Returns a new pDigitalSignal, obtained from <strong>s</strong> via interpolation.
      * New samples are spaced by new_Ts (new sampling period).
      * Smoothing Spline interpolation is used.
      */    
    pDigitalSignal *ResampleSmoothingSplines(pDigitalSignal &s,Double_t new_Ts,double l, int nbits_filter, int nbits_coeff, int nbits_rec, int ResNbits=-1);

     /*!  Returns a new pDigitalSignal, obtained from <strong>s</strong> via interpolation and differentiation.
      * New samples are spaced by new_Ts (new sampling period).
      * Smoothing Spline interpolation is used.
      * Can also include a PoleZeroCorrection if <strong>tauPZC</strong>>0.
      */ 
    pDigitalSignal *ResampleSmoothingSplinesDerivative(pDigitalSignal &s,Double_t new_Ts, Double_t tau_PZC,double l, int nbits_filter, int nbits_coeff, int nbits_rec, int ResNbits=-1);


/*! <p>Delay based on sinc interpolation (can handle a delay value which is not a multiple of the
    *  sampling period).</p> 
    * <p>Samples are recalculated on reconstructed signal. For reconstruction,
    * sinc interpolation is used (exact if Shannon conditions are satisfied, otherwise only approx.).</p>
    * <p> tshift is the delay in nanoseconds.</p>
    * For details, see Oppenheim-Schaffer "Discrete-Time signal processing" Example 4.7.
    * To time-shift a signal by an integer multiple of Ts, use SampleDelay() instead. 
    */
   pSignal *Delay(pSignal &s, double tshift);

/*! <p>Delay based on sinc interpolation (can handle a delay value which is not a multiple of the
    *  sampling period).</p> 
    * <p>Samples are recalculated on reconstructed signal. For reconstruction,
    * a windowed sinc kernel of length N is used.</p>
    * <p> tshift is the delay in nanoseconds.</p>
    * For details, see Oppenheim-Schaffer "Discrete-Time signal processing" Example 4.7.
    * To time-shift a signal by an integer multiple of Ts, use SampleDelay() instead. 
    */
   pSignal *Delay(pSignal &s, double tshift, int N);
   
/*! <p>Returns a pSignal as produced in ARC-CFD timing: the original signal is delayed, amplified by 
    * 1/fraction and subtracted to itself to produce a zero-crossing.</p>
    *  <p> The delay is based on sinc interpolation and it can handle a delay value which is not a multiple of the
    *  sampling period. Give the delay value in nanoseconds!</p> 
    *  <p> fraction must be in the (0,1) interval.
    */

   pSignal *BuildSincARCSignal(pSignal &s, double delay, double fraction);

   /*! <p>Returns a pSignal as produced in ARC-CFD timing: the original signal is delayed, amplified by 
    * 1/fraction and subtracted to itself to produce a zero-crossing.</p>
    *  <p> The delay is based on windowed sinc interpolation of length N and it can handle a delay value which is not a multiple of the
    *  sampling period. Give the delay value in nanoseconds!</p> 
    *  <p> fraction must be in the (0,1) interval.</p>
     * <p>begin, end: ARC signal will be calculated only for samples in the [begin, end] interval.</p>

    */

   pSignal *BuildSincARCSignal(pSignal &s, double delay, double fraction,int N, int begin, int end);

 /*! <p>Returns a pSignal as produced in ARC-CFD timing: the original signal is delayed, amplified by 
    * 1/fraction and subtracted to itself to produce a zero-crossing.</p>
    *  <p> The delay is based on cubic interpolation and it can handle a delay value which is not a multiple of the
    *  sampling period. Give the delay value in nanoseconds!</p> 
    *  <p> fraction must be in the (0,1) interval.</p>
     * <p>maxt: length in nanoseconds of the produced signal.</p>

    */
   pSignal *BuildCubicARCSignal(pSignal &s, double delay, double fraction,  double maxt);


      /*! <p>Returns a pSignal as produced in ARC-CFD timing: the original signal is delayed, amplified by 
    * 1/fraction and subtracted to itself to produce a zero-crossing.</p>
    *  <p> The delay must be given in samples, so the actual delay in ns is an integer multiple of the
    *  sampling period.</p> 
    *  <p> fraction must be in the (0,1) interval.
     * <p>begin, end: ARC signal will be calculated only for samples in the [begin, end] interval.</p>
    */

   pSignal *BuildARCSignal(pSignal &s, int delay, double fraction, int begin, int end);

    
 ClassDef(pDSP,2);
};



#endif

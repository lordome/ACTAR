
#ifndef p_CFD
#define p_CFD

#include <TObject.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TGClient.h>
#include <TVectorD.h>
#include <TFile.h>
#include <TBenchmark.h>
#include <TDecompLU.h>

#include "pUtility.h"
#include "pSignal.h"
#include "pDSP.h"

// CFD type defines: (actually defined in pSignal.h!)
/*#define LINEAR_CFD  0
#define CUBIC_CFD  1
#define CUBICCONV_CFD 2
#define CUBICSPLINE_CFD 3
#define PENTA_CFD  4
#define SMOOTHINGSPLINE_CFD 5*/

#define CFD_CUBIC 1
#define CFD_CUBICCONV 2
#define CFD_CUBICSPLINE 3
#define CFD_PENTA 4
#define CFD_SMSPLINE 5

#define CFD_FORWARD 0
#define CFD_REVERSE 1

#define POLARITY_POS  1
#define POLARITY_NEG  -1

using namespace std;


#define DEBUG
/*! \brief Class performing Constant Fraction Discrimination (CFD) on pSignal
using different interpolation techniques:
<ul>
<li> Linear
<li> Cubic Convolution (Keys)
<li> Cubic (Bardelli)
<li> Cubic Splines
<li> Smoothing Splines
<li> Pentic
</ul>
*/
class pCFD: public TObject{

 public:
  //class constructors
  pCFD();
  virtual ~pCFD(){};
  //threshold crossing using newton method, with different interpolation

  
  /*!  \brief  Returns the time value (with origin t=0 at the first sample) for which
 * signal = fmax (threshold crossing), reconstructing the continuous signal via linear interpolation.
 * <ul>
 * <li>s: input signal to be interpolated
 * <li>x2: index of sample immediately before threshold crossing (must be determined in advance, before calling this method)
 *<li> fmax: threshold value
 * </ul>
 */     
  double LinearInterpolation(pSignal *s,int x2, double fmax);

/*!  \brief  Returns the time value (with origin t=0 at the first sample) for which
 * signal = fmax (threshold crossing), reconstructing the continuous signal via cubic interpolation (Key's
 * kernel, see: R.G.Keys, Cubic Convolution Interpolation for Digital Image Processing, IEEE Trans. Acoust., Speech,
 * Signal Processing, vol. ASSP-29, Dec. 1981) and solving the 3rd order equation using Newton method
 * of successive approximations (the crossing obtained via linear interpolation is used as first guess).  Key's interpolation is implemented via the equivalent cubic polynomial
 * which is used to interpolate between the two samples, one before and one after the crossing.
 * Four samples are used to determine the coefficients of the polynomial. 
 * <ul>
 * <li>s: input signal to be interpolated
 * <li>x2: index of sample immediately before threshold crossing (must be determined in advance, before calling this method)
 *<li> fmax: threshold value
 * <li> Nrecurr: number of iterations for recursive solution
 * </ul>
 */     
  double CubicConvInterpolation(pSignal *s,int x2, double fmax, int Nrecurr);
     
 /*!  \brief  Returns the time value (with origin t=0 at the first sample) for which
 * signal = fmax (threshold crossing), reconstructing the continuous signal via cubic interpolation (Bardelli's
 * kernel) and solving the 3rd order equation using Newton method
 * of successive approximations (the crossing obtained via linear interpolation is used as first guess). 
 * This particular interpolation is equivalent to finding the threshold crossing of 
 * the unique cubic passing through four samples, two before and two after the crossing.
 *  These four samples are used to determine the coefficients of the polynomial. 
 * <ul>
 * <li>s: input signal to be interpolated
 * <li>x2: index of sample immediately before threshold crossing (must be determined in advance, before calling this method)
 *<li> fmax: threshold value
 * <li> Nrecurr: number of iterations for recursive solution
 * </ul>
 */     
  double CubicInterpolation(pSignal *s,int x2, double fmax, int Nrecurr);

     /*!  \brief  Returns the time value (with origin t=0 at the first sample) for which
 * signal = fmax (threshold crossing), reconstructing the continuous signal from cubic Spline coefficients,
 * obtained from the original signal using an LTI filter described by Unser
 * (see: Unser et al., B-Spline Signal Processing: Part I-Theory,  IEEE Trans. Signal Processing,
 * vol.  TSP-41, N 2 , Feb. 1993)  and solving the 3rd order equation using Newton method
 * of successive approximations. See article for details.
 * The coefficients can be obtained from the signal using either pDSP::BuildCubicSplineCoeffSignal() or pDSP::BuildSmoothingSplineCoeffSignal().
 * <ul>
 * <li>coeff: coefficient sequence for cubic splines expansion of the signal
 * <li>x2: index of sample immediately before threshold crossing (must be determined in advance, before calling this method)
 *<li> fmax: threshold value
 * <li> Nrecurr: number of iterations for recursive solution
 * </ul>
 */     
  double CubicSplineInterpolation(pSignal *coeff,int x2, double fmax, int Nrecurr);

    /*!  \brief  Returns the time value (with origin t=0 at the first sample) for which
 * signal = fmax (threshold crossing), reconstructing the continuous signal via 5th order interpolation
 * and solving the 5th order equation using Newton method
 * of successive approximations   (the crossing obtained via linear interpolation is used as first guess). 
 * This particular interpolation is equivalent to finding the threshold crossing of 
 * the unique 5th order polynomial passing through six samples, three before and three after the crossing.
 *  These six samples are used to determine the coefficients of the polynomial. 
 * <ul>
 * <li>s: input signal to be interpolated
 * <li>x2: index of sample immediately before threshold crossing (must be determined in advance, before calling this method)
 *<li> fmax: threshold value
 * <li> Nrecurr: number of iterations for recursive solution
 * </ul>
 */
  double PentaInterpolation(pSignal *s,int x3, double fmax, int Nrecurr);

     /*!  \brief  General method to calculate Constant Fraction Discriminator (CFD) timing.
 * The desired fraction and the signal maximum amplitude must be passed to the method.
 * <ul>
 * <li> s: input signal for CFD  
 * <li> fraction: CFD fraction (0&lt;fraction&lt;1) 
 * <li> Max: signal maximum (threshold=fraction*Max)
 * <li> polarity: if >= 0 signal assumed of positive polarity, if <0 negative polarity is assumed 
 * <li> type: interpolation  type (0=linear, 1=Cubic Bardelli, 2=Cubic Key, 3=Cubic Spline, 4=5th order, 5=Smoothing Spline)
 * <li> Nrecurr: number of iterations (if =0 it is equivalent to linear interpolation)
 * <li> reverse: if =0 the first crossing from the beginning of the signal is calculated, if =1 search goes
 *        backward in time starting at start)
 * <li> start: search start time 
 * <li> nbits: bits used for filter quantization if Cubic Spline or Smoothing Spline is used
 * <li> l: Smoothing parameter for Smoothing Spline CFD
 * </ul>
 */
  double CFD(pSignal *s,double fraction, double Max, int polarity, int type, int Nrecurr, int reverse=0, double start=0, int nbits=-1,double l=1);


     /*!  \brief  General method to calculate Amplitude-Rise-Time Compensated (ARC)  timing.
 * The desired fraction and the delay in ns must be passed to the method.
 * <ul>
 * <li> s: input signal for ARC  
 * <li> threshold: amplitude value to be trespassed (zero crossing is searched while going backwards from threshold crossing towards the beginning of the signal)
 * <li> fraction: ARC fraction (0&lt;fraction&lt;1) 
 * <li> polarity: if >= 0 signal assumed of positive polarity, if <0 negative polarity is assumed 
 * <li> type: interpolation  type (0=linear, 1=Cubic Bardelli, 2=Cubic Key, 3=Cubic Spline, 4=5th order)
 * <li> Nrecurr: number of iterations (if =0 it is equivalent to linear interpolation)
 * <li> start: search start time in ns 
 * <li> length: half length of sinc kernel used to compute the delayed signal
 * <li> begin, end: ARC signal will be calculated only for samples in the [begin, end] interval.
 * </ul>
 */

  double ARC(pSignal *s,double threshold, double fraction, double delay, int polarity, int type, int Nrecurr, double start, int length, int begin, int end);

  /*! \brief Calls the general ARC function calculating ARC signal from start to end of the original signal */
  double ARC(pSignal *s,double threshold, double fraction, double delay, int polarity, int type, int Nrecurr, double start, int length){
    return ARC(s, threshold, fraction, delay, polarity, type, Nrecurr, start, length, 0, s->GetN());
  }
  
  /*! \brief ARC-CFD exploiting a cubic fractional delay FIR filter. Copied from Bardelli's FSignal::FindTzeroArcCFD() 
   * <ul>
   * <li> s: pointer to original signal 
   * <li> delay: delay in ns (not necessarily an integer multiple of sampling period  
   * <li> fraction: fraction between 0 and 1
   * <li> maxt: length of zero crossing signal
   * </ul>
   */
  double ARCCubic(pSignal *s, double delay, double fraction, double maxt);

  /*! \brief ARC-CFD exploiting a cubic fractional delay FIR filter. Copied from Bardelli's FSignal::FindTzeroArcCFD() 
   * <ul>
   * <li> s: pointer to original signal 
   * <li> delay: delay in ns (not necessarily an integer multiple of sampling period  
   * <li> fraction: fraction between 0 and 1
   * <li> maxt: length of zero crossing signal
   * </ul>
   */

/*!  \brief  standard forward-in-time Cubic CFD with 3 iterations, positive polarity */
 double PosCubicCFD(pSignal *s,double fraction, double Max){
   return CFD(s,fraction, Max, POLARITY_POS, CUBIC_CFD,  3, 0, 0);
 }
 
 /*! \brief standard forward-in-time Cubic CFD with 3 iterations, negative polarity */
 double NegCubicCFD(pSignal *s,double fraction, double Max){
   return CFD(s, fraction, Max, POLARITY_NEG, CUBIC_CFD,  3, 0, 0);
 }

  static TVectorD RFSyncBuildWeightsWhiteNoise(double tau_clk, int Nsamples, double rfperiod_ns, int build_cos, int Npoly, int Narmo);
  static TVectorD RFSyncBuildWeights(pSignal *R,int Nsamples, double rfperiod_ns, int build_cos, int Npoly, int Narmo);
  static double   RFSyncComputeT0(pSignal *s, int Nstart, TVectorD *wcoseno, TVectorD *wseno,double rfperiod_ns);
  

  ClassDef(pCFD,2);
};

#endif

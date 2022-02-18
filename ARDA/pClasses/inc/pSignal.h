
#ifndef p_Signal
#define p_Signal 

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <TObject.h>
#include <TGraph.h>
#include <TF1.h>
#include <TH1F.h>
#include <TMath.h>
#include <TRandom.h>
#include <TArrayD.h>
#include <TAxis.h>
#include <TMatrixF.h>
#include <TMatrixD.h>


// CFD type defines:
#define LINEAR_CFD  0
#define CUBIC_CFD  1
#define CUBICCONV_CFD 2
#define CUBICSPLINE_CFD 3
#define PENTA_CFD  4
#define SMOOTHINGSPLINE_CFD 5

using namespace std;
// Basic classes:
// pSignal: samples storage and simple operations on samples, signal generation
//             (sinusoidal, preamp, PMT signals...)
// pADC: A/D conversion simulation
// pFT: contains a Fourier Tranform
// pDSP: handles Fourier transforms, Z-Tranforms, Filters, various LTI system operations, convolution, recursive difference
//                    equations....

#define DEBUG
/*! \brief Class modelling a digitized signal
 * Inherits from TArrayD(), samples are stored as doubles (floating point values)
 */
class pSignal: public TObject, public TArrayD{

 public: 
// class constructors
//! creates a zero length pSignal with standard values of parameters (as fixed by Init())
     pSignal();
     //! creates a pSignal of length N samples, with standard values of other parameters (see Init())
     pSignal(int N);
     //! creates a pSignal which is a copy of pSignal a
     pSignal(const pSignal &a);
     //! creates a pSignal from a TArrayD
     pSignal(const TArrayD &a);
     //! inits internal variables to standard values
     void Init();     
     //! class destructor
     virtual ~pSignal(){}
     
     // signal generation
    void BuildSine(double amplitude, double freq, double toff);
   
   // graphical utilities

   //! returns a TGraph object which is a graphical representation of the signal
    //! useful to do some more graphic tricks than allowed by pSignal class
    //! tshift allows for shifting the samples in time (useful to get negative values
    //!  of times for plots).
    TGraph  *Sig2Graph(double tshift);

   //! returns a TGraph object which is a graphical representation of the signal
    //! useful to do some more graphic tricks than allowed by pSignal class.
    TGraph  *Sig2Graph();
    
    //! returns a TH1F histogram which is a graphical representation of the signal
    TH1F  *Sig2Histo();
    
    //! draw the pSignal either as TGraph or as TH1F depending on option
    /*! <ul>
     * <li> if option contains "histo" a TH1F will be created and  plotted
     * <li> otherwise a TGraph will be created and  plotted
     * </ul>
     * e.g. :
     \code
       s->Draw("ALP");
       \endcode
       will plot the pSignal as a TGraph with markers, lines between markers, also drawing axis.
     */  
    void Draw(Option_t *option);

//    // getters
//     //! get the pointer to the TArrayD object actually storing the samples
//     TArrayD *GetArray() {return this;}
    
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
     
     //! get time-axis units ("samples" or "ns")
     TString *GetUnits(){return units;}
     
     //! get line color
     int GetLineColor(){return lcol;} 

     //! get marker color
     int GetMarkerColor(){return mcol;}

    //! get marker style
     int GetMarkerStyle(){return mstyle;}

     //! get fractional flag (if =1, samples where taken with ADC in fractional mode, i.e. producing values in the -1,+1 range
     int GetFractional(){return fractional;}
     
  // setters
  //! set signal length in samples
    int SetN(int value) {nsamples = value; this->Set(value); return nsamples;}

    //! set signal resolution in bits (useful when signal obtained from ADC)
    int SetNbits(int value) {Nbits = value; return Nbits;}

    //! sets signal sampling period in ns (used, e.g., for X axis calculations in physical units).
    //! Sampling frequency is updated accordingly
    double SetTs(double value) {Ts=value; Fs=1.0/Ts; return Ts;}
    
    //! set time axis units: if "samples" the abscissa reports the sample index;
    //! if "ns" the abscissa is the sample index times the sampling period (physical time)
        TString *SetUnits(const char *val){units->Clear(); units->Append(val); return units; }

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
    double CubicSplineInterpolation(int x2, double fmax, int Nrecurr);

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
 double CFD(double fraction, double Max, int polarity, int type, int Nrecurr, int reverse=0, double start=0);

 /*! \brief calculates integral between tstart and tstop */
 double  TimeIntegral(double tstart, double tstop);

 
 /*!  \brief  standard forward-in-time Cubic CFD with 3 iterations, positive polarity */
 double PosCubicCFD(double fraction, double Max){
   return CFD(fraction, Max, 1, CUBIC_CFD,  3, 0, 0);
 }
 
 /*! \brief standard forward-in-time Cubic CFD with 3 iterations, negative polarity */
 double NegCubicCFD(double fraction, double Max){
   return CFD(fraction, Max, -1, CUBIC_CFD,  3, 0, 0);
 }

 
    /*! \brief produces a pSignal which is the difference (sample by sample) between the  original pSignal and the TF1 function pointed by f.
     * Useful when "f" points to
    * the fitted function and you want to calculate fit residuals (nomen omen!)*/   
    pSignal *Residuals(TF1 *f);

    
    //  variable calculations
   
  //! \brief looks for maximum or minimum value (according to maxmin flag), writes value into val, returns sample number
  int MaxMin(Double_t *val, int maxmin){
    int i, index=-1;
    Double_t val_here;
    
    if(maxmin==1) val_here = -1e10; 
    if(maxmin==0) val_here = 1e10; 
    for(i=0; i<  GetN(); i++){
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
  void AddConstant(double value){
      for(int i=0; i<nsamples;i++) {
	    this->SetAt(this->At(i)+value, i);  
	
      }
  }

  
  //!  \brief  add  a gaussian random number to each sample
  
void AddRandom(double sigma_noise)
{
  for(int i=0;i<nsamples;i++)
    this->SetAt( this->At(i)+gRandom->Gaus(0,sigma_noise),i);
}


 
  //!  \brief  add two signals sample by sample
  pSignal *Add(pSignal &add){
    Int_t N = nsamples;
    if(add.GetN()<nsamples) N = add.GetN();
      for(int i=0; i<N;i++) {
	this->SetAt(this->At(i)+add.At(i), i);  
	
      }
      return this;
  }
  
  //!  \brief  subtract two signals sample by sample
  pSignal *Subtract(pSignal &sub){
    Int_t N = nsamples;
    if(sub.GetN()<nsamples) N = sub.GetN();
      for(int i=0; i<N;i++) {
	this->SetAt(this->At(i)-sub.At(i), i);  
	
      }
      return this;
  }

//!   \brief  scales all samples by the same factor (value)
    void Scale(double value){
      for(int i=0; i<nsamples;i++) {
	    this->SetAt(this->At(i)*value, i);  
	
      }
  }


  // ============  operators ==================
  //! \brief  Removes from pSignal a portion [start, end) where start and end are the indexes of the first and last samples to be removed.
    /*! Returns a pointer to the pSignal itself. The original pSignal is shortened accordingly.*/
    pSignal *removeFromSignal(int start, int end);
    
    //! \brief  Copies from pSignal a portion [start, end) where start and end are the indexes of the first and last samples to be copied.
    /*! Returns a pointer to a new pSignal, containing the copied portion of the original pSignal. The original pSignal is
     unchanged.*/
    pSignal *copyFromSignal(int start, int end);

    //!  \brief Cuts from pSignal a portion [start, end) where start and end are the indexes of the first and last samples to be cut.
    /*!Returns a pointer to a new pSignal, containing the cut portion of the original pSignal. The original pSignal is
     shortened accordingly.*/
    pSignal *cutFromSignal(int start, int end);

    //! \brief Inserts into a pSignal another pSignal, pointed at by sig.<br>
    /*!  start: index of the sample where the insertion starts.<br>
     Returns a pointer to the pSignal itself, lengthened as needed.<br>
     N.B.: the pSignal pointed at by sig does not need to have the same resolution, sampling rate, etc. as the pSignal where you intend to insert it!*/
    pSignal *pasteToSignal(int start, pSignal *sig);

    
    
    
  // ============  operators ==================
  //!  \brief  to copy one signal to another using = sign
    pSignal operator  = (const pSignal &);

    
    
    //============= Input/Output ================

    //!  \brief  save in ASCII format, suitable for gnuplot etc.
    //!  if savex!=0 sample time is saved in the first column
    //! otherwise only sample value is saved.
void  SaveAsASCII(char *filename, int savex=0);

    //!  \brief  save in SPE format, suitable for pewspop (data analysis program by G.Poggi)
 void  SaveAsSPE(char *filename);

  //!  \brief  routine borrowed from FClasses, by Bardelli, to acquire signals from
//! Tektronix scopes
static pSignal TekAcquireISF(int Nsig, int ch, const char *outputdir, const char *ip);

  //! \brief  routine borrowed from FClasses, by Bardelli, to acquire signals from
//! Tektronix scopes
int ReadBinTek(const char *fname) /* ISF tek format */;


protected:

 double Ts;   // sampling period in ns
 double Fs;   // sampling frequency in GHz
 double range; // peak-to-peak range, default +- 1V
 int Nbits;  // number of bits for quantization
 int fractional; // set to 1 for fractional (i.e. abs(sample)<1) output 
 int nsamples;  // number of samples in pSignal
 // graphic parameters
 int mstyle;
 int mcol;
 int lcol;
 TString *units;
 // TArrayD *s; //->
 
 ClassDef(pSignal,3);
};
#endif

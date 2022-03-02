#ifndef p_SplineArcSignal
#define p_SplineArcSignal

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
#include "pSignal.h"
#include "pDSP.h"

using namespace std;

class pSplineArcSignal: public TObject{
	
	public :    
	 pSplineArcSignal();
     //! creates a pSplineArcSignal of length N samples, with standard values of other parameters (see Init())
     pSplineArcSignal(int N);
     //! creates a pSplineArcSignal which is a copy of pSplineArcSignal a
     pSplineArcSignal(const pSplineArcSignal &a);
     //!creates a pSplineArcSignal from interpolation of a pSignal
     pSplineArcSignal(pSignal a,double delay1,double delay2, double ampl,int nbits=-1,int do_smoothing=0,double l=1);
     
     //! inits internal variables to standard values
     void Init();     
     //! class destructor
     virtual ~pSplineArcSignal(){}
      
      TGraph  *Sig2Graph(int);
    
	 //! returns a TH1F histogram which is a graphical representation of the signal
     TH1F  *Sig2Histo(int);
     void Draw(int Ups,Option_t *option);
     //getters
      //! get  length in samples
     int GetN() {return nsamples;}
     
     //! gets  sampling period in ns
     double GetTs() {return Ts;}
     
     //! get sampling frequency in GHz
    double GetFs() {return Fs;}
    
      //! get line color
     int GetLineColor(){return lcol;} 

     //! get marker color
     int GetMarkerColor(){return mcol;}

    //! get marker style
     int GetMarkerStyle(){return mstyle;}
     
    //Setters 
      //! sets line color for plots
    int SetLineColor(int val){lcol=val; return lcol;} 
    //! sets marker color for plots
    int SetMarkerColor(int val){mcol=val; return mcol;}
    //! sets marker style for plots
    int SetMarkerStyle(int val){mstyle=val; return mstyle;}
    
     //! set signal length in samples
    int SetN(int value) {
		nsamples = value; 
		//this->Set(value); 
		int i;
		for(i=0;i<4;i++) coeffs[i].SetN(value);
		samples.SetN(value);
		return nsamples;
		}
    
    //! sets signal sampling period in ns (used, e.g., for X axis calculations in physical units).
    //! Sampling frequency is updated accordingly
    double SetTs(double value) {
		Ts=value; 
		Fs=1.0/Ts;
		int i;
		for(i=0;i<4;i++) coeffs[i].SetTs(value);
		samples.SetTs(value);
		return Ts;
	 }
    
       //! sets signal frequency in GHz,   
    //! sampling period is updated accordingly
    double SetFs(double value) {
		Fs=value; 
		Ts=1.0/Fs;
		int i;
		for(i=0;i<4;i++) coeffs[i].SetFs(value);
		samples.SetFs(value);
		return Fs;
	}
    
    double SetDelay1(double value){
		Tdel1=value;
		return value;
	}
	
	double SetDelay2(double value){
		Tdel2=value;
		return value;
	}
	
	double SetSubtractAmplitude(double value){
		DAmpl=value;
		return value;
	}
	
    double EvalAt(double t){
		return EvalStdAt(t)-DAmpl*EvalStdAt(t-Tdel1)+(DAmpl-1)*EvalStdAt(t-Tdel2);
	}
    
    double FindZeroCrossing(int nrecurr);
    
    protected:
	double EvalStdAt(double t){
		int i=floor(t/Ts);
		if(i<0) return 0;
		if(i>=nsamples) return 0;
		double dt=t/Ts-i;
		int j;
		double res=0;
		for(j=0;j<4;j++) res+=coeffs[j].At(i)*pow(dt,j);
		return res;
	}	
		
		
		
	
	
	
	
	
	
	
	
	double DAmpl;	//delayed amplification factor
	double Tdel1,Tdel2;	//delay in ns
	double Ts;   // sampling period in ns
	double Fs;   // sampling frequency in GHz
	int nsamples;  // number of samples in pSignal
	// graphic parameters
	int mstyle;
	int mcol;
	int lcol;
	pSignal coeffs[4];
	pSignal samples;

 ClassDef(pSplineArcSignal,3);
};
#endif

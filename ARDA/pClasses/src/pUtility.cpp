





#include <TMath.h>
#include <TF1.h>
#include <TRandom.h>


#define DEBUG

#include "pUtility.h"

ClassImp(pUtility);

// class pUtility {
// 
//  public: 
// 
//    static TF1 *Preamp2TF1(double tzero, double risetime, double tau, double baseline, double tmax){
// // funzione simile a segnale PA (gli fa bene anche essere un po' filtrata, successivamente)
//       TF1 *f = new TF1("f","[3]+((1-(1+(x-[0])/[1])*exp(-(x-[0])/[1])))*exp(-(x-[0])/[2])*(x>[0])",0,200000);
//       f->SetParameter(0,tzero);
//       f->SetParameter(1,risetime);
//       f->SetParameter(2,tau);
//       f->SetParameter(3,baseline);
//  /*    
//      
//      TF1 *f = new TF1("preamp","(exp(-(x-[0])/[2])-(1+(x-[0])/[1])*exp(-(x-[0])/[1]))*(x>[0])",0,tmax);
//      f->SetParameter(0,tzero);
//      f->SetParameter(1,tau);
//      f->SetParameter(2,risetime);*/
//      f->SetNpx(floor(tmax));
//      return f;
//    }
//    
//    static TF1 *Sine2TF1(double ampl, double freq, double toff, double tmax){
//           TF1 *f = new TF1("sine",Form("%f*sin(2.*TMath::Pi()*%f*(x-%f))",ampl,freq,toff),0, tmax);
//           f->SetNpx(10000);
// 	  return f;
//    }
//    
//    static TF1 *PMT2TF1(double ampl, double taur, double tauf, double tmax){
//         TF1 *f = new TF1("pmt",Form("-%e*(exp(-x/%e)-exp(-x/%e))",ampl,taur,tauf),0,tmax);
//         f->SetNpx(10000);
//         return f;
//    }
//    
//    
// 
// 
// };
// 

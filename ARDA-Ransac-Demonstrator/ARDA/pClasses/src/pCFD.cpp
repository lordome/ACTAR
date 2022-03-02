
#include "pCFD.h"

ClassImp(pCFD);


//============= CONSTRUCTORS================

pCFD::pCFD()
{
}

//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//

//determination of threshold crossing using different interpolation techniques and Newton method

double pCFD::LinearInterpolation(pSignal *s,int x2, double fmax){
    double xi0=(fmax-s->At(x2))/(s->At(x2+1)-s->At(x2));
    return s->GetTs()*(x2+xi0);
}

//--------------------------------------------------------------------------//

double pCFD::CubicConvInterpolation(pSignal *s,int x2, double fmax, int Nrecurr)
{
#if DBGLEVEL>0 
  printf ("--- %s ----\n", __PRETTY_FUNCTION__);
#endif
 /*
    NOTA: tutti i conti fatti con i tempi in "canali". aggiungero' il *Ts
    solo nel return.
  */
  /**** 2) normal CFD ***************************************************/
  double xi0=(fmax-s->At(x2))/(s->At(x2+1)-s->At(x2));
  if(Nrecurr<=0) return s->GetTs()*(x2+xi0);
  
  /**** 3) approx successive. *******************************************/
  int xk_index =x2;
  int N=s->GetN();
  double cm1, cN, ckm1, ck, ckp1, ckp2;
  ckm1=ck=ckp1=ckp2=0;
  if(xk_index>=1 && xk_index <= N-3){
      ckm1 = s->At(xk_index-1);
      ck   = s->At(xk_index);
      ckp1  = s->At(xk_index+1);
      ckp2  = s->At(xk_index+2);
  }else if(xk_index==0){
     cm1 = 3*s->At(0)-3*s->At(1)+s->At(2);
     ckm1 = cm1;
      ck   = s->At(0);
      ckp1  = s->At(1);
      ckp2  = s->At(2);
  }else if(xk_index==N-2){
     cN = 3*s->At(N-1)-3*s->At(N-2)+s->At(N-3);
     ckm1 = s->At(N-3);
      ck   = s->At(N-2);
      ckp1  = s->At(N-1);
      ckp2  = cN;
  }
  else return -1;//non dovrebbe succedere mai, ma rimuove un warning...., scatta se x2 non è dentro il segnale
  
  double a3=-0.5*ckm1+1.5*ck-1.5*ckp1+0.5*ckp2;
  double a2=ckm1-2.5*ck+2*ckp1-0.5*ckp2;
  double a1=-0.5*ckm1+0.5*ckp1;
  double a0=ck;

  double xi=xi0;
  for(int rec=0;rec<Nrecurr;rec++)
    {
#if DBGLEVEL>0 
      double delta_xi=(fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi);
      printf("%s: s[x2-1]=%e s[x2]=%e s[x2+1]=%e s[x2+2]=%e\n",__PRETTY_FUNCTION__,s->At(x2-1),s->At(x2),s->At(x2+1),s->At(x2+2));
      printf("%s: a0=%e a1=%e a2=%e a3=%e fmax=%e\n",__PRETTY_FUNCTION__,a0,a1,a2,a3,fmax);
      printf("%s: delta_xi[%d]=%e\n",__PRETTY_FUNCTION__,rec+1, delta_xi);
      xi += delta_xi;
#else
      xi += (fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi);
#endif
    }
  return s->GetTs()*(x2+xi);
}

//--------------------------------------------------------------------------//

double pCFD::CubicInterpolation(pSignal *s,int x2, double fmax, int Nrecurr){
   /**** 2) normal CFD ***************************************************/
  if(x2>s->GetN()-2){
    printf("pSignal::CubicInterpolation: x2 out of bounds = %d\n",x2);
    return -1;
  }
  double xi0=(fmax-s->At(x2))/(s->At(x2+1)-s->At(x2));
  if(Nrecurr<=0) return s->GetTs()*(x2+xi0);

  if(x2<1){
    printf("pSignal::CubicInterpolation: x2 out of bounds = %d\n",x2);
    return -1;
  }

  /**** 3) approx successive. *******************************************/
  // scrivo il polinomio come a3*xi**3+a2*xi**2+a1*xi+a0
  // dove xi=tcfd-x2 (ovvero 0<xi<1)
  // con maple:
  //                                         3
  //   (1/2 y2 - 1/6 y1 + 1/6 y4 - 1/2 y3) xi
  //
  //                                      2
  //          + (-y2 + 1/2 y3 + 1/2 y1) xi
  //
  //          + (- 1/2 y2 - 1/6 y4 + y3 - 1/3 y1) xi + y2

  double a3=0.5*s->At(x2)-(1./6.)*s->At(x2-1)+(1./6.)*s->At(x2+2)-0.5*s->At(x2+1);
  double a2=(-s->At(x2) + 0.5*s->At(x2+1) + 0.5*s->At(x2-1));
  double a1=(- 0.5* s->At(x2) - 1./6. *s->At(x2+2)+ s->At(x2+1) - 1./3.* s->At(x2-1));
  double a0=s->At(x2);
  double xi=xi0;
  for(int rec=0;rec<Nrecurr;rec++)
    {
#if DBGLEVEL>0 
      double delta_xi=(fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi);
      printf("pCFD::CubicInterpolation: s[x2-1]=%e s[x2]=%e s[x2+1]=%e s[x2+2]=%e\n",s->At(x2-1),s->At(x2),s->At(x2+1),s->At(x2+2));
      printf("pCFD::CubicInterpolation: a0=%e a1=%e a2=%e a3=%e fmax=%e\n",a0,a1,a2,a3,fmax);
      printf("pCFD::CubicInterpolation: delta_xi[%d]=%e\n",rec+1, delta_xi);
      xi += delta_xi;
#else
      xi += (fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi);
#endif
    }
  return s->GetTs()*(x2+xi);
}

//--------------------------------------------------------------------------//

double pCFD::CubicSplineInterpolation(pSignal *coeff,int x2, double fmax, int Nrecurr){
  double data[4];
  int i;
  for(i=-1;i<3;i++){
    data[i+1]=0;
    if((x2+i>=0)&&(x2+i<coeff->GetN())) data[i+1]=coeff->At(x2+i);
    //  printf("%f\n",data[i+1]);
  }
  double y2,y3;
  y2=data[0]+4*data[1]+data[2];
  y3=data[1]+4*data[2]+data[3];
  double xi0=(6*fmax-y2)/(y3-y2);
  // printf("%f\t%f\t%f\t%f\n",fmax,y2,y3,xi0);
  double a3=-data[0]/6.+data[1]/2.-data[2]/2.+data[3]/6.;
  double a2=data[0]/2.-data[1]+data[2]/2.;
  double a1=-data[0]/2.+data[2]/2.;
  double a0=data[0]/6.+2.*data[1]/3.+data[2]/6.-fmax;
  double xi=xi0;
  for(int rec=0;rec<Nrecurr;rec++)
    {
#if DBGLEVEL>0 
      double delta_xi=(a3*xi*xi*xi+a2*xi*xi+a1*xi+a0)/(3*a3*xi*xi+2*a2*xi+a1);
      printf("%s: s[x2-1]=%e s[x2]=%e s[x2+1]=%e s[x2+2]=%e\n",__PRETTY_FUNCTION__,coeff->At(x2-1),coeff->At(x2),coeff->At(x2+1),coeff->At(x2+2));
      printf("%s: a0=%e a1=%e a2=%e a3=%e fmax=%e\n",__PRETTY_FUNCTION__,a0,a1,a2,a3,fmax);
      printf("%s: delta_xi[%d]=%e\n",__PRETTY_FUNCTION__,rec+1, delta_xi);
      xi -= delta_xi;
#else
 	xi -=(a3*xi*xi*xi+a2*xi*xi+a1*xi+a0)/(3*a3*xi*xi+2*a2*xi+a1);
	//   	printf("xi=%f\n", xi);
#endif
    }
  return coeff->GetTs()*(x2+xi);
}

//--------------------------------------------------------------------------//

double pCFD::PentaInterpolation(pSignal *s,int x3, double fmax, int Nrecurr){
#if DBGLEVEL>0
  printf ("--- %s ----\n", __PRETTY_FUNCTION__);
#endif
 /*
    NOTA: tutti i conti fatti con i tempi in "canali". aggiungero' il *Ts
    solo nel return.
  */
  /**** 2) normal CFD ***************************************************/
  
   const double *data = s->GetArray();

  
  double xi0=(fmax-data[x3])/(data[x3+1]-data[x3]);
  if(Nrecurr<=0) return s->GetTs()*(x3+xi0);
  
  /**** 3) approx successive. *******************************************/
  // scrivo il polinomio come a5*xi**5+a4*xi**4+a3*xi**3+a2*xi**2+a1*xi+a0
  // dove xi=tcfd-x2 (ovvero 0<xi<1)
  // coefficienti calcolati con MUPAd (tool di MatLab):
// a0 = y3
// a1 = 1/20 y1 − 1/2 y2 − 1/3 y3 + y4 − 1/4 y5 + 1/30 y6
// a2 = 2/3 y2 − 1/24 y1 − 5/4 y3 + 0.2/3 y4 − 1/24 y5
// a3 = 5/12 y3 − 1/24 y2 − 1/24 y1 − 7/12 y4 + 7/24 y5 − 1/24 y6
// a4 = 1/24 y1 − 1/6 y2 + 0.25 y3 − 1/6 y4 + 1/24 y5
// a5 = 1/24 y2 − 1/120 y1 − 1/12 y3 + 1/12 y4 − 1/24 y5 + 1/120 y6


  double a0=data[x3];
  double a1= 1/20 * data[x3-2] - 1/2 * data[x3-1] - 1/3 * data[x3] + data[x3+1] - 1/4 * data[x3+2] + 1/30 * data[x3+3];
  double a2 = 2/3 * data[x3-1] - 1/24 * data[x3-2] - 5/4 * data[x3] + 0.2/3 * data[x3+1] - 1/24 * data[x3+2]; 
  double a3 = 5/12 * data[x3] - 1/24 * data[x3-1] - 1/24 * data[x3-2] - 7/12 * data[x3+1] + 7/24 * data[x3+2] - 1/24 * data[x3+3]; 
  double a4 = 1/24 * data[x3-2] - 1/6 * data[x3-1] + 0.25 * data[x3] - 1/6 * data[x3+1] + 1/24 * data[x3+2];
  double a5 = 1/24 * data[x3-1] - 1/120 * data[x3-2] - 1/12 * data[x3] + 1/12 * data[x3+1] - 1/24 * data[x3+2] + 1/120 * data[x3+3];

  double xi=xi0;
  for(int rec=0;rec<Nrecurr;rec++)
    {
#if DBGLEVEL>0
      double delta_xi=(fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi-a4*xi*xi*xi*xi-a5*xi*xi*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi+4*a4*xi*xi*xi+5*a5*xi*xi*xi*xi);
      printf("%s: s[x3-2]=%e s[x3-1]=%e s[x3]=%e s[x3+1]=%e s[x3+2]=%e s[x3+3]=%e\n",__PRETTY_FUNCTION__,
	      data[x3-2],data[x3-1],data[x3],data[x3+1],data[x3+2],data[x3+3]);
      printf("%s: a0=%e a1=%e a2=%e a3=%e a4=%e a5=%e  fmax=%e\n",__PRETTY_FUNCTION__,a0,a1,a2,a3,a4,a5,fmax);
      printf("%s: delta_xi[%d]=%e\n",__PRETTY_FUNCTION__,rec+1, delta_xi);
      xi += delta_xi;
#else
      xi +=(fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi-a4*xi*xi*xi*xi-a5*xi*xi*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi+4*a4*xi*xi*xi+5*a5*xi*xi*xi*xi);
#endif
    }
  return s->GetTs()*(x3+xi);

}


//************************************************************CORE FUNCTION***********************************//

double pCFD::CFD(pSignal *s,double fraction, double Max, int polarity, int type, int Nrecurr, int reverse, double start,int nbits,double l){
   double *data=s->GetArray();
   int NSamples=s->GetN();
   pDSP sigProc;
   pSignal *BScoeff;
   pSignal *IntSig;
   pSignal *temp;
   double **dummy=new double*;
   int delta=0;
   int i;
   double tcross;
   double fmax=fraction*Max;
   double Ts=s->GetTs();
   int x2=(int)(start/Ts);  // mod 2016-10-17 so we can start into the signal even if not reverse mode, Gab
   if (type==CUBICSPLINE_CFD){//Signal passing for samples but shorter!
     delta=sigProc.LTICalcCubicSplineFilter(nbits,dummy);
      if(reverse){
	x2=(int)(start/Ts);
	if(x2<=0 || x2>=NSamples-delta) return -1;
	for(; x2>0 ; x2--){
          if(polarity>=0 && data[x2]<fmax)	  break;
          if(polarity<0 && data[x2]>fmax) 	  break;
	}
      }else{
	for(;x2<NSamples-delta;x2++){
          if(polarity>=0 && data[x2]>fmax) break;
          if(polarity<0 && data[x2]<fmax)	break;
	}
	x2--;
      }
      if(x2<0) x2=0;
      if(x2>s->GetN()-1-delta) x2=s->GetN()-1-delta;
      delete *dummy;
   }
   else if (type==SMOOTHINGSPLINE_CFD){//Signal not passing for samples and shorter!
      delta=sigProc.LTICalcSmoothingSplineFilter(nbits,l,dummy);
      IntSig=sigProc.ResampleSmoothingSplines(*s,Ts,l,nbits);
      if(reverse){
	x2=(int)(start/Ts);
	if(x2<=0 || x2>=IntSig->GetN()) return -1;
	for(; x2>0 ; x2--){
          if(polarity>=0 && IntSig->At(x2)<fmax)	  break;
          if(polarity<0 && IntSig->At(x2)>fmax) 	  break;
	}
      }else{
	for(;x2<IntSig->GetN();x2++){
          if(polarity>=0 && IntSig->At(x2)>fmax) break;
          if(polarity<0 && IntSig->At(x2)<fmax)	break;
	}
	x2--;
      }
      if(x2<0) x2=0;
      if(x2>IntSig->GetN()-1) x2=IntSig->GetN()-1;
      delete IntSig;
      delete *dummy;
   }
   else{ //SMOOTHING SPLINE AND CUBIC SPLINE REQUIRE DIFFERENT TREATMENT!
     if(reverse){
	x2=(int)(start/Ts);
	if(x2<=0 || x2>=NSamples) return -1;
	for(; x2>0 ; x2--){
          if(polarity>=0 && data[x2]<fmax)	  break;
          if(polarity<0 && data[x2]>fmax) 	  break;
	}
      }else{
	for(;x2<NSamples;x2++){
          if(polarity>=0 && data[x2]>fmax) break;
          if(polarity<0 && data[x2]<fmax)	break;
	}
	x2--;
      }
      if(x2<0) x2=0;
      if(x2>s->GetN()-1) x2=s->GetN()-1;
   }
   delete dummy;

   switch(type){
   case LINEAR_CFD:
     return LinearInterpolation(s,x2, fmax);
     break;
   case CUBIC_CFD:
     return CubicInterpolation(s,x2, fmax, Nrecurr);
     break;
   case CUBICCONV_CFD:
     return CubicConvInterpolation(s,x2, fmax, Nrecurr);
     break;
   case CUBICSPLINE_CFD:
     //Optimization=evaluate spline coefficients only near the tcfd.... I have x2
     temp=new pSignal(4+2*delta);
     temp->SetTs(Ts);
     for(i=0;i<4+2*delta;i++) temp->SetAt(s->At(x2+i-1-delta),i);
     BScoeff=sigProc.BuildCubicSplineCoeffSignal(nbits,temp);
     tcross=CubicSplineInterpolation(BScoeff,delta+1, fmax, Nrecurr)+Ts*(x2-delta-1);
     delete temp;
     delete BScoeff;
     return tcross;
     break;
   case PENTA_CFD:
     return PentaInterpolation(s,x2, fmax, Nrecurr);
     break;
   case SMOOTHINGSPLINE_CFD:
     temp=new pSignal(4+2*delta);
     temp->SetTs(Ts);
     for(i=0;i<4+2*delta;i++) temp->SetAt(s->At(x2+i-1-delta),i);
     BScoeff=sigProc.BuildSmoothingSplineCoeffSignal(nbits,l,temp);
     tcross=CubicSplineInterpolation(BScoeff,delta+1, fmax, Nrecurr)+Ts*(x2-delta-1);
     delete temp;
     delete BScoeff;
     return tcross;
     break;
   default:
     return LinearInterpolation(s,x2, fmax);
     break;
   }
}

double pCFD::ARC(pSignal *s,double threshold, double fraction, double delay, int polarity, int type, int Nrecurr, double start, int length,int begin, int end){
   int NSamples=s->GetN();
   pDSP sigProc;
   double tcross;
   double Ts=s->GetTs();
   int x2=(int)(start/Ts); 

   pSignal *arc;
   float dummy;
   // if delay differs from an integer multiple of Ts by less than 100 ps, then we use BuildARCSignal (much faster!)
   //   if(fabs(modf(delay/s->GetTs(),&dummy)<0.1)){
   //    int delay_samples = delay/s->GetTs();
   //arc = sigProc.BuildARCSignal(*s,delay_samples,fraction,begin,end);
       //  }
       //  else{
        arc = sigProc.BuildSincARCSignal(*s,delay,fraction,length,begin,end);
       //  }
   double *data=arc->GetArray();

   // first we look for threshold crossing, on the negative side if signal is positive and on positive side if its negative
   for(;x2<NSamples;x2++){
       if(polarity>=0 && data[x2]<-threshold) break;
       if(polarity<0 && data[x2]>threshold)	break;
   }
   x2--; // x2 is now the sample index before crossing, now we must go back and find zero crossing


   for(;x2>0;x2--){
     if(polarity>=0 && data[x2]>0) break; // we come from the negative side
     if(polarity<0 && data[x2]<0)	break;  // we come from the positive side
   }

   
   switch(type){
   case LINEAR_CFD:
     tcross = LinearInterpolation(arc,x2, 0.);
     delete arc;
     return tcross;
     break;
   case CUBIC_CFD:
     tcross = CubicInterpolation(arc,x2, 0., Nrecurr);
     delete arc;
     return tcross;
     break;
   case CUBICCONV_CFD:
     tcross =CubicConvInterpolation(arc,x2, 0., Nrecurr);
     delete arc;
     return tcross;
     break;
   case PENTA_CFD:
     tcross =PentaInterpolation(arc,x2, 0., Nrecurr);
     delete arc;
     return tcross;
     break;
   default:
     tcross =LinearInterpolation(arc,x2,0.);
     delete arc;
     return tcross;
     break;
   }
}


double pCFD::ARCCubic(pSignal *s, double delay, double fraction, double maxt)
{
  // ARC constant fraction: delay puo' essere frazionario!!!
  // fractional delay fatto con interpolazione cubica!

#if DBGLEVEL>2    
  TCanvas *cplot = new TCanvas("cplot","Debug ARC",800,600);
  pDSP dsp;
#endif
  pSignal sacfd;
  sacfd.SetTs(s->GetTs());
  sacfd.SetN(maxt/sacfd.GetTs());
  
  double x=delay/s->GetTs();
  int x_int=(int)floor(x);
  double xi=x-x_int;
  // occhio che "xi" va guardata a sinistra del punto in esame!!
  x_int ++;
  xi=1-xi;

  /************ COEFFICIENTI PER IL "FILTRO" (fractional delay!) ************/
  double a1=-1./6.*xi*xi*xi+1./2.*xi*xi-1./3.*xi;
  double a2=+1./2.*xi*xi*xi-xi*xi-1./2.*xi+1;
  double a3=-1./2.*xi*xi*xi+1./2.*xi*xi+xi;
  double a4=1./6.*xi*xi*xi-1./6.*xi;
  /******************************************************/
  a1 /= -fraction;
  a2 /= -fraction;
  a3 /= -fraction;
  a4 /= -fraction;

 
  int N=sacfd.GetN();
  if(N+(2-x_int)>s->GetN())
    N=s->GetN()-(2-x_int);

  double *data_out = sacfd.GetArray(); 
  double *data_in = s->GetArray(); 
  //  for(int i=0;i<1+x_int;i++) data_out[i]=0;
  memset(data_out,0,x_int*sizeof(double));
  int k=-1;

  for(int i=1+x_int;i<N;i++)
    {
      data_out[i]=
	a1*data_in[i-1-x_int]+
	a2*data_in[i-0-x_int]+
	a3*data_in[i+1-x_int]+
	a4*data_in[i+2-x_int]
	+data_in[i]
	;
    }
  /////  s->Draw();  sacfd.Draw("same",2);

  int imax=0;
  //-- cerco il max
  for(int i=0;i<N;i++)
    if(data_out[imax]<data_out[i])
      imax=i;
  //-- torno indietro.
  for(k=imax;k>=0;k--)
    if(data_out[k]<0) break;
#if DBGLEVEL>0
    printf("pCFD::ARCCubic: calling CubicInterpolation(%p, %d, 0, 2)\n",&sacfd,k);
#endif    
  double t=CubicInterpolation(&sacfd,k,0,2);
#if DBGLEVEL>0
        printf("pCFD::ARCCubic: imax=%d data[imax]=%f k=%d t=%f\n",imax,data_out[imax],k,t);
       printf("pCFD::ARCCubic: sacfd[k-1]=%f sacfd[k]=%f sacfd[k+1]=%f sacfd[k+2]=%f\n",
           sacfd.At(k-1), sacfd.At(k), sacfd.At(k+1),sacfd.At(k+2) );
#endif    

#if DBGLEVEL>2    
        sacfd.Draw("ALP");  
        pSignal *sacfd_int = dsp.ResampleCubicBardelli(sacfd,sacfd.GetTs()/10.);
        sacfd_int->SetLineColor(kRed);
        sacfd_int->SetMarkerColor(kRed);
        sacfd_int->Draw("LP");
        cplot->Modified();
        cplot->Update();
 //       gClient->HandleInput();
 //       gSystem->ProcessEvents();
        cplot->WaitPrimitive();
        if(cplot !=NULL) delete cplot;
        cplot = NULL;
#endif
  return t;
}

// ***************************************
// Routines for digitizer synchronization using a periodic waveform
// Taken from FClasses with minimal changes. All credits to Luigi Bardelli!
//****************************************


TVectorD pCFD::RFSyncBuildWeightsWhiteNoise(double tau_clk, int Nsamples, double rfperiod_ns, int build_cos, int Npoly, int Narmo) 
// Npoly=3 means avoid 0,x,x^2 ; Narmo=2 means explicitly avoid 2*omega, 3*omega
{
  /************ CHECK IF IT EXISTS ***********************/
  char filename[1000];
  sprintf(filename,"rfweights_tau%.3f_nsa%d_rf%.1f_%s_npoly%d_narmo%d.root",tau_clk,Nsamples,rfperiod_ns,build_cos==1?"cos":"sin",Npoly,Narmo);

  TFile *fsav=gFile;
  TFile f(filename);
  if(!f.IsZombie())
    if(f.Get("rfw")!=NULL)
      {
	printf("%s: NSamples=%d  reading from %s...\n",__FUNCTION__,Nsamples, filename);
	TVectorD w(Nsamples);
	w=*(TVectorD*)f.Get("rfw");
	f.Close();
	gFile=fsav;
	return w;
      }  
  /*******************************************************/

  pSignal R(10);
  R.SetTs(tau_clk);
  R.SetAt(1,0);
  TVectorD w=pCFD::RFSyncBuildWeights(&R,Nsamples,rfperiod_ns,build_cos,Npoly,Narmo);
  TFile fout(filename,"RECREATE");
  fout.cd();
  w.Write("rfw");
  fout.Write();
  fout.Close();
  gFile=fsav;

  return w;

}
/***********************************************/
TVectorD pCFD::RFSyncBuildWeights(pSignal *R,int Nsamples, double rfperiod_ns, int build_cos, int Npoly, int Narmo) 
// Npoly=3 means avoid 0,x,x^2 ; Narmo=2 means explicitly avoid 2*omega, 3*omega
{

#if DBGLEVEL>0
  printf ("--- %s ----\n", __PRETTY_FUNCTION__);
#endif
  // organizzazione matrice :
  // [ Nsamples ] cos sin [ Npoly ] [2*Narmo ]

  if(Narmo<0)
    {
      // compute armonics.
      Narmo=(int)floor(rfperiod_ns / R->GetTs());
      if(Narmo>6) Narmo=6;
      if(Narmo<2) Narmo=2;
      printf("%s: using Narmo=%d\n",__PRETTY_FUNCTION__, Narmo);
    }

  char stringa[1000];
  sprintf(stringa,"%s: %d samples, period=%.3f ns, coseno=%d, Npoly=%d, Narmo=%d",__FUNCTION__,Nsamples,rfperiod_ns,build_cos,Npoly,Narmo);

  TBenchmark bench; bench.Start(stringa);
  double tauclk=R->GetTs();
  double omega= 2*M_PI/(rfperiod_ns/tauclk);

  int Nmatrix=Nsamples+2+Npoly+2*Narmo;

  TMatrixDSym M(Nmatrix);
  double *Rdata=R->GetArray();


  /************************************************ INIZIO M *************/
  // termini di autocorr
  for(int ir=0;ir<Nsamples;ir++) // loop on rows
    for(int ic=ir;ic<Nsamples;ic++) // loop on cols
      {// ic>=ir.
	int idx=ic-ir;	
	if(idx<0) printf("%s: INTERNAL ERROR: idx=%d\n",__PRETTY_FUNCTION__, idx);
	if(idx<R->GetN())
	  {
	    double v=Rdata[idx];
	    if(fabs(v)<1e-12) v=0;
	    M(ir,ic)=v;
	    M(ic,ir)=v;
	  }
	else
	  {
	    M(ir,ic)=0;
	    M(ic,ir)=0;
	  }
      }
  // termini con cos.
  for(int ic=0;ic<Nsamples;ic++) // loop on rows
    {
      M(Nsamples+0,ic)=cos(omega*ic);
      M(ic,Nsamples+0)=cos(omega*ic);
    }
  // termini con sin.
  for(int ic=0;ic<Nsamples;ic++) // loop on rows
    {
      M(Nsamples+1,ic)=sin(omega*ic);
      M(ic,Nsamples+1)=sin(omega*ic);
    }
  // termini con poly
  for(int ipoly=0;ipoly<Npoly;ipoly++)    
    for(int ic=0;ic<Nsamples;ic++) // loop on rows
      {
	double v=pow((double)ic,(double)ipoly);
	M(Nsamples+2+ipoly,ic)=v;
	M(ic,Nsamples+2+ipoly)=v;
      }
  // termini con armo
  for(int iarmo=0;iarmo<Narmo;iarmo++)    
    for(int ic=0;ic<Nsamples;ic++) // loop on rows
      {
	double vs=sin((2.+iarmo)*omega*ic);
	double vc=cos((2.+iarmo)*omega*ic);

	int idxcos=Nsamples+2+Npoly+2*iarmo+0;
	int idxsin=Nsamples+2+Npoly+2*iarmo+1;

	M(idxcos,ic)=vc;
	M(ic,idxcos)=vc;
	M(idxsin,ic)=vs;
	M(ic,idxsin)=vs;
      }
  /************************************************ FINE M ***************/

  // termine noto
  TVectorD noto(Nmatrix);
  if(build_cos==1)
    noto(Nsamples+0)=1;
  else
    noto(Nsamples+1)=1;
  /***************************************** CONTO *****************************/
  TDecompLU Mdecomp(M);
  if(Mdecomp.Solve(noto)==kFALSE)
    {
      printf("ERROR in decomposition!\n");
      return TVectorD();
    }
  noto.ResizeTo(Nsamples);
  /**********************************/
  bench.Stop(stringa);
  bench.Print(stringa);

  return noto;
}


double pCFD::RFSyncComputeT0(pSignal *s, int Nstart, TVectorD *wcoseno, TVectorD *wseno,double rfperiod_ns)
{
#if DBGLEVEL>0
  printf ("--- %s ----\n", __PRETTY_FUNCTION__);
#endif

  // check boundaries.
  if(wseno->GetNrows()!=wcoseno->GetNrows())
    {
      printf("ERROR in %s: different lenght of cos/sin !? %d != %d\n",__FUNCTION__, wcoseno->GetNrows(),wseno->GetNrows());
      return -1;
    }

  if(s->GetN()<Nstart+wcoseno->GetNrows())
    {
      printf("ERROR in %s: signal too short! Nsamples=%d, NSta=%d, Nweights=%d\n",__FUNCTION__,s->GetN(),Nstart, wcoseno->GetNrows());
      return -1;
    }
  //---------------------------------
  // compute weights
  double wcos=0;
  double wsin=0;
  int N=wcoseno->GetNrows();
  double  *data=s->GetArray();
  double *datac=wcoseno->GetMatrixArray();
  double *datas=wseno  ->GetMatrixArray();

  for(int i=0;i<N;i++)
    {
      wcos += data[i+Nstart]*datac[i];
      wsin += data[i+Nstart]*datas[i];
    }
  //---------------------------------
  // now T0
  /*
    s=sin(omega*(t-t0)) ===> s=sin(omega*t)*cos(omega*t0)-cos(omega*t)*sin(omega*t0)

   */

  double T0=-atan2(wcos,wsin)* rfperiod_ns/(2.*M_PI);
    
  if(T0<0) T0+=rfperiod_ns;
  return T0;

}


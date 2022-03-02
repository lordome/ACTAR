

#include "pDSP.h"

ClassImp(pDSP);

pDSP::pDSP()
{
}

pSignal *pDSP::ApplyWindowing(pSignal &s, int window_type, double beta)
{
    // vedi pag. 468 oppenheim-shafer
  int N=s.GetSize()-1;
    double alpha=0.5*(N+1); // for beta=0 is a rectangular window
  switch(window_type){
  case 0:/*-------------------- BARLETT ----------------------*/
    for(int n=0;n<=N;n++)
      s.SetAt(s.At(n)* ( n<N/2 ? 2.*n/(double)N : 2.- 2.*n/(double)N ), n);
    break;
  case 1:/*-------------------- HANNING ----------------------*/
    for(int n=0;n<=N;n++)
      s.SetAt(s.At(n)* (0.5-0.5*cos(2*TMath::Pi()*n/(double)N) ), n);
    break;
  case 2:/*-------------------- HAmmING ----------------------*/
    for(int n=0;n<=N;n++)
      s.SetAt(s.At(n)* (0.54-0.46*cos(2*TMath::Pi()*n/(double)N)), n);
    break;
  case 3:/*-------------------- BLACKMAN --------------------*/
    for(int n=0;n<=N;n++)
      s.SetAt(s.At(n)* (0.42-0.5*cos(2*TMath::Pi()*n/(double)N)+0.08*cos(4*TMath::Pi()*n/(double)N)), n);
    break;
  case 4:/*-------------------- KAISER --------------------*/
    for(int n=0;n<=N;n++)
      s.SetAt(s.At(n)*TMath::BesselI0(beta*sqrt(1-pow((n-alpha)/alpha,2)))/TMath::BesselI0(beta), n);
    break;
  default:
    printf("ERROR in %s: windowtype %d not valid!\n","ApplyWindowing", window_type);
  };
  return &s;
}

//! calculates the Fast Fourier Trasform of a Real valued signal
pFT  *pDSP::FFT(pSignal &s,const char *opt,int window){
//   printf("\n\n\n*******+  ATTENZIONE!!! ANCORA DA TESTARE!!!!!\n\n\n");
       pSignal *f = new pSignal(s);
       Int_t Nfft = f->GetSize(); 
//        printf("%s: calling TVirtualFFT...\n",__PRETTY_FUNCTION__);
       TVirtualFFT *fft = TVirtualFFT::FFT(1, &Nfft, opt);
       // windowing
       if(window>=0 && window <=3) ApplyWindowing(*f, window);
       // padding
       f->SetN(pow(2,(floor(log(f->GetN()) / log(2)) + 1))); // it seems that Set() pads with zeroes anyway
       fft->SetPoints(f->GetArray());
       fft->Transform();
       delete f;
       pFT *ft_out = new pFT(Nfft/2); 

       for(int i=0; i< Nfft/2;i++){
             Double_t re,im;
             fft->GetPointComplex(i, re, im);
             ft_out->GetArrayReal()->SetAt(re,i);
             ft_out->GetArrayImag()->SetAt(im,i);
       }
       ft_out->SetTs(s.GetTs());
       return ft_out;
}



pFT *pDSP::DFT(pSignal &s,float t0, float t1) 
{

  Double_t tau_clk = s.GetTs();

  if(s.GetN()<=0) 
    {
      printf("--- pDSP::DFT ---- **********ERROR: Signal length=%d <=0 !\n",s.GetN());
      return NULL;
    }
  int nbit=(int)ceil(log((float)s.GetN())/log(2.f));
  if((1<<nbit)==s.GetN())
    printf("WARNING IN %s: for DFT with size==2**N use pDSP::FFT(....)!!!!\n",__PRETTY_FUNCTION__);

  int nstart=0,nstop=s.GetN()-1;
  if(t0>0 && t1>=0 && t1>t0)
    {
      nstart=(int)rint(t0/tau_clk);
      nstop =(int)rint(t1/tau_clk);
    }
 if(nstart<0 || nstart>nstop || nstop>(s.GetN()-1))
   {
     printf("--- pDSP::DFT ---- ***ERROR: Invalid range t0=%f..t1=%f !\n------- Valid  from 0 to %f.\n"
	    ,t0,t1,(s.GetN()-1)*tau_clk);
     return NULL;
   }
//  printf("nstart=%d, nstop=%d\n",nstart,nstop);
 int Nft;
   Nft=(int)floor((nstop-nstart+1)/2.0)+1;

   //double ft[Nft];
 double flash[s.GetN()];
 for(int i=0;i<s.GetN();i++){
   flash[i]=(double)s.At(i);
 }
   pFT *ft_out = new pFT(Nft); 


  for(int k=0;k<Nft;k++)
    {
      double re=0, im=0;
      for(int i=nstart;i<=nstop;i++)
	{
	  re +=flash[i]*cos(-2*M_PI*i*k/(nstop-nstart+1));
	  im +=flash[i]*sin(-2*M_PI*i*k/(nstop-nstart+1));
	}

        ft_out->GetArrayReal()->SetAt(re,k);
        ft_out->GetArrayImag()->SetAt(im,k);

    }
 
  return ft_out;
}

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/

double pDSP::GetDataInterLinear(pSignal &s, double t)
{
// Fornisce il valore di pSignal all'istante t.
  if(s.GetN()<=0) return 1E10;
  Double_t tau_clk = s.GetTs();
  int n=(int)(floor(t/tau_clk));
  if(n<=0) return s.At(0);
  if(n>s.GetN()-2) return s.At(s.GetN()-1);
  if(n*tau_clk == t) return s.At(n);
  double y1=s.At(n);
  double y2=s.At(n+1); //quello prima e quello dopo.
  double x1=tau_clk*n;
  
  return (t-x1)/tau_clk*(y2-y1)+y1;
}

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
double pDSP::GetDataInterCubic(pSignal &s, double t)
{
  Double_t tau_clk = s.GetTs();
  int x2=(int)(t/tau_clk);
  if(x2<1 || x2>s.GetN()-2) return GetDataInterLinear(s,t);
  Double_t *data=s.GetArray();
  
  double a3=0.5*data[x2]-(1./6.)*data[x2-1]+(1./6.)*data[x2+2]-0.5*data[x2+1];
  double a2=(-data[x2] + 0.5*data[x2+1] + 0.5*data[x2-1]);
  double a1=(- 0.5* data[x2] - 1./6. *data[x2+2]+ data[x2+1] - 1./3.* data[x2-1]);
  double a0=data[x2];
  double xi=(t/tau_clk-x2);
  return a3*xi*xi*xi+a2*xi*xi+a1*xi+a0;
}

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
double pDSP::GetDataCubicConvolution(pSignal &c, double t)
{
  // see Keys IEEE Trans. Acoustic Speech, vol. ASSP-29, NO.6, DECEMBER 1981
  Double_t h = c.GetTs();
  Double_t xk=floor(t/h)*h;
  int xk_index = (int)(t/h);
  Double_t s = (t-xk)/h;

  Double_t *data=c.GetArray();

  int N = c.GetN();

  double cm1, cN, ckm1, ck, ckp1, ckp2;
  ckm1=ck=ckp1=ckp2=0;
  if(xk_index>=1 && xk_index <= N-3){
      ckm1 = data[xk_index-1];
      ck   = data[xk_index];
      ckp1  = data[xk_index+1];
      ckp2  = data[xk_index+2];
  }else if(xk_index==0){
     cm1 = 3*data[0]-3*data[1]+data[2];
     ckm1 = cm1;
      ck   = data[0];
      ckp1  = data[1];
      ckp2  = data[2];
  }else if(xk_index==N-2){
     cN = 3*data[N]-3*data[N-1]+data[N-2];
     ckm1 = data[N-3];
      ck   = data[N-2];
      ckp1  = data[N-1];
      ckp2  = cN;
  }
  else return -1;//non dovrebbe succedere mai, ma rimuove un warning...., scatta se t non è dentro il segnale
  
  double a3=-0.5*ckm1+1.5*ck-1.5*ckp1+0.5*ckp2;
  double a2=ckm1-2.5*ck+2*ckp1-0.5*ckp2;
  double a1=-0.5*ckm1+0.5*ckp1;
  double a0=ck;

  return a3*s*s*s+a2*s*s+a1*s+a0;
}


/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
pSignal *pDSP::ResampleLinear(pSignal &s, Double_t new_Ts)
{

  const int Nsa=s.GetSize();
  pSignal *interpolato = new pSignal(Nsa*s.GetTs()/new_Ts);
  interpolato->SetTs(new_Ts);
  for(int i=0;i<interpolato->GetN();i++)
    interpolato->SetAt(GetDataInterLinear(s,i*new_Ts),i);
  return interpolato;
} 

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
pSignal *pDSP::ResampleCubicBardelli(pSignal &s, Double_t new_Ts)
{

  const int Nsa=s.GetSize();
  pSignal *interpolato = new pSignal(Nsa*s.GetTs()/new_Ts);
  interpolato->SetTs(new_Ts);
  for(int i=0;i<interpolato->GetN();i++)
    interpolato->SetAt(GetDataInterCubic(s,i*new_Ts),i);
  return interpolato;
} 

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
pSignal *pDSP::ResampleCubicConvolution(pSignal &s, Double_t new_Ts)
{

  const int Nsa=s.GetSize();
  pSignal *interpolato = new pSignal(Nsa*s.GetTs()/new_Ts);
  interpolato->SetTs(new_Ts);
  for(int i=0;i<interpolato->GetN();i++)
    interpolato->SetAt(GetDataCubicConvolution(s,i*new_Ts),i);
  return interpolato;
} 

/**********************************************************/

TGraph* pDSP::BuildWindowedSincTGraph(double tauclk_orig, int npoints_orig_LR, int noversampling, int window_type)
{
  int NP=2*npoints_orig_LR*noversampling+1;
  double *x= new double [ NP ];
  double *y= new double [ NP ];
  
  for(int k=0;k<NP;k++)
    {
      x[k]=-npoints_orig_LR+(double)k/(double)noversampling;
      if(k==npoints_orig_LR*noversampling)
	y[k]=1;
      else
	y[k]= sin(M_PI*x[k])/( M_PI*x[k]  );
    }
  //  const double M_PI=3.14159265358979312e+00;
  //  windowing.... da FSignal.cxx
  int N=NP-1;
  switch(window_type){
  case 0:/*-------------------- BARLETT ----------------------*/
    for(int n=0;n<=N;n++)
      y[n]*= ( n<N/2 ? 2.*n/(double)N : 2.- 2.*n/(double)N );
    break;
  case 1:/*-------------------- HANNING ----------------------*/
    for(int n=0;n<=N;n++)
      y[n]*= 0.5-0.5*cos(2*M_PI*n/(double)N);
    break;
  case 2:/*-------------------- HAmmING ----------------------*/
    for(int n=0;n<=N;n++)
      y[n]*= 0.54-0.46*cos(2*M_PI*n/(double)N);
    break;
  case 3:/*-------------------- BLACKMAN --------------------*/
    for(int n=0;n<=N;n++)
      y[n]*= 0.42-0.5*cos(2*M_PI*n/(double)N)+0.08*cos(4*M_PI*n/(double)N);
    break;
  default:
    printf("ERROR in %s: windowtype %d not valid!\n",__PRETTY_FUNCTION__, window_type);
  };  
  //---------------------------------

  TGraph *g= new TGraph(NP,x,y);
  g->SetMarkerStyle(20);
  delete [] x;
  delete [] y;
  return g;
}


pSignal pDSP::BuildWindowedSinc(double tauclk_orig, int npoints_orig_LR, int noversampling, int window_type)
{

  TGraph *g=BuildWindowedSincTGraph(tauclk_orig,  npoints_orig_LR, noversampling, window_type);

  int NP=g->GetN();
  pSignal s(1);
  s.SetN(NP);
  s.SetTs(tauclk_orig/noversampling);
  for(int i=0;i<NP;i++)
    s.SetAt(g->GetY()[i],i);
  delete g;
  return s;
}

pSignal * pDSP::ResampleSincConvolution(pSignal &s, double taufinal, int npoints_LR, int window_type){
  
  const int Nsa=s.GetN();
  const double T=s.GetTs();
  pSignal *interpolato = new pSignal(1);
  interpolato->SetTs(taufinal);
  int oversam = T/taufinal+0.5;
  interpolato->SetN((int)(Nsa+2*npoints_LR+1)*oversam);
  pSignal sinc = BuildWindowedSinc(T,npoints_LR,oversam,window_type);
  printf("T=%f npoints_LR=%d  oversam=%d  sinc.nsamples=%d  sinc.tau=%f\n",
	 T,npoints_LR,oversam,sinc.GetN(), sinc.GetTs());
  //getchar();
  for(int i=0;i<interpolato->GetN();i++)
  {
    double y=0.;
    for(int j=npoints_LR+(i*taufinal-sinc.GetN()*sinc.GetTs())/T; j <= npoints_LR+i*taufinal/T; j++){ 
      //     if((i*taufinal-j*T)<0 || (i*taufinal-j*T)>npoints_LR*T)  printf("arg=%f\n",i*taufinal-j*T);

      int kk = i-(j-npoints_LR)*T/taufinal;
      if((j>0 && j<s.GetN())   &&  (kk>0 && kk < sinc.GetN()) )   y += s.At(j)*sinc.At(kk);
    }
    // for(int j=i*oversam-2*npoints_LR; j <= i*oversam; j++){
    //   y += Adc->At(j)*sinc.GetData(i*taufinal-j*T);
    // }
    interpolato->SetAt(y, i);
  }
  //  interpolato <<= ((npoints_LR-1) * T);
//   pDSP dsp;
//   pSignal *interpolato_delayed = dsp.SampleDelay(*interpolato, npoints_LR);
//   delete interpolato;
//   return interpolato_delayed;
     
   return interpolato;

}

/////////////////////////////////////////////////////////

pSignal * pDSP::SampleDelay(pSignal &s, int ndelay)
{
        pSignal *delayed = new pSignal(s.GetN()+ndelay);
        *delayed = s;
        for(int i=0; i< s.GetN(); i++){
            if(i< delayed->GetN()  && i-ndelay<s.GetN() && i>=0 && i-ndelay>=0)   delayed->SetAt(s.At(i-ndelay),    i);
	if(i>=0 && i-ndelay<0) delayed->SetAt(0,i);
	if(i<delayed->GetN() && i-ndelay>=s.GetN()) delayed->SetAt(0,i);	
        }
  return delayed;
}

pDigitalSignal * pDSP::SampleDelay(pDigitalSignal &s, int ndelay)
{
        pDigitalSignal *delayed = new pDigitalSignal(s);
	delayed->SetN(delayed->GetN()+ndelay);
	// *delayed = s;
        for(int i=0; i< delayed->GetN(); i++){
	  if(i-ndelay<s.GetN() && i-ndelay>=0)   delayed->SetAt(s.At(i-ndelay),    i);
	  else delayed->SetAt(0,i);
        }
  return delayed;
}




/////////////////////////////////////////////////////////

pSignal * pDSP::Decimation(pSignal &s, int m, int do_filter)
{
  Int_t oldN = s.GetN();
  if(do_filter){
    printf("%s: ANTIALIAS Filter still to be implemented\n", __PRETTY_FUNCTION__);
 
  }
  if(m<=1) {
    printf("%s: m<=1 ignoring...\n", __PRETTY_FUNCTION__);
     return NULL;
  }
  pSignal *decimato = new pSignal(oldN/m);

  for(int i=0, j=0; j<(oldN-m)/m; i+=m, j++){
    decimato->SetAt(s.At(i),j);
  }
  decimato->SetTs(s.GetTs()*m);

//   printf("%s: old Nsamples=%d   new Nsamples=%d\n",__PRETTY_FUNCTION__,oldN,decimato->GetN());
  return decimato;
}

pDigitalSignal * pDSP::Decimation(pDigitalSignal &s, int m, int do_filter)
{
  Int_t oldN = s.GetN();
  if(do_filter){
    printf("%s: ANTIALIAS Filter still to be implemented\n", __PRETTY_FUNCTION__);
 
  }
  if(m<=1) {
    printf("%s: m<=1 ignoring...\n", __PRETTY_FUNCTION__);
     return NULL;
  }
  pDigitalSignal *decimato = new pDigitalSignal(s);
  decimato->SetN(oldN/m);
  for(int i=0, j=0; j<decimato->GetN(); i+=m, j++){
    decimato->SetAt(s.At(i),j);
  }
  decimato->SetTs(s.GetTs()*m);
  //  Int_t newN = decimato->GetN();

//   printf("%s: old Nsamples=%d   new Nsamples=%d\n",__PRETTY_FUNCTION__,oldN,decimato->GetN());
  return decimato;
}

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
void pDSP::LTICalcCRRCCoeff(double tau, double Ts, double **b, double **a)
{
  double tau_sample = tau/Ts;
  
  double c = exp(-1.0/tau_sample);
  
  printf("pDSP::LTICalcCRRCCoeff: tau=%f  Ts=%f   c=%f\n",tau, Ts, c);
  *a = new double[3];
  *b = new double[2];
  
  (*a)[0] = 0.;
  (*a)[1] = 2.0*c;
  (*a)[2] = -c*c;
  (*b)[0] = c*(1.-c);
  (*b)[1] = -c*(1.-c);
  printf("pDSP::LTICalcCRRCCoeff: a1=%f   a2=%f   b0=%f   b1=%f\n",(*a)[1], (*a)[2], (*b)[0], (*b)[1]);
 
}

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
void pDSP::LTICalcRCLowPassFilter(double f_c, double Ts, double **b, double **a)
{
  double omega_c = 2.0*TMath::Pi()*f_c;
  double tau_c = 1.0/omega_c;
  double tau_sample = tau_c/Ts;
  
  double c = exp(-1.0/tau_sample);
  
  printf("pDSP::LTICalcRCLowPassFilter: f_c=%f  tau_c=%f  Ts=%f   c=%f\n",f_c, tau_c, Ts, c);
  *a = new double[2];
  *b = new double[2];
  
  (*a)[0] = 0.;
  (*a)[1] = c;
  (*b)[0] = 1.0-c;
  (*b)[1] = 0;
  printf("pDSP::LTICalcRCLowPassFilter: a1=%f    b0=%f   b1=%f\n",(*a)[1],  (*b)[0], (*b)[1]);
 
}

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
void pDSP::LTICalcRCHighPassFilter(double f_c, double Ts, double **b, double **a)
{
  double omega_c = 2.0*TMath::Pi()*f_c;
  double tau_c = 1.0/omega_c;
  double tau_sample = tau_c/Ts;
  
  double c = exp(-1.0/tau_sample);
  
  printf("pDSP::LTICalcRCHighPassFilter: f_c=%f  tau_c=%f  Ts=%f   c=%f\n",f_c, tau_c, Ts, c);
  *a = new double[2];
  *b = new double[2];
  
  (*a)[0] = 0.;
  (*a)[1] = c;
  (*b)[0] = c;
  (*b)[1] = -c;
  printf("pDSP::LTICalcRCHighPassFilter: a1=%f    b0=%f   b1=%f\n",(*a)[1],  (*b)[0], (*b)[1]);
 
}

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
void pDSP::LTICalcPZCorrection(double tau, double Ts, double **b, double **a)
{
  double tau_sample = tau/Ts;
  
  double c = exp(-1.0/tau_sample);
  
  printf("pDSP::LTICalcPZCorrection: tau=%f  Ts=%f   c=%f\n",tau, Ts, c);
  *a = new double[2];
  *b = new double[2];
  
  (*a)[0] = 0.;
  (*a)[1] = 0.0;
  (*b)[0] = 1.0;
  (*b)[1] = -c;
  printf("pDSP::LTICalcPZCorrection: a1=%f    b0=%f   b1=%f\n",(*a)[1],  (*b)[0], (*b)[1]);
 
}

void pDSP::LTICalcPZCorrection(Long64_t c_pzc, int scaling,Long64_t **b,Long64_t **a){
   printf("pDSP::LTICalcPZCorrection: c=%Ld\n", c_pzc);
  *a = new Long64_t[2];
  *b = new Long64_t[2];
  
  (*a)[0] = 0.;
  (*a)[1] = 0.0;
  (*b)[0] = (Long64_t)pow(2,scaling);
  (*b)[1] = -c_pzc;
  printf("pDSP::LTICalcPZCorrection: a1=%Ld    b0=%Ld   b1=%Ld\n",(*a)[1],  (*b)[0], (*b)[1]);
}

void pDSP::LTICalcPZCorrection(double tau, double Ts, int scaling,Long64_t **b,Long64_t **a){
  Long64_t c_pzc=(Long64_t)(exp(-Ts/tau)*pow(2,scaling)+0.5);
  LTICalcPZCorrection(c_pzc,scaling,b,a);
}

/***********************************************************************************/
/***********************************************************************************/
/***********************************************************************************/
void pDSP::LTICalcCRRCwPZC(double tau, double taupzc, double Ts, double **b, double **a)
{
  double tau_sample = tau/Ts;
  double taupzc_sample = taupzc/Ts;
  
  double c = exp(-1.0/tau_sample);
  double omc = 1-c;
  double c_pz = exp(-1.0/taupzc_sample);
 
  printf("pDSP::LTICalcCRRCwPZC: tau=%f  Ts=%f   c=%f\n",tau, Ts, c);
  *a = new double[4];
  *b = new double[3];
  
  (*a)[0] = 0.;
  (*a)[1] = (2*c+1);
  (*a)[2] = -c*c-2*c;
  (*a)[3] = c*c;
  
  (*b)[0] = c*omc;
  (*b)[1] = -c*omc*(1.0+c_pz);
  (*b)[2] = c_pz*c*omc;
  
  printf("pDSP::LTICalcCRRCwPZC: a1=%f    a2=%f   a3=%f    b0=%f   b1=%f    b2=%f\n",(*a)[1],  (*a)[2], (*a)[3], (*b)[0], (*b)[1], (*b)[2]);
 
}

pSignal *pDSP::LTIDifferenceEquation(pSignal *s, int orderX, int orderY, double *b, double *a)
{
  pSignal *out = new pSignal(*s);
//   out->SetN(s->GetN());
  
  for(int n=0; n < out->GetN(); n++){
      double y_n=0;
      for(int k=0; k<=orderX;k++) if(n-k >= 0) y_n += b[k]*s->At(n-k);
      for(int k=1; k<=orderY;k++) if(n-k >= 0) y_n += a[k]*out->At(n-k);  // set orderY=0 to calculate a FIR system
//       printf("n=%d   x[n-1]=%f  x[n]=%f   y[n-2]=%f  y[n-1]=%f  y[n]=%f\n",n,x->At(n-1), x->At(n), y->At(n-2), y->At(n-1),y_n);
     out->SetAt(y_n, n);
  }
  return out;
}

pDigitalSignal *pDSP::LTIDifferenceEquation(pDigitalSignal *s, int orderX, int orderY, Long64_t *b, Long64_t *a,int ResNbits)
{
  pDigitalSignal *out = new pDigitalSignal(*s);
//   out->SetN(s->GetN());
  if(ResNbits>0) out->SetNbits(ResNbits);
  
  for(int n=0; n < out->GetN(); n++){
      Long64_t y_n=0;
      for(int k=0; k<=orderX;k++) if(n-k >= 0) y_n += b[k]*s->At(n-k);
      for(int k=1; k<=orderY;k++) if(n-k >= 0) y_n += a[k]*out->At(n-k);  // set orderY=0 to calculate a FIR system
//       printf("n=%d   x[n-1]=%f  x[n]=%f   y[n-2]=%f  y[n-1]=%f  y[n]=%f\n",n,x->At(n-1), x->At(n), y->At(n-2), y->At(n-1),y_n);
     out->SetAt(y_n, n);
  }
  out->SaturateSignal();
  return out;
}


pSignal *pDSP::LTIAccumulator(pSignal *s)
{
  pSignal *out = new pSignal(*s);
//   out->SetN(s->GetN());
  
  double y_n;
  
  for(int n=0; n < out->GetN(); n++){
      y_n= s->At(n);
      if(n>0) y_n += out->At(n-1);      
     out->SetAt(y_n, n);
  }
  return out;
}

pDigitalSignal *pDSP::LTIAccumulator(pDigitalSignal *s,int ResNbits){
   pDigitalSignal *out = new pDigitalSignal(*s);
//   out->SetN(s->GetN());
  if(ResNbits>0) out->SetNbits(ResNbits);
  Long64_t y_n;
  
  for(int n=0; n < out->GetN(); n++){
      y_n= s->At(n);
      if(n>0) y_n += out->At(n-1);      
     out->SetAt(y_n, n);
  }
  out->SaturateSignal();
  return out;
}

pSignal *pDSP::LTIAccumulator(pSignal *s, int start)
{
  pSignal *out = new pSignal(*s);
//   out->SetN(s->GetN());
  
  double y_n;
  if(start<0 || start>= out->GetN()) {
    printf("%s: invalid start value %d\n",__PRETTY_FUNCTION__,start);
    return NULL;
  }
  for(int n=start; n < out->GetN(); n++){
      y_n= s->At(n);
      if(n>0) y_n += out->At(n-1);      
     out->SetAt(y_n, n);
  }
  return out;
}


pSignal *pDSP::LTITrapezoidalShaper(pSignal *s, int Nrise, int Mflat)
{
  pSignal *out = new pSignal(*s);
//   out->SetN(s->GetN());
  
  for(int n=0; n < out->GetN(); n++){
      double y_n= 0;
      y_n += s->At(n);
      if(n>=Nrise)  y_n -= s->At(n-Nrise);
      if(n>= (Nrise+Mflat))  y_n -= s->At(n-Nrise-Mflat);
      if(n>= (2*Nrise+Mflat))  y_n += s->At(n-2*Nrise-Mflat);
      y_n /= (double)Nrise;      
      if(n>0) y_n += out->At(n-1);
      
//       printf("n=%d   x[n-1]=%f  x[n]=%f   y[n-2]=%f  y[n-1]=%f  y[n]=%f\n",n,x->At(n-1), x->At(n), y->At(n-2), y->At(n-1),y_n);
     out->SetAt(y_n, n);
  }
  return out;
}

pDigitalSignal *pDSP::LTITrapezoidalShaper(pDigitalSignal *s, int Nrise, int Mflat,int ResNbits){
  pDigitalSignal *out = new pDigitalSignal(*s);
//   out->SetN(s->GetN());
  if(ResNbits>0) out->SetNbits(ResNbits);
  for(int n=0; n < out->GetN(); n++){
      Long64_t y_n= 0;
      y_n += s->At(n);
      if(n>=Nrise)  y_n -= s->At(n-Nrise);
      if(n>= (Nrise+Mflat))  y_n -= s->At(n-Nrise-Mflat);
      if(n>= (2*Nrise+Mflat))  y_n += s->At(n-2*Nrise-Mflat);
      // y_n /= (double)Nrise;      
      if(n>0) y_n += out->At(n-1);
      
//       printf("n=%d   x[n-1]=%f  x[n]=%f   y[n-2]=%f  y[n-1]=%f  y[n]=%f\n",n,x->At(n-1), x->At(n), y->At(n-2), y->At(n-1),y_n);
     out->SetAt(y_n, n);
  }
  out->SaturateSignal();
  return out;
}
  
pSignal *pDSP::LTITrapezoidalShaperPZC(pSignal *s, int Nrise, int Mflat, double taupzc, double Ts)
{
  double taupzc_sample = taupzc/Ts;
  double c_pz = exp(-1.0/taupzc_sample);
  //  printf("taupzc=%g\n",taupzc);
  pSignal *out = new pSignal(*s);
//   out->SetN(s->GetN());
  
  for(int n=0; n < out->GetN(); n++){
      double y_n= 0;
      y_n += s->At(n);
      if(n>0)  y_n -= c_pz*s->At(n-1);
         
      if(n>=Nrise)  y_n -= s->At(n-Nrise);
      if(n>=(Nrise+1))  y_n += c_pz*s->At(n-Nrise-1);
         
      if(n>= (Nrise+Mflat))  y_n -= s->At(n-Nrise-Mflat);
      if(n>= (Nrise+Mflat+1))  y_n += c_pz*s->At(n-Nrise-Mflat-1);
         
      if(n>= (2*Nrise+Mflat))  y_n += s->At(n-2*Nrise-Mflat);
      if(n>= (2*Nrise+Mflat+1))  y_n -= c_pz*s->At(n-2*Nrise-Mflat-1);
         
      y_n /= (double)Nrise;      
      if(n>0) y_n += 2.0*out->At(n-1);
      if(n>1) y_n -= out->At(n-2);
      
//       printf("n=%d   x[n-1]=%f  x[n]=%f   y[n-2]=%f  y[n-1]=%f  y[n]=%f\n",n,x->At(n-1), x->At(n), y->At(n-2), y->At(n-1),y_n);
     out->SetAt(y_n, n);
  }
  return out;
}

pDigitalSignal *pDSP::LTITrapezoidalShaperPZC(pDigitalSignal *s, int Nrise, int Mflat, Long64_t c_pz, int scaling, int ResNbits){

  pDigitalSignal *out = new pDigitalSignal(*s);
  Long64_t sf=(Long64_t)1<<scaling;
    
//   out->SetN(s->GetN());
  if(ResNbits>0) out->SetNbits(ResNbits);
  for(int n=0; n < out->GetN(); n++){
      Long64_t y_n= 0;
      y_n +=sf*s->At(n);
      if(n>0)  y_n -= c_pz*s->At(n-1);
         
      if(n>=Nrise)  y_n -=sf*s->At(n-Nrise);
      if(n>=(Nrise+1))  y_n += c_pz*s->At(n-Nrise-1);
         
      if(n>= (Nrise+Mflat))  y_n -=sf*s->At(n-Nrise-Mflat);
      if(n>= (Nrise+Mflat+1))  y_n += c_pz*s->At(n-Nrise-Mflat-1);
         
      if(n>= (2*Nrise+Mflat))  y_n +=sf*s->At(n-2*Nrise-Mflat);
      if(n>= (2*Nrise+Mflat+1))  y_n -= c_pz*s->At(n-2*Nrise-Mflat-1);
         
      // y_n /= (double)Nrise;      
      if(n>0) y_n += 2.0*out->At(n-1);
      if(n>1) y_n -= out->At(n-2);
      
//       printf("n=%d   x[n-1]=%f  x[n]=%f   y[n-2]=%f  y[n-1]=%f  y[n]=%f\n",n,x->At(n-1), x->At(n), y->At(n-2), y->At(n-1),y_n);
     out->SetAt(y_n, n);
  }
  out->SaturateSignal();
  return out;
}

pDigitalSignal *pDSP::LTITrapezoidalShaperPZC(pDigitalSignal *s, int Nrise, int Mflat, double taupzc, double Ts, int scaling, int ResNbits){
  double taupzc_sample = taupzc/Ts;
  Long64_t c_pz =(Long64_t)(exp(-1.0/taupzc_sample)*pow(2,scaling)+0.5);
  return LTITrapezoidalShaperPZC(s,Nrise,Mflat,c_pz,scaling,ResNbits);
}

pSignal *pDSP::AutoCorrelation(pSignal &s, int nsamples)
{
    double sumcor;
    double mu = s.Average(0,nsamples-1);
    if(nsamples <= 0) return NULL;
    pSignal *autocorr = new pSignal(nsamples);
    autocorr->SetTs(s.GetTs());
    
    for(int h=0; h<nsamples; h++){
	sumcor=0.;
	for(int n=0; n<nsamples-h; n++) {
	      sumcor += (s.At(n+h)-mu)*(s.At(n)-mu);
	}
  	autocorr->SetAt(sumcor/(nsamples-h),h);
// 	autocorr->SetAt(sumcor/nsamples,h);
    }
    return autocorr;
}

//! calculates the Autocorrelation sequence using the FFT
pSignal  *pDSP::FFTAutocorr(pSignal &s){
//   printf("\n\n\n*******+  ATTENZIONE!!! ANCORA DA TESTARE!!!!!\n\n\n");
       pSignal *f = new pSignal(s);
       Int_t Nfft = f->GetSize(); 
//        printf("%s: calling TVirtualFFT...\n",__PRETTY_FUNCTION__);
       TVirtualFFT *fft_forward = TVirtualFFT::FFT(1, &Nfft, "R2C M K");
       TVirtualFFT *fft_backward = TVirtualFFT::FFT(1, &Nfft, "C2R M K");
      // padding
//        f->SetN(pow(2,(floor(log(f->GetN()) / log(2)) + 1))); // it seems that Set() pads with zeroes anyway
       fft_forward->SetPoints(f->GetArray());
       fft_forward->Transform();

       for(int i=0; i< Nfft/2;i++){
             Double_t re,im;
             fft_forward->GetPointComplex(i, re, im);
             fft_backward->SetPoint(i,re*re+im*im);
       }
       fft_backward->Transform();
      for(int i=0; i< Nfft;i++){
             f->SetAt(fft_backward->GetPointReal(i)/f->GetN()/f->GetN(),i);
       }
        
       return f;
}


//Cubic Spline interpolation using LTI filter[Unser93]
int pDSP::LTICalcCubicSplineFilter(int nbits,double **a){
  int ncut;
  //Request that |h[n]|<0.5LSB for |n|>ncut
  if(nbits>2) ncut=floor((0.5*log(3)+(nbits-1)*log(2))/(-log(2-sqrt(3))));
  else ncut=50;
  *a=new double[2*ncut+1];
  int i;
  if(nbits>2){//quantize coeffs
    for(i=0;i<=ncut;i++){
      (*a)[ncut+i]=(*a)[ncut-i]=(double)floor(sqrt(3)*pow(sqrt(3)-2,(double)i)*pow(2,nbits-2)+0.5)/pow(2,nbits-2);
    }
  }
  else{
    for(i=0;i<=ncut;i++){//no quantization
      (*a)[ncut+i]=(*a)[ncut-i]=sqrt(3)*pow(sqrt(3)-2,(double)i);
    }
  }
  return ncut;
}

pSignal *pDSP::BuildCubicSplineCoeffSignal(int nbits,pSignal *in){
  double **xc=new double*;
  double yc;
  int ncut=LTICalcCubicSplineFilter(nbits,xc);
  pSignal *out=LTIDifferenceEquation(in,2*ncut+1,0,*xc,&yc);
  pSignal *res=new pSignal(*out);
  res->SetN(out->GetN()-ncut);
  int i;
  for(i=0;i<res->GetN();i++) res->SetAt(out->At(i+ncut),i);
  delete *xc;
  delete xc;
  delete out;
  return res;
}

double pDSP::GetDataInterCubicSplines(pSignal &coeff, double t){
  Double_t tau_clk=coeff.GetTs();
  int x2=(int)floor(t/tau_clk);
  Double_t xi=t/tau_clk-x2;
  Double_t data[4];
  int i;
  for(i=-1;i<3;i++){
    data[i+1]=0;
    if((x2+i>=0)&&(x2+i<coeff.GetN())) data[i+1]=coeff.At(x2+i);
  }
  double a3=-data[0]/6.+data[1]/2.-data[2]/2.+data[3]/6.;
  double a2=data[0]/2.-data[1]+data[2]/2.;
  double a1=-data[0]/2.+data[2]/2.;
  double a0=data[0]/6.+2.*data[1]/3.+data[2]/6.;
  return a3*xi*xi*xi+a2*xi*xi+a1*xi+a0;
}

double pDSP::GetDataInterCubicSplinesDerivative(pSignal &coeff, double t){
  Double_t tau_clk=coeff.GetTs();
  int x2=(int)floor(t/tau_clk);
  Double_t xi=t/tau_clk-x2;
  Double_t data[4];
  int i;
  for(i=-1;i<3;i++){
    data[i+1]=0;
    if((x2+i>=0)&&(x2+i<coeff.GetN())) data[i+1]=coeff.At(x2+i);
  }
  double a2=(-data[0]+3*data[1]-3*data[2]+data[3])/2.;
  double a1=data[0]-2*data[1]+data[2];
  double a0=(data[2]-data[0])/2.;
  return a2*xi*xi+a1*xi+a0;
}
    
pSignal *pDSP::ResampleCubicSplines(pSignal &s,Double_t new_Ts, int nbits){
  pSignal *C=BuildCubicSplineCoeffSignal(nbits,&s);
  int nsamples=C->GetN()*C->GetTs()/new_Ts;
  pSignal *interpolato=new pSignal(nsamples);
  interpolato->SetTs(new_Ts);
  for(int i=0;i<interpolato->GetN();i++)
    interpolato->SetAt(GetDataInterCubicSplines(*C,i*new_Ts),i);
  delete C;
  return interpolato;
}

pSignal *pDSP::ResampleCubicSplinesDerivative(pSignal &s,Double_t new_Ts, Double_t tauPZC, int nbits){
  Double_t k=0;
  if(tauPZC<=0) k=0;
  else k=s.GetTs()/tauPZC;
  pSignal *C=BuildCubicSplineCoeffSignal(nbits,&s);
  int nsamples=C->GetN()*C->GetTs()/new_Ts;
  pSignal *interpolato=new pSignal(nsamples);
  interpolato->SetTs(new_Ts);
  for(int i=0;i<interpolato->GetN();i++)
    interpolato->SetAt(GetDataInterCubicSplinesDerivative(*C,i*new_Ts)+k*GetDataInterCubicSplines(*C,i*new_Ts),i);
  delete C;
  return interpolato;
}

int pDSP::LTICalcSmoothingSplineFilter(int nbits,double l,double **a)
{
  //Evaluate radius of one poles of z-transform
  double at=(1-24*l)/(6*l);
  double b=(4+36*l)/(6*l);
  double A=2-b;
  double B=2+at*at-2*b;
  Double_t x1,x0,x2,x3;
  double der,fx;
  double fxold1;
  //double fxold2;
  x0=1000;
  x1=1000;
  x2=1000;
  fx=pow(x1,4)+A*pow(x1,3)+B*pow(x1,2)+A*x1+1;
  fxold1=fx+1;
  int fexit=0;
  int loopcount=0;
  do{
    der=4*pow(x1,3)+3*A*pow(x1,2)+2*B*x1+A;
    x3=x2;
    x2=x0;
    x0=x1;
    x1=x0-fx/der;
    if (x1==x2){
      x1=(x1+x0)/2.;
      loopcount++;
    }
    else if (x1==x3){
      x1=(x1+x0+x2)/3.;
      loopcount++;
    }
    if ((x1==x0)&&(x1==x2)&&(x1==x3)) fexit=1;
    if(loopcount==100) fexit=1;
    //  fxold2=fxold1;
    fxold1=fx;
    fx=pow(x1,4)+A*pow(x1,3)+B*pow(x1,2)+A*x1+1;   
  }while (((fx>0.000000001)||(fxold1!=fx))&&(fexit==0));
  x0=x1;//x0 is the square radius of the pole
  double r=sqrt(x0);
  double cphi=-at*r/(2*(r*r+1));
  if((cphi<-1)||(cphi>1)||(l<0.1)){
    // printf("Error Aborting\n");
    return -1;
  }
  double sphi=sqrt(1-cphi*cphi);
  double Re,Im;
  Re=sqrt(x0)*cphi;
  Im=sqrt(x0)*sphi;
  double a1,a2,b1,b2;
  double t;
  t=4*Re*Re*(x0-1)/(x0+1)+1-x0*x0;
  b2=1/(l*t);
  a2=-2*Re*x0*b2/(1+x0);
  b1=-x0*x0*b2;
  a1=-a2;
  double czr,czi;
  //double cizr,cizi;
  //double cbzr,cbzi;
  //double cbizr,cbizi;
  czr=a1/2;
  czi=(-a1*Re-b1)/(2*Im);
  // cbzr=a1-czr;
  //cbzi=-czi;
  //double iRe,iIm;
  //iRe=Re/x0;
  //iIm=-Im/x0;
  //cizr=a2/2;
  //cizi=(-a2*iRe-b2)/(2*iIm);
  //cbizr=a2-cizr;
  //cbizi=-cizi;
  //double ra0,ra1;
  //double phic;
  double phib,phiz;
  phiz=atan2(sphi,cphi);
  phib=atan2(czi,czr);
  //  phic=atan2(cizi,cizr);
  //printf("%f\t%f\t%f\n",phiz,phib,phic);
  //printf("%f\n",(phib-2*phiz-phic)/M_PI);
  double roB;
  //  double roC;
  roB=sqrt(czr*czr+czi*czi);
  //roC=sqrt(cizr*cizr+cizi*cizi);
  //printf("%f\t%f\t%f\n%f\n",x0,roB,roC,x0*roC);
  double roZ=sqrt(x0);
  int ncut=100;
  if(nbits>2) ncut=(int)floor((log(2*roB*roZ)+(nbits-1)*log(2))/log(roZ));
  *a=new double[2*ncut+1];
  int i;
  if(nbits>2){
    for(i=0;i<=ncut;i++){
      (*a)[ncut+i]=(*a)[ncut-i]=((double)floor(-2*roB*cos(phib-(i+1)*phiz)/pow(roZ,i+1)*pow(2,nbits-2)+0.5))/pow(2,nbits-2);
    }
  }
  else{
    for(i=0;i<=ncut;i++){
      (*a)[ncut+i]=(*a)[ncut-i]=-2*roB*cos(phib-(i+1)*phiz)/pow(roZ,i+1);
    }
  }
  return ncut;
}

pSignal *pDSP::BuildSmoothingSplineCoeffSignal(int nbits,double l,pSignal *in){
  double **xc=new double*;
  double yc;
  int ncut=LTICalcSmoothingSplineFilter(nbits,l,xc);
  if(ncut<0){
    printf("Warning! Smoothing splines algorithm failed! Using standard Interpolation\n");
    delete *xc;
    delete xc;
    return BuildCubicSplineCoeffSignal(nbits,in);
  }
  pSignal *out=LTIDifferenceEquation(in,2*ncut+1,0,*xc,&yc);
  pSignal *res=new pSignal(*out);
  res->SetN(out->GetN()-ncut);
  int i;
  for(i=0;i<res->GetN();i++) res->SetAt(out->At(i+ncut),i);
  delete *xc;
  delete xc;
  delete out;
  return res;
}

pSignal *pDSP::ResampleSmoothingSplines(pSignal &s,Double_t new_Ts,double l,int nbits){
  pSignal *C=BuildSmoothingSplineCoeffSignal(nbits,l,&s);
  int nsamples=C->GetN()*C->GetTs()/new_Ts;
  pSignal *interpolato=new pSignal(nsamples);
  interpolato->SetTs(new_Ts);
  for(int i=0;i<interpolato->GetN();i++)
    interpolato->SetAt(GetDataInterCubicSplines(*C,i*new_Ts),i);
  delete C;
  return interpolato;
}

pSignal *pDSP::ResampleSmoothingSplinesDerivative(pSignal &s,Double_t new_Ts, Double_t tauPZC,double l,int nbits){
  Double_t k=0;
  if(tauPZC<=0) k=0;
  else k=s.GetTs()/tauPZC;
  pSignal *C=BuildSmoothingSplineCoeffSignal(nbits,l,&s);
  int nsamples=C->GetN()*C->GetTs()/new_Ts;
  pSignal *interpolato=new pSignal(nsamples);
  //printf("Nsamples: %d, Ts= %f, new_Ts=%f,  C-GetN= %d\n",nsamples, C->GetTs(),new_Ts,C->GetN());
  interpolato->SetTs(new_Ts);
  for(int i=0;i<interpolato->GetN();i++)
    interpolato->SetAt(GetDataInterCubicSplinesDerivative(*C,i*new_Ts)+k*GetDataInterCubicSplines(*C,i*new_Ts),i);
  delete C;
  return interpolato;
}

//pDigitalSignal interpolating methods

int pDSP::LTICalcCubicSplineFilter(int nbits,Long64_t **a){
  int ncut;
  //Request that |h[n]|<0.5LSB for |n|>ncut
  if(nbits<3) nbits=3;
  ncut=floor((0.5*log(3)+(nbits-1)*log(2))/(-log(2-sqrt(3))));
  *a=new Long64_t[2*ncut+1];
  int i;
  for(i=0;i<=ncut;i++){
    (*a)[ncut+i]=(*a)[ncut-i]=(Long64_t)floor(sqrt(3)*pow(sqrt(3)-2,(double)i)*pow(2,nbits-2)+0.5);
  }
  return ncut;
}

pDigitalSignal *pDSP::BuildCubicSplineCoeffSignal(int nbits,pDigitalSignal *in, int ResNbits){
  Long64_t **xc=new Long64_t*;
  Long64_t yc;
  int Nbits=ResNbits;
  if(Nbits<0)Nbits=in->GetNbits()+nbits;
  int ncut=LTICalcCubicSplineFilter(nbits,xc);
  pDigitalSignal *out=LTIDifferenceEquation(in,2*ncut+1,0,*xc,&yc,Nbits);
  pDigitalSignal *res=new pDigitalSignal(*out);
  res->SetN(out->GetN()-ncut);
  int i;
  for(i=0;i<res->GetN();i++) res->SetAt(out->At(i+ncut),i);
  res->SaturateSignal();
  delete *xc;
  delete xc;
  delete out;
  return res;
}

Long64_t pDSP::GetDataInterCubicSplines(pDigitalSignal &coeff, int nbits_rec, double t){
  Double_t tau_clk=coeff.GetTs();
  int x2=(int)floor(t/tau_clk);
  Double_t xi=t/tau_clk-x2;
  Long64_t data[4];
  int i;
  for(i=-1;i<3;i++){
    data[i+1]=0;
    if((x2+i>=0)&&(x2+i<coeff.GetN())) data[i+1]=coeff.At(x2+i);
  }
  //Rethink to simulate real interpolating process
  /*pSignal: Coeff->polynom->Value
    pDigitalSignal: Coeff->Kernel samples (digitized)->Value
  */
  Long64_t coeffi[4];
  coeffi[0]=(Long64_t)floor(pow(1-xi,3)*pow(2,nbits_rec-1)/6.+0.5);
  coeffi[1]=(Long64_t)floor((pow(xi,3)/2.-pow(xi,2)+2./3.)*pow(2,nbits_rec-1)+0.5);
  coeffi[2]=(Long64_t)floor((-pow(xi,3)/2.+pow(xi,2)/2.+xi/2.+1./6.)*pow(2,nbits_rec-1)+0.5);
  coeffi[3]=(Long64_t)floor(pow(xi,3)*pow(2,nbits_rec-1)/6.+0.5);
  Long64_t result=0;
  for(i=0;i<4;i++) result+=data[i]*coeffi[i];
  return result;
}

Long64_t pDSP::GetDataInterCubicSplinesDerivative(pDigitalSignal &coeff, int nbits_rec, double t){
  Double_t tau_clk=coeff.GetTs();
  int x2=(int)floor(t/tau_clk);
  Double_t xi=t/tau_clk-x2;
  Long64_t data[4];
  int i;
  for(i=-1;i<3;i++){
    data[i+1]=0;
    if((x2+i>=0)&&(x2+i<coeff.GetN())) data[i+1]=coeff.At(x2+i);
  }
   //Rethink to simulate real interpolating process
  /*pSignal: Coeff->polynom->Value
    pDigitalSignal: Coeff->Kernel samples (digitized)->Value
  */
  Long64_t coeffi[4];
  coeffi[0]=(Long64_t)floor(-pow(1-xi,2)*pow(2,nbits_rec-1)/2.+0.5);
  coeffi[1]=(Long64_t)floor((3*pow(xi,2)/2.-2*xi)*pow(2,nbits_rec-1)+0.5);
  coeffi[2]=(Long64_t)floor((-3*pow(xi,2)/2.+xi+0.5)*pow(2,nbits_rec-1)+0.5);
  coeffi[3]=(Long64_t)floor(pow(xi,2)*pow(2,nbits_rec-1)/2.+0.5);
  Long64_t result=0;
  for(i=0;i<4;i++) result+=data[i]*coeffi[i];
  return result;
}

Long64_t pDSP::GetDataInterCubicSplinesDerivativePZC(pDigitalSignal &coeff, int nbits_rec, Double_t tauPZC, double t){
  double k;
  if(tauPZC<=0) k=0;
  else k=coeff.GetTs()/tauPZC;
  Double_t tau_clk=coeff.GetTs();
  int x2=(int)floor(t/tau_clk);
  Double_t xi=t/tau_clk-x2;
  Long64_t data[4];
  int i;
  for(i=-1;i<3;i++){
    data[i+1]=0;
    if((x2+i>=0)&&(x2+i<coeff.GetN())) data[i+1]=coeff.At(x2+i);
  }
  //Rethink to simulate real interpolating process
  /*pSignal: Coeff->polynom->Value
    pDigitalSignal: Coeff->Kernel samples (digitized)->Value
  */
  Long64_t coeffi[4];
  coeffi[0]=(Long64_t)floor((-pow(1-xi,2)/2.+k*pow(1-xi,3)/6.)*pow(2,nbits_rec-1)+0.5);
  coeffi[1]=(Long64_t)floor((3*pow(xi,2)/2.-2*xi+k*(pow(xi,3)/2.-pow(xi,2)+2./3.))*pow(2,nbits_rec-1)+0.5);
  coeffi[2]=(Long64_t)floor((-3*pow(xi,2)/2.+xi+0.5+k*(-pow(xi,3)/2.+pow(xi,2)/2.+xi/2.+1./6.))*pow(2,nbits_rec-1)+0.5);
  coeffi[3]=(Long64_t)floor((pow(xi,2)/2.+k*pow(xi,3)/6.)*pow(2,nbits_rec-1)+0.5);
  Long64_t result=0;
  for(i=0;i<4;i++) result+=data[i]*coeffi[i];
  return result;
}


pDigitalSignal *pDSP::ResampleCubicSplines(pDigitalSignal &s,Double_t new_Ts, int nbits_filter, int nbits_coeff, int nbits_rec, int ResNbits){
  pDigitalSignal *C=BuildCubicSplineCoeffSignal(nbits_filter,&s,nbits_coeff);
  int nsamples=C->GetN()*C->GetTs()/new_Ts;
  int nbits=ResNbits;
  if(nbits<=0) nbits=nbits_coeff+nbits_rec+2;
  pDigitalSignal *interpolato=new pDigitalSignal(nsamples,nbits);
  interpolato->SetSigned(1);
  interpolato->SetTs(new_Ts);
  for(int i=0;i<interpolato->GetN();i++)
    interpolato->SetAt(GetDataInterCubicSplines(*C,nbits_rec,i*new_Ts),i);
  interpolato->SaturateSignal();
  delete C;
  return interpolato;
}

pDigitalSignal *pDSP::ResampleCubicSplinesDerivative(pDigitalSignal &s,Double_t new_Ts, Double_t tauPZC, int nbits_filter, int nbits_coeff, int nbits_rec, int ResNbits){
  pDigitalSignal *C=BuildCubicSplineCoeffSignal(nbits_filter,&s,nbits_coeff);
  int nsamples=C->GetN()*C->GetTs()/new_Ts;
  int nbits=ResNbits;
  if(nbits<=0) nbits=nbits_coeff+nbits_rec+2;
  pDigitalSignal *interpolato=new pDigitalSignal(nsamples,nbits);
  interpolato->SetSigned(1);
  interpolato->SetTs(new_Ts);
  for(int i=0;i<interpolato->GetN();i++)
    interpolato->SetAt(GetDataInterCubicSplinesDerivativePZC(*C,nbits_rec,tauPZC,i*new_Ts),i);
  delete C;
  interpolato->SaturateSignal();
  return interpolato;
}

int pDSP::LTICalcSmoothingSplineFilter(int nbits,double l,Long64_t **a)
{
  if(l<0.1) return -1;
  //Evaluate radius of one poles of z-transform
  double at=(1-24*l)/(6*l);
  double b=(4+36*l)/(6*l);
  double A=2-b;
  double B=2+at*at-2*b;
  Double_t x1,x0,x2,x3;
  double der,fx;
  double fxold1;
  //  double fxold2;
  x0=1000;
  x1=1000;
  x2=1000;
  fx=pow(x1,4)+A*pow(x1,3)+B*pow(x1,2)+A*x1+1;
  fxold1=fx+1;
  int fexit=0;
  int loopcount=0;
  do{
    der=4*pow(x1,3)+3*A*pow(x1,2)+2*B*x1+A;
    x3=x2;
    x2=x0;
    x0=x1;
    x1=x0-fx/der;
    if (x1==x2){
      x1=(x1+x0)/2.;
      loopcount++;
    }
    else if (x1==x3){
      x1=(x1+x0+x2)/3.;
      loopcount++;
    }
    if ((x1==x0)&&(x1==x2)&&(x1==x3)) fexit=1;
    if(loopcount==100) fexit=1;
    //  fxold2=fxold1;
    fxold1=fx;
    fx=pow(x1,4)+A*pow(x1,3)+B*pow(x1,2)+A*x1+1;   
  }while (((fx>0.000000001)||(fxold1!=fx))&&(fexit==0));
  x0=x1;//x0 is the square radius of the pole
  double r=sqrt(x0);
  double cphi=-at*r/(2*(r*r+1));
  if((cphi<-1)||(cphi>1)||(l<0.1)){
    // printf("Error Aborting\n");
    return -1;
  }
  double sphi=sqrt(1-cphi*cphi);
  double Re,Im;
  Re=sqrt(x0)*cphi;
  Im=sqrt(x0)*sphi;
  double a1,a2,b1,b2;
  double t;
  t=4*Re*Re*(x0-1)/(x0+1)+1-x0*x0;
  b2=1/(l*t);
  a2=-2*Re*x0*b2/(1+x0);
  b1=-x0*x0*b2;
  a1=-a2;
  double czr,czi;
  //double cizr,cizi;
  //double cbzr,cbzi;
  //double cbizr,cbizi;
  czr=a1/2;
  czi=(-a1*Re-b1)/(2*Im);
  //cbzr=a1-czr;
  //cbzi=-czi;
  //double iRe,iIm;
  //iRe=Re/x0;
  //iIm=-Im/x0;
  //cizr=a2/2;
  //cizi=(-a2*iRe-b2)/(2*iIm);
  //cbizr=a2-cizr;
  //cbizi=-cizi;
  //double ra0,ra1;
  //double phic;
  double phib,phiz;
  phiz=atan2(sphi,cphi);
  phib=atan2(czi,czr);
  //phic=atan2(cizi,cizr);
  //printf("%f\t%f\t%f\n",phiz,phib,phic);
  //printf("%f\n",(phib-2*phiz-phic)/M_PI);
  double roB;
  //double roC;
  roB=sqrt(czr*czr+czi*czi);
  //roC=sqrt(cizr*cizr+cizi*cizi);
  //printf("%f\t%f\t%f\n%f\n",x0,roB,roC,x0*roC);
  double roZ=sqrt(x0);
  int ncut;
  int Rbits=nbits;
  if(nbits<3) Rbits=3;
  double hk=-2*roB*cos(phib-phiz)/roZ;
  int offset=(int)floor(log(hk)/log(2));
  // printf("%f\t%d\n",hk,offset);
  ncut=(int)floor(log(pow(2,Rbits-offset)*roB/roZ)/log(roZ));
  *a=new Long64_t[2*ncut+1];
  int i;
  //  printf("%f\t%f\t%f\t%f\n",roZ,roB,phiz,phib);
  for(i=0;i<=ncut;i++){
    hk=-2*roB*cos(phib-(i+1)*phiz)/pow(roZ,i+1);
    (*a)[ncut+i]=(*a)[ncut-i]=(Long64_t)floor(hk*pow(2,Rbits-2-offset)+0.5);
    //  printf("%f\t%Ld\t%4.4x\n",hk, (*a)[ncut+i], (*a)[ncut+i]);
  }
  for(i=0;i<ncut;i++){
    if((*a)[i]==0) ncut--;
    else break;
  }
  delete *a;
  *a=new Long64_t[2*ncut+1];
  for(i=0;i<=ncut;i++){
    hk=-2*roB*cos(phib-(i+1)*phiz)/pow(roZ,i+1);
    (*a)[ncut+i]=(*a)[ncut-i]=(Long64_t)floor(hk*pow(2,Rbits-2-offset)+0.5);
    //  printf("%f\t%Ld\t%4.4x\n",hk, (*a)[ncut+i], (*a)[ncut+i]);
  }
  return ncut;
}

pDigitalSignal *pDSP::BuildSmoothingSplineCoeffSignal(int nbits,double l,pDigitalSignal *in,int ResNbits){
  Long64_t **xc=new Long64_t*;
  Long64_t yc;
  int Nbits=ResNbits;
  if(Nbits<0)Nbits=in->GetNbits()+nbits;
  int ncut=LTICalcSmoothingSplineFilter(nbits,l,xc);
  if(ncut<0){
    printf("Warning! Smoothing splines algorithm failed! Using standard Interpolation\n");
    delete *xc;
    delete xc;
    return BuildCubicSplineCoeffSignal(nbits,in,ResNbits);
  }
  pDigitalSignal *out=LTIDifferenceEquation(in,2*ncut+1,0,*xc,&yc,Nbits);
  pDigitalSignal *res=new pDigitalSignal(*out);
  res->SetN(out->GetN()-ncut);
  int i;
  for(i=0;i<res->GetN();i++) res->SetAt(out->At(i+ncut),i);
  res->SaturateSignal();
  delete *xc;
  delete xc;
  delete out;
  return res;
}

pDigitalSignal *pDSP::ResampleSmoothingSplines(pDigitalSignal &s,Double_t new_Ts,double l, int nbits_filter, int nbits_coeff, int nbits_rec, int ResNbits){
  pDigitalSignal *C=BuildSmoothingSplineCoeffSignal(nbits_filter,l,&s,nbits_coeff);
  int nsamples=C->GetN()*C->GetTs()/new_Ts;
  int nbits=ResNbits;
  if(nbits<=0) nbits=nbits_coeff+nbits_rec+2;
  pDigitalSignal *interpolato=new pDigitalSignal(nsamples,nbits);
  interpolato->SetSigned(1);
  interpolato->SetTs(new_Ts);
  for(int i=0;i<interpolato->GetN();i++)
    interpolato->SetAt(GetDataInterCubicSplines(*C,nbits_rec,i*new_Ts),i);
  interpolato->SaturateSignal();
  delete C;
  return interpolato;
}

pDigitalSignal *pDSP::ResampleSmoothingSplinesDerivative(pDigitalSignal &s,Double_t new_Ts, Double_t tau_PZC,double l, int nbits_filter, int nbits_coeff, int nbits_rec, int ResNbits){
  pDigitalSignal *C=BuildSmoothingSplineCoeffSignal(nbits_filter,l,&s,nbits_coeff);
  int nsamples=C->GetN()*C->GetTs()/new_Ts;
  int nbits=ResNbits;
  if(nbits<=0) nbits=nbits_coeff+nbits_rec+2;
  pDigitalSignal *interpolato=new pDigitalSignal(nsamples,nbits);
  interpolato->SetSigned(1);
  interpolato->SetTs(new_Ts);
  for(int i=0;i<interpolato->GetN();i++)
    interpolato->SetAt(GetDataInterCubicSplinesDerivativePZC(*C,nbits_rec,tau_PZC,i*new_Ts),i);
  delete C;
  interpolato->SaturateSignal();
  return interpolato;
}


pSignal *pDSP::Delay(pSignal &s, double tshift)  // apply time shift (can handle a shift not multiple of tauclk)
{
  float *y = (float *)calloc(s.GetN(),sizeof(float));
  pSignal *out = new pSignal();
  out->SetTs(s.GetTs());
  out->SetN(s.GetN());
  for(int n=0; n<s.GetN();n++){
       y[n]=0;
       for(int k=0; k<s.GetN();k++){ // ci potrebbero essere effetti ai bordi...che fare?
         if((n-k-tshift/s.GetTs()))
	   y[n] += s.At(k)*sin(TMath::Pi()*(n-k-tshift/s.GetTs()))/TMath::Pi()/(n-k-tshift/s.GetTs());
         else
             y[n] += s.At(k);
       }
  }
  for(int n=0; n<s.GetN();n++) out->SetAt(y[n],n);
  free(y);
  return out;
}

pSignal *pDSP::Delay(pSignal &s, double tshift, int N)  // apply time shift (can handle a shift not multiple of tauclk)
{
  int len=2*N;
  float *y = (float *)calloc(s.GetN(),sizeof(float));
  pSignal *out = new pSignal();
  out->SetTs(s.GetTs());
  out->SetN(s.GetN());
  for(int n=0; n<s.GetN();n++){
       y[n]=0;
       for(int k=0; k<s.GetN();k++){ // ci potrebbero essere effetti ai bordi...che fare?
	   if(fabs(n-k-tshift/s.GetTs())<N){
             if((n-k-tshift/s.GetTs()))
	       y[n] += s.At(k)*sin(TMath::Pi()*(n-k-tshift/s.GetTs()))/TMath::Pi()/(n-k-tshift/s.GetTs())*
		 (0.54-0.46*cos(2*TMath::Pi()* ((n-k-tshift/s.GetTs())/(double)len-0.5)));
	     else
	       y[n] += s.At(k);
	   }
       }
  }
  for(int n=0; n<s.GetN();n++) out->SetAt(y[n],n);
  free(y);
  return out;
}

pSignal *pDSP::BuildSincARCSignal(pSignal &s, double delay, double fraction)
{
  float *y = (float *)calloc(s.GetN(),sizeof(float));
  pSignal *out = new pSignal();
  out->SetTs(s.GetTs());
  out->SetN(s.GetN());
  for(int n=0; n<s.GetN();n++){
       y[n]=0;
       for(int k=0; k<s.GetN();k++){ // ci potrebbero essere effetti ai bordi...che fare?
         if((n-k-delay/s.GetTs()))
	   y[n] += s.At(k)*sin(TMath::Pi()*(n-k-delay/s.GetTs()))/TMath::Pi()/(n-k-delay/s.GetTs());
         else
	   y[n] += s.At(k);
       }
  }
  for(int n=0; n<s.GetN();n++) out->SetAt(fraction*s.At(n)-y[n],n);
  free(y);
  return out;
  
}


pSignal *pDSP::BuildSincARCSignal(pSignal &s, double delay, double fraction, int N, int begin, int end)
{
  int len=2*N;
  float *y = (float *)calloc(s.GetN(),sizeof(float));
  pSignal *out = new pSignal();
  out->SetTs(s.GetTs());
  out->SetN(s.GetN());
  if(begin<0) begin=0;
  if(end>s.GetN()) end=s.GetN();
  for(int n=begin; n<end;n++)
     {
       y[n]=0;
       for(int k=0; k<s.GetN();k++){
	 double x = n-k-delay/s.GetTs();
	 if(fabs(x)<N)
	   {				       
            if(x!=0)
	       y[n] += s.At(k)*sin(TMath::Pi()*x)/TMath::Pi()/x*
		 (0.54+0.46*cos(2*TMath::Pi()* (x/len)));// we use 0.54+0.46 instead of 0.54-0.46 because we need a centered (non causal) window
	                                                  //  in ApplyWindowing we use the minus sign because we center the window at mid-signal. 
            else
	       y[n] += s.At(k);
         }
				   
	 }
     }
   
   
  for(int n=begin; n<end;n++) out->SetAt(fraction*s.At(n)-y[n],n);
  free(y);
  return out;
  
}

pSignal *pDSP::BuildARCSignal(pSignal &s, int delay, double fraction, int begin, int end)
{

  pSignal *out = new pSignal();
  out->SetTs(s.GetTs());
  out->SetN(s.GetN());
  if(begin<0) begin=0;
  if(end>s.GetN()) end=s.GetN();
  for(int n=begin; n<end;n++){
    if(n>=delay)
      out->SetAt(fraction*s.At(n)-s.At(n-delay),n);
      }
  return out;
  
}



pSignal *pDSP::BuildCubicARCSignal(pSignal &s, double delay, double fraction,  double maxt)
{
  // ARC constant fraction: delay puo' essere frazionario!!!
  // fractional delay fatto con interpolazione cubica!

  pSignal *sacfd = new pSignal(s);
  sacfd->SetTs(s.GetTs());
  sacfd->SetN(maxt/sacfd->GetTs());
  
  double x=delay/s.GetTs();
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

 
  int N=sacfd->GetN();
  if(N+(2-x_int)>s.GetN())
    N=s.GetN()-(2-x_int);

  double *data_out = sacfd->GetArray(); 
  double *data_in = s.GetArray(); 
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

  
  return  sacfd;
}


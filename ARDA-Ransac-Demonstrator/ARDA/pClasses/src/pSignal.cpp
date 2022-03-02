
#include "pSignal.h"

 ClassImp(pSignal); 

 
 //============= CONSTRUCTORS================
 
//_____________________________________________________________________________
pSignal::pSignal(){

  Init();
}
 
//_____________________________________________________________________________
pSignal::pSignal(int N){

  Init();

  //  s = new TArrayD(N);
  Set(N);
  nsamples = N;

}
//_____________________________________________________________________________
pSignal::pSignal(const TArrayD &a){

  Init();

  //  s = new TArrayD(N);
  Set(a.GetSize());
  nsamples = a.GetSize();

  for(int i=0;i<nsamples;i++) this->AddAt(a.At(i),i);


}

//___________________________________________________________________________

void pSignal::Init()
{

  Ts=1.0;   // sampling period in ns
  Fs=1.0;   // sampling frequency in GHz
  range=2.0; // peak-to-peak range, default +- 1V
  Nbits=14;  // number of bits for quantization
  fractional = 0; // set to 1 for fractional (i.e. abs(sample)<1) output 
  nsamples=0;  // number of samples in pSignal
  mstyle = 4;
  mcol = 1;
  lcol = 1;

  units = new TString("ns");

}

//_____________________________________________________________________________
pSignal::pSignal(const pSignal &a){

  this->Ts = a.Ts;
  this->Fs = a.Fs;
  this->range = a.range;
  this->Nbits= a.Nbits; 
  this->fractional = a.fractional; 
  //  this->nsamples = a.nsamples;
  //  this->s = new TArrayD(nsamples);
  //  Set(a.nsamples);
  SetN(a.nsamples);
  for(int i=0;i<nsamples;i++) this->AddAt(a.At(i),i);
  this->mstyle = a.mstyle;
  this->mcol = a.mcol;
  this->lcol = a.lcol;
  this->units = a.units;

}



//============= GRAPHICS ================

//_____________________________________________________________________________


TGraph *pSignal::Sig2Graph(){

  return Sig2Graph(0.0);

}

TGraph *pSignal::Sig2Graph(double tshift){
  
  TGraph *g = new TGraph();
  
  int nshift = floor(tshift/GetTs());

  for(int i=0; i<nsamples;i++){
    if(units->CompareTo("samples")==0) g->SetPoint(i,i-nshift,this->At(i));
    if(units->CompareTo("ns")==0) g->SetPoint(i,i*Ts-tshift,this->At(i));
  }
  g->SetMarkerSize(0.5);
  //  g->SetMarkerStyle(4);
  g->SetMarkerStyle(mstyle);
  g->SetMarkerColor(mcol);
  g->SetLineColor(lcol);
  if(units->CompareTo("samples")==0) g->GetXaxis()->SetTitle(Form("n (%s)",units->Data()));
  if(units->CompareTo("ns")==0) g->GetXaxis()->SetTitle(Form("t (%s)",units->Data()));

  return g;
  
  
}

//_____________________________________________________________________________
TH1F *pSignal::Sig2Histo(){
  
  TH1F *h = new TH1F("pSignal","pSignal",nsamples,0,nsamples*Ts);
  h->GetXaxis()->SetTitle(units->Data());
  
  for(int i=0; i<nsamples;i++) h->SetBinContent(i+1,this->At(i));
  if(units->CompareTo("samples")==0) h->GetXaxis()->SetTitle(Form("n (%s)",units->Data()));
  if(units->CompareTo("ns")==0) h->GetXaxis()->SetTitle(Form("t (%s)",units->Data()));

  return h;
  
  
}

//_____________________________________________________________________________
void pSignal::Draw(Option_t *option){
  TString o(option);
  if(o.Contains("h")){
    TH1F *h = Sig2Histo();
    h->Draw(option);
  }else{
    TGraph *g = Sig2Graph();
    g->Draw(option);
  }
}

//============= MATH ================

// freq in MHz
void pSignal::BuildSine(double amplitude, double freq, double toff)
{
//   freq /= 1000.;  // convert to GHz, since Ts is in nanosec
  double omega = 2.*TMath::Pi()*freq;
  for(int i=0; i<nsamples;i++) {
     this->SetAt(amplitude*sin(omega*(i*Ts-toff)), i);
 
#if DBGLEVEL>0 
    printf("%s s[%d] = %f\n",__PRETTY_FUNCTION__,i,this->At(i));
#endif
  }
}



//_____________________________________________________________________________
TFitResultPtr pSignal::Fit(TF1* f1, Option_t* option, Option_t* goption, Axis_t xmin, Axis_t xmax)
{
     TGraph *g = Sig2Graph();
     g->Draw("ALP");
     return g->Fit(f1, option, goption, xmin, xmax);
     
     
  
}

//_____________________________________________________________________________
pSignal *pSignal::Residuals(TF1 *f){
    pSignal *retsig = new pSignal(*this);
    for(int i=0; i<nsamples;i++) {
     retsig->SetAt( f->Eval(i*Ts) - this->At(i), i);

   }
   return retsig;
}


//=============== Timing ==================

double pSignal::LinearInterpolation(int x2, double fmax)
{
   double xi0=(fmax-this->At(x2))/(this->At(x2+1)-this->At(x2));
   return Ts*(x2+xi0);

}

double pSignal::CubicInterpolation(int x2, double fmax, int Nrecurr)
{

  /**** 2) normal CFD ***************************************************/
  if(x2>GetN()-2){
    //    printf("pSignal::CubicInterpolation: x2 out of bounds = %d\n",x2);
    return -1;
  }
  double xi0=(fmax-this->At(x2))/(this->At(x2+1)-this->At(x2));
  if(Nrecurr<=0) return Ts*(x2+xi0);

  if(x2<1){
    //    printf("pSignal::CubicInterpolation: x2 out of bounds = %d\n",x2);
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

  double a3=0.5*this->At(x2)-(1./6.)*this->At(x2-1)+(1./6.)*this->At(x2+2)-0.5*this->At(x2+1);
  double a2=(-this->At(x2) + 0.5*this->At(x2+1) + 0.5*this->At(x2-1));
  double a1=(- 0.5* this->At(x2) - 1./6. *this->At(x2+2)+ this->At(x2+1) - 1./3.* this->At(x2-1));
  double a0=this->At(x2);
  double xi=xi0;
  for(int rec=0;rec<Nrecurr;rec++)
    {
#if DBGLEVEL>0 
      double delta_xi=(fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi);
      printf("%s s[x2-1]=%e s[x2]=%e s[x2+1]=%e s[x2+2]=%e\n",__PRETTY_FUNCTION__, this->At(x2-1),this->At(x2),this->At(x2+1),this->At(x2+2));
      printf("%s a0=%e a1=%e a2=%e a3=%e fmax=%e\n",__PRETTY_FUNCTION__,a0,a1,a2,a3,fmax);
      printf("%s delta_xi[%d]=%e\n",__PRETTY_FUNCTION__,rec+1, delta_xi);
      xi += delta_xi;
#else
      xi += (fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi);
#endif
    }
  return Ts*(x2+xi);
}

double pSignal::Integrate(double baseline, int polarity, int start, int stop){
double sum = 0;
for(int i=start;i<stop;i++) sum += polarity*(At(i)-baseline);
return sum;
}

/***********************************************************************************/
double pSignal::CubicConvInterpolation(int x2, double fmax, int Nrecurr)
{
#if DBGLEVEL>0 
  printf ("--- %s ----\n", __PRETTY_FUNCTION__);
#endif
 /*
    NOTA: tutti i conti fatti con i tempi in "canali". aggiungero' il *Ts
    solo nel return.
  */
  /**** 2) normal CFD ***************************************************/
  double xi0=(fmax-this->At(x2))/(this->At(x2+1)-this->At(x2));
  if(Nrecurr<=0) return Ts*(x2+xi0);
  
  /**** 3) approx successive. *******************************************/
  int xk_index =x2;
  int N=GetN();
  double cm1, cN, ckm1, ck, ckp1, ckp2;
  ckm1=ck=ckp1=ckp2=0;
  if(xk_index>=1 && xk_index <= N-3){
      ckm1 = this->At(xk_index-1);
      ck   = this->At(xk_index);
      ckp1  = this->At(xk_index+1);
      ckp2  = this->At(xk_index+2);
  }else if(xk_index==0){
     cm1 = 3*this->At(0)-3*this->At(1)+this->At(2);
     ckm1 = cm1;
      ck   = this->At(0);
      ckp1  = this->At(1);
      ckp2  = this->At(2);
  }else if(xk_index==N-2){
     cN = 3*this->At(N-1)-3*this->At(N-2)+this->At(N-3);
     ckm1 = this->At(N-3);
      ck   = this->At(N-2);
      ckp1  = this->At(N-1);
      ckp2  = cN;
  }
  else return -1;//non dovrebbe succedere mai, ma rimuove un warning...., scatta se t non è dentro il segnale
  
  
  double a3=-0.5*ckm1+1.5*ck-1.5*ckp1+0.5*ckp2;
  double a2=ckm1-2.5*ck+2*ckp1-0.5*ckp2;
  double a1=-0.5*ckm1+0.5*ckp1;
  double a0=ck;

  double xi=xi0;
  for(int rec=0;rec<Nrecurr;rec++)
    {
#if DBGLEVEL>0 
      double delta_xi=(fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi);
      printf("%s s[x2-1]=%e s[x2]=%e s[x2+1]=%e s[x2+2]=%e\n",__PRETTY_FUNCTION__,this->At(x2-1),this->At(x2),this->At(x2+1),this->At(x2+2));
      printf("%s a0=%e a1=%e a2=%e a3=%e fmax=%e\n",__PRETTY_FUNCTION__,a0,a1,a2,a3,fmax);
      printf("%s delta_xi[%d]=%e\n",__PRETTY_FUNCTION__,rec+1, delta_xi);
      xi += delta_xi;
#else
      xi += (fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi);
#endif
    }
  return Ts*(x2+xi);
}

/***********************************************/
double pSignal::CubicSplineInterpolation(int x2, double fmax, int Nrecurr)
{
  int dimensione=18;	//dimensione della matrice dei campioni.!!!!deve essere pari!!!!
  //#define dimensione 18

  int xk_index = x2;
  int N=GetN();
  double xi0=(fmax-this->At(x2))/(this->At(x2+1)-this->At(x2));
  if(Nrecurr<=0) return Ts*(x2+xi0);
  if(xk_index>N-2) return Ts*(x2+xi0);//non si puÃ² interpolare per questo punto
  
  /**** 3) approx successive. *******************************************/

  double cm1, cNm1;


    TMatrixD e(dimensione, dimensione);
    TArrayD data_e(dimensione*dimensione);
    for(int k=0,i=0;i<dimensione;i++){
        data_e[k]=4.;
        if((k+1)<pow(dimensione,2)) data_e[k+1]=1.;
        if((k-1)>0)                 data_e[k-1]=1.;
        k+=dimensione+1;
    }
    e.SetMatrixArray(data_e.GetArray());
    e*=1./6/Ts;
    e.Invert();

	double dati_b[]={-1,3,-3,1,3,-6,3,0,-3,0,3,0,1,4,1,0};
	TMatrixD delta(4, 4, dati_b);

	
     TMatrixD samples(dimensione, 1);
     TMatrixD coeff(dimensione,1);
     TMatrixD b(4,1);
     TMatrixD coefficienti(4,1);
     
     const double *data = this->GetArray();
     
	if(xk_index<(dimensione/2-1)){
	samples.SetMatrixArray(data);//prendiamo i dati a partire dal primo
	}
	else if(xk_index>(N-dimensione/2-1)){
        samples.SetMatrixArray(data+N-dimensione);//prendiamo 18 dati partendo dalla fine
	}
	else{
        samples.SetMatrixArray(data+xk_index-(dimensione/2-1));//perche l'interp deve avvenire tra i campioni centrali della matrice
	}
        double *samples_interp = samples.GetMatrixArray();

	//        coeff=e*samples;
        coeff.Mult(e,samples);

        double *ck = coeff.GetMatrixArray();

	if(xk_index<(dimensione/2-1)){
		if(xk_index==0){
			cm1 = (*samples_interp)*6*Ts-((*ck)*4+(*(ck+1)));
			double caso_zero[]={cm1, *ck, *(ck+1), *(ck+2)};
			coefficienti.SetMatrixArray(caso_zero);
		}
		else coefficienti.SetMatrixArray(ck+xk_index-1);
	}
	else if(xk_index>(N-dimensione/2-1)){
		if(xk_index==N-2){
			cNm1 = (*(samples_interp+dimensione-1))*6*Ts-(*(ck+dimensione-1)*4+(*(ck+dimensione-2)));
			double casoN[]={*(ck+dimensione-3), *(ck+dimensione-2), *(ck+dimensione-1), cNm1};
			coefficienti.SetMatrixArray(casoN);
		}
		else coefficienti.SetMatrixArray(ck+dimensione-(N-xk_index+1));
	}
	else{
        coefficienti.SetMatrixArray(ck+(dimensione/2-2));
	}

	//	b=delta*coefficienti;
        b.Mult(delta,coefficienti);
        double *b_interp = b.GetMatrixArray();
	double a0 = *b_interp;
	double a1 = *(b_interp+1);
	double a2 = *(b_interp+2);
	double a3 = *(b_interp+3);

  double xi=xi0;
  for(int rec=0;rec<Nrecurr;rec++)
    {
#if DBGLEVEL>0 
      double delta_xi=(fmax-(1./6/Ts*(a3-a2*xi-a1*xi*xi-a0*xi*xi*xi)))/(a2+2*a1*xi+3*a0*xi*xi);
      printf("%s s[x2-1]=%e s[x2]=%e s[x2+1]=%e s[x2+2]=%e\n",__PRETTY_FUNCTION__,this->At(x2-1),this->At(x2),this->At(x2+1),this->At(x2+2));
      printf("%s a0=%e a1=%e a2=%e a3=%e fmax=%e\n",__PRETTY_FUNCTION__,a0,a1,a2,a3,fmax);
      printf("%s delta_xi[%d]=%e\n",__PRETTY_FUNCTION__,rec+1, delta_xi);
      xi += delta_xi;
#else
 	xi += (fmax-(1./6/Ts*(a3+a2*xi+a1*xi*xi+a0*xi*xi*xi)))/(1./6/Ts*(a2+2*a1*xi+3*a0*xi*xi));
	//   	printf("xi=%f\n", xi);
#endif
    }
  return Ts*(xk_index+xi);
}

/***********************************************************************************/

double pSignal::CubicSplineLTIInterpolation(int x2, double fmax, int Nrecurr)
{
  double xi0=(fmax-this->At(x2))/(this->At(x2+1)-this->At(x2));
  double data[4];
  int i;
  for(i=-1;i<3;i++){
    data[i+1]=0;
    if((x2+i>=0)&&(x2+i<this->GetN())) data[i+1]=this->At(x2-i);
  }
  double a3=-data[0]/6.+data[1]/2.-data[2]/2.+data[3]/6.;
  double a2=data[0]/2.-data[1]+data[2]/2.;
  double a1=-data[0]/2.+data[2]/2.;
  double a0=data[0]/6.+2.*data[1]/3.+data[2]/6.-fmax;
  double xi=xi0;
  for(int rec=0;rec<Nrecurr;rec++)
    {
#if DBGLEVEL>0 
      double delta_xi=(a3*xi*xi*xi+a2*xi*xi+a1*xi+a0)/(3*a3*xi*xi+2*a2*xi+a1);
      printf("%s s[x2-1]=%e s[x2]=%e s[x2+1]=%e s[x2+2]=%e\n",__PRETTY_FUNCTION__,this->At(x2-1),this->At(x2),this->At(x2+1),this->At(x2+2));
      printf("%s a0=%e a1=%e a2=%e a3=%e fmax=%e\n",__PRETTY_FUNCTION__,a0,a1,a2,a3,fmax);
      printf("%s delta_xi[%d]=%e\n",__PRETTY_FUNCTION__,rec+1, delta_xi);
      xi -= delta_xi;
#else
 	xi -=(a3*xi*xi*xi+a2*xi*xi+a1*xi+a0)/(3*a3*xi*xi+2*a2*xi+a1);
	//   	printf("xi=%f\n", xi);
#endif
    }
  return Ts*(x2+xi);
}

/***********************************************************************************/
double pSignal::PentaInterpolation(int x3, double fmax, int Nrecurr)
{
#if DBGLEVEL>0
  printf ("--- %s ----\n", __PRETTY_FUNCTION__);
#endif
 /*
    NOTA: tutti i conti fatti con i tempi in "canali". aggiungero' il *Ts
    solo nel return.
  */
  /**** 2) normal CFD ***************************************************/
  
   const double *data = this->GetArray();

  
  double xi0=(fmax-data[x3])/(data[x3+1]-data[x3]);
  if(Nrecurr<=0) return Ts*(x3+xi0);
  
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
      printf("%s s[x3-2]=%e s[x3-1]=%e s[x3]=%e s[x3+1]=%e s[x3+2]=%e s[x3+3]=%e\n",__PRETTY_FUNCTION__,
	      data[x3-2],data[x3-1],data[x3],data[x3+1],data[x3+2],data[x3+3]);
      printf("%s a0=%e a1=%e a2=%e a3=%e a4=%e a5=%e  fmax=%e\n",__PRETTY_FUNCTION__,a0,a1,a2,a3,a4,a5,fmax);
      printf("%s delta_xi[%d]=%e\n",__PRETTY_FUNCTION__,rec+1, delta_xi);
      xi += delta_xi;
#else
      xi +=(fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi-a4*xi*xi*xi*xi-a5*xi*xi*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi+4*a4*xi*xi*xi+5*a5*xi*xi*xi*xi);
#endif
    }
  return Ts*(x3+xi);

}



/**********************************************/
double pSignal::CFD(double fraction, double Max, int polarity, int type, int Nrecurr, int reverse, double start)
{

  double *data=this->GetArray();
  int NSamples=GetN();
  double fmax=fraction*Max;
  /**** 1) ricerca del punto x2 tale che x2 e' il precedente di tcfd ****/
  int x2=0;
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
  if(x2>GetN()-1) x2=GetN()-1;
  switch(type){
  case LINEAR_CFD:
    return LinearInterpolation(x2, fmax);
    break;
  case CUBIC_CFD:
    return CubicInterpolation(x2, fmax, Nrecurr);
    break;
  case CUBICCONV_CFD:
    return CubicConvInterpolation(x2, fmax, Nrecurr);
    break;
  case CUBICSPLINE_CFD:
    return CubicSplineInterpolation(x2, fmax, Nrecurr);
    break;
  case PENTA_CFD:
    return PentaInterpolation(x2, fmax, Nrecurr);
    break;
    /* case CUBICSPLINELTI_CFD:
    return CubicSplineLTIInterpolation(x2,fmax,Nrecurr);
    break;*/
  }
   return LinearInterpolation(x2, fmax);
}


double  pSignal::TimeIntegral(double tstart, double tstop)
{
   int istart = (int)(tstart/GetTs()+0.5);
   int istop = (int) (tstop/GetTs()+0.5);
   
   double sum=0;
   for(int i=istart; i<=istop; i++) sum += At(i);
   
   return sum;
   
  
  
}

//============= Input/Output ================

void  pSignal::SaveAsASCII(char *filename, int savex){
  FILE *f;
  
  f = fopen(filename, "w");
  if (f==NULL){
    printf("%s: error opening file %s for writing\n",__PRETTY_FUNCTION__,filename);
    return;
  }
  fprintf(f,"#  Nbits,   Ts,   frac,   nsamples,   range,   mstyle,  mcol,      lcol\n");   
  fprintf(f,"# %d,  %f,  %d,  %d, %f, %d, %d, %d\n",GetNbits(),GetTs(),GetFractional(),GetN(),GetRange(),GetMarkerStyle(),GetMarkerColor(),GetLineColor());   
  for(int i=0; i< GetN(); i++)
      if(!savex){
            fprintf(f,"%g\n", this->At(i));
      }else{
            fprintf(f,"%g   %g\n", i*this->GetTs(), this->At(i));
      }
  fclose(f);
}

void  pSignal::SaveAsSPE(char *filename){
  FILE *f;
  
  f = fopen(filename, "w");
  if (f==NULL){
    printf("%s: error opening file %s for writing\n",__PRETTY_FUNCTION__,filename);
    return;
  }
  fprintf(f,"0 %d %f 0 %s\n",GetN(), GetTs(), GetUnits()->Data());
  for(int i=0; i< GetN(); i++){
	if(i!=0 && i%8==0) fprintf(f,"\n");
    fprintf(f,"%g  ", this->At(i));
  }
  fprintf(f,"\n");
  fclose(f);
}


/************************************************************************/
pSignal pSignal::TekAcquireISF(int Nsig, int ch, const char *outputdir, const char *ip)
{
  char temp[2000];
  sprintf(temp,"wget -O %s/TEK%5.5d.isf \"http://%s/getwfm.isf?command=select:ch%d%%20on&command=save:waveform:fileformat%%20internal&wfmsend=Get\" >/dev/null 2>/dev/null", outputdir,Nsig, ip, ch);   
  pSignal s(1);
  int r=system(temp);
  if(r!=0)
    {
      printf("ERROR in %s: error %d executing \"%s\"....\n", __PRETTY_FUNCTION__,
	     r, temp);
      return s;
    }
  sprintf(temp,"%s/TEK%5.5d.isf",outputdir,Nsig);
  s.ReadBinTek(temp);
  return s;  
}

/**********************************************************************/ 
int pSignal::ReadBinTek(const char *fname) /* ISF tek format */
{
  // dati di TDS3000B
  // questo e' un metodo scemo, assume un header del tipo:
  // occhio ai bit e affini.................
  // scala finale in mV
  /*

    :WFMPRE:BYT_NR 2;BIT_NR 16;ENCDG BIN;BN_FMT RI;BYT_OR MSB;NR_PT 10000;WFID "Ch1, DC coupling, 5.0E-1 V/div, 2.0E-7 s/div, 10000 points, Sample mode";PT_FMT Y;XINCR 2.0E-10;PT_OFF 0;XZERO -9.08E-7;XUNIT "s";YMULT 7.8125E-5;YZERO 0.0E0;YOFF 1.0624E4;YUNIT "V";:CURVE #

  */
  int fd=open(fname,O_RDONLY);
  FILE *f=fdopen(fd,"r");
  if(f==NULL)
    return -1;
  char header[1000];
  memset(header, 0, sizeof(header));
  char c=0;
  int n=0;
  int Nbytes=0;
  int l=0;
  while(1)
    {
      if(fscanf(f,"%c",&c)!=1) break;
      //      printf("c=%c\n",c);
      header[n++]=c;
      if(c=='#')
	{
	  // lettura dopo curve: primo numero: len di len...
	  fscanf(f,"%c",&c);
	  l=c-'0';
	  char *temp2=new char [l+1];
	  for(int i=0;i<l;i++)
	    fscanf(f, "%c", temp2+i);
	  temp2[l]=0;
	  Nbytes=atoi(temp2);
	  delete [] temp2;
	  break;
	}
    }
//   printf("Header is %s\n",header);
//   printf("Nbytes is %d\n",Nbytes);

  if(Nbytes<=10)
    return -1;
  /****** search V/div *****/
  float Ymult=-1;
  {
    char *ptr=strstr(header, "YMULT");
    if(ptr!=NULL){
      ptr += strlen( "YMULT");
      Ymult=atof(ptr);
      //      printf("V/div: %e\n", Ymult);
    }
    else
      {
	char *ptr=strstr(header, "YMU");
	if(ptr!=NULL){
	  ptr += strlen( "YMU");
	  Ymult=atof(ptr);
	  //      printf("V/div: %e\n", Ymult);
	}
	else
	  return -2;	
      }


  }
  /****** search n/div ****/
  float Xincr=-1;
  {
    char *ptr=strstr(header, "XINCR");
    if(ptr!=NULL){
      ptr += strlen( "XINCR");
      Xincr=atof(ptr);
      //      printf("tau: %e\n", Xincr);
    }
    else
      {
	char *ptr=strstr(header, "XIN");
	if(ptr!=NULL){
	  ptr += strlen( "XIN");
	  Xincr=atof(ptr);
	  //      printf("tau: %e\n", Xincr);
	}
	else
	  return -3;
      }
  }  
  //questa era la versione pre-DPO
  //   /****** search V/div *****/
  //   float Ymult=-1;
  //   {
  //     char *ptr=strstr(header, "YMULT");
  //     if(ptr!=NULL){
  //       ptr += strlen( "YMULT");
  //       Ymult=atof(ptr);
  //       //      printf("V/div: %e\n", Ymult);
  //     }
  //     else
  //       return -2;
  //   }
  //   /****** search n/div ****/
  //   float Xincr=-1;
  //   {
  //     char *ptr=strstr(header, "XINCR");
  //     if(ptr!=NULL){
  //       ptr += strlen( "XINCR");
  //       Xincr=atof(ptr);
  //       //      printf("tau: %e\n", Xincr);
  //     }
  //     else
  //       return -3;
  //   }
  SetTs(Xincr*1e9);
  SetN(Nbytes/2);

#if __BYTE_ORDER == __LITTLE_ENDIAN
  char *dat=new char [ Nbytes ];
  lseek(fd, strlen(header)+1+l, SEEK_SET);
  read(fd, dat, Nbytes );
  for(int i=0;i<Nbytes/2;i++)
    {
      float v=dat[2*i+1]+dat[2*i]*0xFF;
      this->SetAt(v*Ymult*1e3,i); // scala finale in mV
    }
#else
  short *dat=new short[Nbytes/2];
  lseek(fd, strlen(header)+1+l, SEEK_SET);
  read(fd, dat, Nbytes );
  for(int i=0;i<Nbytes/2;i++)
    {
      float v=dat[i];
      //      float v=dat[2*i+1]+dat[2*i]*0xFF;
      this->SetAt( v*Ymult*1e3, i); // scala finale in mV
    }
#endif  
  close(fd);
  delete [] dat;  
  fclose(f);
  return 0;
}
/***************************************************************************************************/
// COPY CUT AND PASTE OPERATORS


pSignal *pSignal::removeFromSignal(int start, int end)
{
  if(end<=start){
    printf("%s: end must be greater than start! start=%d end=%d\n",__PRETTY_FUNCTION__,start,end);
  }
  if(end>GetN()){
    printf("%s: end must be smaller than total length! end=%d length=%d\n",__PRETTY_FUNCTION__,end,GetN());
  }
  if(start<0){
    printf("%s: start cannot be negative! start=%d\n",__PRETTY_FUNCTION__,start);
  }
  int nsamples = end - start;
  int newlength = GetN()-nsamples;
  for(int i=0; i<nsamples; i++) this->SetAt(this->At(end+i), start+i);
  this->SetN(newlength);

  return this;
}

pSignal *pSignal::copyFromSignal(int start, int end)
{
  if(end<=start){
    printf("%s: end must be greater than start! start=%d end=%d\n",__PRETTY_FUNCTION__,start,end);
  }
  if(end>GetN()){
    printf("%s: end must be smaller than total length! end=%d length=%d\n",__PRETTY_FUNCTION__,end,GetN());
  }
  if(start<0){
    printf("%s: start cannot be negative! start=%d\n",__PRETTY_FUNCTION__, start);
  }
  int nsamples = end - start;

  pSignal *clipsig = new pSignal(*this);
  clipsig->SetN(nsamples);
  for(int i=0; i<clipsig->GetN();i++) clipsig->SetAt(this->At(start+i),i);

  return clipsig;


}


pSignal *pSignal::cutFromSignal(int start, int end)
{
  pSignal *cutsig = this->copyFromSignal(start, end);
  this->removeFromSignal(start, end);
  return cutsig;
}

pSignal *pSignal::pasteToSignal(int start, pSignal *sig)
{
  if(start>GetN()){
    printf("%s: start cannot greater than total length! start=%d len=%d\n",__PRETTY_FUNCTION__,start,GetN());
  }
  if(start<0){
    printf("%s: start cannot be negative! start=%d\n",__PRETTY_FUNCTION__,start);
  }
  int oldlen = this->GetN();
  int newlen = oldlen + sig->GetN();
  this->SetN(newlen);

  for(int i=this->GetN()-1, j=oldlen-1; j>=start; i--,j--) this->SetAt(this->At(j),i);
  for(int i=start, j=0; j<sig->GetN(); i++,j++) this->SetAt(sig->At(j),i);

  return this;
  
}


// OPERATORS

/***************************************************************************/
pSignal pSignal::operator  = (const pSignal &a)
{

  this->Ts = a.Ts;
  this->Fs = a.Fs;
  this->range = a.range;
  this->Nbits= a.Nbits; 
  this->fractional = a.fractional; 
  this->nsamples = a.nsamples;
  //  this->s = new TArrayD(nsamples);
  this->Set(nsamples);
  for(int i=0;i<nsamples;i++) this->AddAt(a.At(i),i);
  this->mstyle = a.mstyle;
  this->mcol = a.mcol;
  this->lcol = a.lcol;
  this->units = a.units;
  
  return *this;
}


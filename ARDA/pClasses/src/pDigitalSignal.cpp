
#include "pDigitalSignal.h"

 ClassImp(pDigitalSignal); 

 
 //============= CONSTRUCTORS================
 
//_____________________________________________________________________________
pDigitalSignal::pDigitalSignal(){

  Init();
}
 
//_____________________________________________________________________________
pDigitalSignal::pDigitalSignal(int N,int nbits,int Signed){

  Init();

  //  s = new TArrayD(N);
  Set(N);
  nsamples = N;
  SetSigned(Signed);
  if (nbits>0) Nbits=nbits;
  BuildMask();

}
//_____________________________________________________________________________
pDigitalSignal::pDigitalSignal(const TArrayL64 &a){

  Init();

  //  s = new TArrayD(N);
  Set(a.GetSize());
  nsamples = a.GetSize();

  for(int i=0;i<nsamples;i++) this->AddAt(a.At(i),i);


}

//___________________________________________________________________________

void pDigitalSignal::Init()
{

  Ts=1.0;   // sampling period in ns
  Fs=1.0;   // sampling frequency in GHz
  range=2.0; // peak-to-peak range, default +- 1V
  Nbits=14;  // number of bits for quantization
  Signed=1;  //Signed notation
  nsamples=0;  // number of samples in pSignal
  mstyle = 4;
  mcol = 1;
  lcol = 1;
  units = new TString("ns");
  BuildMask();
}

//_____________________________________________________________________________
pDigitalSignal::pDigitalSignal(pSignal &a){

  this->Ts = a.GetTs();
  this->Fs = a.GetFs();
  this->range = a.GetRange();
  this->Nbits= a.GetNbits(); 
  this->nsamples = a.GetN();
  //  this->s = new TArrayD(nsamples);
  this->Signed=1;
  Set(this->nsamples);
  if(a.GetFractional()){
    for(int i=0;i<nsamples;i++) this->AddAt((Long64_t)(a.At(i)*pow(2,Nbits)),i);
  }
  else{
    for(int i=0;i<nsamples;i++) this->AddAt((Long64_t)a.At(i),i);
  }
  this->mstyle = a.GetMarkerStyle();
  this->mcol = a.GetMarkerColor();
  this->lcol = a.GetLineColor();
  this->units = a.GetUnits();
  BuildMask();
  SaturateSignal();

}


pDigitalSignal::pDigitalSignal(const pDigitalSignal &a){

  this->Ts = a.Ts;
  this->Fs = a.Fs;
  this->range = a.range;
  this->Nbits= a.Nbits; 
  this->nsamples = a.nsamples;
  this->Signed=a.Signed;
  this->Mask=a.Mask;
  //  this->s = new TArrayD(nsamples);
  Set(a.nsamples);
  for(int i=0;i<nsamples;i++) this->AddAt((Long64_t)a.At(i),i);
  this->mstyle = a.mstyle;
  this->mcol = a.mcol;
  this->lcol = a.lcol;
  this->units = a.units;

}


//============= GRAPHICS ================

//_____________________________________________________________________________


TGraph *pDigitalSignal::Sig2Graph(){

  return Sig2Graph(0.0);

}

TGraph *pDigitalSignal::Sig2Graph(double tshift){
  
  TGraph *g = new TGraph();
  
  int nshift = floor(tshift/GetTs());

  for(int i=0; i<nsamples;i++){
    if(units->CompareTo("samples")==0) g->SetPoint(i,i-nshift,(Double_t)Saturate(this->At(i)));
    if(units->CompareTo("ns")==0) g->SetPoint(i,i*Ts-tshift,(Double_t)Saturate(this->At(i)));
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

void pDigitalSignal::BuildSine(double amplitude, double freq, double toff){
  Long64_t temp;
  Long64_t tMask=0;
  Long64_t Min,Max;
  Min=-pow(2,Nbits-1);
  Max=pow(2,Nbits-1)-1;
  int i;
  for(i=0;i<Nbits-1;i++) tMask|=(Long64_t)1<<i;
  this->SetSigned(1);
  Double_t factor=pow(2,Nbits-1);
  double omega = 2.*TMath::Pi()*freq;
  for(i=0;i<nsamples;i++){
    temp=(Long64_t)(factor*amplitude*sin(omega*(i*Ts-toff)));
    if(temp>Max) temp=Max;
    if(temp<Min) temp=Min;
    this->SetAt(Saturate(temp),i);
  }
}
    
    



  
//_____________________________________________________________________________
TH1F *pDigitalSignal::Sig2Histo(){
  
  TH1F *h = new TH1F("pSignal","pSignal",nsamples,0,nsamples*Ts);
  h->GetXaxis()->SetTitle(units->Data());
  
  for(int i=0; i<nsamples;i++) h->SetBinContent(i+1,(Double_t)Saturate(this->At(i)));
  if(units->CompareTo("samples")==0) h->GetXaxis()->SetTitle(Form("n (%s)",units->Data()));
  if(units->CompareTo("ns")==0) h->GetXaxis()->SetTitle(Form("t (%s)",units->Data()));

  return h;
  
  
}

//_____________________________________________________________________________
void pDigitalSignal::Draw(Option_t *option){
  TString o(option);
  if(o.Contains("h")){
    TH1F *h = Sig2Histo();
    h->Draw();
  }else{
    TGraph *g = Sig2Graph();
    g->Draw(option);
  }
}

//============= MATH ================




//_____________________________________________________________________________
TFitResultPtr pDigitalSignal::Fit(TF1* f1, Option_t* option, Option_t* goption, Axis_t xmin, Axis_t xmax)
{
     TGraph *g = Sig2Graph();
     g->Draw("ALP");
     return g->Fit(f1, option, goption, xmin, xmax);
     
     
  
}

//_____________________________________________________________________________
pSignal *pDigitalSignal::Residuals(TF1 *f){
  pSignal *res=new pSignal(nsamples);
    for(int i=0; i<nsamples;i++) {
      res->SetAt( f->Eval(i*Ts) - (Double_t)(this->At(i)), i);

   }
   return res;
}


//=============== Timing ==================

double pDigitalSignal::LinearInterpolation(int x2, double fmax)
{
  double xi0=(fmax-(Double_t)this->At(x2))/((Double_t)this->At(x2+1)-(Double_t)this->At(x2));
   return Ts*(x2+xi0);

}

double pDigitalSignal::CubicInterpolation(int x2, double fmax, int Nrecurr)
{

  /**** 2) normal CFD ***************************************************/
  if(x2>GetN()-2){
    printf("pSignal::CubicInterpolation: x2 out of bounds = %d\n",x2);
    return -1;
  }
  double xi0=(fmax-(Double_t)this->At(x2))/((Double_t)this->At(x2+1)-(Double_t)this->At(x2));
  if(Nrecurr<=0) return Ts*(x2+xi0);

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

  double a3=0.5*(Double_t)this->At(x2)-(1./6.)*(Double_t)this->At(x2-1)+(1./6.)*(Double_t)this->At(x2+2)-0.5*(Double_t)this->At(x2+1);
  double a2=(-(Double_t)this->At(x2) + 0.5*(Double_t)this->At(x2+1) + 0.5*(Double_t)this->At(x2-1));
  double a1=(- 0.5* (Double_t)this->At(x2) - 1./6. *(Double_t)this->At(x2+2)+ (Double_t)this->At(x2+1) - 1./3.* (Double_t)this->At(x2-1));
  double a0=(Double_t)this->At(x2);
  double xi=xi0;
  for(int rec=0;rec<Nrecurr;rec++)
    {
#if DBGLEVEL>0 
      double delta_xi=(fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi);
      printf("s[x2-1]=%e s[x2]=%e s[x2+1]=%e s[x2+2]=%e\n",(Double_t)this->At(x2-1),(Double_t)this->At(x2),(Double_t)this->At(x2+1),(Double_t)this->At(x2+2));
      printf("a0=%e a1=%e a2=%e a3=%e fmax=%e\n",a0,a1,a2,a3,fmax);
      printf("delta_xi[%d]=%e\n",rec+1, delta_xi);
      xi += delta_xi;
#else
      xi += (fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi);
#endif
    }
  return Ts*(x2+xi);
}

double pDigitalSignal::Integrate(double baseline, int polarity, int start, int stop){
double sum = 0;
 for(int i=start;i<stop;i++) sum += polarity*((Double_t)At(i)-baseline);
return sum;
}

/***********************************************************************************/
double pDigitalSignal::CubicConvInterpolation(int x2, double fmax, int Nrecurr)
{
#if DBGLEVEL>0 
  printf ("--- %s ----\n", __PRETTY_FUNCTION__);
#endif
 /*
    NOTA: tutti i conti fatti con i tempi in "canali". aggiungero' il *Ts
    solo nel return.
  */
  /**** 2) normal CFD ***************************************************/
  double xi0=(fmax-(Double_t)this->At(x2))/((Double_t)this->At(x2+1)-(Double_t)this->At(x2));
  if(Nrecurr<=0) return Ts*(x2+xi0);
  
  /**** 3) approx successive. *******************************************/
  int xk_index =x2;
  int N=GetN();
  double cm1, cN, ckm1, ck, ckp1, ckp2;
  ckm1=ck=ckp1=ckp2=0;
  
  if(xk_index>=1 && xk_index <= N-3){
      ckm1 = (Double_t)this->At(xk_index-1);
      ck   = (Double_t)this->At(xk_index);
      ckp1  = (Double_t)this->At(xk_index+1);
      ckp2  = (Double_t)this->At(xk_index+2);
  }else if(xk_index==0){
     cm1 = 3*(Double_t)this->At(0)-3*(Double_t)this->At(1)+(Double_t)this->At(2);
     ckm1 = cm1;
      ck   = (Double_t)this->At(0);
      ckp1  = (Double_t)this->At(1);
      ckp2  = (Double_t)this->At(2);
  }else if(xk_index==N-2){
     cN = 3*(Double_t)this->At(N-1)-3*(Double_t)this->At(N-2)+(Double_t)this->At(N-3);
     ckm1 = (Double_t)this->At(N-3);
      ck   = (Double_t)this->At(N-2);
      ckp1  = (Double_t)this->At(N-1);
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
      printf("s[x2-1]=%e s[x2]=%e s[x2+1]=%e s[x2+2]=%e\n",(Double_t)this->At(x2-1),(Double_t)this->At(x2),(Double_t)this->At(x2+1),(Double_t)this->At(x2+2));
      printf("a0=%e a1=%e a2=%e a3=%e fmax=%e\n",a0,a1,a2,a3,fmax);
      printf("delta_xi[%d]=%e\n",rec+1, delta_xi);
      xi += delta_xi;
#else
      xi += (fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi);
#endif
    }
  return Ts*(x2+xi);
}

/***********************************************/
/*
double pDigitalSignal::CubicSplineInterpolation(int x2, double fmax, int Nrecurr)
{
  int dimensione=18;	//dimensione della matrice dei campioni.!!!!deve essere pari!!!!
  //#define dimensione 18

  int xk_index = x2;
  int N=GetN();
  double xi0=(fmax-(Double_t)this->At(x2))/((Double_t)this->At(x2+1)-(Double_t)this->At(x2));
  if(Nrecurr<=0) return Ts*(x2+xi0);
  if(xk_index>N-2) return Ts*(x2+xi0);//non si puÃ² interpolare per questo punto
*/
  /**** 3) approx successive. *******************************************/
/*
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
     
     const Long64_t *data = this->GetArray();
     
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
      printf("s[x2-1]=%e s[x2]=%e s[x2+1]=%e s[x2+2]=%e\n",this->At(x2-1),this->At(x2),this->At(x2+1),this->At(x2+2));
      printf("a0=%e a1=%e a2=%e a3=%e fmax=%e\n",a0,a1,a2,a3,fmax);
      printf("delta_xi[%d]=%e\n",rec+1, delta_xi);
      xi += delta_xi;
#else
 	xi += (fmax-(1./6/Ts*(a3+a2*xi+a1*xi*xi+a0*xi*xi*xi)))/(1./6/Ts*(a2+2*a1*xi+3*a0*xi*xi));
	//   	printf("xi=%f\n", xi);
#endif
    }
  return Ts*(xk_index+xi);
}
*/
/***********************************************************************************/

double pDigitalSignal::CubicSplineLTIInterpolation(int x2, double fmax, int Nrecurr)
{
  //double xi0=(fmax-(Double_t)this->At(x2))/((Double_t)this->At(x2+1)-(Double_t)this->At(x2));
  double data[4];
  int i;
  for(i=-1;i<3;i++){
    data[i+1]=0;
    if((x2+i>=0)&&(x2+i<this->GetN())) data[i+1]=(Double_t)this->At(x2-i);
  }
  double a3=-data[0]/6.+data[1]/2.-data[2]/2.+data[3]/6.;
  double a2=data[0]/2.-data[1]+data[2]/2.;
  double a1=-data[0]/2.+data[2]/2.;
  double a0=data[0]/6.+2.*data[1]/3.+data[2]/6.-fmax;
  double y1=data[0]+4*data[1]+data[2];
  double y2=data[1]+4*data[2]+data[3];
  double xi=(6.*fmax-y1)/(y2-y1);
  for(int rec=0;rec<Nrecurr;rec++)
    {
#if DBGLEVEL>0 
      double delta_xi=(a3*xi*xi*xi+a2*xi*xi+a1*xi+a0)/(3*a3*xi*xi+2*a2*xi+a1);
      printf("s[x2-1]=%e s[x2]=%e s[x2+1]=%e s[x2+2]=%e\n",(Double_t)this->At(x2-1),(Double_t)this->At(x2),(Double_t)this->At(x2+1),(Double_t)this->At(x2+2));
      printf("a0=%e a1=%e a2=%e a3=%e fmax=%e\n",a0,a1,a2,a3,fmax);
      printf("delta_xi[%d]=%e\n",rec+1, delta_xi);
      xi -= delta_xi;
#else
 	xi -=(a3*xi*xi*xi+a2*xi*xi+a1*xi+a0)/(3*a3*xi*xi+2*a2*xi+a1);
	//   	printf("xi=%f\n", xi);
#endif
    }
  return Ts*(x2+xi);
}

/***********************************************************************************/
double pDigitalSignal::PentaInterpolation(int x3, double fmax, int Nrecurr)
{
#if DBGLEVEL>0
  printf ("--- %s ----\n", __PRETTY_FUNCTION__);
#endif
 /*
    NOTA: tutti i conti fatti con i tempi in "canali". aggiungero' il *Ts
    solo nel return.
  */
  /**** 2) normal CFD ***************************************************/
  
   const Long64_t *data = this->GetArray();

  
  double xi0=(fmax-(Double_t)data[x3])/((Double_t)data[x3+1]-(Double_t)data[x3]);
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


  double a0=(Double_t)data[x3];
  double a1= 1/20 * (Double_t)data[x3-2] - 1/2 * (Double_t)data[x3-1] - 1/3 * (Double_t)data[x3] + (Double_t)data[x3+1] - 1/4 * (Double_t)data[x3+2] + 1/30 * (Double_t)data[x3+3];
  double a2 = 2/3 * (Double_t)data[x3-1] - 1/24 * (Double_t)data[x3-2] - 5/4 * (Double_t)data[x3] + 0.2/3 * (Double_t)data[x3+1] - 1/24 * (Double_t)data[x3+2]; 
  double a3 = 5/12 * (Double_t)data[x3] - 1/24 * (Double_t)data[x3-1] - 1/24 * (Double_t)data[x3-2] - 7/12 * (Double_t)data[x3+1] + 7/24 * (Double_t)data[x3+2] - 1/24 * (Double_t)data[x3+3]; 
  double a4 = 1/24 * (Double_t)data[x3-2] - 1/6 * (Double_t)data[x3-1] + 0.25 * (Double_t)data[x3] - 1/6 * (Double_t)data[x3+1] + 1/24 * (Double_t)data[x3+2];
  double a5 = 1/24 * (Double_t)data[x3-1] - 1/120 * (Double_t)data[x3-2] - 1/12 * (Double_t)data[x3] + 1/12 * (Double_t)data[x3+1] - 1/24 * (Double_t)data[x3+2] + 1/120 * (Double_t)data[x3+3];

  double xi=xi0;
  for(int rec=0;rec<Nrecurr;rec++)
    {
#if DBGLEVEL>0
      double delta_xi=(fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi-a4*xi*xi*xi*xi-a5*xi*xi*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi+4*a4*xi*xi*xi+5*a5*xi*xi*xi*xi);
      printf("s[x3-2]=%e s[x3-1]=%e s[x3]=%e s[x3+1]=%e s[x3+2]=%e s[x3+3]=%e\n",
	      (Double_t)data[x3-2],(Double_t)data[x3-1],(Double_t)data[x3],(Double_t)data[x3+1],(Double_t)data[x3+2],(Double_t)data[x3+3]);
      printf("a0=%e a1=%e a2=%e a3=%e a4=%e a5=%e  fmax=%e\n",a0,a1,a2,a3,a4,a5,fmax);
      printf("delta_xi[%d]=%e\n",rec+1, delta_xi);
      xi += delta_xi;
#else
      xi +=(fmax-a0-a1*xi-a2*xi*xi-a3*xi*xi*xi-a4*xi*xi*xi*xi-a5*xi*xi*xi*xi*xi)/(a1+2*a2*xi+3*a3*xi*xi+4*a4*xi*xi*xi+5*a5*xi*xi*xi*xi);
#endif
    }
  return Ts*(x3+xi);

}



/**********************************************/
double pDigitalSignal::CFD(double fraction, Long64_t Max, int polarity, int type, int Nrecurr, int reverse, double start)
{

  Long64_t *data=this->GetArray();
  int NSamples=GetN();
  double fmax=fraction*(Double_t)Max;
  /**** 1) ricerca del punto x2 tale che x2 e' il precedente di tcfd ****/
  int x2=0;
  if(reverse){
      x2=(int)(start/Ts);
      if(x2<=0 || x2>=NSamples) return -1;
      for(; x2>0 ; x2--){
	if(polarity>=0 && (Double_t)data[x2]<fmax)	  break;
	if(polarity<0 && (Double_t)data[x2]>fmax) 	  break;
      }
  }else{
      for(;x2<NSamples;x2++){
	if(polarity>=0 && (Double_t)data[x2]>fmax) break;
          if(polarity<0 && (Double_t)data[x2]<fmax)	break;
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
    // case CUBICSPLINE_CFD:
    //return CubicSplineInterpolation(x2, fmax, Nrecurr);
    //break;
  case PENTA_CFD:
    return PentaInterpolation(x2, fmax, Nrecurr);
    break;
  default:
    return LinearInterpolation(x2, fmax);
    break;
    /* case CUBICSPLINELTI_CFD:
    return CubicSplineLTIInterpolation(x2,fmax,Nrecurr);
    break;*/
  }
  
}


Long64_t  pDigitalSignal::TimeIntegral(double tstart, double tstop)
{
   int istart = (int)(tstart/GetTs()+0.5);
   int istop = (int) (tstop/GetTs()+0.5);
   
   Long64_t sum=0;
   for(int i=istart; i<=istop; i++) sum += At(i);
   
   return sum;
   
  
  
}

//============= Input/Output ================

void  pDigitalSignal::SaveAsASCII(char *filename){
  FILE *f;
  
  f = fopen(filename, "w");
  if (f==NULL){
    printf("%s: error opening file %s for writing\n",__PRETTY_FUNCTION__,filename);
    return;
  }
  fprintf(f,"#  Nbits,   Ts,   Signed,   nsamples,   range,   mstyle,  mcol,      lcol\n");   
  fprintf(f,"# %d,  %f,  %d,  %d, %f, %d, %d, %d\n",GetNbits(),GetTs(),GetSigned(),GetN(),GetRange(),GetMarkerStyle(),GetMarkerColor(),GetLineColor());   
  for(int i=0; i< GetN(); i++)
    fprintf(f,"%Ld\n", this->At(i));
  fclose(f);
}




// OPERATORS

/***************************************************************************/
pDigitalSignal pDigitalSignal::operator  = (const pDigitalSignal &a)
{

  this->Ts = a.Ts;
  this->Fs = a.Fs;
  this->range = a.range;
  this->Nbits= a.Nbits; 
  this->Signed = a.Signed;
  this->Mask=a.Mask;
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




/*************Signal Combination methods*************/

pDigitalSignal *pDigitalSignal::Add(pDigitalSignal &add, int ResBits){
   int i;
   SaturateSignal();
   if(ResBits>0) SetNbits(ResBits);
   int length=nsamples;
   if(add.GetN()<length) length=add.GetN();
   for(i=0;i<length;i++) this->SetAt(this->At(i)+add.At(i),i);
   SaturateSignal();
   return this;
}

pDigitalSignal *pDigitalSignal::Subtract(pDigitalSignal &sub,int ResBits){
  int i;
  SaturateSignal();
  if(ResBits>0) SetNbits(ResBits);
  int length=nsamples;
  if(sub.GetN()<length) length=sub.GetN();
  for(i=0;i<length;i++) this->SetAt(this->At(i)-sub.At(i),i);
  SaturateSignal();
  return this;
}

pDigitalSignal *pDigitalSignal::Mult(Long64_t factor,int ResBits){
  int i;
  SaturateSignal();
  if(ResBits>0) SetNbits(ResBits);
  for(i=0;i<nsamples;i++) this->SetAt(this->At(i)*factor,i);
  SaturateSignal();
  return this;
}


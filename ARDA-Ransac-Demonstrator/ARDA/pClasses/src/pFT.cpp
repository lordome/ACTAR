

#include "pFT.h"

ClassImp(pFT);

 pFT::pFT(int N){
  
  re = new TArrayD(N);
  im = new TArrayD(N);
  nsamples = N;
  Ts = 1.;
  units = new TString("Samples^-1");
  Fs=1.0;   // sampling frequency in GHz
  range=2.0; // peak-to-peak range, default +- 1V
  Nbits=14;  // number of bits for quantization
  fractional = 0; // set to 1 for fractional (i.e. abs(sample)<1) output 
 // graphic parameters
  mstyle = 4;
  mcol = 1;
  lcol = 1;
}

pFT::pFT(const pFT &a)
{

  this->Ts = a.Ts;
  this->nsamples = a.nsamples;
  this->re = new TArrayD(nsamples);
  this->im = new TArrayD(nsamples);
 
  for(int i=0;i<nsamples;i++){
   re->AddAt(a.re->At(i),i);
   im->AddAt(a.im->At(i),i);
  }
}

pSignal *pFT::Magnitude()
{
  pSignal *m = new pSignal(nsamples);

  for(int i=0; i<nsamples; i++) m->SetAt(sqrt(re->At(i)*re->At(i)+im->At(i)*im->At(i)),i);

  return m;
}

pSignal *pFT::Phase()
{
  pSignal *ph = new pSignal(nsamples);

  for(int i=0; i<nsamples; i++) {
     double fft_ph = atan2(im->At(i),re->At(i))*180./TMath::Pi();  // phase in degrees 
     double mag = sqrt(re->At(i)*re->At(i)+im->At(i)*im->At(i));
     if (TMath::Abs(im->At(i)) < 1e-13)fft_ph=0.;
     if (mag < 1e-10)fft_ph=0.;
     if (fft_ph < 1e-10)fft_ph=0.;
     ph->SetAt(fft_ph,i);
  }
  return ph;
}

pSignal *pFT::Real()
{
  pSignal *m = new pSignal(nsamples);

  for(int i=0; i<nsamples; i++) m->SetAt(re->At(i),i);

  return m;
}

pSignal *pFT::Imag()
{
  pSignal *m = new pSignal(nsamples);

  for(int i=0; i<nsamples; i++) m->SetAt(im->At(i),i);

  return m;
}



TObject *pFT::DrawReal(Option_t *option)
{
  TString o(option);
  pSignal *real = Real();
  if(o.Contains("h")){
    TH1F *h = real->Sig2Histo();
    if(units->Contains("Samples^-1"))
      h->GetXaxis()->SetLimits(0,0.5);     
    else if (units->Contains("GHz"))
      h->GetXaxis()->SetLimits(0,0.5*Fs);
     else if (units->Contains("rad"))
      h->GetXaxis()->SetLimits(0,TMath::Pi());
     else if (units->Contains("Samples"))
      h->GetXaxis()->SetLimits(0,nsamples);
     else
       printf("pFT::DrawReal: undefined units\n");
   h->GetXaxis()->SetTitle(units->Data());
   h->Draw();
   return (TObject *)h;
  }else{
    TGraph *g = real->Sig2Graph();
    // double *x = g->GetX();
    double *y = g->GetY();
    if(units->Contains("Samples^-1"))
      for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii/(g->GetN()-1)*0.5, y[ii]);
     else if (units->Contains("GHz"))
       for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii/(g->GetN()-1)*0.5*Fs,y[ii]);
     else if (units->Contains("rad"))
       for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii/(g->GetN()-1)*TMath::Pi(), y[ii]);
     else if (units->Contains("Samples"))
          for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii, y[ii]);
     else
       printf("pFT::DrawReal: undefined units\n");
     
     g->SetMarkerStyle(20);
     g->SetMarkerSize(1);
     
     g->Draw(option);
         g->GetXaxis()->SetTitle(units->Data());
         return (TObject *)g;
 }
  delete real;
}


TObject *pFT::DrawImag(Option_t *option)
{
  TString o(option);
  pSignal *imag = Imag();
  if(o.Contains("h")){
    TH1F *h = imag->Sig2Histo();
    if(units->Contains("Samples^-1"))
      h->GetXaxis()->SetLimits(0,0.5);     
    else if (units->Contains("GHz"))
      h->GetXaxis()->SetLimits(0,0.5*Fs);
     else if (units->Contains("rad"))
      h->GetXaxis()->SetLimits(0,TMath::Pi());
     else if (units->Contains("Samples"))
      h->GetXaxis()->SetLimits(0,nsamples);
     else
       printf("pFT::DrawImag: undefined units\n");
   h->GetXaxis()->SetTitle(units->Data());
   h->Draw();
   return (TObject *)h;
  }else{
    TGraph *g = imag->Sig2Graph();
    //double *x = g->GetX();
    double *y = g->GetY();
    if(units->Contains("Samples^-1"))
      for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii/(g->GetN()-1)*0.5, y[ii]);
     else if (units->Contains("GHz"))
       for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii/(g->GetN()-1)*0.5*Fs,y[ii]);
     else if (units->Contains("rad"))
       for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii/(g->GetN()-1)*TMath::Pi(), y[ii]);
     else if (units->Contains("Samples"))
          for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii, y[ii]);
     else
       printf("pFT::DrawImag: undefined units\n");

     g->SetMarkerStyle(20);
     g->SetMarkerSize(1);
     g->Draw(option);
     g->GetXaxis()->SetTitle(units->Data());
     return (TObject *)g;
 }
  delete imag;
}

TH1F *pFT::FT2MagHisto()
{
  pSignal *mag = Magnitude();

    TH1F *h = mag->Sig2Histo();
    if(units->Contains("Samples^-1"))
      h->GetXaxis()->SetLimits(0,0.5);     
    else if (units->Contains("GHz"))
      h->GetXaxis()->SetLimits(0,0.5*Fs);
     else if (units->Contains("rad"))
      h->GetXaxis()->SetLimits(0,TMath::Pi());
     else if (units->Contains("Samples"))
      h->GetXaxis()->SetLimits(0,nsamples);
     else
       printf("pFT::DrawMagnitude: undefined units\n");
   h->GetXaxis()->SetTitle(units->Data());
  delete mag;
   return h;
}

TObject * pFT::DrawPhase(Option_t *option)
{
  TString o(option);
  pSignal *ph = Phase();
  if(o.Contains("h")){
    TH1F *h = ph->Sig2Histo();
    if(units->Contains("Samples^-1"))
      h->GetXaxis()->SetLimits(0,0.5);     
    else if (units->Contains("GHz"))
      h->GetXaxis()->SetLimits(0,0.5*Fs);
     else if (units->Contains("rad"))
      h->GetXaxis()->SetLimits(0,TMath::Pi());
     else if (units->Contains("Samples"))
      h->GetXaxis()->SetLimits(0,nsamples);
     else
       printf("pFT::DrawPhase: undefined units\n");
   h->GetXaxis()->SetTitle(units->Data());
    h->Draw();
         return (TObject *)h;
  }else{
    TGraph *g = ph->Sig2Graph();
    //double *x = g->GetX();
    double *y = g->GetY();
    if(units->Contains("Samples^-1"))
           for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii/g->GetN()*0.5, y[ii]);
     else if (units->Contains("GHz"))
          for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii/g->GetN()*0.5*Fs,y[ii]);
     else if (units->Contains("rad"))
          for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii/g->GetN()*TMath::Pi(), y[ii]);
     else if (units->Contains("Samples"))
          for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii, y[ii]);
     else
       printf("pFT::DrawPhase: undefined units\n");
     
     g->SetMarkerStyle(20);
     g->SetMarkerSize(1);
     g->Draw(option);
     g->GetXaxis()->SetTitle(units->Data());
     return (TObject *)g;

  }
  delete ph;
}

TObject *pFT::DrawMagnitude(Option_t *option)
{
  TString o(option);
  pSignal *mag = Magnitude();
  if(o.Contains("h")){
    TH1F *h = mag->Sig2Histo();
    if(units->Contains("Samples^-1"))
      h->GetXaxis()->SetLimits(0,0.5);     
    else if (units->Contains("GHz"))
      h->GetXaxis()->SetLimits(0,0.5*Fs);
     else if (units->Contains("rad"))
      h->GetXaxis()->SetLimits(0,TMath::Pi());
     else if (units->Contains("Samples"))
      h->GetXaxis()->SetLimits(0,nsamples);
     else
       printf("pFT::DrawMagnitude: undefined units\n");
   h->GetXaxis()->SetTitle(units->Data());
   h->Draw();
   return (TObject *)h;
  }else{
    TGraph *g = mag->Sig2Graph();
    //double *x = g->GetX();
    double *y = g->GetY();
    if(units->Contains("Samples^-1"))
      for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii/(g->GetN()-1)*0.5, y[ii]);
     else if (units->Contains("GHz"))
       for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii/(g->GetN()-1)*0.5*Fs,y[ii]);
     else if (units->Contains("rad"))
       for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii/(g->GetN()-1)*TMath::Pi(), y[ii]);
     else if (units->Contains("Samples"))
          for(int ii=0; ii<g->GetN(); ii++) g->SetPoint(ii,(double)ii, y[ii]);
     else
       printf("pFT::DrawMagnitude: undefined units\n");
     g->SetMarkerStyle(20);
     g->SetMarkerSize(1);
     g->Draw(option);
         g->GetXaxis()->SetTitle(units->Data());
         return (TObject *)g;
 }
  delete mag;
}



pFT pFT::operator  = (const pFT &so)
{
  this->nsamples = so.nsamples;
  this->Ts =so.Ts;
  re->Set(nsamples);
  im->Set(nsamples);
  for(int i=0;i<nsamples;i++){
    this->re->AddAt(re->At(i),i);
    this->im->AddAt(im->At(i),i);
//     printf("i=%d   %f\n",i, s->At(i));
  }
  return *this;
}



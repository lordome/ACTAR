#include "pSplineArcSignal.h"

 ClassImp(pSplineArcSignal); 

//============= CONSTRUCTORS================ 
pSplineArcSignal::pSplineArcSignal(){
	Init();
}

pSplineArcSignal::pSplineArcSignal(int N){
	Init();
	SetN(N);
}

pSplineArcSignal::pSplineArcSignal(const pSplineArcSignal &a){
  this->Ts = a.Ts;
  this->Fs = a.Fs;
  this->nsamples = a.nsamples;
  int i;
  for(i=0;i<4;i++) this->coeffs[i]=a.coeffs[i];
  this->samples=a.samples;
  this->mstyle = a.mstyle;
  this->mcol = a.mcol;
  this->lcol = a.lcol;
  this->Tdel1=a.Tdel1;
  this->Tdel2=a.Tdel2;
  this->DAmpl=a.DAmpl;
}

pSplineArcSignal::pSplineArcSignal(pSignal a,double delay1,double delay2, double ampl,int nbits,int do_smoothing,double l){
	Init();
	this->DAmpl=ampl;
	this->Tdel1=delay1;
	this->Tdel2=delay2;
	this->Ts = a.GetTs();
	this->Fs = a.GetFs();
	//first, generate spline coefficients samples
	pDSP dsp;
	pSignal *bcoeff;/////////////////////////////////////////////////////////////////////////////////// no delete
	if(do_smoothing==0) bcoeff=dsp.BuildCubicSplineCoeffSignal(nbits,&a);
	else bcoeff=dsp.BuildSmoothingSplineCoeffSignal(nbits,l,&a);
	SetN(bcoeff->GetN()-3);
	//evaluating coeffs....
	int i;
	for(i=0;i<nsamples;i++){
		coeffs[0].SetAt(bcoeff->At(i)/6.+2.*bcoeff->At(i+1)/3.+bcoeff->At(i+2)/6.,i);
		coeffs[1].SetAt(-bcoeff->At(i)/2.+bcoeff->At(i+2)/2.,i);
		coeffs[2].SetAt(bcoeff->At(i)/2.-bcoeff->At(i+1)+bcoeff->At(i+2)/2.,i);
		coeffs[3].SetAt(-bcoeff->At(i)/6.+bcoeff->At(i+1)/2.-bcoeff->At(i+2)/2.+bcoeff->At(i+3)/6.,i);
	}
	for(i=0;i<nsamples;i++) samples.SetAt(EvalAt(i*Ts),i);

	delete bcoeff; ///////////////////////////////////////////////////////////////////////////////////// 8 06
}

TGraph *pSplineArcSignal::Sig2Graph(int Ups){
  
  TGraph *g = new TGraph();
  for(int i=0; i<nsamples*Ups;i++){
    g->SetPoint(i,i*Ts/(double)Ups,this->EvalAt(i*Ts/(double)Ups));
  }
  g->SetMarkerSize(0.5);
  //  g->SetMarkerStyle(4);
  g->SetMarkerStyle(mstyle);
  g->SetMarkerColor(mcol);
  g->SetLineColor(lcol);
  g->GetXaxis()->SetTitle("t (ns)");

  return g;
  
  
}

//_____________________________________________________________________________
TH1F *pSplineArcSignal::Sig2Histo(int Ups){
  
  TH1F *h = new TH1F("pSignal","pSignal",nsamples*Ups,0,nsamples*Ts);
  h->GetXaxis()->SetTitle("ns");
  
  for(int i=0; i<nsamples*Ups;i++) h->SetBinContent(i+1,this->EvalAt(i*Ts/(double)Ups));
  h->GetXaxis()->SetTitle("t (ns)");

  return h;
  
  
}	
			
void pSplineArcSignal::Draw(int Ups ,Option_t *option){
  TString o(option);
  if(o.Contains("h")){
    TH1F *h = Sig2Histo(Ups);
    h->Draw();
  }else{
    TGraph *g = Sig2Graph(Ups);
    g->Draw(option);
  }
}
	
	
//=================Init================//
void pSplineArcSignal::Init()
{

  Ts=1.0;   // sampling period in ns
  Fs=1.0;   // sampling frequency in GHz
  nsamples=0;  // number of samples in pSignal
  mstyle = 4;
  mcol = 1;
  lcol = 1;
  Tdel1=2;
  Tdel2=10;
  DAmpl=1;
}

double pSplineArcSignal::FindZeroCrossing(int nrecurr){
	//build samples signal and find minimum index;
	int i;
	for(i=0;i<nsamples;i++) samples.SetAt(EvalAt(i*Ts),i);
	double min;
	int istart=samples.MaxMin(&min,0);
	//evaluate fractional delay remainders and order them
	double del[3];
	double temp;
	del[0]=0;
	del[1]=Tdel1/Ts-floor(Tdel1/Ts);
	del[2]=Tdel2/Ts-floor(Tdel1/Ts);
	if(del[2]<del[1]) temp=del[1];
	del[1]=del[2];
	del[2]=temp;
	double tright,tleft;
	tleft=istart*Ts;
	//find zero crossing interval
	for(i=istart-1;i>=0;i--){
		tright=tleft;
		tleft=(i+del[2])*Ts;
		if(EvalAt(tleft)>=0)break;
		tright=tleft;
		tleft=(i+del[1])*Ts;
		if(EvalAt(tleft)>=0)break;
		tright=tleft;
		tleft=(i+del[0])*Ts;
		if(EvalAt(tleft)>=0)break;
	}
	//build coefficients
	int icoeff;
	double dt;
	double C0[4],C1[4],C2[4];
	double C[4];
	//C0
	icoeff=floor(tleft/Ts);
	dt=tleft/Ts-icoeff;
	C0[3]=coeffs[3].At(icoeff);
	C0[2]=coeffs[2].At(icoeff)+3*coeffs[3].At(icoeff)*dt;
	C0[1]=coeffs[1].At(icoeff)+2*coeffs[2].At(icoeff)*dt+3*coeffs[3].At(icoeff)*pow(dt,2);
	C0[0]=coeffs[0].At(icoeff)+coeffs[1].At(icoeff)*dt+coeffs[2].At(icoeff)*pow(dt,2)+coeffs[3].At(icoeff)*pow(dt,3);
	//C1
	icoeff=floor((tleft-Tdel1)/Ts);
	dt=(tleft-Tdel1)/Ts-icoeff;
	C1[3]=coeffs[3].At(icoeff);
	C1[2]=coeffs[2].At(icoeff)+3*coeffs[3].At(icoeff)*dt;
	C1[1]=coeffs[1].At(icoeff)+2*coeffs[2].At(icoeff)*dt+3*coeffs[3].At(icoeff)*pow(dt,2);
	C1[0]=coeffs[0].At(icoeff)+coeffs[1].At(icoeff)*dt+coeffs[2].At(icoeff)*pow(dt,2)+coeffs[3].At(icoeff)*pow(dt,3);
	//C2
	icoeff=floor((tleft-Tdel2)/Ts);
	dt=(tleft-Tdel2)/Ts-icoeff;
	C2[3]=coeffs[3].At(icoeff);
	C2[2]=coeffs[2].At(icoeff)+3*coeffs[3].At(icoeff)*dt;
	C2[1]=coeffs[1].At(icoeff)+2*coeffs[2].At(icoeff)*dt+3*coeffs[3].At(icoeff)*pow(dt,2);
	C2[0]=coeffs[0].At(icoeff)+coeffs[1].At(icoeff)*dt+coeffs[2].At(icoeff)*pow(dt,2)+coeffs[3].At(icoeff)*pow(dt,3);


//	printf("P(t)=");
	for(i=0;i<4;i++){
		 C[i]=C0[i]-DAmpl*C1[i]+(DAmpl-1)*C2[i];
		// printf("%lf(%lfx-%lf)^(%d)+",C[i],Ts,tleft,i);
	 }
	// printf("\n");

	//First guess
	double xleft=0;
	double xright=(tright-tleft)/Ts;
	double yleft=C[0];
	double yright=C[0]+C[1]*xright+C[2]*pow(xright,2)+C[3]*pow(xright,3);
	double xcross=(xleft-xright)*yleft/(yright-yleft)+xleft;
	double f,df;
	for(i=0;i<nrecurr;i++){
		f=C[0]+C[1]*xcross+C[2]*pow(xcross,2)+C[3]*pow(xcross,3);
		df=C[1]+2*C[2]*xcross+3*C[3]*pow(xcross,2);
		xcross-=f/df;
	//	cout<<xcross<<endl;
	}
	return tleft+xcross*Ts;	
}

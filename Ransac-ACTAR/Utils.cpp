///////////////////////////////////////
//                                   //
// T. Roger - GANIL 2015             //
// email: roger@ganil.fr             //
//                                   //
// Utils misc. functions file:       //
//     - Automatic pad calibration   //
//     - Pad treatment functions     //
//     - ....                        //
//                                   //
///////////////////////////////////////

#include <TCanvas.h>
#include <TGraph.h>
#include <TSpectrum.h>
#include <math.h>
#include <TVector3.h>

#include <stdio.h>
#include <iostream>
#include <Parameters.h>

#include <Utils.h>


#define R2D 57.2957795
#define D2R 0.01745329
#define PI  3.14159265

using namespace std;



void Calibrate(TH2I* hPSummary)
{
	TCanvas* Ccal=new TCanvas("Ccal","Ccal",600,600);
	TH2I* padsummary_cal=new TH2I("padsummary_cal","padsummary_cal",NB_COBO*NB_ASAD*NB_AGET*NB_CHANNEL,0,NB_COBO*NB_ASAD*NB_AGET*NB_CHANNEL, hPSummary->GetNbinsY(),-16,4096);
	TH1D* hPad;
	FILE* fcal=fopen("cal/allign_charge_new.dat","w");
	FILE* fdat=fopen("cal/peaks_new.dat","w");	
	int ncobo, nasad, naget, nchannel;
	int npeaks;
	char OK;
	int cobo0, cobo1;
	
	float min_peak_pct=0.3;
	
	do
	{
		cout << "which cobos need to be calibrated? (first last): ";
		cin >> cobo0 >> cobo1; 
		cout << "choose a reference (cobo asad aget channel): ";
		cin >> ncobo >> nasad >> naget >> nchannel;
		cout << "reference is now cobo " << ncobo << " asad " << nasad << " aget " << naget << "  channel " << nchannel << endl;
		hPad=hPSummary->ProjectionY("",ncobo*NB_ASAD*NB_AGET*NB_CHANNEL + nasad*NB_AGET*NB_CHANNEL + naget*NB_CHANNEL + nchannel +1,ncobo*NB_ASAD*NB_AGET*NB_CHANNEL + nasad*NB_AGET*NB_CHANNEL + naget*NB_CHANNEL + nchannel +1);
		hPad->Draw();
		Ccal->Update();
		cout << "Is reference channel OK (y/n)?" << endl;
		cin >> OK;
	}
	while(OK!='y');
	cout << "how many peaks should be used? ";
	cin >> npeaks;
	
	float CAL[NB_COBO*NB_ASAD*NB_AGET*NB_CHANNEL][2];
	
	TSpectrum* S;
	TF1* fitgaus=new TF1("fitgaus","gaus",10,4090);
	TF1* fitpol1=new TF1("fitpol1","pol1",10,4090);
	TGraph* Gallign=new TGraph(npeaks);


	double *REF=0;
	S=new TSpectrum(100,2);
	bool isOK=false;
	do
	{
		S->Search(hPad,10,"nobackground noMarkov",min_peak_pct);
		REF=S->GetPositionX();
		Ccal->Update();
		for (int i=0;i<npeaks;i++)
			for (int j=0;j<npeaks-1;j++)
				if (REF[j]>REF[j+1])
				{
					float var=REF[j];
					REF[j]=REF[j+1];
					REF[j+1]=var;
				}
		if(npeaks==S->GetNPeaks()) isOK=true;
		else
		{
			cout << "Could not find " << npeaks << " peaks with the current setting \nPlease enter new search limit" << endl;
			cin >> min_peak_pct;
		}
	}
	while(!isOK);
// 	Ccal->WaitPrimitive();
	
	for(int i=0;i<npeaks;i++)
	{
		hPad->Fit(fitgaus,"RQ","",REF[i]-10,REF[i]+10);
// 		if(i==0) hPad->Fit(fitgaus,"RQ","",0,REF[0]+(REF[1]-REF[0])/2.);
// 		else if(i==npeaks-1) hPad->Fit(fitgaus,"RQ","",REF[npeaks-1]-(REF[npeaks-1]-REF[npeaks-2])/2.,4095);
// 		else hPad->Fit(fitgaus,"RQ","",REF[i]-(REF[i]-REF[i-1])/2.,REF[i]+(REF[i+1]-REF[i])/2.);
		REF[i]=fitgaus->GetParameter(1);
		cout << "Unordered Reference peak #" << i+1 << ": mean = " << fitgaus->GetParameter(1) << "  sigma = " << fitgaus->GetParameter(2) << endl;
	}
	for (int i=0;i<npeaks;i++)
		for (int j=0;j<npeaks-1;j++)
			if (REF[j]>REF[j+1])
			{
				float var=REF[j];
				REF[j]=REF[j+1];
				REF[j+1]=var;
			}
	for(int i=0;i<npeaks;i++) cout << "Reference peak #" << i+1 << ": at " << REF[i] << endl;
	Ccal->Update();
	

	TSpectrum* s=new TSpectrum(100,2);
	for(int cobo=cobo0;cobo<=cobo1;cobo++)
		for(int asad=0;asad<NB_ASAD ;asad++)
	for(int aget=0;aget<NB_AGET;aget++)
		for(int channel=0;channel<NB_CHANNEL ;channel++)
		{
			double *pos=0;
			hPad=hPSummary->ProjectionY("",cobo*NB_ASAD*NB_AGET*NB_CHANNEL + asad*NB_AGET*NB_CHANNEL + aget*NB_CHANNEL + channel +1,cobo*NB_ASAD*NB_AGET*NB_CHANNEL + asad*NB_AGET*NB_CHANNEL + aget*NB_CHANNEL + channel +1);
			s->Search(hPad,10,"nobackground noMarkov",min_peak_pct);
			pos=s->GetPositionX();
			
// 			hPad->Draw();
// 			Ccal->Update();
// 			Ccal->WaitPrimitive();
			if(s->GetNPeaks()==npeaks)
			{
				for(int loop=0;loop<2;loop++)
					for (int i=0;i<npeaks;i++)
						for (int j=0;j<npeaks-1;j++)
							if (pos[j]>pos[j+1])
							{
								double var=pos[j];
								pos[j]=pos[j+1];
								pos[j+1]=var;
							}
// 				for(int i=0;i<npeaks;i++) cout << "Found peak #" << i+1 << ": at " << pos[i] << endl;
							
				for(int i=0;i<npeaks;i++)
				{
					hPad->Fit(fitgaus,"RQ","",pos[i]-10,pos[i]+10);
// 					if(i==0) hPad->Fit(fitgaus,"RQ","",0,pos[0]+(pos[1]-pos[0])/2.);
// 					else if(i==npeaks-1) hPad->Fit(fitgaus,"RQ","",pos[npeaks-1]-(pos[npeaks-1]-pos[npeaks-2])/2.,4095);
// 					else hPad->Fit(fitgaus,"RQ","",pos[i]-(pos[i]-pos[i-1])/2.,pos[i]+(pos[i+1]-pos[i])/2.);
					Gallign->SetPoint(i,fitgaus->GetParameter(1),REF[i]);
					fprintf(fdat,"%d %d %d %d %d\t%f\n",cobo,asad,aget,channel,i+1,fitgaus->GetParameter(1));
				}
				Gallign->Fit(fitpol1,"RQ","",0,4095);
// 				Gallign->Draw("AP*");
// 				Ccal->Update();
// 				Ccal->WaitPrimitive();
				fprintf(fcal,"%d\t%d\t%d\t%d\t%.2f\t%.6f\n",cobo,asad,aget,channel,fitpol1->GetParameter(0),fitpol1->GetParameter(1));

			}
			else
			{
				if(channel!=11 && channel !=22 && channel!=45 && channel!=56)
				{
					cout << "cobo " << cobo << " asad " << asad << " aget " << aget << "  channel " << channel << " cannot be calibrated: found " << s->GetNPeaks() << " peaks instead of " << npeaks << endl;
					hPad->Draw();
					Ccal->Update();
					Ccal->WaitPrimitive();
				}
				fprintf(fcal,"%d\t%d\t%d\t%d\t%.2f\t%.6f\n",cobo,asad,aget,channel,0.,1.);

			}
			CAL[cobo*NB_ASAD*NB_AGET*NB_CHANNEL + asad*NB_AGET*NB_CHANNEL + aget*NB_CHANNEL + channel][0]=fitpol1->GetParameter(0);
			CAL[cobo*NB_ASAD*NB_AGET*NB_CHANNEL + asad*NB_AGET*NB_CHANNEL + aget*NB_CHANNEL + channel][1]=fitpol1->GetParameter(1);
		}
	
	fclose(fcal);
	fclose(fdat);
	for(int i=1;i<=NB_COBO*NB_ASAD*NB_AGET*NB_CHANNEL;i++)
		for(int j=1;j<=padsummary_cal->GetNbinsY();j++)
			padsummary_cal->Fill(i-1,CAL[i-1][0]+CAL[i-1][1]*hPSummary->GetYaxis()->GetBinCenter(j),hPSummary->GetBinContent(i,j));
	padsummary_cal->Draw("colz");
	Ccal->Update();
	Ccal->WaitPrimitive();
}


void CleanPad(float PAD[NPADY][NPADX],float TIME[NPADY][NPADX], float THR)
{
	short NEIGHBOUR[NPADY][NPADX];
	for(int r=0;r<NPADY;r++)
		for(int c=0;c<NPADX;c++)
		{
			NEIGHBOUR[r][c]=0;
			if(r>0)
			{
				if(c>0 && PAD[r-1][c-1]) NEIGHBOUR[r][c]++;
				if(c<NPADX-1 && PAD[r-1][c+1]) NEIGHBOUR[r][c]++;
				if(PAD[r-1][c]>THR) NEIGHBOUR[r][c]++;
			}
			if(r<NPADY-1)
			{
				if(c>0 && PAD[r+1][c-1]) NEIGHBOUR[r][c]++;
				if(c<NPADX-1 && PAD[r+1][c+1]) NEIGHBOUR[r][c]++;
				if(PAD[r+1][c]>THR) NEIGHBOUR[r][c]++;
			}
			if(c>0 && PAD[r][c-1]) NEIGHBOUR[r][c]++; 
			if(c<NPADX-1 && PAD[r][c+1]) NEIGHBOUR[r][c]++; 
			
// 			if(c>=40 && c<=47 && r<=10) NEIGHBOUR[r][c]=3;
// 			if(c<30 && r<12) NEIGHBOUR[r][c]=0;
		}
	
	for(int r=0;r<NPADY;r++)
		for(int c=0;c<NPADX;c++)
		{
			if(r>0 && c>0 && r<NPADY-1 && c<NPADX-1 && NEIGHBOUR[r][c]<3) PAD[r][c]=TIME[r][c]=0;
			if((r==0 || c==0 || r==NPADY-1 || c==NPADX-1) && NEIGHBOUR[r][c]<2) PAD[r][c]=TIME[r][c]=0;
		}
}

bool GetVertexFromRMS(TH2F* visu_charge,int& i_vertex,int& i_end)
{
	float BLM=0;
	float BLRMS=0;
	float RMSdev=2;
	int cpt=0;
	i_vertex=NPADX;
	i_end=0;
	for(int c=NPADX-2;c>=NPADX-9;c--)
	{
		BLRMS+=visu_charge->ProjectionY("",c,c)->GetRMS();
		BLM+=visu_charge->ProjectionY("",c,c)->GetMean();
	}
	BLRMS/=7.;
	BLM/=7.;
	
	if(BLM<15 || BLM>20) return(false);
	
	for(int c=NPADX-10;c>=0;c--)
	{
		if(visu_charge->ProjectionY("",c,c)->GetRMS()>RMSdev*BLRMS)
		{
			cpt++;
			i_vertex=c;
		}
		else
		{
			cpt=0;
			i_vertex=NPADX;
		}
		if(cpt>2) break;
	}
	cpt=0;
	for(int c=i_vertex+2;c>=0;c--)
	{
		if(visu_charge->ProjectionY("",c,c)->GetRMS()<RMSdev*BLRMS)
		{
			cpt++;
			i_end=c;
		}
		else
		{
			cpt=0;
			i_end=0;
		}
		if(cpt>2) break;
	}
	if(cpt>2) return(true);
	else return(false);
}


bool GetVerticalConsistensy(MTrack* T1,MTrack* T2)
{
	double a1=(T1->Yh-T1->Ym)/(T1->Xh-T1->Xm);
	double a2=(T2->Yh-T2->Ym)/(T2->Xh-T2->Xm);
	double b1=T1->Ym-a1*T1->Xm;
	double b2=T2->Ym-a2*T2->Xm;
	
	double x=(b2-b1)/(a1-a2);
	double y=a1*x+b1;
	
	double alpha1=(y-T1->Yh)/(T1->Yh-T1->Ym);
	double alpha2=(y-T2->Yh)/(T2->Yh-T2->Ym);
	
	double Dz=alpha2*(T2->Zh-T2->Zm)+T2->Zh-(alpha1*(T1->Zh-T1->Zm)+T1->Zh);
	
	if(fabs(Dz)<5) return(true);
	else return(false);
}


float AverageEloss(MTrack* T, float PAD[NPADY][NPADX])
{
	float Sum=0;
	float a=(T->Yh-T->Ym)/(T->Xh-T->Xm);
	float b=T->Ym-a*T->Xm;
	for(int r=T->zy_s;r<=NPADY;r++)
		for(int c=0;c<NPADX;c++)
			if((a*(c+1.)+b-(r+1.))/sqrt(a*a+1.) < 5.)
				Sum+=PAD[r][c];
	
	float length;
	if(T->Ym-(T->Yh-T->Ym)*T->Xm/(T->Xh-T->Xm) > NPADY)
	{
		length=sqrt(pow(T->Xm+(T->Xh-T->Xm)*(T->zy_s-T->Ym)/(T->Yh-T->Ym) - (T->Xm+(T->Xh-T->Xm)*(NPADY-T->Ym)/(T->Yh-T->Ym)),2)+pow(NPADY-T->zy_s,2));
		T->L2DXY->SetX1(T->Xm+(T->Xh-T->Xm)*(T->zy_s-T->Ym)/(T->Yh-T->Ym));
		T->L2DXY->SetX2(T->Xm+(T->Xh-T->Xm)*(NPADY-T->Ym)/(T->Yh-T->Ym));
		T->L2DXY->SetY1(T->zy_s);
		T->L2DXY->SetY2(NPADY);
	}
	else
	{
		length=sqrt(pow(T->Xm+(T->Xh-T->Xm)*(T->zy_s-T->Ym)/(T->Yh-T->Ym),2)+pow(T->Ym-(T->Yh-T->Ym)*T->Xm/(T->Xh-T->Xm)- T->zy_s,2));
		T->L2DXY->SetX1(T->Xm+(T->Xh-T->Xm)*(T->zy_s-T->Ym)/(T->Yh-T->Ym));
		T->L2DXY->SetX2(0);
		T->L2DXY->SetY1(T->zy_s);
		T->L2DXY->SetY2(T->Ym-(T->Yh-T->Ym)*T->Xm/(T->Xh-T->Xm));
	}
	
	length=sqrt(pow(length,2)+pow(T->Zm+(T->Zh-T->Zm)*(T->L2DXY->GetY1()-T->Ym)/(T->Yh-T->Ym) - T->Zm+(T->Zh-T->Zm)*(T->L2DXY->GetY2()-T->Ym)/(T->Yh-T->Ym) ,2));
// 	cout << length << endl;
	
	return(Sum/length);
}


void FitMat(float PAD[NPADY][NPADX], int Rmin, int Rmax, int Cmin, int Cmax, float threshold, float &a, float &b)
{
	int Row, Col;
	float A, B, C, UEV, Q, X, Xg, Y, Yg;
	A=B=C=UEV=Q=X=Y=Xg=Yg=0.;
	
	for (Row=Rmin;Row<=Rmax;Row++)
		for (Col=Cmin;Col<=Cmax;Col++)
			if(PAD[Row][Col]>threshold)
			{
				X= Col*2.+1.;
				Y= Row*2.+1.;
				Q+=PAD[Row][Col];
				Xg+=X*PAD[Row][Col];
				Yg+=Y*PAD[Row][Col];
			}
	Xg/=Q;
	Yg/=Q;
	
	for (Row=Rmin;Row<=Rmax;Row++)
		for(Col=Cmin;Col<=Cmax;Col++)
			if(PAD[Row][Col]>threshold)
			{
				X= Col*2.+1.;
				Y= Row*2.+1.;
				A+=PAD[Row][Col]*(X-Xg)*(X-Xg);
				B+=PAD[Row][Col]*(X-Xg)*(Y-Yg);
				C+=PAD[Row][Col]*(Y-Yg)*(Y-Yg);
			}
	UEV=0.5*(A+C+sqrt((A+C)*(A+C)-4*(A*C-B*B)));
	a=B/(UEV-C);
	b=Yg-a*Xg;
}


float FitMat(std::vector <int> vX, std::vector <int> vY, std::vector <float> vQ, std::vector <int> inliner, float &a, float &b)
{
	int Row, Col;
	float A, B, C, UEV, Q, Xg, Yg;
	A=B=C=UEV=Q=Xg=Yg=0.;
	
	for(unsigned int ind=0;ind<inliner.size();ind++)
	{
		Q+=vQ[ind];
		Xg+=vX[ind]*vQ[ind];
		Yg+=vY[ind]*vQ[ind];
	}
	Xg/=Q;
	Yg/=Q;
	
	for(unsigned int ind=0;ind<inliner.size();ind++)
	{
		A+=vQ[ind]*(vX[ind]-Xg)*(vX[ind]-Xg);
		B+=vQ[ind]*(vX[ind]-Xg)*(vY[ind]-Yg);
		C+=vQ[ind]*(vY[ind]-Yg)*(vY[ind]-Yg);
	}
	UEV=0.5*(A+C+sqrt((A+C)*(A+C)-4*(A*C-B*B)));
	a=B/(UEV-C);
	b=Yg-a*Xg;
	
	float chi2=0;
	for(unsigned int ind=0;ind<inliner.size();ind++)
		chi2+=vQ[ind]*pow(a*vX[ind]-vY[ind]+b,2)/(a*a+1.);
	chi2/=Q;	
	return(chi2);
}


float FitMat3D(std::vector<float> vX, std::vector<float> vY, std::vector<float> vZ, std::vector<float> vQ, float threshold, MTrack* T)
{
	// adapted from: http://fr.scribd.com/doc/31477970/Regressions-et-trajectoires-3D
	
	int Rmin=T->zy_s;
	int Rmax=T->zy_e;
	int Cmin=T->zx_s;
	int Cmax=T->zx_e;
	int R, C;
	double Q,X,Y,Z;
	double Xm,Ym,Zm;
	double Sxx,Sxy,Syy,Sxz,Szz,Syz;
	double theta;
	double K11,K22,K12,K10,K01,K00;
	double c0,c1,c2;
	double p,q,r,dm2;
	double rho,phi;
	double a,b;
	
	Q=Xm=Ym=Zm=0.;
	Sxx=Syy=Szz=Sxy=Sxz=Syz=0.;
	
	for(unsigned int i=0;i<vX.size();i++)
		if(vQ[i]>threshold)
		{
			X= vX[i];
			Y= vY[i];
			Z= vZ[i];//*VDRIFT;
			Q+=vQ[i]/10.;
			Xm+=X*vQ[i]/10.;
			Ym+=Y*vQ[i]/10.;
			Zm+=Z*vQ[i]/10.;
			Sxx+=X*X*vQ[i]/10.;
			Syy+=Y*Y*vQ[i]/10.;
			Szz+=Z*Z*vQ[i]/10.;
			Sxy+=X*Y*vQ[i]/10.;
			Sxz+=X*Z*vQ[i]/10.;
			Syz+=Y*Z*vQ[i]/10.;
		}
	Xm/=Q;
	Ym/=Q;
	Zm/=Q;
	Sxx/=Q;
	Syy/=Q;
	Szz/=Q;
	Sxy/=Q;
	Sxz/=Q;
	Syz/=Q;
	Sxx-=(Xm*Xm);
	Syy-=(Ym*Ym);
	Szz-=(Zm*Zm);
	Sxy-=(Xm*Ym);
	Sxz-=(Xm*Zm);
	Syz-=(Ym*Zm);
	
	theta=0.5*atan((2.*Sxy)/(Sxx-Syy));
	
	K11=(Syy+Szz)*pow(cos(theta),2)+(Sxx+Szz)*pow(sin(theta),2)-2.*Sxy*cos(theta)*sin(theta);
	K22=(Syy+Szz)*pow(sin(theta),2)+(Sxx+Szz)*pow(cos(theta),2)+2.*Sxy*cos(theta)*sin(theta);
	K12=-Sxy*(pow(cos(theta),2)-pow(sin(theta),2))+(Sxx-Syy)*cos(theta)*sin(theta);
	K10=Sxz*cos(theta)+Syz*sin(theta);
	K01=-Sxz*sin(theta)+Syz*cos(theta);
	K00=Sxx+Syy;
	
	c2=-K00-K11-K22;
	c1=K00*K11+K00*K22+K11*K22-K01*K01-K10*K10;
	c0=K01*K01*K11+K10*K10*K22-K00*K11*K22;
	
	
	p=c1-pow(c2,2)/3.;
	q=2.*pow(c2,3)/27.-c1*c2/3.+c0;
	r=pow(q/2.,2)+pow(p,3)/27.;
	
	
	if(r>0) dm2=-c2/3.+pow(-q/2.+sqrt(r),1./3.)+pow(-q/2.-sqrt(r),1./3.);
	if(r<0)
	{
		rho=sqrt(-pow(p,3)/27.);
		phi=acos(-q/(2.*rho));
		dm2=min(-c2/3.+2.*pow(rho,1./3.)*cos(phi/3.),min(-c2/3.+2.*pow(rho,1./3.)*cos((phi+2.*M_PI)/3.),-c2/3.+2.*pow(rho,1./3.)*cos((phi+4.*M_PI)/3.)));
	}
	a=-K10*cos(theta)/(K11-dm2)+K01*sin(theta)/(K22-dm2);
	b=-K10*sin(theta)/(K11-dm2)-K01*cos(theta)/(K22-dm2);

	T->Xm=Xm;
	T->Ym=Ym;
	T->Zm=Zm;
	T->Xh=((1.+b*b)*Xm-a*b*Ym+a*Zm)/(1.+a*a+b*b);
	T->Yh=((1.+a*a)*Ym-a*b*Xm+b*Zm)/(1.+a*a+b*b);
	T->Zh=((a*a+b*b)*Zm+a*Xm+b*Ym)/(1.+a*a+b*b);
	
	T->UpdateTLines();	
	return(dm2/Q);
}



void MakeConfigFileThr(char* f_base_name, char* f_name, float BL[NB_COBO*NB_ASAD*NB_AGET*NB_CHANNEL],float THR)
{
	FILE* f_in=fopen(f_base_name,"r");
	FILE* f_out=fopen(f_name,"w");
	
	char line[2048];
	if(f_in!=NULL && f_out!=NULL)
	{
		for(int i=0;i<2;i++)
		{
			fgets(line,2048,f_in);
			fputs(line,f_out);
		}
		do
		{
			fgets(line,2048,f_in);
			if(strcmp(line,"</Setup>")!=1) fputs(line,f_out);
		}
		while(!feof(f_in));
		
		char instance[256];
		
		fprintf(f_out,"\t<Node id=\"CoBo\">\n");
		for(int i1=0;i1<2;i1++)
		{
			sprintf(instance,"<Instance id=\"Crate00_Slot0%d\">",i1);
			fprintf(f_out,"\t\t%s\n",instance);
			for(int i2=0;i2<NB_ASAD;i2++)
			{
				sprintf(instance,"<AsAd id=\"%d\">",i2);
				fprintf(f_out,"\t\t\t%s\n",instance);
				for(int i3=0;i3<NB_AGET;i3++)
				{
					sprintf(instance,"<Aget id=\"%d\">",i3);
					fprintf(f_out,"\t\t\t\t%s\n",instance);
					for(int i4=0;i4<NB_CHANNEL;i4++)
					{
						sprintf(instance,"<channel id=\"%d\">",i4);
						fprintf(f_out,"\t\t\t\t\t%s\n",instance);
						sprintf(instance,"<zeroSuppressionThreshold>%d</zeroSuppressionThreshold>",(int)(BL[i1*NB_ASAD*NB_AGET*NB_CHANNEL + i2*NB_AGET*NB_CHANNEL + i3*NB_CHANNEL + i4]+THR));
						fprintf(f_out,"\t\t\t\t\t\t%s\n",instance);
						fprintf(f_out,"\t\t\t\t\t</channel>\n");
					}
					fprintf(f_out,"\t\t\t\t</Aget>\n");
				}
				fprintf(f_out,"\t\t\t</AsAd>\n");
			}
			fprintf(f_out,"\t\t</Instance>\n");
		}
		fprintf(f_out,"\t</Node>\n");
		fprintf(f_out,"</Setup>\n");
		fclose(f_in);
		fclose(f_out);
	}
	else cout << "file " << f_base_name << " does not exists" << endl;
}


float Profile2D(float PAD[128][128],float theta, float correctionX, TH1D* Profile, int Ndiv, float& xdec, float& ydec, float& HistLength, float& XQmax)
{
	if(Ndiv>10)
	{
		cout << "Ndiv must be smaller than (or equal to) 10 " << endl;
		return(-1);
	}

	float xmin=1000;
	float xmax=-1000;
	
	float x1,y1,x2,y2;
	
	for(int i=0;i<128;i++)
		for(int j=0;j<128;j++)
			if(PAD[i][j]>0)
			{
				if( (((i+0.5)*Ndiv)*(1./Ndiv)*cos(theta)+((j+0.5)*Ndiv)*(1./Ndiv)*sin(theta) )*correctionX < xmin)
				{
					xmin=(((i+0.5)*Ndiv)*(1./Ndiv)*cos(theta)+((j+0.5)*Ndiv)*(1./Ndiv)*sin(theta) )*correctionX;
					x1=(i+0.5);
					y1=(j+0.5);
				}
				if( (((i+0.5)*Ndiv)*(1./Ndiv)*cos(theta)+((j+0.5)*Ndiv)*(1./Ndiv)*sin(theta) )*correctionX > xmax)
				{
					xmax=(((i+0.5)*Ndiv)*(1./Ndiv)*cos(theta)+((j+0.5)*Ndiv)*(1./Ndiv)*sin(theta) )*correctionX;
					x2=(i+0.5);
					y2=(j+0.5);
				}
			}
			
	for(int i=0;i<128;i++)
		for(int j=0;j<128;j++)
			if(PAD[i][j]>0)
				for(int ki=0;ki<Ndiv;ki++)
					for(int kj=0;kj<Ndiv;kj++)
						Profile->Fill(( ((i+0.5)*Ndiv+ki)*(1./Ndiv)*cos(theta)+((j+0.5)*Ndiv+kj)*(1./Ndiv)*sin(theta) )*correctionX -xmin, PAD[i][j]/(Ndiv*Ndiv));
	int binQmax=0;
	int firstbin=0;
	int lastbin=0;
	float Qmax=0;
	XQmax=0;
	
	Profile->Smooth();
	
	for(int bin=1;bin<Profile->GetNbinsX();bin++)
	{
		if(!firstbin && Profile->GetBinContent(bin)) firstbin=bin;
		if(Profile->GetBinContent(bin)>Qmax)
		{
			Qmax=Profile->GetBinContent(bin);
			binQmax=bin;
			XQmax=binQmax;
		}
	}
	
	for(int bin=Profile->GetNbinsX()-1;bin>0;bin--)
		if(!lastbin && Profile->GetBinContent(bin)) lastbin=bin;
	
	
	xdec=y1;
	ydec=y1;
	
	if(lastbin-binQmax > binQmax-firstbin)
	{
		float tmp_array[lastbin-firstbin+1];
		for(int bin=firstbin;bin<=lastbin;bin++)
			tmp_array[bin-firstbin]=Profile->GetBinContent(bin);
		for(int bin=firstbin;bin<=lastbin;bin++)
			Profile->SetBinContent(bin, tmp_array[lastbin-bin]);
		binQmax=lastbin-(binQmax-firstbin);
		xdec=x2;
		ydec=y2;
	}
	
	int np=0;
	float Rtot=0;
	float X0=0;
	float Y0=0;

	int bin0=binQmax;
	while(Profile->GetBinContent(bin0)>Qmax/5. && bin0<lastbin+1) bin0++;
	if((Profile->GetBinContent(bin0-1)-Profile->GetBinContent(bin0))!=0) Rtot=Profile->GetBinCenter(bin0-1)+(Profile->GetBinCenter(bin0)-Profile->GetBinCenter(bin0-1))*(Profile->GetBinContent(bin0-1)-Qmax/5.)/(Profile->GetBinContent(bin0-1)-Profile->GetBinContent(bin0));
	else Rtot=-1;
	
	HistLength=lastbin-firstbin;

// 	Rtot=xmax-xmin;

	return(Rtot);
}



float Profile3D(std::vector<float> X, std::vector<float> Y, std::vector<float> Z, std::vector<float> Q, MTrack* T, TH1D* Profile, TH1D* ProfileDer, double dist_max, float& Qmean, bool& isInverted, TMarker* Mark[5])
{
	if(X.size()<2)
	{
		return(0);
	}
	
	else
	{
		TVector3 u;
		u.SetXYZ((T->Xh-T->Xm) , (T->Yh-T->Ym) , (T->Zh-T->Zm));
		float norm_u=u.Mag();

		TVector3 AB;
		float dist_to_line;
	
		std::vector<float> ProjPos;
		std::vector<float> ProjQ;
	
// point (0,0,0) projected position taken as reference
		AB.SetXYZ((0-T->Xm) , (0-T->Ym) , (0-T->Zm));
		float refProjPos=AB.Dot(u) / norm_u;
		float alpha=refProjPos / norm_u;
		float XP_0 = T->Xm+alpha*u.X();
		float YP_0 = T->Ym+alpha*u.Y();
		float ZP_0 = T->Zm+alpha*u.Z();
		
		float minProjPos=1E12;
		
		
// filling profile vectors with relevant voxels
		if(X.size()>1)for(unsigned int i=0;i<X.size();i++)// for(int kx=-1;kx<2;kx++) for(int ky=-1;ky<2;ky++) for(int kz=-1;kz<2;kz++)
		{
			float kx=0; float ky=0; float kz=0;
			AB.SetXYZ((X[i]+kx/3.-XP_0) , (Y[i]+ky/3.-YP_0) , (Z[i]+kz/3.-ZP_0));
			dist_to_line=((AB.Cross(u)).Mag()) / norm_u;
			AB.SetXYZ(0-X[i]-kx/3. , 0-Y[i]-ky/3. , 0-Z[i]-kz/3.);
			
			//cout << X[i] << " " << Y[i] << " " << Z[i] << " \t\t";
			if(dist_to_line<dist_max)
			{
				float curProjPos=AB.Dot(u) / norm_u;
				ProjPos.push_back(curProjPos);
				ProjQ.push_back(Q[i]/27.);
				if(curProjPos<minProjPos)
				{
					minProjPos=curProjPos;
					Mark[0]->SetX(X[i]+kx/3.);
					Mark[0]->SetY(Y[i]+ky/3.);
				}
				//cout << curProjPos << " " << Q[i] << endl;
			}
		}


// filling profile with vector content
		for(unsigned int i=0;i<ProjPos.size();i++)
		{
			Profile->Fill(ProjPos[i]-minProjPos , ProjQ[i]);
		}
		Profile->Smooth(1);
		
		
// re-ordonancing charge profile if necessary (Bragg peak at the end) 
		int binQmax=0;
		int firstbin=0;
		int lastbin=0;
		float Qmax=0;
		float Qtot=0;
		isInverted=false;

		for(int bin=1;bin<Profile->GetNbinsX();bin++)
		{
			if(!firstbin && Profile->GetBinContent(bin)) firstbin=bin;
			if(Profile->GetBinContent(bin)>Qmax)
			{
				Qmax=Profile->GetBinContent(bin);
				binQmax=bin;
			}
			Qtot+=Profile->GetBinContent(bin);
		}
	
		for(int bin=Profile->GetNbinsX()-1;bin>0;bin--)
			if(!lastbin && Profile->GetBinContent(bin)) lastbin=bin;
		
		if(lastbin-binQmax > binQmax-firstbin)
		{
			float tmp_array[lastbin-firstbin+1];
			for(int bin=firstbin;bin<=lastbin;bin++)
				tmp_array[bin-firstbin]=Profile->GetBinContent(bin);
			for(int bin=firstbin;bin<=lastbin;bin++)
				Profile->SetBinContent(bin, tmp_array[lastbin-bin]);
			binQmax=lastbin-(binQmax-firstbin);
			isInverted=true;
		}


// Range calculation from the profile shape	
		float Rtot=0;
		float X0=0;
		float Y0=0;

		int bin0=binQmax;
		while(Profile->GetBinContent(bin0)>Qmax/5. && bin0<lastbin+1) bin0++;
		if((Profile->GetBinContent(bin0-1)-Profile->GetBinContent(bin0))!=0)
			Rtot=Profile->GetBinCenter(bin0-1)+(Profile->GetBinCenter(bin0)-Profile->GetBinCenter(bin0-1))*(Profile->GetBinContent(bin0-1)-Qmax/5.) /(Profile->GetBinContent(bin0-1)-Profile->GetBinContent(bin0));
		else Rtot=-1;
		
		
// profile derivative: look for the "real" start of the profile (inflexion point = maximum derivative at the start)
		for(int bin=1;bin<Profile->GetNbinsX();bin++) ProfileDer->SetBinContent(bin,Profile->GetBinContent(bin+1)-Profile->GetBinContent(bin));
		ProfileDer->Smooth();
		double firstmaxder=100;
		double posfirstmaxder=0;
		for(int bin=19;bin<ProfileDer->GetNbinsX();bin++)
			if(ProfileDer->GetBinContent(bin)>firstmaxder && ProfileDer->GetBinContent(bin+1)<ProfileDer->GetBinContent(bin))
			{
				firstmaxder=ProfileDer->GetBinContent(bin);
				posfirstmaxder=ProfileDer->GetBinCenter(bin)+0.5;
				Mark[1]->SetX(posfirstmaxder);
				Mark[1]->SetY(firstmaxder);
				bin=ProfileDer->GetNbinsX();
			}
		Mark[2]->SetX(Rtot);
		Mark[2]->SetY(Qmax/5.);
		Mark[2]->SetMarkerStyle(21);		


// for visualisation: Track Line 	
		float Xmin=XP_0+(minProjPos/refProjPos)*(T->Xm-XP_0);
		float Ymin=YP_0+(minProjPos/refProjPos)*(T->Ym-YP_0);
		float Zmin=ZP_0+(minProjPos/refProjPos)*(T->Zm-ZP_0);
		float Xmax=XP_0+((minProjPos+Rtot)/refProjPos)*(T->Xm-XP_0);
		float Ymax=YP_0+((minProjPos+Rtot)/refProjPos)*(T->Ym-YP_0);
		float Zmax=ZP_0+((minProjPos+Rtot)/refProjPos)*(T->Zm-ZP_0);
		
		if(!isInverted)
		{
			T->Xm=Xmin;
			T->Ym=Ymin;
			T->Zm=Zmin;
			T->Xh=Xmax;
			T->Yh=Ymax;
			T->Zh=Zmax;
		}
		else
		{
			T->Xm=Xmax;
			T->Ym=Ymax;
			T->Zm=Zmax;
			T->Xh=Xmin;
			T->Yh=Ymin;
			T->Zh=Zmin;
		
		}	
		T->UpdateTLines();
		
		Qmean=Qtot/(Rtot);
		
		return(Rtot);
	}
}


float Profile3D_v2(std::vector<float> X, std::vector<float> Y, std::vector<float> Z, std::vector<float> Q, MTrack* T, TH2D* Profile2D, TH2D* Zdistrib, TH1D* Profile, TH1D* ProfileDer, double dist_max, float& Qmean, bool& isInverted, TMarker* Mark[5])
{
	if(X.size()<2)
	{
		return(0);
	}
	
	else
	{
		TVector3 u;
		u.SetXYZ((T->Xh-T->Xm) , (T->Yh-T->Ym) , (T->Zh-T->Zm));
		float norm_u=u.Mag();

		TVector3 AB;
		float dist_to_line;
	
		std::vector<float> ProjPos;
		std::vector<float> ProjTrans;
		std::vector<float> ProjQ;
		std::vector<float> ProjZ;
	
// point (0,0,0) projected position taken as reference
		AB.SetXYZ((0-T->Xm) , (0-T->Ym) , (0-T->Zm));
		float refProjPos=AB.Dot(u) / norm_u;
		float alpha=refProjPos / norm_u;
		float XP_0 = T->Xm+alpha*u.X();
		float YP_0 = T->Ym+alpha*u.Y();
		float ZP_0 = T->Zm+alpha*u.Z();
		
		float minProjPos=1E12;
		
		
// filling profile vectors with relevant voxels
		if(X.size()>1)for(unsigned int i=0;i<X.size();i++) for(int kx=-1;kx<2;kx++) for(int ky=-1;ky<2;ky++) for(int kz=-1;kz<2;kz++)
		{
			AB.SetXYZ((X[i]+kx/3.-XP_0) , (Y[i]+ky/3.-YP_0) , (Z[i]+kz/3.-ZP_0));
			dist_to_line=((AB.Cross(u)).Mag()) / norm_u;
			AB.SetXYZ(0-X[i]-kx/3. , 0-Y[i]-ky/3. , 0-Z[i]-kz/3.);
			if(dist_to_line<dist_max)
			{
				float curProjPos=AB.Dot(u) / norm_u;
				ProjPos.push_back(curProjPos);
				ProjQ.push_back(Q[i]/27.);
				if(curProjPos<minProjPos)
				{
					minProjPos=curProjPos;
					Mark[0]->SetX(X[i]+kx/3.);
					Mark[0]->SetY(Y[i]+ky/3.);
				}
			}
		}


// filling profile with vector content
		for(unsigned int i=0;i<ProjPos.size();i++)
		{
			Profile->Fill(ProjPos[i]-minProjPos, ProjQ[i]);
		}
		Profile->Smooth(1);
		
		
// re-ordonancing charge profile if necessary (Bragg peak at the end) 
		int binQmax=0;
		int firstbin=0;
		int lastbin=0;
		float Qmax=0;
		float Qtot=0;
		isInverted=false;

		for(int bin=1;bin<Profile->GetNbinsX();bin++)
		{
			if(!firstbin && Profile->GetBinContent(bin)) firstbin=bin;
			if(Profile->GetBinContent(bin)>Qmax)
			{
				Qmax=Profile->GetBinContent(bin);
				binQmax=bin;
			}
			Qtot+=Profile->GetBinContent(bin);
		}
	
		for(int bin=Profile->GetNbinsX()-1;bin>0;bin--)
			if(!lastbin && Profile->GetBinContent(bin)) lastbin=bin;
		
		if(lastbin-binQmax > binQmax-firstbin)
		{
			float tmp_array[lastbin-firstbin+1];
			for(int bin=firstbin;bin<=lastbin;bin++)
				tmp_array[bin-firstbin]=Profile->GetBinContent(bin);
			for(int bin=firstbin;bin<=lastbin;bin++)
				Profile->SetBinContent(bin, tmp_array[lastbin-bin]);
			binQmax=lastbin-(binQmax-firstbin);
			isInverted=true;
		}


// Range calculation from the profile shape	
		float Rtot=0;
		float X0=0;
		float Y0=0;

		int bin0=binQmax;
		while(Profile->GetBinContent(bin0)>Qmax/5. && bin0<lastbin+1) bin0++;
		if((Profile->GetBinContent(bin0-1)-Profile->GetBinContent(bin0))!=0)
			Rtot=Profile->GetBinCenter(bin0-1)+(Profile->GetBinCenter(bin0)-Profile->GetBinCenter(bin0-1))*(Profile->GetBinContent(bin0-1)-Qmax/5.) /(Profile->GetBinContent(bin0-1)-Profile->GetBinContent(bin0));
		else Rtot=-1;
		
		
// profile derivative: look for the "real" start of the profile (inflexion point = maximum derivative at the start)
		for(int bin=1;bin<Profile->GetNbinsX();bin++) ProfileDer->SetBinContent(bin,Profile->GetBinContent(bin+1)-Profile->GetBinContent(bin));
		ProfileDer->Smooth();
		double firstmaxder=100;
		double posfirstmaxder=0;
		for(int bin=19;bin<ProfileDer->GetNbinsX();bin++)
			if(ProfileDer->GetBinContent(bin)>firstmaxder && ProfileDer->GetBinContent(bin+1)<ProfileDer->GetBinContent(bin))
			{
				firstmaxder=ProfileDer->GetBinContent(bin);
				posfirstmaxder=ProfileDer->GetBinCenter(bin)+0.5;
				Mark[1]->SetX(posfirstmaxder);
				Mark[1]->SetY(firstmaxder);
				bin=ProfileDer->GetNbinsX();
			}
		Mark[2]->SetX(Rtot);
		Mark[2]->SetY(Qmax/5.);
		Mark[2]->SetMarkerStyle(21);		


// for visualisation: Track Line 	
		float Xmin=XP_0+(minProjPos/refProjPos)*(T->Xm-XP_0);
		float Ymin=YP_0+(minProjPos/refProjPos)*(T->Ym-YP_0);
		float Zmin=ZP_0+(minProjPos/refProjPos)*(T->Zm-ZP_0);
		float Xmax=XP_0+((minProjPos+Rtot)/refProjPos)*(T->Xm-XP_0);
		float Ymax=YP_0+((minProjPos+Rtot)/refProjPos)*(T->Ym-YP_0);
		float Zmax=ZP_0+((minProjPos+Rtot)/refProjPos)*(T->Zm-ZP_0);
		
		if(!isInverted)
		{
			T->Xm=Xmin;
			T->Ym=Ymin;
			T->Zm=Zmin;
			T->Xh=Xmax;
			T->Yh=Ymax;
			T->Zh=Zmax;
		}
		else
		{
			T->Xm=Xmax;
			T->Ym=Ymax;
			T->Zm=Zmax;
			T->Xh=Xmin;
			T->Yh=Ymin;
			T->Zh=Zmin;
		
		}	
		T->UpdateTLines();
		
		TVector3 u_XY;
		u_XY.SetXYZ((Xmax-Xmin) , (Ymax-Ymin),0);
		
		
		if(X.size()>1)for(unsigned int i=0;i<X.size();i++) for(int kx=-1;kx<2;kx++) for(int ky=-1;ky<2;ky++) for(int kz=-1;kz<2;kz++)
		{
			AB.SetXYZ((X[i]+kx/3.-XP_0) , (Y[i]+ky/3.-YP_0) , (Z[i]+kz/3.-ZP_0));
			dist_to_line=((AB.Cross(u)).Mag()) / norm_u;
			AB.SetXYZ(0-X[i]-kx/3. , 0-Y[i]-ky/3. , 0-Z[i]-kz/3.);
			if(dist_to_line<dist_max)
			{
				AB.SetXYZ((X[i]+kx/3.)-Xmin , (Y[i]+ky/3.)-Ymin,0);
				ProjZ.push_back(Z[i]);
				if(AB.Phi()>u_XY.Phi()) ProjTrans.push_back(((AB.Cross(u_XY)).Mag()) / u_XY.Mag());
				else ProjTrans.push_back(-1.*((AB.Cross(u_XY)).Mag()) / u_XY.Mag());
			}
		}
		if(ProjTrans.size()==ProjPos.size() && ProjTrans.size()==ProjQ.size()) for(unsigned int i=0;i<ProjTrans.size();i++)
		{
			Profile2D->Fill(ProjPos[i]-minProjPos,ProjTrans[i], ProjQ[i]);
			Zdistrib->Fill(ProjPos[i]-minProjPos,ProjZ[i], ProjQ[i]);
		}
		else cout << "Problem vector size: Transverse=" << ProjTrans.size() << " Position=" << ProjPos.size() << " Q=" << ProjQ.size() << endl;

			
			

		
		Qmean=Qtot/(Rtot);
		
		return(Rtot);
	}
}

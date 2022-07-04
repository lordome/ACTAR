// prommenons
// Commetuasdegrandesdents
#include "Parameters.h"

#ifndef NPADX
#define NPADX 128
#endif

#ifndef NPADY
#define NPADY 128
#endif

#ifndef NB_COBO
#define NB_COBO 17
#endif

#ifndef NB_ASAD
#define NB_ASAD 4
#endif

#ifndef NB_AGET
#define NB_AGET 4
#endif

#ifndef NB_CHANNEL
#define NB_CHANNEL 68
#endif

#ifndef NB_SAMPLES
#define NB_SAMPLES 512
#endif

#define ONLINE_WAIT_NEWRUN 20
#define MERGE_DPT 150

#ifdef Vdrift
#undef Vdrift
#endif
// Q: Which is the physical dimension? cm/us?
//#define Vdrift 1.109 // runs 84-86
#ifndef NPADX
#define NPADX 128
#endif

#ifndef NPADY
#define NPADY 128
#endif

#ifndef NB_COBO
#define NB_COBO 17
#endif

#ifndef NB_ASAD
#define NB_ASAD 4
#endif

#ifndef NB_AGET
#define NB_AGET 4
#endif

#ifndef NB_CHANNEL
#define NB_CHANNEL 68
#endif

#ifndef NB_SAMPLES
#define NB_SAMPLES 512
#endif

#define ONLINE_WAIT_NEWRUN 20
#define MERGE_DPT 150

#ifdef Vdrift
#undef Vdrift
#endif
// Q: Which is the physical dimension? cm/us?
//#define Vdrift 1.109 // runs 84-86
//#define Vdrift 1.178 // runs 72-74
#define Vdrift 1.0 // runs dummy

//#define Vdrift 1.178 // runs 72-74
#define Vdrift 1.0 // runs dummy

#include "TFile.h"
#include "TEnv.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TVector3.h"
#include <utility>
#include <map>
#include <string>

#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TH2F.h"
#include "TF1.h"
#include "TGraph.h"
#include "TF1.h"
#include "TCutG.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TRandom.h"
#include "TDirectory.h"
#include "TDatime.h"
#include "MEvent.h"
#include "MEventReduced.h"

#include "Utils.h"
#include <algorithm>

#include "TSystem.h"

#include "DataParameters.h"

#include <stdio.h>
#include <iostream>

#ifdef NB_COBO
#undef NB_COBO
#endif
#define NB_COBO 17

#ifdef Vdrift
#undef Vdrift
#endif

// Q: Which is the physical dimension? cm/us?
//#define Vdrift 1.109 // runs 84-86
//#define Vdrift 1.178 // runs 72-74
#define Vdrift 1.0 // runs dummy

using namespace std;

void PadRepair(float[128][128], float[128][128]);
double BraggProfile(double *, double *);
int Ransac2D(TRandom *, std::vector<int>, std::vector<int>, std::vector<float>, float &, float &);

const int NImplant = 300;

struct DataImplantation
{
	unsigned long RUN;
	unsigned long EN;

	float TS;

	float DE;
	float TOF;

	int X;
	int Y;

	int Z;
	int A;
};

struct DataDecay
{
	unsigned long RUN;
	unsigned long EN;

	float TS;
	float theta_XY;
	float theta_XZ;
	float theta_YZ;
	float theta_V;

	float dQdX1;

	float Qtot;
	float Qpic;
	float Range;

	float Xstart;
	float Ystart;
	float Zstart;
	float Xstop;
	float Ystop;
	float Zstop;

	float Qmean;
	float Qavg;

	float Width1;
	float Height1;
	float Width2;
	float Height2;

	int NHoles;
	int NSaturations;

	bool stops;
	bool crossHole;
	bool isInverted;
};

float XBragg[44] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86};
float YBragg[44] = {6.98732, 20.2008, 38.3615, 50.9091, 58.0761, 62.093, 65.1691, 67.3573, 68.9746, 69.8732, 70.1691, 70.7717, 72.0507, 73.5941, 75.4863, 77.6533, 79.6934, 82.1036, 84.4609, 87.2622, 89.8837, 92.8858, 96.3319, 100.148, 103.393, 106.247, 109.736, 113.679, 118.362, 123.319, 128.689, 134.313, 140.708, 149.239, 160.856, 177.125, 198.446, 220.677, 230.042, 199.101, 121.934, 45.6025, 11.3848, 5.20085};

int TreeRead(int run_f, int run_l)
{
	int UpdateFreq = 5000;

	/////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// CANVAS, HISTOS, VISU... //////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	TCanvas *C = new TCanvas("C", "C", 1350, 900);
	C->Divide(3, 2);

	TCanvas *CId = new TCanvas("CId", "CId", 1200, 600);
	CId->Divide(2, 2);
	TH2F *hId_1 = new TH2F("hId_1", "hId_1", 300, 0, 1.5E4, 300, 0, 1.5E4);
	TH2F *hId_2 = new TH2F("hId_2", "hId_2", 200, 0, 200, 300, 0, 1E5);
	TH2F *hId_3 = new TH2F("hId_3", "hId_3", 300, 0, 1E4, 300, 0, 2E5);

	TH1D *hProfileX = new TH1D("hProfileX", "hProfileX", 128, 0, 128);
	TH1D *hProfileY = new TH1D("hProfileY", "hProfileY", 128, 0, 128);
	TH1D *hImpRange = new TH1D("hImpRange", "hImpRange", 128, 1, 129);
	TH1D *hDecRange = new TH1D("hDecRange", "hDecRange", 128, 1, 129);
	TH1D *hDecRangeFit = new TH1D("hDecRangeFit", "hDecRangeFit", 128, 1, 129);
	TH1D *hProfile1 = new TH1D("hProfile1", "hProfile1", 220, -20, 200);
	TH1D *hProfile2 = new TH1D("hProfile2", "hProfile2", 110, -20, 200);
	TH1D *hProfileGainMeasurement = new TH1D("hProfileGainMeasurement", "hProfileGainMeasurement", 128, 0, 128);
	TH1D *hRangeMeasurement = new TH1D("hProfileRangeMeasurement", "hProfileRangeMeasurement", 320, 0, 160);
	TH2D *hProfileNew2D = new TH2D("hProfileNew2D", "hProfileNew2D", 128, 0, 128, 100, -50, 50);
	TH2D *hZdistribution = new TH2D("hZdistribution", "hZdistribution", 128, 0, 128, 256, 0, 256);
	TH1D *hProfileDer = new TH1D("hProfileDer", "hProfileDer", 320, -20, 300);

	TH1F *hSignal = new TH1F("hSignal", "hSignal", 256, 0, 256);
	TH2F *hSignalCumul = new TH2F("hSignalCumul", "hSignalCumul", 256, 0, 256, 4096, -9.6, 100);

	TH2I *hPadSummary = new TH2I("hPADSummary", "hPADSummary", NB_COBO * NB_ASAD * NB_AGET * NB_CHANNEL, 0, NB_COBO * NB_ASAD * NB_AGET * NB_CHANNEL, 1000, 500, 4000);
	TH2I *hTimeSummary = new TH2I("hTimeSummary", "hTimeSummary", NB_COBO * NB_ASAD * NB_AGET * NB_CHANNEL, 0, NB_COBO * NB_ASAD * NB_AGET * NB_CHANNEL, 512, 0, 512);

	TH2F *hImpXY = new TH2F("hImpXY", "hImpXY", 128, 0, 128, 128, 0, 128);

	TH2F *XY = new TH2F("XY", "XY", 128, 0, 128, 128, 0, 128);
	TH2F *XZ = new TH2F("XZ", "XZ", 128, 0, 128, 128, 0, 256);
	TH2F *YZ = new TH2F("YZ", "YZ", 128, 0, 128, 128, 0, 256);

	TH2F *XYr = new TH2F("XYr", "XYr", 128, 0, 128, 128, 0, 128);
	TH2F *XZr = new TH2F("XZr", "XZr", 128, 0, 128, 128, 0, 256);
	TH2F *YZr = new TH2F("YZr", "YZr", 128, 0, 128, 128, 0, 256);

	TH1F *hTSImplant = new TH1F("hTSImplant", "hTSImplant", 1500, 0, 5);

	TH2F *XYclean = new TH2F("XYclean", "XYclean", 128, 0, 128, 128, 0, 128);

	TH2F *hSi = new TH2F("hSi", "hSi", 11, 0, 11, 1024, 0, 16384);
	TH1F *hTheta = new TH1F("hTheta", "hTheta", 180, -90, 90);
	TH1F *hPhi = new TH1F("hPhi", "hPhi", 180, -90, 90);
	TH1F *hGATCONF = new TH1F("hGATCONF", "hGATCONF", 4096, 0, 4096);
	TH2F *hCalibA = new TH2F("hCalibA", "hCalibA", 128, 0, 128, 128, 0, 128);

	TGraph *GBragg = new TGraph(44, XBragg, YBragg);
	TGraph *Gtransv = new TGraph(220);

	////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// RUN FILE, TTREES, OUTPUT FILES //////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////

	TString TreeName;
	TDatime TheDate;

	// 	FILE* ff=fopen("R26E70845.dat","w");

	TString OutDirectory;
	OutDirectory.Form("Out_%d/", TheDate.GetDate());
	system(Form("mkdir Out_%d", TheDate.GetDate()));
	TreeName.Form("Out_%d/Output_%d-%d.root", TheDate.GetDate(), run_f, run_l);
	TFile *Fout = new TFile(TreeName.Data(), "recreate");

	TTree *TreeOutDec = new TTree("Decay", "");
	DataDecay DECAY;
	TreeOutDec->Branch("DEC", &DECAY, "RUN/l:EN/l:TS/F:theta_XY/F:theta_XZ/F:theta_YZ/F:theta_V/F:dQdX1/F:Qtot/F:Qpic/F:Range/F:Xstart/F:Ystart/F:Zstart/F:Xstop/F:Ystop/F:Zstop/F:Qmean/F:Qavg/F:Width1/F:Height1/F:Width2/F:Height2/F:NHoles/I:NSaturations/I:stops/O:crossHole/O:isInverted/O", 1600);
	// TreeOutDec->Branch("DEC",&DECAY,"Qtot/F:Qpic/F:Range/F:X/F:Y/F:XQmax/F:ProfLength/F:NQsup500/I:Nleft/I:Nright/I:stops/O",16000);

	TTree *TreeOutImp = new TTree("Implant", "");
	DataImplantation IMPLANT;
	TreeOutImp->Branch("IMP", &IMPLANT, "RUN/l:EN/l:TS/F:DE/F:TOF/F:X/I:Y/I:Z/I:A/I", 16000);

	TChain *Tree = new TChain("Tree", "");

	int RUN;
	for (int run = run_f; run <= run_l; run++)
	{
		TString RunFile;

		// 		RunFile.Form("/home/roger/Desktop/E690_FROM_HOME/1st_LEVEL/root_test/fc_deconv_60/Tree_Run_%04d.root",run); // Deconv physics runs
		// 		RunFile.Form("/media/roger/ACTAR_USB_2/E690_root/root_integral_20200324/Tree_Run_%04d.root",run); // Intg no deconv physics runs
		// 		RunFile.Form("/home/roger/Desktop/E690_FROM_HOME/1st_LEVEL/root_calib2020/Tree_Run_%04d_Merged.root",run); // test physics runs
		// 		RunFile.Form("/home/roger/Bureau/ANALYSIS_E690/1st_LEVEL/root_calib_deconv_max_20200408/Tree_Run_%04d.root",run); // Calib runs
		RunFile.Form("/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/root_e796/Tree_Run_%04d_Merged.root", run); // test physics runs
		Tree->AddFile(RunFile.Data(), 0, "ACTAR_TTree");
		RUN = run;
	}

	MEventReduced *EvtRed = 0;

	Tree->SetBranchAddress("data", &EvtRed);

	/////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// LOOKUP TABLE, CALIBS... //////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

	int TABLE[6][NB_COBO * NB_ASAD * NB_AGET * NB_CHANNEL];
	int TABLE_INV[128][128][4];

	// 	FILE* fTable=fopen("LT_GANIL_NewCF.dat","r");
	FILE *fTable = fopen("../dat/LT.dat", "r");
	if (fTable != NULL)
	{
		cout << "Using LookupTable from: "
			 << "../dat/LT.dat" << endl;

		for (int i = 0; i < NB_COBO * NB_ASAD * NB_AGET * NB_CHANNEL; i++)
		{
			fscanf(fTable, "%d %d %d %d %d %d", &TABLE[0][i], &TABLE[1][i], &TABLE[2][i], &TABLE[3][i], &TABLE[4][i], &TABLE[5][i]);
		}
		fclose(fTable);
	}
	else
	{
		cout << "Using DEFAULT LookupTable" << endl;

		int index = 0;
		for (int co = 0; co < NB_COBO; co++)
		{
			for (int as = 0; as < NB_ASAD; as++)
			{
				for (int ag = 0; ag < NB_AGET; ag++)
				{

					for (int ch = 0; ch < NB_CHANNEL; ch++)
					{
						int ind = co * NB_ASAD * NB_AGET * NB_CHANNEL + as * NB_AGET * NB_CHANNEL + ag * NB_CHANNEL + ch;
						TABLE[0][ind] = co;
						TABLE[1][ind] = as;
						TABLE[2][ind] = ag;
						TABLE[3][ind] = ch;
						if (ch != 11 && ch != 22 && ch != 45 && ch != 56)
						{

							TABLE[4][ind] = (int)(index / 128);
							TABLE[5][ind] = index % 128;
							index++;
						}
						else
						{
							TABLE[4][ind] = -1;
							TABLE[5][ind] = -1;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < 16 * NB_ASAD * NB_AGET * NB_CHANNEL; i++)
	{
		if (TABLE[0][i] < 16 && TABLE[4][i] != -1 && TABLE[5][i] != -1 && TABLE[4][i] < 128 && TABLE[5][i] < 128)
		{
			TABLE_INV[TABLE[4][i]][TABLE[5][i]][0] = TABLE[0][i];
			TABLE_INV[TABLE[4][i]][TABLE[5][i]][1] = TABLE[1][i];
			TABLE_INV[TABLE[4][i]][TABLE[5][i]][2] = TABLE[2][i];
			TABLE_INV[TABLE[4][i]][TABLE[5][i]][3] = TABLE[3][i];
		}
	}

	int co, as, ag, ch;
	double CAL[NB_COBO][NB_ASAD][NB_AGET][NB_CHANNEL][3];
	// FILE* fCal=fopen("/home/roger/Desktop/ANALYSIS/E780/ACTAR_ANALYSIS_LIGHT/cal/allign_97_integral-deconv.dat","r");
	FILE *fCal = fopen("../cal/allign_run_8-12_NoDeconv_2nd.dat", "r");
	int indx = 0;
	if (fCal == NULL)
	{
		cout << "Calibration File not valid: working with raw data" << endl;
		for (int co = 0; co < NB_COBO; co++)
			for (int as = 0; as < NB_ASAD; as++)
				for (int ag = 0; ag < NB_AGET; ag++)
					for (int ch = 0; ch < NB_CHANNEL; ch++)
					{
						CAL[co][as][ag][ch][0] = 1.;
						CAL[co][as][ag][ch][1] = 0.;
					}
	}
	else
	{
		while (!feof(fCal))
		{
			fscanf(fCal, "%d %d %d %d", &co, &as, &ag, &ch);
			fscanf(fCal, "%lf %lf %lf", &CAL[co][as][ag][ch][0], &CAL[co][as][ag][ch][1], &CAL[co][as][ag][ch][2]);
			hCalibA->Fill(TABLE[4][indx], TABLE[5][indx], CAL[co][as][ag][ch][0]);
			indx++;
		}
		fclose(fCal);
	}

	TString fENName;
	fENName.Form("Run_%.4d.events", RUN);
	FILE *fEN = fopen(fENName.Data(), "w");

	/*	fCal=fopen("Baseline/Threshold_R72-82.dat","r");
	while(!feof(fCal))
	{
		fscanf(fCal,"%d %d %d %d",&co,&as,&ag,&ch);
		fscanf(fCal,"%lf",&CAL[co][as][ag][ch][2]);
	}
	fclose(fCal);
	*/

	/////////////////////////////////////////////////////////////////
	////                VXI Parameters treatment                 ////
	/////////////////////////////////////////////////////////////////

	MEvent *Evt = new MEvent();
	std::vector<TString> Par;
	std::vector<int> Numbers;

	float GATCONF;
	Par.push_back(TString("GATCONF"));
	Numbers.push_back(0);
	float SI0[11];
	Par.push_back(TString("SI0_"));
	Numbers.push_back(11);
	float E3D6;	   // Par.push_back(TString("E3D6"));      Numbers.push_back(0);
	float E4D6;	   // Par.push_back(TString("E4D6"));      Numbers.push_back(0);
	float T3D6_HF; // Par.push_back(TString("T3D6HF"));   Numbers.push_back(0);
	float T4D6_HF; // Par.push_back(TString("T4D6HF"));   Numbers.push_back(0);

	Evt->InitVXIParameters((char *)("/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/root_e796/ACTIONS_e796.CHC_PAR.run_0032.dat.10-03-20_07h04m31s"), Par, Numbers);
	cout << "VXI init done" << endl;

	unsigned long int TS_PREV = 0;

	/////////////////////////////////////////////////////////////////
	////            Track reconstruction definitions             ////
	/////////////////////////////////////////////////////////////////

	float PAD[128][128], TIME[128][128];
	MTrack *T = new MTrack();
	T->zx_e = 117;
	T->zx_s = 10;
	T->zy_s = 30;
	T->zy_e = 100;
	MTrack *Tb = new MTrack();
	Tb->zx_e = 69;
	Tb->zx_s = 58;
	Tb->zy_s = 1;
	Tb->zy_e = 100;
	// 	TCanvas* Can=new TCanvas("Can","Can",800,800);

	TRandom *Rand = new TRandom();

	double XHole[5][2] = {{35, 59}, {42, 87}, {33, 54}, {79, 123}, {44, 84}};
	double YHole[5][2] = {{110, 115}, {78, 82}, {62, 66}, {47, 50}, {15, 18}};

	TGraph *Hole[5];
	for (int i = 0; i < 5; i++)
	{
		Hole[i] = new TGraph(5);
		Hole[i]->SetPoint(0, XHole[i][0], YHole[i][0]);
		Hole[i]->SetPoint(1, XHole[i][1], YHole[i][0]);
		Hole[i]->SetPoint(2, XHole[i][1], YHole[i][1]);
		Hole[i]->SetPoint(3, XHole[i][0], YHole[i][1]);
		Hole[i]->SetPoint(4, XHole[i][0], YHole[i][0]);
	}

	TMarker *Mark[5];
	for (int i = 0; i < 5; i++)
		Mark[i] = new TMarker(0, 0, 2);

	/////////////////////////////////////////////////////////////////
	////             Absolute normalization stuff                ////
	/////////////////////////////////////////////////////////////////

	unsigned long nCATS2 = 0;
	unsigned long nCATS2_sca_low = 0;
	unsigned long nCATS2_sca_high = 0;
	int cats_counter[2] = {0, 0};

	TH2F *hDSSD = new TH2F("hDSSD", "hDSSD", 272 * 4, 0, 272 * 4, 512, 0, 2048);

	// 	CId->cd();
	// 	hCalibA->Draw("colz");
	// 	CId->WaitPrimitive();

	int evt_list[5] = {290, 512, 1136, 1381};
	int Nlist = 0;
	int nCpt = 0;

	/////////////////////////////////////////////////////////////////
	////                       Event Loop                        ////
	/////////////////////////////////////////////////////////////////

	long int EntryTotal = Tree->GetEntries();
	cout << "Treating " << Tree->GetEntries() << " entries" << endl;

	long int ENTRIES[8] = {32, 38, 45, 60, 62, 63, 79, 81};
	//
	// 	int readalwayslist[24]={1703,1707,1888,1890,1898,1902,2142,2146,2150,2155,2165,2167,2262,2264,2431,2433,2267,2269,2707,2710,3230,3232,3234,3236};
	// 	for(int i=0;i<24;i++)
	// 	{
	// 		XY->Fill(TABLE[4][readalwayslist[i]],TABLE[5][readalwayslist[i]]);
	// 		if(TABLE[5][readalwayslist[i]]>60) cout << readalwayslist[i] << " ";
	// 	}
	// 	cout << endl;
	// 	CId->cd();
	// 	XY->Draw("colz");
	// 	CId->WaitPrimitive();
	// 	CId->WaitPrimitive();

	for (long int entry = 0; entry < EntryTotal; entry++)
	// 	for(int ent=0;ent<8;ent++)
	{
		// 		long int entry=ENTRIES[ent];
		Tree->GetEntry(entry);

		if (!((entry + 1) % (EntryTotal / 100)))
		{
			cout << "In progress: " << entry * 100. / Tree->GetEntries() << " %       XH = " << cats_counter[0] << " / " << cats_counter[1] << "     \r";
			cout.flush();
		}
		//
		XY->Reset();
		YZ->Reset();
		XZ->Reset();
		XYr->Reset();
		YZr->Reset();
		XZr->Reset();
		hSignal->Reset();

		for (int xx = 0; xx < 128; xx++)
			for (int yy = 0; yy < 128; yy++)
				PAD[xx][yy] = TIME[xx][yy] = 0;

		TString S;
		S.Form("Event %ld - entry %ld", EvtRed->event, entry);

		unsigned long EN = EvtRed->event;
		unsigned long TS = EvtRed->timestamp;

		bool hasSat = false;
		bool hasSilicon = false;
		bool tobedrawn = false;

		DECAY.NSaturations = 0;

		for (unsigned int it = 0; it < EvtRed->CoboAsad.size(); it++)
		{
			co = EvtRed->CoboAsad[it].globalchannelid >> 11;
			as = (EvtRed->CoboAsad[it].globalchannelid - (co << 11)) >> 9;
			ag = (EvtRed->CoboAsad[it].globalchannelid - (co << 11) - (as << 9)) >> 7;
			ch = EvtRed->CoboAsad[it].globalchannelid - (co << 11) - (as << 9) - (ag << 7);
			int where = co * NB_ASAD * NB_AGET * NB_CHANNEL + as * NB_AGET * NB_CHANNEL + ag * NB_CHANNEL + ch;

			if (EvtRed->CoboAsad[it].hasSaturation)
			{
				hasSat = true;
				// 				cout << "saturations!!!" << endl;
			}
			if (hasSat)
				DECAY.NSaturations++;

			if (co == 31)
				for (unsigned int hit = 0; hit < EvtRed->CoboAsad[it].peaktime.size(); hit++)
				{
					int index = Evt->labelVXI[(int)(EvtRed->CoboAsad[it].peaktime[hit])];

					if (index == 0)
					{
						GATCONF = EvtRed->CoboAsad[it].peakheight[hit];
						hGATCONF->Fill(GATCONF);
					}
					if (index >= 1000 && index < 1011)
						hSi->Fill(index - 1000, EvtRed->CoboAsad[it].peakheight[hit]);
					if (index == 2000)
						E4D6 = EvtRed->CoboAsad[it].peakheight[hit];
					if (index == 3000)
						T3D6_HF = EvtRed->CoboAsad[it].peakheight[hit];
					if (index == 4000)
						T4D6_HF = EvtRed->CoboAsad[it].peakheight[hit];
				}

			if (co == 16)
				for (unsigned int hit = 0; hit < EvtRed->CoboAsad[it].peaktime.size(); hit++)
				{
					hDSSD->Fill(as * NB_AGET * NB_CHANNEL + ag * NB_CHANNEL + ch, EvtRed->CoboAsad[it].peakheight[hit]);
				}

			float Qpad = 0;
			if (co != 31 && co != 16)
				for (unsigned int hit = 0; hit < EvtRed->CoboAsad[it].peakheight.size(); hit++)
				{
					hPadSummary->Fill(where, pow(EvtRed->CoboAsad[it].peakheight[hit], 2) * CAL[co][as][ag][ch][0] + EvtRed->CoboAsad[it].peakheight[hit] * CAL[co][as][ag][ch][1] + CAL[co][as][ag][ch][2]);
					hTimeSummary->Fill(where, EvtRed->CoboAsad[it].peaktime[hit]);
					if (TABLE[4][where] != -1 && TABLE[5][where] != -1)
					{
						PAD[TABLE[4][where]][TABLE[5][where]] += (EvtRed->CoboAsad[it].peakheight[hit]); //*CAL[co][as][ag][ch][0]+CAL[co][as][ag][ch][1]);
						TIME[TABLE[4][where]][TABLE[5][where]] += EvtRed->CoboAsad[it].peaktime[hit] * Vdrift;
						XY->Fill(TABLE[4][where], TABLE[5][where], EvtRed->CoboAsad[it].peakheight[hit]);
						XZ->Fill(TABLE[4][where], EvtRed->CoboAsad[it].peaktime[hit] * Vdrift, EvtRed->CoboAsad[it].peakheight[hit]);
						YZ->Fill(TABLE[5][where], EvtRed->CoboAsad[it].peaktime[hit] * Vdrift, EvtRed->CoboAsad[it].peakheight[hit]);

						if (EvtRed->CoboAsad[it].peakheight[hit] > Qpad)
							Qpad = EvtRed->CoboAsad[it].peakheight[hit];
					}
				}
		}

		//   return(0);

		hId_1->Fill(T3D6_HF, E3D6);

		int cptImplant_1 = 0;
		int cptImplant_2 = 0;

		///////////////////////////////////////////////
		// Selection of the 53Co implantation events //
		///////////////////////////////////////////////
		/*
		//if(CUT46Cr->IsInside(T3D6_HF,E3D6) || CUT47Cr->IsInside(T3D6_HF,E3D6) || CUT48Mn->IsInside(T3D6_HF,E3D6) || CUT49Mn->IsInside(T3D6_HF,E3D6) || CUT50Mn->IsInside(T3D6_HF,E3D6) || CUT50Fe->IsInside(T3D6_HF,E3D6) || CUT51Fe->IsInside(T3D6_HF,E3D6) || CUT52Fe->IsInside(T3D6_HF,E3D6) || CUT52Co->IsInside(T3D6_HF,E3D6) || CUT53Co->IsInside(T3D6_HF,E3D6) || CUT54Co->IsInside(T3D6_HF,E3D6) || CUT54Ni->IsInside(T3D6_HF,E3D6) || CUT55Ni->IsInside(T3D6_HF,E3D6) || CUT55Cu->IsInside(T3D6_HF,E3D6) || CUT56Cu->IsInside(T3D6_HF,E3D6) || CUT57Zn->IsInside(T3D6_HF,E3D6))
		if(E3D6>1000)
		{
			if(CUT46Cr->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=24; IMPLANT.A=46;}
			else if(CUT47Cr->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=24; IMPLANT.A=47;}
			else if(CUT48Mn->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=25; IMPLANT.A=48;}
			else if(CUT49Mn->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=25; IMPLANT.A=49;}
			else if(CUT50Mn->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=25; IMPLANT.A=50;}
			else if(CUT50Fe->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=26; IMPLANT.A=50;}
			else if(CUT51Fe->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=26; IMPLANT.A=51;}
			else if(CUT52Fe->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=26; IMPLANT.A=52;}
			else if(CUT52Co->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=27; IMPLANT.A=52;}
			else if(CUT53Co->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=27; IMPLANT.A=53;}
			else if(CUT54Co->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=27; IMPLANT.A=54;}
			else if(CUT54Ni->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=28; IMPLANT.A=54;}
			else if(CUT55Ni->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=28; IMPLANT.A=55;}
			else if(CUT55Cu->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=29; IMPLANT.A=55;}
			else if(CUT56Cu->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=29; IMPLANT.A=56;}
			else if(CUT57Zn->IsInside(T3D6_HF,E3D6)) {IMPLANT.Z=30; IMPLANT.A=57;}
			else {IMPLANT.Z=0; IMPLANT.A=0;}

			IMPLANT.DE=E3D6;
			IMPLANT.TOF=T3D6_HF;

			hProfileX->Reset();
			hProfileY->Reset();
			for(int xx=0;xx<128;xx++)
				for(int yy=55;yy<73;yy++)
					hProfileX->Fill(xx,PAD[xx][yy]);

			for(int xx=128;xx>2;xx--)
				if(hProfileX->GetBinContent(xx)>0 && hProfileX->GetBinContent(xx-1)>0 && hProfileX->GetBinContent(xx-2)>0)
				{
					hImpRange->Fill(xx);
					IMPLANT.X=xx-1;
					xx=0;
				}

			for(int yy=0;yy<128;yy++)
				hProfileY->Fill(yy,PAD[IMPLANT.X-1][yy]);

			IMPLANT.Y=hProfileY->GetMaximumBin()-1;
			IMPLANT.TS=TS/1.E8;
			IMPLANT.EN=EN;
			IMPLANT.RUN=run_f+Tree->GetTreeNumber();


			hImpXY->Fill(IMPLANT.X,IMPLANT.Y);

			TS_PREV=EvtRed->timestamp;

			TreeOutImp->Fill();
		}
	*/

		///////////////////////////////////////////////
		//       Selection of the decay events       //
		///////////////////////////////////////////////
		/*	if(E3D6 < 450 )
	//  		if(true)
		{
			float VOX_THR=-1.0;
	// 			CleanPad(PAD,TIME,VOX_THR);
			std::vector <float> X, Y, Z, N;
			std::vector <float> Q;
			float alinexy, blinexy;
			float alinexz, blinexz;
			float alineyz, blineyz;
			float Qtot=0;
			int Npads=0;
	*/

		////////////////////////////////////////////////
		//       Voxels calib, 2D histo filling       //
		////////////////////////////////////////////////
		/*
			for(unsigned int it=0; it<EvtRed->CoboAsad.size(); it++)
			{
				co=EvtRed->CoboAsad[it].globalchannelid>>11 ;
				as=(EvtRed->CoboAsad[it].globalchannelid - (co<<11))>>9;
				ag=(EvtRed->CoboAsad[it].globalchannelid - (co<<11)-(as<<9))>>7;
				ch=EvtRed->CoboAsad[it].globalchannelid - (co<<11)-(as<<9)-(ag<<7);
				int where=co*NB_ASAD*NB_AGET*NB_CHANNEL + as*NB_AGET*NB_CHANNEL + ag*NB_CHANNEL + ch;
	// 				cout << EvtRed->CoboAsad[it].peakheight.size() << endl;
				float ChargeMax=0;
				if(co!=31 && co!=16) if(TABLE[4][where]!=-1 && TABLE[5][where]!= -1)
				{
					for(unsigned int hit=0;hit<EvtRed->CoboAsad[it].peakheight.size();hit++)
					{
						if(ch!=11 && ch!=22 && ch!=45 && ch!=56) hSignalCumul->Fill(EvtRed->CoboAsad[it].peaktime[hit],EvtRed->CoboAsad[it].peakheight[hit]);

						double Charge= EvtRed->CoboAsad[it].peakheight[hit]*CAL[co][as][ag][ch][0]+CAL[co][as][ag][ch][1];
						// if(Charge>VOX_THR)
						//if(Charge>CAL[co][as][ag][ch][2]+0.5 && EvtRed->CoboAsad[it].peaktime[hit]*Vdrift>30 && EvtRed->CoboAsad[it].peaktime[hit]*Vdrift<160)
						{
							X.push_back(TABLE[4][where]+0.5);
							Y.push_back(TABLE[5][where]+0.5);
							Z.push_back(EvtRed->CoboAsad[it].peaktime[hit]*Vdrift);
							Q.push_back(Charge);
							N.push_back(X.size()-10);
							Qtot+=Charge;
							XY->Fill(TABLE[4][where],TABLE[5][where],Charge);
							XZ->Fill(TABLE[4][where],EvtRed->CoboAsad[it].peaktime[hit]*Vdrift,Charge);
							YZ->Fill(TABLE[5][where],EvtRed->CoboAsad[it].peaktime[hit]*Vdrift,Charge);
							XYr->Fill(TABLE[4][where],TABLE[5][where],EvtRed->CoboAsad[it].peakheight[hit]);
							XZr->Fill(TABLE[4][where],EvtRed->CoboAsad[it].peaktime[hit],EvtRed->CoboAsad[it].peakheight[hit]);
							YZr->Fill(TABLE[5][where],EvtRed->CoboAsad[it].peaktime[hit],EvtRed->CoboAsad[it].peakheight[hit]);
							if(Charge>ChargeMax) ChargeMax=Charge;//EvtRed->CoboAsad[it].peakheight[hit];
	// 							if(co!=31 && co!=16 && hit==1) hPadSummary->Fill(where,Charge);//EvtRed->CoboAsad[it].peakheight[hit]);
							if(TABLE[4][where]==37 && TABLE[5][where]==35)
							{
								hSignal->Fill(EvtRed->CoboAsad[it].peaktime[hit],EvtRed->CoboAsad[it].peakheight[hit]);
								hSignal->SetTitle(Form("%d",where));
								tobedrawn=true;
							}
						}
					}
					if(ChargeMax>VOX_THR) Npads++;
				}
				hPadSummary->Fill(where,ChargeMax);//EvtRed->CoboAsad[it].peakheight[hit]);
			}


			hProfile1->Reset();
			hProfile2->Reset();
			hProfileNew2D->Reset();
			hZdistribution->Reset();
	*/

		////////////////////////////////////
		//       TRACKING algorithm       //
		////////////////////////////////////

		// 			int Ninliner=0;
		// 			if(X.size()) Ninliner=Ransac2D(Rand,X,Y,Q,alinexy,blinexy); FitMat(X,Y,Q,N,alinexy,blinexy);
		// 			T->L2DXY->SetX1(10.); T->L2DXY->SetY1(10.*alinexy+blinexy); T->L2DXY->SetX2(100.); T->L2DXY->SetY2(100.*alinexy+blinexy);
		// 			if(X.size()) Ransac2D(Rand,X,Z,Q,alinexz,blinexz); FitMat(X,Z,Q,N,alinexz,blinexz);
		// 			T->L2DXZ->SetX1(10.); T->L2DXZ->SetY1(10.*alinexz+blinexz); T->L2DXZ->SetX2(100.); T->L2DXZ->SetY2(100.*alinexz+blinexz);
		// 			if(X.size()) Ransac2D(Rand,Y,Z,Q,alineyz,blineyz); FitMat(Y,Z,Q,N,alineyz,blineyz);
		// 			T->L2DYZ->SetX1(10.); T->L2DYZ->SetY1(10.*alineyz+blineyz); T->L2DYZ->SetX2(100.); T->L2DYZ->SetY2(100.*alineyz+blineyz);

		/*			FitMat3D(X,Y,Z,Q,3.,T);
			alinexy=(T->Ym-T->Yh)/(T->Xm-T->Xh);
			alinexz=(T->Zm-T->Zh)/(T->Xm-T->Xh);
			alineyz=(T->Zm-T->Zh)/(T->Ym-T->Yh);
	*/
		//////////////////////////////////////////////////////
		//       Range & Consecutive Bins calculation       //
		//////////////////////////////////////////////////////
		/*
			int NConsecutiveBins=0;
			int NCB=0;
			int NH=0;

			DECAY.Range=Profile3D(X, Y, Z, Q, T, hProfile2, hProfileDer, 20, DECAY.Qmean, DECAY.isInverted, Mark);
	// 			DECAY.Range=Profile3D_v2(X, Y, Z, Q, T, hProfileNew2D, hZdistribution, hProfile2, hProfileDer, 50, DECAY.Qmean, DECAY.isInverted, Mark);

			DECAY.dQdX1=0;
			for(int bin=1;bin<hProfileDer->GetNbinsX();bin++)
			{
				if(DECAY.dQdX1>0 && hProfileDer->GetBinContent(bin)<DECAY.dQdX1+1)
					break;
				if(hProfileDer->GetBinContent(bin)>DECAY.dQdX1)
					DECAY.dQdX1=hProfileDer->GetBinContent(bin);
			}

			for(int bin=1;bin<hProfile1->GetNbinsX()-1;bin++)
			{
				if(hProfile2->GetBinContent(bin) && !hProfile2->GetBinContent(bin+1)) NH++;
			}
	*/

		////////////////////////////////////////////////////////////////////////////////////////////////
		//       Re-correct Range if Vdrift was divided by 10 (diminish the effects of shaping)       //
		////////////////////////////////////////////////////////////////////////////////////////////////

		// 			T->Zm*=10.;
		// 			T->Zh*=10.;
		// 			DECAY.Range=sqrt(pow(T->Xh-T->Xm,2) + pow(T->Yh-T->Ym,2) + pow(T->Zh-T->Zm,2));
		// 			alinexy=(T->Ym-T->Yh)/(T->Xm-T->Xh);
		// 			alinexz=(T->Zm-T->Zh)/(T->Xm-T->Xh);
		// 			alineyz=(T->Zm-T->Zh)/(T->Ym-T->Yh);

		/////////////////////////////////////////
		//       Track width computation       //
		/////////////////////////////////////////
		/*

			if(hProfileNew2D->ProjectionY("",10,10)->Integral()>0)
				hProfileNew2D->ProjectionY("",10,10)->Fit(fitGaus,"RQ","",-100,100);
			double width1=fitGaus->GetParameter(2);
			if(hProfileNew2D->ProjectionY("",11,11)->Integral()>0)
				hProfileNew2D->ProjectionY("",11,11)->Fit(fitGaus,"RQ","",-100,100);
			width1+=fitGaus->GetParameter(2);
			width1/=2.;
			double height1=(hZdistribution->ProjectionY("",10,10)->GetMean() + hZdistribution->ProjectionY("",11,11)->GetMean())/2.;

			int lastbinwidth=0;
			double width2=0;
			double height2=0;
			for(int bin=hProfileNew2D->GetNbinsX();bin>=0;bin--)
			{
				if(hProfileNew2D->ProjectionY("",bin+1,bin+1)->Integral()>0)
				{
					lastbinwidth++;
					if(lastbinwidth>9 && lastbinwidth<12)
					{
						hProfileNew2D->ProjectionY("",bin+1,bin+1)->Fit(fitGaus,"RQ","",-100,100);
						width2+=fitGaus->GetParameter(2)/2.;
						height2+=hZdistribution->ProjectionY("",bin+1,bin+1)->GetMean()/2.;
					}
					if(lastbinwidth>11) break;
				}
			}
			DECAY.Width1=width1;
			DECAY.Height1=height1;
			DECAY.Width2=width2;
			DECAY.Height2=height2;
	*/

		////////////////////////////////////////////////
		//       CrossHole condition evaluation       //
		////////////////////////////////////////////////
		/*		DECAY.crossHole=true;
			if(T->Xm>=0 && T->Xm<128 && T->Xh>=0 && T->Xh<128 && T->Ym>=0 && T->Ym<128 && T->Yh>=0 && T->Yh<128)
			{
				DECAY.crossHole=false;
				if(fabs(atan(alinexy))<TMath::Pi()/2.)
					for(float X=min(T->Xm,T->Xh);X<=max(T->Xm,T->Xh);X+=0.3)
					{
						for(int i=0;i<5;i++)
							if(X>XHole[i][0] && X<XHole[i][1] && (alinexy*X+T->Ym-alinexy*T->Xm)>YHole[i][0] && (alinexy*X+T->Ym-alinexy*T->Xm)<YHole[i][1])
							{
								DECAY.crossHole=true;
								X=max(T->Xm,T->Xh)+1;
								break;
							}
					}
				else for(float Y=min(T->Ym,T->Yh);Y<=max(T->Ym,T->Yh);Y+=0.3)
					{
						for(int i=0;i<5;i++)
							if(Y>YHole[i][0] && Y<YHole[i][1] && ((Y-T->Ym+alinexy*T->Xm)/alinexy)>YHole[i][0] && ((Y-T->Ym+alinexy*T->Xm)/alinexy)<YHole[i][1])
							{
								DECAY.crossHole=true;
								Y=max(T->Ym,T->Yh)+1;
								break;
							}
					}
			}
	*/

		////////////////////////////////////////
		//       Stops inside condition       //
		////////////////////////////////////////
		/*			bool stopsinside=true;

			if(T->Xm<1 || T->Xm>126 || T->Ym<1 || T->Ym>126)
				stopsinside=false;

			int Nrightside=0;
			if(T->Xh<2) for(int j=(int)max(T->Yh-5.,0.) ; j<(int)min(T->Yh+5.,127.) ; j++)
				if(j>=0 && j<128) if(PAD[0][j] || PAD[1][j]) Nrightside++;
			if(Nrightside>1) stopsinside=false;

			int Nleftside=0;
			if(T->Xh>124) for(int j=(int)max(T->Yh-5.,0.) ; j<(int)min(T->Yh+5.,127.) ; j++)
				if(j>=0 && j<128) if(PAD[126][j] || PAD[127][j]) Nleftside++;
			if(Nleftside>1) stopsinside=false;

			int Ntopside=0;
			if(T->Yh>124) for(int i=(int)max(T->Xh-5.,0.) ; i<(int)min(T->Xh+5.,127.) ; i++)
				if(i>=0 && i<128) if(PAD[i][126] || PAD[i][127]) Ntopside++;
			if(Ntopside>1) stopsinside=false;

			int Nbottomside=0;
			if(T->Yh<2) for(int i=(int)max(T->Xh-5.,0.) ; i<(int)min(T->Xh+5.,127.) ; i++)
				if(i>=0 && i<128) if(PAD[i][0] || PAD[i][1]) Nbottomside++;
			if(Nbottomside>1) stopsinside=false;
	*/
		// 			cout << entry << " " << stopsinside << "      "  << Nrightside << " " << Nleftside << " " << Ntopside << " " << Nbottomside << endl;
		//////////////////////////////////////
		//       Output TTree filling       //
		//////////////////////////////////////
		/*
			DECAY.Xstart=T->Xm;
			DECAY.Ystart=T->Ym;
			DECAY.Zstart=T->Zm;
			DECAY.Xstop=T->Xh;
			DECAY.Ystop=T->Yh;
			DECAY.Zstop=T->Zh;

			DECAY.theta_V=180.*atan((DECAY.Zstop-DECAY.Zstart)/sqrt(pow(DECAY.Xstop-DECAY.Xstart,2)+pow(DECAY.Ystop-DECAY.Ystart,2)))/3.14159;

			DECAY.theta_XY=180*atan(alinexy)/3.14159;
			if(DECAY.Zstop>DECAY.Zstart)
			{
				DECAY.theta_XZ=fabs(180*atan(alinexz)/3.14159);
				DECAY.theta_YZ=fabs(180*atan(alineyz)/3.14159);
			}
			else
			{
				DECAY.theta_XZ=-fabs(180*atan(alinexz)/3.14159);
				DECAY.theta_YZ=-fabs(180*atan(alineyz)/3.14159);
			}
			DECAY.stops=stopsinside;
			DECAY.Qtot=Qtot;
			DECAY.Qpic=hProfile2->GetMaximum();
			if(Npads>0) DECAY.Qavg=Qtot/Npads;
			else DECAY.Qavg=0;
			DECAY.RUN=run_f+Tree->GetTreeNumber();
			DECAY.TS=TS/1.E8;
			DECAY.EN=EN;
			TreeOutDec->Fill();

	*/
		///////////////////////////////
		//       Visualisation       //
		///////////////////////////////

		// if(CUTGQmR2->IsInside(DECAY.Qmean,DECAY.Range) && CUTGshit->IsInside(DECAY.Qmean,DECAY.Range) && DECAY.stops)
		// if(DECAY.EN==EvtNSel[iNSel] && DECAY.RUN==RunNSel[iNSel])
		if (/*DECAY.stops &&*/ entry % 1 == 0)
		{
			// 				iNSel++;
			// 				if(iNSel>NSel) break;

			C->SetTitle(S.Data());
			CId->Update();
			C->cd(1);
			XY->Draw("colz");
			XY->SetTitle(Form("Inv: %d", DECAY.isInverted));
			T->L2DXY->Draw("same");
			Mark[0]->Draw("same");
			Mark[3]->Draw("same");
			for (int i = 0; i < 5; i++)
				Hole[i]->Draw("L");
			C->cd(2);
			XZ->Draw("colz");
			T->L2DXZ->Draw("same");
			C->cd(3);
			YZ->Draw("colz");
			T->L2DYZ->Draw("same");
			// 				YZ->Draw("colz");
			C->cd(4);
			XY->SetTitle(Form("R: %f  - tv: %f", DECAY.Range, DECAY.theta_V));
			hProfile2->Draw("");
			// 				hProfile2->SetLineColor(2);
			//
			Mark[1]->Draw("same");
			Mark[2]->Draw("same");
			// 				fitBragg->SetParameter(0,DECAY.Range2-80);
			// 				cout << "initial param: " << fitBragg->GetParameter(0) << endl;
			// 				if(DECAY.Qpic2>210) hProfile2->Fit(fitBragg,"RQ","",10,DECAY.Range2+5);
			// 				else hProfile2->Fit(fitBragg,"RQ","",10,DECAY.Range2-5);
			// 				hDecRangeFit->Fill(81-fitBragg->GetParameter(0));
			// 				hZdistribution->Draw("colz");
			// 				C->cd(4);
			// 				XYr->Draw("colz");
			C->cd(5);
			hSi->Draw("colz");
			C->cd(6);
			hGATCONF->Draw("");
			C->Update();
			// 				C->SaveAs(Form("%sE%ld-R%ld-ENTRY%d.png",OutDirectory.Data(),EN,RUN,entry));
			// 				TFile* ff1=new TFile(Form("../Events.Jerome/run_0072_evt%06d_TR.root",DECAY.EN),"recreate");
			// 				XY->Write();
			// 				ff1->Close();
			// 				delete ff1;
			//				C->WaitPrimitive();
			// // 				C->WaitPrimitive();
			C->WaitPrimitive();

			////////////////////////////////////////////////
			//       CUT condition for picture view       //
			////////////////////////////////////////////////

			// 				if(CUTG3->IsInside(DECAY.Qpic2,DECAY.Range2)) C->SaveAs(Form("Out_20200415/CUTG3-stops_E%ld-R%ld-ENTRY%ld_Qpic%.1f-Range%.1f.png",DECAY.EN,DECAY.RUN,entry,DECAY.Qpic2,DECAY.Range2));
			// 				if(CUTG4->IsInside(DECAY.Qpic2,DECAY.Range2)) C->SaveAs(Form("Out_20200415/CUTG4-stops_E%ld-R%ld-ENTRY%ld_Qpic%.1f-Range%.1f.png",DECAY.EN,DECAY.RUN,entry,DECAY.Qpic2,DECAY.Range2));
			// 				if(CUTG5->IsInside(DECAY.Qpic2,DECAY.Range2)) C->SaveAs(Form("Out_20200415/CUTG5-stops_E%ld-R%ld-ENTRY%ld_Qpic%.1f-Range%.1f.png",DECAY.EN,DECAY.RUN,entry,DECAY.Qpic2,DECAY.Range2));
			// 				C->WaitPrimitive();
		}
	}
	//	}

	//	CId->cd();
	//	hSignalCumul->Draw("colz");
	//	CId->Update();
	// 	C->WaitPrimitive();
	// 	C->WaitPrimitive();

	Fout->Write();

	//	TFile* fff=new TFile("PadSummary.root","recreate");
	//	hPadSummary->Write();
	//	fff->Close();

	// 	TreeOutDec->Write();
	// 	TreeOutImp->Write();
	Fout->Close();

	return 0;
}

int Ransac2D(TRandom *Rand, std::vector<int> X, std::vector<int> Y, std::vector<float> Q, float &a, float &b)
{
	int k = 300;
	double dmax = 3.;
	int nmin = 40;

	int max_size = 0;
	float chi2min = 1E6;

	for (int i = 0; i < k; i++)
	{
		int nit = 0;
		unsigned int p1;
		do
		{
			p1 = (unsigned int)(Rand->Uniform(0, X.size()));
			nit++;
			if (nit > 5)
				return (-1);
		} while (!Q[p1]);
		unsigned int p2;
		nit = 0;
		do
		{
			p2 = (unsigned int)(Rand->Uniform(0, X.size()));
			nit++;
			if (nit > 5)
				return (-1);
		} while ((p2 == p1 || !Q[p2] || X[p2] == X[p1]));

		float aline, bline;
		aline = (Y[p2] - Y[p1]) / (X[p2] - X[p1]);
		bline = Y[p2] - aline * X[p2];

		std::vector<int> alsoin;
		alsoin = std::vector<int>();
		for (unsigned int p = 0; p < X.size(); p++)
			if (p != p1 && p != p2 && Q[p])
				if (fabs(aline * X[p] - Y[p] + bline) / sqrt(aline * aline + 1.) < dmax)
					alsoin.push_back(p);

		if (alsoin.size() > (unsigned int)nmin)
		{
			double chi2 = FitMat(X, Y, Q, alsoin, aline, bline);
			if (alsoin.size() > (unsigned int)max_size)
			{
				max_size = alsoin.size();
				a = aline;
				b = bline;
				chi2min = chi2;
			}
		}
	}

	return (max_size);
}

void PadRepair(float PAD[128][128], float TIME[128][128])
{
	if (PAD[63][54] && PAD[65][54])
		PAD[64][54] = (PAD[63][54] + PAD[65][54]) / 2.;
	if (PAD[65][54] && PAD[65][56])
		PAD[65][55] = (PAD[65][54] + PAD[65][56]) / 2.;
	if (PAD[63][58] && PAD[63][60])
		PAD[63][59] = (PAD[63][58] + PAD[63][60]) / 2.;
	if (PAD[62][57] && PAD[62][59])
		PAD[62][58] = (PAD[62][57] + PAD[62][59]) / 2.;
	// 	if(PAD[64][63] && PAD[64][66]) PAD[64][64]=PAD[64][65]=(PAD[64][63]+PAD[64][66])/2.;
	// 	if(PAD[65][63] && PAD[65][66]) PAD[65][64]=PAD[65][65]=(PAD[65][63]+PAD[65][66])/2.;
}

double BraggProfile(double *X, double *P)
{
	double x = X[0];
	// float XBragg[44]= {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,58,60,62,64,66,68,70,72,74,76,78,80,82,84,86};
	float YBragg[44] = {6.98732, 20.2008, 38.3615, 50.9091, 58.0761, 62.093, 65.1691, 67.3573, 68.9746, 69.8732, 70.1691, 70.7717, 72.0507, 73.5941, 75.4863, 77.6533, 79.6934, 82.1036, 84.4609, 87.2622, 89.8837, 92.8858, 96.3319, 100.148, 103.393, 106.247, 109.736, 113.679, 118.362, 123.319, 128.689, 134.313, 140.708, 149.239, 160.856, 177.125, 198.446, 220.677, 230.042, 199.101, 121.934, 45.6025, 11.3848, 5.20085};

	double res;
	int ind = (int)((x - P[0]) / 2.);
	if (ind < 1)
		res = YBragg[0];
	else if (ind > 43)
		res = YBragg[43];
	else
		res = YBragg[ind];

	return (res);
}

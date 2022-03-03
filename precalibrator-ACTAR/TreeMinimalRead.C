// prommenons
// Commetuasdegrandesdents

#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <TH2F.h>
#include <TF1.h>
#include <TGraph.h>
#include <TF1.h>
#include <TCutG.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TRandom.h>
#include <TDirectory.h>
#include <TDatime.h>
#include "TVector3.h"

#include "MEventReduced.h"
#include "MEvent.h"
#include <Parameters.h>
#include "Utils.h"
#include <algorithm>

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

TVector3 zero(0, 0, 0); // zero for spatial reference


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

int TreeMinimalRead(int run_f, int run_l)
{
    int UpdateFreq = 5000;

    TCanvas *C = new TCanvas("C", "C", 1350, 900);
    C->Divide(3, 1);

    TH2F *XY = new TH2F("XY", "XY", 128, 0, 128, 128, 0, 128);
    TH2F *XZ = new TH2F("XZ", "XZ", 128, 0, 128, 128, 0, 256);
    TH2F *YZ = new TH2F("YZ", "YZ", 128, 0, 128, 128, 0, 256);

    TString TreeName;
    TDatime TheDate;

    // 	FILE* ff=fopen("R26E70845.dat","w");

    TString OutDirectory;
    OutDirectory.Form("Out_%d/", TheDate.GetDate());
    system(Form("mkdir Out_%d", TheDate.GetDate()));
    TreeName.Form("Out_%d/Output_%d-%d.root", TheDate.GetDate(), run_f, run_l);
    TFile *Fout = new TFile(TreeName.Data(), "recreate");

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

    int TABLE[6][NB_COBO * NB_ASAD * NB_AGET * NB_CHANNEL];
    int TABLE_INV[128][128][4];

    // 	FILE* fTable=fopen("LT_GANIL_NewCF.dat","r");
    FILE *fTable = fopen("../dat/LT.dat", "r");
    if (fTable != NULL)
    {
        cout << "Using LookupTable from: "
             << "../dat/LT_GANIL.dat" << endl;

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
            // hCalibA->Fill(TABLE[4][indx], TABLE[5][indx], CAL[co][as][ag][ch][0]);
            indx++;
        }
        fclose(fCal);
    }

    TString fENName;
    fENName.Form("Run_%.4d.events", RUN);
    FILE *fEN = fopen(fENName.Data(), "w");

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
    float E3D6;    // Par.push_back(TString("E3D6"));      Numbers.push_back(0);
    float E4D6;    // Par.push_back(TString("E4D6"));      Numbers.push_back(0);
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
        // XYr->Reset();
        // YZr->Reset();
        // XZr->Reset();
        // hSignal->Reset();

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

        // DECAY.NSaturations = 0;

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
            // if (hasSat)
            //     DECAY.NSaturations++;

            if (co == 31)
                for (unsigned int hit = 0; hit < EvtRed->CoboAsad[it].peaktime.size(); hit++)
                {
                    int index = Evt->labelVXI[(int)(EvtRed->CoboAsad[it].peaktime[hit])];

                    // if (index == 0)
                    // {
                    //     GATCONF = EvtRed->CoboAsad[it].peakheight[hit];
                    //     hGATCONF->Fill(GATCONF);
                    // }
                    // if (index >= 1000 && index < 1011)
                    //     hSi->Fill(index - 1000, EvtRed->CoboAsad[it].peakheight[hit]);
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
                    // hPadSummary->Fill(where, pow(EvtRed->CoboAsad[it].peakheight[hit], 2) * CAL[co][as][ag][ch][0] + EvtRed->CoboAsad[it].peakheight[hit] * CAL[co][as][ag][ch][1] + CAL[co][as][ag][ch][2]);
                    // hTimeSummary->Fill(where, EvtRed->CoboAsad[it].peaktime[hit]);
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

        ///////////////////////////////
        //       Visualisation       //
        ///////////////////////////////

        if (entry % 1 == 0)
        {

            C->SetTitle(S.Data());
            // CId->Update();
            C->cd(1);
            XY->Draw("colz");
            // XY->SetTitle(Form("Inv: %d", DECAY.isInverted));
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
            C->Update();

            C->WaitPrimitive();
        }
    }

    Fout->Write();

    Fout->Close();

    return 0;
}

/*int Ransac(TRandom *Rand, std::vector<int> X, std::vector<int> Y, std::vector<float> Z, std::vector<float> Q, float &a, float &b)
{

    int rndpoint1 = 0; // points index into the hits vector
    int rndpoint2 = 0;
    int iterations = 0; // number of iterations
    array<Double_t, 4> sample1 = {0, 0, 0, 0};
    array<Double_t, 4> sample2 = {0, 0, 0, 0};
    array<TVector3, 2> maybeModel = {zero, zero};
    array<TVector3, 2> bestModel = {zero, zero};
    std::vector<int> alsoInliers; // vector to be filled with the possible indexes.
    std::vector<int> bestInliers; // vector to be filled with the best indexes.
    // cFittedLine<int> besttrack;
    Double_t besterr = DBL_MAX;
    Double_t thiserr = 0;
    std::vector<double> energie;
    double trackenergy = 0;

    double trackcount = 0;
    int beamtrackcount = 0;
    
    int loops = 10;

    std::vector<int> unfittedPoints;
    for (unsigned int i = 0; i < X.size(); i++)
    {
        unfittedPoints.push_back(i);
    } // it's a vector containing indexes. The ones that will be included in a cluster will be ==-1.

    trackcount = 0;

    while (1)
    { // looking for normal tracks. The method used is exactly the previous one.

        besterr = DBL_MAX;
        iterations = 0;
        bestInliers.clear();
        besttrack.getPoints().clear();
        trackenergy = 0;

        while (iterations < loops)
        {

            rndpoint1 = rnd->Uniform(0, X.size());
            rndpoint2 = rnd->Uniform(0, X.size());

            while (unfittedPoints[rndpoint1] == -1 || Y[rndpoint1] == -1 || X[rndpoint1] == -1)
            {
                rndpoint1 = rnd->Uniform(0, X.size());
            };
            while (unfittedPoints[rndpoint2] == -1 || Y[rndpoint2] == -1 || X[rndpoint2] == -1)
            {
                rndpoint2 = rnd->Uniform(0, X.size());
            };

            maybeModel[0].SetXYZ(0, 0, 0);
            maybeModel[1].SetXYZ(0, 0, 0);
            alsoInliers.clear();

            // WARNING First implementation : XY dimension, no Z. Will be added back
            sample1 = {X[rndpoint1], Y[rndpoint1], Z[i], Q[rndpoint1]}; 
            sample2 = {X[rndpoint2], Y[rndpoint2], Z[i], Q[rndpoint2]};

            maybeModel = GetParamFromSample(sample1, sample2);

            double GetErrorMean = 0;
            double sumvalue = 0;
            double totalenergy = 0;

            // For every point in data,
            // if point fits maybeModel with an error smaller than WIDTH,
            // add point to alsoInliers, compute totalenergy and compute sumvalue (it's used to test the inliers.)
            for (unsigned int i = 0; i < unfittedPoints.size(); i++)
            {
                if (unfittedPoints[i] == -1)
                {
                    continue;
                }

                array<Double_t, 4> samplex = {X[i], Y[i], Z[i], Q[i]};

                if (GetError(maybemodel, samplex) < pow(width, 2) && (hits[i].isTrackable() || hits[i].getY() > 40))
                {
                    iif(hits[i].getY()<4 || hits[i].getY()>60 || hits[i].getX()>120 || (hits[i].getX()<64 && hits[i].getX()>32 && hits[i].getY()>52) ){
                      gone=true;
                    }
                    alsoinliers.push_back(i);
                    arrayD4 samplex = {hits[i].getX(), hits[i].getY(), hits[i].getZ(), hits[i].getEnergy()};
                    sumvalue += GetError(maybemodel, samplex) * hits[i].getEnergy();
                    totalenergy += hits[i].getEnergy();

                } // end if ( GetError(maybemodel,samplex)<width*width && hits[i].isFittable() && i!=-1 )
            }     // end for (int i=0;i<unfittedPoints.size();i++)

            GetErrorMean = sumvalue / (totalenergy * alsoinliers.size()); //è una buona scelta

            if (alsoinliers.size() > trsize && GetErrorMean < besterr && totalenergy > threshold)
            {
                bestmodel = maybemodel;
                besterr = GetErrorMean;
                bestinliers = alsoinliers;
                trackenergy = totalenergy;
            } // end if (alsoinliers.size()>20 && GetErrorMean<besterr)

            iterations++;
        } // end while (iterations<loops)
        // cout << bestinliers.size() << "size" << endl;

        if (bestinliers.empty())
        {
            break;
        } // se non ho trovato tracce buone, fermati.

        double media = 0;
        double count = 0;
        for (unsigned int i = 0; i < bestinliers.size(); i++)
        {
            media += hits[bestinliers[i]].getY();
        }

        media /= bestinliers.size();

        vector<int> temporary;

        if (media > 34)
        {
            for (unsigned int i = 0; i < bestinliers.size(); i++)
            {
                if (hits[bestinliers[i]].getY() > 33.9)
                    temporary.push_back(bestinliers[i]);
            }
            bestinliers = temporary;
        }

        if (media < 30)
        {
            for (unsigned int i = 0; i < bestinliers.size(); i++)
            {
                if (hits[bestinliers[i]].getY() < 30.1)
                    temporary.push_back(bestinliers[i]);
            }
            bestinliers = temporary;
        }

        for (unsigned int i = 0; i < bestinliers.size(); i++)
        {
            unfittedPoints[bestinliers[i]] = -1;
        } // setto i punti clusterizzati a -1.

        if (trackenergy != 0)
        {
            energie.push_back(trackenergy / bestinliers.size());
        }
        besttrack.getPoints().insert(besttrack.getPoints().begin(), bestinliers.begin(), bestinliers.end());
        besttrack.setFittable(true);
        tracker->getLines().push_back(besttrack);

        trackcount++;
    } // end while (1)

    return 1;
}
*/

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

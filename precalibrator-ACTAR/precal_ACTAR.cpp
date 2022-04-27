
#include <stdio.h>
#include <iostream>
using namespace std;

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

#include "/home/lorenzo/Desktop/ARDAC/ARDA/inc/cPhysicalHit.h"
#include "/home/lorenzo/Desktop/ARDAC/ARDA/inc/cPhysicalEvent.h"
#include "/home/lorenzo/Desktop/ARDAC/ARDA/inc/cUserInput.h"
#include "/home/lorenzo/Desktop/ARDAC/ARDA/inc/cLookupTable.h"
#include "/home/lorenzo/Desktop/ARDAC/ARDA/inc/cRawEvent.h"

#include <chrono>

// gSystem->AddIncludePath("home/lorenzo/Desktop/ARDAC/ARDA/inc")

struct calCoeff
{
    Double_t m;
    Double_t q;
};

int preCal(int run_f, int run_l)
{

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
            // hCalibA->Fill(TABLE[4][indx], TABLE[5][indx], CAL[co][as][ag][ch][0]);
            indx++;
        }
        fclose(fCal);
    }

    // Open output file and save the calibrations used
    TFile fout("outputPrecalibrator.root", "RECREATE");
    if (fout.IsZombie())
    {
        std::cout << "Output file creation failed" << std::endl;
        return 0;
    }
    fout.cd();
    // foscal.Write("oscal");
    // fpadcal.Write("padcal");
    // velDat.Write("veldat");

    // Creating output tree
    TTree outTree("physicalEventTree", "physicalEventTree");
    cPhysicalEvent *phEvt = nullptr;
    outTree.Branch("event", &phEvt);
    outTree.SetDirectory(&fout);
    outTree.AutoSave();

    // Cycle on the input file

    // TFile fin(argv[argi]);
    // if (fin.IsZombie())
    // {
    //     cout << "Unable to open file " << argv[argi] << endl;
    //     continue;
    // }

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

    // Now I cicle all the events in the input TTree
    long int EntryTotal = Tree->GetEntries();
    cout << "Treating " << Tree->GetEntries() << " entries" << endl;
    long int ENTRIES[8] = {32, 38, 45, 60, 62, 63, 79, 81};

    for (long int entry = 0; entry < EntryTotal; entry++)
    // 	for(int ent=0;ent<8;ent++)
    {
        // 		long int entry=ENTRIES[ent];
        Tree->GetEntry(entry);

        if (!((entry + 1) % (EntryTotal / 100)))
        {
            cout << "In progress: " << entry * 100. / Tree->GetEntries() << " % "
                 << "     \r";
            cout.flush();
        }

        // WRITING SINGLE EVENT
        delete phEvt;
        phEvt = new cPhysicalEvent;

        phEvt->setEventNumber(EvtRed->event);
        phEvt->setTimestamp(EvtRed->timestamp);

        for (unsigned int it = 0; it < EvtRed->CoboAsad.size(); it++)
        {
            co = EvtRed->CoboAsad[it].globalchannelid >> 11;
            as = (EvtRed->CoboAsad[it].globalchannelid - (co << 11)) >> 9;
            ag = (EvtRed->CoboAsad[it].globalchannelid - (co << 11) - (as << 9)) >> 7;
            ch = EvtRed->CoboAsad[it].globalchannelid - (co << 11) - (as << 9) - (ag << 7);
            int where = co * NB_ASAD * NB_AGET * NB_CHANNEL + as * NB_AGET * NB_CHANNEL + ag * NB_CHANNEL + ch;

            cout << "To check if where and gID are the same... " << where << "   " << EvtRed->CoboAsad[it].globalchannelid << endl;

            float Qpad = 0;
            if (co != 31 && co != 16)
            {
                for (unsigned int hit = 0; hit < EvtRed->CoboAsad[it].peakheight.size(); hit++)
                {
                    if (TABLE[4][where] != -1 && TABLE[5][where] != -1)
                    {
                        cPhysicalHit phHit;
                        phHit.setGlobalChannelId(where);
                        phHit[0] = TABLE[4][where]; // lt->getTable()[gid].getCol() * velocity[0];
                        phHit[1] = TABLE[5][where]; // lt->getTable()[gid].getRow() * velocity[1];
                        phHit[2] = EvtRed->CoboAsad[it].peaktime[hit] * Vdrift;
                        phHit.setEnergy(EvtRed->CoboAsad[it].peakheight[hit]);
                        phHit.setTrackable(true);

                        phEvt->getHits().push_back(phHit);
                    }
                }
            }
        }

        outTree.Fill();
    }

    fout.cd();
    outTree.Write("", TObject::kOverwrite);

    cout << endl;

    return 0;
}
// if (argc < 3)
// {
//     cout << "Usage: precalibrator input1.root input2.root [...] output.root" << endl;
//     return 0;
// }

// Loading oscar calibration
/* map<int, calCoeff> oscarCal;
    TEnv foscal("oscal.cal");
    for (int i = 21000; i < 21400; i++)
    {
        string mname = to_string(i) + "m";
        string qname = to_string(i) + "q";

        if (foscal.Defined(mname.data()) && foscal.Defined(qname.data()))
        {
            calCoeff val;
            val.m = foscal.GetValue(mname.data(), val.m);
            val.q = foscal.GetValue(qname.data(), val.q);

            oscarCal.emplace(i, val);
        }
    }*/

// Loading pad calibration
/* map<int, calCoeff> padCal;
    TEnv fpadcal("padcal.cal");
    for (int i = 0; i < 20000; i++)
    {
        string mname = to_string(i) + "m";
        string qname = to_string(i) + "q";

        if (fpadcal.Defined(mname.data()) && fpadcal.Defined(qname.data()))
        {
            calCoeff val;
            val.m = fpadcal.GetValue(mname.data(), val.m);
            val.q = fpadcal.GetValue(qname.data(), val.q);

            padCal.emplace(i, val);
        }
    }*/

// Load conversion from ADC sample to distance
/* TVector3 velocity;
TEnv velDat("velocity.cal");
velocity[0] = velDat.GetValue("x", velocity[0]);
velocity[1] = velDat.GetValue("y", velocity[1]);
velocity[2] = velDat.GetValue("z", velocity[2]);
*/

// TTree *inTree = (TTree *)fin.Get("rawEventTree");
// if (!inTree)
// {
//     std::cout << "Unable to open TTree in file " << argv[argi] << std::endl;
// }

// Retrieve lookup table
// cLookupTable *lt = (cLookupTable *)fin.Get("lookupTable");
// if (!lt)
// {
//     cout << "Unable to open cLookupTable in file " << argv[argi] << endl;
// }

// TTreeReader rdr(inTree);
// TTreeReaderValue<cRawEvent> event(rdr, "event");
// Long64_t nent = rdr.GetEntries(false);

/*
    // while (rdr.Next())
    // {
    //     if (rdr.GetCurrentEntry() % 10 == 0)
    //     {
    //         cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;
    //     }

    //     // WRITING SINGLE EVENT
    //     delete phEvt;
    //     phEvt = new cPhysicalEvent;

    //     phEvt->setEventNumber(event->getEventNumber());
    //     phEvt->setTimestamp(event->getTimestamp());

    // SET RUNNUMBER!!!!

    // for (auto &hit : event->getHits())
    // {
    //     // WRITING SINGLE HIT
    //     int gid = hit.getGlobalChannelId();
    //     // HIT ON PADPLANE
    //     if (padCal.count(gid))
    //     {
    //         cPhysicalHit phHit;
    //         phHit.setGlobalChannelId(gid);
    //         phHit[0] = lt->getTable()[gid].getCol() * velocity[0];
    //         phHit[1] = lt->getTable()[gid].getRow() * velocity[1];
    //         phHit[2] = hit.getPeakTime() * velocity[2];
    //         phHit.setEnergy(hit.getPeakHeight() * padCal[gid].m + padCal[gid].q);
    //         phHit.setTrackable(true);

    //         // Noisy channels not to use for tracking
    //         if (gid >= 100 && gid < 200)
    //             phHit.setTrackable(false);

    //         // The hits on the beam are noisier, so it is better to add a cut
    //         if (phHit[1] >= 26. && phHit[1] <= 36. && phHit.GetEnergy() <= 60.)
    //             phHit.setTrackable(false);

    //         phEvt->getHits().push_back(phHit);
    //     }

    //     // HIT ON OSCAR
    //     if (gid >= 21000 && gid < 21500)
    //     {
    //         if (oscarCal.count(gid) == 1)
    //         {
    //             cPhysicalHit phHit;
    //             phHit.setGlobalChannelId(gid);

    //             phHit.setEnergy(hit.getPeakHeight() * oscarCal[gid].m + oscarCal[gid].q);
    //             phHit.setTrackable(false);

    //             phEvt->getHits().push_back(phHit);
    //         }
    //     }
    // }

    // outTree.Fill();

*/
#include <iostream>

#include "TCanvas.h"
#include "TH3F.h"
#include "TH1.h"
#include "TSystem.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"

#include "digit.h"

#include "/home/ats/ACTAR/ARDA-Ransac-Demonstrator/ARDA/inc/cPhysicalHit.h"
#include "/home/ats/ACTAR/ARDA-Ransac-Demonstrator/ARDA/inc/cPhysicalEvent.h"

using namespace std;

#if defined(__MAKECINT__)
#pragma link C++ class ActarPadSignal;
#pragma link C++ class projectionOnPadPlane;
#pragma link C++ class padsGeometry;
#pragma link C++ class amplificationManager;
#pragma link C++ class driftManager;
#endif

void convertSimToHits()
{

    TFile *_file0 = TFile::Open("root_files/dig_files/20_O_2H_transfer.root");
    TTree *digiTree = (TTree *)_file0->Get("digiTree");
    digiTree->Print();

    TClonesArray *padSignalCA = new TClonesArray("ActarPadSignal", 50);
    digiTree->SetBranchAddress("padSignals", &padSignalCA);
    ActarPadSignal *padSignal = new ActarPadSignal;

    std::vector<std::vector<float>> padCharge(100, std::vector<float>(100, 0));
    std::vector<std::vector<float>> padTime(100, std::vector<float>(100, 0));
    std::vector<std::vector<float>> padHeight(100, std::vector<float>(100, 0));

    double_t threshold = 0;
    Double_t thresholdADC = threshold + 0.1;

    // DEFINE OUTPUT FILE
    // Open output file and save the calibrations used
    TFile fout("outputConvertSimHits.root", "RECREATE");
    if (fout.IsZombie())
    {
        std::cout << "Output file creation failed" << std::endl;
        return;
    }
    fout.cd();

    // Creating output tree
    TTree outTree("physicalEventTree", "physicalEventTree");
    cPhysicalEvent *phEvt = new cPhysicalEvent;
    outTree.Branch("event", &phEvt);
    outTree.SetDirectory(&fout);
    outTree.AutoSave();

    double Vdrift = 1.;

    for (int i = 0; i < digiTree->GetEntries(); ++i)
    {

        phEvt->setEventNumber(i);
        phEvt->setTimestamp(i);

        digiTree->GetEntry(i);

        int numberOfPads = padSignalCA->GetEntries();

        for (int itPads = 0; itPads < numberOfPads; ++itPads)
        {

            cPhysicalHit phHit;

            padSignal = (ActarPadSignal *)padSignalCA->At(itPads);

            auto charge = padSignal->GetChargeDeposited();

            if (charge > thresholdADC)
            {

                phHit[0] = padSignal->GetPadRow();    // lt->getTable()[gid].getCol() * velocity[0];
                phHit[1] = padSignal->GetPadColumn(); // lt->getTable()[gid].getRow() * velocity[1];
                phHit[2] = (padSignal->GetInitTime() + padSignal->GetFinalTime()) / 2. * Vdrift;
                phHit.setEnergy(padSignal->GetChargeDeposited());
                phHit.setTrackable(true);

                phEvt->getHits().push_back(phHit);
            }
        }

        if (i % 2 != 0)
        {
            cout << "prima del fill" << endl;
            outTree.Fill();

            cout << "Arrivato al fill" << endl;

            delete phEvt;
            phEvt = new cPhysicalEvent;
        }
    }

    fout.cd();
    outTree.Write("", TObject::kOverwrite);

    cout << endl;
}

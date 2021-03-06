////////////////////////////////////////////////////
//                                                //
// L.Domenichetti  - LNL 2021                     //
// email: lorenzo.domenichetti@studenti.unipd.it  //
//                                                //
// ardaDrawEvent:    ARDA has to be loaded        //
//        - Outputs canvas with events.           //
//        - Arguments to be set: dimension        //
//          It has to be 2 or 3 (TH2 or TH3)      //
//                                                //
////////////////////////////////////////////////////

#include <iostream>
#include "TFile.h"
#include "TH3F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TEnv.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TVector3.h"
#include "LinkDef.h"
#include "TStyle.h"
#include <utility>
#include <map>
#include <string>
// include needed to avoid compile time issues.
// Remove it and use interpretation mode (ROOT 6.24)
#include "/home/lorenzo/Desktop/ARDAC/ARDA/inc/cPhysicalEvent.h"
using namespace std;

int ardaDrawEvent(int dimensions = 3)
{
    TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600);

    // input file in cPhysicalEvents
    // TString ifname = "/home/lorenzo/Desktop/ACTAR_git/precalibrator-ACTAR/pyOutputPrecal.root";
    TString ifname = "/home/lorenzo/Desktop/ROOTFiles/preCalE823_run0069.root"; // preCalE823_run0033.root

    TFile *ifile = new TFile(ifname.Data(), "READ");
    if (ifile->IsZombie())
    {
        printf("Unable to open inputfile: %s", ifname.Data());
        return -1;
    }
    TTree *physicalEventTree = (TTree *)ifile->Get("physicalEventTree");
    if (physicalEventTree->IsZombie())
    {
        printf("Unable to open inputtree: ");
        return -1;
    }

    TTreeReader rdr(physicalEventTree);
    TTreeReaderValue<cPhysicalEvent> event(rdr, "event"); // reading input file

    TH2F *h2 = new TH2F();
    TH3F *h3 = new TH3F();

    if (dimensions != 2 && dimensions != 3)
    {
        cout << "Input dimension has to be 2 or 3! Unable to build histogram pointer." << endl;
        return -1;
    }

    std::cout << rdr.GetEntries() << std::endl;
    while (rdr.Next())
    {
        list<cPhysicalHit> hitslist = event->getHits(); // picking up the list from the event and converting it into a vector.

        // if (event->getEventNumber() != 331)
        //     continue;

        cout << event->getEventNumber() << endl;
        if (dimensions == 2)
        {
            delete h2;
            h2 = new TH2F("histo2D", "", 128, 0, 128, 128, 0, 128);
        }
        else if (dimensions == 3)
        {
            delete h3;
            h3 = new TH3F("histo3D", "histo3D", 128, 0, 128, 128, 0, 128, 100, 0, 500);
        }
        for (auto &i : hitslist)
        {
            if (dimensions == 2)
            {

                // box dimension is energy
                h2->Fill(i[0], i[1], i.getEnergy());

                // same box dimension
                // h2->Fill(i[0], i[1]);
            }
            else if (dimensions == 3)
            {
                h3->Fill(i[0], i[1], i[2]);
            }
        }

        if (dimensions == 2)
        {
            h2->GetXaxis()->SetTitle("Asse x [mm/2]");
            h2->GetYaxis()->SetTitle("Asse y [mm/2]");
            h2->Draw("COLZ");
        }

        if (dimensions == 3)
            h3->Draw("LEGO");

        canvas->WaitPrimitive();
        canvas->Update();
    }

    return 1;
}


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
// #include "LinkDef.h"
#include "TStyle.h"
#include <utility>
#include <map>
#include <string>
// include needed to avoid compile time issues.
// Remove it and use interpretation mode (ROOT 6.24)
#include "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/inc/cPhysicalEvent.h"
using namespace std;

int eventDiagnostic()
{
    // input file in cPhysicalEvents
    // TString ifname = "/home/lorenzo/Desktop/simACTAR/ActarsimBuild/outputConvertSimHits.root"; // preCalE823_run0033.root
    TString ifname = "/home/lorenzo/Desktop/ROOTFiles/100SelectedEvents.root"; // preCalE823_run0033.root


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

    TH1F *hZ = new TH1F("hZ", "hZ", 250, 0, 250);
    TH1F *hEnergy = new TH1F("hEnergy", "Energy Spectrum", 250, 0, 100000);

    while (rdr.Next())
    {
        for (auto &i : event->getHits())
        {
            hEnergy->Fill(i.getEnergy());
            hZ->Fill(i[2]);
        }
    }

    TCanvas *canvasEnergy = new TCanvas("canvasE", "canvasE", 800, 600);
    canvasEnergy->cd();
    hEnergy->Draw();

    TCanvas *canvasZ = new TCanvas("canvasZ", "canvasZ", 800, 600);
    canvasZ->cd();
    hZ->Draw();

    canvasZ->WaitPrimitive();
    return 1;
}
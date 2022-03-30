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

int fitYoloDraw()
{

    int iteration = 0;

    int dimensions = 3;
    TCanvas *canvas = NULL;

    // input file in cPhysicalEvents
    TString ifname = "../../ROOTFiles/outputPrecalibrator_run140.root";

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

    while (rdr.Next())
    {
        iteration += 1;

        list<cPhysicalHit> hitslist = event->getHits(); // picking up the list from the event and converting it into a vector.

        cout << event->getEventNumber() << endl;

        canvas = new TCanvas("canvas", "canvas", 800, 600);

        h3 = new TH3F("histo3D", "", 128, 0, 128, 128, 0, 128, 100, 0, 500);

        for (auto &i : hitslist)
        {
            h3->Fill(i[0], i[1], i[2]);
        }

        h3->Draw("LEGO");

        h3->GetXaxis()->SetLabelOffset(999);
        h3->GetYaxis()->SetLabelOffset(999);
        h3->GetZaxis()->SetLabelOffset(999);

        h3->GetXaxis()->SetAxisColor(0);
        h3->GetYaxis()->SetAxisColor(0);
        h3->GetZaxis()->SetAxisColor(0);
        
        // canvas->WaitPrimitive();
        // canvas->Update();

        string title = "Images/picture_" + to_string(iteration) + ".jpg";
        canvas->SaveAs(title.c_str());


        delete canvas;
        delete h3;
    }

    return 1;
}
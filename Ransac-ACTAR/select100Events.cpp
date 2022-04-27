#include <iostream>
#include <fstream>
#include <array>
#include <cmath>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <list>
#include <time.h>

#include "TFile.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TMinuit.h"
#include "TEnv.h"
#include "TTree.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TPolyLine3D.h"
#include "TTreeReader.h"
#include "TRandom3.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH3F.h"

// #include "../commonDependencies/cPhysicalHit.h"
// #include "../commonDependencies/cPhysicalEvent.h"
// #include "../commonDependencies/cTrackerFine.h"
// #include "../commonDependencies/cFittedEvent.h"
// #include "../commonDependencies/cVertexFinder.h"
// #include "../commonDependencies/cDrawEvents.h"
// #include "../commonDependencies/cTrackerRansac.h"
// #include "../commonDependencies/cUtils.h"


#include "cPhysicalHit.h"
#include "cPhysicalEvent.h"
 #include "cTrackerFine.h"
#include "cFittedEvent.h"
#include "cVertexFinder.h"
#include "cDrawEvents.h"
#include "cTrackerRansac.h"
#include "cUtils.h"

#ifdef __CLING__
#pragma link C++ class cTrackerRansac < cPhysicalHit> + ;
#endif

using namespace std;

int fit(string inputFileName = "../../ROOTFiles/outputPrecalibrator_run140.root")
{

    std::ifstream inFile;
    inFile.open("../goodEvents.txt");
    std::vector<int> goodEvts;
    int temp;
    while (inFile >> temp)
    {
        goodEvts.push_back(temp);
    }

    std::cout << "gEvt.size()" << goodEvts.size() << std::endl;

    TString dataFileName = inputFileName;
    // Opening the input file.
    TFile *ifile = new TFile(dataFileName.Data(), "READ");
    if (ifile->IsZombie())
    {
        printf("Unable to open inputfile: %s", dataFileName.Data());
        return -1;
    }

    gROOT->SetBatch(kFALSE);

    TTree *physicalEventTree = (TTree *)ifile->Get("physicalEventTree");
    if (physicalEventTree->IsZombie())
    {
        printf("Unable to open inputtree: ");
        return -1;
    }

    TTreeReader rdr(physicalEventTree);
    TTreeReaderValue<cPhysicalEvent> event(rdr, "event"); // reading input file

    // Open output file
    TString filename = "prova100.root";
    TFile fout(filename.Data(), "RECREATE");
    if (fout.IsZombie())
    {
        cout << "Output file creation failed" << endl;
        return 0;
    }
    fout.cd();


    TTree fOutTree("physicalEventTree", "physicalEventTree");
    cPhysicalEvent *fitEvt = nullptr;
    fOutTree.Branch("event", &fitEvt);
    fOutTree.SetDirectory(&fout);
    fOutTree.AutoSave();

    int nent = rdr.GetEntries(false);

    int it_count = -1;

    int gEvt_it = 0;

    while (rdr.Next())
    {

        // cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;

        if (rdr.GetCurrentEntry() < goodEvts[gEvt_it] || goodEvts[gEvt_it] < 0)
        {
            continue;
        }
        std::cout << rdr.GetCurrentEntry() << "   " << goodEvts[gEvt_it] <<  std::endl;
        gEvt_it++;

        if (rdr.GetCurrentEntry() > 800)
        {
            break;
        }

        delete fitEvt;
        fitEvt = new cPhysicalEvent();

        std::cout << "evtNumber" << event->getEventNumber() << std::endl;
        fitEvt->setEventNumber(event->getEventNumber());
        fitEvt->setTimestamp(event->getTimestamp());
        fitEvt->getHits().clear();

        for (auto &it_list : event->getHits())
        {
            fitEvt->getHits().push_back(it_list);
        }

        fOutTree.Fill();
    }
    fout.cd();
    fOutTree.Write();
    fout.Close();
    return 0;
}

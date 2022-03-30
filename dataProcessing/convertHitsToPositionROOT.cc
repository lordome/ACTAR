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

#include "../commonDependencies/cPhysicalHit.h"
#include "../commonDependencies/cPhysicalEvent.h"
#include "../commonDependencies/cTrackerFine.h"
#include "../commonDependencies/cFittedEvent.h"
#include "../commonDependencies/cVertexFinder.h"
#include "../commonDependencies/cDrawEvents.h"
#include "../commonDependencies/cTrackerRansac.h"
#include "../commonDependencies/cUtils.h"

#ifdef __CLING__
#pragma link C++ class cTrackerRansac < cPhysicalHit> + ;
#endif

using namespace std;

int fit(TString inputFileName = "run140Analysed.root")
{
    // Opening the input file.
    TFile *ifile = new TFile(inputFileName.Data(), "READ");
    if (ifile->IsZombie())
    {
        printf("Unable to open inputfile: %s", inputFileName.Data());
        return -1;
    }

    gROOT->SetBatch(kFALSE);

    TTree *trackTree = (TTree *)ifile->Get("trackTree");
    if (trackTree->IsZombie())
    {
        printf("Unable to open inputtree: ");
        return -1;
    }

    // cFittedEvent<cPhysicalHit> *fitEvt = new cFittedEvent<cPhysicalHit>();

    TTreeReader rdr(trackTree);
    TTreeReaderValue<cFittedEvent<cPhysicalHit>> fitEvt(rdr, "Full_event"); // reading input file

    int nent = rdr.GetEntries(false);

    TH1F *hZ = new TH1F("hZ", "hZ", 512, 0, 512);

    TString filename = "dataForNN.root";
    TFile fout(filename.Data(), "RECREATE");
    if (fout.IsZombie())
    {
        cout << "Output file creation failed" << endl;
        return 0;
    }
    fout.cd();

    std::vector<std::vector<double>> grid(128, std::vector<double>(128,0));

    TTree *fOutTree = new TTree("trackTree", "trackTree");
    fOutTree->Branch("grid", &grid);
    fOutTree->SetDirectory(&fout);
    fOutTree->AutoSave();

    while (rdr.Next())
    {

        //  if (rdr.GetCurrentEntry() >= it_count)
        // {
        //     it_count += 100;
        // }

        std::vector<std::vector<double>> tmp_grid(128, std::vector<double>(128,0));

        cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;

        for (auto &it_lines : fitEvt->getLines())
        {

            for (auto &it_hits : it_lines.getPoints())
            {
                tmp_grid[(int)it_hits.getX()][(int)it_hits.getY()] += it_hits.getEnergy();
            }
        }

        grid = tmp_grid;
        fOutTree->Fill();
    }

    fOutTree->Write();
    return 0;
}

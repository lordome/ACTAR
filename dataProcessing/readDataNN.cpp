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

#include "../commonDependencies/cTrackerFine.h"
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

    TTreeReader rdr(trackTree);
    TTreeReaderValue<std::vector<std::vector<double>>> grid(rdr, "grid"); // reading input file

    int nent = rdr.GetEntries(false);

    while (rdr.Next())
    {

        TH2F *h2d = new TH2F("histo2D", "histo2D", 128, 0, 128, 128, 0, 128);

        for(int i = 0; i<128; i++){
            for(int j=0; j<128; j++){
                h2d->Fill(i,j,(*grid)[i][j]);
            }
        }

        
        TCanvas *c = new TCanvas();
        h2d->Draw("colz");

        c->WaitPrimitive();
        delete c;
    }

    return 0;
}

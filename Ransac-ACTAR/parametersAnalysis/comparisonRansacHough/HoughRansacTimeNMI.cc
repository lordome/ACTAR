
#include <TROOT.h>
#include <functional>
#include <numeric>
#include <TChain.h>
#include <TFile.h>
#include <fstream>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TH1.h>
#include "TProofOutputFile.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TCanvas.h"
#include "TH3F.h"
#include "TGraph.h"

// Headers needed by this particular selector
// #include "../root_e796/LinkDef.h"
// #include "../commonDependencies/cPhysicalHit.h"
// #include "../commonDependencies/cPhysicalEvent.h"
// #include "cFittedEvent.h"
// #include "cTrackerFine.h"
// #include "cVertexFinder.h"
// #include "cDrawEvents.h"
// #include "cUtils.h"

using namespace std;

int fit(TString inputFileName = "100EventsHough.root")
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

    // READING INPUT FILE
    TTreeReader rdr(trackTree);
    TTreeReaderValue<double> eventNum(rdr, "eventNum");
    TTreeReaderValue<double> executionTime(rdr, "executionTime");

    TTreeReaderValue<map<string, double>> parMap(rdr, "parameters");
    TTreeReaderValue<TString> inputFile(rdr, "inputFile");

    TTreeReaderValue<map<int, int>> identificationMap(rdr, "identificationMap");

    while (rdr.Next())
    {

        cout << *eventNum << endl;

        for (auto& itMap: identificationMap){
            
        }
    }

    // TCanvas *c1 = new TCanvas("c1", "c1", 200, 10, 500, 300);
    // TH2D *h2D_1 = new TH2D("h2d_1", "h2d_1", 10, 0, 500, 10, 0, 500);
    // for (unsigned int i = 0; i < vecParams.size(); i++)
    // {
    //     cout << vecParams[i].first << "  " << vecParams[i].second << "  " << energiesGraph[i] << endl;
    //     h2D_1->Fill(vecParams[i].first, vecParams[i].second, energiesGraph[i]);
    // }
    // h2D_1->Draw("COLZ");

    return 100;
}

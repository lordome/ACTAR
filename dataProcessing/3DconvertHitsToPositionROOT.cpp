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
#include "TPolyLine3D.h"
#include "TTreeReader.h"
#include "TRandom3.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TH3F.h"

// #include "../commonDependencies/cPhysicalHit.h"
// #include "../commonDependencies/cPhysicalEvent.h"
// #include "../commonDependencies/cFittedEvent.h"

#include "../commonDependencies/cTrackerFine.h"
#include "../commonDependencies/cVertexFinder.h"
#include "../commonDependencies/cDrawEvents.h"
#include "../commonDependencies/cTrackerRansac.h"
#include "../commonDependencies/cUtils.h"

#ifndef __loaderVEC__
#pragma link C++ class vector < vector < vector < double>>> + ;
#endif

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

    std::vector<std::vector<std::vector<double>>> grid(3, std::vector<std::vector<double>>(128, std::vector<double>(128, 0)));

    TTree *fOutTree = new TTree("trackTree", "trackTree");
    fOutTree->Branch("grid", &grid);
    fOutTree->SetDirectory(&fout);
    fOutTree->AutoSave();

    std::vector<int> vertexSize;
    int counter = 0;
    while (rdr.Next())
    {
        counter++;
        if (counter > 10000)
        {
            continue;
        }
        grid = std::vector<std::vector<std::vector<double>>>(3, std::vector<std::vector<double>>(128, std::vector<double>(128, 0)));

        std::vector<std::vector<double>> xy_grid(128, std::vector<double>(128, 0));
        std::vector<std::vector<double>> xz_grid(128, std::vector<double>(128, 0));
        std::vector<std::vector<double>> yz_grid(128, std::vector<double>(128, 0));

        std::vector<std::vector<std::vector<double>>> tmp_grid(3, std::vector<std::vector<double>>(128, std::vector<double>(128, 0)));

        cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;

        for (auto &it_lines : fitEvt->getLines())
        {
            for (auto &it_hits : it_lines.getPoints())
            {
                grid[0][(int)it_hits.getX()][(int)it_hits.getY()] += it_hits.getEnergy();
                grid[1][(int)it_hits.getX()][(int)(it_hits.getZ() / 4.)] += it_hits.getEnergy();
                grid[2][(int)it_hits.getY()][(int)(it_hits.getZ() / 4.)] += it_hits.getEnergy();
            }
        }
        // grid = tmp_grid;
        fOutTree->Fill();

        vertexSize.push_back(fitEvt->getVertex().size());
    }

    fOutTree->Write();

    std::ofstream output_file("./vertSize_10000evt.txt");
    std::ostream_iterator<int> output_iterator(output_file, "\n");
    std::copy(vertexSize.begin(), vertexSize.end(), output_iterator);

    return 0;
}

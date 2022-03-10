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

int fit(TString inputFileName = "provaOutputcFittedEvent.root")
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

    TH1F *h1 = new TH1F("histAng", "histAng", 270, 0, 90);

    TH1F *hPosVertex = new TH1F("hPosVertex", "hPosVertex", 200, -120, 150);

    TH2F *hAngEnergy = new TH2F("histAng", "histAng", 270, 0, 90, 500, 0, 300000);

    while (rdr.Next())
    {

        int binX = 128;
        int binY = 128;
        int binZ = 100;
        float maxX = 128.;
        float maxY = 128.;
        float maxZ = 500.;

        cDrawEvents<cFittedEvent<cPhysicalHit>> *drawEvt =
            new cDrawEvents<cFittedEvent<cPhysicalHit>>(binX, binY, binZ, maxX, maxY, maxZ);

        drawEvt->setEvent(*fitEvt);
        // drawEvt->drawComponents2D(false, 0, 0, 800, 500);
        // drawEvt->drawColors2D(false, 0, 600, 800, 500);
        // drawEvt->drawTracks3D(true, 800, 0, 1115, 1000);

        delete drawEvt;

        for (auto &it_lines : fitEvt->getLines())
        {
            double ang = 0;
            ang = fabs(it_lines.getDirection().Phi() * 180 / M_PI);
            if (ang > 90)
            {
                ang = -1. * (ang - 180);
            }

            double encount = 0;
            for (auto &it_hits : it_lines.getPoints())
            {
                encount += it_hits.getEnergy();
            }
            h1->Fill(ang);

            hAngEnergy->Fill(ang, encount);
        }

        for (auto &it_ver : fitEvt->getVertex())
        {
            TVector3 vec = TVector3(it_ver);
            hPosVertex->Fill(vec[0]);
        }
    }

    h1->Draw();

    new TCanvas();
    hAngEnergy->Draw("colz");

    new TCanvas();
    hPosVertex->Draw();

    return 0;
}

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

    while (rdr.Next())
    {

        // for (auto &line : fitEvt->getLines())
        // {
        //     double energyCount = 0;
        //     for (auto &pts : line.getPoints())
        //     {
        //         energyCount += pts.getEnergy();
        //     }
        // }

        // Draw the analysed Event;

        int binX = 128;
        int binY = 128;
        int binZ = 100;
        float maxX = 128.;
        float maxY = 128.;
        float maxZ = 500.;

        cDrawEvents<cFittedEvent<cPhysicalHit>> *drawEvt =
            new cDrawEvents<cFittedEvent<cPhysicalHit>>(binX, binY, binZ, maxX, maxY, maxZ);

        drawEvt->setEvent(*fitEvt);
        drawEvt->drawComponents2D(false, 0, 0, 800, 500);
        drawEvt->drawColors2D(false, 0, 600, 800, 500);
        drawEvt->drawTracks3D(true, 800, 0, 1115, 1000);

        delete drawEvt;
    }

    return 0;
}

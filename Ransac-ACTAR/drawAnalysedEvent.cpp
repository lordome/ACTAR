////////////////////////////////////////////////////
//                                                //
// L.Domenichetti  - LNL 2021                     //
// email: lorenzo.domenichetti@studenti.unipd.it  //
//                                                //
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
#include "../commonDependencies/cPhysicalHit.h"
#include "../commonDependencies/cPhysicalEvent.h"
#include "../commonDependencies/cTrackerFine.h"
#include "../commonDependencies/cFittedEvent.h"
#include "../commonDependencies/cVertexFinder.h"
#include "../commonDependencies/cDrawEvents.h"
#include "../commonDependencies/cTrackerRansac.h"
#include "../commonDependencies/cUtils.h"
#include "../commonDependencies/cVertex.h"

using namespace std;

int fit(string inputFileName = "input_parameters_draw.txt")
{
    TString dataFileName;
    map<string, double> parMap;
    getInputMap(inputFileName, parMap, dataFileName);

    // TString dataFileName; = dataFileName;

    // Drawing parameters
    int binX = parMap["binX"];
    int binY = parMap["binY"];
    int binZ = parMap["binZ"];
    float maxX = parMap["maxX"];
    float maxY = parMap["maxY"];
    float maxZ = parMap["maxZ"];

    int startFrom = parMap["startFrom"];

    // Opening the input file.
    TFile *ifile = new TFile(dataFileName.Data(), "READ");
    if (ifile->IsZombie())
    {
        printf("Unable to open inputfile: %s", dataFileName.Data());
        return -1;
    }

    TTree *physicalEventTree = (TTree *)ifile->Get("trackTree");
    if (physicalEventTree->IsZombie())
    {
        printf("Unable to open inputtree: ");
        return -1;
    }

    // cFittedEvent<cPhysicalHit> *fitEvt = new cFittedEvent<cPhysicalHit>();

    TTreeReader rdr(physicalEventTree);
    TTreeReaderValue<cFittedEvent<cPhysicalHit>> fitEvt(rdr, "Full_event"); // reading input file

    int nent = rdr.GetEntries();

    while (rdr.Next())
    {

        if (rdr.GetCurrentEntry() > startFrom)
        {
            cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;

            for (auto &it_ver : fitEvt->getVertex())
            {

                cout << "Entry = " << rdr.GetCurrentEntry() << endl;
                TVector3 vec = TVector3(it_ver);
                cout << vec[0] << "   " << fitEvt->getVertex().size() << endl;
            }

            // drawEvt->drawTracks3D(true, 800, 600, 1115, 500);

            cDrawEvents<cFittedEvent<cPhysicalHit>> *drawEvt =
                new cDrawEvents<cFittedEvent<cPhysicalHit>>(binX, binY, binZ, maxX, maxY, maxZ);

            drawEvt->setEvent(*fitEvt);
            // drawEvt->drawAll3D(false);
            // drawEvt->drawAll2D(false);
            drawEvt->drawComponents2D(false, 0, 0, 800, 500);
            drawEvt->drawColors2D(false, 1000, 600, 800, 500);
            drawEvt->drawVertex(true, 800, 0, 1115, 500);

            delete drawEvt;
        }
    }

    return 1;
}

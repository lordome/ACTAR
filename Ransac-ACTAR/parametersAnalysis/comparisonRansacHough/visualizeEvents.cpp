
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
#include "../../../commonDependencies/cPhysicalHit.h"
#include "../../../commonDependencies/cPhysicalEvent.h"
#include "cFittedLine.h"
#include "cFittedEvent.h"
// #include "cTrackerFine.h"
// #include "cVertexFinder.h"
#include "../../../commonDependencies/cDrawEvents.h"
#include "../../../commonDependencies/cUtils.h"

using namespace std;

std::vector<int> decode(int value)
{
    int z = int(value / (128 * 128));
    value = value - z * 128 * 128;
    int y = int(value / (128));
    value = value - y * 128;
    return std::vector<int>({value, y, z});
}

int fit(TString inputFileName = "prova.root")
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
    // TTreeReaderValue<double> executionTime(rdr, "executionTime");

    TTreeReaderValue<map<string, double>> parMap(rdr, "parameters");
    TTreeReaderValue<TString> inputFile(rdr, "inputFile");

    TTreeReaderValue<map<int, int>> identificationMap(rdr, "identificationMap");

    while (rdr.Next())
    {

        // cout << *eventNum << endl;

        auto max = std::max_element((*identificationMap).begin(), (*identificationMap).end(), [](const auto &x, const auto &y)
                                    { return x.second < y.second; });
        // vector<cFittedLine<cPhysicalHit>> vecLines(max->second - 1);

        vector<cFittedLine<cPhysicalHit>> vecLines(max->second, cFittedLine<cPhysicalHit>());

        // for (int i = 0; i < max->second - 1; ++i)
        // {
        //     vecLines.push_back(new );
        // }

        std::list<cPhysicalHit> unfPts;

        cout << max->second << "  " << vecLines.size() << endl;

        cPhysicalHit *tmpHit = 0;
        for (auto &itMap : *identificationMap)
        {
            tmpHit = new cPhysicalHit;

            auto decoded = decode(itMap.first);
            tmpHit->setX(decoded[0]);
            tmpHit->setY(decoded[1]);
            tmpHit->setZ(decoded[2]);

            // cout << "ti rompi qui?" << itMap.second << endl;

            if (itMap.second > 0)
            {
                vecLines.at(itMap.second - 1).getPoints().push_back(*tmpHit);
            }
            else
            {
                unfPts.push_back(*tmpHit);
            }
        }

        cFittedEvent<cPhysicalHit> *fitEvt = new cFittedEvent<cPhysicalHit>;

        std::list<cFittedLine<cPhysicalHit>> lstLines;
        lstLines.insert(lstLines.begin(), vecLines.begin(), vecLines.end());

        fitEvt->setLines(lstLines);
        fitEvt->setUnfittedPoints(unfPts);

        int binX = 128;
        int binY = 128;
        int binZ = 128;
        float maxX = 128;
        float maxY = 128;
        float maxZ = 512;

        cDrawEvents<cFittedEvent<cPhysicalHit>> *drawEvt = new cDrawEvents<cFittedEvent<cPhysicalHit>>(binX, binY, binZ, maxX, maxY, maxZ);
        drawEvt->setEvent(*fitEvt);
        drawEvt->drawColors2D(false, 0, 0, 800, 500, "CanvasColors");
        drawEvt->drawComponents2D(true, 800, 0, 800, 500, " components2D");

        // cFittedEvent(int &event, int &run, int &time, std::list<cFittedLine<T>> &lin, std::list<T> &unfPts, std::list<T> &ancHit, std::list<cVertex<T>> &ver);
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

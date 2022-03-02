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

// #include "ARDA_extraClasses/cPhysicalHit.h"
// #include "ARDA_extraClasses/cPhysicalEvent.h"
// #include "ARDA_extraClasses/cFittedLine.h"
// #include "ARDA_extraClasses/cFittedEvent.h"

#include "../root_e796/ARDA_extraClasses/cPhysicalHit.h"
#include "../root_e796/ARDA_extraClasses/cPhysicalEvent.h"
#include "../trackingHough/cTrackerFine.h"
#include "../trackingHough/cFittedEvent.h"
#include "../trackingHough/cVertexFinder.h"
#include "TCanvas.h"
#include "TH3F.h"
#include "cDrawEvents.h"
#include "cTrackerRansac.h"

#ifdef __CLING__
#pragma link C++ class cTrackerRansac < cPhysicalHit> + ;
#endif

using namespace std;

int fit(string input_file = "input_parameters.txt")
{

    ifstream inFile(input_file);
    if (!inFile)
    {
        std::cout << "\nError opening file.\n";
        return 13;
    }

    string Var_Name, inp_n, data_file_name;
    double Var_Value;
    map<string, int> parMap;

    inFile >> Var_Name >> data_file_name;
    while (inFile >> Var_Name >> Var_Value)
    {
        parMap.insert(make_pair(Var_Name, Var_Value));
    }
    inFile.close();

    // Setting Hyperparameters
    gROOT->SetBatch(kFALSE);
    TString ifname = data_file_name;

    double threshold = parMap["tracksEnergyThreshold"];        // minimum energy requested for a single cluster.
    double fenergy = parMap["beamEnergyThreshold"];            // energy threshold for a track in order to be considered a beam track.
    double width = parMap["tracksWidth"];                      // maximum distance from model accepted in clustering
    double fwidth = parMap["beamWidth"];                       // maximum distance from model accepted in clustering for beam tracks
    double SqVertex_Width = parMap["squaredVertexAcceptance"]; // maximum distance accepted between two different cluster
    double loops = parMap["numberLoops"];                      // number of loops, i.e. number of random couples chosen.
    double trsize = parMap["trackMinSize"];                    // min number of pads required in order to consider a cluster a real track
    double besize = parMap["beamMinSize"];                     // min number of pads required in order to consider a cluster a real track FOR BEAM

    bool oneEventOnly = parMap["oneEventOnly"];
    int toAnalyse = parMap["toAnalyse"];
    int startFrom = parMap["startFrom"];

    double zRescaling = parMap["zRescaling"];

    // Drawing parameters
    int binX = parMap["binX"];
    int binY = parMap["binY"];
    int binZ = parMap["binZ"];
    float maxX = parMap["maxX"];
    float maxY = parMap["maxY"];
    float maxZ = parMap["maxZ"];

    // Opening the input file.
    TFile *ifile = new TFile(ifname.Data(), "READ");
    if (ifile->IsZombie())
    {
        printf("Unable to open inputfile: %s", ifname.Data());
        return -1;
    }

    gROOT->SetBatch(kFALSE);

    TTree *physicalEventTree = (TTree *)ifile->Get("physicalEventTree");
    if (physicalEventTree->IsZombie())
    {
        printf("Unable to open inputtree: ");
        return -1;
    }

    TFile fout("outputTRACKS.root", "RECREATE");
    if (fout.IsZombie())
    {
        std::cout << "Output file creation failed" << std::endl;
        return 0;
    }
    fout.cd();

    // Creating output tree
    TTree outTree("physicalEventTree", "physicalEventTree");
    cPhysicalEvent *phEvt = nullptr;
    outTree.Branch("event", &phEvt);
    outTree.SetDirectory(&fout);
    outTree.AutoSave();

    cFittedEvent<cPhysicalHit> *fitEvt = new cFittedEvent<cPhysicalHit>();

    TTreeReader rdr(physicalEventTree);
    TTreeReaderValue<cPhysicalEvent> event(rdr, "event"); // reading input file

    int nent = rdr.GetEntries(false);

    while (rdr.Next())
    {

        delete phEvt;
        phEvt = new cPhysicalEvent;

        if (rdr.GetCurrentEntry() % 100 == 0)
            cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;

        if ((!oneEventOnly || event->getEventNumber() == toAnalyse) && event->getEventNumber() > startFrom && rdr.GetCurrentEntry() < 15000)
        {
            delete fitEvt;
            fitEvt = new cFittedEvent<cPhysicalHit>();
            fitEvt->setEventNumber(event->getEventNumber());
            fitEvt->setTimestamp(event->getTimestamp());
            fitEvt->getLines().clear();

            list<cPhysicalHit> hitslist = event->getHits(); // picking up the list from the event and converting it into a vector.

            cTrackerRansac<cPhysicalHit> traC;

            traC.setTracksEnergyThreshold(threshold);
            traC.setBeamEnergyThreshold(fenergy);
            traC.setTracksWidth(width);
            traC.setBeamWidth(fwidth);
            traC.setLoopsNumber(SqVertex_Width);
            traC.setTrackMinSize(trsize);
            traC.setBeamMinSize(besize);

            for (auto &h : hitslist)
            {
                // if (h.getGlobalChannelId() >= 20000)
                // {
                //     fitEvt->getAncillaryHit().push_back(h);
                //     continue;
                // }

                // Take only the pad hits that are set as trackable
                if (!h.isTrackable() || h.GetEnergy() < 0)
                {
                    fitEvt->getUnfittedPoints().push_back(h);
                    continue;
                }

                // The remaining points are good to be tracked
                traC.addPoint(h);
            }

            bool beamFittable = false;
            bool beamOneSide = false;
            bool beamTracks = true;
            traC.Ransac(traC.beamEnergyThreshold, traC.beamMinSize, traC.beamWidth, beamTracks, beamOneSide, beamFittable);

            bool trackFittable = true;
            bool trackOneSide = false;
            beamTracks = false;
            traC.Ransac(traC.tracksEnergyThreshold, traC.beamMinSize, traC.tracksWidth, beamTracks, trackOneSide, trackFittable);

            // end "clustering"

            // Fit the lines
            traC.fitLines();

            for (auto &it_line : traC.lines)
            {
                fitEvt->getLines().push_back(it_line);
            }

            for (auto &it_point : traC.points)
            {
                fitEvt->getUnfittedPoints().push_back(it_point);
            }

            // Save the fitted lines and the unfitted points in the final event
            // cVertexFinder<cPhysicalHit> vrt;
            // vrt.setMinZ(0);
            // std::vector<double> pStart = {64., 64., 500. / (zRescaling * 2.)};
            // vrt.setParStart(pStart);
            // vrt.setMaxZ(500);
            // vrt.setMaxDist(sqrt(55.));
            // vrt.findVertex(fitEvt);

            // cDrawEvents<cFittedEvent<cPhysicalHit>> *drawEvt =
            //     new cDrawEvents<cFittedEvent<cPhysicalHit>>(binX, binY, binZ, maxX, maxY, maxZ);

            // drawEvt->setEvent(fitEvt);
            // drawEvt->drawColors2D(false);
            // drawEvt->drawTracks3D(true);

            // delete drawEvt;

            int num_tracks = 0;
            for (auto &it_lines : traC.lines)
            {
                if (it_lines.isFittable())
                {
                    num_tracks += 1;
                }
            }
            
            if (num_tracks >= 1)
            {
                phEvt->setEventNumber(event->getEventNumber());
                phEvt->setRunNumber(event->getRunNumber());

                phEvt->getHits().clear();
                for (auto &it_hits : event->getHits())
                    phEvt->getHits().push_back(it_hits);

                outTree.Fill();
            }
        }
        else
        {
            continue;
        }
    }

    fout.cd();
    outTree.Write("", TObject::kOverwrite);

    return 0;
}

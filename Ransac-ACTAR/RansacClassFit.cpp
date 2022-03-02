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
    map<string, double> parMap;

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

    cFittedEvent<cPhysicalHit> *fitEvt = new cFittedEvent<cPhysicalHit>();

    TTreeReader rdr(physicalEventTree);
    TTreeReaderValue<cPhysicalEvent> event(rdr, "event"); // reading input file

    int nent = rdr.GetEntries(false);

    while (rdr.Next())
    {

        auto start = std::chrono::steady_clock::now();

        cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;

        if ((!oneEventOnly || event->getEventNumber() == toAnalyse) && event->getEventNumber() > startFrom)
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
            traC.Ransac(fenergy, besize, fwidth, beamTracks, beamOneSide, beamFittable);

            bool trackFittable = true;
            bool trackOneSide = false;
            beamTracks = false;
            traC.Ransac(threshold, trsize, width, beamTracks, trackOneSide, trackFittable);

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
            cVertexFinder<cPhysicalHit> vrt;
            vrt.setMinZ(0);
            std::vector<double> pStart = {64., 64., 500. / (zRescaling * 2.)};
            vrt.setParStart(pStart);
            vrt.setMaxZ(500);
            vrt.setMaxDist(sqrt(55.));
            vrt.findVertex(fitEvt);

            auto end = std::chrono::steady_clock::now();

            cout << "Time FOR ONE RUN = " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << endl;

            cDrawEvents<cFittedEvent<cPhysicalHit>> *drawEvt =
                new cDrawEvents<cFittedEvent<cPhysicalHit>>(binX, binY, binZ, maxX, maxY, maxZ);

            drawEvt->setEvent(fitEvt);
            drawEvt->drawColors2D(false);
            drawEvt->drawTracks3D(true);

            delete drawEvt;
        }
        else
        {
            continue;
        }
    }

    return 0;
}

// arrayI2 GetSample(Int_t size)
// {
//     arrayI2 index = {0, 0};
//     while (index[0] == index[1])
//     {
//         index[0] = rnd->Uniform(0, size);
//         index[1] = rnd->Uniform(0, size);
//     }
//     return index;
// }

// arrayV2 GetParamWithSample(arrayD4 sample1, arrayD4 sample2)
// {
//     TVector3 p1(sample1[0], sample1[1], sample1[2]);
//     TVector3 p2(sample2[0], sample2[1], sample2[2]);
//     TVector3 u = p2 - p1;
//     arrayV2 r = {p1, u.Unit()};
//     return r;
// }

// Double_t GetError_function(arrayV2 model, arrayD4 p, double zScale)
// {
//     TVector3 a = {p[0] - model[0][0], p[1] - model[0][1], p[2] - model[0][2]};
//     TVector3 H = model[0] + a.Dot(model[1]) * model[1];
//     TVector3 d = {p[0] - H[0], p[1] - H[1], (p[2] - H[2]) / zScale}; // it was necessary to put this factor 10 due to measurement units (as I have set the precalibrator).

//     return d.Mag2();
// }

// double ClusterTest(double &sumvalue, double &totalenergy, vector<int> &inliers)
// {
//     double test = sumvalue / (totalenergy * inliers.size());
//     return test;
// }

// extern void FunctionForTrack(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
// {
//     // Since arguments are defined by the MINUIT algorith, nobody can change them.
//     arrayV2 minuitmodel;
//     minuitmodel[0] = {par[0], par[1], par[2]};
//     minuitmodel[1] = {par[3], par[4], par[5]};
//     Double_t err = 0;
//     Double_t chisq = 0;

//     for (unsigned int i = 0; i < indextofit.size(); i++)
//     {
//         pointstofit = {indextofit[i].getX(), indextofit[i].getY(), indextofit[i].getZ(), energyWeightInFitting * indextofit[i].getEnergy()};
//         chisq += GetError_function(minuitmodel, pointstofit, zRescalingFit);
//     }
//     f = chisq;
// }

// arrayV2 MinuitForTrack(cFittedLine<cPhysicalHit> &track)
// {
//     // Initialization
//     indextofit.clear();
//     indextofit = vector<cPhysicalHit>(track.getPoints().begin(), track.getPoints().end());
//     arrayV2 outvectors;

//     // Valiables
//     Double_t Pos[3] = {0};
//     Double_t PosErr[3] = {0};
//     Double_t Dir[3] = {0};
//     Double_t DirErr[3] = {0};
//     Double_t delta = 10.;

//     // Minuit
//     TMinuit *gMinuit = new TMinuit(6);
//     gMinuit->SetPrintLevel(-1); //-1:quiet, 0:normal, 1:verbose
//     gMinuit->SetErrorDef(1.);

//     // Set Paramters
//     // double_t
//     // Double_t maxX = numx;
//     // Double_t maxY = numy;
//     // Double_t maxZ = numz;
//     gMinuit->DefineParameter(0, "Ax", 0, 0.01, minMinuitVal, numx);
//     gMinuit->DefineParameter(1, "Ay", 64, 10, minMinuitVal, numy);
//     gMinuit->DefineParameter(2, "Az", 300, 50, minMinuitVal, numz);
//     gMinuit->DefineParameter(3, "Bx", 0., 1., -100, 100);
//     gMinuit->DefineParameter(4, "By", 0.01, 1, -100, 100);
//     gMinuit->DefineParameter(5, "Bz", 0., 30., -100, 100);

//     // Calculation MINUIT using Migrad algorithm
//     gMinuit->SetFCN(FunctionForTrack);
//     gMinuit->Migrad();

//     // Get Results
//     gMinuit->GetParameter(0, Pos[0], PosErr[0]);
//     gMinuit->GetParameter(1, Pos[1], PosErr[1]);
//     gMinuit->GetParameter(2, Pos[2], PosErr[2]);
//     gMinuit->GetParameter(3, Dir[0], DirErr[0]);
//     gMinuit->GetParameter(4, Dir[1], DirErr[1]);
//     gMinuit->GetParameter(5, Dir[2], DirErr[2]);

//     TVector3 Position(Pos[0], Pos[1], Pos[2]);
//     TVector3 PositionErr(PosErr[0], PosErr[1], PosErr[2]);
//     TVector3 Direction(Dir[0], Dir[1], Dir[2]);
//     TVector3 DirectionErr(DirErr[0], DirErr[1], DirErr[2]);

//     outvectors[0] = Position;
//     outvectors[1] = Direction;

//     delete gMinuit;

//     return outvectors;
// }

// // end track

// // begin Vertex

// void FunctionForVertex(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
// {
//     // Since arguments are defined by the MINUIT algorith, nobody can change them.
//     arrayD4 p = {par[0], par[1], par[2], 1.};
//     arrayV2 model;
//     Double_t err = 0;
//     Double_t chisq = 0;

//     for (auto &i : eTrack.getLines())
//     {
//         model[0] = i.getBasepoint();
//         model[1] = i.getDirection();
//         err = GetError_function(model, p, zRescaling); // si può pesare questo test con il chiquadro di ogni traccia: più una traccia è precisa, minore sarà il suo contributo all'errore finale. /GlobalTrack.GetChi2(i);
//         chisq += err;
//     }

//     f = chisq;
// }

// TVector3 MinuitForVertex(cFittedEvent<cPhysicalHit> &fitEvt, vector<int> besttracks)
// {
//     // Initialization
//     eTrack.getLines().clear();
//     for (auto &i : besttracks)
//     {
//         list<cFittedLine<cPhysicalHit>>::iterator iterTrack = fitEvt.getLines().begin();
//         advance(iterTrack, i);
//         if (!iterTrack->isFittable())
//         {
//             continue;
//         }
//         eTrack.getLines().push_back(*iterTrack);
//     }

//     // Valiables
//     Double_t Vertex[3] = {0};
//     Double_t Vertex_err[3] = {0};
//     Double_t delta = 100.;

//     // Minuit
//     TMinuit *gMinuit = new TMinuit(3);
//     gMinuit->SetPrintLevel(-1); //-1:quiet, 0:normal, 1:verbose
//     gMinuit->SetErrorDef(1.);

//     // Set Paramters
//     Double_t maxX = numx;
//     Double_t maxY = numy;
//     Double_t maxZ = numz;
//     gMinuit->DefineParameter(0, "Vx", 0, 0.01, -delta, maxX + delta);
//     gMinuit->DefineParameter(1, "Vy", maxY / 2., 0.01, -delta, maxY + delta);
//     gMinuit->DefineParameter(2, "Vz", maxZ / 2., 0.01, -delta, maxZ + delta);

//     // Calculation MINUIT using Migrad algorithm
//     gMinuit->SetFCN(FunctionForVertex);
//     gMinuit->Migrad();

//     // Get Results
//     gMinuit->GetParameter(0, Vertex[0], Vertex_err[0]);
//     gMinuit->GetParameter(1, Vertex[1], Vertex_err[1]);
//     gMinuit->GetParameter(2, Vertex[2], Vertex_err[2]);

//     TVector3 ver(Vertex[0], Vertex[1], Vertex[2]);

//     delete gMinuit;

//     return ver;
// }

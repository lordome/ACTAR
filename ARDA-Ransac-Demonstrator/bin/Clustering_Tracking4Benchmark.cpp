#include "TFile.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TMinuit.h"
#include "TEnv.h"
#include "TTree.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TTreeReader.h"
#include "TRandom3.h"
#include "TSystem.h"
#include "TROOT.h"

#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <list>
#include <array>
#include <time.h>

#include "cPhysicalEvent.h"
#include "cFittedLine.h"
#include "cFittedEvent.h"

using namespace std;

typedef array<Int_t, 2> arrayI2;
typedef array<Double_t, 3> arrayD3;
typedef array<Double_t, 4> arrayD4;
typedef array<TVector3, 2> arrayV2;

TVector3 zero(0, 0, 0); // zero vector
arrayD4 pointstofit;
cFittedEvent<int> eTrack;
vector<int> indextofit;
vector<cPhysicalHit> hits;

TRandom3 *rnd = new TRandom3;

//Parameters for MINUIT
int numx = 128;
int numy = 64;
double numz = 100; //numz depends on time and drift speed. (100: default value)

arrayI2 GetSample(Int_t size);                                // Two points select randomly
arrayV2 GetParamWithSample(arrayD4 sample1, arrayD4 sample2); // Calculation of 3D line from two vectors
Double_t GetError(arrayV2 model, arrayD4 p);                  // SQUARE! of the distance between the point and the line

//for tracking
extern void FunctionForTrack(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag); // MINUIT function for track calculation
arrayV2 MinuitForTrack(cFittedLine<int> &track);                                                   // MINUIT algorithm for track

// for vertex
extern void FunctionForVertex(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag); // MINUIT function for vertex calculation
TVector3 MinuitForVertex(cFittedEvent<int> &tracker, vector<int> besttracks);                       // MINUIT algorithm for vertex

double ClusterTest(double &sumvalue, double &totalenergy, vector<int> &inliers); //function used to test the clusters.
void DrawEvent(cFittedEvent<int> *tracker);
void DrawOscar(list<cPhysicalHit> &hitslist);

bool InOscar(int gid)
{

    if ((gid > 21000) && (gid < 21234) && (gid != 21011) && (gid != 21022) && (gid != 21045) && (gid != 21056) && (gid != 21111) && (gid != 21122) && (gid != 21145) && (gid != 21156) && (gid != 21231) /*
    && ( gid != 21211 ) && ( gid != 21222 ) */
    )
    {
        return true;
    }
    else
    {
        return false;
    }
    /*if( ( gid>21034 && gid<21045 ) || ( gid>21045 && gid<21056 ) || ( gid>21056 && gid<21068 ) || ( gid>=21134 && gid<21145 ) || ( gid>21145 && gid<21156) || ( gid>21156 && gid<21168) || ( gid>=21200 && gid<21211) || ( gid>=21212 && gid<21217) ){
    return true;

  }else{
  return false;
}*/
};

TVector3 VecFascio(1, 0, 0); //unit vector defining beam direction.

int Ransac_Analyze(string input_file = "")
{

    ifstream inFile(input_file);

    string Var_Name;
    double Var_Value;
    string inp_n;
    string data_file_name;
    map<string, int> mymap;

    if (!inFile)
    {
        std::cout << "\nError opening file.\n";
        return 13;
    }

    inFile >> inp_n >> data_file_name;

    TString ifname = data_file_name;

    while (inFile >> Var_Name >> Var_Value)
    {
        mymap.insert(make_pair(Var_Name, Var_Value));
    }

    // for (auto it = mymap.cbegin(); it != mymap.cend(); ++it)
    // {
    //     std::cout << it->first << " " << it->second << "\n";
    // }
    // std::cout << inp_n << " " << data_file_name << endl;

    inFile.close();
    //Setting Hyperparameters
    gROOT->SetBatch(kFALSE);

    //int toanalyse=1518; //Proviamo a semplificare per un primo momento. Questo ci dice il numero di evento da analizzare.
    //cout << "Set event num:" ;
    //cin >> toanalyse;
    double threshold = mymap["threshold"];           // minimum energy requested for a single cluster.
    double fenergy = mymap["fenergy"];               //energy threshold for a track in order to be considered a beam track.
    double width = mymap["width"];                   // maximum distance from model accepted in clustering
    double fwidth = mymap["fwidth"];                 // maximum distance from model accepted in clustering for beam tracks
    double SqVertex_Width = mymap["SqVertex_Width"]; //maximum distance accepted between two different cluster
    double loops = mymap["loops"];                   // number of loops, i.e. number of random couples chosen.
    double trsize = mymap["trsize"];                 //min number of pads required in order to consider a cluster a real track
    double besize = mymap["besize"];                 //min number of pads required in order to consider a cluster a real track FOR BEAM

    // end Setting Hyperparameters

    //Opening the input file.
    TFile *ifile = new TFile(ifname.Data(), "READ");
    if (ifile->IsZombie())
    {
        printf("Unable to open inputfile: %s", ifname.Data());
        return -1;
    }
    TTree *physicalEventTree = (TTree *)ifile->Get("physicalEventTree");
    if (physicalEventTree->IsZombie())
    {
        printf("Unable to open inputtree: ");
        return -1;
    }

    // Open output file
    TString filename = ifname;
    filename = "PROVA_411_Analysis.root";
    TFile fout(filename.Data(), "UPDATE");
    if (fout.IsZombie())
    {
        cout << "Output file creation failed" << endl;
        return 0;
    }
    fout.cd();

    cFittedEvent<int> *tracker = nullptr;
    cFittedEvent<cPhysicalHit> *tracker_withHits = nullptr;

    TTree *fOutTree = new TTree("trackTree", "trackTree");
    fOutTree->Branch("Full_event", &tracker_withHits);
    // double EnergyMaxOscar;
    // fOutTree->Branch("Oscar_Max_Energy", &EnergyMaxOscar);
    fOutTree->SetDirectory(&fout);
    fOutTree->AutoSave();

    TTreeReader rdr(physicalEventTree);
    TTreeReaderValue<cPhysicalEvent> event(rdr, "event"); //reading input file

    int rndpoint1 = 0; //points index into the hits vector
    int rndpoint2 = 0;
    int iterations = 0; //number of iterations
    arrayD4 sample1 = {0, 0, 0, 0};
    arrayD4 sample2 = {0, 0, 0, 0};
    arrayV2 maybemodel = {zero, zero};
    arrayV2 bestmodel = {zero, zero};
    vector<int> alsoinliers; //vector to be filled with the possible indexes.
    vector<int> bestinliers; //vector to be filled with the best indexes.
    cFittedLine<int> besttrack;
    Double_t besterr = DBL_MAX;
    Double_t thiserr = 0;
    vector<int> unfittedPoints; //vector that contains every index.

    vector<double> energie;
    double trackenergy = 0;

    int nent = rdr.GetEntries(false);
    double trackcount = 0;
    int beamtrackcount = 0;
    energie.clear();

    while (rdr.Next())
    {

        energie.clear();

        list<cPhysicalHit> hitslist = event->getHits(); //picking up the list from the event and converting it into a vector.
                                                        // Used basically in order to access directly the points inside the list.
        hits = vector<cPhysicalHit>(hitslist.begin(), hitslist.end());

        delete tracker;
        delete tracker_withHits;
        tracker = new cFittedEvent<int>();
        tracker_withHits = new cFittedEvent<cPhysicalHit>;
        tracker->getLines().clear();
        tracker_withHits->getLines().clear();

        // double OsEnmax = 0;
        // double OsGidmax = 0;
        // for (auto &i : hits)
        // {
        //     if (InOscar(i.getGlobalChannelId()))
        //     {
        //         tracker_withHits->getAncillaryHit().push_back(i);
        //         if (i.getEnergy() > OsEnmax)
        //         {
        //             OsEnmax = i.getEnergy();
        //             OsGidmax = i.getGlobalChannelId();
        //         }
        //     }
        // }
        // EnergyMaxOscar = OsEnmax;

        unfittedPoints.clear();

        for (unsigned int i = 0; i < hits.size(); i++)
        {
            unfittedPoints.push_back(i);
        } //it's a vector containing indexes. The ones that will be included in a cluster will be ==-1.

        cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;

        tracker->setEventNumber(event->getEventNumber());
        tracker_withHits->setEventNumber(event->getEventNumber());
        //begin clustering

        //!!!!! The while loop has an always true condition, in order not to have an upper limit of tracks.
        //This loop (in both beam and non-beam tracks) stops when the track dimension is == 0 after a whole cycle.
        beamtrackcount = 0;

        while (1)
        { //There are particular conditions, the points chosen randomly must have same y & z, the energy threshold is very high, the number of pads required has to be high.

            iterations = 0;
            besterr = DBL_MAX;
            besttrack.getPoints().clear();
            bestinliers.clear();

            while (iterations < loops)
            {

                rndpoint1 = rnd->Uniform(0, hits.size());
                rndpoint2 = rnd->Uniform(0, hits.size());

                while (!hits[rndpoint1].isTrackable() || unfittedPoints[rndpoint1] == -1)
                {                                             //isTrackable lets us select some hits that are not useful for tracking (ancillaryHit, noisy pad.)
                    rndpoint1 = rnd->Uniform(0, hits.size()); //isTrackable is selected into precalibrator.cc
                };
                while (!hits[rndpoint2].isTrackable() || unfittedPoints[rndpoint2] == -1)
                {
                    rndpoint2 = rnd->Uniform(0, hits.size());
                };

                if (hits[rndpoint1].getZ() != hits[rndpoint2].getZ() && hits[rndpoint1].getY() != hits[rndpoint2].getY())
                {
                    iterations++;
                    continue;
                }; // testing if y1==y2 && z1==z2

                maybemodel[0].SetXYZ(0, 0, 0);
                maybemodel[1].SetXYZ(0, 0, 0);
                alsoinliers.clear();

                sample1 = {hits[rndpoint1].getX(), hits[rndpoint1].getY(), hits[rndpoint1].getZ(), hits[rndpoint1].getEnergy()};
                sample2 = {hits[rndpoint2].getX(), hits[rndpoint2].getY(), hits[rndpoint2].getZ(), hits[rndpoint2].getEnergy()};

                maybemodel = GetParamWithSample(sample1, sample2); //building the 3D line between the random points.

                double GetErrorMean = 0;
                double sumvalue = 0;
                double totalenergy = 0;

                // For every point in data, if point fits maybemodel with an error(squared) smaller than width,
                // add its index to alsoinliers, compute totalenergy and compute sumvalue (used to test the inliers.)

                for (unsigned int i = 0; i < unfittedPoints.size(); i++)
                {

                    if (unfittedPoints[i] == -1)
                    {
                        continue;
                    }

                    arrayD4 samplex = {hits[i].getX(), hits[i].getY(), hits[i].getZ(), hits[i].getEnergy()};

                    if (GetError(maybemodel, samplex) < pow(fwidth, 2) && hits[i].isTrackable())
                    {

                        alsoinliers.push_back(i);
                        sumvalue += GetError(maybemodel, samplex) * hits[i].getEnergy();
                        totalenergy += hits[i].getEnergy();

                    } //end if (GetError(maybemodel,samplex)<pow(fwidth,2) && hits[i].isTrackable())

                } // end for (int i=0;i<unfittedPoints.size();i++)

                GetErrorMean = ClusterTest(sumvalue, totalenergy, alsoinliers); //function that evaluates the cluster(it can be changed)

                if (alsoinliers.size() > besize && GetErrorMean < besterr && totalenergy > fenergy)
                { // comparing the cluster to the previous best one.
                    // if the current cluster is better than the previous best, it simply becomes the new best.
                    bestmodel = maybemodel;
                    besterr = GetErrorMean;
                    bestinliers = alsoinliers;

                } // end if (alsoinliers.size()>20 && GetErrorMean<besterr && totalenergy>fenergy)
                iterations++;
            } //end while (iterations<loops)

            //cout << bestinliers.size() << "fsize" << endl; //outputs the size of the best cluster found: it's a useful and simple check.

            if (bestinliers.empty())
            {
                break;
            } //endind the cycle whether the best tracks has dimension == 0.

            for (unsigned int i = 0; i < bestinliers.size(); i++)
            {
                unfittedPoints[bestinliers[i]] = -1;
            } //setting all the used points to -1, so they won't be used anymore.

            besttrack.getPoints().insert(besttrack.getPoints().begin(), bestinliers.begin(), bestinliers.end()); //setting besttrack (conversion from vector to list)
            besttrack.setFittable(false);
            tracker->getLines().push_back(besttrack); //saving besttrack into tracker, which is going to be saved into the output tree.

            beamtrackcount++;
        } // end while (1){

        trackcount = 0;
        while (1)
        { //looking for normal tracks. The method used is exactly the previous one.

            besterr = DBL_MAX;
            iterations = 0;
            bestinliers.clear();
            besttrack.getPoints().clear();
            trackenergy = 0;

            while (iterations < loops)
            {

                rndpoint1 = rnd->Uniform(0, hits.size());
                rndpoint2 = rnd->Uniform(0, hits.size());

                while (!hits[rndpoint1].isTrackable() || unfittedPoints[rndpoint1] == -1)
                {
                    rndpoint1 = rnd->Uniform(0, hits.size());
                };
                while (!hits[rndpoint2].isTrackable() || unfittedPoints[rndpoint2] == -1)
                {
                    rndpoint2 = rnd->Uniform(0, hits.size());
                };

                maybemodel[0].SetXYZ(0, 0, 0);
                maybemodel[1].SetXYZ(0, 0, 0);
                alsoinliers.clear();

                sample1 = {hits[rndpoint1].getX(), hits[rndpoint1].getY(), hits[rndpoint1].getZ(), hits[rndpoint1].getEnergy()}; // Per semplicità, togliamo la terza dimensione: utilizziamo solamente x,y,E. Sarà da riaggiungere.
                sample2 = {hits[rndpoint2].getX(), hits[rndpoint2].getY(), hits[rndpoint2].getZ(), hits[rndpoint2].getEnergy()};

                maybemodel = GetParamWithSample(sample1, sample2);

                double GetErrorMean = 0;

                double sumvalue = 0;
                double totalenergy = 0;

                // For every point in data,
                // if point fits maybemodel with an error smaller than WIDTH,
                // add point to alsoinliers, compute totalenergy and compute sumvalue (it's used to test the inliers.)
                for (unsigned int i = 0; i < unfittedPoints.size(); i++)
                {
                    if (unfittedPoints[i] == -1)
                    {
                        continue;
                    }

                    arrayD4 samplex = {hits[i].getX(), hits[i].getY(), hits[i].getZ(), hits[i].getEnergy()};

                    if (GetError(maybemodel, samplex) < pow(width, 2) && (hits[i].isTrackable() || hits[i].getY() > 40))
                    {
                        /*if(hits[i].getY()<4 || hits[i].getY()>60 || hits[i].getX()>120 || (hits[i].getX()<64 && hits[i].getX()>32 && hits[i].getY()>52) ){
            gone=true;
          }*/
                        alsoinliers.push_back(i);
                        arrayD4 samplex = {hits[i].getX(), hits[i].getY(), hits[i].getZ(), hits[i].getEnergy()};
                        sumvalue += GetError(maybemodel, samplex) * hits[i].getEnergy();
                        totalenergy += hits[i].getEnergy();

                    } //end if ( GetError(maybemodel,samplex)<width*width && hits[i].isFittable() && i!=-1 )
                }     // end for (int i=0;i<unfittedPoints.size();i++)

                GetErrorMean = sumvalue / (totalenergy * alsoinliers.size()); //è una buona scelta

                if (alsoinliers.size() > trsize && GetErrorMean < besterr && totalenergy > threshold)
                {
                    bestmodel = maybemodel;
                    besterr = GetErrorMean;
                    bestinliers = alsoinliers;
                    trackenergy = totalenergy;
                } // end if (alsoinliers.size()>20 && GetErrorMean<besterr)

                iterations++;
            } //end while (iterations<loops)
            //cout << bestinliers.size() << "size" << endl;

            if (bestinliers.empty())
            {
                break;
            } //se non ho trovato tracce buone, fermati.

            double media = 0;
            double count = 0;
            for (unsigned int i = 0; i < bestinliers.size(); i++)
            {
                media += hits[bestinliers[i]].getY();
            }

            media /= bestinliers.size();

            vector<int> temporary;

            if (media > 34)
            {
                for (unsigned int i = 0; i < bestinliers.size(); i++)
                {
                    if (hits[bestinliers[i]].getY() > 33.9)
                        temporary.push_back(bestinliers[i]);
                }
                bestinliers = temporary;
            }

            if (media < 30)
            {
                for (unsigned int i = 0; i < bestinliers.size(); i++)
                {
                    if (hits[bestinliers[i]].getY() < 30.1)
                        temporary.push_back(bestinliers[i]);
                }
                bestinliers = temporary;
            }

            for (unsigned int i = 0; i < bestinliers.size(); i++)
            {
                unfittedPoints[bestinliers[i]] = -1;
            } //setto i punti clusterizzati a -1.

            if (trackenergy != 0)
            {
                energie.push_back(trackenergy / bestinliers.size());
            }
            besttrack.getPoints().insert(besttrack.getPoints().begin(), bestinliers.begin(), bestinliers.end());
            besttrack.setFittable(true);
            tracker->getLines().push_back(besttrack);

            trackcount++;
        }
        // end "clustering"

        vector<cFittedLine<int>> vectracks(tracker->getLines().begin(), tracker->getLines().end()); //converting a list to a vector;

        //begin tracking
        for (unsigned int i = 0; i < vectracks.size(); i++)
        {
            indextofit.clear();
            if (!vectracks[i].isFittable())
            {
                continue;
            } //tracking only non-beam clusters.
            arrayV2 outV;
            outV = MinuitForTrack(vectracks[i]);

            vectracks[i].setBasepoint(outV[0]);
            vectracks[i].setDirection(outV[1]);
        }

        list<cFittedLine<int>> topush;
        topush.insert(topush.begin(), vectracks.begin(), vectracks.end()); //converting a vector to a list.
        tracker->getLines().clear();
        tracker->setLines(topush);
        topush.clear();

        for (auto &i : vectracks)
        {
            cFittedLine<cPhysicalHit> besttrack_withHits(i.getDirection(), i.getBasepoint());
            for (auto &j : i.getPoints())
            {
                besttrack_withHits.getPoints().push_back(hits[j]);
            }
            tracker_withHits->getLines().push_back(besttrack_withHits);
        }

        // end tracking

        fOutTree->Fill();
    }

    fOutTree->Write();

    return 0;
}

arrayI2 GetSample(Int_t size)
{
    arrayI2 index = {0, 0};
    while (index[0] == index[1])
    {
        index[0] = rnd->Uniform(0, size);
        index[1] = rnd->Uniform(0, size);
    }
    return index;
}

arrayV2 GetParamWithSample(arrayD4 sample1, arrayD4 sample2)
{
    TVector3 p1(sample1[0], sample1[1], sample1[2]);
    TVector3 p2(sample2[0], sample2[1], sample2[2]);
    TVector3 u = p2 - p1;
    arrayV2 r = {p1, u.Unit()};
    return r;
}

Double_t GetError(arrayV2 model, arrayD4 p)
{
    TVector3 a = {p[0] - model[0][0], p[1] - model[0][1], p[2] - model[0][2]};
    TVector3 H = model[0] + a.Dot(model[1]) * model[1];
    TVector3 d = {p[0] - H[0], p[1] - H[1], (p[2] - H[2]) * 10}; //it was necessary to put this factor 10 due to measurement units (as I have set the precalibrator).

    return d.Mag2();
}

double ClusterTest(double &sumvalue, double &totalenergy, vector<int> &inliers)
{
    double test = sumvalue / (totalenergy * inliers.size());
    return test;
}

extern void FunctionForTrack(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
    // Since arguments are defined by the MINUIT algorith, nobody can change them.
    arrayV2 minuitmodel;
    minuitmodel[0] = {par[0], par[1], par[2]};
    minuitmodel[1] = {par[3], par[4], par[5]};
    Double_t err = 0;
    Double_t chisq = 0;

    for (unsigned int i = 0; i < indextofit.size(); i++)
    {
        pointstofit = {hits[indextofit[i]].getX(), hits[indextofit[i]].getY(), hits[indextofit[i]].getZ(), hits[indextofit[i]].getEnergy()};
        err = GetError(minuitmodel, pointstofit);
        chisq += err;
    }
    f = chisq;
}

arrayV2 MinuitForTrack(cFittedLine<int> &track)
{
    // Initialization
    indextofit.clear();
    indextofit = vector<int>(track.getPoints().begin(), track.getPoints().end());
    arrayV2 outvectors;

    // Valiables
    Double_t Pos[3] = {0};
    Double_t PosErr[3] = {0};
    Double_t Dir[3] = {0};
    Double_t DirErr[3] = {0};
    Double_t delta = 10.;

    // Minuit
    TMinuit *gMinuit = new TMinuit(6);
    gMinuit->SetPrintLevel(-1); //-1:quiet, 0:normal, 1:verbose
    gMinuit->SetErrorDef(1.);

    // Set Paramters
    Double_t maxX = numx;
    Double_t maxY = numy;
    Double_t maxZ = numz;
    gMinuit->DefineParameter(0, "Ax", 0, 0.01, -100, 228);
    gMinuit->DefineParameter(1, "Ay", 32, 0.01, -100, 164);
    gMinuit->DefineParameter(2, "Az", 8, 0.01, -100, 117);
    gMinuit->DefineParameter(3, "Bx", 0., 0.01, -100, 100);
    gMinuit->DefineParameter(4, "By", 0.01, 0.001, -100, 100);
    gMinuit->DefineParameter(5, "Bz", 0., 0.01, -100, 100);

    // Calculation MINUIT using Migrad algorithm
    gMinuit->SetFCN(FunctionForTrack);
    gMinuit->Migrad();

    // Get Results
    gMinuit->GetParameter(0, Pos[0], PosErr[0]);
    gMinuit->GetParameter(1, Pos[1], PosErr[1]);
    gMinuit->GetParameter(2, Pos[2], PosErr[2]);
    gMinuit->GetParameter(3, Dir[0], DirErr[0]);
    gMinuit->GetParameter(4, Dir[1], DirErr[1]);
    gMinuit->GetParameter(5, Dir[2], DirErr[2]);

    TVector3 Position(Pos[0], Pos[1], Pos[2]);
    TVector3 PositionErr(PosErr[0], PosErr[1], PosErr[2]);
    TVector3 Direction(Dir[0], Dir[1], Dir[2]);
    TVector3 DirectionErr(DirErr[0], DirErr[1], DirErr[2]);

    outvectors[0] = Position;
    outvectors[1] = Direction;

    delete gMinuit;

    return outvectors;
}

//end track

//begin Vertex

void FunctionForVertex(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
    // Since arguments are defined by the MINUIT algorith, nobody can change them.
    arrayD4 p = {par[0], par[1], par[2], 1.};
    arrayV2 model;
    Double_t err = 0;
    Double_t chisq = 0;

    for (auto &i : eTrack.getLines())
    {
        model[0] = i.getBasepoint();
        model[1] = i.getDirection();
        err = GetError(model, p); // si può pesare questo test con il chiquadro di ogni traccia: più una traccia è precisa, minore sarà il suo contributo all'errore finale. /GlobalTrack.GetChi2(i);
        chisq += err;
    }

    f = chisq;
}

TVector3 MinuitForVertex(cFittedEvent<int> &tracker, vector<int> besttracks)
{
    // Initialization
    eTrack.getLines().clear();
    for (auto &i : besttracks)
    {
        list<cFittedLine<int>>::iterator iterTrack = tracker.getLines().begin();
        advance(iterTrack, i);
        if (!iterTrack->isFittable())
        {
            continue;
        }
        eTrack.getLines().push_back(*iterTrack);
    }

    // Valiables
    Double_t Vertex[3] = {0};
    Double_t Vertex_err[3] = {0};
    Double_t delta = 100.;

    // Minuit
    TMinuit *gMinuit = new TMinuit(3);
    gMinuit->SetPrintLevel(-1); //-1:quiet, 0:normal, 1:verbose
    gMinuit->SetErrorDef(1.);

    // Set Paramters
    Double_t maxX = numx;
    Double_t maxY = numy;
    Double_t maxZ = numz;
    gMinuit->DefineParameter(0, "Vx", 0, 0.01, -delta, maxX + delta);
    gMinuit->DefineParameter(1, "Vy", maxY / 2., 0.01, -delta, maxY + delta);
    gMinuit->DefineParameter(2, "Vz", maxZ / 2., 0.01, -delta, maxZ + delta);

    // Calculation MINUIT using Migrad algorithm
    gMinuit->SetFCN(FunctionForVertex);
    gMinuit->Migrad();

    // Get Results
    gMinuit->GetParameter(0, Vertex[0], Vertex_err[0]);
    gMinuit->GetParameter(1, Vertex[1], Vertex_err[1]);
    gMinuit->GetParameter(2, Vertex[2], Vertex_err[2]);

    TVector3 ver(Vertex[0], Vertex[1], Vertex[2]);

    delete gMinuit;

    return ver;
}

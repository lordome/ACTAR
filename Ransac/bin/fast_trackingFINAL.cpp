#define fast_trackingFINAL_cxx
// The class definition in fast_trackingFINAL.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("fast_trackingFINAL.C")
// root> T->Process("fast_trackingFINAL.C","some options")
// root> T->Process("fast_trackingFINAL.C+")
//
#include <array>
#include "trackingFINAL.h"
#include "TFile.h"
#include <TH2.h>
#include <TStyle.h>
#include <TMath.h>
#include <TVector3.h>
#include <TSystem.h>
#include "TRandom3.h"
#include "TMinuit.h"
#include "cPhysicalEvent.h"
#include "cPhysicalHit.h"
#include "cFittedEvent.h"
#include <fstream>
#include "cLine3DFCN.h"
#include <Math/Functor.h>
#include <Fit/Fitter.h>
//#include "LinkDef.h"
using namespace std;

fast_trackingFINAL::~fast_trackingFINAL()
{
  if (fFile)
  {
    SafeDelete(fOutTree);
    SafeDelete(fFile);
  }
}

typedef array<Int_t, 2> arrayI2;
typedef array<Double_t, 3> arrayD3;
typedef array<Double_t, 4> arrayD4;
typedef array<TVector3, 2> arrayV2;

TVector3 zero(0, 0, 0);
TRandom3 *rnd = new TRandom3;
arrayD4 pointstofit;
cFittedEvent<int> eTrack;
vector<int> indextofit;
vector<cPhysicalHit> hits;

//Parameters for MINUIT
int numx = 128;
int numy = 64;
double numz = 100;

//Variables for RANSAC
double threshold = 4000;    // minimum energy requested for a single cluster.
double fenergy = 40000;     //energy threshold for a track in order to be considered a beam track.
double width = 4;           // maximum distance from model accepted in clustering
double fwidth = 3.5;        // maximum distance from model accepted in clustering for beam tracks
double SqVertex_Width = 55; //maximum distance accepted between two different cluster
double loops = 1000;        // number of loops, i.e. number of random couples chosen.
double trsize = 18;         //min number of pads required in order to consider a cluster a real track
double besize = 60;         //min number of pads required in order to consider a cluster a real track FOR BEAM

//#include "cRansacTracker.h"

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

bool InOscar(int gid)
{

  if ((gid > 21000) && (gid < 21234) && (gid != 21011) && (gid != 21022) && (gid != 21045) && (gid != 21056) && (gid != 21111) && (gid != 21122) && (gid != 21145) && (gid != 21156) && (gid != 21211) && (gid != 21222) && (gid != 21231))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void fast_trackingFINAL::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();
}

void fast_trackingFINAL::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

  fProofFile = new TProofOutputFile("finaltry.root", "M");
  fProofFile->SetTitle("finaltry.root"); // File name

  fFile = fProofFile->OpenFile("RECREATE");
  if (fFile && fFile->IsZombie())
    SafeDelete(fFile);

  // Cannot continue
  if (!fFile)
  {
    Info("SlaveBegin", "could not create '%s': instance is invalid!", fProofFile->GetName());
    return;
  }

  fOutTree = new TTree("trackTree", "trackTree");
  fOutTree->Branch("Full_event", &tracker_withHits);
  double EnergyMaxOscar;
  fOutTree->Branch("Oscar_Max_Energy", &EnergyMaxOscar);
  fOutTree->SetDirectory(fFile);
  fOutTree->AutoSave();
}

Bool_t fast_trackingFINAL::Process(Long64_t entry)
{
  // The Process() function is called for each entry in the tree (or possibly
  // keyed object in the case of PROOF) to be processed. The entry argument
  // specifies which entry in the currently loaded tree is to be processed.
  // When processing keyed objects with PROOF, the object is already loaded
  // and is available via the fObject pointer.
  //
  // This function should contain the \"body\" of the analysis. It can contain
  // simple or elaborate selection criteria, run algorithms on the data
  // of the event and typically fill histograms.
  //
  // The processing can be stopped by calling Abort().
  //
  // Use fStatus to set the return value of TTree::Process().
  //
  // The return value is currently not used.

  fReader.SetEntry(entry);

  list<cPhysicalHit> hitslist = event->getHits(); //picking up the list from the event and converting it into a vector.
  hits = vector<cPhysicalHit>(hitslist.begin(), hitslist.end());

  double OsEnmax = 0;
  double OsGidmax = 0;
  for (auto &i : hits)
  {
    if (InOscar(i.getGlobalChannelId()) && i.getEnergy() > OsEnmax)
    {
      OsEnmax = i.getEnergy();
      OsGidmax = i.getGlobalChannelId();
    }
  }
  if (OsEnmax > 350)
  {
    delete tracker;
    delete tracker_withHits;
    tracker = new cFittedEvent<int>();
    tracker_withHits = new cFittedEvent<cPhysicalHit>;
    tracker->getLines().clear();
    tracker_withHits->getLines().clear();

    tracker->setEventNumber(event->getEventNumber());
    tracker_withHits->setEventNumber(event->getEventNumber());

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
    energie.clear();
    double trackenergy = 0;
    TVector3 vertex_asTVector;
    double trackcount = 0;
    int beamtrackcount = 0;
    bool gone = false;
    bool bestgone = false;

    unfittedPoints.clear();
    unfittedPoints.reserve(hits.size());
    for (unsigned int i = 0; i < hits.size(); i++)
    {
      unfittedPoints.push_back(i);
    } //it's a vector containing indexes. The ones that will be included in a cluster will be ==-1.

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
        {                                           //isTrackable lets us select some hits that are not useful for tracking (ancillaryHit, noisy pad.)
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
        }   // end for (int i=0;i<unfittedPoints.size();i++)

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
    } // end while (1)

    // end "clustering"

    vector<cFittedLine<int>> vectracks(tracker->getLines().begin(), tracker->getLines().end()); //converting a list to a vector;

    for (auto &i : vectracks)
    {
      cFittedLine<cPhysicalHit> besttrack_withHits(i.getDirection(), i.getBasepoint());
      for (auto &j : i.getPoints())
      {
        besttrack_withHits.getPoints().push_back(hits[j]);
      }
      tracker_withHits->getLines().push_back(besttrack_withHits);
    }

    //begin tracking
    for (auto &i : tracker_withHits->getLines())
    {

      if (!i.isFittable())
      {
        continue;
      } //tracking only non-beam clusters.
      auto points = i.getPoints();
      // i deve essere una lista di punti
      // Create  the "chi square" function, and load the points of the line in it
      cLine3DFCN<cPhysicalHit> fcnFunctor;
      fcnFunctor.setPoints(points);

      // Create the fitter object
      ROOT::Fit::Fitter fitter;

      // Create a functor that wraps the "chi square" function in order for the Fitter
      // to like it
      ROOT::Math::Functor fcn(fcnFunctor, 4);

      // Look for the point that is further from the first in the array. Then use
      // the line passing the two as starting parameter for the fit
      TVector3 firstp(points.front()[0], points.front()[1], points.front()[2]);
      TVector3 maxp;
      double curMaxDist = 0.;
      for (auto &p : points)
      {
        TVector3 currentPoint(p[0], p[1], p[2]);
        double curDist = (currentPoint - firstp).Mag();
        if (curMaxDist < curDist)
        {
          curMaxDist = curDist;
          maxp = currentPoint;
        }
      }

      TVector3 n = (firstp - maxp).Unit();
      double thstart = TMath::ACos(n[0]);
      double phistart = TMath::ATan(n[2] / n[1]);

      TVector3 xtil(-sin(thstart), cos(thstart) * cos(phistart), cos(thstart) * sin(phistart));
      TVector3 ytil(0, -sin(phistart), cos(phistart));

      // Parameter starting point
      double pStart[4] = {thstart, phistart, firstp * xtil, firstp * ytil};

      // Set up the fit and run it
      fitter.SetFCN(fcn, pStart);
      fitter.Config().ParamsSettings()[0].SetValue(thstart);
      fitter.Config().ParamsSettings()[1].SetValue(phistart);
      fitter.Config().ParamsSettings()[2].SetValue(firstp * xtil);
      fitter.Config().ParamsSettings()[3].SetValue(firstp * ytil);
      fitter.FitFCN();

      // Get the fit result
      auto &r = fitter.Result();
      const std::vector<double> &par = r.Parameters();

      i.setBasepoint(fcnFunctor.getBasepoint(par));
      i.setDirection(fcnFunctor.getDirection(par));
    }
    // end for(int i=0; i<vectracks.size(); i++)

    // end tracking
    /*
      // begin vertex.
      arrayV2 model;
      vector<int> maybetracks; //indici delle possibili tracce da fittare
      vector<int> besttracks; //indici delle tracce migliori
      besttracks.clear();

      //In this for loop different clusters are comapred:
      //if a cluster has a pad which (squared) distance from the model of another track is smaller than
      //SqVertex_Width, then this two tracks are from the same event
      for (unsigned int a=0; a<vectracks.size() ; a++) {
        if (vectracks[a].isFittable()==false) {continue;}

        //cout << a << "  :aTrack" << endl;
        maybetracks.clear();
        cFittedLine<int> aTrack=vectracks[a];
        maybetracks.push_back(a);

        model[0] = aTrack.getBasepoint();
        model[1] = aTrack.getDirection();

        for (unsigned int i=0; i<vectracks.size()  ; i++){

          if (i==a || vectracks[i].isFittable()==false ) {continue;}
          for(auto &h : vectracks[i].getPoints()) {

            arrayD4 point = {hits[h].getX(),hits[h].getY(),hits[h].getZ(), hits[h].getEnergy() };

            if (GetError(model, point)<SqVertex_Width){
                //cout << "trovato: "<< i << endl;
                maybetracks.push_back(i);
                break;                      //a single point is enough.
            } // end if (GetError(model, point)<SqVertex_Width)
          } // end for(auto &h: vectracks[i].getPoints())

          if (maybetracks.size()>besttracks.size() && maybetracks.size()>1){
            besttracks=maybetracks;
          }

        }  // end for (int i=0; i<vectracks.size() ; i++)
      }   // end for (int a=0; a<vectracks.size() ; a++)

      // end    Vertex: starting second "ransac" (not random actually).

      // Vertex
      AA = TVector3(0,0,0);
      AA = MinuitForVertex(*fitEvt, besttracks);
    //  cVertex<int> vertex(AA.x(),AA.y(),AA.z());
    //  for( unsigned int i=0; i<besttracks.size(); i++){
    //    vertex.getTracks().push_back( vectracks[i] );
    //  }
      //list<cVertex<int>> lvertex;
      //lvertex.push_back(vertex);
      list<TVector3> lvertex;
      lvertex.push_back( AA );
      fitEvt -> vertex = lvertex;
      lvertex.clear();

*/
    auto n = vectracks.size();
    for (unsigned int a = 0; a < n; a++)
    {
      if (vectracks[a].isFittable() == false)
      {
        vectracks[a].setGone(true);
        continue;
      }
      for (auto &i : vectracks[a].getPoints())
      {
        if (hits[i].getY() < 4 || hits[i].getX() > 120 || hits[i].getY() > 60 /*|| (hits[i].getX()<64 && hits[i].getX()>32 && hits[i].getY()>54) */)
        {
          vectracks[a].setGone(true);
          break;
        }
      }
    }
    // list<cFittedLine<int>> topush;
    // topush.insert(topush.begin(), vectracks.begin(), vectracks.end()); //converting a vector to a list.
    // fitEvt->getLines().clear();
    // fitEvt->setLines(topush);
    // topush.clear();

  } // end if ( gidmax != 21231 )
  fOutTree->Fill();

  return kTRUE;
}

void fast_trackingFINAL::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

  //fOutTree->Fill();

  if (fFile)
  {
    Bool_t cleanup = kFALSE;
    TDirectory *savedir = gDirectory;
    if (1)
    {

      fFile->cd();
      fOutTree->Write();
      fProofFile->Print();

      //fFile->Print();
      fOutput->Add(fProofFile);
    }
    else
    {
      cleanup = kTRUE;
    }
    fOutTree->SetDirectory(0);
    gDirectory = savedir;
    fFile->Close();
    // Cleanup, if needed
    if (cleanup)
    {
      TUrl uf(*(fFile->GetEndpointUrl()));
      SafeDelete(fFile);
      gSystem->Unlink(uf.GetFile());
      SafeDelete(fProofFile);
    }
  }
}

void fast_trackingFINAL::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
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

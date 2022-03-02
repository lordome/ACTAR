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

#include "ARDA_extraClasses/cPhysicalHit.h"
#include "ARDA_extraClasses/cPhysicalEvent.h"
#include "ARDA_extraClasses/cFittedLine.h"
#include "ARDA_extraClasses/cFittedEvent.h"

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

// Parameters for minuit
int numx = 0;
int numy = 0;
double numz = 0.;           // numz depends on time and drift speed. (100: default value)
double minMinuitVal = -100; // 100 as default

double zRescaling = 0.;
double energyWeightInFitting = 1.;

arrayI2 GetSample(Int_t size);                                // Two points selected randomly
arrayV2 GetParamWithSample(arrayD4 sample1, arrayD4 sample2); // Calculation of 3D line from two vectors
Double_t GetError(arrayV2 model, arrayD4 p);                  // SQUARE! of the distance between the point and the line

Double_t GetError(arrayV2 model, arrayD4 p, double zCorrection); // SQUARE! of the distance between the point and the line

// for tracking
extern void FunctionForTrack(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag); // MINUIT function for track calculation
arrayV2 MinuitForTrack(cFittedLine<int> &track);                                                   // MINUIT algorithm for track

// for vertex
extern void FunctionForVertex(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag); // MINUIT function for vertex calculation
TVector3 MinuitForVertex(cFittedEvent<int> &tracker, vector<int> besttracks);                       // MINUIT algorithm for vertex

double ClusterTest(double &sumvalue, double &totalenergy, vector<int> &inliers); // function used to test the clusters.

int fit(string input_file = "input_parameters.txt")
{

  ifstream inFile(input_file);

  string Var_Name, inp_n, data_file_name;
  double Var_Value;
  map<string, int> parMap;

  if (!inFile)
  {
    std::cout << "\nError opening file.\n";
    return 13;
  }

  inFile >> Var_Name >> data_file_name;

  TString ifname = data_file_name;

  while (inFile >> Var_Name >> Var_Value)
  {
    parMap.insert(make_pair(Var_Name, Var_Value));
    cout << Var_Name << "  " << Var_Value << endl;
  }

  inFile.close();
  // Setting Hyperparameters
  gROOT->SetBatch(kFALSE);

  // int toanalyse=1518; //Proviamo a semplificare per un primo momento. Questo ci dice il numero di evento da analizzare.
  // cout << "Set event num:" ;
  // cin >> toanalyse;
  double threshold = parMap["tracksEnergyThreshold"];        // minimum energy requested for a single cluster.
  double fenergy = parMap["beamEnergyThreshold"];            // energy threshold for a track in order to be considered a beam track.
  double width = parMap["tracksWidth"];                      // maximum distance from model accepted in clustering
  double fwidth = parMap["beamWidth"];                       // maximum distance from model accepted in clustering for beam tracks
  double SqVertex_Width = parMap["squaredVertexAcceptance"]; // maximum distance accepted between two different cluster
  double loops = parMap["numberLoops"];                      // number of loops, i.e. number of random couples chosen.
  double trsize = parMap["trackMinSize"];                    // min number of pads required in order to consider a cluster a real track
  double besize = parMap["beamMinSize"];                     // min number of pads required in order to consider a cluster a real track FOR BEAM

  // end Setting Hyperparameters

  numx = parMap["numx"];
  numy = parMap["numy"];
  numz = parMap["numz"];

  zRescaling = parMap["zRescaling"];
  energyWeightInFitting = parMap["energyWeightInFitting"];

  bool oneEventOnly = parMap["oneEventOnly"];
  int toAnalyse = parMap["toAnalyse"];
  int startFrom = parMap["startFrom"];

  // Opening the input file.
  TFile *ifile = new TFile(ifname.Data(), "READ");
  if (ifile->IsZombie())
  {
    printf("Unable to open inputfile: %s", ifname.Data());
    return -1;
  }

  // Setting Hyperparameters
  gROOT->SetBatch(kFALSE);

  // end Setting Hyperparameters

  TTree *physicalEventTree = (TTree *)ifile->Get("physicalEventTree");
  if (physicalEventTree->IsZombie())
  {
    printf("Unable to open inputtree: ");
    return -1;
  }

  // Creating output tree

  cFittedEvent<int> *tracker = new cFittedEvent<int>();

  TTreeReader rdr(physicalEventTree);
  TTreeReaderValue<cPhysicalEvent> event(rdr, "event"); // reading input file

  int rndpoint1 = 0; // points index into the hits vector
  int rndpoint2 = 0;
  int iterations = 0; // number of iterations
  arrayD4 sample1 = {0, 0, 0, 0};
  arrayD4 sample2 = {0, 0, 0, 0};
  arrayV2 maybemodel = {zero, zero};
  arrayV2 bestmodel = {zero, zero};
  vector<int> alsoinliers; // vector to be filled with the possible indexes.
  vector<int> bestinliers; // vector to be filled with the best indexes.
  cFittedLine<int> besttrack;
  Double_t besterr = DBL_MAX;
  Double_t thiserr = 0;
  vector<int> unfittedPoints; // vector that contains every index.

  double problemi = 0;

  vector<double> energie;
  double trackenergy = 0;
  energie.clear();

  float_t numtracks = -4;
  float_t xver = -10;

  int nent = rdr.GetEntries(false);
  double vercount = 0;
  double vercount3 = 0;
  double vercount2 = 0;
  TVector3 AA;
  double trackcount = 0;
  int beamtrackcount = 0;
  bool gone = false;
  bool bestgone = false;

  vector<double> vec_trackEnergy;
  double bestenergy;

  vector<double> buoni;

  TCanvas *canvas3dDiviso = new TCanvas();

  vector<TH3F *> vHisto3D;
  vHisto3D.emplace_back(new TH3F("histofirst", "", 128, 0, 128, 128, 0, 128, 100, 0, 500));

  TH3F *totalEvent = new TH3F();

  while (rdr.Next())
  {

    cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;

    if ((!oneEventOnly || event->getEventNumber() == toAnalyse) && event->getEventNumber() > startFrom)
    {
      // toanalyse+277)     {
      //  if (event->getEventNumber() == 1746 + 277 )     {

      cout << event->getEventNumber() << endl;
      energie.clear();
      delete tracker;
      tracker = new cFittedEvent<int>();
      tracker->setEventNumber(event->getEventNumber());
      tracker->setTimestamp(event->getTimestamp());
      tracker->getLines().clear();

      list<cPhysicalHit> hitslist = event->getHits(); // picking up the list from the event and converting it into a vector.
      hits = vector<cPhysicalHit>(hitslist.begin(), hitslist.end());

      unfittedPoints.clear();
      unfittedPoints.reserve(hits.size());
      for (unsigned int i = 0; i < hits.size(); i++)
      {
        unfittedPoints.push_back(i);
      } // it's a vector containing indexes. The ones that will be included in a cluster will be ==-1.

      // begin clustering

      //!!!!! The while loop has an always true condition, in order not to have an upper limit of tracks.
      // This loop (in both beam and non-beam tracks) stops when the track dimension is == 0 after a whole cycle.
      beamtrackcount = 0;

      while (1)
      { // There are particular conditions, the points chosen randomly must have same y & z, the energy threshold is very high, the number of pads required has to be high.

        iterations = 0;
        besterr = DBL_MAX;
        besttrack.getPoints().clear();
        bestinliers.clear();

        while (iterations < loops)
        {

          rndpoint1 = rnd->Uniform(0, hits.size());
          rndpoint2 = rnd->Uniform(0, hits.size());

          while (!hits[rndpoint1].isTrackable() || unfittedPoints[rndpoint1] == -1)
          {                                           // isTrackable lets us select some hits that are not useful for tracking (ancillaryHit, noisy pad.)
            rndpoint1 = rnd->Uniform(0, hits.size()); // isTrackable is selected into precalibrator.cc
          };
          while (!hits[rndpoint2].isTrackable() || unfittedPoints[rndpoint2] == -1)
          {
            rndpoint2 = rnd->Uniform(0, hits.size());
          };

          if (abs(hits[rndpoint1].getZ() - hits[rndpoint2].getZ()) != 0 || abs(hits[rndpoint1].getY() - hits[rndpoint2].getY()) > 0.5)
          {
            iterations++;
            continue;
          }; // testing if y1==y2 && z1==z2

          maybemodel[0].SetXYZ(0, 0, 0);
          maybemodel[1].SetXYZ(0, 0, 0);
          alsoinliers.clear();

          sample1 = {hits[rndpoint1].getX(), hits[rndpoint1].getY(), hits[rndpoint1].getZ(), hits[rndpoint1].getEnergy()};
          sample2 = {hits[rndpoint2].getX(), hits[rndpoint2].getY(), hits[rndpoint2].getZ(), hits[rndpoint2].getEnergy()};

          maybemodel = GetParamWithSample(sample1, sample2); // building the 3D line between the random points.

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
            } // end if (GetError(maybemodel,samplex)<pow(fwidth,2) && hits[i].isTrackable())

          } // end for (int i=0;i<unfittedPoints.size();i++)

          GetErrorMean = ClusterTest(sumvalue, totalenergy, alsoinliers); // function that evaluates the cluster(it can be changed)

          if (alsoinliers.size() > besize && GetErrorMean < besterr && totalenergy > fenergy)
          { // comparing the cluster to the previous best one.
            // if the current cluster is better than the previous best, it simply becomes the new best.
            bestmodel = maybemodel;
            besterr = GetErrorMean;
            bestinliers = alsoinliers;
            bestenergy = totalenergy;

          } // end if (alsoinliers.size()>20 && GetErrorMean<besterr && totalenergy>fenergy)
          iterations++;
        } // end while (iterations<loops)

        // cout << bestinliers.size() << "fsize" << endl; //outputs the size of the best cluster found: it's a useful and simple check.

        if (bestinliers.empty())
        {
          break;
        } // endind the cycle whether the best tracks has dimension == 0.

        vec_trackEnergy.push_back(bestenergy);
        for (unsigned int i = 0; i < bestinliers.size(); i++)
        {
          unfittedPoints[bestinliers[i]] = -1;
        } // setting all the used points to -1, so they won't be used anymore.

        if (trackenergy != 0)
        {
          energie.push_back(trackenergy / bestinliers.size());
        }

        besttrack.getPoints().insert(besttrack.getPoints().begin(), bestinliers.begin(), bestinliers.end()); // setting besttrack (conversion from vector to list)
        besttrack.setFittable(false);
        tracker->getLines().push_back(besttrack); // saving besttrack into tracker, which is going to be saved into the output tree.

        beamtrackcount++;
      } // end while (1){

      // cout << "Beam tcnt: " << beamtrackcount << endl;
      // cout << "Beam track energy" << endl;
      // for (auto &i : vec_trackEnergy)
      // {
      //   cout << "En_i = " << i << endl;
      // }

      trackcount = 0;

      while (1)
      { // looking for normal tracks. The method used is exactly the previous one.

        besterr = DBL_MAX;
        iterations = 0;
        bestinliers.clear();
        besttrack.getPoints().clear();
        trackenergy = 0;

        while (iterations < loops)
        {

          rndpoint1 = rnd->Uniform(0, hits.size());
          rndpoint2 = rnd->Uniform(0, hits.size());

          auto hits1 = hits[rndpoint1];
          auto hits2 = hits[rndpoint2];

          while (!hits[rndpoint1].isTrackable() || unfittedPoints[rndpoint1] == -1)
          {
            rndpoint1 = rnd->Uniform(0, hits.size());
          };
          while (!hits[rndpoint2].isTrackable() || unfittedPoints[rndpoint2] == -1)
          {
            rndpoint2 = rnd->Uniform(0, hits.size());
          };

          if (((hits1[0] * -hits2[0]) * (hits1[0] * -hits2[0]) - (hits1[1] * -hits2[1]) * (hits1[1] * -hits2[1])) < 20)
          {
            iterations++;
            continue;
          }; // testing if y1==y2 && z1==z2

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

            if (GetError(maybemodel, samplex) < pow(width, 2) /*&& ( hits[i].isTrackable()*/)
            {
              alsoinliers.push_back(i);
              arrayD4 samplex = {hits[i].getX(), hits[i].getY(), hits[i].getZ(), hits[i].getEnergy()};
              sumvalue += GetError(maybemodel, samplex) * hits[i].getEnergy();
              totalenergy += hits[i].getEnergy();

            } // end if ( GetError(maybemodel,samplex)<width*width && hits[i].isFittable() && i!=-1 )
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
        } // end while (iterations<loops)
        // cout << bestinliers.size() << "size" << endl;

        if (bestinliers.empty())
        {
          break;
        } // se non ho trovato tracce buone, fermati.

        double media = 0;
        double count = 0;
        for (unsigned int i = 0; i < bestinliers.size(); i++)
        {
          media += hits[bestinliers[i]].getY();
        }

        media /= bestinliers.size();

        vector<int> temporary;

        if (media > 68)
        {
          for (unsigned int i = 0; i < bestinliers.size(); i++)
          {
            if (hits[bestinliers[i]].getY() > 63.9)
              temporary.push_back(bestinliers[i]);
          }
          bestinliers = temporary;
        }

        if (media < 60)
        {
          for (unsigned int i = 0; i < bestinliers.size(); i++)
          {
            if (hits[bestinliers[i]].getY() < 60.1)
              temporary.push_back(bestinliers[i]);
          }
          bestinliers = temporary;
        }

        for (unsigned int i = 0; i < bestinliers.size(); i++)
        {
          unfittedPoints[bestinliers[i]] = -1;
        } // setto i punti clusterizzati a -1.

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

      vector<cFittedLine<int>> vectracks(tracker->getLines().begin(), tracker->getLines().end()); // converting a list to a vector;

      // begin tracking
      for (unsigned int i = 0; i < vectracks.size(); i++)
      {
        indextofit.clear();

        double energy_track = 0;
        for (auto &it : vectracks[i].getPoints())
        {
          energy_track += hits[it].getEnergy();
        }
        if (!vectracks[i].isFittable())
        {
          continue;
        } // tracking only non-beam clusters.
        arrayV2 outV;
        outV = MinuitForTrack(vectracks[i]);

        if(abs(outV[1][2]) > 0.999 ){
          vectracks[i].setFittable(false);
        }

        cout << "BEAM?" << (vectracks[i].isFittable() ? "NO  " : "YES  ") << "Size: " << vectracks[i].getPoints().size() << " Energy: " << energy_track << " Average energy: " << energie[i] << endl;
        cout << "Basepoint:" << outV[0][0] << "  " << outV[0][1] << "  " << outV[0][2] << "Direction:" << outV[1][0] << "  " << outV[1][1] << "  " << outV[1][2] << endl;

        vectracks[i].setBasepoint(outV[0]);
        vectracks[i].setDirection(outV[1]);
      } // end for(int i=0; i<vectracks.size(); i++)




      list<cFittedLine<int>> topush;
      topush.insert(topush.begin(), vectracks.begin(), vectracks.end()); // converting a vector to a list.
      tracker->getLines().clear();
      tracker->setLines(topush);
      topush.clear(); // saving the tracked lines.
      // end tracking

      // begin vertex.
      arrayV2 model;
      vector<int> maybetracks; // indici delle possibili tracce da fittare
      vector<int> besttracks;  // indici delle tracce migliori
      besttracks.clear();

      // in this for loop different cluster are comapred:
      // if a cluster has a pad which (squared) distance from the model of another track is smaller than
      // SqVertex_Width, then this two tracks are from the same event
      for (unsigned int a = 0; a < vectracks.size(); a++)
      {
        if (vectracks[a].isFittable() == false)
        {
          continue;
        }

        // cout << a << "  :aTrack" << endl;
        maybetracks.clear();
        cFittedLine<int> aTrack = vectracks[a];
        maybetracks.push_back(a);

        model[0] = aTrack.getBasepoint();
        model[1] = aTrack.getDirection();

        for (unsigned int i = 0; i < vectracks.size(); i++)
        {

          if (i == a || vectracks[i].isFittable() == false)
          {
            continue;
          }
          for (auto &h : vectracks[i].getPoints())
          {

            arrayD4 point = {hits[h].getX(), hits[h].getY(), hits[h].getZ(), hits[h].getEnergy()};

            if (GetError(model, point) < SqVertex_Width)
            {
              // cout << "trovato: "<< i << endl;
              maybetracks.push_back(i);
              break; // a single point is enough.
            }        // end if (GetError(model, point)<SqVertex_Width)
          }          // end for(auto &h: vectracks[i].getPoints())

          if (maybetracks.size() > besttracks.size() && maybetracks.size() > 1)
          {
            besttracks = maybetracks;
          }

        } // end for (int i=0; i<vectracks.size() ; i++)
      }   // end for (int a=0; a<vectracks.size() ; a++)

      // end  Vertex: starting second "ransac" (not random actually).

      // Vertex
      // cout << "Numero di tracce per vertice: " << besttracks.size() << endl;
      AA = TVector3(0, 0, 0);
      AA = MinuitForVertex(*tracker, besttracks);
      cVertex<int> vertex(AA.x(), AA.y(), AA.z());
      for (unsigned int i = 0; i < besttracks.size(); i++)
      {
        vertex.getTracks().push_back(vectracks[i]);
      }
      list<cVertex<int>> lvertex;
      lvertex.push_back(vertex);
      tracker->setVertex(lvertex);
      lvertex.clear();

      for (auto &a : besttracks)
      {
        if (vectracks[a].isFittable() == false)
        {
          continue;
          vectracks[a].setGone(true);
        }
        for (auto &i : vectracks[a].getPoints())
        {
          if (hits[i].getY() < 4 || hits[i].getX() > 120 /*|| (hits[i].getX()<64 && hits[i].getX()>32 && hits[i].getY()>54) || hits[i].getY()>60 */)
          {
            vectracks[a].setGone(true);
            break;
          }
        }
      }

      /*
        TH3F *h3d = new TH3F("histoall", "", 128, 0, 128, 128, 0, 128, 100, 0, 500);
        TH2F *h2d = new TH2F("Istogramma2D", "", 128, 0, 128, 128, 0, 128);

        for (unsigned int i = 0; i < hits.size(); i++)
        {
          for (int j = 0; j < hits[i].getEnergy(); j++)
          {
            h3d->Fill(hits[i].getX(), hits[i].getY(), hits[i].getZ());
            h2d->Fill(hits[i].getX(), hits[i].getY());
          }
        }
      */
      /*
        TCanvas *canvas2d = new TCanvas("Canvas2d", "Canvas2d", 1600, 1200);
        canvas2d->cd();
        h2d->GetXaxis()->SetTitle(" Asse x [mm] ");
        h2d->GetYaxis()->SetTitle(" Asse y [mm] ");
        // h2d->GetXaxis()->SetTitleOffset(2);
        //  h2d->GetYaxis()->SetTitleOffset(2);
        h2d->Draw("COLZ");

        TCanvas *canvas3d = new TCanvas("Canvas3d", "Canvas3d", 1600, 1200);
        h3d->GetXaxis()->SetTitle(" Asse x [mm] ");
        h3d->GetYaxis()->SetTitle(" Asse y [mm] ");
        h3d->GetZaxis()->SetTitle(" Asse z [a.u.] ");
        h3d->GetXaxis()->SetTitleOffset(2);
        h3d->GetYaxis()->SetTitleOffset(2);
        h3d->GetZaxis()->SetTitleOffset(1.2);

        h3d->Draw("LEGO");
      */

      for (auto p : vHisto3D)
      {
        delete p;
      }
      vHisto3D.clear();

      delete canvas3dDiviso;
      canvas3dDiviso = new TCanvas("Canvas3D", "Canvas3D", 800, 600);
      canvas3dDiviso->Divide(3, 3);
      canvas3dDiviso->cd();

      // qui manca da inserire uno switch per la divisione del canvas.
      vHisto3D.emplace_back(new TH3F("histoall", "", 128, 0, 128, 128, 0, 128, 100, 0, 500));
      for (unsigned int i = 0; i < hits.size(); i++)
      {
        vHisto3D[0]->Fill(hits[i].getX(), hits[i].getY(), hits[i].getZ());
      }
      canvas3dDiviso->cd(1);
      vHisto3D[0]->Draw("lego");

      string title;

      for (unsigned int i = 0; i < vectracks.size() && i < 9; i++)
      {
        title = "vHisto3D" + to_string(i);
        vHisto3D.emplace_back(new TH3F(title.c_str(), title.c_str(), 128, 0, 128, 128, 0, 128, 100, 0, 500));
        vHisto3D.at(i)->GetXaxis()->SetTitle(" Asse x [mm] ");
        vHisto3D.at(i)->GetYaxis()->SetTitle(" Asse y [mm] ");
        vHisto3D.at(i)->GetZaxis()->SetTitle(" Asse z [a.u.] ");
        vHisto3D.at(i)->GetXaxis()->SetTitleOffset(2);
        vHisto3D.at(i)->GetYaxis()->SetTitleOffset(2);
        vector<int> index1(vectracks[i].getPoints().begin(), vectracks[i].getPoints().end());
        for (unsigned int j = 0; j < index1.size(); j++)
        {
          vHisto3D[i + 1]->Fill(hits[index1[j]].getX(), hits[index1[j]].getY(), hits[index1[j]].getZ());
        }
        canvas3dDiviso->cd(i + 2);
        vHisto3D[i + 1]->Draw("LEGO");

        if (vectracks[i].isFittable())
        { // now drawing line
          TPolyLine3D *drawObj = new TPolyLine3D();

          TVector3 base1 = vectracks[i].getBasepoint() + vectracks[i].getDirection() * 1000;
          drawObj->SetNextPoint(base1.x(), base1.y(), base1.z());

          TVector3 bn = vectracks[i].getBasepoint() - vectracks[i].getDirection() * 1000;

          drawObj->SetNextPoint(bn.x(), bn.y(), bn.z());

          drawObj->Draw("same");
        }
      }

      title = "vHisto3DRicostruito";
      delete totalEvent;
      totalEvent = new TH3F(title.c_str(), "", 128, 0, 128, 128, 0, 128, 100, 0, 500);

      for (unsigned int i = 0; i < besttracks.size(); i++)
      {

        vector<int> index1(vectracks[besttracks[i]].getPoints().begin(), vectracks[besttracks[i]].getPoints().end());
        for (unsigned int j = 0; j < index1.size(); j++)
        {

          totalEvent->Fill(hits[index1[j]].getX(), hits[index1[j]].getY(), hits[index1[j]].getZ());

          // for (unsigned int k = 0; k < hits[index1[j]].getEnergy(); k++)
          // {
          // }
        }
      }
      canvas3dDiviso->cd(8);
      totalEvent->Draw("lego");
      totalEvent->GetXaxis()->SetTitle(" Asse x [mm] ");
      totalEvent->GetYaxis()->SetTitle(" Asse y [mm] ");
      totalEvent->GetZaxis()->SetTitle(" Asse z [a.u.] ");
      totalEvent->GetXaxis()->SetTitleOffset(2);
      totalEvent->GetYaxis()->SetTitleOffset(2);
      totalEvent->GetZaxis()->SetTitleOffset(1.2);
      canvas3dDiviso->Update();
      canvas3dDiviso->WaitPrimitive();

      //  break;

      /*TCanvas *canvasRicostruito = new TCanvas("canvasRicostruito", "canvasRicostruito", 1600, 1200);

      canvasRicostruito->Divide(3, 1);
      canvasRicostruito->cd(1);
      h3d->Draw("LEGO");
      canvasRicostruito->cd(3);
      vHisto3D[5]->Draw("lego");

      for (unsigned int i = 0; i < vectracks.size() && i < 6; i++)
      {
        if (vectracks[i].isFittable() && i != 2)
        { // now drawing line
          TPolyLine3D *drawObj = new TPolyLine3D();

          TVector3 base1 = vectracks[i].getBasepoint() - vectracks[i].getDirection() * ((1 + vectracks[i].getBasepoint().x()) / vectracks[i].getDirection().x());
          drawObj->SetNextPoint(base1.x(), base1.y(), base1.z());

          TVector3 bn = vectracks[i].getBasepoint() + vectracks[i].getDirection() * ((66 - vectracks[i].getBasepoint().y()) / vectracks[i].getDirection().y());

          drawObj->SetNextPoint(bn.x(), bn.y(), bn.z());
          drawObj->SetLineColor(2);
          drawObj->SetLineWidth(4);
          drawObj->Draw("same");
        }
        if (i == 2)
        {
          TPolyLine3D *drawObj = new TPolyLine3D();
          TVector3 base1 = vectracks[i].getBasepoint() + vectracks[i].getDirection() * ((1 + vectracks[i].getBasepoint().x()) / vectracks[i].getDirection().x());
          drawObj->SetNextPoint(base1.x(), base1.y(), base1.z());
          TVector3 bn = vectracks[i].getBasepoint() - vectracks[i].getDirection() * ((1 + vectracks[i].getBasepoint().y()) / vectracks[i].getDirection().y());
          drawObj->SetNextPoint(bn.x(), bn.y(), bn.z());
          drawObj->SetLineColor(2);
          drawObj->SetLineWidth(4);
          drawObj->Draw("same");
        }
      }*/

      /*vector<TH3F *> vHisto3DRansac;

      TCanvas *canvasRansacColori = new TCanvas("CanvasRColor", "CanvasRColor", 1600, 1200);
      canvasRansacColori->cd();

      for (unsigned int i = 0; i < vectracks.size() && i < 6; i++)
      {
        title = "vHisto3D" + to_string(i);
        vHisto3DRansac.emplace_back(new TH3F(title.c_str(), "", 64, 0, 128, 32, 0, 64, 100, 0, 17));
        vHisto3DRansac.at(i)->GetXaxis()->SetTitle(" Asse x [mm] ");
        vHisto3DRansac.at(i)->GetYaxis()->SetTitle(" Asse y [mm] ");
        vHisto3DRansac.at(i)->GetZaxis()->SetTitle(" Asse z [a.u.] ");
        vHisto3DRansac.at(i)->GetXaxis()->SetTitleOffset(2);
        vHisto3DRansac.at(i)->GetYaxis()->SetTitleOffset(2);
        vHisto3DRansac.at(i)->GetZaxis()->SetTitleOffset(1.2);
        vHisto3DRansac.at(i)->SetFillColor(i + 1);
        vector<int> index1(vectracks[i].getPoints().begin(), vectracks[i].getPoints().end());
        for (unsigned int j = 0; j < index1.size(); j++)
        {
          vHisto3DRansac[i]->Fill(hits[index1[j]].getX(), hits[index1[j]].getY(), hits[index1[j]].getZ(), hits[index1[j]].getEnergy());
        }
        vHisto3DRansac[i]->SetFillColor(i + 1);
        canvasRicostruito->cd(2);

        vHisto3DRansac[i]->Draw("BOX3 same");
      }

      title = "vHisto3DRicostruitoRansac";
      vHisto3D.emplace_back(new TH3F(title.c_str(), "", 64, 0, 128, 32, 0, 64, 100, 0, 17));

      TCanvas *canvasRansac = new TCanvas("CanvasRansac", "CanvasRansac", 1600, 1200);
      canvasRansac->cd();

      for (unsigned int i = 0; i < vectracks.size(); i++)
      {
        vector<int> index1(vectracks[besttracks[i]].getPoints().begin(), vectracks[besttracks[i]].getPoints().end());
        for (unsigned int j = 0; j < index1.size(); j++)
        {
          for (unsigned int k = 0; k < hits[index1[j]].getEnergy(); k++)
          {
            vHisto3D[6]->Fill(hits[index1[j]].getX(), hits[index1[j]].getY(), hits[index1[j]].getZ());
          }
        }
      }
      vHisto3D[6]->Draw("lego");
      vHisto3D.at(6)->GetXaxis()->SetTitle(" Asse x [mm] ");
      vHisto3D.at(6)->GetYaxis()->SetTitle(" Asse y [mm] ");
      vHisto3D.at(6)->GetZaxis()->SetTitle(" Asse z [a.u.] ");
      vHisto3D.at(6)->GetXaxis()->SetTitleOffset(2);
      vHisto3D.at(6)->GetYaxis()->SetTitleOffset(2);
      vHisto3D.at(6)->GetZaxis()->SetTitleOffset(1.2);

      vector<TH2F *> vHisto2D;

      TCanvas *canvas2DDiviso = new TCanvas("cani", "ciao", 1600, 1200);
      canvas2DDiviso->Divide(3, 2);
      canvas2DDiviso->cd();
      // qui manca da inserire uno switch per la divisione del canvas.
      vHisto2D.emplace_back(new TH2F("vHisto2Dall", "", 64, 0, 128, 32, 0, 64));
      for (unsigned int i = 0; i < hits.size(); i++)
      {
        for (int k = 0; k < hits[i].getEnergy(); k++)
          vHisto2D[0]->Fill(hits[i].getX(), hits[i].getY());
      }
      canvas2DDiviso->cd(1);
      vHisto2D[0]->Draw("colz");

      for (unsigned int i = 0; i < vectracks.size() && i < 6; i++)
      {
        string title = "vHisto2D" + to_string(i);
        vHisto2D.emplace_back(new TH2F(title.c_str(), title.c_str(), 64, 0, 128, 32, 0, 64));
        vHisto2D.at(i)->GetXaxis()->SetTitle(" Asse x [mm] ");
        vHisto2D.at(i)->GetYaxis()->SetTitle(" Asse y [mm] ");
        vector<int> index1(vectracks[i].getPoints().begin(), vectracks[i].getPoints().end());
        for (unsigned int j = 0; j < index1.size(); j++)
        {
          for (int a = 0; a < hits[index1[j]].getEnergy(); a++)
            vHisto2D[i + 1]->Fill(hits[index1[j]].getX(), hits[index1[j]].getY());
        }
        canvas2DDiviso->cd(i + 2);
        vHisto2D[i + 1]->Draw("colz");

        if (vectracks[i].isFittable())
        { // now drawing line
          TPolyLine3D *drawObj = new TPolyLine3D();

          TVector3 base1 = vectracks[i].getBasepoint() - vectracks[i].getDirection() * 100;
          drawObj->SetNextPoint(base1.x(), base1.y(), 0);

          TVector3 bn = vectracks[i].getBasepoint() + vectracks[i].getDirection() * 100;

          drawObj->SetNextPoint(bn.x(), bn.y(), 0);

          drawObj->Draw("same");
        }
      }*/

      /*TCanvas *canvas2D3D = new TCanvas;
      canvas2D3D->Divide(2, 1);
      canvas2D3D->cd(1);
      h2d->Draw("COLZ");
      canvas2D3D->cd(2);
      h3d->Draw("lego");

      canvas2D3D->WaitPrimitive();
      */
    }
    else
    {
      continue;
    }

    // break;
  }

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
  TVector3 d = {p[0] - H[0], p[1] - H[1], (p[2] - H[2]) / zRescaling}; // it was necessary to put this factor 10 due to measurement units (as I have set the precalibrator).

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
    pointstofit = {hits[indextofit[i]].getX(), hits[indextofit[i]].getY(), hits[indextofit[i]].getZ(), energyWeightInFitting * hits[indextofit[i]].getEnergy()};
    chisq += GetError(minuitmodel, pointstofit);
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
  // double_t
  // Double_t maxX = numx;
  // Double_t maxY = numy;
  // Double_t maxZ = numz;
  gMinuit->DefineParameter(0, "Ax", 0, 0.01, minMinuitVal, numx);
  gMinuit->DefineParameter(1, "Ay", 64, 10, minMinuitVal, numy);
  gMinuit->DefineParameter(2, "Az", 300, 50, minMinuitVal, numz);
  gMinuit->DefineParameter(3, "Bx", 0., 1., -100, 100);
  gMinuit->DefineParameter(4, "By", 0.01, 1, -100, 100);
  gMinuit->DefineParameter(5, "Bz", 0., 30., -100, 100);

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

// end track

// begin Vertex

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

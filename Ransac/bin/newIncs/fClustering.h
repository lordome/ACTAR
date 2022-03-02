#ifndef fClustering_h
#define fClustering_h

#include <iostream>
#include <vector>
#include <list>
#include <array>

#include "TVector3.h"
#include "TRandom3.h"

#include "cPhysicalEvent.h"
#include "cLookupTable.h"
#include "cRawEvent.h"
#include "cFittedLine.h"
#include "cFittedEvent.h"

#include "fParameters.h"

using namespace std;

typedef array<Double_t, 4> arrayD4;
typedef array<TVector3, 2> arrayV2;

TVector3 zero( 0,0,0 );//(0,0,0); 			// zero vector
extern vector<cPhysicalHit> hits;
//extern vector<double> energie;

TRandom3 *rnd = new TRandom3;

arrayV2 GetParamWithSample(arrayD4 sample1, arrayD4 sample2);			// Calculation of 3D line from two vectors
Double_t GetError(arrayV2 model, arrayD4 p);							// SQUARE! of the distance between the point and the line
double ClusterTest(double& sumvalue, double& totalenergy, vector<int>& inliers); //function used to test the clusters.

cFittedEvent<int> *Cluster(cFittedEvent<int> *tracker , list<cPhysicalHit>& hitslist, double& trackcount, double evNumber){

  fParameters* para = new fParameters();

  double trackenergy;
  int rndpoint1=0; //points index into the hits vector
  int rndpoint2=0;
  int iterations =0; //number of iterations
  arrayD4 sample1 = {0,0,0,0};
  arrayD4 sample2 = {0,0,0,0};
  arrayV2 maybemodel = {zero, zero};
  arrayV2 bestmodel = {zero, zero};
  vector<int> alsoinliers; //vector to be filled with the possible indexes.
  vector<int> bestinliers; //vector to be filled with the best indexes.
  cFittedLine<int> besttrack;
  Double_t besterr = DBL_MAX;
  Double_t thiserr = 0;
  vector<int> unfittedPoints; //vector that contains every index.
  unfittedPoints.clear();

  vector<double> energie;

  hits = vector<cPhysicalHit>(hitslist.begin(),hitslist.end()); //picking up the list from the event and converting it into a vector.

  for (unsigned int i=0; i<hits.size(); i++) {
    unfittedPoints.push_back(i);
  } //it's a vector containing indexes. The ones that will be included in a cluster will be ==-1.

  delete tracker;
  tracker = new cFittedEvent<int>;
  tracker->getLines().clear();

  tracker->setEventNumber(evNumber);

  while (1){    //There are particular conditions, the points chosen randomly must have same y & z, the energy threshold is very high, the number of pads required has to be high.

    iterations =0;
    besterr=DBL_MAX;
    besttrack.getPoints().clear();
    bestinliers.clear();

    while (iterations<para->loops){

      rndpoint1 = rnd->Uniform(0,hits.size());
      rndpoint2 = rnd->Uniform(0,hits.size());

      while (!hits[rndpoint1].isTrackable() || unfittedPoints[rndpoint1]==-1){ //isTrackable lets us select some hits that are not useful for tracking (ancillaryHit, noisy pad.)
        rndpoint1 = rnd->Uniform(0,hits.size());                                //isTrackable is selected into precalibrator.cc
      };
      while (!hits[rndpoint2].isTrackable() || unfittedPoints[rndpoint2]==-1){
        rndpoint2 = rnd->Uniform(0,hits.size());
      };

      if (hits[rndpoint1].getZ()!=hits[rndpoint2].getZ() && hits[rndpoint1].getY()!=hits[rndpoint2].getY()) {
        iterations++;
        continue;
      }; // testing if y1==y2 && z1==z2

      maybemodel[0].SetXYZ(0,0,0);
      maybemodel[1].SetXYZ(0,0,0);
      alsoinliers.clear();

      sample1 = {hits[rndpoint1].getX(), hits[rndpoint1].getY(), hits[rndpoint1].getZ() , hits[rndpoint1].getEnergy()};
      sample2= {hits[rndpoint2].getX(), hits[rndpoint2].getY(), hits[rndpoint2].getZ(), hits[rndpoint2].getEnergy()};

    	maybemodel = GetParamWithSample(sample1, sample2); //building the 3D line between the random points.

      double GetErrorMean=0;
      double sumvalue=0;
      double totalenergy=0;

      // For every point in data, if point fits maybemodel with an error(squared) smaller than width,
    	// add its index to alsoinliers, compute totalenergy and compute sumvalue (used to test the inliers.)

    	for (unsigned int i=0;i<unfittedPoints.size();i++) {

        if (unfittedPoints[i]==-1) {continue;}

        arrayD4 samplex = {hits[i].getX(), hits[i].getY(), hits[i].getZ(), hits[i].getEnergy()};

        if (GetError(maybemodel,samplex)<pow(para->fwidth,2) && hits[i].isTrackable()) {

          alsoinliers.push_back(i);
          sumvalue += GetError(maybemodel, samplex)*hits[i].getEnergy();
          totalenergy+=hits[i].getEnergy();

        } //end if (GetError(maybemodel,samplex)<pow(fwidth,2) && hits[i].isTrackable())

      } // end for (int i=0;i<unfittedPoints.size();i++)

      GetErrorMean = ClusterTest(sumvalue, totalenergy, alsoinliers); //function that evaluates the cluster(it can be changed)

      if (alsoinliers.size()>(para->besize) && GetErrorMean<besterr && totalenergy>(para->fenergy) ) { // comparing the cluster to the previous best one.
                                                                                 // if the current cluster is better than the previous best, it simply becomes the new best.
        bestmodel=maybemodel;
        besterr=GetErrorMean;
        bestinliers=alsoinliers;

      } // end if (alsoinliers.size()>20 && GetErrorMean<besterr && totalenergy>fenergy)
      iterations++;
    } //end while (iterations<loops)

    //cout << bestinliers.size() << "fsize" << endl; //outputs the size of the best cluster found: it's a useful and simple check.

    if( bestinliers.empty() ) {break;} //endind the cycle whether the best tracks has dimension == 0.

    for (unsigned int i=0; i<bestinliers.size();i++) {
      unfittedPoints[bestinliers[i]]=-1;
    } //setting all the used points to -1, so they won't be used anymore.

    besttrack.getPoints().insert(besttrack.getPoints().begin(),bestinliers.begin(),bestinliers.end());  //setting besttrack (conversion from vector to list)
    besttrack.setFittable(false);
    tracker->getLines().push_back(besttrack); //saving besttrack into tracker, which is going to be saved into the output tree.
  } // end while (1){

   trackcount=0;
  while (1){ //looking for normal tracks. The method used is exactly the previous one.

    besterr=DBL_MAX;
    iterations = 0;
    bestinliers.clear();
    besttrack.getPoints().clear();
    trackenergy=0;

    while (iterations<para->loops){

      rndpoint1 = rnd->Uniform(0,hits.size());
      rndpoint2 = rnd->Uniform(0,hits.size());

      while (!hits[rndpoint1].isTrackable() || unfittedPoints[rndpoint1]==-1){
        rndpoint1 = rnd->Uniform(0,hits.size());
      };
      while (!hits[rndpoint2].isTrackable() || unfittedPoints[rndpoint2]==-1){
        rndpoint2 = rnd->Uniform(0,hits.size());
      };

      maybemodel[0].SetXYZ(0,0,0);
      maybemodel[1].SetXYZ(0,0,0);
      alsoinliers.clear();

      sample1 = {hits[rndpoint1].getX(), hits[rndpoint1].getY(), hits[rndpoint1].getZ() , hits[rndpoint1].getEnergy()};
      sample2= {hits[rndpoint2].getX(), hits[rndpoint2].getY(), hits[rndpoint2].getZ(), hits[rndpoint2].getEnergy()};

      maybemodel = GetParamWithSample(sample1, sample2);

      double GetErrorMean=0;

      double sumvalue=0;
      double totalenergy=0;

      // For every point in data,
      // if point fits maybemodel with an error smaller than WIDTH,
      // add point to alsoinliers, compute totalenergy and compute sumvalue (it's used to test the inliers.)
      for (unsigned int i=0;i<unfittedPoints.size();i++) {
        if (unfittedPoints[i]==-1) {continue;}

        arrayD4 samplex = {hits[i].getX(), hits[i].getY(), hits[i].getZ(), hits[i].getEnergy()};

        if ( GetError(maybemodel,samplex)<pow(para->width,2) && ( hits[i].isTrackable() || hits[i].getY() > 40 ) ) {
          /*if(hits[i].getY()<4 || hits[i].getY()>60 || hits[i].getX()>120 || (hits[i].getX()<64 && hits[i].getX()>32 && hits[i].getY()>52) ){
            gone=true;
          }*/
          alsoinliers.push_back(i);
          arrayD4 samplex = {hits[i].getX(), hits[i].getY(), hits[i].getZ(), hits[i].getEnergy()};
          sumvalue += GetError(maybemodel, samplex)*hits[i].getEnergy();
          totalenergy+=hits[i].getEnergy();

        } //end if ( GetError(maybemodel,samplex)<width*width && hits[i].isFittable() && i!=-1 )
      } // end for (int i=0;i<unfittedPoints.size();i++)

      GetErrorMean=ClusterTest(sumvalue, totalenergy, alsoinliers); //è una buona scelta

      if  (alsoinliers.size()>(para->trsize)  && GetErrorMean<besterr && totalenergy>(para->threshold) ) {
        bestmodel=maybemodel;
        besterr=GetErrorMean;
        bestinliers=alsoinliers;
        trackenergy=totalenergy;
      } // end if (alsoinliers.size()>20 && GetErrorMean<besterr)

      iterations++;
    } //end while (iterations<loops)
    //cout << bestinliers.size() << "size" << endl;


    if( bestinliers.empty() ) {
      break;
    } //se non ho trovato tracce buone, fermati.

    double media = 0;
    double count = 0;
    for ( unsigned int i = 0; i < bestinliers.size(); i++ ) {
      media += hits[bestinliers[i]].getY();
    }

    media /= bestinliers.size();

    vector<int> temporary;

    if ( media > 34 ){
      for ( unsigned int i = 0; i < bestinliers.size(); i++ ) {
        if( hits[bestinliers[i]].getY() > 33.9 )
        temporary.push_back( bestinliers[i] );
      }
      bestinliers = temporary;
    }

    if ( media < 30 ){
      for ( unsigned int i = 0; i < bestinliers.size(); i++ ) {
        if( hits[bestinliers[i]].getY() < 30.1 )
        temporary.push_back( bestinliers[i] );
      }
      bestinliers = temporary;
    }

    for (unsigned int i=0; i<bestinliers.size();i++) {
      unfittedPoints[bestinliers[i]]=-1;
    } //setto i punti clusterizzati a -1.

    if(trackenergy!=0){
      energie.push_back(trackenergy/bestinliers.size());
    }
    besttrack.getPoints().insert(besttrack.getPoints().begin(),bestinliers.begin(),bestinliers.end());
    besttrack.setFittable(true);
    tracker->getLines().push_back(besttrack);

    trackcount++;
  } // end while (1)

      // end "clustering"

      return tracker;

}


arrayV2 GetParamWithSample(arrayD4 sample1, arrayD4 sample2) {
	TVector3 p1(sample1[0],sample1[1],sample1[2]);
	TVector3 p2(sample2[0],sample2[1],sample2[2]);
	TVector3 u = p2-p1;
	arrayV2 r = {p1, u.Unit()};
	return r;
}


Double_t GetError(arrayV2 model, arrayD4 p) {
	TVector3 a = {p[0]-model[0][0], p[1]-model[0][1], p[2]-model[0][2]};
	TVector3 H = model[0]+a.Dot(model[1])*model[1];
	TVector3 d = {p[0]-H[0], p[1]-H[1], (p[2]-H[2])*10};  //it was necessary to put this factor 10 due to measurement units (as I have set the precalibrator).

	return d.Mag2();
}

double ClusterTest(double& sumvalue, double& totalenergy, vector<int>& inliers) {
  double test=sumvalue/(totalenergy*inliers.size());
  return test;
}

#endif

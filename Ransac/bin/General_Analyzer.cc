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

int Ransac_Analyze(TString input_file = "")
{

    //Opening the input file.
    TFile *ifile = new TFile(ifname.Data(), "READ");
    if (ifile->IsZombie())
    {
        printf("Unable to open inputfile: %s", ifname.Data());
        return -1;
    }
    TTree *trackTree = (TTree *)ifile->Get("trackTree");
    if (physicalEventTree->IsZombie())
    {
        printf("Unable to open inputtree: ");
        return -1;
    }


}

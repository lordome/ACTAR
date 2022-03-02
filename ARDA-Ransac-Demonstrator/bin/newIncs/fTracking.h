#ifndef Tracking_h
#define Tracking_h

#include <iostream>
#include <vector>
#include <string>
#include <list>
#include "TFile.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TMinuit.h"
#include "TEnv.h"
#include "TTree.h"
#include "TVector3.h"
#include "TCanvas.h"
#include <array>
#include "TPolyLine3D.h"
#include "TTreeReader.h"
#include "TRandom3.h"
#include "cFittedLine.h"
#include "cFittedEvent.h"

#include "fParameters.h"

typedef array<Int_t, 2> arrayI2;
typedef array<Double_t, 3> arrayD3;
typedef array<Double_t, 4> arrayD4;
typedef array<TVector3, 2> arrayV2;

vector<int> indextofit;
arrayD4 pointstofit;

extern void FunctionForTrack(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);	// MINUIT function for track calculation
arrayV2 MinuitForTrack( cFittedLine<int> &track);		// MINUIT algorithm for track


void Track( cFittedEvent<int> *tracker){

  vector<cFittedLine<int>> vectracks(tracker->getLines().begin(), tracker->getLines().end()); //converting a list to a vector;

  //begin tracking
  for(unsigned int i=0; i<vectracks.size(); i++){
    indextofit.clear();
    if (!vectracks[i].isFittable()) {continue;}   //tracking only non-beam clusters.
    arrayV2 outV;
    outV= MinuitForTrack(vectracks[i]);

    vectracks[i].setBasepoint(outV[0]);
    vectracks[i].setDirection(outV[1]);
  }// end for(int i=0; i<vectracks.size(); i++)

  list<cFittedLine<int>> topush;
  topush.insert(topush.begin(), vectracks.begin(), vectracks.end()); //converting a vector to a list.
  tracker->getLines().clear();
  tracker->setLines(topush); //saving the tracked lines.
  // end tracking

  return;
}





extern void FunctionForTrack(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag) {
	// Since arguments are defined by the MINUIT algorith, nobody can change them.
	arrayV2 minuitmodel;
	minuitmodel[0] = {par[0],par[1],par[2]};
	minuitmodel[1] = {par[3],par[4],par[5]};
	Double_t err = 0;
	Double_t chisq = 0;

	for (unsigned int i=0;i<indextofit.size();i++) {
    pointstofit = {hits[indextofit[i]].getX(),hits[indextofit[i]].getY(),hits[indextofit[i]].getZ(),hits[indextofit[i]].getEnergy()};
		err = GetError(minuitmodel, pointstofit);
		chisq += err;
	}
	f = chisq;
}

arrayV2 MinuitForTrack(cFittedLine<int>& track) {
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
	gMinuit->SetPrintLevel(-1);	//-1:quiet, 0:normal, 1:verbose
	gMinuit->SetErrorDef(1.);

	// Set Paramters
  fParameters *par = new fParameters;
	Double_t maxX = par->numx;
	Double_t maxY = par->numy;
	Double_t maxZ = par->numz;
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

	outvectors[0]=Position;
	outvectors[1]=Direction;

  delete gMinuit;

  return outvectors;
}

//end track
#endif

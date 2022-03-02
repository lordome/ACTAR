#ifndef fVertexing_h
#define fVertexing_h

#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <list>

#include "TVector3.h"
#include "TTreeReader.h"

#include "cFittedLine.h"
#include "cVertex.h"
#include "cFittedEvent.h"

#include "fParameters.h"
#include "fClustering.h"

using namespace std;

extern void FunctionForVertex(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);	// MINUIT function for vertex calculation
TVector3 MinuitForVertex(cFittedEvent<int> &tracker , vector<int> besttracks);	// MINUIT algorithm for vertex


typedef array<Int_t, 2> arrayI2;
typedef array<Double_t, 3> arrayD3;
typedef array<Double_t, 4> arrayD4;
typedef array<TVector3, 2> arrayV2;

extern vector<cPhysicalHit> hits;
cFittedEvent<int> eTrack;

Double_t GetError(arrayV2 model, arrayD4 p);							// SQUARE! of the distance between the point and the line



void Vertex ( cFittedEvent<int>* tracker , vector<int>& besttracks ){

  fParameters *param = new fParameters();

  vector<cFittedLine<int>> vectracks(tracker->getLines().begin(), tracker->getLines().end()); //converting a list to a vector;

  arrayV2 model;
  vector<int> maybetracks; //indici delle possibili tracce da fittare
  besttracks.clear();
  maybetracks.clear();

  //in this for loop different cluster are comapred:
  //if a cluster has a pad which (squared) distance from the model of another track is smaller than
  //SqVertex_Width, then this two tracks are from the same event
  for ( unsigned int a=0; a<vectracks.size() ; a++ ) {
    if ( vectracks[a].isFittable()==false ) { continue; }

    //cout << a << "  :aTrack" << endl;
    maybetracks.clear();
    cFittedLine<int> aTrack=vectracks[a];
    maybetracks.push_back(a);

    model[1] = aTrack.getDirection();
    model[0] = aTrack.getBasepoint();

    for ( unsigned int i=0; i<vectracks.size() ; i++ ){

      if ( i==a || vectracks[i].isFittable()==false ) {continue;}
      for(auto &h : vectracks[i].getPoints()) {

        arrayD4 point = { hits[h].getX(),hits[h].getY(),hits[h].getZ(), hits[h].getEnergy() } ;

        if( GetError( model, point ) < param->SqVertex_Width ){

            maybetracks.push_back(i);
            break;                      //a single point is enough.
        } // end if (GetError(model, point)<SqVertex_Width)
      } // end for(auto &h: vectracks[i].getPoints())

      if (maybetracks.size()>besttracks.size() && maybetracks.size()>1){
        besttracks=maybetracks;
      }

    }  // end for (int i=0; i<vectracks.size() ; i++)
  }   // end for (int a=0; a<vectracks.size() ; a++)


  // Vertex (looking for a single vertex)
  TVector3 Ver = TVector3(0,0,0);
  Ver=MinuitForVertex( *tracker, besttracks );

  cVertex<int> vertex( Ver.x(), Ver.y(), Ver.z() );

  for( unsigned int i=0; i<besttracks.size(); i++ ){
    vertex.getTracks().push_back(vectracks[i]);
  }

  tracker->getVertex().push_back(vertex);
}



void FunctionForVertex(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag) {
	// Since arguments are defined by the MINUIT algorith, nobody can change them.
	arrayD4 p = {par[0], par[1], par[2], 1.};
	arrayV2 model;
	Double_t err = 0;
	Double_t chisq = 0;

	for (auto &i: eTrack.getLines()) {
    model[0] = i.getBasepoint();
    model[1] = i.getDirection();
		err = GetError(model, p); // si può pesare questo test con il chiquadro di ogni traccia: più una traccia è precisa, minore sarà il suo contributo all'errore finale. /GlobalTrack.GetChi2(i);
		chisq += err;
	}

	f = chisq;
}


TVector3 MinuitForVertex(cFittedEvent<int> &tracker, vector<int> besttracks) {
	// Initialization
  eTrack.getLines().clear();
  for (auto &i:besttracks){
    list<cFittedLine<int>>::iterator iterTrack = tracker.getLines().begin();
    advance(iterTrack,i);
    if (!iterTrack->isFittable()) {continue;}
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
  fParameters *para = new fParameters();

	Double_t maxX = para->numx;
	Double_t maxY = para->numy;
	Double_t maxZ = para->numz;
	gMinuit->DefineParameter(0, "Vx", 0, 0.01, -delta,maxX+delta);
	gMinuit->DefineParameter(1, "Vy", maxY/2., 0.01, -delta,maxY+delta);
	gMinuit->DefineParameter(2, "Vz", maxZ/2., 0.01, -delta,maxZ+delta);

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


#endif

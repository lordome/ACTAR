#ifndef fDrawing_h
#define fDrawing_h

#include <iostream>
#include <vector>
#include <string>
#include <list>

#include "TFile.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TPolyLine3D.h"

#include "cFittedLine.h"
#include "cVertex.h"
#include "cFittedEvent.h"


#include "fParameters.h"

using namespace std;

extern vector<cPhysicalHit> hits;

void DrawEvent( cFittedEvent<int>* tracker ){

  vector<TH3F*> histo ;

  vector<cFittedLine<int>> vectracks(tracker->getLines().begin(), tracker->getLines().end()); //converting a list to a vector;

  TCanvas *canvas = new TCanvas ("cani", "ciao", 1600,1200);
  canvas->Divide(3,2);

  //qui manca da inserire uno switch per la divisione del canvas.
  histo.emplace_back( new TH3F ( "histoall", "histoall", 64,0,128, 32,0,64 ,100,0,17 ) );
  for ( unsigned int i=0; i<hits.size(); i++ ) {
    histo[0]->Fill( hits[i].getX(), hits[i].getY(), hits[i].getZ() );
  }
  canvas->cd(1);
  histo[0]->Draw("lego");


  for( unsigned int i=0; i<vectracks.size() && i<6; i++ ){
    string title = "histo"+to_string( i );
    histo.emplace_back( new TH3F ( title.c_str(), title.c_str(), 64,0,128, 32,0,64 ,100,0,17 ) );
    vector<int> index1 ( vectracks[i].getPoints().begin(),vectracks[i].getPoints().end() );
    for (unsigned int j=0; j<index1.size(); j++) {
      histo[i+1]->Fill( hits[index1[j]].getX(), hits[index1[j]].getY(), hits[index1[j]].getZ() );
    }
    canvas->cd( i+2 );
    histo[i+1]->Draw( "lego" );

    if( vectracks[i].isFittable() ){  //now drawing line
      TPolyLine3D* drawObj = new TPolyLine3D();

      TVector3 base1= vectracks[i].getBasepoint() - vectracks[i].getDirection()*100;
      drawObj->SetNextPoint( base1.x(), base1.y(), base1.z() );

      TVector3 bn = vectracks[i].getBasepoint() + vectracks[i].getDirection()*100;

      drawObj->SetNextPoint( bn.x(), bn.y(), bn.z() );

      drawObj->Draw( "same" );

    }
  }

  //end DrawEvent


}

#endif

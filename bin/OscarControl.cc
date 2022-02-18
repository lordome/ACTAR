#include <iostream>
#include <vector>
#include <array>
#include <list>

#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TTreeReader.h"
#include "TH2D.h"

#include "/home/lorenzo/Desktop/ardatest/ARDA/inc/cPhysicalEvent.h"
#include "/home/lorenzo/Desktop/ardatest/ARDA/inc/cLookupTable.h"
#include "/home/lorenzo/Desktop/ardatest/ARDA/inc/cRawEvent.h"
#include "/home/lorenzo/Desktop/ardatest/ARDA/inc/cFittedLine.h"
#include "/home/lorenzo/Desktop/ardatest/ARDA/inc/cVertex.h"
#include "/home/lorenzo/Desktop/ardatest/ARDA/inc/cFittedEvent.h"


#include "fParameters.h"
#include "fClustering.h"
#include "fTracking.h"
#include "fVertexing.h"
#include "fDrawing.h"

#include "/home/lorenzo/Desktop/ardatest/LinkDef.h"

using namespace std;

typedef array<TVector3, 2> arrayV2;

vector<cPhysicalHit> hits;

cFittedEvent<int> *Cluster( cFittedEvent<int>* tracker, list<cPhysicalHit>& hits,  double& trackcount, double evNumber );
void Track ( cFittedEvent<int>* tracker );
void Vertex ( cFittedEvent<int>* tracker , vector<int>& besttracks );
void DrawEvent ( cFittedEvent<int>* tracker );

int GetRow( int gid);

int OscarControl (TString ifname="") {

  int toanalyse=0; //Proviamo a semplificare per un primo momento. Questo ci dice il numero di evento da analizzare.
  cout << "Set event num:" ;
  cin >> toanalyse ;

  TFile *ifile = new TFile(ifname.Data(), "READ");   //Opening the input file.
  if (ifile->IsZombie()){
    printf("Unable to open inputfile: %s", ifname.Data());
    return -1;
  }

  TTree *physicalEventTree= (TTree*) ifile->Get("physicalEventTree");
  if (physicalEventTree->IsZombie()){
    printf("Unable to open inputtree: " );
    return -1;
  }


  TFile fout("OscarControlFORSE.root", "UPDATE");
  if (fout.IsZombie()) {
    cout << "Output file creation failed" << endl;
    return 0;
  }
  fout.cd();

  vector<TH2D*> vecHisto ;

/*  TTree outTree ( "OscarHistos", "OscarHistos" );
  outTree.SetDirectory(&fout);
  outTree.AutoSave();*/

  cFittedEvent<int> *tracker = nullptr ;

  TTreeReader rdr(physicalEventTree);
  TTreeReaderValue<cPhysicalEvent> event(rdr, "event"); //reading input file

  TH2D *histo = new  TH2D ( "test", "test", 100, -2000, 3000, 200, -1000, 10000 );
  for( unsigned int i=0; i<80; ++i){
    static string hTitle;

    hTitle = "Canale" + to_string(i);
    vecHisto.emplace_back( new TH2D ( hTitle.c_str(), hTitle.c_str(), 2000, -3000, 20000, 2000, -3000, 20000 ) ) ;
  }

  int nent = rdr.GetEntries(false);

  vector<double> vHitCal1;

  while ( rdr.Next() ) {

    vHitCal1.clear();
    vHitCal1.reserve(180);
    for(unsigned int i = 0; i<180; i++){
      vHitCal1.push_back(0.);
    }

    if (rdr.GetCurrentEntry() % 1000 == 0) {
      cout << "\rConverting entry " << rdr.GetCurrentEntry() << " of " << nent << flush;
    }

    list<cPhysicalHit> hitslist = event->getHits();
    double calLow = 0;
    double calHigh = 0;
    for( auto & i: hitslist){
      //proviamo il silicio numero 53
      int row = GetRow ( i.getGlobalChannelId() );
      if ( row < 0    ) continue;
      if ( row > 0 && row < 180 ){
        vHitCal1[ row ] += i.getEnergy();
      }

      //if ( row == 30  ) calLow  += i.getEnergy();
      //if ( row == 130 ) calHigh += i.getEnergy();
      /*if ( i.getGlobalChannelId() == 21113 ) calLow  += i.getEnergy();
      if ( i.getGlobalChannelId() == 21147 ) calHigh += i.getEnergy();*/

    }

    //cout << "CALLOW::CALHIGH" << calLow << "::" << calHigh << endl;
    for( unsigned int i=0; i<80; ++i){

      vecHisto[i] -> Fill(vHitCal1[i], vHitCal1[i+100]);
      vecHisto[i] -> GetXaxis() -> SetTitle( to_string(i).c_str() );
      vecHisto[i] -> GetYaxis() -> SetTitle( to_string(i+100.0).c_str() );
    }

  }// end while (rdr.Next())
  TCanvas *c = new TCanvas ( "Canvas", "Canvas", 800, 600);
  for( unsigned int i=0; i<80; ++i){

    vecHisto[i] -> Write();

  }

  return 0;
}


int GetRow( int gid ){

   if ( gid>21000 ) {

     if( gid>=21000 && gid<21011 ){
       int t = gid-21000+1;
       return t;
     }
     if( gid>21011 && gid<21022 ){
       int t = gid-21000;
       return t;
     }
     if( gid>21022 && gid<21034 ){
       int t = gid-21000-1;
       return t;
     }
     if( gid>=21100 && gid<21111 ){
       int t = gid-21100+33;
       return t;
     }
     if( gid>21111 && gid<21122){
       int t = gid-21112+44;
       return t;
     }
     if( gid>21122 && gid<21134){
       int t = gid-21123+54;
       return t;
     }
     if( gid>=21200 && gid<21211){
       int t = gid-21200+65;
       return t;
     }
     if( gid>=21212 && gid<21217){
       int t = gid-21212+76;
       return t;
    }

    if( gid>=21034 && gid<21045 ){
      int t2 = gid-21034+1+100;
      return t2;
    }
    if( gid>21045 && gid<21056 ){
      int t2 = gid-21046+12+100;
      return t2;
    }
    if( gid>21056 && gid<21068 ){
      int t2 = gid-21057+22+100;
      return t2;
    }
    if( gid>=21134 && gid<21145 ){
      int t2 = gid-21134+33+100;
      return t2;
    }
    if( gid>21145 && gid<21156){
      int t2 = gid-21146+44+100;
      return t2;
    }
    if( gid>21156 && gid<21168){
      int t2 = gid-21157+54+100;
      return t2;
    }
    if( gid>=21216 && gid<21222){
      int t2 = gid-21217+65+100;
      return t2;
    }
    if( gid>=21222 && gid<21234){
      int t2 = gid-21223+70+100;
     return t2;
   }
 }else{
   return -1;
 }

 return -1;
}




//scrittura su txt semplice + check con istogramma.
/*TH3F *h2 = new TH3F ("histo2", "histo2", 64,0,128, 32,0,64,100,0,17);

ofstream ofile ("testo.txt");
for (auto &a : besttracks) {
  vector <int> pts(vectracks[a].getPoints().begin(),vectracks[a].getPoints().end());
   for ( auto & a: pts){
     ofile << hits[a].getX() << "  " << hits[a].getY() << "  " << hits[a].getZ() << "  " << hits[a].getEnergy() << "  " << endl;
     h2->Fill(hits[a].getX(), hits[a].getY(), hits[a].getZ());
   }
 }
 h2->Draw("lego");*/

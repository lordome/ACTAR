#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TTreeReader.h"
#include "TH1F.h"
#include "TH2F.h"


#include "cFittedLine.h"
#include "cVertex.h"
#include "cFittedEvent.h"


#include "fParameters.h"

using namespace std;
bool InOscar( int gid );

void DrawOscar ( list<cPhysicalHit>& hitslist ){

  //considering only OSCAR channels.
  vector<cPhysicalHit> vHits = vector<cPhysicalHit>( hitslist.begin(), hitslist.end() );


  map<int, double> OscarEnergies;
  map<int, double> OscarCal2Energies;

  TH1F *histo = new TH1F(" ", " ", 200, -500, 10000);
  TH1F *histocal2 = new TH1F(" histocal2 ", " histocal2 ", 200, -500, 10000);


  for ( unsigned int i=0; i<hits.size(); i++ ){
    int gid = hits[i].getGlobalChannelId();
    /* if ( gid>21000 ) {

       if( gid>=21000 && gid<21011 ){
         int t = gid-21000+1;
         OscarEnergies[t]+=hits[i].getEnergy();
       }
       if( gid>21011 && gid<21022 ){
         int t = gid-21000;
         OscarEnergies[t]+=hits[i].getEnergy();
       }
       if( gid>21022 && gid<21034 ){
         int t = gid-21000-1;
         OscarEnergies[t]+=hits[i].getEnergy();
       }
       if( gid>=21100 && gid<21111 ){
         int t = gid-21100+33;
         OscarEnergies[t]+=hits[i].getEnergy();
       }
       if( gid>21111 && gid<21122){
         int t = gid-21112+44;
         OscarEnergies[t] = hits[i].getEnergy();
       }
       if( gid>21122 && gid<21134){
         int t = gid-21123+54;
         OscarEnergies[t] = hits[i].getEnergy();
       }
       if( gid>=21200 && gid<21211){
         int t = gid-21200+65;
         OscarEnergies[t] = hits[i].getEnergy();
       }
       if( gid>=21212 && gid<21217){
         int t = gid-21212+76;
        OscarEnergies[t] = hits[i].getEnergy();
      }

    }

    if ( gid>21000 ) {

      if( gid>=21034 && gid<21045 ){
        int t2 = gid-21034+1;
        OscarCal2Energies[t2]+=hits[i].getEnergy();
      }
      if( gid>21045 && gid<21056 ){
        int t2 = gid-21046+12;
        OscarCal2Energies[t2]+=hits[i].getEnergy();
      }
      if( gid>21056 && gid<21068 ){
        int t2 = gid-21057+22;
        OscarCal2Energies[t2]+=hits[i].getEnergy();
      }
      if( gid>=21134 && gid<21145 ){
        int t2 = gid-21134+33;
        OscarCal2Energies[t2]+=hits[i].getEnergy();
      }
      if( gid>21145 && gid<21156){
        int t2 = gid-21146+44;
        OscarCal2Energies[t2] = hits[i].getEnergy();
      }
      if( gid>21156 && gid<21168){
        int t2 = gid-21157+54;
        OscarCal2Energies[t2] = hits[i].getEnergy();
      }
      if( gid>=21216 && gid<21222){
        int t2 = gid-21217+65;
        OscarCal2Energies[t2] = hits[i].getEnergy();
      }
      if( gid>=21222 && gid<21234){
        int t2 = gid-21223+70;
       OscarCal2Energies[t2] = hits[i].getEnergy();
     }
   }
*/
  if ( InOscar( gid ) ) {
    if ( hits[i].getEnergy() > 2000 ){
      cout << "Gid == " << gid << endl;
    }
    histo->Fill( hits[i].getEnergy() );
  }
    if( gid>=21034 && gid<21045 ){
      int t2 = gid-21034+1;
      OscarEnergies[t2]+=hits[i].getEnergy();
    }
    if( gid>21045 && gid<21056 ){
      int t2 = gid-21046+12;
      OscarEnergies[t2]+=hits[i].getEnergy();
    }
    if( gid>21056 && gid<21068 ){
      int t2 = gid-21057+22;
      OscarEnergies[t2]+=hits[i].getEnergy();
    }
    if( gid>=21134 && gid<21145 ){
      int t2 = gid-21134+33;
      OscarEnergies[t2]+=hits[i].getEnergy();
    }
    if( gid>21145 && gid<21156){
      int t2 = gid-21146+44;
      OscarEnergies[t2] = hits[i].getEnergy();
    }
    if( gid>21156 && gid<21168){
      int t2 = gid-21157+54;
      OscarEnergies[t2] = hits[i].getEnergy();
    }
    if( gid>=21200 && gid<21211){
      int t = gid-21200+65;
      OscarEnergies[t] = hits[i].getEnergy();
    }
    if( gid>=21212 && gid<21217){
      int t = gid-21212+76;
      OscarEnergies[t] = hits[i].getEnergy();
   }

  }

  TH2F *histo2D = new TH2F ( "bidim", "bidim", 12, 0, 12, 7, 0, 7 );
  TH2F *histo2Dcal2 = new TH2F ( "bidimcal2", "bidimcal2", 12, 0, 12, 7, 0, 7 );

  for( auto& it: OscarEnergies ){
    //histo->Fill(it.second);
    if( it.first <= 10 ){
      histo2D->Fill( it.first, 6., it.second );
      continue;
    }
    if( it.first <= 22 ){
      histo2D->Fill( it.first-11., 5., it.second );
      continue;
    }
    if( it.first <= 34 ){
      histo2D->Fill( it.first-23., 4., it.second );
      continue;
    }
    if( it.first <= 46 ){
      histo2D->Fill( it.first-35., 3., it.second );
      continue;
    }
    if( it.first <= 58 ){
      histo2D->Fill( it.first-47., 2., it.second );
      continue;
    }
    if( it.first <= 70 ){
      histo2D->Fill( it.first-59., 1., it.second );
      continue;
    }
    if( it.first <= 80 ){
      histo2D->Fill( it.first-70., 0., it.second );
      continue;
    }

  }

  /*for( auto& i: OscarCal2Energies ){
    histocal2->Fill(it.second);
    if(it.second > 2000){
      cout << it.first << "  iFirst" << endl;
    }
    if( it.first <= 10 ){
      histo2Dcal2->Fill( it.first, 6., it.second );
      continue;
    }
    if( it.first <= 22 ){
      histo2Dcal2->Fill( it.first-11., 5., it.second );
      continue;
    }
    if( it.first <= 34 ){
      histo2Dcal2->Fill( it.first-23., 4., it.second );
      continue;
    }
    if( it.first <= 46 ){
      histo2Dcal2->Fill( it.first-35., 3., it.second );
      continue;
    }
    if( it.first <= 58 ){
      histo2Dcal2->Fill( it.first-47., 2., it.second );
      continue;
    }
    if( it.first <= 70 ){
      histo2Dcal2->Fill( it.first-59., 1., it.second );
      continue;
    }
    if( it.first <= 80 ){
      histo2Dcal2->Fill( it.first-70., 0., it.second );
      continue;
    }

  }*/

  TCanvas *c2d = new TCanvas ( "c2d", "c2d", 800,600);
  histo2D->SetTitle("Beware: the beam 'enters' from the right side");
  histo2D->Draw("COLZ");

  /*TCanvas *c2dcal2 = new TCanvas ( "c2dcal2", "c2dcal2", 800,600);
  histo2Dcal2->SetTitle("CAL2!!: Beware: the beam 'enters' from the right side");
  histo2Dcal2->Draw("COLZ");*/

  TCanvas *c1d = new TCanvas ( "c1d", "c1d", 800,600);
  histo->SetTitle("Oscar energies");
  histo->Draw();

  /*TCanvas *c1dcal2 = new TCanvas ( "cal2hi", "cal2histo", 800,600);
  histocal2->SetTitle("Oscar energies cal 2");
  histocal2->Draw();*/



}


bool InOscar( int gid ){

  if( ( gid >  21000 ) && ( gid <  21234 ) ) {
      return true;
  }else{
      return false;
  }

}

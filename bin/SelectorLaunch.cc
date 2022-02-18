#include <iostream>
#include "TFile.h"
#include "TH2F.h"
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
#include "cPhysicalHit.h"
#include "cPhysicalEvent.h"
#include "cFittedLine.h"
#include "cFittedEvent.h"
#include <utility>
#include <vector>
#include <map>
#include <string>
#include <list>
#include <time.h>
#include "TSystem.h"
#include "TROOT.h"

#include "/home/lorenzo/Desktop/fromNuclex1/trackingTest/LinkDef.h"

using namespace std;

int SelectorLaunch (TString ifname = "Raw_ROOTFiles/411Cal.root"){

  TFile *ifile = new TFile(ifname.Data(), "READ");
  if (ifile->IsZombie()){
    printf("Unable to open inputfile: %s", ifname.Data());
    return -1;
  }
  TTree *physicalEventTree= (TTree*) ifile->Get("physicalEventTree");
  if (physicalEventTree->IsZombie()){
    printf("Unable to open inputtree: " );
    return -1;
  }

  cout << "Begin Process" << endl;

  physicalEventTree->Process( "trackingFINAL.cpp" );

  cout << "end Process" << endl;


  return 0;
}

//////// Questa macro deve essere runnata in una sessione ROOT con caricati le librerie.
///////  Funziona lo stesso, da solo qualche avviso di mancanza di dizionari.
/////// Questa macro semplicamente disegna un evento 2D o 3D. Alcune righe di codice sono commentate. Queste permettono di dividere il canvas.
/////// Cambiando il parametro b, è possibile disegnare un numero N di eventi ( qui come esempio 40 = 8x5 eventi ). L.D.

#include <iostream>
#include "TFile.h"
#include "TH3F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TEnv.h"
#include "TTree.h"
#include "TTreeReader.h"
#include "TVector3.h"
#include <utility>
#include <map>
#include <string>
using namespace std;

int SimpleDrawFromTree()
{

  TString ifname = "/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/root_e796/outputPrecalibrator.root";

  int a = 0;
  int b = 0;
  cout << "Start?" << endl;
  cin >> a;
  // a = 1746;
  // b=a+1;
  b = a + 1;

  TCanvas *canvas = new TCanvas("canvas", "canvas", 800, 600);
  // canvas->Divide(8,5);

  TTree *physicalEventTree = 0;

  TFile *ifile = new TFile(ifname.Data(), "READ");
  if (ifile->IsZombie())
  {
    printf("Unable to open inputfile: %s", ifname.Data());
    return -1;
  }

  ifile->GetObject("physicalEventTree", physicalEventTree);

  //canvas->cd(1);

  TH2F *h2 = new TH2F("histo2D", "histo2D", 128, 0, 128, 128, 0, 128);

  while (1)
  {
    cout << "while.. " << endl;
    a = a + 1;
    b = b + 1;

    for (int i = a; i < b; i++)
    {

      string stro = to_string(i);

      char c[stro.size() + 1];
      strcpy(c, stro.c_str());

      string entry = "Entry$==" + stro;

      char ent[entry.size() + 1];
      strcpy(ent, entry.c_str());

      physicalEventTree->Draw("hits.y:hits.x>>h(128, 0, 128, 128, 0, 128)", "hits.energy",  "colz");
      // gSystem->ProcessEvents();
      canvas->WaitPrimitive();
      canvas->Update();
    }

    //physicalEventTree->Draw("hits.z:hits.y:hits.x >> histo (64,0,128, 32,0,64, 400,0,16)",ent, "LEGO");

    /*TH3F *h2  new TH3F ("histo", "histo" , 64,0,128, 32,0,64, 100,0,16);

    */
  }

  return 1;
}

#include <iostream>
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStyle.h"

using namespace std;

void rootlogon()
{
  gSystem->Load("loader_C.so");
  gROOT->ProcessLine(".x ../callRootlogon.C");
}

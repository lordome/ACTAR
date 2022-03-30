#include <iostream>
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStyle.h"

using namespace std;

void rootlogon()
{
  gROOT->ProcessLine(".x ../callRootlogon.C");
}

#include "TROOT.h"

using namespace std;

void rootlogon()
{
  gROOT->ProcessLine(".x ../../callRootlogon.C");
}

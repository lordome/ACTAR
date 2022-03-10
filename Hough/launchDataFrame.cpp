#include "TProof.h"
#include "TSelector.h"
#include "TChain.h"
#include "TProofChain.h"
//#include "RDataFrame.h"

void runDataFrame()
{
    ROOT::EnableImplicitMT();                    // Tell ROOT you want to go parallel
    ROOT::RDataFrame d("physicalEventTree", "../../ROOTFiles/outputPrecalibrator_run140.root"); // Interface to TTree and TChain
}

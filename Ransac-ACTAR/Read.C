#include "TSystem.h"
#include "TROOT.h"
#include "TStyle.h"

int Read(int runf, int runl)
{
	/***** for ganp373 ********/
	gSystem->SetFlagsOpt("-O2 -std=c++11");

	gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/inc/\"");
	gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/Ganil_Actar/MFMlib-JDF/install/include/mfmlib/\"");
	gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/utils/reduction/include/\"");
	gSystem->AddIncludePath("-I\"/home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/utils/GFFT/src/\"");

	gROOT->ProcessLine(".L /home/lorenzo/Desktop/Ganil_Actar/MFMlib-JDF/install/lib/libmfm.so");
	gROOT->ProcessLine(".L /home/lorenzo/Desktop/Ganil_Actar/ACTAR_ANALYSIS_LIGHT/libACTARshared.so");
	// gROOT->ProcessLine(".L MTrack.cpp+g");
	// gROOT->ProcessLine(".L Utils.cpp+g");
	// gROOT->ProcessLine(".L TreeRead.C+g");
	gROOT->ProcessLine(".L TreeRead.C+g");

	gStyle->SetPadRightMargin(0.15);
	TString S;
	S.Form("TreeRead(%d,%d)", runf, runl);
	gROOT->ProcessLine(S.Data());

	return 1;
	/***** for ganp562 ********/
	// 	gSystem->SetFlagsOpt("-O2 -std=c++11");
	// 	gSystem->AddIncludePath("-I\"/home/roger/Desktop/ANALYSIS/E780/ACTAR_ANALYSIS_LIGHT/inc/\"");
	// 	gSystem->AddIncludePath("-I\"/home/roger/Desktop/ANALYSIS/E780/ACTAR_ANALYSIS_LIGHT/libMFM/\"");
	//      gSystem->AddIncludePath("-I\"/home/eactar/Programs/reduction/include/\"");
	// 	gSystem->AddIncludePath("-I\"/home/roger/Desktop/ANALYSIS/E780/ACTAR_ANALYSIS_LIGHT/utils/GFFT/include\"");
	// 	gROOT->ProcessLine(".L /home/roger/Desktop/ANALYSIS/E780/ACTAR_ANALYSIS_LIGHT/libMFM/libMFM.so");
	// 	gROOT->ProcessLine(".L /home/roger/Desktop/ANALYSIS/E780/ACTAR_ANALYSIS_LIGHT/libACTARshared.so");
	// 	gROOT->ProcessLine(".L MTrack.cpp+g");
	// 	gROOT->ProcessLine(".L Utils.cpp+g");
	// 	gROOT->ProcessLine(".L TreeRead.C++g");
	// 	gStyle->SetPadRightMargin(0.15);
	// 	TString S;
	// 	S.Form("TreeRead(%d,%d)",runf,runl);
	// 	gROOT->ProcessLine(S.Data());
}

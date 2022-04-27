int Merge(int runf, int runl)
{
	gSystem->SetFlagsOpt("-O2 -std=c++11");
	gSystem->AddIncludePath("-I\"../inc/\"");
	gROOT->ProcessLine(".L ../libACTARshared.so");
	gROOT->ProcessLine(".L TreeMerge.C++g");
	TString S;
	for(int run=runf;run<=runl;run++)
	{
		S.Form("TreeMerge(%d)",run);
		gROOT->ProcessLine(S.Data());
	}
}

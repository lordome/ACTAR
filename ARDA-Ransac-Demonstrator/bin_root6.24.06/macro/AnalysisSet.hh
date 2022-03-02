#ifndef ANALYSISSET_HH
#define ANALYSISSET_HH 1


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "TFile.h"
#include "TTree.h"

class TFile;
class TTree;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class AnalysisSet {
	public:
		AnalysisSet();
		~AnalysisSet();
		
		TFile *FileOpen(TString filename);
		TFile *CreateNewFile(TString name, TString title, Int_t verbose=0);
		
		Bool_t CheckOpenTFile(TFile *file);
		Bool_t CheckOpenTTree(TTree *file);

		void PrintErrorMsg(TString pos, Int_t verbose=0, Bool_t flag=kTRUE);
		void SetErrorComment(TString comment) {fComment.push_back(comment);}
		void ClearErrorComment() {fComment.clear(); vector<TString>().swap(fComment);}
	
	private:
		vector<TString> fComment;
		TString fFileName;
		TString fTreeName;
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#endif

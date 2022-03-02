///////////////////////////////////////////////////////////////////////////////
//
//                            AnalysisSet.cc
//
//  This is a file for class of basic analysis.
//  Open a TFile of TTree, or display an error message.
//  It can be used for any analysis.
//


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Include File
// For C++
#include <stdio.h>
#include <iostream>

// For ROOT
#include "TFile.h"
#include "TSystem.h"

// For
#include "AnalysisSet.hh"

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AnalysisSet::AnalysisSet() {}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AnalysisSet::~AnalysisSet() {}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AnalysisSet::PrintErrorMsg(TString pos, Int_t verbose, Bool_t flag) {
	if (verbose!=-1) {
		cout<<endl;
		if (flag) {
			cout<<"++++  Error Msg ("<<pos<<")  ++++"<<endl;
		} else {
			cout<<"++++  Warning Msg ("<<pos<<")  ++++"<<endl;
		}
		cout<<"++"<<endl;
		for (Int_t i=0;i<(Int_t)fComment.size();i++) {
			cout<<"+  "<<fComment[i]<<endl;
		}
		cout<<"++"<<endl;
		if (flag) {
			cout<<"++++  Error Msg ("<<pos<<")  ++++"<<endl;
		} else {
			cout<<"++++  Warning Msg ("<<pos<<")  ++++"<<endl;
		}
		cout<<endl;
	}

	ClearErrorComment();
	if (flag) exit(1);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TFile *AnalysisSet::FileOpen(TString filename) {
	fFileName = filename;
	FileStat_t info;
	TFile *file;
	if (gSystem->GetPathInfo(filename.Data(), info)!=0) {
		file = new TFile();
		return file;
	}
	file = new TFile(filename.Data(), "read");


	return file;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TFile *AnalysisSet::CreateNewFile(TString name, TString title, Int_t verbose) {
	fTreeName = name;
	if (verbose!=-1) {
		FileStat_t info;
		while (gSystem->GetPathInfo(name.Data(), info)==0) {
			TString str;
			SetErrorComment(Form("File \"%s\" is exist.", name.Data()));
			SetErrorComment("Do you want to overwrite it ?");
			PrintErrorMsg("CreateNewFile", verbose, kFALSE);
			ClearErrorComment();
			cout<<" Overwrite [y/n/q/?] -->";cin>>str;
			str.ToLower();
			if (str=="y"||str=="yes") {
				cout<<" Recreate the file ("<<name<<")."<<endl<<endl;
				break;
			} else if (str=="n"||str=="no") {
				cout<<endl;
				cout<<" Please enter the new file name :"<<endl;
				cout<<" If you enter a file name that does not exist,"<<endl;
				cout<<" the program will run immediately."<<endl;
				cout<<" Please enter with an extension also (ex. test.root)"<<endl;
				cout<<" File Name --->";cin>>name;
				TString str2 = name;
				str2.ToLower();
				if (str2=="q"||str2=="quit") {
					cout<<endl;
					cout<<" Since overwrite was rejected, it exit."<<endl;
					cout<<endl;
					exit(1);
				}
			} else if (str=="q"||str=="quit") {
				cout<<endl;
				cout<<" Since overwrite was rejected, it exit."<<endl;
				cout<<endl;
				exit(1);
			} else {
				cout<<" Please enter the correct character."<<endl;
				cout<<" [y/yes]  : Recreate the File."<<endl;
				cout<<" [n/no]   : You can change the file name."<<endl;
				cout<<" [q/quit] : quit the program."<<endl;
				cout<<endl;
			}
		}
	}

	TFile *file = new TFile(name.Data(),"recreate",title.Data());

	return file;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Bool_t AnalysisSet::CheckOpenTFile(TFile *file) {
	if (file->IsOpen()) return kTRUE;

	SetErrorComment(Form("File \"%s\" does not exist",fFileName.Data()));
	SetErrorComment("Please check the file name.");
	PrintErrorMsg("File does't exist", 0, kFALSE);
	return kFALSE;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Bool_t AnalysisSet::CheckOpenTTree(TTree *tree) {
	if (tree==nullptr) {
		SetErrorComment(Form("There is no tree \"%s\" required for analysis.", fTreeName.Data()));
		SetErrorComment("Please change the input file.");
		SetErrorComment("If you need a more help,");
		SetErrorComment("please see the item of \"Before Use\" in the source code.");
		PrintErrorMsg("TTree does't exist", 0, kFALSE);
		return kFALSE;
	}
	return kTRUE;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

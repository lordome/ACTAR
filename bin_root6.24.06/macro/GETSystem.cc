///////////////////////////////////////////////////////////////////////////////
//
//                            GETSystem.cc
//
//  This is a file of class for GET electoronics.
//  It is mainly used for converting the Maya and DSSD detector 
//  from the channel of GET system to the coordinate system of the real space.
//



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Include Files
// For C++
#include <algorithm>
#include <array>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>
#include <numeric>
// For ROOT
#include "TSystem.h"
#include "TRandom3.h"
#include "TVector3.h"
// For
#include "GETSystem.hh"
#include "AnalysisSet.hh"

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//=================================================================//
//-+-+-+-+-+-+-+-+-+-+- GET Electronics Class -+-+-+-+-+-+-+-+-+-+-//
//=================================================================//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GETData::GETData() {
	fCoBo.clear();
	fAsAd.clear();
	fAGet.clear();
	fChan.clear();
	fRow.clear();
	fCol.clear();
	fFitA.clear();
	fFitB.clear();
	fChanPos.clear();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GETData::~GETData() {
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GETData::ReadTable(TString filename) {
	AnalysisSet *ana = new AnalysisSet();
	ifstream fin;
	fin.open(filename.Data(), ios::in);
	if (fin.fail()) {
		ana->SetErrorComment(Form("Data Table \"%s\" does not exist",filename.Data()));
		ana->PrintErrorMsg("Read Table Error", kFALSE);
	}
	string str;
	string tmp1;
	string tmp2;
	Int_t Column = 0;
	Int_t Column1 = 0;
	Int_t Column2 = 0;
	getline(fin,str);
	istringstream stream1(str);
	istringstream stream2(str);
	while (getline(stream2, tmp2, ' ')) Column2++;
	while (getline(stream1, tmp1, '\t')) Column1++;
	Column = max(Column1, Column2);

	Int_t pad, cobo, asad, aget, chan, row, col;
	Double_t a, b;
	if (Column==7) {
		fin>>pad>>cobo>>asad>>aget>>chan>>row>>col;
		while (!fin.eof()) {
			if (row!=-1&&col!=-1) {
				fPad.push_back(pad);
				fCoBo.push_back(cobo);
				fAsAd.push_back(asad);
				fAGet.push_back(aget);
				fChan.push_back(chan);
				fRow.push_back(row);
				fCol.push_back(col);
				fFitA.push_back(0);
				fFitB.push_back(0);
			}
			fin>>pad>>cobo>>asad>>aget>>chan>>row>>col;
		}
	} else if (Column==9) {
		fin>>pad>>cobo>>asad>>aget>>chan>>row>>col>>a>>b;
		while (!fin.eof()) {
			if (row!=-1&&col!=-1) {
				fPad.push_back(pad);
				fCoBo.push_back(cobo);
				fAsAd.push_back(asad);
				fAGet.push_back(aget);
				fChan.push_back(chan);
				fRow.push_back(row);
				fCol.push_back(col);
				fFitA.push_back(a);
				fFitB.push_back(b);
			}
			fin>>pad>>cobo>>asad>>aget>>chan>>row>>col>>a>>b;
		}
	} else {
		ana->SetErrorComment(Form("The format of data table \"%s\" does not match.",filename.Data()));
		ana->SetErrorComment("GET system format =>");
		ana->SetErrorComment("pad	cobo	asad	aget	chan	row	col");
		ana->SetErrorComment("Energy Calibration format =>");
		ana->SetErrorComment("pad	cobo	asad	aget	chan	row	col	FitA	FitB");
		ana->PrintErrorMsg("Read Table Error", kTRUE);
	}
		
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

		
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Bool_t GETData::CheckGETSystem(Int_t cobo, Int_t asad, Int_t aget, Int_t chan) {
	for (auto i=0;i<Size();i++) {
		if (cobo==GetCoBo(i)&&asad==GetAsAd(i)&&aget==GetAGet(i)&&chan==GetChan(i)) return true;
	}
	return false;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GETData::AddChanPos(Int_t cobo, Int_t asad, Int_t aget, Int_t chan, Double_t x, Double_t y, Double_t z) {
	array<Int_t, 4> arr = {cobo, asad, aget, chan};
	TVector3 xyz(x,y,z);
	
	fChanPos[arr] = xyz;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t GETData::GetRow(Int_t cobo, Int_t asad, Int_t aget, Int_t chan) {
	for (auto i=0;i<Size();i++) {
		if (cobo==fCoBo[i]&&asad==fAsAd[i]&&aget==fAGet[i]&&chan==fChan[i]) return fRow[i];
	}
	return -1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t GETData::GetCol(Int_t cobo, Int_t asad, Int_t aget, Int_t chan) {
	for (auto i=0;i<Size();i++) {
		if (cobo==fCoBo[i]&&asad==fAsAd[i]&&aget==fAGet[i]&&chan==fChan[i]) return fCol[i];
	}
	return -1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t GETData::GetFitA(Int_t cobo, Int_t asad, Int_t aget, Int_t chan) {
	for (auto i=0;i<Size();i++) {
		if (cobo==fCoBo[i]&&asad==fAsAd[i]&&aget==fAGet[i]&&chan==fChan[i]) return fFitA[i];
	}
	return -1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t GETData::GetFitB(Int_t cobo, Int_t asad, Int_t aget, Int_t chan) {
	for (auto i=0;i<Size();i++) {
		if (cobo==fCoBo[i]&&asad==fAsAd[i]&&aget==fAGet[i]&&chan==fChan[i]) return fFitB[i];
	}
	return -1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TVector3 GETData::GetPosXYZ(Int_t cobo, Int_t asad, Int_t aget, Int_t chan) {
	array<Int_t, 4> arr = {cobo, asad, aget, chan};
	TVector3 xyz = fChanPos[arr];

	return xyz;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t GETData::GetPosX(Int_t cobo, Int_t asad, Int_t aget, Int_t chan) {
	array<Int_t, 4> arr = {cobo, asad, aget, chan};
	TVector3 xyz = fChanPos[arr];

	return xyz.x();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t GETData::GetPosY(Int_t cobo, Int_t asad, Int_t aget, Int_t chan) {
	array<Int_t, 4> arr = {cobo, asad, aget, chan};
	TVector3 xyz = fChanPos[arr];

	return xyz.y();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t GETData::GetPosZ(Int_t cobo, Int_t asad, Int_t aget, Int_t chan) {
	array<Int_t, 4> arr = {cobo, asad, aget, chan};
	TVector3 xyz = fChanPos[arr];

	return xyz.z();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


///////////////////////////////////////////////////////////////////////////////
//
//                            DataSet.cc
//
//  This is a file for classes of data set, which is used for TTree.
//  There are five classes (PadData, MayaData, DSSDData, TrackData, and VertexData).
//
//  PadData       : for Pad detector class
//  OrgEvent      : Original event number
//  Event         : Event number of current TTree
//  X             : X-value of pad
//  Y             : Y-value of pad        
//  Z             : Z-value of pad        
//  Energy        : Energy of one pad
//  Distance      : Distance from vertex
//  Fpoint        : First point of track
//  Epoint        : End point of track
//  TrackNumber   : Track number
//
//  MayaData      : for Maya detector class
//  OrgEvent      : Original event number
//  Event         : Event number of current TTree
//  X             : X-value of pad
//  Y             : Y-value of pad        
//  Z             : Z-value of pad        
//  Energy        : Energy of one pad
//  Time          : Time 
//
//  DSSDData      : for DSSD detector class
//  OrgEvent      : Original event number
//  Event         : Event number of current TTree
//  ID            : Detector ID (1-4)
//  X             : X-value of pad
//  Y             : Y-value of pad        
//  Z             : Z-value of pad        
//  Energy        : Energy of one pad
//  Time          : Time 
//
//  TrackData     : for track class
//  OrgEvent      : Original event number
//  Event         : Event number of current TTree
//  Pos           : Vector of A (l=A+tB)
//  Dir           : Vector of B (l=A+tB)
//  PosErr        : Error value of A
//  DirErr        : Error value of B
//  Chi2          : Chi-squared value
//  Energy        : Total energy of track
//  Length        : Length from vertex to end point of track
//  ReachingPoint : Position when the track reaches the wall
//  TrackNumber   : Track number
//
//  VertexData    : for vertex class
//  OrgEvent      : Original event number
//  Event         : Event number of current TTree
//  Vertex        : Vertex point
//  VertexErr     : Error of vertex
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
#include "DataSet.hh"

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//==========================================================//
//-+-+-+-+-+-+-+-+-+-+- Pad Data Class -+-+-+-+-+-+-+-+-+-+-//
//==========================================================//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PadData::PadData() :fOrgEvent(0), fEvent(0) {
	fX.clear();
	fY.clear();
	fZ.clear();
	fEnergy.clear();
	fDistance.clear();
	fFpoint.clear();
	fEpoint.clear();
	fTrackNumber.clear();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PadData::~PadData() {
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::ClearStr() {
	// Initialization of structure
	fOrgEvent = 0;
	fEvent = 0;
	fX.clear();
	fY.clear();
	fZ.clear();
	fEnergy.clear();
	fDistance.clear();
	fFpoint.clear();
	fEpoint.clear();
	fTrackNumber.clear();

	vector<Double_t>().swap(fX);
	vector<Double_t>().swap(fY);
	vector<Double_t>().swap(fZ);
	vector<Double_t>().swap(fEnergy);
	vector<Double_t>().swap(fDistance);
	vector<Int_t>().swap(fFpoint);
	vector<Int_t>().swap(fEpoint);
	vector<Int_t>().swap(fTrackNumber);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::ClearStr(Int_t i) {
	// Set Dummy
	ClearStr();
	SetOrgEvent(i);
	SetEvent(i);
	SetX(0,i);
	SetY(0,i);
	SetZ(0,i);
	SetEnergy(0,i);
	SetDistance(0,i);
	SetFpoint(0,i);
	SetEpoint(0,i);
	SetTrackNumber(0,i);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::CopyStr(PadData& input) {
	ClearStr();
	
	SetStrEvent(input);
	SetX(input.GetX());
	SetY(input.GetY());
	SetZ(input.GetZ());
	SetEnergy(input.GetEnergy());
	SetDistance(input.GetDistance());
	SetFpoint(input.GetFpoint());
	SetEpoint(input.GetEpoint());
	SetTrackNumber(input.GetTrackNumber());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::CopyStr(PadData *input) {
	ClearStr();
	
	SetStrEvent(input);
	SetX(input->GetX());
	SetY(input->GetY());
	SetZ(input->GetZ());
	SetEnergy(input->GetEnergy());
	SetDistance(input->GetDistance());
	SetFpoint(input->GetFpoint());
	SetEpoint(input->GetEpoint());
	SetTrackNumber(input->GetTrackNumber());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::SetStrEvent(PadData& input) {
	SetOrgEvent(input.GetOrgEvent());
	SetEvent(input.GetEvent());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::SetStrEvent(PadData *input) {
	SetOrgEvent(input->GetOrgEvent());
	SetEvent(input->GetEvent());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::AddStr(Int_t org, Double_t x, Double_t y, Double_t z, Double_t e) {
	SetOrgEvent(org);
	AddX(x); AddY(y); AddZ(z);
	AddEnergy(e); AddDistance(0);
	AddFpoint(0); AddEpoint(0); AddTrackNumber(0);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::AddStr(PadData& input, Int_t index, Bool_t flag) {
	if (flag) SetStrEvent(input);
	AddX(input.GetX(index));
	AddY(input.GetY(index));
	AddZ(input.GetZ(index));
	AddEnergy(input.GetEnergy(index));
	AddDistance(input.GetDistance(index));
	AddFpoint(input.GetFpoint(index));
	AddEpoint(input.GetEpoint(index));
	AddTrackNumber(input.GetTrackNumber(index));
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::AddStr(PadData& input) {
	SetStrEvent(input);
	for (Int_t i=0;i<input.Size();i++) AddStr(input, i, "false");
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::SetXYZE(vector<arrayD4> xyze) {
	for (Int_t i=0;i<(Int_t)xyze.size();i++) {
		AddX(xyze[i][0]);
		AddY(xyze[i][1]);
		AddZ(xyze[i][2]);
		AddEnergy(xyze[i][3]);
		AddDistance(0);
		AddFpoint(0);
		AddEpoint(0);
		AddTrackNumber(0);
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
arrayD4 PadData::GetXYZE(Int_t i) {
	arrayD4 xyze = {GetX(i), GetY(i), GetZ(i), GetEnergy(i)};
	return xyze;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TVector3 PadData::GetXYZ(Int_t i) {
	TVector3 xyz(GetX(i), GetY(i), GetZ(i));
	return xyz;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t PadData::CalTotalEnergy() {
	Double_t total = 0;
	for (Int_t i=0;i<Size();i++) {
		total += GetEnergy(i);
	}
	return total;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::CalLength(VertexData vertex) {
	if (vertex.GetVertex().Mag()==0) return;
	
	TVector3 vert = vertex.GetVertex();
	TVector3 xyz;
	for (Int_t i=0;i<Size();i++) {
		xyz = GetXYZ(i);
		SetDistance(i, (xyz-vert).Mag());
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::Erase(PadData input) {
	Int_t evorg = GetOrgEvent();

	vector<arrayD4> orgdata;
	for (Int_t i=0;i<Size();i++) {
		orgdata.push_back(GetXYZE(i));
	}
	
	vector<arrayD4> inpdata;
	for (Int_t i=0;i<input.Size();i++) {
		inpdata.push_back(input.GetXYZE(i));
	}

	auto itr = orgdata.begin();
	while (itr!=orgdata.end()) {
		for (Int_t i=0;i<input.Size();i++) {
			if ((*itr)==inpdata[i]) {
				itr = orgdata.erase(itr);
				goto NEXT;
			}
		}
		itr++;
NEXT: ;
	}
	
	ClearStr();
	SetOrgEvent(evorg);
	SetXYZE(orgdata);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t PadData::FindFpoint(Int_t i) {
	if (i==0) return -1;
	for (Int_t j=0;j<Size();j++) {
		if (fFpoint[j]==i) return j;
	}
	return -1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t PadData::FindFpointX(Int_t i) {
	if (i==0) return -1;
	for (Int_t j=0;j<Size();j++) {
		if (fFpoint[j]==i) return fX[j];
	}
	return -1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t PadData::FindFpointY(Int_t i) {
	if (i==0) return -1;
	for (Int_t j=0;j<Size();j++) {
		if (fFpoint[j]==i) return fY[j];
	}
	return -1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t PadData::FindEpoint(Int_t i) {
	if (i==0) return -1;
	for (Int_t j=0;j<Size();j++) {
		if (fEpoint[j]==i) return j;
	}
	return -1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t PadData::FindEpointX(Int_t i) {
	if (i==0) return -1;
	for (Int_t j=0;j<Size();j++) {
		if (fEpoint[j]==i) return fX[j];
	}
	return -1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t PadData::FindEpointY(Int_t i) {
	if (i==0) return -1;
	for (Int_t j=0;j<Size();j++) {
		if (fEpoint[j]==i) return fY[j];
	}
	return -1;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::Sort(TString type, PadData& pad) {
	struct data {
		Double_t x;
		Double_t y;
		Double_t z;
		Double_t e;
		Double_t d;
		Int_t fp;
		Int_t ep;
		Int_t tn;
	};
	
	vector<data> dataset;
	for (Int_t i=0;i<Size();i++) {
		dataset.push_back({GetX(i),GetY(i),GetZ(i),GetEnergy(i),GetDistance(i), GetFpoint(i),GetEpoint(i),GetTrackNumber(i)});
	}

	type.ToLower();
	if (type=="energy") {
		sort(dataset.begin(), dataset.end(), [](const data& a, const data& b){return a.e<b.e;});
	} else if (type=="distance") {
		sort(dataset.begin(), dataset.end(), [](const data& a, const data& b){return a.d<b.d;});
	} else if (type=="x") {
		sort(dataset.begin(), dataset.end(), [](const data& a, const data& b){return a.x<b.x;});
	} else if (type=="y") {
		sort(dataset.begin(), dataset.end(), [](const data& a, const data& b){return a.y<b.y;});
	} else if (type=="z") {
		sort(dataset.begin(), dataset.end(), [](const data& a, const data& b){return a.z<b.z;});
	}
	
	pad.ClearStr();
	pad.SetOrgEvent(GetOrgEvent());
	pad.SetEvent(GetEvent());
	for (Int_t i=0;i<Size();i++) {
		pad.AddX(dataset[i].x);
		pad.AddY(dataset[i].y);
		pad.AddZ(dataset[i].z);
		pad.AddEnergy(dataset[i].e);
		pad.AddDistance(dataset[i].d);
		pad.AddFpoint(dataset[i].fp);
		pad.AddEpoint(dataset[i].ep);
		pad.AddTrackNumber(dataset[i].tn);
	}

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PadData::ShowStat(TString pos, TString comment) {
	cout<<"## Show Current Status of PadData ##"<<endl;
	cout<<" Position : "<<pos<<endl;
	cout<<" Comments : "<<comment<<endl;
	cout<<"#= = = = = = = = = = = = = Event No."<<GetOrgEvent()<<" = = = = = = = = = = = = =#"<<endl;
	cout<<"#= = = = = = = = = = = = = Size is "<<Size()<<" = = = = = = = = = = = = =#"<<endl;
	cout<<"; column ;   row  ; height ; energy ; distan ; fpoint ; epoint ; trackNo. ;"<<endl;
	cout<<";--------;--------;--------;--------;--------;--------;--------;----------;"<<endl;
	for (Int_t i=0;i<Size();i++) {
		cout<<";"<<right<<setw(7)<<fX[i]<<" ;"
			<<setw(7)<<fY[i]<<" ;"
			<<setw(7)<<fZ[i]<<" ;"
			<<setw(7)<<fEnergy[i]<<" ;"
			<<setw(7)<<fDistance[i]<<" ;"
			<<setw(7)<<fFpoint[i]<<" ;"
			<<setw(7)<<fEpoint[i]<<" ;"
			<<setw(9)<<fTrackNumber[i]<<" ;"
			<<endl;
			}
	cout<<"#= = = = = = = = = = = = = Event No."<<GetOrgEvent()<<" = = = = = = = = = = = = =#"<<endl;
	cout<<endl<<endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



//============================================================//
//-+-+-+-+-+-+-+-+-+-+- Track Data Class -+-+-+-+-+-+-+-+-+-+-//
//============================================================//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TrackData::TrackData() :fOrgEvent(0), fEvent(0) {
	fPos.clear();
	fDir.clear();
	fPosErr.clear();
	fDirErr.clear();
	fChi2.clear();
	fEnergy.clear();
	fLength.clear();
	fReachingPoint.clear();
	fTrackNumber.clear();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TrackData::~TrackData() {
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TrackData::ClearStr() {
	// Initialization of structure
	fOrgEvent = 0;
	fEvent = 0;
	fPos.clear();
	fDir.clear();
	fPosErr.clear();
	fDirErr.clear();
	fChi2.clear();
	fEnergy.clear();
	fLength.clear();
	fReachingPoint.clear();
	fTrackNumber.clear();
	
	vector<TVector3>().swap(fPos);
	vector<TVector3>().swap(fDir);
	vector<TVector3>().swap(fPosErr);
	vector<TVector3>().swap(fDirErr);
	vector<Double_t>().swap(fChi2);
	vector<Double_t>().swap(fEnergy);
	vector<Double_t>().swap(fLength);
	vector<TVector3>().swap(fReachingPoint);
	vector<Int_t>().swap(fTrackNumber);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TrackData::ClearStr(Double_t i) {
	// Set Dummy
	ClearStr();
	
	SetOrgEvent((Int_t)i);
	SetEvent((Int_t)i);

	TVector3 Dummy(i,i,i);
	AddPosition(Dummy);
	AddDirection(Dummy);
	AddPositionErr(Dummy);
	AddDirectionErr(Dummy);
	AddChi2(i);
	AddEnergy(i);
	AddLength(i);
	AddReachingPoint(Dummy);
	AddTrackNumber((Int_t)i);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TrackData::CopyStr(TrackData& input) {
	ClearStr();
	SetOrgEvent(input.GetOrgEvent());
	SetEvent(input.GetEvent());
	SetPosition(input.GetPosition());
	SetDirection(input.GetDirection());
	SetPositionErr(input.GetPositionErr());
	SetDirectionErr(input.GetDirectionErr());
	SetChi2(input.GetChi2());
	SetEnergy(input.GetEnergy());
	SetLength(input.GetLength());
	SetReachingPoint(input.GetReachingPoint());
	SetTrackNumber(input.GetTrackNumber());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TrackData::CopyStr(TrackData *input) {
	ClearStr();

	SetOrgEvent(input->GetOrgEvent());
	SetEvent(input->GetEvent());
	SetPosition(input->GetPosition());
	SetDirection(input->GetDirection());
	SetPositionErr(input->GetPositionErr());
	SetDirectionErr(input->GetDirectionErr());
	SetChi2(input->GetChi2());
	SetEnergy(input->GetEnergy());
	SetLength(input->GetLength());
	SetReachingPoint(input->GetReachingPoint());
	SetTrackNumber(input->GetTrackNumber());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TrackData::SetStrEvent(TrackData& input) {
	SetOrgEvent(input.GetOrgEvent());
	SetEvent(input.GetEvent());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TrackData::AddStr(TrackData& input, Int_t index, Bool_t flag) {
	if (flag) SetStrEvent(input);
	AddPosition(input.GetPosition(index));
	AddDirection(input.GetDirection(index));
	AddPositionErr(input.GetPositionErr(index));
	AddDirectionErr(input.GetDirectionErr(index));
	AddChi2(input.GetChi2(index));
	AddEnergy(input.GetEnergy(index));
	AddLength(input.GetLength(index));
	AddReachingPoint(input.GetReachingPoint(index));
	AddTrackNumber(input.GetTrackNumber(index));
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TrackData::AddStr(TrackData& input) {
	SetStrEvent(input);
	for (Int_t i=0;i<input.Size();i++) AddStr(input, i, "false");
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TrackData::CalLength(PadData pad, VertexData vertex) {
	if (vertex.GetVertex().Mag()==0) return;
	
	TVector3 vert = vertex.GetVertex();
	TVector3 fxyz;
	TVector3 exyz;
	Double_t flength;
	Double_t elength;
	Double_t length;

	for (Int_t i=0;i<Size();i++) {
		fxyz = pad.GetXYZ(pad.FindFpoint(i+1));
		exyz = pad.GetXYZ(pad.FindEpoint(i+1));
		
		flength = (vert-fxyz).Mag();
		elength = (vert-exyz).Mag();
		length = (flength>elength ? flength : elength); 

		SetLength(i, length);
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Bool_t TrackData::PunchThrough(PadData *pad, Int_t i, Double_t minx, Double_t maxx, Double_t miny, Double_t maxy, Double_t minz, Double_t maxz) {
	Bool_t flag1 = false; 
	Bool_t flag2 = false;
	TVector3 epoint = pad->GetXYZ(pad->FindEpoint(i));
	TVector3 fpoint = pad->GetXYZ(pad->FindFpoint(i));
	
	// first point
	if (fpoint.x()>minx&&fpoint.x()<maxx) {
		if (fpoint.y()>miny&&fpoint.y()<maxy) {
			if (fpoint.z()>minz&&fpoint.z()<maxz) {
				flag2 = true;
			}
		}
	}

	// end point
	if (epoint.x()>minx&&epoint.x()<maxx) {
		if (epoint.y()>miny&&epoint.y()<maxy) {
			if (epoint.z()>minz&&epoint.z()<maxz) {
				flag1 = true;
			}
		}
	}

	if (flag1&&flag2) return false;
	return true;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Bool_t TrackData::StopInside(PadData *pad, Int_t i, Double_t minx, Double_t maxx, Double_t miny, Double_t maxy, Double_t minz, Double_t maxz) {
	Bool_t flag1 = false; 
	Bool_t flag2 = false;
	TVector3 epoint = pad->GetXYZ(pad->FindEpoint(i));
	TVector3 fpoint = pad->GetXYZ(pad->FindFpoint(i));
	
	// first point
	if (fpoint.x()>minx&&fpoint.x()<maxx) {
	if (fpoint.y()>miny&&fpoint.y()<maxy) {
	if (fpoint.z()>minz&&fpoint.z()<maxz) {
				flag2 = true;
			}
		}
	}

	// end point
	if (epoint.x()>minx&&epoint.x()<maxx) {
	if (epoint.y()>miny&&epoint.y()<maxy) {
	if (epoint.z()>minz&&epoint.z()<maxz) {
				flag1 = true;
			}
		}
	}

	if (flag1&&flag2) return true;
	return false;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TrackData::Sort(TString type, TrackData& track) {
	struct data {
		TVector3 pos;
		TVector3 dir;
		TVector3 poserr;
		TVector3 direrr;
		Double_t chi2;
		Double_t ene;
		Double_t len;
		Int_t num;
		TVector3 reach;
	};

	vector<data> dataset;
	for (Int_t i=0;i<Size();i++) {
		dataset.push_back({GetPosition(i),GetDirection(i),GetPositionErr(i),GetDirectionErr(i),GetChi2(i),GetEnergy(i),GetLength(i),GetTrackNumber(i), GetReachingPoint(i)});
	}

	if (type=="energy") {
		sort(dataset.begin(), dataset.end(), [](const data& a, const data& b){return a.ene>b.ene;});
	} else if (type=="length") {
		sort(dataset.begin(), dataset.end(), [](const data& a, const data& b){return a.len>b.len;});
	} else if (type=="number") {
		sort(dataset.begin(), dataset.end(), [](const data& a, const data& b){return a.num>b.num;});
	}
	
	track.ClearStr();
	track.SetOrgEvent(GetOrgEvent());
	track.SetEvent(GetEvent());
	for (Int_t i=0;i<Size();i++) {
		track.AddPosition(dataset[i].pos);
		track.AddDirection(dataset[i].dir);
		track.AddPositionErr(dataset[i].poserr);
		track.AddDirectionErr(dataset[i].direrr);
		track.AddChi2(dataset[i].chi2);
		track.AddEnergy(dataset[i].ene);
		track.AddLength(dataset[i].len);
		track.AddTrackNumber(dataset[i].num);
		track.AddReachingPoint(dataset[i].reach);
	}

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TrackData::ShowStat(TString pos, TString comment) {
	cout<<"## Show Current Status of TrackData ##"<<endl;
	cout<<" Position : "<<pos<<endl;
	cout<<" Comments : "<<comment<<endl;
	cout<<"#= = = = = = = = = = Event No."<<GetOrgEvent()<<" = = = = = = = = = =#"<<endl;
	for (Int_t i=0;i<Size();i++) {
		cout<<i+1<<"-th Track"<<endl;
		cout<<"Track Number    : "<<GetTrackNumber(i)<<endl;
		cout<<"Position        : "; GetPosition(i).Print();
		cout<<"Position Error  : "; GetPositionErr(i).Print();
		cout<<"Direction       : "; GetDirection(i).Print();
		cout<<"Direction Error : "; GetDirectionErr(i).Print();
		cout<<"Chi2            : "<<GetChi2(i)<<endl;
		cout<<"Energy          : "<<GetEnergy(i)<<endl;
		cout<<"Length          : "<<GetLength(i)<<endl<<endl;
	}
	cout<<endl<<endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//=============================================================//
//-+-+-+-+-+-+-+-+-+-+- Vertex Data Class -+-+-+-+-+-+-+-+-+-+-//
//=============================================================//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
VertexData::VertexData() :fOrgEvent(0), fEvent(0) {
	fVertex.SetXYZ(0,0,0);
	fVertexErr.SetXYZ(0,0,0);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
VertexData::~VertexData() {
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void VertexData::ClearStr() {
	// Initialization of structure
	fOrgEvent = 0;
	fEvent = 0;
	fVertex.SetXYZ(0,0,0);
	fVertexErr.SetXYZ(0,0,0);
	
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void VertexData::ClearStr(Double_t i) {
	// Set Dummy
	TVector3 Dummy(i,i,i);
	
	SetOrgEvent((Int_t)i);
	SetEvent((Int_t)i);
	SetVertex(Dummy);
	SetVertexErr(Dummy);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void VertexData::CopyStr(VertexData& input) {
	ClearStr();
	SetOrgEvent(input.GetOrgEvent());
	SetEvent(input.GetEvent());
	
	SetVertex(input.GetVertex());
	SetVertexErr(input.GetVertexErr());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void VertexData::CopyStr(VertexData *input) {
	ClearStr();
	SetOrgEvent(input->GetOrgEvent());
	SetEvent(input->GetEvent());
	
	SetVertex(input->GetVertex());
	SetVertexErr(input->GetVertexErr());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Bool_t VertexData::IsBeamArea(Double_t minx, Double_t maxx, Double_t miny, Double_t maxy, Double_t minz, Double_t maxz) {
	if (GetVertexX()<minx||GetVertexX()>maxx) return kFALSE;
	if (GetVertexY()<miny||GetVertexY()>maxy) return kFALSE;
	if (GetVertexZ()<minz||GetVertexZ()>maxz) return kFALSE;
	return kTRUE;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void VertexData::ShowStat(TString pos, TString comment) {
	cout<<"## Show Current Status of VertexData ##"<<endl;
	cout<<" Position : "<<pos<<endl;
	cout<<" Comments : "<<comment<<endl;
	cout<<"#= = = = = = = = = = Event No."<<GetOrgEvent()<<" = = = = = = = = = =#"<<endl;
	cout<<"Vertex       : "; GetVertex().Print();
	cout<<"Vertex Error : "; GetVertexErr().Print();
	cout<<endl<<endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//===========================================================//
//-+-+-+-+-+-+-+-+-+-+- Maya Data Class -+-+-+-+-+-+-+-+-+-+-//
//===========================================================//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MayaData::MayaData() :fOrgEvent(0), fEvent(0) {
	fID.clear();
	fX.clear();
	fY.clear();
	fZ.clear();
	fEnergy.clear();
	fTime.clear();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
MayaData::~MayaData() {
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t MayaData::Size(TString str) {
	Int_t Right = 0;
	Int_t Left = 0;
	Int_t Sum = 0;
	for (auto i=0;i<(Int_t)fX.size();i++) {
		if (fY[i]>0) Left++;
		if (fY[i]<0) Right++;
		Sum++;
	}
	
	str.ToLower();
	if (str=="right") return Right;
	if (str=="left")  return Left;
	if (str.IsNull()) return Sum;
	return 0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MayaData::ClearStr() {
	// Initialization of structure
	fOrgEvent = 0;
	fEvent = 0;
	fID.clear();
	fX.clear();
	fY.clear();
	fZ.clear();
	fEnergy.clear();
	fTime.clear();
	
	vector<Int_t>().swap(fID);
	vector<Double_t>().swap(fX);
	vector<Double_t>().swap(fY);
	vector<Double_t>().swap(fZ);
	vector<Double_t>().swap(fEnergy);
	vector<Double_t>().swap(fTime);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MayaData::ClearStr(Int_t i) {
	// Dummy
	ClearStr();

	SetOrgEvent(i);
	SetEvent(i);
	AddID(i);
	AddX(i);
	AddY(i);
	AddZ(i);
	AddEnergy(i);
	AddTime(i);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MayaData::CopyStr(MayaData& input) {
	ClearStr();
	
	SetStrEvent(input);
	SetID(input.GetID());
	SetX(input.GetX());
	SetY(input.GetY());
	SetZ(input.GetZ());
	SetEnergy(input.GetEnergy());
	SetTime(input.GetTime());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MayaData::CopyStr(MayaData *input) {
	ClearStr();
	
	SetStrEvent(input);
	SetID(input->GetID());
	SetX(input->GetX());
	SetY(input->GetY());
	SetZ(input->GetZ());
	SetEnergy(input->GetEnergy());
	SetTime(input->GetTime());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MayaData::SetStrEvent(MayaData& input) {
	SetOrgEvent(input.GetOrgEvent());
	SetEvent(input.GetEvent());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MayaData::SetStrEvent(MayaData *input) {
	SetOrgEvent(input->GetOrgEvent());
	SetEvent(input->GetEvent());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MayaData::AddStr(Int_t org, Int_t id, Double_t x, Double_t y, Double_t z, Double_t e, Double_t t) {
	SetOrgEvent(org);
	AddID(id);
	AddX(x); AddY(y); AddZ(z);
	AddEnergy(e); AddTime(t);
}	
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t MayaData::GetTotalEnergy(TString str) {
	str.ToLower();
	Double_t ene = 0;
	for (auto i=0;i<Size();i++) {
		if (str.IsNull()) {
			ene += GetEnergy(i);
		} else if (str=="right") {
			if (GetY(i)<0) ene+= GetEnergy(i);
		} else if (str=="left") {
			if (GetY(i)>0) ene+= GetEnergy(i);
		} else {
			return 0;
		}
	}
	return ene;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t MayaData::GetTotalEnergy(Int_t id) {
	Double_t ene = 0;
	for (auto i=0;i<Size();i++) {
		if (GetID(i)==id) {
			ene += GetEnergy(i);
		}
	}
	return ene;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TVector3 MayaData::GetPosition(Int_t i) {
	TVector3 vec(GetX(i), GetY(i), GetZ(i));
	return vec;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MayaData::ShowStat(TString pos, TString comment) {
	cout<<"## Show Current Status of MayaData ##"<<endl;
	cout<<" Position : "<<pos<<endl;
	cout<<" Comments : "<<comment<<endl;
	cout<<"#= = = = = = = = = = Event No."<<GetOrgEvent()<<" = = = = = = = = = =#"<<endl;
	cout<<" X : Y : Z : E : T "<<endl;
	for (auto i=0;i<Size();i++) {
		cout<<GetX(i)<<" : "<<GetY(i)<<" : "<<GetZ(i)<<" : "<<GetEnergy(i)<<" : "<<GetTime(i)<<endl;
	}
	cout<<endl<<endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//===========================================================//
//-+-+-+-+-+-+-+-+-+-+- DSSD Data Class -+-+-+-+-+-+-+-+-+-+-//
//===========================================================//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DSSDData::DSSDData() :fOrgEvent(0), fEvent(0) {
	fID.clear();
	fX.clear();
	fY.clear();
	fZ.clear();
	fEnergy.clear();
	fTime.clear();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DSSDData::~DSSDData() {
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DSSDData::ClearStr() {
	// Initialization of structure
	fOrgEvent = 0;
	fEvent = 0;
	fID.clear();
	fX.clear();
	fY.clear();
	fZ.clear();
	fEnergy.clear();
	fTime.clear();
	
	vector<Int_t>().swap(fID);
	vector<Double_t>().swap(fX);
	vector<Double_t>().swap(fY);
	vector<Double_t>().swap(fZ);
	vector<Double_t>().swap(fEnergy);
	vector<Double_t>().swap(fTime);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DSSDData::ClearStr(Int_t i) {
	// Dummy
	ClearStr();
	
	SetOrgEvent(i);
	SetEvent(i);
	AddID(i);
	AddX(i);
	AddY(i);
	AddZ(i);
	AddEnergy(i);
	AddTime(i);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DSSDData::CopyStr(DSSDData& input) {
	ClearStr();
	
	SetStrEvent(input);
	SetID(input.GetID());
	SetX(input.GetX());
	SetY(input.GetY());
	SetZ(input.GetZ());
	SetEnergy(input.GetEnergy());
	SetTime(input.GetTime());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DSSDData::CopyStr(DSSDData *input) {
	ClearStr();
	
	SetStrEvent(input);
	SetID(input->GetID());
	SetX(input->GetX());
	SetY(input->GetY());
	SetZ(input->GetZ());
	SetEnergy(input->GetEnergy());
	SetTime(input->GetTime());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DSSDData::SetStrEvent(DSSDData& input) {
	SetOrgEvent(input.GetOrgEvent());
	SetEvent(input.GetEvent());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DSSDData::SetStrEvent(DSSDData *input) {
	SetOrgEvent(input->GetOrgEvent());
	SetEvent(input->GetEvent());
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DSSDData::AddStr(Int_t org, Int_t id, Double_t x, Double_t y, Double_t z, Double_t e, Double_t t) {
	SetOrgEvent(org);
	AddID(id);
	AddX(x); AddY(y); AddZ(z);
	AddEnergy(e); AddTime(t);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t DSSDData::GetTotalEnergy() {
	Double_t total = 0;
	for (auto i=0;i<Size();i++) {
		total += GetEnergy(i);
	}
	return total;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t DSSDData::GetTotalEnergy(Int_t id) {
	Double_t ene = 0;
	for (auto i=0;i<Size();i++) {
		if (GetID(i)==id) {
			ene += GetEnergy(i);
		}
	}
	return ene;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TVector3 DSSDData::GetPosition(Int_t i) {
	TVector3 vec(GetX(i), GetY(i), GetZ(i));
	return vec;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DSSDData::ConvertPosition() {
	Int_t orgeve = GetOrgEvent();
	
	Int_t indexY[4] = {0};
	Int_t indexZ[4] = {0};
	Int_t id = 0;
	Double_t x = GetX(0);
	Double_t y[4] = {0};
	Double_t z[4] = {0};
	Double_t e[4] = {0};
	Double_t t[4] = {DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX};
	for (auto i=1;i<Size();i++) {
		if (GetTime(i)<510*10) {
			id = GetID(i)-1;
			if (GetY(i)!=DBL_MAX) {
				y[id] += GetY(i);
				indexY[id]++;
			}
			if (GetZ(i)!=DBL_MAX) {
				z[id] += GetZ(i);
				indexZ[id]++;
			}
			e[id] += GetEnergy(i);
			t[id] = min(t[id], GetTime(i));
		}
	}
	


	ClearStr();
	for (auto i=0;i<4;i++) {
		if (indexY[i]>0&&indexZ[i]>0) {
			y[i] = y[i]/(Double_t)(indexY[i]);
			z[i] = z[i]/(Double_t)(indexZ[i]);
			AddStr(orgeve, i+1, x, y[i], z[i], e[i], t[i]);
		}
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DSSDData::ShowStat(TString pos, TString comment) {
	cout<<"## Show Current Status of DSSDData ##"<<endl;
	cout<<" Position : "<<pos<<endl;
	cout<<" Comments : "<<comment<<endl;
	cout<<"#= = = = = = = = = = Event No."<<GetOrgEvent()<<" = = = = = = = = = =#"<<endl;
	cout<<" ID : X : Y : Z : E : T "<<endl;
	for (auto i=0;i<Size();i++) {
		cout<<GetID(i)<<" : "<<GetX(i)<<" : "<<GetY(i)<<" : "<<GetZ(i)<<" : "<<GetEnergy(i)<<" : "<<GetTime(i)<<endl;
	}
	cout<<endl<<endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

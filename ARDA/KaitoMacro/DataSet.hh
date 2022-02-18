#ifndef DATASET_HH
#define DATASET_HH 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Include Files
// For C++
#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <numeric>
#include <fstream>
#include <vector>
#include <string>
// For ROOT
#include "TSystem.h"
#include "TRandom3.h"
#include "TVector3.h"
#include "TObject.h"
#include "TMath.h"

using namespace std;

class PadData;
class TrackData;
class VertexData;
class MayaData;
class DSSDData;

typedef array<Int_t, 4> arrayI4;
typedef array<Double_t, 4> arrayD4;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//==========================================================//
//-+-+-+-+-+-+-+-+-+-+- Pad Data Class -+-+-+-+-+-+-+-+-+-+-//
//==========================================================//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class PadData : public TObject
{
	public:
		// ===== Constructor & Destructor===== //
		PadData();
		~PadData();

		// ===== Functions : Get Size ===== //
		inline Int_t Size()		const {return (Int_t)fX.size();}
		
		// ===== Functions : Clear Structure ===== //
		void ClearStr();
		void ClearStr(Int_t i);

		// ===== Functions : Copy Structure ===== //
		void CopyStr(PadData& input);
		void CopyStr(PadData *input);

		// ===== Functions : Add Structure ===== //
		void AddStr(Int_t org, Double_t x, Double_t y, Double_t z, Double_t e);
		void AddStr(PadData& input, Int_t index, Bool_t flag="true");
		void AddStr(PadData& input);

		// ===== Functions : Others ===== //
		Double_t CalTotalEnergy();
		void CalLength(VertexData vertex);
		void ShowStat(TString pos, TString comment);
		void Erase(PadData input);
		void Sort(TString type, PadData& pad);
		
		// ===== Functions : Set ===== //
		void SetStrEvent(PadData& input);				// see source code
		void SetStrEvent(PadData *input);				// see source code
		void SetXYZE(vector<arrayD4> xyze);				// see source code
		inline void SetOrgEvent(Int_t val)				{fOrgEvent = val;}
		inline void SetEvent(Int_t val)					{fEvent = val;}

		inline void SetX(vector<Double_t> vec)			{fX = vec;}
		inline void SetY(vector<Double_t> vec)			{fY = vec;}
		inline void SetZ(vector<Double_t> vec)			{fZ = vec;}
		inline void SetEnergy(vector<Double_t> vec)		{fEnergy = vec;}
		inline void SetDistance(vector<Double_t> vec)	{fDistance = vec;}
		inline void SetFpoint(vector<Int_t> vec)		{fFpoint = vec;}
		inline void SetEpoint(vector<Int_t> vec)		{fEpoint = vec;}
		inline void SetTrackNumber(vector<Int_t> vec)	{fTrackNumber = vec;}

		inline void SetX(Int_t i, Double_t val)			{fX[i] = val;}
		inline void SetY(Int_t i, Double_t val)			{fY[i] = val;}
		inline void SetZ(Int_t i, Double_t val)			{fZ[i] = val;}
		inline void SetEnergy(Int_t i, Double_t val)	{fEnergy[i] = val;}
		inline void SetDistance(Int_t i, Double_t val)	{fDistance[i] = val;}
		inline void SetFpoint(Int_t i, Int_t val)		{fFpoint[i] = val;}
		inline void SetEpoint(Int_t i, Int_t val)		{fEpoint[i] = val;}
		inline void SetTrackNumber(Int_t i, Int_t val)	{fTrackNumber[i] = val;}
		

		// ===== Functions : Add ===== //
		inline void AddX(Double_t val)			{fX.push_back(val);}
		inline void AddY(Double_t val)			{fY.push_back(val);}
		inline void AddZ(Double_t val)			{fZ.push_back(val);}
		inline void AddEnergy(Double_t val)		{fEnergy.push_back(val);}
		inline void AddDistance(Double_t val)	{fDistance.push_back(val);}
		inline void AddFpoint(Int_t val)		{fFpoint.push_back(val);}
		inline void AddEpoint(Int_t val)		{fEpoint.push_back(val);}
		inline void AddTrackNumber(Int_t val)	{fTrackNumber.push_back(val);}
		
		// ===== Functions : Get ===== //
		arrayD4 GetXYZE(Int_t i);				// see source code
		TVector3 GetXYZ(Int_t i);				// see source code
		inline Int_t GetOrgEvent()				const {return fOrgEvent;}
		inline Int_t GetEvent()					const {return fEvent;}
		
		inline vector<Double_t> GetX()			const {return fX;}
		inline vector<Double_t> GetY()			const {return fY;}
		inline vector<Double_t> GetZ()			const {return fZ;}
		inline vector<Double_t> GetEnergy()		const {return fEnergy;}
		inline vector<Double_t> GetDistance()	const {return fDistance;}
		inline vector<Int_t> GetFpoint()		const {return fFpoint;}
		inline vector<Int_t> GetEpoint()		const {return fEpoint;}
		inline vector<Int_t> GetTrackNumber()	const {return fTrackNumber;}

		inline Double_t GetX(Int_t i)			const {return fX[i];}
		inline Double_t GetY(Int_t i)			const {return fY[i];}
		inline Double_t GetZ(Int_t i)			const {return fZ[i];}
		inline Double_t GetEnergy(Int_t i)		const {return fEnergy[i];}
		inline Double_t GetDistance(Int_t i)	const {return fDistance[i];}
		inline Int_t GetFpoint(Int_t i)			const {return fFpoint[i];}
		inline Int_t GetEpoint(Int_t i)			const {return fEpoint[i];}
		inline Int_t GetTrackNumber(Int_t i)	const {return fTrackNumber[i];}
		
		// ===== Functions : Find ===== //
		Int_t FindFpoint(Int_t i);
		Double_t FindFpointX(Int_t i);
		Double_t FindFpointY(Int_t i);
		Int_t FindEpoint(Int_t i);
		Double_t FindEpointX(Int_t i);
		Double_t FindEpointY(Int_t i);

		ClassDef(PadData, 1);

	private:
		// Event Info
		Int_t fOrgEvent;
		Int_t fEvent;

		// Pad Info
		vector<Double_t> fX;
		vector<Double_t> fY;
		vector<Double_t> fZ;
		vector<Double_t> fEnergy;
		vector<Double_t> fDistance;
		vector<Int_t> fFpoint;
		vector<Int_t> fEpoint;
		vector<Int_t> fTrackNumber;
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//============================================================//
//-+-+-+-+-+-+-+-+-+-+- Track Data Class -+-+-+-+-+-+-+-+-+-+-//
//============================================================//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class TrackData : public TObject
{
	public:
		// ===== Constructor & Destructor===== //
		TrackData();
		~TrackData();

		// ===== Functions : Get Size ===== //
		inline Int_t Size()		const {return (Int_t)fPos.size();}
		
		// ===== Functions : Clear Structure ===== //
		void ClearStr();
		void ClearStr(Double_t i);

		// ===== Functions : Copy Structure ===== //
		void CopyStr(TrackData& input);
		void CopyStr(TrackData *input);
		
		// ===== Functions : Add Structure ===== //
		void AddStr(TrackData& input, Int_t index, Bool_t flag="true");
		void AddStr(TrackData& input);

		// ===== Functions : Others ===== //
		void CalLength(PadData pad, VertexData vertex);
		Bool_t PunchThrough(PadData *pad, Int_t i, Double_t minx, Double_t maxx, Double_t miny, Double_t maxy, Double_t minz, Double_t maxz);
		Bool_t StopInside(PadData *pad, Int_t i, Double_t minx, Double_t maxx, Double_t miny, Double_t maxy, Double_t minz, Double_t maxz);
		void ShowStat(TString pos, TString comment);
		void Sort(TString type, TrackData& track);

		// ===== Functions : Set ===== //
		void SetStrEvent(TrackData& input);
		inline void SetOrgEvent(Int_t val)					{fOrgEvent = val;}
		inline void SetEvent(Int_t val)						{fEvent = val;}

		inline void SetPosition(vector<TVector3> vec)		{fPos = vec;}
		inline void SetDirection(vector<TVector3> vec)		{fDir = vec;}
		inline void SetPositionErr(vector<TVector3> vec)	{fPosErr = vec;}
		inline void SetDirectionErr(vector<TVector3> vec)	{fDirErr = vec;}
		inline void SetChi2(vector<Double_t> vec)			{fChi2 = vec;}
		inline void SetEnergy(vector<Double_t> vec)			{fEnergy = vec;}
		inline void SetLength(vector<Double_t> vec)			{fLength = vec;}
		inline void SetReachingPoint(vector<TVector3> vec)	{fReachingPoint = vec;}
		inline void SetTrackNumber(vector<Int_t> vec)		{fTrackNumber = vec;}
		
		inline void SetPosition(Int_t i, TVector3 val)		{fPos[i] = val;}
		inline void SetDirection(Int_t i, TVector3 val)		{fDir[i] = val;}
		inline void SetPositionErr(Int_t i, TVector3 val)	{fPosErr[i] = val;}
		inline void SetDirectionErr(Int_t i, TVector3 val)	{fDirErr[i] = val;}
		inline void SetChi2(Int_t i, Double_t val)			{fChi2[i] = val;}
		inline void SetEnergy(Int_t i, Double_t val)		{fEnergy[i] = val;}
		inline void SetLength(Int_t i, Double_t val)		{fLength[i] = val;}
		inline void SetReachingPoint(Int_t i, TVector3 val)	{fReachingPoint[i] = val;}
		inline void SetTrackNumber(Int_t i, Int_t val)		{fTrackNumber[i] = val;}
		

		// ===== Functions : Add ===== //
		inline void AddPosition(TVector3 val)		{fPos.push_back(val);}
		inline void AddDirection(TVector3 val)		{fDir.push_back(val);}
		inline void AddPositionErr(TVector3 val)	{fPosErr.push_back(val);}
		inline void AddDirectionErr(TVector3 val)	{fDirErr.push_back(val);}
		inline void AddChi2(Double_t val)			{fChi2.push_back(val);}
		inline void AddEnergy(Double_t val)			{fEnergy.push_back(val);}
		inline void AddLength(Double_t val)			{fLength.push_back(val);}
		inline void AddReachingPoint(TVector3 val)	{fReachingPoint.push_back(val);}
		inline void AddTrackNumber(Int_t val)		{fTrackNumber.push_back(val);}
		
		// ===== Functions : Get ===== //
		inline Int_t GetOrgEvent()					const {return fOrgEvent;}
		inline Int_t GetEvent()						const {return fEvent;}
		
		inline vector<TVector3> GetPosition()		const {return fPos;}
		inline vector<TVector3> GetDirection()		const {return fDir;}
		inline vector<TVector3> GetPositionErr()	const {return fPosErr;}
		inline vector<TVector3> GetDirectionErr()	const {return fDirErr;}
		inline vector<Double_t> GetChi2()			const {return fChi2;}
		inline vector<Double_t> GetEnergy()			const {return fEnergy;}
		inline vector<Double_t> GetLength()			const {return fLength;}
		inline vector<TVector3> GetReachingPoint()	const {return fReachingPoint;}
		inline vector<Int_t> GetTrackNumber()		const {return fTrackNumber;}

		inline TVector3 GetPosition(Int_t i)		const {return fPos[i];}
		inline Double_t GetPositionX(Int_t i)		const {return fPos[i].x();}
		inline Double_t GetPositionY(Int_t i)		const {return fPos[i].y();}
		inline Double_t GetPositionZ(Int_t i)		const {return fPos[i].z();}
		inline TVector3 GetDirection(Int_t i)		const {return fDir[i];}
		inline Double_t GetDirectionX(Int_t i)		const {return fDir[i].x();}
		inline Double_t GetDirectionY(Int_t i)		const {return fDir[i].y();}
		inline Double_t GetDirectionZ(Int_t i)		const {return fDir[i].z();}
		inline TVector3 GetPositionErr(Int_t i)		const {return fPosErr[i];}
		inline Double_t GetPositionErrX(Int_t i)	const {return fPosErr[i].x();}
		inline Double_t GetPositionErrY(Int_t i)	const {return fPosErr[i].y();}
		inline Double_t GetPositionErrZ(Int_t i)	const {return fPosErr[i].z();}
		inline TVector3 GetDirectionErr(Int_t i)	const {return fDirErr[i];}
		inline Double_t GetDirectionErrX(Int_t i)	const {return fDirErr[i].x();}
		inline Double_t GetDirectionErrY(Int_t i)	const {return fDirErr[i].y();}
		inline Double_t GetDirectionErrZ(Int_t i)	const {return fDirErr[i].z();}
		inline Double_t GetChi2(Int_t i)			const {return fChi2[i];}
		inline Double_t GetEnergy(Int_t i)			const {return fEnergy[i];}
		inline Double_t GetLength(Int_t i)			const {return fLength[i];}
		inline TVector3 GetReachingPoint(Int_t i)	const {return fReachingPoint[i];}
		inline Double_t GetReachingPointX(Int_t i)	const {return fReachingPoint[i].x();}
		inline Double_t GetReachingPointY(Int_t i)	const {return fReachingPoint[i].y();}
		inline Double_t GetReachingPointZ(Int_t i)	const {return fReachingPoint[i].z();}
		inline Int_t GetTrackNumber(Int_t i)		const {return fTrackNumber[i];}
		
		inline Double_t GetTheta(Int_t i)				const {return fDir[i].Theta()*180./TMath::Pi()-90.;}	
		inline Double_t GetPhi(Int_t i)					const {return fDir[i].Phi()*180./TMath::Pi();}	
		inline Double_t GetAngle(Int_t i, TVector3 vec)	const {return fDir[i].Angle(vec)*180./TMath::Pi();}

		ClassDef(TrackData, 1);

	private:
		// Event Info
		Int_t fOrgEvent;
		Int_t fEvent;

		// Track Info
		vector<TVector3> fPos;
		vector<TVector3> fDir;
		vector<TVector3> fPosErr;
		vector<TVector3> fDirErr;
		vector<Double_t> fChi2;
		vector<Double_t> fEnergy;
		vector<Double_t> fLength;
		vector<TVector3> fReachingPoint;
		vector<Int_t> fTrackNumber;
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//=============================================================//
//-+-+-+-+-+-+-+-+-+-+- Vertex Data Class -+-+-+-+-+-+-+-+-+-+-//
//=============================================================//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class VertexData : public TObject
{
	public:
		// ===== Constructor & Destructor===== //
		VertexData();
		~VertexData();

		// ===== Functions : Get Size ===== //
		
		// ===== Functions : Clear Structure ===== //
		void ClearStr();
		void ClearStr(Double_t i);

		// ===== Functions : Copy Structure ===== //
		void CopyStr(VertexData& input);
		void CopyStr(VertexData *input);

		// ===== Functions : Others ===== //
		Bool_t IsBeamArea(Double_t minx, Double_t maxx, Double_t miny, Double_t maxy, Double_t minz, Double_t maxz);
		void ShowStat(TString pos, TString comment);
		
		// ===== Functions : Set ===== //
		inline void SetOrgEvent(Int_t val)		{fOrgEvent = val;}
		inline void SetEvent(Int_t val)			{fEvent = val;}
		inline void SetVertex(TVector3 val)		{fVertex = val;}
		inline void SetVertexErr(TVector3 val)	{fVertexErr = val;}
		
		// ===== Functions : Get ===== //
		inline Int_t GetOrgEvent()		const {return fOrgEvent;}
		inline Int_t GetEvent()			const {return fEvent;}
		inline TVector3 GetVertex()		const {return fVertex;}
		inline Double_t GetVertexX()	const {return fVertex.x();}
		inline Double_t GetVertexY()	const {return fVertex.y();}
		inline Double_t GetVertexZ()	const {return fVertex.z();}
		inline TVector3 GetVertexErr()	const {return fVertexErr;}
		inline Double_t GetVertexErrX()	const {return fVertexErr.x();}
		inline Double_t GetVertexErrY()	const {return fVertexErr.y();}
		inline Double_t GetVertexErrZ()	const {return fVertexErr.z();}
		
		ClassDef(VertexData, 1);

	private:
		// Event Info
		Int_t fOrgEvent;
		Int_t fEvent;

		// Vertex Info
		TVector3 fVertex;
		TVector3 fVertexErr;
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//===========================================================//
//-+-+-+-+-+-+-+-+-+-+- Maya Data Class -+-+-+-+-+-+-+-+-+-+-//
//===========================================================//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class MayaData : public TObject
{
	public:
		// ===== Constructor & Destructor===== //
		MayaData();
		~MayaData();

		// ===== Functions : Get Size ===== //
		Int_t Size(TString str="");

		// ===== Functions : Clear Structure ===== //
		void ClearStr();
		void ClearStr(Int_t i);

		// ===== Functions : Copy Structure ===== //
		void CopyStr(MayaData& input);
		void CopyStr(MayaData *input);
		
		// ===== Functions : Add Structure ===== //
		void AddStr(Int_t org, Int_t id, Double_t x, Double_t y, Double_t z, Double_t e, Double_t t);
		
		// ===== Functions : Others ===== //
		Double_t GetTotalEnergy(TString str="");
		Double_t GetTotalEnergy(Int_t id);
		TVector3 GetPosition(Int_t i);
		void ShowStat(TString pos, TString comment);

		// ===== Functions : Set ===== //
		void SetStrEvent(MayaData& input);			// see source code
		void SetStrEvent(MayaData *input);			// see source code
		inline void SetOrgEvent(Int_t val)			{fOrgEvent = val;}
		inline void SetEvent(Int_t val)				{fEvent = val;}
		
		inline void SetID(vector<Int_t> val)		{fID = val;}
		inline void SetX(vector<Double_t> val)		{fX = val;}
		inline void SetY(vector<Double_t> val)		{fY = val;}
		inline void SetZ(vector<Double_t> val)		{fZ = val;}
		inline void SetEnergy(vector<Double_t> val)	{fEnergy = val;}
		inline void SetTime(vector<Double_t> val)	{fTime = val;}

		// ===== Functions : Add ===== //
		inline void AddID(Int_t i)			{fID.push_back(i);}
		inline void AddX(Double_t d)		{fX.push_back(d);}
		inline void AddY(Double_t d)		{fY.push_back(d);}
		inline void AddZ(Double_t d)		{fZ.push_back(d);}
		inline void AddEnergy(Double_t d)	{fEnergy.push_back(d);}
		inline void AddTime(Double_t d)		{fTime.push_back(d);}

		// ===== Functions : Get ===== //
		inline Int_t GetOrgEvent()			const {return fOrgEvent;}
		inline Int_t GetEvent()				const {return fEvent;}
		
		inline vector<Int_t> GetID()		const {return fID;}
		inline vector<Double_t> GetX()		const {return fX;}
		inline vector<Double_t> GetY()		const {return fY;}
		inline vector<Double_t> GetZ()		const {return fZ;}
		inline vector<Double_t> GetEnergy()	const {return fEnergy;}
		inline vector<Double_t> GetTime()	const {return fTime;}

		inline Int_t GetID(Int_t i)			const {return fID[i];}
		inline Double_t GetX(Int_t i)		const {return fX[i];}
		inline Double_t GetY(Int_t i)		const {return fY[i];}
		inline Double_t GetZ(Int_t i)		const {return fZ[i];}
		inline Double_t GetEnergy(Int_t i)	const {return fEnergy[i];}
		inline Double_t GetTime(Int_t i)	const {return fTime[i];}
		
		
		ClassDef(MayaData, 1);

	private:
		// Event Info
		Int_t fOrgEvent;
		Int_t fEvent;

		// Maya Info
		vector<Int_t> fID;
		vector<Double_t> fX;
		vector<Double_t> fY;
		vector<Double_t> fZ;
		vector<Double_t> fEnergy;
		vector<Double_t> fTime;

};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



//===========================================================//
//-+-+-+-+-+-+-+-+-+-+- DSSD Data Class -+-+-+-+-+-+-+-+-+-+-//
//===========================================================//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class DSSDData : public TObject
{
	public:
		// ===== Constructor & Destructor===== //
		DSSDData();
		~DSSDData();

		// ===== Functions : Get Size ===== //
		inline Int_t Size()		{return (Int_t)fX.size();}

		// ===== Functions : Clear Structure ===== //
		void ClearStr();
		void ClearStr(Int_t i);

		// ===== Functions : Copy Structure ===== //
		void CopyStr(DSSDData& input);
		void CopyStr(DSSDData *input);
		
		// ===== Functions : Add Structure ===== //
		void AddStr(Int_t org, Int_t id, Double_t x, Double_t y, Double_t z, Double_t e, Double_t tim);
		
		// ===== Functions : Others ===== //
		Double_t GetTotalEnergy();
		Double_t GetTotalEnergy(Int_t id);
		TVector3 GetPosition(Int_t i);  
		void ShowStat(TString pos, TString comment);
		void ConvertPosition();

		// ===== Functions : Set ===== //
		void SetStrEvent(DSSDData& input);				// see source code
		void SetStrEvent(DSSDData *input);				// see source code
		inline void SetOrgEvent(Int_t val)		{fOrgEvent = val;}
		inline void SetEvent(Int_t val)			{fEvent = val;}
		
		inline void SetID(vector<Int_t> val)		{fID = val;}
		inline void SetX(vector<Double_t> val)		{fX = val;}
		inline void SetY(vector<Double_t> val)		{fY = val;}
		inline void SetZ(vector<Double_t> val)		{fZ = val;}
		inline void SetEnergy(vector<Double_t> val)	{fEnergy = val;}
		inline void SetTime(vector<Double_t> val)	{fTime = val;}

		// ===== Functions : Add ===== //
		inline void AddID(Int_t i)			{fID.push_back(i);}
		inline void AddX(Double_t d)		{fX.push_back(d);}
		inline void AddY(Double_t d)		{fY.push_back(d);}
		inline void AddZ(Double_t d)		{fZ.push_back(d);}
		inline void AddEnergy(Double_t d)	{fEnergy.push_back(d);}
		inline void AddTime(Double_t d)		{fTime.push_back(d);}

		// ===== Functions : Get ===== //
		inline Int_t GetOrgEvent()			const {return fOrgEvent;}
		inline Int_t GetEvent()				const {return fEvent;}
		
		inline vector<Int_t> GetID()		const {return fID;}
		inline vector<Double_t> GetX()		const {return fX;}
		inline vector<Double_t> GetY()		const {return fY;}
		inline vector<Double_t> GetZ()		const {return fZ;}
		inline vector<Double_t> GetEnergy()	const {return fEnergy;}
		inline vector<Double_t> GetTime()	const {return fTime;}

		inline Int_t GetID(Int_t i)			const {return fID[i];}
		inline Double_t GetX(Int_t i)		const {return fX[i];}
		inline Double_t GetY(Int_t i)		const {return fY[i];}
		inline Double_t GetZ(Int_t i)		const {return fZ[i];}
		inline Double_t GetEnergy(Int_t i)	const {return fEnergy[i];}
		inline Double_t GetTime(Int_t i)	const {return fTime[i];}
		

		ClassDef(DSSDData, 1);

	private:
		// Event Info
		Int_t fOrgEvent;
		Int_t fEvent;

		// DSSD Info
		vector<Int_t> fID;
		vector<Double_t> fX;
		vector<Double_t> fY;
		vector<Double_t> fZ;
		vector<Double_t> fEnergy;
		vector<Double_t> fTime;
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#endif

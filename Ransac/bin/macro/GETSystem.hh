#ifndef GETSYSTEM_HH
#define GETSYSTEM_HH 1

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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class GETData
{
	public:
		// ===== Constructor & Destructor===== //
		GETData();
		~GETData();

		// ===== Functions : Get Size ===== //
		inline Int_t Size()     const {return (Int_t)fCoBo.size();}
		

		// ===== Functions : Others ===== //
		void ReadTable(TString filename);
		Bool_t CheckGETSystem(Int_t cobo, Int_t asad, Int_t aget, Int_t chan);

		// ===== Functions : Add ===== //
		inline void AddGETSystem(Int_t cobo, Int_t asad, Int_t aget, Int_t chan) {
			fCoBo.push_back(cobo);
			fAsAd.push_back(asad);
			fAGet.push_back(aget);
			fChan.push_back(chan);
		}
		void AddChanPos(Int_t cobo, Int_t asad, Int_t aget, Int_t chan, Double_t x, Double_t y, Double_t z);
		
		// ===== Functions : Get ===== //
		inline vector<Int_t> GetCoBo()		const {return fCoBo;}
		inline vector<Int_t> GetAsAd()		const {return fAsAd;}
		inline vector<Int_t> GetAGet()		const {return fAGet;}
		inline vector<Int_t> GetChan()		const {return fChan;}
		inline vector<Int_t> GetRow() 		const {return fRow;}
		inline vector<Int_t> GetCol() 		const {return fCol;}
		inline vector<Double_t> GetFitA()	const {return fFitA;}
		inline vector<Double_t> GetFitB()	const {return fFitB;}

		inline Int_t GetCoBo(Int_t i)		const {return fCoBo[i];}
		inline Int_t GetAsAd(Int_t i) 		const {return fAsAd[i];}
		inline Int_t GetAGet(Int_t i) 		const {return fAGet[i];}
		inline Int_t GetChan(Int_t i) 		const {return fChan[i];}
		inline Int_t GetRow(Int_t i)  		const {return fRow[i];}
		inline Int_t GetCol(Int_t i)  		const {return fCol[i];}
		inline Double_t GetFitA(Int_t i)	const {return fFitA[i];}
		inline Double_t GetFitB(Int_t i)	const {return fFitB[i];}

		Int_t GetRow(Int_t cobo, Int_t asad, Int_t aget, Int_t chan);
		Int_t GetCol(Int_t cobo, Int_t asad, Int_t aget, Int_t chan);
		
		Double_t GetFitA(Int_t cobo, Int_t asad, Int_t aget, Int_t chan);
		Double_t GetFitB(Int_t cobo, Int_t asad, Int_t aget, Int_t chan);

		TVector3 GetPosXYZ(Int_t cobo, Int_t asad, Int_t aget, Int_t chan);
		Double_t GetPosX(Int_t cobo, Int_t asad, Int_t aget, Int_t chan);
		Double_t GetPosY(Int_t cobo, Int_t asad, Int_t aget, Int_t chan);
		Double_t GetPosZ(Int_t cobo, Int_t asad, Int_t aget, Int_t chan);


	private:
		vector<Int_t> fPad;
		vector<Int_t> fCoBo;
		vector<Int_t> fAsAd;
		vector<Int_t> fAGet;
		vector<Int_t> fChan;
		vector<Int_t> fRow;
		vector<Int_t> fCol;
		vector<Double_t> fFitA;
		vector<Double_t> fFitB;

		map<array<Int_t,4>, TVector3> fChanPos;
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

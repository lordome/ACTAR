#ifndef DETECTORSET_HH
#define DETECTORSET_HH 1


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include <map>

#include "TVector3.h"
#include "TString.h"

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class DetectorSet {
	public:
		DetectorSet();
		DetectorSet(TString str);
		~DetectorSet();
		
		void ClearData();
		void SetType(TString str);


		inline TString GetType()	const {return fTypeOfACTAR;}
		
		inline TVector3 DetectorMax(Double_t d=1.)	const {return fDetectorMax*d;}
		inline Double_t DetectorMaxX(Double_t d=1.)	const {return fDetectorMax.X()*d;}
		inline Double_t DetectorMaxY(Double_t d=1.)	const {return fDetectorMax.Y()*d;}
		inline Double_t DetectorMaxZ(Double_t d=1.)	const {return fDetectorMax.Z()*d;}

		inline TVector3 DetectorMin(Double_t d=1.)	const {return fDetectorMin*d;}
		inline Double_t DetectorMinX(Double_t d=1.)	const {return fDetectorMin.X()*d;}
		inline Double_t DetectorMinY(Double_t d=1.)	const {return fDetectorMin.Y()*d;}
		inline Double_t DetectorMinZ(Double_t d=1.)	const {return fDetectorMin.Z()*d;}
		
		inline TVector3 PadMax(Double_t d=1.)	const {return fPadMax*d;}
		inline Double_t PadMaxX(Double_t d=1.)	const {return fPadMax.X()*d;}
		inline Double_t PadMaxY(Double_t d=1.)	const {return fPadMax.Y()*d;}
		inline Double_t PadMaxZ(Double_t d=1.)	const {return fPadMax.Z()*d;}

		inline TVector3 PadMin(Double_t d=1.)	const {return fPadMin*d;}
		inline Double_t PadMinX(Double_t d=1.)	const {return fPadMin.X()*d;}
		inline Double_t PadMinY(Double_t d=1.)	const {return fPadMin.Y()*d;}
		inline Double_t PadMinZ(Double_t d=1.)	const {return fPadMin.Z()*d;}

		inline TVector3 BeamPosition(Double_t d=1.)		const {return fBeamPosition*d;}
		inline Double_t BeamPositionX(Double_t d=1.)	const {return fBeamPosition.X()*d;}
		inline Double_t BeamPositionY(Double_t d=1.)	const {return fBeamPosition.Y()*d;}
		inline Double_t BeamPositionZ(Double_t d=1.)	const {return fBeamPosition.Z()*d;}
		
		inline TVector3 BeamDirection(Double_t d=1.)	const {return fBeamDirection*d;}
		inline Double_t BeamDirectionX(Double_t d=1.)	const {return fBeamDirection.X()*d;}
		inline Double_t BeamDirectionY(Double_t d=1.)	const {return fBeamDirection.Y()*d;}
		inline Double_t BeamDirectionZ(Double_t d=1.)	const {return fBeamDirection.Z()*d;}
		
		inline TVector3 NumberOfPad(Double_t d=1.)		const {return fNumberOfPad*d;}
		inline Double_t NumberOfPadX(Double_t d=1.)		const {return fNumberOfPad.X()*d;}
		inline Double_t NumberOfPadY(Double_t d=1.)		const {return fNumberOfPad.Y()*d;}
		inline Double_t NumberOfPadZ(Double_t d=1.)		const {return fNumberOfPad.Z()*d;}
		
		inline TVector3 NumberOfMaya(Double_t d=1.)		const {return fNumberOfMaya*d;}
		inline Double_t NumberOfMayaX(Double_t d=1.)	const {return fNumberOfMaya.X()*d;}
		inline Double_t NumberOfMayaY(Double_t d=1.)	const {return fNumberOfMaya.Y()*d;}
		inline Double_t NumberOfMayaZ(Double_t d=1.)	const {return fNumberOfMaya.Z()*d;}

		inline TVector3 NumberOfDSSD(Double_t d=1.)		const {return fNumberOfDSSD*d;}
		inline Double_t NumberOfDSSDX(Double_t d=1.)	const {return fNumberOfDSSD.X()*d;}
		inline Double_t NumberOfDSSDY(Double_t d=1.)	const {return fNumberOfDSSD.Y()*d;}
		inline Double_t NumberOfDSSDZ(Double_t d=1.)	const {return fNumberOfDSSD.Z()*d;}
		
		inline TVector3 NumberOfStrip(Double_t d=1.)	const {return fNumberOfStrip*d;}
		inline Double_t NumberOfStripX(Double_t d=1.)	const {return fNumberOfStrip.X()*d;}
		inline Double_t NumberOfStripY(Double_t d=1.)	const {return fNumberOfStrip.Y()*d;}
		inline Double_t NumberOfStripZ(Double_t d=1.)	const {return fNumberOfStrip.Z()*d;}

		inline TVector3 SizeOfPad(Double_t d=1.)		const {return fSizeOfPad*d;}
		inline Double_t SizeOfPadX(Double_t d=1.)		const {return fSizeOfPad.X()*d;}
		inline Double_t SizeOfPadY(Double_t d=1.)		const {return fSizeOfPad.Y()*d;}
		inline Double_t SizeOfPadZ(Double_t d=1.)		const {return fSizeOfPad.Z()*d;}
		
		inline TVector3 SizeOfMaya(Double_t d=1.)		const {return fSizeOfMaya*d;}
		inline Double_t SizeOfMayaX(Double_t d=1.)		const {return fSizeOfMaya.X()*d;}
		inline Double_t SizeOfMayaY(Double_t d=1.)		const {return fSizeOfMaya.Y()*d;}
		inline Double_t SizeOfMayaZ(Double_t d=1.)		const {return fSizeOfMaya.Z()*d;}
		
		inline TVector3 SizeOfDSSD(Double_t d=1.)		const {return fSizeOfDSSD*d;}
		inline Double_t SizeOfDSSDX(Double_t d=1.)		const {return fSizeOfDSSD.X()*d;}
		inline Double_t SizeOfDSSDY(Double_t d=1.)		const {return fSizeOfDSSD.Y()*d;}
		inline Double_t SizeOfDSSDZ(Double_t d=1.)		const {return fSizeOfDSSD.Z()*d;}

		inline Double_t PitchOfPad(Double_t d=1.)		const {return fPitchOfPad*d;}
		inline Double_t PitchOfMaya(Double_t d=1.)		const {return fPitchOfMaya*d;}
		inline Double_t PitchOfDSSD(Double_t d=1.)		const {return fPitchOfDSSD*d;}
		inline Double_t PitchOfStrip(Double_t d=1.)		const {return fPitchOfStrip*d;}
		
		inline TVector3 CenterOfPad(Double_t d=1.)		const {return fCenterOfPad*d;}
		inline Double_t CenterOfPadX(Double_t d=1.)		const {return fCenterOfPad.X()*d;}
		inline Double_t CenterOfPadY(Double_t d=1.)		const {return fCenterOfPad.Y()*d;}
		inline Double_t CenterOfPadZ(Double_t d=1.)		const {return fCenterOfPad.Z()*d;}
		
		inline TVector3 CenterOfMayaR(Double_t d=1.)	const {return fCenterOfMayaR*d;}
		inline Double_t CenterOfMayaRX(Double_t d=1.)	const {return fCenterOfMayaR.X()*d;}
		inline Double_t CenterOfMayaRY(Double_t d=1.)	const {return fCenterOfMayaR.Y()*d;}
		inline Double_t CenterOfMayaRZ(Double_t d=1.)	const {return fCenterOfMayaR.Z()*d;}
		
		inline TVector3 CenterOfMayaL(Double_t d=1.)	const {return fCenterOfMayaL*d;}
		inline Double_t CenterOfMayaLX(Double_t d=1.)	const {return fCenterOfMayaL.X()*d;}
		inline Double_t CenterOfMayaLY(Double_t d=1.)	const {return fCenterOfMayaL.Y()*d;}
		inline Double_t CenterOfMayaLZ(Double_t d=1.)	const {return fCenterOfMayaL.Z()*d;}
		
		inline TVector3 CenterOfDSSD(Double_t d=1.)		const {return fCenterOfDSSD*d;}
		inline Double_t CenterOfDSSDX(Double_t d=1.)	const {return fCenterOfDSSD.X()*d;}
		inline Double_t CenterOfDSSDY(Double_t d=1.)	const {return fCenterOfDSSD.Y()*d;}
		inline Double_t CenterOfDSSDZ(Double_t d=1.)	const {return fCenterOfDSSD.Z()*d;}
		
		inline TVector3 InnerPadMax(Double_t d=1.)		const {return fInnerPadMax*d;}
		inline Double_t InnerPadMaxX(Double_t d=1.)		const {return fInnerPadMax.X()*d;}
		inline Double_t InnerPadMaxY(Double_t d=1.)		const {return fInnerPadMax.Y()*d;}
		inline Double_t InnerPadMaxZ(Double_t d=1.)		const {return fInnerPadMax.Z()*d;}
		
		inline TVector3 InnerPadMin(Double_t d=1.)		const {return fInnerPadMin*d;}
		inline Double_t InnerPadMinX(Double_t d=1.)		const {return fInnerPadMin.X()*d;}
		inline Double_t InnerPadMinY(Double_t d=1.)		const {return fInnerPadMin.Y()*d;}
		inline Double_t InnerPadMinZ(Double_t d=1.)		const {return fInnerPadMin.Z()*d;}
		
		inline TVector3 CenterOfMayaRPad(Int_t i, Double_t d=1.) {
			TVector3 vec(0,0,0);
			auto itr = fCenterOfMayaRPad.find(i);
			if (itr != fCenterOfMayaRPad.end()) {
				vec = itr->second;
			}
			return vec*d;
		}
		inline Double_t CenterOfMayaRPadX(Int_t i, Double_t d=1.) {
			TVector3 vec = CenterOfMayaRPad(i,d);
			return vec.X();
		}
		inline Double_t CenterOfMayaRPadY(Int_t i, Double_t d=1.) {
			TVector3 vec = CenterOfMayaRPad(i,d);
			return vec.Y();
		}
		inline Double_t CenterOfMayaRPadZ(Int_t i, Double_t d=1.) {
			TVector3 vec = CenterOfMayaRPad(i,d);
			return vec.Z();
		}
		
		inline TVector3 CenterOfMayaLPad(Int_t i, Double_t d=1.) {
			TVector3 vec(0,0,0);
			auto itr = fCenterOfMayaLPad.find(i);
			if (itr != fCenterOfMayaLPad.end()) {
				vec = itr->second;
			}
			return vec*d;
		}
		inline Double_t CenterOfMayaLPadX(Int_t i, Double_t d=1.) {
			TVector3 vec = CenterOfMayaLPad(i,d);
			return vec.X();
		}
		inline Double_t CenterOfMayaLPadY(Int_t i, Double_t d=1.) {
			TVector3 vec = CenterOfMayaLPad(i,d);
			return vec.Y();
		}
		inline Double_t CenterOfMayaLPadZ(Int_t i, Double_t d=1.) {
			TVector3 vec = CenterOfMayaLPad(i,d);
			return vec.Z();
		}

		inline TVector3 CenterOfDSSDPad(Int_t i, Double_t d=1.)	{
			TVector3 vec(0,0,0);
			auto itr = fCenterOfDSSDPad.find(i);
			if (itr != fCenterOfDSSDPad.end()) {
				vec = itr->second;
			}
			return vec*d;
		}
		inline Double_t CenterOfDSSDPadX(Int_t i, Double_t d=1.) {
			TVector3 vec = CenterOfDSSDPad(i,d);
			return vec.X();
		}
		inline Double_t CenterOfDSSDPadY(Int_t i, Double_t d=1.) {
			TVector3 vec = CenterOfDSSDPad(i,d);
			return vec.Y();
		}
		inline Double_t CenterOfDSSDPadZ(Int_t i, Double_t d=1.) {
			TVector3 vec = CenterOfDSSDPad(i,d);
			return vec.Z();
		}

	private:
		TString fTypeOfACTAR;
		
		TVector3 fDetectorMax;
		TVector3 fDetectorMin;
		TVector3 fPadMax;
		TVector3 fPadMin;

		TVector3 fBeamPosition;
		TVector3 fBeamDirection;
		
		TVector3 fNumberOfPad;
		TVector3 fNumberOfMaya;
		TVector3 fNumberOfDSSD;
		TVector3 fNumberOfStrip;

		TVector3 fSizeOfPad;
		TVector3 fSizeOfMaya;
		TVector3 fSizeOfDSSD;

		Double_t fPitchOfPad;
		Double_t fPitchOfMaya;
		Double_t fPitchOfDSSD;
		Double_t fPitchOfStrip;
		
		TVector3 fCenterOfPad;
		TVector3 fCenterOfMayaR;
		TVector3 fCenterOfMayaL;
		TVector3 fCenterOfDSSD;
		
		TVector3 fInnerPadMin;
		TVector3 fInnerPadMax;

		map<Int_t, TVector3> fCenterOfMayaRPad;
		map<Int_t, TVector3> fCenterOfMayaLPad;
		map<Int_t, TVector3> fCenterOfDSSDPad;

};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#endif

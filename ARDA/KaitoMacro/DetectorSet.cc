
#include "DetectorSet.hh"

DetectorSet::DetectorSet() {
	ClearData();
}

DetectorSet::DetectorSet(TString str) {
	SetType(str);
}

DetectorSet::~DetectorSet() {
}

void DetectorSet::ClearData() {
	TVector3 zero(0,0,0);

	fTypeOfACTAR = "";

	fDetectorMax = zero;
	fDetectorMin = zero;

	fPadMax = zero;
	fPadMin = zero;
	
	fBeamPosition = zero;
	fBeamDirection= zero;

	fNumberOfPad   = zero;
	fNumberOfMaya  = zero;
	fNumberOfDSSD  = zero;
	fNumberOfStrip = zero;

	fSizeOfPad  = zero;
	fSizeOfMaya = zero;
	fSizeOfDSSD = zero;

	fPitchOfPad = 0;
	fPitchOfMaya = 0;
	fPitchOfDSSD = 0;
	fPitchOfStrip = 0;

	fCenterOfPad   = zero;
	fCenterOfMayaR = zero;
	fCenterOfMayaL = zero;
	fCenterOfDSSD  = zero;
	
	fInnerPadMin = zero;
	fInnerPadMax = zero;

	fCenterOfMayaRPad.clear();
	fCenterOfMayaLPad.clear();
	fCenterOfDSSDPad.clear();
}

void DetectorSet::SetType(TString str) {
	if (str=="demo") {
		fTypeOfACTAR = str;
		
		fDetectorMax.SetXYZ(179, 108, 210);
		fDetectorMin.SetXYZ(-55, -68, 0);
		
		fBeamPosition.SetXYZ(0, 32, 105);
		fBeamDirection.SetXYZ(1, 0, 0);
		
		fNumberOfPad.SetXYZ(64, 32, 0);
		fNumberOfMaya.SetXYZ(3, 0, 2);
		fNumberOfDSSD.SetXYZ(0, 2, 2);
		fNumberOfStrip.SetXYZ(0, 32, 32);

		fSizeOfPad.SetXYZ(2, 2, 0);
		fSizeOfMaya.SetXYZ(50, 0, 50);
		fSizeOfDSSD.SetXYZ(0, 64, 64);
		
		fPitchOfPad = 0;
		fPitchOfMaya = 6;
		fPitchOfDSSD = 8;
		fPitchOfStrip = 2;

		fPadMin.SetXYZ(0, 0, 0);
		fPadMax.SetX(fNumberOfPad.X()*(fSizeOfPad.X()+fPitchOfPad)+fPitchOfPad);
		fPadMax.SetY(fNumberOfPad.Y()*(fSizeOfPad.Y()+fPitchOfPad)+fPitchOfPad);
		fPadMax.SetZ(170.);
		
		fCenterOfPad.SetXYZ(PadMaxX(0.5), PadMaxY(0.5), 0);
		fCenterOfMayaR.SetXYZ(fCenterOfPad.X(), fDetectorMin.Y(), fBeamPosition.Z());
		fCenterOfMayaL.SetXYZ(fCenterOfPad.X(), fDetectorMax.Y(), fBeamPosition.Z());
		fCenterOfDSSD.SetXYZ(fDetectorMax.X(), fBeamPosition.Y(), fBeamPosition.Z());
		

		Double_t delta = 3.;
		fInnerPadMin.SetXYZ(delta, delta, delta);
		fInnerPadMax.SetXYZ(fPadMax.X()-delta, fPadMax.Y()-delta, fPadMax.Z()-delta);
		
		fCenterOfMayaRPad[0].SetXYZ(8,   -68, 77);
		fCenterOfMayaRPad[1].SetXYZ(64,  -68, 77);
		fCenterOfMayaRPad[2].SetXYZ(120, -68, 77);
		fCenterOfMayaRPad[3].SetXYZ(120, -68, 133);
		fCenterOfMayaRPad[4].SetXYZ(64,  -68, 133);
		fCenterOfMayaRPad[5].SetXYZ(8,   -68, 133);
		
		fCenterOfMayaLPad[0].SetXYZ(120, 108, 77);
		fCenterOfMayaLPad[1].SetXYZ(64,  108, 77);
		fCenterOfMayaLPad[2].SetXYZ(8,   108, 77);
		fCenterOfMayaLPad[3].SetXYZ(8,   108, 133);
		fCenterOfMayaLPad[4].SetXYZ(64,  108, 133);
		fCenterOfMayaLPad[5].SetXYZ(120, 108, 133);

		fCenterOfDSSDPad[0].SetXYZ(179, -4, 141);
		fCenterOfDSSDPad[1].SetXYZ(179, -4, 69);
		fCenterOfDSSDPad[2].SetXYZ(179, 68, 141);
		fCenterOfDSSDPad[3].SetXYZ(179, 68, 69);


	} else {
		ClearData();
	}
}


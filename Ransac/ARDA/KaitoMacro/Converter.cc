///////////////////////////////////////////////////////////////////////////////
//
//                            Converter.cc
//
//	With this code, the TTree format and unit system are converted.
//	Class format is adopted for Branch of TTree. (See DataSet.cc)
//	The position information of all detectors (Pad, Maya, and DSSD)
//	is converted into a unified coodinate system.
//	The origin of this coordinate system is the position of (col=0, row=0) of the pad detector.
//
//  Before Use:
//  - You need to use the ROOT file created by Combine.cc
//	- Detector type is necessary for position of detectors. (see DetectorSet.cc)
//	- For z-axis of Pad detector, the information of velocity of electron is necessary.
//	- Three type of data table are required ; Pad, Maya, and DSSD.
//
//  Usage :
//  >>root -l
//  >>.L $MACRO/DetectorSet.cc+
//  >>.L $MACRO/AnalsysiSet.cc+
//  >>.L $MACRO/DataSet.cc+
//  >>.L $MACRO/GETSystem.cc+
//  >>.L Converter.cc+
//  >>Converter([Ifilename], [Ofilename], [Type], [Velocity], [LookupPad], [LookupMaya], [LookupDSSD], [EneCalPad], [EneCalMaya], [EneCalDSSD], [Verbose])
//
//  [Ifilename]/string  : Input file name (path of root file)
//  [Ofilename]/string  : Output file name (path of root file)
//  [Type]/string       : Detector type
//  [Velocity]/double   : Drift velocity of electron, which depends on gas (type, pressure, electro filed) [um/ns]
//  [Delay]/double      : Delay correction time (in sample)
//  [Samplefreq]/double : Sampling frequency [MHz]
//  [LookupPad]/string  : LookupTable for Pad detector
//  [LookupMaya]/string : LookupTable for Maya detector
//  [LookupDSSD]/string : LookupTable for DSSD detector
//  [EneCalPad]/string  : Energy caliblation data table for Pad
//  [EneCalMaya]/string : Energy caliblation data table for Maya
//  [EneCalDSSD]/string : Energy caliblation data table for DSSD
//  [Verbose]/int       : verbose = -1:quiet, 0:normal, 1:details, 2:for debug
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Include Files
// For C++
#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdio.h>
// For ROOT
#include "TFile.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TTree.h"
// For RANSAC
#include "DetectorSet.hh"
#include "AnalysisSet.hh"
#include "DataSet.hh"
#include "GETSystem.hh"
#include "LinkDef.h"
// For ARDA
#include "cLookupTable.h"
#include "cHit.h"
#include "cRawEvent.h"

using namespace std;

typedef array<Double_t, 3> arrayD3;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Parameters
#ifndef __FILENAME__
#define __FILENAME__ (strrchr(__FILE__,'/') ? strrchr(__FILE__,'/')+1 : __FILE__)
#endif
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Functions
void Usage_Converter(Int_t verbose);
TVector3 PositionOfPad(TString type, Int_t col, Int_t row);
TVector3 PositionOfDSSD(TString type, Int_t col, Int_t row);
TVector3 PositionOfMaya(TString type, Int_t aget, Int_t col);
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Converter(TString Ifilename,
		TString Ofilename,
		TString Type,
		Double_t Velocity,
		Double_t Delay,
		Double_t Samplefreq,
		TString LookupTableForPad,
		TString LookupTableForMaya,
		TString LookupTableForDSSD,
		TString EneCalTableForPad,
		TString EneCalTableForMaya,
		TString EneCalTableForDSSD,
		Int_t Verbose = 0)
{

	//===== Start : Usage & Comment =====//
	if (Ifilename.IsNull()) {
		Usage_Converter(Verbose);
		return;
	}

	if (Verbose!=-1) {
		cout<<endl;
		cout<<"<< ============= "<<__FILENAME__<<" ============= >>"<<endl;
		cout<<"<< With this code, the TTree format and unit system are converted."<<endl;
		cout<<"<< Class format is adopted for Branch of TTree. (See DataSet.cc)"<<endl;
		cout<<"<< The position information of all detectors (Pad, Maya, and DSSD)"<<endl;
		cout<<"<< is converted into a unified coodinate system."<<endl;
		cout<<"<< The origin of this coordinate system is the position of (col=0, row=0) of the pad detector."<<endl;
		cout<<"<< "<<endl<<endl;
	}

	DetectorSet *detector = new DetectorSet(Type);
	AnalysisSet *analysis = new AnalysisSet();
	//===== End : Usage & Comment =====//

	//===== Start : Setting Time =====//
	time_t StartTime;
	time(&StartTime);
	//===== End : Setting Time =====//


	//===== Start : File open =====//
	// Input TFile
	TFile *Ifile = analysis->FileOpen(Ifilename);
	if (!(analysis->CheckOpenTFile(Ifile))) return;

	// Input lookup table
	cLookupTable *lookupTable = (cLookupTable*) Ifile->Get("lookupTable");

	// Input TTree : rawEventTree
	TTree *Itree = (TTree*)Ifile->Get("rawEventTree");
	if (!(analysis->CheckOpenTTree(Itree))) return;

	// Input TTree : Information
	TTree *Iinfo = (TTree*)Ifile->Get("Information");
	if (!(analysis->CheckOpenTTree(Iinfo))) return;

	// Input TBranches : event
	cRawEvent* rawEvt = nullptr;
	Itree->SetBranchAddress("event", &rawEvt);

	// Input TBranches : Information
	Int_t run = 0;
	Iinfo->SetBranchAddress("Run", &run);
	Iinfo->GetEntry(0);
	//===== End : File open =====//

	//===== Start : Recreate File =====//
	// Output TFile
	if (Ofilename.IsNull()) {
		Ofilename = Form("Run%d_Convert.root",run);
	}
	TFile *Ofile = analysis->CreateNewFile(Ofilename, Form("Run #%d",run), Verbose);
	// Output TTree : Information
	TTree *Oinfo = new TTree("Information", "info");
	Oinfo->Branch("Run",          &run,      "Run/I");
	Oinfo->Branch("DetectorType", &Type);
	Oinfo->Branch("Velocity",     &Velocity, "Velocity/D");
	Oinfo->Fill();

	// Output TTree : tree
	PadData *pad = new PadData();
	MayaData *maya = new MayaData();
	DSSDData *dssd = new DSSDData();
	TTree *Otree = new TTree("conv", Form("Conversion from %s",Ifilename.Data()));
	Otree->Branch("pad",     &pad,    16000,0);
	Otree->Branch("maya",    &maya,   16000,0);
	Otree->Branch("dssd",    &dssd,   16000,0);
	//===== End : Recreate File =====//

	//===== Start : Show Status =====//
	if (Verbose!=-1) {
		cout<<"# Start Status "<<endl;
		cout<<"== Run Stats            "<<endl;
		cout<<"|- Input File name  (comb) : "<<Ifile->GetName()<<endl;
		cout<<"|- Output File name (conv) : "<<Ofile->GetName()<<endl;
		cout<<"== LookupTable"<<endl;
		cout<<"|- Pads detector           : "<<LookupTableForPad<<endl;
		cout<<"|- Maya detector           : "<<LookupTableForMaya<<endl;
		cout<<"|- DSSD detector           : "<<LookupTableForDSSD<<endl;
		cout<<"== EneCalTable"<<endl;
		cout<<"|- Pads detector           : "<<EneCalTableForPad<<endl;
		cout<<"|- Maya detector           : "<<EneCalTableForMaya<<endl;
		cout<<"|- DSSD detector           : "<<EneCalTableForDSSD<<endl;
		cout<<"# Finish Status "<<endl<<endl;
	}
	//===== End : Show Status =====//



	//===== Start : Initialization of GET Electronics =====//
	// GETData class
	GETData PadsGET;
	GETData MayaGET;
	GETData DSSDGET;
	GETData ExclGET;

	GETData MayaEneCal;
	GETData DSSDEneCal;

	// Read Lookup Table
	PadsGET.ReadTable(LookupTableForPad.Data());
	MayaGET.ReadTable(LookupTableForMaya.Data());
	DSSDGET.ReadTable(LookupTableForDSSD.Data());

	// Read Energy Calibration Table
	MayaEneCal.ReadTable(EneCalTableForMaya.Data());
	DSSDEneCal.ReadTable(EneCalTableForDSSD.Data());

	// Exclusion data
	ExclGET.AddGETSystem(3,0,0,0);
	ExclGET.AddGETSystem(0,2,0,11);
	ExclGET.AddGETSystem(0,2,0,22);
	ExclGET.AddGETSystem(0,2,0,45);
	ExclGET.AddGETSystem(0,2,0,56);
	ExclGET.AddGETSystem(0,2,3,56);
	ExclGET.AddGETSystem(0,2,3,11);
	ExclGET.AddGETSystem(0,2,3,22);

	// convert Row Col information to position
	// (cobo, asad, aget, chan) -> (x,y,z)
	TVector3 pxyz;
	TVector3 dxyz;
	TVector3 mxyz;
	for (auto i=0;i<PadsGET.Size();i++) {
		pxyz = PositionOfPad(Type, PadsGET.GetCol(i), PadsGET.GetRow(i));
		PadsGET.AddChanPos(
				PadsGET.GetCoBo(i), //CoBo
				PadsGET.GetAsAd(i),	//AsAd
				PadsGET.GetAGet(i),	//AGET
				PadsGET.GetChan(i),	//chan
				pxyz.x(),			//x
				pxyz.y(),			//y
				pxyz.z());			//z
	}

	for (auto i=0;i<DSSDGET.Size();i++) {
		dxyz = PositionOfDSSD(Type, DSSDGET.GetCol(i), DSSDGET.GetRow(i));
		DSSDGET.AddChanPos(
				DSSDGET.GetCoBo(i), //CoBo
				DSSDGET.GetAsAd(i),	//AsAd
				DSSDGET.GetAGet(i),	//AGET
				DSSDGET.GetChan(i),	//chan
				dxyz.x(),			//x
				dxyz.y(),			//y
				dxyz.z());			//z
	}

	for (auto i=0;i<MayaGET.Size();i++) {
		mxyz = PositionOfMaya(Type, MayaGET.GetAGet(i), MayaGET.GetCol(i));
		MayaGET.AddChanPos(
				MayaGET.GetCoBo(i), //CoBo
				MayaGET.GetAsAd(i),	//AsAd
				MayaGET.GetAGet(i),	//AGET
				MayaGET.GetChan(i),	//chan
				mxyz.x(),			//x
				mxyz.y(),			//y
				mxyz.z());			//z

	}
	//===== End : Initialization of GET Electronics =====//


	//===== Start : Conversion =====//
	time_t EndTime;
	Int_t maxentry = Itree->GetEntries();
	Int_t preevent = 0;
	TVector3 padsxyz;
	TVector3 mayaxyz;
	TVector3 dssdxyz;
	Int_t mayaid = 0;
	Int_t dssdid = 0;
	Double_t mayaene = 0;
	Double_t dssdene = 0;

	if (Verbose!=-1) {
		cout<<"# Start  : Conversion ROOT Files."<<endl;
		cout<<"|- Total entires are "<<maxentry<<endl;
	}

	// Loop for all entries
	for (auto iEntry=0;iEntry<maxentry;iEntry++) {
		// Show current status
		if (Verbose!=-1&&iEntry%10000==0) {
			EndTime = time(nullptr);
			cout<<"|- Current entry is  "<<iEntry<<" ; "<<fixed<<setprecision(4)<<100.*iEntry/maxentry<<"% ("<<EndTime-StartTime<<" sec)"<<"\r"<<flush;
		}

		// Get entry
		Itree->GetEntry(iEntry);

		Int_t event = rawEvt->getEventNumber();

		// Goes through the hits
		for (cHit& hit: rawEvt->getHits()) {
			Int_t globch  = hit.getGlobalChannelId();
			Int_t cobo    = cLookupTable::getCoboFromGlobalChannelId(globch);
			Int_t asad    = cLookupTable::getAsadFromGlobalChannelId(globch);
			Int_t aget    = cLookupTable::getAgetFromGlobalChannelId(globch);
			Int_t chan    = cLookupTable::getChanFromGlobalChannelId(globch);

			Double_t maxchan = hit.getPeakTime();
			Double_t max     = hit.getPeakHeight();

			if (PadsGET.CheckGETSystem(cobo, asad, aget, chan))  { // For Pad
				padsxyz = PadsGET.GetPosXYZ(cobo, asad, aget, chan);
				pad->AddStr(event, padsxyz[0], padsxyz[1], Velocity*(maxchan - Delay)/Samplefreq + detector->BeamPositionZ(), max);
			} else if (MayaGET.CheckGETSystem(cobo, asad, aget, chan)) { // For Maya
				mayaxyz = MayaGET.GetPosXYZ(cobo, asad, aget, chan);
				mayaene = abs(max)*MayaEneCal.GetFitA(cobo, asad, aget, chan)+MayaEneCal.GetFitB(cobo, asad, aget, chan);
				maya->AddStr(event, chan, mayaxyz.x(), mayaxyz.y(), mayaxyz.z(), mayaene, maxchan*10);
			} else if (DSSDGET.CheckGETSystem(cobo, asad, aget, chan)) { // For DSSD
				dssdid  = DSSDGET.GetRow(cobo, asad, aget, chan)+1;
				dssdxyz = DSSDGET.GetPosXYZ(cobo, asad, aget, chan);
				dssdene = max*DSSDEneCal.GetFitA(cobo, asad, aget, chan) + DSSDEneCal.GetFitB(cobo, asad, aget, chan);
				dssd->AddStr(event, dssdid, dssdxyz.x(), dssdxyz.y(), dssdxyz.z(), dssdene, maxchan*10);
			} else if (ExclGET.CheckGETSystem(cobo, asad, aget, chan)) {
				// skip
			} else {
				cout << "GET electronics data doesn't match the defined value."        << endl
				     << Form("CoBo=%d, AsAd=%d, AGet=%d, chan=%d",cobo,asad,aget,chan) << endl
						 << "You have to check the \"LookupTable_forXXX.dat\"."            << endl;
			}
		}

		// Maya data
		if (maya->Size() == 0) {
			maya->ClearStr(0);
			maya->SetOrgEvent(pad->GetOrgEvent());
		}

		// DSSD data
		if (dssd->Size() == 0) {
			dssd->ClearStr(0);
			dssd->SetOrgEvent(pad->GetOrgEvent());
		} else {
			dssd->ConvertPosition();
		}

		Otree->Fill();

		pad->ClearStr();
		maya->ClearStr();
		dssd->ClearStr();

		if (Verbose==2&&event==10) break;
	}
	if (Verbose!=-1) {
		cout<<endl<<"#Finish : Conversion ROOT Files."<<endl<<endl;
	}
	//===== End : Conversion =====//

	//===== Start : Final Results =====//
	Ofile->Write();
	if (Verbose!=-1) {
		cout<<"# Start Results "<<endl;
		cout<<"== Detector Size [mm]"<<endl;
		cout<<"|- Type  : "<<detector->GetType()<<endl;
		cout<<"|- Min X : "<<detector->DetectorMinX()<<endl;
		cout<<"|- Min Y : "<<detector->DetectorMinY()<<endl;
		cout<<"|- Min Z : "<<detector->DetectorMinZ()<<endl;
		cout<<"|- Max X : "<<detector->DetectorMaxX()<<endl;
		cout<<"|- Max Y : "<<detector->DetectorMaxY()<<endl;
		cout<<"|- Max Z : "<<detector->DetectorMaxZ()<<endl;
		if (Verbose==2) {
			cout<<"== Pads Detector"<<endl;
			cout<<"|- i-th ; (CoBo, AsAd, AGet, chan) ; (  x,   y,   z) ; "<<endl;
			for (auto i=0;i<PadsGET.Size();i++) {
				cout<<"|- "<<setw(4)<<i+1<<" ;";
				cout<<" ("<<setw(4)<<PadsGET.GetCoBo(i);
				cout<<", "<<setw(4)<<PadsGET.GetAsAd(i);
				cout<<", "<<setw(4)<<PadsGET.GetAGet(i);
				cout<<", "<<setw(4)<<PadsGET.GetChan(i)<<") ;";
				cout<<" ("<<setw(3)<<1.+PadsGET.GetCol(i)*2.;
				cout<<", "<<setw(3)<<1.+PadsGET.GetRow(i)*2.;
				cout<<", "<<setw(3)<<0<<")"<<endl;
			}
			cout<<"== Maya Detector"<<endl;
			cout<<"|- i-th ; (CoBo, AsAd, AGet, chan) ; (  x,   y,   z) ; "<<endl;
			for (auto i=0;i<MayaGET.Size();i++) {
				cout<<"|- "<<setw(4)<<i+1<<" ;";
				cout<<" ("<<setw(4)<<MayaGET.GetCoBo(i);
				cout<<", "<<setw(4)<<MayaGET.GetAsAd(i);
				cout<<", "<<setw(4)<<MayaGET.GetAGet(i);
				cout<<", "<<setw(4)<<MayaGET.GetChan(i)<<") ;";
				cout<<" ("<<setw(3)<<MayaGET.GetPosX(MayaGET.GetCoBo(i),MayaGET.GetAsAd(i),MayaGET.GetAGet(i),MayaGET.GetChan(i));
				cout<<", "<<setw(3)<<MayaGET.GetPosY(MayaGET.GetCoBo(i),MayaGET.GetAsAd(i),MayaGET.GetAGet(i),MayaGET.GetChan(i));
				cout<<", "<<setw(3)<<MayaGET.GetPosZ(MayaGET.GetCoBo(i),MayaGET.GetAsAd(i),MayaGET.GetAGet(i),MayaGET.GetChan(i))<<")"<<endl;

				cout<<" fitA = "<<MayaEneCal.GetFitA(MayaGET.GetCoBo(i), MayaGET.GetAsAd(i), MayaGET.GetAGet(i), MayaGET.GetChan(i))<<endl;
				cout<<" fitB = "<<MayaEneCal.GetFitB(MayaGET.GetCoBo(i), MayaGET.GetAsAd(i), MayaGET.GetAGet(i), MayaGET.GetChan(i))<<endl;
			}
			cout<<"== DSSD Detector"<<endl;
			cout<<"|- i-th ; (CoBo, AsAd, AGet, chan) ; (  x,   y,   z) ; "<<endl;
			for (auto i=0;i<DSSDGET.Size();i++) {
				cout<<"|- "<<setw(4)<<i+1<<" ;";
				cout<<" ("<<setw(4)<<DSSDGET.GetCoBo(i);
				cout<<", "<<setw(4)<<DSSDGET.GetAsAd(i);
				cout<<", "<<setw(4)<<DSSDGET.GetAGet(i);
				cout<<", "<<setw(4)<<DSSDGET.GetChan(i)<<") ;";
				cout<<" ("<<setw(3)<<DSSDGET.GetPosX(DSSDGET.GetCoBo(i),DSSDGET.GetAsAd(i),DSSDGET.GetAGet(i),DSSDGET.GetChan(i));
				cout<<", "<<setw(3)<<DSSDGET.GetPosY(DSSDGET.GetCoBo(i),DSSDGET.GetAsAd(i),DSSDGET.GetAGet(i),DSSDGET.GetChan(i));
				cout<<", "<<setw(3)<<DSSDGET.GetPosZ(DSSDGET.GetCoBo(i),DSSDGET.GetAsAd(i),DSSDGET.GetAGet(i),DSSDGET.GetChan(i))<<")"<<endl;
			}
		}
		cout<<"# Finish Status "<<endl<<endl;
	}
	//===== End : Final Results =====//

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Usage_Converter(Int_t verbose) {
	TString str = __FILENAME__;
	str.Remove(str.Length()-3);
	if (verbose!=-1) {
		cout<<"Usage : "<<endl;
		cout<<">> .L $MACRO/DetectorSet.cc+"<<endl;
		cout<<">> .L $MACRO/AnalysisSet.cc+"<<endl;
		cout<<">> .L $MACRO/DataSet.cc+"<<endl;
		cout<<">> .L $MACRO/GETSystem.cc+"<<endl;
		cout<<">> .L "<<__FILENAME__<<"+"<<endl;
		cout<<">> "<<str<<"([Ifilename], [Ofilename], [Type], [Velocity], [LookupPad], [LookupMaya], [LookupDSSD], [EneCalPad], [EneCalMaya], [EneCalDSSD], [Verbose])"<<endl;
		cout<<" "<<endl;
		cout<<"[Ifilename]/string  : Input file name (path of root file)"<<endl;
		cout<<"[Ofilename]/string  : Output file name (path of root file)"<<endl;
		cout<<"[Type]/string       : Detector type"<<endl;
		cout<<"[Velocity]/double   : Drift velocity of electron, which depends on gas (type, pressure, electro filed) [mm/nusec]"<<endl;
		cout<<"[LookupPad]/string  : LookupTable for Pad detector"<<endl;
		cout<<"[LookupMaya]/string : LookupTable for Maya detector"<<endl;
		cout<<"[LookupDSSD]/string : LookupTable for DSSD detector"<<endl;
		cout<<"[EneCalPad]/string  : Energy caliblation data table for Pad"<<endl;
		cout<<"[EneCalMaya]/string : Energy caliblation data table for Maya"<<endl;
		cout<<"[EneCalDSSD]/string : Energy caliblation data table for DSSD"<<endl;
		cout<<"[Verbose]/int       : verbose = -1:quiet, 0:normal, 1:details, 2:for debug"<<endl;
		cout<<endl;
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TVector3 PositionOfPad(TString type, Int_t col, Int_t row) {
	DetectorSet det;
	det.SetType(type);

	Double_t x0=0,y0=0,z0=0;
	Double_t x=0,y=0,z=0;
	TVector3 xyz(0,0,0);
	x0 = det.CenterOfPadX()-(det.NumberOfPadX(0.5)*det.SizeOfPadX())+det.SizeOfPadX(0.5);
	y0 = det.CenterOfPadY()-(det.NumberOfPadY(0.5)*det.SizeOfPadY())+det.SizeOfPadY(0.5);
	z0 = 0;
	x = x0 + col*(det.SizeOfPadX()+det.PitchOfPad());
	y = y0 + row*(det.SizeOfPadY()+det.PitchOfPad());
	z = z0;
	xyz.SetXYZ(x, y, z);

	return xyz;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TVector3 PositionOfDSSD(TString type, Int_t col, Int_t row) {
	DetectorSet det;
	det.SetType(type);

	Double_t x=DBL_MAX;
	Double_t y=DBL_MAX;
	Double_t z=DBL_MAX;
	TVector3 xyz(0,0,0);

	Double_t x0 = det.CenterOfDSSDX();
	Double_t y0 = det.CenterOfDSSDY();
	Double_t z0 = det.CenterOfDSSDZ();
	Double_t d = det.PitchOfStrip();
	Double_t pit = det.PitchOfDSSD(0.5);
	Int_t num = det.NumberOfStripY();
	Double_t strip = 0;
	x = x0;

	if (col<num) {
		strip = col+1;
		if (row==0||row==1) y = (y0-pit) - strip*d;
		if (row==2||row==3) y = (y0+pit+det.SizeOfDSSDY()) - strip*d;
	} else{
		strip = col-num+1;
		if (row==0||row==2) z = (z0+pit+det.SizeOfDSSDY()) - strip*d;
		if (row==1||row==3) z = (z0-pit) - strip*d;
	}
	xyz.SetXYZ(x, y, z);
	return xyz;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
TVector3 PositionOfMaya(TString type, Int_t aget, Int_t col) {
	DetectorSet det;
	det.SetType(type);

	Double_t x=0;
	Double_t y=0;
	Double_t z=0;
	Double_t pos[6] = {120, 64, 8, 8, 64, 120};
	TVector3 xyz(0,0,0);

	if (aget==0) {
		y = det.CenterOfMayaLY();
		if (col<3) {
			x = pos[col];
			z = det.CenterOfMayaLZ()-det.PitchOfMaya()-det.SizeOfMayaZ(0.5);
		} else {
			x = pos[col];
			z = det.CenterOfMayaLZ()+det.PitchOfMaya()+det.SizeOfMayaZ(0.5);
		}
	}

	if (aget==3) {
		y = det.CenterOfMayaRY();
		if (col<3) {
			x = pos[col+3];
			z = det.CenterOfMayaRZ()-det.PitchOfMaya()-det.SizeOfMayaZ(0.5);
		} else {
			x = pos[col-3];
			z = det.CenterOfMayaRZ()+det.PitchOfMaya()+det.SizeOfMayaZ(0.5);
		}
	}

	xyz.SetXYZ(x, y, z);
	return xyz;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

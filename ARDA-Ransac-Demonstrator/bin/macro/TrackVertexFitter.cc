///////////////////////////////////////////////////////////////////////////////
//
//                            TrackVertexFitter.cc
//
//  This code uses the RANSAC and MINUIT algorithms to fit tracks and vertex in Pad detector.
//  Pad, Maya, and DSSD informations are categorized according to the number of tracks.
//  In addition to that, track and vertex information are newly described in TTree.
//	When vertex fitting is done, the beam position is also taken into consideration.
//
//  Three kinds of parameters are required for the RANSAC algorithm.
//  These must be entered as arguments to this code.
//  The "MAXLOOP" parameter means the number of iterations of RANSAC,
//  and as the number increases, the accuracy increases.
//  However, the execution time increases in proportion to this parameter.
//  The "THRESHOLD" parameter means the threshold value of the total charge within one track.
//  This is effective to exclude noise track,
//  but it should be carefully selected as it may also exclude the main track.
//  The "WIDTH" parameter means the maximum width allowed as a track.
//  This value is greatly affected by scattering of electrons,
//  so it depends on experimental conditions (gas type, gas pressure, electric field).
//	Notice that the all units are mm.
//
//	There is one optional parameter "MinRange" and "MaxRange".
//	The fitting range of the z-axis can be limited by this parameters.
//	The z-axis value of the pad detector was calculated as time * drift velocity.
//	Since the time information causes saturation at 5.1 musec,
//	it is effective for fitting excluding noise entries.
//
//  Before Use:
//  - The ROOT file created by Converter.cc code is necessally.
//
//  Usage :
//  >>root -l
//  >>.L $MACRO/DetectorSet.cc+
//  >>.L $MACRO/AnalysisSet.cc+
//  >>.L $MACRO/DataSet.cc+
//  >>.L TrackVertexFitter.cc+
//  >>TrackVertexFitter([Ifilename], [Ofilename], [MAXLOOP], [THRESHOLD], [WIDTH], [Verbose])
//
//  [Ifilename]/string : input file name (path of root file)
//  [Ofilename]/string : output file name (path of root file)
//                       If Ofileanme is NULL, output file name is created Int_tmaticaly (Run[Run#]_TrackVertex.root)
//  [MAXLOOP]/int      : maximum number of iterations allowed in the algorithm
//  [THRESHOLD]/int    : threshold of total charge in model
//  [WIDTH]/double     : threshold value to determine when a data point fits a model [mm]
//	[Verbose]/int      : verbose = -1:quiet, 0:normal, 1:details, 2:for debug
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
#include "TMinuit.h"
#include "TRandom3.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TTree.h"
#include "TVector3.h"
// For RANSAC
#include "DetectorSet.hh"
#include "AnalysisSet.hh"
#include "DataSet.hh"
#include "LinkDef.h"
using namespace std;

typedef array<Int_t, 2> arrayI2;
typedef array<Double_t, 3> arrayD3;
typedef array<Double_t, 4> arrayD4;
typedef array<TVector3, 2> arrayV2;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Parameters
#ifndef __FILENAME__
#define __FILENAME__ (strrchr(__FILE__,'/') ? strrchr(__FILE__,'/')+1 : __FILE__)
#endif

Int_t MaxEventVerbose2 = 30;	// when verbose==2, analysis will stop at MaxEventVerbose2 events

const Int_t MAXTRACK = 5;		// Number of maximum track that RANSAC calculates
Double_t MinRange = 0;			// Minimum z-axis [mm]
Double_t MaxRange = 170;		// Maximum z-axis [mm]

TVector3 zero(0,0,0);			// zero vector
PadData GlobalPad;				// Global PadData Class for minuit
TrackData GlobalTrack;			// Global TrackData Class for minuit

DetectorSet *detector = new DetectorSet();
AnalysisSet *analysis = new AnalysisSet();
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Functions
void Usage_TrackVertexFitter(Int_t verbose);

// For RANSAC
arrayI2 GetSample(Int_t size);											// Two points select randomly
arrayV2 GetParamWithSample(arrayD4 sample1, arrayD4 sample2);			// Calculation of 3D line from tow vectors
Double_t GetError(arrayV2 model, arrayD4 p);							// Square of the distance between the point and the line
Double_t GetMean(arrayV2 model, PadData data);							// Mean value of square of distance
void CalFirstEndPoint(PadData& pad, TrackData& track, Int_t itrack);	// Calculation of first and end point of track
void ReturnDummyResult(PadData& input, PadData& pad, TrackData& track);	// Return the dummy result
// RANSAC
Bool_t RANSAC(PadData& data, PadData& outpad, Int_t loop, Int_t threshold, Double_t width, Int_t itrack, Int_t verbose);			// RANSAC algorithm
Int_t mRANSAC(PadData& input, PadData& outpad, TrackData& outtrack, Int_t loop, Int_t threshold, Double_t width, Int_t verbose);	// multi RANSAC

// For MINUIT
extern void FunctionForTrack(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t ifalg);	// MINUIT function for calculation of track
extern void FunctionForVertex(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t ifalg);	// MINUIT function for calculation of vertex
Double_t GetReducedChi2(TVector3 fita, TVector3 fitb, PadData data);								// Calculation of chi2 value
// MINUIT
void MinuitForTrack(PadData& pad, TrackData& track);		// MINUIT algorithm for track
void MinuitForVertex(TrackData& track, VertexData& vertex);	// MINUIT algorithm for vertex
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// ##### MAIN FUNCTION ##### //
void TrackVertexFitter(TString Ifilename="",
		TString Ofilename="",
		Int_t MAXLOOP=2000,
		Int_t THRESHOLD=12000,
		Double_t WIDTH=5.0,
		Int_t Verbose=0)
{
	//===== Start : Usage & Comments =====//
	if (Ifilename.IsNull()) {
		Usage_TrackVertexFitter(Verbose);
		return;
	}

	if (Verbose!=-1) {
		cout<<endl;
		cout<<"<< ============= "<<__FILENAME__<<" ============= >>"<<endl;
		cout<<"<< This program calculates the track and vertex at pad detector in ACTAR TPC."<<endl;
		cout<<"<< For computing, RANSAC and MINUIT algorithm are used."<<endl;
		cout<<"<< The information of results will be saved in new ROOT File."<<endl;
		cout<<"<< "<<endl<<endl;
	}
	//===== End : Usage & Comments =====//


	//===== Start : Setting Random Seed =====//
	time_t StartTime;
	srand(time(&StartTime));
	Int_t seed = rand();
	gRandom = new TRandom3(seed);
	//===== End : Setting Random Seed =====//


	//===== Start : File open =====//
	// Input TFile
	TFile *Ifile = analysis->FileOpen(Ifilename);
	if (!(analysis->CheckOpenTFile(Ifile))) return;

	// Input TTree : conv
	TTree *Itree = (TTree*)Ifile->Get("conv");
	if(!(analysis->CheckOpenTTree(Itree))) return;

	PadData *pad = new PadData();
	MayaData *maya = new MayaData();
	DSSDData *dssd = new DSSDData();
	TBranch *bpad = Itree->GetBranch("pad");
	TBranch *bmaya = Itree->GetBranch("maya");
	TBranch *bdssd = Itree->GetBranch("dssd");
	bpad->SetAddress(&pad);
	bmaya->SetAddress(&maya);
	bdssd->SetAddress(&dssd);

	// Input TTree : Information
	Int_t run = 0;
	Double_t velocity=0;
	TString *type=0;
	TTree *Iinfo = (TTree*)Ifile->Get("Information");
	Iinfo->SetBranchAddress("Run",          &run);
	Iinfo->SetBranchAddress("DetectorType", &type);
	Iinfo->SetBranchAddress("Velocity",     &velocity);
	Iinfo->GetEntry(0);
	TString tname = type->Data();
	detector->SetType(tname);
	Double_t dminx = detector->DetectorMinX();
	Double_t dminy = detector->DetectorMinY();
	Double_t dminz = detector->DetectorMinZ();
	Double_t dmaxx = detector->DetectorMaxX();
	Double_t dmaxy = detector->DetectorMaxY();
	Double_t dmaxz = detector->DetectorMaxZ();
	//===== End : File open =====//


	//===== Start : Recreate File =====//
	// Output TFile
	if (Ofilename.IsNull()) {
		Ofilename = Form("Run%d_TrackVertex.root",run);
	}
	TFile *Ofile = analysis->CreateNewFile(Ofilename, "Track & Vertex Fit", Verbose);

	// Output TTree : Information
	Int_t MaxTrack = MAXTRACK;
	TTree *infotree = Iinfo->CloneTree();
	infotree->Branch("MaxTrack",     &MaxTrack, "MaxTrack/I");
	infotree->Branch("MaxLoop",		 &MAXLOOP,	"MaxLoop/I");
	infotree->Branch("Threshold",    &THRESHOLD,"Threshold/I");
	infotree->Branch("Width",	     &WIDTH,	"Width/D");
	infotree->Branch("MinRange",     &MinRange,	"MinRange/D");
	infotree->Branch("MaxRange",     &MaxRange,	"MaxRange/D");
	infotree->Fill();

	// Output TTree : Track1, Track2, ..., and Track99
	PadData *Opad[MAXTRACK+1];
	MayaData *Omaya[MAXTRACK+1];
	DSSDData *Odssd[MAXTRACK+1];
	TrackData *Otrack[MAXTRACK+1];
	VertexData *Overtex[MAXTRACK+1];
	TTree *Otree[MAXTRACK+1];
	for (auto i=0;i<MAXTRACK+1;i++) {
		Opad[i] = new PadData();
		Omaya[i] = new MayaData();
		Odssd[i] = new DSSDData();
		Otrack[i] = new TrackData();
		Overtex[i] = new VertexData();

		if (i<MAXTRACK) {
			Otree[i] = new TTree(Form("Track%d",i+1),Form("Number of Tracks = %d",i+1));
		} else {
			Otree[i] = new TTree("Track99",Form("Number of Tracks > %d",MAXTRACK));
		}

		Otree[i]->Branch("pad",		&Opad[i],	16000,0);
		Otree[i]->Branch("maya",	&Omaya[i],	16000,0);
		Otree[i]->Branch("dssd",	&Odssd[i],	16000,0);
		Otree[i]->Branch("track",	&Otrack[i],	16000,0);
		Otree[i]->Branch("vertex",	&Overtex[i],16000,0);
	}
	//===== End : Recreate File =====//


	//===== Start : Show Status =====//
	if (Verbose!=-1) {
		cout<<"# Start Status "<<endl;
		cout<<"== Run Stats            "<<endl;
		cout<<"|- Random Seed        : "<<seed<<endl;
		cout<<"|- Input File name    : "<<Ifile->GetName()<<endl;
		cout<<"|- Output File name   : "<<Ofile->GetName()<<endl;
		cout<<"== RANSAC Parameters    "<<endl;
		cout<<"|- Max loop of RANSAC : "<<MAXLOOP<<endl;
		cout<<"|- Length of RASAC    : "<<THRESHOLD<<endl;
		cout<<"|- Width of RASAC     : "<<WIDTH<<endl;
		cout<<"|- Max No. tracks     : "<<MAXTRACK<<endl;
		cout<<"|- Drift Velocity     : "<<velocity<<endl;
		cout<<"|- Minimum Range of z : "<<MinRange<<endl;
		cout<<"|- Maximum Range of z : "<<MaxRange<<endl;
		cout<<"== Beam Informations    "<<endl;
		cout<<"|- Beam Point         : ("<<detector->BeamPositionX()<<", "<<detector->BeamPositionY()<<", "<<detector->BeamPositionZ()<<")"<<endl;
		cout<<"|- Beam Direction     : ("<<detector->BeamDirectionX()<<", "<<detector->BeamDirectionY()<<", "<<detector->BeamDirectionZ()<<")"<<endl;
		cout<<"# Finish Status "<<endl<<endl;
	}
	//===== End : Show Status =====//


	//===== Start : Analysis =====//
	// Variables
	time_t EndTime;
	Int_t maxevent = Itree->GetEntries();

	PadData randata;
	PadData outpad;
	MayaData outmaya;
	DSSDData outdssd;
	TrackData outtrack;
	VertexData outvertex;

	Int_t index[MAXTRACK+1] = {0};
	Int_t itrack = 0;
	Double_t fpoint = 0;
	Double_t epoint = 0;
	TVector3 cordirection(0,0,0);

	Double_t coeff[6] = {0};
	Double_t mincoeff = 0;
	TVector3 reachpoint(0,0,0);

	// Loop for all entries
	if (Verbose!=-1) {
		cout<<"# Start  : Calculation of number of tracks."<<endl;
		cout<<"|- Total entires are "<<maxevent<<endl;
	}
	for (auto iEvent=0;iEvent<maxevent;iEvent++) {
		// Show Status
		if (iEvent%100==0&&iEvent!=0&&Verbose!=-1) {
			EndTime = time(nullptr);
			if (Verbose==0) cout<<"|- Current entry is  "<<iEvent<<" ; "<<fixed<<setprecision(2)<<100.*iEvent/maxevent<<"% ("<<EndTime-StartTime<<" sec)"<<"\r"<<flush;
			if (Verbose==1) {
				cout<<"|- Current entry is  "<<iEvent<<" ; "<<fixed<<setprecision(2)<<100.*iEvent/maxevent<<"% ("<<EndTime-StartTime<<" sec)"<<endl;
				cout<<"|- Show Result of Previous Event "<<endl;
				cout<<" |- event ID         : "<<outpad.GetOrgEvent()<<endl;
				cout<<" |- Number of Tracks : "<<outtrack.Size()<<endl;
				cout<<"  |- i-th Track ; fitA(x,y,z)    ; fitB(x,y,z) [l = fitA + t*fitB]"<<endl;
				for (auto i=0;i<outtrack.Size();i++) {
					cout<<"  |- "<<i+1<<"          ; ("<<outtrack.GetPositionX(i)<<", "<<outtrack.GetPositionY(i)<<", "<<outtrack.GetPositionZ(i)<<") ; ";
					cout<<"("<<outtrack.GetDirectionX(i)<<", "<<outtrack.GetDirectionY(i)<<", "<<outtrack.GetDirectionZ(i)<<")"<<endl;
				}
				cout<<"  |- i-th Track ; Energy      ; Length "<<endl;
				for (auto i=0;i<outtrack.Size();i++) {
					cout<<"  |- "<<i+1<<"          ; "<<outtrack.GetEnergy(i)<<"       ; "<<outtrack.GetLength(i)<<endl;
				}
				cout<<" |- Vertex "<<endl;
				cout<<"  |- ("<<outvertex.GetVertexX()<<", "<<outvertex.GetVertexY()<<" ,"<<outvertex.GetVertexZ()<<") ";
				cout<<" +- ("<<outvertex.GetVertexErrX()<<", "<<outvertex.GetVertexErrY()<<" ,"<<outvertex.GetVertexErrZ()<<") "<<endl;
				cout<<endl;
			}
		}

		// Get Entry
		bpad->GetEntry(iEvent);
		bmaya->GetEntry(iEvent);
		bdssd->GetEntry(iEvent);

		// Copy & Clear Classes
		randata.CopyStr(pad);
		outmaya.CopyStr(maya);
		outdssd.CopyStr(dssd);
		outpad.ClearStr();
		outtrack.ClearStr();
		outvertex.ClearStr();

		// multi RANSAC
		itrack = mRANSAC(randata, outpad, outtrack, MAXLOOP, THRESHOLD, WIDTH, Verbose);

		// MINUIT for Vertex
		MinuitForVertex(outtrack, outvertex);
		outpad.CalLength(outvertex);
		outtrack.CalLength(outpad, outvertex);

		// Track Direction Check
		for (auto i=0;i<itrack;i++) {
			fpoint = outpad.GetY(outpad.FindFpoint(i+1));	// Y-value of first point of (i+1)-th track
			epoint = outpad.GetY(outpad.FindEpoint(i+1));	// Y-value of end point of (i+1)-th track

			if (abs(fpoint-outvertex.GetVertexY())<1) {	// Y-value of first point of tarck and vertex is same value
				if (outtrack.GetDirectionX(i)<0) {		// X-Direction should be plus
					cordirection.SetXYZ(-outtrack.GetDirectionX(i), -outtrack.GetDirectionY(i), -outtrack.GetDirectionZ(i));
					outtrack.SetDirection(i, cordirection);
				}
			} else if (fpoint>outvertex.GetVertexY()) {	// Y-value of first point of track is larger than vertex point (1st or 2nd quadrant from the view of vertex)
				if (epoint>fpoint) {					// End point of track is farther from vertex than first point (maybe 1st quadrant)
					if (outtrack.GetDirectionX(i)<0) {	// X-Direction should be plus
						cordirection.SetXYZ(-outtrack.GetDirectionX(i), -outtrack.GetDirectionY(i), -outtrack.GetDirectionZ(i));
						outtrack.SetDirection(i, cordirection);
					}
				} else if (epoint<fpoint) {				// End point of track is closer from vertex than first poin (maybe 2nd quadrant)
					if (outtrack.GetDirectionX(i)>0) {	// X-Direction should be minus
						cordirection.SetXYZ(-outtrack.GetDirectionX(i), -outtrack.GetDirectionY(i), -outtrack.GetDirectionZ(i));
						outtrack.SetDirection(i, cordirection);
					}
				} else {								// End point and first point of track have same Y-value (beam direction)
					if (outtrack.GetDirectionX(i)<0) {	// X-Direction should be plus
						cordirection.SetXYZ(-outtrack.GetDirectionX(i), -outtrack.GetDirectionY(i), -outtrack.GetDirectionZ(i));
						outtrack.SetDirection(i, cordirection);
					}
				}
			} else {									// Y-value of first point of track is smaller than vertex point (3rd or 4th quadrant from the view of vertex)
				if (epoint>fpoint) {					// End point of tarck is closer from vertex than first point (maybe 3rd quadrant)
					if (outtrack.GetDirectionX(i)>0) {	// X-Direction should be minus
						cordirection.SetXYZ(-outtrack.GetDirectionX(i), -outtrack.GetDirectionY(i), -outtrack.GetDirectionZ(i));
						outtrack.SetDirection(i, cordirection);
					}
				} else if (epoint<fpoint) {				// End point of track is farther from vertex than first point (maybe 4th quadrant)
					if (outtrack.GetDirectionX(i)<0) {	// X-Direction should be plus
						cordirection.SetXYZ(-outtrack.GetDirectionX(i), -outtrack.GetDirectionY(i), -outtrack.GetDirectionZ(i));
						outtrack.SetDirection(i, cordirection);
					}
				} else {								// End point and first point of track have same Y-value (beam direction)
					if (outtrack.GetDirectionX(i)<0) {	// X-Direction should be plus
						cordirection.SetXYZ(-outtrack.GetDirectionX(i), -outtrack.GetDirectionY(i), -outtrack.GetDirectionZ(i));
						outtrack.SetDirection(i, cordirection);
					}
				}
			}

			// Reaching Point of Track
			for (auto j=0;j<6;j++) coeff[j] = -1;
			if (outtrack.GetDirectionX(i)!=0) {
				coeff[0] = (dminx-outtrack.GetPositionX(i))/outtrack.GetDirectionX(i);
				coeff[1] = (dmaxx-outtrack.GetPositionX(i))/outtrack.GetDirectionX(i);
			}
			if (outtrack.GetDirectionY(i)!=0) {
				coeff[2] = (dminy-outtrack.GetPositionY(i))/outtrack.GetDirectionY(i);
				coeff[3] = (dmaxy-outtrack.GetPositionY(i))/outtrack.GetDirectionY(i);
			}
			if (outtrack.GetDirectionZ(i)!=0) {
				coeff[4] = (dminz-outtrack.GetPositionZ(i))/outtrack.GetDirectionZ(i);
				coeff[5] = (dmaxz-outtrack.GetPositionZ(i))/outtrack.GetDirectionZ(i);
			}

			mincoeff = DBL_MAX;
			for (auto j=0;j<6;j++) {
				if (coeff[j]>0&&coeff[j]<mincoeff) mincoeff = coeff[j];
			}

			reachpoint = outtrack.GetPosition(i);
			reachpoint += mincoeff*outtrack.GetDirection(i);
			outtrack.SetReachingPoint(i, reachpoint);
		}


		if (Verbose==2) {
			outpad.ShowStat("Main Function", "Fianl Result");
			outmaya.ShowStat("Main Function", "Final Result");
			outdssd.ShowStat("Main Function", "Final Result");
			outtrack.ShowStat("Main Function","Final Result");
			outvertex.ShowStat("Main Function","Final Result");
		}


		// Fill Datas
		if (itrack>0&&itrack<MAXTRACK) {
			Opad[itrack-1]->CopyStr(outpad);
			Opad[itrack-1]->SetEvent(index[itrack-1]);
			Otrack[itrack-1]->CopyStr(outtrack);
			Otrack[itrack-1]->SetEvent(index[itrack-1]);
			Overtex[itrack-1]->CopyStr(outvertex);
			Overtex[itrack-1]->SetEvent(index[itrack-1]);
			Omaya[itrack-1]->CopyStr(outmaya);
			Omaya[itrack-1]->SetEvent(index[itrack-1]);
			Odssd[itrack-1]->CopyStr(outdssd);
			Odssd[itrack-1]->SetEvent(index[itrack-1]);

			Otree[itrack-1]->Fill();
			index[itrack-1]++;
		} else {
			Opad[MAXTRACK]->CopyStr(outpad);
			Opad[MAXTRACK]->SetEvent(index[MAXTRACK]);
			Otrack[MAXTRACK]->CopyStr(outtrack);
			Otrack[MAXTRACK]->SetEvent(index[MAXTRACK]);
			Overtex[MAXTRACK]->CopyStr(outvertex);
			Overtex[MAXTRACK]->SetEvent(index[MAXTRACK]);
			Omaya[MAXTRACK]->CopyStr(outmaya);
			Omaya[MAXTRACK]->SetEvent(index[MAXTRACK]);
			Odssd[MAXTRACK]->CopyStr(outdssd);
			Odssd[MAXTRACK]->SetEvent(index[MAXTRACK]);

			Otree[MAXTRACK]->Fill();
			index[MAXTRACK]++;
		}
		if (Verbose==2&&iEvent==MaxEventVerbose2) break;
	}

	if (Verbose!=-1) {
		cout<<endl;
		cout<<"# End  : Calculation of number of tracks."<<endl<<endl;
	}
	//===== End : Analysis =====//


	//===== Start : Final Result =====//
	Ofile->Write();
	if (Verbose!=-1) {
		EndTime = time(nullptr);
		cout<<"## Final Status "<<endl;
		cout<<"== Run Status "<<endl;
		cout<<"|- Input File name     : "<<Ifile->GetName()<<endl;
		cout<<"|- Output File name    : "<<Ofile->GetName()<<endl;
		cout<<"== RANSAC Status "<<endl;
		cout<<"|- Max loop of RANSAC  : "<<MAXLOOP<<endl;
		cout<<"|- Threshold of Charge : "<<THRESHOLD<<endl;
		cout<<"|- Width of RASAC      : "<<WIDTH<<endl;
		cout<<"|- Max No. tracks      : "<<MAXTRACK<<endl;
		cout<<"|- Drift Velocity      : "<<velocity<<endl;
		cout<<"== Fitting Results "<<endl;
		cout<<"|- Total Event         : "<<maxevent<<endl;
		for (auto i=0;i<=MAXTRACK;i++) {
			if (i<MAXTRACK) {
				cout<<"|- "<<i+1<<" Tracks            : "<<index[i]<<" ("<<setprecision(3)<<(100.*index[i]/maxevent)<<"%)"<<endl;
			} else {
				cout<<"|- 99Tracks            : "<<index[i]<<" ("<<setprecision(3)<<(100.*index[i]/maxevent)<<"%)"<<endl;
			}
		}
		cout<<"|- Time                : "<<EndTime-StartTime<<"sec"<<endl;
		cout<<"## Final Status "<<endl<<endl;
	}
	//===== End : Final Result =====//
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Usage_TrackVertexFitter(Int_t verbose) {
	TString str = __FILENAME__;
	str.Remove(str.Length()-3);
	if (verbose!=-1) {
		cout<<"Usage : "<<endl;
		cout<<">> .L $MACRO/DetectorSet.cc+"<<endl;
		cout<<">> .L $MACRO/AnalysisSet.cc+"<<endl;
		cout<<">> .L $MACRO/DataSet.cc+"<<endl;
		cout<<">> .L "<<__FILENAME__<<"+"<<endl;
		cout<<">> "<<str<<"([Ifilename] [Ofilename] [MAXLOOP] [THRESHOLD] [WIDTH] [Verbose])"<<endl<<endl;
		cout<<"[Ifilename]/string : input file name (path of root file)"<<endl;
		cout<<"[Ofilename]/string : output file name (path of root file)"<<endl;
		cout<<"                     If Ofileanme is NULL, output file name is created Int_tmaticaly. (Run[Run#]_TrackVertex.root)"<<endl;
		cout<<"[MAXLOOP]/int      : maximum number of iterations allowed in the algorithm"<<endl;
		cout<<"[THRESHOLD]/int    : threshold of total charge in model"<<endl;
		cout<<"[WIDTH]/double     : threshold value to determine when a data point fits a model [mm]"<<endl;
		cout<<"[Verbose]/int      : -1:quiet, 0:normal, 1:details, 2:for debug"<<endl;
		cout<<endl;
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
arrayI2 GetSample(Int_t size) {
	arrayI2 index = {0,0};
	while (index[0]==index[1]) {
		index[0] = gRandom->Uniform(0,size);
		index[1] = gRandom->Uniform(0,size);
	}
	return index;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
arrayV2 GetParamWithSample(arrayD4 sample1, arrayD4 sample2) {
	TVector3 p1(sample1[0],sample1[1],sample1[2]);
	TVector3 p2(sample2[0],sample2[1],sample2[2]);
	TVector3 u = p2-p1;
	arrayV2 r = {p1, u.Unit()};
	return r;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t GetError(arrayV2 model, arrayD4 p) {
	TVector3 a = {p[0]-model[0][0], p[1]-model[0][1], p[2]-model[0][2]};
	TVector3 H = model[0]+a.Dot(model[1])*model[1];
	TVector3 d = {p[0]-H[0], p[1]-H[1], p[2]-H[2]};
	return d.Mag2();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t GetMean(arrayV2 model, PadData data) {
	Double_t sumvalue = 0;
	Double_t sumcharge = data.CalTotalEnergy();
	for (auto i=0;i<data.Size();i++) {
		sumvalue += GetError(model, data.GetXYZE(i))*data.GetEnergy(i);
	}
	return sumvalue/sumcharge;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void CalFirstEndPoint(PadData& pad, TrackData& track, Int_t itrack) {
	if (track.Size()!=1) {
		analysis->SetErrorComment("Track size is not match");
		analysis->SetErrorComment("There should be 1");
		analysis->PrintErrorMsg("CalFirstEndPoint", 0);
	}

	// Valiables
	Double_t fpoint = detector->PadMaxX();
	Double_t epoint = 0;
	Double_t fperr = DBL_MAX;
	Double_t eperr = DBL_MAX;
	Int_t findex = 0;
	Int_t eindex = 0;
	Double_t Err = 0;
	arrayV2 model;
	model[0] = track.GetPosition(0);
	model[1] = track.GetDirection(0);

	// Search the start point and end point
	for (auto i=0;i<pad.Size();i++) {
		Err = GetError(model, pad.GetXYZE(i));
		// first point
		if (pad.GetX(i)<fpoint) {
			fpoint = pad.GetX(i);
			fperr = Err;
			findex = i;
		} else if (pad.GetX(i)==fpoint) {
			if (Err<fperr) {
				fpoint = pad.GetX(i);
				fperr = Err;
				findex = i;
			}
		}

		// end point
		if (pad.GetX(i)>epoint) {
			epoint = pad.GetX(i);
			eperr = Err;
			eindex = i;
		} else if (pad.GetX(i)==epoint) {
			if (Err<eperr) {
				epoint = pad.GetX(i);
				eperr = Err;
				eindex = i;
			}
		}
	}

	// Input the informations (first point and end point) to the data
	for (auto i=0;i<pad.Size();i++) {
		pad.SetTrackNumber(i, itrack+1);
		if (i==findex) pad.SetFpoint(i, itrack+1);
		if (i==eindex) pad.SetEpoint(i, itrack+1);
	}
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ReturnDummyResult(PadData& input, PadData& pad, TrackData& track) {
	pad.ClearStr();
	pad.AddStr(input);
	for (auto i=0;i<input.Size();i++) {
		pad.SetFpoint(i, -1);
		pad.SetEpoint(i, -1);
		pad.SetTrackNumber(i, -1);
	}
	track.ClearStr(0);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Bool_t RANSAC(
		PadData& data,			// Input data set
		PadData& outpad,		// Output data set (pad)
		TrackData& outtrack,	// Output data set (track)
		Int_t loop,				// Maximum number of iterations allowed in the algorithm
		Int_t threshold,		// Number of charge required to assert that a model fits well to data
		Double_t width,			// The value to determine when a data point fits a model
		Int_t itrack,			// It means the itrack-th track
		Int_t verbose)
{
	// Random sample consensus (RANSAC) analysis
	// <https://en.wikipedia.org/wiki/Random_sample_consensus>

	Bool_t FissionFlag = kFALSE;

	//===== Start : First Cut =====//
	// If total charge less than threshold, return 0
	if (data.CalTotalEnergy()<threshold) {
		ReturnDummyResult(data, outpad, outtrack);
		return FissionFlag;
	}
	//===== End : First Cut =====//

	//===== Start : RANSAC =====//
	// Valiables
	Int_t iterations = 0;
	Int_t size = data.Size();
	arrayI2 rndpoint = {0,0};
	arrayD4 sample1 = {0,0,0,0};
	arrayD4 sample2 = {0,0,0,0};
	arrayV2 maybemodel = {zero, zero};
	arrayV2 bestmodel = {zero, zero};
	arrayV2 bestmodel_err = {zero, zero};
	PadData alsoinliers;
	PadData bestinliers;
	Double_t besterr = DBL_MAX;
	Double_t thiserr = 0;

	// Main loop
	while (iterations<loop) {
		// Initialization
		maybemodel[0].SetXYZ(0,0,0);
		maybemodel[1].SetXYZ(0,0,0);
		alsoinliers.ClearStr();

		// 2 randomly selected values from data
		rndpoint = GetSample(size);
		sample1 = data.GetXYZE(rndpoint[0]);
		sample2 = data.GetXYZE(rndpoint[1]);
		maybemodel = GetParamWithSample(sample1, sample2);

		// For every point in data,
		// if point fits maybemodel with an error smaller than WIDTH,
		// add point to alsoinliers
		for (auto iPoint=0;iPoint<size;iPoint++) {
			if (GetError(maybemodel,data.GetXYZE(iPoint))>width*width) continue;
			if (data.GetZ(iPoint)<MinRange||data.GetZ(iPoint)>MaxRange) continue;
			alsoinliers.AddStr(data, iPoint);
		}

		// If the total charge in alsoinliers is larger than THRESHOLD
		// This implies that we may have found a good model
		// now test how good it is
		if (alsoinliers.CalTotalEnergy()>threshold) {
			thiserr = GetMean(maybemodel, alsoinliers);
			if (thiserr<besterr) {
				bestmodel = maybemodel;
				besterr = thiserr;
				bestinliers = alsoinliers;
			}
		}
		iterations++;
	}
	//===== End : RANSAC =====//


	//===== Start : Second Cut =====//
	// no track
	if (bestmodel[0]==zero&&bestmodel[1]==zero) {
		ReturnDummyResult(data, outpad, outtrack);
		return FissionFlag;
	}
	//===== End : Second Cut =====//


	//===== Start : Minuit for Track =====//
	outpad.ClearStr();
	outpad.AddStr(bestinliers);
	MinuitForTrack(outpad, outtrack);
	//===== End : Minuit for Track =====//


	//===== Start : Calculation of First and End Point =====//
	CalFirstEndPoint(outpad, outtrack, itrack);
	//===== End : Calculation of First and End Point =====//


	//===== Start : Find fission event =====//
	Double_t totalEnergy = 0;
	Double_t reducedchi2 = 0;
	Bool_t flag1 = kFALSE;
	Bool_t flag2 = kFALSE;

	PadData dummypad1;
	PadData dummypad2;
	TrackData dummytrack1;
	TrackData dummytrack2;
	TVector3 beampos = detector->BeamPosition();
	// If track cross the beam area
	if ((beampos.y()-outpad.GetY(outpad.FindFpoint(itrack+1)))*(beampos.y()-outpad.GetY(outpad.FindEpoint(itrack+1)))<0) {
		// Dividing upper side and lower side
		for (auto i=0;i<outpad.Size();i++) {
			if (outpad.GetY(i)>beampos.y()) {
				dummypad1.AddStr(outpad, i);
			} else {
				dummypad2.AddStr(outpad, i);
			}
		}

		//===== Start : Third Cut =====//
		// If total energy of borth track less than threshold, return 0
		if (dummypad1.CalTotalEnergy()<threshold&&dummypad2.CalTotalEnergy()<threshold) {
			ReturnDummyResult(data, outpad, outtrack);
			return FissionFlag;
		}
		//===== End : Third Cut =====//


		// Check upper side track
		// If total energy less than threshold, data is removed from bestinliers
		if (dummypad1.CalTotalEnergy()<threshold) {
			bestinliers.Erase(dummypad1);
			itrack--;
		} else {
			CalFirstEndPoint(dummypad1, outtrack, itrack);

			MinuitForTrack(dummypad1, dummytrack1);
			dummytrack1.AddTrackNumber(itrack+1);
			dummytrack1.AddChi2(GetReducedChi2(bestmodel[0], bestmodel[1], dummypad1));
			dummytrack1.AddEnergy(dummypad1.CalTotalEnergy());
			dummytrack1.AddLength(0);			//Dummy
			dummytrack1.AddReachingPoint(zero);	//Dummy
			flag1 = kTRUE;
		}

		// Check lower side track
		// If total energy less than threshold, data is removed from bestinliers
		if (dummypad2.CalTotalEnergy()<threshold) {
			bestinliers.Erase(dummypad2);

		} else {
			CalFirstEndPoint(dummypad2, outtrack, itrack+1);

			MinuitForTrack(dummypad2, dummytrack2);
			dummytrack2.AddTrackNumber(itrack+2);
			dummytrack2.AddChi2(GetReducedChi2(bestmodel[0], bestmodel[1], dummypad2));
			dummytrack2.AddEnergy(dummypad2.CalTotalEnergy());
			dummytrack2.AddLength(0);			//Dummy
			dummytrack2.AddReachingPoint(zero);	//Dummy
			flag2 = kTRUE;
		}

		// Fill the data
		outpad.ClearStr();
		outtrack.ClearStr();
		if (flag1) {
			outpad.AddStr(dummypad1);
			outtrack.AddStr(dummytrack1);
		}
		if (flag2) {
			outpad.AddStr(dummypad2);
			outtrack.AddStr(dummytrack2);
		}
		if (flag1&&flag2) {
			FissionFlag = kTRUE;
		}

		// If track does not cross the beam area
	} else {
		// Fill the data of track (Pad data already filled above)
		outtrack.AddTrackNumber(itrack+1);
		outtrack.AddChi2(GetReducedChi2(bestmodel[0], bestmodel[1], outpad));
		outtrack.AddEnergy(outpad.CalTotalEnergy());
		outtrack.AddLength(0);				//Dummy
		outtrack.AddReachingPoint(zero);	//Dummy
	}
	//===== End : Find fission event =====//

	if (verbose==2) {
		bestinliers.ShowStat("RANSAC","bestinlier data");
		outtrack.ShowStat("RANSAC","outtrack data");
	}

	//===== Start : Erase data =====//
	data.Erase(bestinliers);
	//===== End : Erase data =====//

	if (verbose==2) data.ShowStat("RANSAC","After \"Erase\" the data");

	return FissionFlag;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Int_t mRANSAC(
		PadData& input,		// Input data set
		PadData& outpad,    // Output data set (pad)
		TrackData& outtrack,// Output data set (track)
		Int_t loop,         // Maximum number of iterations allowed in the algorithm
		Int_t threshold,    // Number of charge required to assert that a model fits well to data
		Double_t width,		// The value to determine when a data point fits a model
		Int_t verbose)
{
	// Initialization
	outpad.ClearStr();
	outtrack.ClearStr();

	// Valiables
	TVector3 fitA = zero;
	TVector3 fitB = zero;
	TVector3 errA = zero;
	TVector3 errB = zero;
	PadData dummypad;
	TrackData dummytrack;

	// If the number of points in the data is less than 2, return 0
	if (input.Size()<2) {
		outpad.CopyStr(input);
		return 0;
	}

	// RANSAC analysis is performed until the best model is not found.
	Int_t itrack = 0;
	Bool_t flag = kFALSE;
	while (kTRUE) {
		dummypad.ClearStr();
		dummytrack.ClearStr();
		flag = RANSAC(input, dummypad, dummytrack, loop, threshold, width, itrack, verbose);

		if (dummytrack.GetPosition(0)==zero&&dummytrack.GetDirection(0)==zero) {
			outpad.AddStr(dummypad);
			if (verbose==2) outpad.ShowStat("mRANSAC","output data of RANSAC (no Track)");
			break;
		} else {
			outpad.AddStr(dummypad);
			outtrack.AddStr(dummytrack);
			itrack++;
			if (flag) itrack++;
			if (verbose==2) {
				outpad.ShowStat("mRANSAC",Form("output data of RANSAC (%d-th track)",itrack));
				outtrack.ShowStat("mRANSAC", Form("output data of RANSAC (%d-th track)",itrack));
			}
		}
	}
	return itrack;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FunctionForTrack(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t ifalg) {
	// Since arguments are defined by the MINUIT algorith, nobody can change them.
	arrayV2 minuitmodel;
	minuitmodel[0] = {par[0],par[1],par[2]};
	minuitmodel[1] = {par[3],par[4],par[5]};
	Double_t err = 0;
	Double_t chisq = 0;

	for (auto i=0;i<GlobalPad.Size();i++) {
		err = GetError(minuitmodel, GlobalPad.GetXYZE(i));
		chisq += err;
	}
	f = chisq;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void FunctionForVertex(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t ifalg) {
	// Since arguments are defined by the MINUIT algorith, nobody can change them.
	arrayD4 p = {par[0], par[1], par[2], 1.};
	arrayV2 model;
	Double_t err = 0;
	Double_t chisq = 0;

	for (auto i=0;i<GlobalTrack.Size();i++) {
		model[0] = GlobalTrack.GetPosition(i);
		model[1] = GlobalTrack.GetDirection(i);
		err = GetError(model, p)/GlobalTrack.GetChi2(i);
		chisq += err;
	}

	f = chisq;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t GetReducedChi2(TVector3 fita, TVector3 fitb, PadData data) {
	arrayV2 minuitmodel;
	minuitmodel[0] = fita;
	minuitmodel[1] = fitb;
	Double_t err = 0;
	Double_t chisq = 0;
	for (auto i=0;i<data.Size();i++) {
		err = GetError(minuitmodel, data.GetXYZE(i));
		chisq += err;
	}
	return chisq/(Double_t)(data.Size()-6.);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MinuitForTrack(PadData& pad, TrackData& track) {
	// Initialization
	GlobalPad.ClearStr();
	GlobalPad.AddStr(pad);
	track.ClearStr();
	track.SetOrgEvent(pad.GetOrgEvent());

	// Valiables
	Double_t Pos[3] = {0};
	Double_t PosErr[3] = {0};
	Double_t Dir[3] = {0};
	Double_t DirErr[3] = {0};
	Double_t delta = 10.;

	// Minuit
	TMinuit *gMinuit = new TMinuit(6);
	gMinuit->SetPrintLevel(-1);	//-1:quiet, 0:noraml, 1:verbose
	gMinuit->SetErrorDef(1.);

	// Set Paramters
	Double_t maxX = detector->PadMaxX();
	Double_t maxY = detector->PadMaxY();
	Double_t maxZ = detector->PadMaxZ();
	gMinuit->DefineParameter(0, "Ax", maxX/2., 0.1, 0.-delta, maxX+delta);
	gMinuit->DefineParameter(1, "Ay", maxY/2., 0.1, 0.-delta, maxY+delta);
	gMinuit->DefineParameter(2, "Az", maxZ/2., 0.1, 0.-delta, maxZ+delta);
	gMinuit->DefineParameter(3, "Bx", 0., 0.001, -1.-delta, 1.+delta);
	gMinuit->DefineParameter(4, "By", 0., 0.001, -1.-delta, 1.+delta);
	gMinuit->DefineParameter(5, "Bz", 0., 0.001, -1.-delta, 1.+delta);

	// Calculation MINUIT using Migrad algorithm
	gMinuit->SetFCN(FunctionForTrack);
	gMinuit->Migrad();

	// Get Results
	gMinuit->GetParameter(0, Pos[0], PosErr[0]);
	gMinuit->GetParameter(1, Pos[1], PosErr[1]);
	gMinuit->GetParameter(2, Pos[2], PosErr[2]);
	gMinuit->GetParameter(3, Dir[0], DirErr[0]);
	gMinuit->GetParameter(4, Dir[1], DirErr[1]);
	gMinuit->GetParameter(5, Dir[2], DirErr[2]);

	TVector3 Position(Pos[0], Pos[1], Pos[2]);
	TVector3 PositionErr(PosErr[0], PosErr[1], PosErr[2]);
	TVector3 Direction(Dir[0], Dir[1], Dir[2]);
	TVector3 DirectionErr(DirErr[0], DirErr[1], DirErr[2]);

	track.AddPosition(Position);
	track.AddPositionErr(PositionErr);
	track.AddDirection(Direction);
	track.AddDirectionErr(DirectionErr);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void MinuitForVertex(TrackData& track, VertexData& vertex) {
	// Initialization
	GlobalTrack.ClearStr();
	GlobalTrack.AddStr(track);

	GlobalTrack.AddPosition(detector->BeamPosition());
	GlobalTrack.AddDirection(detector->BeamDirection());
	GlobalTrack.AddChi2(25.0);

	vertex.ClearStr();
	vertex.SetOrgEvent(track.GetOrgEvent());

	// Valiables
	Double_t Vertex[3] = {0};
	Double_t Vertex_err[3] = {0};
	Double_t delta = 100.;

	// Minuit
	TMinuit *gMinuit = new TMinuit(3);
	gMinuit->SetPrintLevel(-1); //-1:quiet, 0:noraml, 1:verbose
	gMinuit->SetErrorDef(1.);

	// Set Paramters
	Double_t maxX = detector->PadMaxX();
	Double_t maxY = detector->PadMaxY();
	Double_t maxZ = detector->PadMaxZ();
	gMinuit->DefineParameter(0, "Vx", maxX/2., 0.01, 0.-delta, maxX+delta);
	gMinuit->DefineParameter(1, "Vy", maxY/2., 0.01, 0.-delta, maxY+delta);
	gMinuit->DefineParameter(2, "Vz", maxZ/2., 0.01, 0.-delta, maxZ+delta);

	// Calculation MINUIT using Migrad algorithm
	gMinuit->SetFCN(FunctionForVertex);
	gMinuit->Migrad();

	// Get Results
	gMinuit->GetParameter(0, Vertex[0], Vertex_err[0]);
	gMinuit->GetParameter(1, Vertex[1], Vertex_err[1]);
	gMinuit->GetParameter(2, Vertex[2], Vertex_err[2]);

	TVector3 ver(Vertex[0], Vertex[1], Vertex[2]);
	TVector3 err(Vertex_err[0], Vertex_err[1], Vertex_err[2]);

	vertex.SetVertex(ver);
	vertex.SetVertexErr(err);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

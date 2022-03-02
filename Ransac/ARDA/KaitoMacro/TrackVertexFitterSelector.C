#define TrackVertexFitterSelector_cxx
// The class definition in TrackVertexFitterSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("TrackVertexFitterSelector.C")
// root> T->Process("TrackVertexFitterSelector.C","some options")
// root> T->Process("TrackVertexFitterSelector.C+")
//


#include "TrackVertexFitterSelector.h"
#include <TH2.h>
#include <TStyle.h>
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
#include "TProof.h"
#include "TNamed.h"
// For RANSAC
#include "DetectorSet.hh"
#include "AnalysisSet.hh"
#include "DataSet.hh"
#include "LinkDef.h"
#include <TSystem.h>

using namespace std;

#define MAXLOOP 2000
#define THRESHOLD 12000
#define WIDTH 5.0
#define Verbose 0

typedef array<Int_t, 2> arrayI2;
typedef array<Double_t, 3> arrayD3;
typedef array<Double_t, 4> arrayD4;
typedef array<TVector3, 2> arrayV2;

PadData TrackVertexFitterSelector::GlobalPad;				// Global PadData Class for minuit
TrackData TrackVertexFitterSelector::GlobalTrack;			// Global TrackData Class for minuit


TrackVertexFitterSelector::TrackVertexFitterSelector(TTree*) {
  MinRange = 0;
  MaxRange = 170;
  zero = TVector3(0,0,0);
  detector = new DetectorSet();
  detector->SetType("demo");
  analysis = new AnalysisSet();
}

TrackVertexFitterSelector::~TrackVertexFitterSelector() {
  delete detector;
  delete analysis;
}

// Opens the file and writes the information tree
void TrackVertexFitterSelector::ReadFile(TString Ifilename) {
  fInName = Ifilename;
  TFile *Ifile = analysis->FileOpen(Ifilename);
	if (!(analysis->CheckOpenTFile(Ifile))) return;

  Int_t run = 0;
	Double_t velocity = 0;
	TString *type = 0;
	TTree *Iinfo = (TTree*)Ifile->Get("Information");
	Iinfo->SetBranchAddress("Run",          &run);
	Iinfo->SetBranchAddress("DetectorType", &type);
	Iinfo->SetBranchAddress("Velocity",     &velocity);
	Iinfo->GetEntry(0);

  Ifile->Close();

  TChain *c = new TChain("conv");

  c->Add(Ifilename);

  // Checks if Proof is present and valid
  if (gProof){
    if (gProof->IsValid()) c->SetProof();
  }

  outFileName = Form("Track_%d_%d.root", run, ((int)(velocity * 10.)));

  c->Process(this, outFileName.Data());
}

void TrackVertexFitterSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

void TrackVertexFitterSelector::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   // Option is set to the filename
   TString option = GetOption();

   fProofFile = new TProofOutputFile(option, "M");
   fProofFile->SetOutputFileName(option);
   fFile = fProofFile->OpenFile("UPDATE");

   dminx = detector->DetectorMinX();
   dminy = detector->DetectorMinY();
   dminz = detector->DetectorMinZ();
   dmaxx = detector->DetectorMaxX();
   dmaxy = detector->DetectorMaxY();
   dmaxz = detector->DetectorMaxZ();

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
     Otree[i]->SetDirectory(fFile);
     Otree[i]->AutoSave();
   }

   for (int j = 0; j < MAXTRACK + 1; j++) {
     index[j] = 0;
   }
   itrack = 0;
   fpoint = 0;
   epoint = 0;
   cordirection = TVector3(0,0,0);
   for (int j = 0; j < 6; j++) {
     coeff[j] = 0;
   }
   mincoeff = 0;
   reachpoint = TVector3(0,0,0);


}

Bool_t TrackVertexFitterSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   fReader.SetEntry(entry);

   randata.CopyStr(*pad);
   outmaya.CopyStr(*maya);
   outdssd.CopyStr(*dssd);
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

   return kTRUE;
}

void TrackVertexFitterSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.


   if (fFile) {
     Bool_t cleanup = kFALSE;
     TDirectory *savedir = gDirectory;
     fFile->cd();
     for (auto i=0;i<MAXTRACK+1;i++) {
       Otree[i]->Write(0, TObject::kOverwrite);
     }
     fProofFile->Print();
     fOutput->Add(fProofFile);
     for (auto i=0;i<MAXTRACK+1;i++) {
       Otree[i]->SetDirectory(0);
     }
     gDirectory = savedir;
     fFile->Close();
     // Cleanup, if needed
     if (cleanup) {
       TUrl uf(*(fFile->GetEndpointUrl()));
       SafeDelete(fFile);
       gSystem->Unlink(uf.GetFile());
       SafeDelete(fProofFile);
     }
   }

   for (auto i=0;i<MAXTRACK+1;i++) {
     delete Opad[i];
     delete Omaya[i];
	 delete Odssd[i];
     delete Otrack[i];
	 delete Overtex[i];
   }
}

void TrackVertexFitterSelector::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.


   TFile *Ifile = analysis->FileOpen(fInName);
   if (!(analysis->CheckOpenTFile(Ifile))) return;

   Int_t run = 0;
   Double_t velocity = 0;
   TString *type = 0;
   TTree *Iinfo = (TTree*)Ifile->Get("Information");
   Iinfo->SetBranchAddress("Run",          &run);
   Iinfo->SetBranchAddress("DetectorType", &type);
   Iinfo->SetBranchAddress("Velocity",     &velocity);
   Iinfo->GetEntry(0);

   Int_t MaxTrack = MAXTRACK;
   Int_t MaxLoop = MAXLOOP;
   Int_t Threshold = THRESHOLD;
   Double_t Width = WIDTH;
   TTree *infotree = Iinfo->CloneTree();
   infotree->Branch("MaxTrack",     &MaxTrack, "MaxTrack/I");
   infotree->Branch("MaxLoop",		 &MaxLoop,	"MaxLoop/I");
   infotree->Branch("Threshold",    &Threshold,"Threshold/I");
   infotree->Branch("Width",	     &Width,	"Width/D");
   infotree->Branch("MinRange",     &MinRange,	"MinRange/D");
   infotree->Branch("MaxRange",     &MaxRange,	"MaxRange/D");
   infotree->Fill();

   TFile f(outFileName.Data(), "update");
   f.cd();
   infotree->CloneTree()->Write("", TObject::kOverwrite);
   f.Close();
   Ifile->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
arrayI2 TrackVertexFitterSelector::GetSample(Int_t size) {
	arrayI2 index = {0,0};
	while (index[0]==index[1]) {
		index[0] = gRandom->Uniform(0,size);
		index[1] = gRandom->Uniform(0,size);
	}
	return index;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
arrayV2 TrackVertexFitterSelector::GetParamWithSample(arrayD4 sample1, arrayD4 sample2) {
	TVector3 p1(sample1[0],sample1[1],sample1[2]);
	TVector3 p2(sample2[0],sample2[1],sample2[2]);
	TVector3 u = p2-p1;
	arrayV2 r = {p1, u.Unit()};
	return r;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t TrackVertexFitterSelector::GetError(arrayV2 model, arrayD4 p) {
	TVector3 a = {p[0]-model[0][0], p[1]-model[0][1], p[2]-model[0][2]};
	TVector3 H = model[0]+a.Dot(model[1])*model[1];
	TVector3 d = {p[0]-H[0], p[1]-H[1], p[2]-H[2]};
	return d.Mag2();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
Double_t TrackVertexFitterSelector::GetMean(arrayV2 model, PadData data) {
	Double_t sumvalue = 0;
	Double_t sumcharge = data.CalTotalEnergy();
	for (auto i=0;i<data.Size();i++) {
		sumvalue += GetError(model, data.GetXYZE(i))*data.GetEnergy(i);
	}
	return sumvalue/sumcharge;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TrackVertexFitterSelector::CalFirstEndPoint(PadData& pad, TrackData& track, Int_t itrack) {
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
void TrackVertexFitterSelector::ReturnDummyResult(PadData& input, PadData& pad, TrackData& track) {
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
Bool_t TrackVertexFitterSelector::RANSAC(
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
Int_t TrackVertexFitterSelector::mRANSAC(
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
void TrackVertexFitterSelector::FunctionForTrack(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t ifalg) {
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
void TrackVertexFitterSelector::FunctionForVertex(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t ifalg) {
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
Double_t TrackVertexFitterSelector::GetReducedChi2(TVector3 fita, TVector3 fitb, PadData data) {
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
void TrackVertexFitterSelector::MinuitForTrack(PadData& pad, TrackData& track) {
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

	delete gMinuit;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void TrackVertexFitterSelector::MinuitForVertex(TrackData& track, VertexData& vertex) {
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

	delete gMinuit;
}

///////////////////////////////////////////////////////////////////////////////
//
//                            ShowAnEvent.cc
//
//	Show the result of an event for all detectors.
//
//
//  Before Use:
//
//  Usage :
//  >>root -l
//  >>.L $MACRO/DetectorSet.cc+
//  >>.L $MACRO/AnalsysiSet.cc+
//  >>.L $MACRO/DataSet,cc+
//  >>.L ShowAnEvent.cc+
//  >>ShowAnEvent([Ifilename], [Itreename], [eve])
//
//  [Ifilename]/string : Input file name
//  [Itreename]/string : TTree name
//  [eve]/int          : Event number
//


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// For C++
#include <stdio.h>
#include <iostream>
//For ROOT
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TTreeIndex.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TF1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraph2D.h"
#include "TGraph2DErrors.h"
#include "TPolyLine3D.h"
// For
#include "DetectorSet.hh"
#include "AnalysisSet.hh"
#include "DataSet.hh"

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ShowAnEvent(TString Ifilename, TString Itreename, Int_t eve){
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);

	DetectorSet *detector = new DetectorSet();
	AnalysisSet *analysis = new AnalysisSet();

	//===== Start : Open TFile =====//
	// Input TFile
	TFile *Ifile = analysis->FileOpen(Ifilename);
	if (!analysis->CheckOpenTFile(Ifile)) return;

	// Input TTree : Informations
	TTree *info = (TTree*)Ifile->Get("Information");
	if (!analysis->CheckOpenTTree(info)) return;

	// Input Branch : Informations
	Int_t MaxTrack = 0;
	TString *Type = 0;
	TString *Path = 0;
	info->SetBranchAddress("DetectorType", &Type);
	info->SetBranchAddress("PathType", &Path);
	info->SetBranchAddress("MaxTrack", &MaxTrack);
	info->GetEntry(0);
	detector->SetType(*Type);
	//detector->SetPathType(Path);

	// Input TTree & Branch : tracks
	TTree *tree = (TTree*)Ifile->Get(Itreename.Data());
	if (!analysis->CheckOpenTTree(tree)) return;

	PadData *pad = new PadData();
	MayaData *maya = new MayaData();
	DSSDData *dssd = new DSSDData();
	TrackData *track = new TrackData();
	VertexData *vertex = new VertexData();

	TBranch *Bpad = tree->GetBranch("pad");
	TBranch *Bmaya = tree->GetBranch("maya");
	TBranch *Bdssd = tree->GetBranch("dssd");
	TBranch *Btrack = tree->GetBranch("track");
	TBranch *Bvertex = tree->GetBranch("vertex");

	Bpad->SetAddress(&pad);
	Bmaya->SetAddress(&maya);
	Bdssd->SetAddress(&dssd);
	Btrack->SetAddress(&track);
	Bvertex->SetAddress(&vertex);
	//===== End : Open TFile =====//


	//===== Start : Canvas =====//
	// Comstant Variable
	Double_t DetMaxX = detector->DetectorMaxX();
	Double_t DetMaxY = detector->DetectorMaxY();
	Double_t DetMaxZ = detector->DetectorMaxZ();
	Double_t DetMinX = detector->DetectorMinX();
	Double_t DetMinY = detector->DetectorMinY();
	Double_t DetMinZ = detector->DetectorMinZ();

	Double_t PadMinX = detector->PadMinX();
	Double_t PadMinY = detector->PadMinY();
	Double_t PadMinZ = detector->PadMinZ();
	Double_t PadMaxX = detector->PadMaxX();
	Double_t PadMaxY = detector->PadMaxY();
	Double_t PadMaxZ = detector->PadMaxZ();

	// TCanvas
	// Main canvas
	TCanvas *cEnergy = new TCanvas("cEnergy","cEnergy",1200,800);
	TPad *cEnergyPad = new TPad("cEnergyPad","cEnergyPad",0.02,0.3,0.7,0.7);
	TPad *cEnergyMayaL = new TPad("cEnergyMayaL","cEnergyMayaL",0.02,0.7,0.7,0.98);
	TPad *cEnergyMayaR = new TPad("cEnergyMayaR","cEnergyMayaR",0.02,0.02,0.7,0.3);
	TPad *cEnergyDSSD = new TPad("cEnergyDSSD","cEnergyDSSD",0.7,0.3,0.98,0.7);
	cEnergyPad->Draw();
	cEnergyMayaL->Draw();
	cEnergyMayaR->Draw();
	cEnergyDSSD->Draw();
	cEnergyPad->SetRightMargin(0.01);
	cEnergyPad->SetLeftMargin(0.1);
	cEnergyPad->SetTopMargin(0.01);
	cEnergyPad->SetBottomMargin(0.1);
	cEnergyMayaL->SetRightMargin(0.01);
	cEnergyMayaL->SetLeftMargin(0.1);
	cEnergyMayaL->SetTopMargin(0.01);
	cEnergyMayaL->SetBottomMargin(0.1);
	cEnergyMayaR->SetRightMargin(0.01);
	cEnergyMayaR->SetLeftMargin(0.1);
	cEnergyMayaR->SetTopMargin(0.01);
	cEnergyMayaR->SetBottomMargin(0.1);
	cEnergyDSSD->SetRightMargin(0.01);
	cEnergyDSSD->SetLeftMargin(0.1);
	cEnergyDSSD->SetTopMargin(0.01);
	cEnergyDSSD->SetBottomMargin(0.1);

	// 3D canvas
	TCanvas *cPad3D = new TCanvas("cPad3D", "Pad detector 3D histogram",1200,800);
	cPad3D->Divide(2,2);

	// Bragg curve canvas
	TCanvas *cBragg = new TCanvas("cBragg","Bragg curve",1200,800);
	cBragg->Divide(2,2);
	//===== End : Canvas =====//


	//===== Start : Histograms =====//
	// Histograms for Pad
	TH2D *hPadXYE = new TH2D("hPadXYE","Pad YvsX with E",(DetMaxX-DetMinX)/2.,DetMinX,DetMaxX,(DetMaxY-DetMinY)/2.,DetMinY,DetMaxY);
	TH2D *hPadYZE = new TH2D("hPadYZE","Pad ZvsY with E",(DetMaxY-DetMinY)/2.,DetMinY,DetMaxY,(DetMaxZ-DetMinZ)/2.,DetMinZ,DetMaxZ);
	TH2D *hPadZXE = new TH2D("hPadZXE","Pad ZvsX with E",(DetMaxX-DetMinX)/2.,DetMinX,DetMaxX,(DetMaxZ-DetMinZ)/2.,DetMinZ,DetMaxZ);
	TH2D *hPadXYZ = new TH2D("hPadXYZ","Pad YvsX with Z",(DetMaxX-DetMinX)/2.,DetMinX,DetMaxX,(DetMaxY-DetMinY)/2.,DetMinY,DetMaxY);

	TGraph2D *hPadXYZ3D;

	TH1D *hBragg[4];
	for (Int_t i=0;i<4;i++) {
		hBragg[i] = new TH1D(Form("hBragg%d",i+1),"distance from vertex with energy",200,0,200);
	}

	// Histograms for Maya
	Double_t xbins[] = {DetMinX, DetMinX+38, DetMinX+88, DetMinX+94, DetMinX+144, DetMinX+150, DetMinX+200, DetMaxX};
	Double_t ybins[] = {DetMinZ, DetMinZ+52, DetMinZ+102, DetMinZ+108, DetMinZ+158, DetMaxZ};
	Int_t xbin = ((Int_t)sizeof(xbins)/sizeof(Double_t))-1;
	Int_t ybin = ((Int_t)sizeof(ybins)/sizeof(Double_t))-1;
	TH2D *hMayaRZXE = new TH2D("hMayaRZXE","Right Maya : ZvsX with E",xbin,xbins,ybin,ybins);
	TH2D *hMayaRZXT = new TH2D("hMayaRZXT","Right Maya : ZvsX with T",xbin,xbins,ybin,ybins);
	TH2D *hMayaLZXE = new TH2D("hMayaLZXE","Left Maya : ZvsX with E",xbin,xbins,ybin,ybins);
	TH2D *hMayaLZXT = new TH2D("hMayaLZXT","Left Maya : ZvsX with T",xbin,xbins,ybin,ybins);

	// Histograms for DSSD
	TH2D *hDSSDYZE = new TH2D("hDSSDYZE","DSSD : YvsZ with E",(DetMaxZ-DetMinZ)/2.,DetMinZ+1,DetMaxZ+1,(DetMaxY-DetMinY)/2.,DetMinY,DetMaxY);
	TH2D *hDSSDYZT = new TH2D("hDSSDYZT","DSSD : YvsZ with T",(DetMaxY-DetMinY)/2.,DetMinY,DetMaxY,(DetMaxZ-DetMinZ)/2.,DetMinZ+1,DetMaxZ+1);
	//===== End : Histograms =====//



	//===== Start : Track Lines & Vertex Point =====//
	// Track lines
	TPolyLine3D *line[5];
	TF1 *line2dXY[5];
	TF1 *line2dYZ[5];
	TF1 *line2dZX[5];
	for (Int_t i=0;i<5;i++) {
		line[i] = new TPolyLine3D(2);
		line2dXY[i] = new TF1(Form("line2dXY_%d",i+1),"[0]+[1]*x",DetMinX,DetMaxX);
		line2dYZ[i] = new TF1(Form("line2dYZ_%d",i+1),"[0]+[1]*x",DetMinY,DetMaxZ);
		line2dZX[i] = new TF1(Form("line2dZX_%d",i+1),"[0]+[1]*x",DetMinX,DetMaxZ);

		line[i]->SetLineColor(pow(2,i));
		line[i]->SetLineWidth(2);
		line2dXY[i]->SetLineColor(pow(2,i));
		line2dYZ[i]->SetLineColor(pow(2,i));
		line2dZX[i]->SetLineColor(pow(2,i));
		line2dXY[i]->SetLineWidth(2);
		line2dYZ[i]->SetLineWidth(2);
		line2dZX[i]->SetLineWidth(2);
	}
	line2dXY[4]->SetLineColor(1);
	line2dXY[4]->SetLineStyle(9);
	line2dXY[4]->SetLineWidth(2);

	// Vertex point
	TGraphErrors *grXY = new TGraphErrors();
	TGraphErrors *grYZ = new TGraphErrors();
	TGraphErrors *grZX = new TGraphErrors();
	TGraph2DErrors *grXYZ = new TGraph2DErrors();
	grXYZ->Set(1);
	grXY->SetMarkerStyle(20);
	grYZ->SetMarkerStyle(20);
	grZX->SetMarkerStyle(20);
	grXYZ->SetMarkerStyle(20);
	grXY->SetMarkerSize(1.3);
	grYZ->SetMarkerSize(1.3);
	grZX->SetMarkerSize(1.3);
	grXYZ->SetMarkerSize(1.3);
	grXY->SetLineWidth(2);
	grYZ->SetLineWidth(2);
	grZX->SetLineWidth(2);
	grXYZ->SetLineWidth(2);


	// Reaching point
	TGraph *grReachMayaR = new TGraph();
	TGraph *grReachMayaL = new TGraph();
	TGraph *grReachDSSD = new TGraph();
	grReachMayaR->SetMarkerStyle(47);
	grReachMayaR->SetMarkerSize(1.3);
	grReachMayaL->SetMarkerStyle(47);
	grReachMayaL->SetMarkerSize(1.3);
	grReachDSSD->SetMarkerStyle(47);
	grReachDSSD->SetMarkerSize(1.3);
	//===== End : Track Lines & Vertex Point =====//


	//===== Start : Detector Area =====//
	// Area of Pad
	TBox *bPadXY = new TBox(PadMinX, PadMinY, PadMaxX+1, PadMaxY);
	TBox *bPadYZ = new TBox(PadMinY, PadMinZ, PadMaxY, PadMaxZ);
	TBox *bPadZX = new TBox(PadMinX, PadMinZ, PadMaxX+1, PadMaxZ);
	bPadXY->SetFillStyle(0);
	bPadYZ->SetFillStyle(0);
	bPadZX->SetFillStyle(0);
	bPadXY->SetLineColor(1);
	bPadYZ->SetLineColor(1);
	bPadZX->SetLineColor(1);
	bPadXY->SetLineWidth(2);
	bPadYZ->SetLineWidth(2);
	bPadZX->SetLineWidth(2);


	// Area of Maya
	const Int_t NumMayaPad = detector->NumberOfMayaX()*detector->NumberOfMayaZ();
	Double_t box_mayaRx1[NumMayaPad];
	Double_t box_mayaRz1[NumMayaPad];
	Double_t box_mayaRx2[NumMayaPad];
	Double_t box_mayaRz2[NumMayaPad];
	Double_t box_mayaLx1[NumMayaPad];
	Double_t box_mayaLz1[NumMayaPad];
	Double_t box_mayaLx2[NumMayaPad];
	Double_t box_mayaLz2[NumMayaPad];
	TBox *bMayaR[NumMayaPad];
	TBox *bMayaL[NumMayaPad];
	for (auto i=0;i<NumMayaPad;i++) {
		box_mayaRx1[i] = detector->CenterOfMayaRPadX(i) - detector->SizeOfMayaX(0.5);
		box_mayaRz1[i] = detector->CenterOfMayaRPadZ(i) - detector->SizeOfMayaZ(0.5);
		box_mayaRx2[i] = detector->CenterOfMayaRPadX(i) + detector->SizeOfMayaX(0.5);
		box_mayaRz2[i] = detector->CenterOfMayaRPadZ(i) + detector->SizeOfMayaZ(0.5);
		box_mayaLx1[i] = detector->CenterOfMayaLPadX(i) - detector->SizeOfMayaX(0.5);
		box_mayaLz1[i] = detector->CenterOfMayaLPadZ(i) - detector->SizeOfMayaZ(0.5);
		box_mayaLx2[i] = detector->CenterOfMayaLPadX(i) + detector->SizeOfMayaX(0.5);
		box_mayaLz2[i] = detector->CenterOfMayaLPadZ(i) + detector->SizeOfMayaZ(0.5);
		bMayaR[i] = new TBox(box_mayaRx1[i], box_mayaRz1[i], box_mayaRx2[i], box_mayaRz2[i]);
		bMayaL[i] = new TBox(box_mayaLx1[i], box_mayaLz1[i], box_mayaLx2[i], box_mayaLz2[i]);
		bMayaR[i]->SetFillStyle(0);
		bMayaR[i]->SetLineColor(i+1);
		bMayaR[i]->SetLineWidth(2);
		bMayaL[i]->SetFillStyle(0);
		bMayaL[i]->SetLineColor(i+1);
		bMayaL[i]->SetLineWidth(2);
	}

	// Area of DSSD
	const Int_t NumDSSDPad = detector->NumberOfDSSDY()*detector->NumberOfDSSDZ();
	Double_t box_dssdy1[NumDSSDPad];
	Double_t box_dssdz1[NumDSSDPad];
	Double_t box_dssdy2[NumDSSDPad];
	Double_t box_dssdz2[NumDSSDPad];
	TBox *bDSSD[NumDSSDPad];
	for (auto i=0;i<NumDSSDPad;i++) {
		box_dssdy1[i] = detector->CenterOfDSSDPadY(i) - detector->SizeOfDSSDY(0.5);
		box_dssdz1[i] = detector->CenterOfDSSDPadZ(i) - detector->SizeOfDSSDZ(0.5);
		box_dssdy2[i] = detector->CenterOfDSSDPadY(i) + detector->SizeOfDSSDY(0.5);
		box_dssdz2[i] = detector->CenterOfDSSDPadZ(i) + detector->SizeOfDSSDZ(0.5);
		bDSSD[i] = new TBox(box_dssdz1[i], box_dssdy1[i], box_dssdz2[i], box_dssdy2[i]);
		bDSSD[i]->SetFillStyle(0);
		bDSSD[i]->SetLineColor(pow(2,i));
		bDSSD[i]->SetLineWidth(2);
	}
	//===== End : Detector Area =====//



	//===== Start : Fill Data =====//
	// Constant variables
	TVector3 BeamDirec = detector->BeamDirection();
	TVector3 BeamPoint = detector->BeamPosition();

	// Get Event
	Bpad->GetEntry(eve);
	Bmaya->GetEntry(eve);
	Bdssd->GetEntry(eve);
	Btrack->GetEntry(eve);
	Bvertex->GetEntry(eve);


	// For Pads
	Int_t x,y,z;
	Double_t ene;
	for (Int_t i=0;i<pad->Size();i++) {
		x = pad->GetX(i);
		y = pad->GetY(i);
		z = pad->GetZ(i);
		ene = pad->GetEnergy(i);

		hPadXYE->Fill(x, y, ene);
		hPadYZE->Fill(y, z, ene);
		hPadZXE->Fill(x, z, ene);
		hPadXYZ->Fill(x, y, z);
	}
	hPadXYZ3D = new TGraph2D(hPadXYZ);


	// For Maya
	for (auto i=0;i<maya->Size();i++) {
		if (maya->GetY(i)>0) {
			hMayaLZXE->Fill(maya->GetX(i), maya->GetZ(i), maya->GetEnergy(i));
			hMayaLZXT->Fill(maya->GetX(i), maya->GetZ(i), maya->GetTime(i));

		} else if (maya->GetY(i)<0) {
			hMayaRZXE->Fill(maya->GetX(i), maya->GetZ(i), maya->GetEnergy(i));
			hMayaRZXT->Fill(maya->GetX(i), maya->GetZ(i), maya->GetTime(i));
		}
	}

	// For DSSD
	for (auto i=0;i<dssd->Size();i++) {
		if (dssd->GetX(i)>0) {
			hDSSDYZE->Fill(dssd->GetZ(i), dssd->GetY(i), dssd->GetEnergy(i));
			hDSSDYZT->Fill(dssd->GetZ(i), dssd->GetY(i), dssd->GetTime(i));
		}
	}

	// For Tracks
	Double_t px=0, py=0, pz=0;
	Double_t dx=0, dy=0, dz=0;
	Double_t minX=0, minY=0, minZ=0;
	Double_t maxX=0, maxY=0, maxZ=0;
	Double_t Max=0, Min=0;
	TVector3 abc[2];

	Double_t interceptXY = 0;
	Double_t interceptYZ = 0;
	Double_t interceptZX = 0;
	Double_t slopeXY = 0;
	Double_t slopeYZ = 0;
	Double_t slopeZX = 0;

	Double_t rx=0, ry=0, rz=0;
	Int_t numtrack = track->Size();
	Int_t index1=0, index2=0, index3=0;

	for (Int_t i=0;i<numtrack;i++) {
		px = track->GetPositionX(i); py = track->GetPositionY(i); pz = track->GetPositionZ(i);
		dx = track->GetDirectionX(i); dy = track->GetDirectionY(i); dz = track->GetDirectionZ(i);

		minX = -px/dx; minY = -py/dy; minZ = -pz/dz;
		maxX = (PadMaxX-px)/dx; maxY = (PadMaxY-py)/dy; maxZ = (PadMaxZ-pz)/dz;
		Min = minX; Max = maxX;
		if ((py+Min*dy)>PadMaxY) Min = maxY;
		if ((py+Min*dy)<0)    Min = minY;
		if ((py+Max*dy)>PadMaxY) Max = maxY;
		if ((py+Max*dy)<0)    Max = minY;

		if ((pz+Min*dz)>PadMaxZ) Min = maxZ;
		if ((pz+Min*dz)<0)    Min = minZ;
		if ((pz+Max*dz)>PadMaxZ) Max = maxZ;
		if ((pz+Max*dz)<0)    Max = minZ;

		abc[0].SetXYZ(px+Min*dx, py+Min*dy, pz+Min*dz);
		abc[1].SetXYZ(px+Max*dx, py+Max*dy, pz+Max*dz);

		line[i]->SetPoint(0, abc[0].x(), abc[0].y(), abc[0].z());
		line[i]->SetPoint(1, abc[1].x(), abc[1].y(), abc[1].z());

		interceptXY = py - dy*px/dx;
		interceptYZ = pz - dz*py/dy;
		interceptZX = pz - dz*px/dx;
		slopeXY = dy/dx;
		slopeYZ = dz/dy;
		slopeZX = dz/dx;

		line2dXY[i]->SetParameter(0, interceptXY);
		line2dXY[i]->SetParameter(1, slopeXY);
		line2dYZ[i]->SetParameter(0, interceptYZ);
		line2dYZ[i]->SetParameter(1, slopeYZ);
		line2dZX[i]->SetParameter(0, interceptZX);
		line2dZX[i]->SetParameter(1, slopeZX);

		rx = track->GetReachingPointX(i);
		ry = track->GetReachingPointY(i);
		rz = track->GetReachingPointZ(i);
		if (rx==DetMaxX) {
			grReachDSSD->SetPoint(index1, rz, ry);
			index1++;
		}
		if (ry==DetMaxY) {
			grReachMayaL->SetPoint(index2, rx, rz);
			index2++;
		}
		if (ry==DetMinY) {
			grReachMayaR->SetPoint(index3, rx, rz);
			index3++;
		}
	}
	line[4]->SetPoint(0, BeamPoint.x(), BeamPoint.y(), BeamPoint.z());
	line[4]->SetPoint(1, BeamPoint.x()+DetMaxX, BeamPoint.y(), BeamPoint.z());
	line2dXY[4]->SetParameter(0, BeamPoint.y());
	line2dXY[4]->SetParameter(1, BeamDirec.y());


	// For Vertex
	Double_t vx = vertex->GetVertexX();
	Double_t vy = vertex->GetVertexY();
	Double_t vz = vertex->GetVertexZ();
	Double_t vxerr = vertex->GetVertexErrX();
	Double_t vyerr = vertex->GetVertexErrY();
	Double_t vzerr = vertex->GetVertexErrZ();

	grXY->SetPoint(0, vx, vy);
	grYZ->SetPoint(0, vy, vz);
	grZX->SetPoint(0, vx, vz);
	grXYZ->SetPoint(1, vx, vy, vz);
	grXY->SetPointError(0, vxerr, vyerr);
	grYZ->SetPointError(0, vyerr, vzerr);
	grZX->SetPointError(0, vxerr, vzerr);
	grXYZ->SetPointError(1, vxerr, vyerr, vzerr);

	// For Bragg Curve
	Double_t distance = 0;
	Int_t trackNo = 0;
	TVector3 dis;
	for (Int_t i=0;i<pad->Size();i++) {
		trackNo = pad->GetTrackNumber(i);
		if (trackNo>0) {
			for (Int_t j=0;j<pad->GetEnergy(i);j++) {
				dis.SetXYZ(pad->GetX(i)+gRandom->Uniform(-1,1), pad->GetY(i)+gRandom->Uniform(-1,1), pad->GetZ(i)+gRandom->Uniform(-1,1));
				hBragg[trackNo-1]->Fill((vertex->GetVertex()-dis).Mag());
			}
		}
	}

	//===== End : Fill Data =====//



	//===== Start : Histograms Stat=====//
	// For Pad
	hPadXYE->SetTitle("YvsX with E;column [mm];row [mm];Energy []");
	hPadYZE->SetTitle("ZvsY with E;row [mm];height [mm];Energy []");
	hPadZXE->SetTitle("ZvsX with E;column [mm];height [mm];Energy []");
	hPadXYZ->SetTitle("XvsYvsZ;column [mm];row [mm];height [mm]");

	hPadXYZ3D->SetName("3D map");
	hPadXYZ3D->SetTitle(";column [mm];row [mm];height [mm]");
	hPadXYZ3D->SetMarkerStyle(20);
	hPadXYZ3D->SetMinimum(PadMinZ);
	hPadXYZ3D->SetMaximum(PadMaxZ);
	hPadXYZ3D->GetXaxis()->SetLimits(PadMinX,PadMaxX);
	hPadXYZ3D->GetYaxis()->SetLimits(PadMinY,PadMaxY);

	for (Int_t i=0;i<4;i++) {
		hBragg[i]->SetTitle(Form("Distance from vertex with E (%d-th track);Distance from vertex [mm];Charge [/mm];",i+1));
		hBragg[i]->SetLineColor(pow(2,i));
	}

	//For Maya
	hMayaRZXE->SetMarkerSize(3);
	hMayaRZXT->SetMarkerSize(3);
	hMayaLZXE->SetMarkerSize(3);
	hMayaLZXT->SetMarkerSize(3);
	hMayaRZXE->SetMarkerColor(0);
	hMayaRZXT->SetMarkerColor(1);
	hMayaLZXE->SetMarkerColor(0);
	hMayaLZXT->SetMarkerColor(1);
	hMayaRZXE->SetBarOffset(0.2);
	hMayaRZXT->SetBarOffset(-0.2);
	hMayaLZXE->SetBarOffset(0.2);
	hMayaLZXT->SetBarOffset(-0.2);

	// Histograms for DSSD
	hDSSDYZE->SetMarkerSize(3);
	hDSSDYZT->SetMarkerSize(3);
	hDSSDYZE->SetMarkerColor(1);
	hDSSDYZT->SetMarkerColor(2);
	hDSSDYZE->SetBarOffset(2.5);
	hDSSDYZT->SetBarOffset(-2.5);
	//===== End : Histograms Stat=====//


	//===== Start : Draw Histograms =====//
	cEnergy->cd();
	cEnergyPad->cd()->SetGrid();
	hPadXYE->Draw("col");
	bPadXY->Draw("same l");
	for (auto i=0;i<numtrack;i++) line2dXY[i]->Draw("same");
	line2dXY[4]->Draw("same");
	grXY->Draw("same p");

	cEnergy->cd();
	cEnergyMayaL->cd()->SetGrid();
	hMayaLZXE->Draw("col text");
	hMayaLZXT->Draw("same text");
	for (auto i=0;i<NumMayaPad;i++) {
		bMayaL[i]->Draw("same l");
	}
	if (index2>0) grReachMayaL->Draw("same p");

	cEnergy->cd();
	cEnergyMayaR->cd()->SetGrid();
	hMayaRZXE->Draw("col text");
	hMayaRZXT->Draw("same text");
	for (auto i=0;i<NumMayaPad;i++) {
		bMayaR[i]->Draw("same l");
	}
	if (index3>0) grReachMayaR->Draw("same p");

	cEnergy->cd();
	cEnergyDSSD->cd()->SetGrid();
	hDSSDYZE->Draw("box text");
	hDSSDYZT->Draw("same text");
	for (auto i=0;i<NumDSSDPad;i++) {
		bDSSD[i]->Draw("same l");
	}
	if (index1>0) grReachDSSD->Draw("same p");

	cPad3D->cd(1);
	hPadXYZ3D->Draw("pcol");
	for (Int_t i=0;i<numtrack;i++) {
		line[i]->Draw("same");
	}
	grXYZ->Draw("same err p");
	line[4]->Draw("same");

	cPad3D->cd(2);
	hPadYZE->Draw("colz");
	for (Int_t i=0;i<numtrack;i++) {
		line2dYZ[i]->Draw("same");
	}
	grYZ->Draw("same p");
	bPadYZ->Draw("same l");


	cPad3D->cd(3);
	hPadZXE->Draw("colz");
	for (Int_t i=0;i<numtrack;i++) {
		line2dZX[i]->Draw("same");
	}
	grZX->Draw("same p");
	bPadZX->Draw("same l");

	cPad3D->cd(4);
	hPadXYE->Draw("colz");
	for (Int_t i=0;i<numtrack;i++) {
		line2dXY[i]->Draw("same");
	}
	grXY->Draw("same p");
	bPadXY->Draw("same l");

	for (Int_t i=0;i<4;i++) {
		cBragg->cd(i+1)->SetGrid();
		hBragg[i]->Draw("HISTO");
	}
	//===== End : Draw Histograms =====//

	cout<<"//==================================//"<<endl;
	cout<<"//         ShowAnEvent.cc           //"<<endl;
	cout<<"//==================================//"<<endl;
	cout<<" "<<endl;
	cout<<"== Event Status "<<endl;
	cout<<"|- Input File             : "<<Ifilename<<endl;
	cout<<"|- TTree Name             : "<<Itreename<<endl;
  cout<<"|- Event Number           : "<<eve<<endl;
	cout<<"|- Original Event Number  : "<<pad->GetOrgEvent()<<endl;
	cout<<"|- Detector Type          : "<<Type->Data()<<endl;
	cout<<" "<<endl;
	cout<<"== Energy "<<endl;
	for (auto i=0;i<numtrack;i++) {
		cout<<"|- Track "<<i+1<<"    : "<<track->GetEnergy(i)<<endl;
	}
	cout<<"|- Right Maya : "<<maya->GetTotalEnergy("Right")<<endl;
	cout<<"|- Left Maya  : "<<maya->GetTotalEnergy("Left")<<endl;
	cout<<"|- DSSD       : "<<dssd->GetTotalEnergy()<<endl;
	cout<<" "<<endl;
	cout<<"== Vertex Point "<<endl;
	cout<<"|- Position (x,y,z)   : "<<"("<<vx<<", "<<vy<<", "<<vz<<")"<<endl;
	cout<<"|- Error (dx, dy, dz) : "<<"("<<vxerr<<", "<<vyerr<<", "<<vzerr<<")"<<endl;

	cout<<" "<<endl;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

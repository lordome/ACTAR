#include <iostream>

#include "TCanvas.h"
#include "TH3F.h"
#include "TH1.h"
#include "TSystem.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"

#include "digit.h"

#include "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/inc/cPhysicalHit.h"
#include "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/inc/cPhysicalEvent.h"
// #include "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/inc/cUserInput.h"
// #include "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/inc/cLookupTable.h"
// #include "/home/lorenzo/Desktop/ACTAR_git/ARDA-Ransac-Demonstrator/ARDA/inc/cRawEvent.h"

using namespace std;

#if defined(__MAKECINT__)
#pragma link C++ class ActarPadSignal;
#pragma link C++ class projectionOnPadPlane;
#pragma link C++ class padsGeometry;
#pragma link C++ class amplificationManager;
#pragma link C++ class driftManager;
#endif

void anaDig()
{

	gSystem->Load("libactar.sl");

	TFile *_file0 = TFile::Open("root_files/dig_files/20_O_2H_transfer.root");
	TTree *digiTree = (TTree *)_file0->Get("digiTree");
	digiTree->Print();

	TClonesArray *padSignalCA = new TClonesArray("ActarPadSignal", 50);
	digiTree->SetBranchAddress("padSignals", &padSignalCA);
	ActarPadSignal *padSignal = new ActarPadSignal;

	TH1D *h = new TH1D("h1", "h1", 500, 0, 128);

	Double_t driftVelocity = 1; // cm/mus (in D2, 1 V/cm*torr)
	const Int_t numberOfRows = 100;
	const Int_t numberOfColumns = 100;
	std::cout << "Number of Rows " << numberOfRows << " Number of Columns " << numberOfColumns << std::endl;

	// Matrix for the charge map
	Double_t **padCharge = new Double_t *[numberOfRows];
	Double_t **padTime = new Double_t *[numberOfRows];
	Double_t **padHeight = new Double_t *[numberOfRows];
	for (Int_t j = 0; j < numberOfRows; j++)
	{
		padCharge[j] = new Double_t[numberOfColumns];
		padTime[j] = new Double_t[numberOfColumns];
		padHeight[j] = new Double_t[numberOfColumns];
	}

	double_t threshold = 0;
	Double_t thresholdADC = threshold + 0.1;

	double sigma_time = 3;

	Double_t Qtot;
	TH3F *h3DTrack = new TH3F("h3DTrack", "h3DTrack", 128, 0, 256, 128, 0, 256, 85, 0, 85);

	// DEFINE OUTPUT FILE
	// Open output file and save the calibrations used
	TFile fout("outputPrecalibrator.root", "RECREATE");
	if (fout.IsZombie())
	{
		std::cout << "Output file creation failed" << std::endl;
		return;
	}
	fout.cd();

	// Creating output tree
	TTree outTree("physicalEventTree", "physicalEventTree");
	cPhysicalEvent *phEvt = new cPhysicalEvent;
	outTree.Branch("event", &phEvt);
	outTree.SetDirectory(&fout);
	outTree.AutoSave();

	for (unsigned int i = 0; i < digiTree->GetEntries(); ++i)
	{
		digiTree->GetEntry(i);
		int numberOfPads = padSignalCA->GetEntries();

		for (unsigned int k = 0; k < numberOfPads; ++k)
		{

			padSignal = (ActarPadSignal *)padSignalCA->At(k);

			Double_t thisCharge = padSignal->GetChargeDeposited();
			Double_t thisTime = (padSignal->GetInitTime() + padSignal->GetFinalTime()) / 2.;
			Double_t thisSigmaTime = padSignal->GetSigmaTime();
			Int_t PadRow = padSignal->GetPadRow();
			Int_t PadColumn = padSignal->GetPadColumn();

			cout << "Row - Column - Height - Energy:  " << PadRow << " - " << PadColumn << " - " << padTime[PadRow - 1][PadColumn - 1] * driftVelocity / 2 + 1 << " - " << padCharge[PadRow - 1][PadColumn - 1] << endl;

			if (thisCharge != 0)
			{
				padCharge[padSignal->GetPadRow() - 1][padSignal->GetPadColumn() - 1] += thisCharge;
				padTime[padSignal->GetPadRow() - 1][padSignal->GetPadColumn() - 1] += thisCharge * thisTime;
			}

			// // Loop on rows & columns
			// for (Int_t c = 0; c < numberOfColumns; c++)
			// { // LOOP on Columns
			// 	for (Int_t r = 0; r < numberOfRows; r++)
			// 	{ // LOOP on Rows
			if (padCharge[PadRow - 1][PadColumn - 1] > thresholdADC)
			{
				padTime[PadRow - 1][PadColumn - 1] = padTime[PadRow - 1][PadColumn - 1] / padCharge[PadRow - 1][PadColumn - 1];
				padTime[PadRow - 1][PadColumn - 1] = gRandom->Gaus(padTime[PadRow - 1][PadColumn - 1], sigma_time);
			}

			Double_t maxtime = 0;
			Double_t mintime = 0;

			if (padCharge[PadRow - 1][PadColumn - 1] > thresholdADC)
			{
				padCharge[PadRow - 1][PadColumn - 1] *= 1;
				Qtot += padCharge[PadRow - 1][PadColumn - 1];
				padHeight[PadRow - 1][PadColumn - 1] = padTime[PadRow - 1][PadColumn - 1] * driftVelocity;

				cout << "Row - Column - Height - Energy:  " << PadRow << " - " << PadColumn << " - " << padTime[PadRow - 1][PadColumn - 1] * driftVelocity / 2 + 1 << " - " << padCharge[PadRow - 1][PadColumn - 1] << endl;
				// visu_charge->SetBinContent(c + 1., r + 1, padCharge[PadRow - 1][PadColumn - 1]);
				// visu_time->SetBinContent(c + 1., r + 1., padTime[PadRow - 1][PadColumn - 1]);

				cPhysicalHit phHit;
				phHit[0] = PadColumn + 1.; // lt->getTable()[gid].getCol() * velocity[0];
				phHit[1] = PadRow + 1.;	   // lt->getTable()[gid].getRow() * velocity[1];
				phHit[2] = padTime[PadRow - 1][PadColumn - 1] * driftVelocity / 2 + 1;
				phHit.setEnergy(padCharge[PadRow - 1][PadColumn - 1]);
				phHit.setTrackable(true);

				phEvt->getHits().push_back(phHit);
			}
		}

		if (i % 2 != 0)
		{

			outTree.Fill();
			// auto *c1 = new TCanvas();
			// h3DTrack->Draw("LEGO");
			// c1->WaitPrimitive();
			// delete h3DTrack;
			// h3DTrack = new TH3F("h3DTrack", "h3DTrack", 128, 0, 256, 128, 0, 256, 85, 0, 85);
			// delete c1;

			for (Int_t j = 0; j < numberOfRows; j++)
			{
				delete padCharge[j];
				delete padTime[j];
				delete padHeight[j];
			}

			for (Int_t j = 0; j < numberOfRows; j++)
			{
				padCharge[j] = new Double_t[numberOfColumns];
				padTime[j] = new Double_t[numberOfColumns];
				padHeight[j] = new Double_t[numberOfColumns];
			}
		}
	}

	fout.cd();
	outTree.Write("", TObject::kOverwrite);

	cout << endl;
}

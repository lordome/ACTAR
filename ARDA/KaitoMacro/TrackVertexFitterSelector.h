#ifndef TrackVertexFitterSelector_h
#define TrackVertexFitterSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TProofOutputFile.h>
#include <array>
#include <TVector3.h>

#include "DetectorSet.hh"
#include "AnalysisSet.hh"
#include "DataSet.hh"
// Headers needed by this particular selector
//#include "macro/DataSet.hh"

#define MAXTRACK 5

class TrackVertexFitterSelector : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<PadData> pad = {fReader, "pad"};
   TTreeReaderValue<MayaData> maya = {fReader, "maya"};
   TTreeReaderValue<DSSDData> dssd = {fReader, "dssd"};


   typedef array<Int_t, 2> arrayI2;
   typedef array<Double_t, 3> arrayD3;
   typedef array<Double_t, 4> arrayD4;
   typedef array<TVector3, 2> arrayV2;

   TrackVertexFitterSelector(TTree * /*tree*/ =0);
   virtual ~TrackVertexFitterSelector();
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   void ReadFile(TString);

   // For RANSAC
   arrayI2 GetSample(Int_t size);											// Two points select randomly
   arrayV2 GetParamWithSample(arrayD4 sample1, arrayD4 sample2);			// Calculation of 3D line from tow vectors
   static Double_t GetError(arrayV2 model, arrayD4 p);							// Square of the distance between the point and the line
   static Double_t GetMean(arrayV2 model, PadData data);							// Mean value of square of distance
   void CalFirstEndPoint(PadData& pad, TrackData& track, Int_t itrack);	// Calculation of first and end point of track
   void ReturnDummyResult(PadData& input, PadData& pad, TrackData& track);	// Return the dummy result
   // RANSAC
   Bool_t RANSAC(PadData& data, PadData& outpad, TrackData& outtrack, Int_t loop, Int_t threshold, Double_t width, Int_t itrack, Int_t verbose);			// RANSAC algorithm
   Int_t mRANSAC(PadData& input, PadData& outpad, TrackData& outtrack, Int_t loop, Int_t threshold, Double_t width, Int_t verbose);	// multi RANSAC

   // For MINUIT
   static void FunctionForTrack(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t ifalg);	// MINUIT function for calculation of track
   static void FunctionForVertex(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t ifalg);	// MINUIT function for calculation of vertex
   Double_t GetReducedChi2(TVector3 fita, TVector3 fitb, PadData data);								// Calculation of chi2 value
   // MINUIT
   void MinuitForTrack(PadData& pad, TrackData& track);		// MINUIT algorithm for track
   void MinuitForVertex(TrackData& track, VertexData& vertex);	// MINUIT algorithm for vertex
   //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


   TFile *fFile;
   TProofOutputFile *fProofFile;
   TString fInName;
   TString outFileName;

   PadData *Opad[MAXTRACK+1];
   MayaData *Omaya[MAXTRACK+1];
   DSSDData *Odssd[MAXTRACK+1];
   TrackData *Otrack[MAXTRACK+1];
   VertexData *Overtex[MAXTRACK+1];
   TTree *Otree[MAXTRACK+1];

   PadData randata;
   PadData outpad;
   MayaData outmaya;
   DSSDData outdssd;
   TrackData outtrack;
   VertexData outvertex;

   Int_t index[MAXTRACK+1];
   Int_t itrack;
   Double_t fpoint;
   Double_t epoint;
   TVector3 cordirection;

   Double_t coeff[6];
   Double_t mincoeff;
   TVector3 reachpoint;

   Double_t dminx;
   Double_t dminy;
   Double_t dminz;
   Double_t dmaxx;
   Double_t dmaxy;
   Double_t dmaxz;

   Double_t MinRange;			// Minimum z-axis [mm]
   Double_t MaxRange;		// Maximum z-axis [mm]

   TVector3 zero;			// zero vector

   static PadData GlobalPad;				// Global PadData Class for minuit
   static TrackData GlobalTrack;			// Global TrackData Class for minuit

   DetectorSet *detector;
   AnalysisSet *analysis;

   ClassDef(TrackVertexFitterSelector,0);

};

#endif

#ifdef TrackVertexFitterSelector_cxx
void TrackVertexFitterSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t TrackVertexFitterSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef TrackVertexFitterSelector_cxx

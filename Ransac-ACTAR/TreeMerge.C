#include <TTree.h>
#include <TFile.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <MEventReduced.h>
#include <Parameters.h>

#define nCOBO 16
#define nVXI 1 
#define MERGE_DEPTH 5000

struct RunInfo{
  int Nevents;
  int FrameCounter[32];
};


int TreeMerge(int run_number)
{
	TString RunFile;
	TString TreeName;
  
  TCanvas* Cprob=new TCanvas("C","C",600,600);
  TH1F* hdup=new TH1F("hdup","hdup",32,0,32);
	
	RunInfo INFO;
  for(int i=0;i<NB_COBO;i++) INFO.FrameCounter[i]=0;
		
// 	RunFile.Form("/run/media/roger/ACTARTPC4/Tree_Run_%04d.root",run_number);
	RunFile.Form("./Tree_Run_%04d.root",run_number);
	TFile* f=new TFile(RunFile.Data(),"read");
  if (f->IsZombie())
  {
    cout << "File " << RunFile.Data() << " does not exist..." << endl;
    return(0);
  }
	
	TreeName.Form("ACTAR_TTree");
	
	TTree* Tree=(TTree*)f->Get(TreeName.Data());
	MEventReduced* EvtRed=0;
	
	
	Tree->SetBranchAddress("data",&EvtRed);
  
  cout << "Merging " << RunFile.Data() << " with " << Tree->GetEntries() << " entries" << endl;
	
	MEventReduced* ReducedEvent=new MEventReduced();
	TString TreeNameFile;
// 	TreeNameFile.Form("/run/media/roger/ACTARTPC4/Tree_Run_%04d_Merged.root",run_number);
	TreeNameFile.Form("./Tree_Run_%04d_Merged.root",run_number);
	TFile* TreeFile=new TFile(TreeNameFile.Data(),"recreate");		
	TTree* MergedTree=new TTree(TreeName.Data(),"2nd level Tree",0);
	MergedTree->Branch("data","MEventReduced",&ReducedEvent,16000,0);
	
	TH1F* chans=new TH1F("chans","chans",3000,0,3000);
	TH1F* chans2=new TH1F("chans2","chans",3000,0,3000);
	
	
	
	std::vector<MEventReduced> Fragments[MERGE_DEPTH];
	for(int i=0;i<MERGE_DEPTH;i++)
		Fragments[i].clear();
	
	
	for(int i=0;i<Tree->GetEntries();i++)
	{
    if(!(i%25000))
    {
      cout << i*100./Tree->GetEntries() << " % done       \r";
		  cout.flush();
    }
    
    Tree->GetEntry(i);
 		for(unsigned int fr=0;fr<EvtRed->CoboAsad.size();fr++)
    {
			chans->Fill(EvtRed->event);			 
    }
    
    //INFO.FrameCounter[EvtRed->CoboAsad[0].globalchannelid>>11]++;
    
		if(!Fragments[EvtRed->event%MERGE_DEPTH].empty())
		{
			if(Fragments[EvtRed->event%MERGE_DEPTH][0].event != EvtRed->event)
			{					
				ReducedEvent->event=Fragments[EvtRed->event%MERGE_DEPTH][0].event;
				ReducedEvent->timestamp=Fragments[EvtRed->event%MERGE_DEPTH][0].timestamp;
				for(unsigned int fr=0;fr<Fragments[EvtRed->event%MERGE_DEPTH].size();fr++)
					for(unsigned int subfr=0;subfr<Fragments[EvtRed->event%MERGE_DEPTH][fr].CoboAsad.size();subfr++)
						ReducedEvent->CoboAsad.push_back(Fragments[EvtRed->event%MERGE_DEPTH][fr].CoboAsad[subfr]);
					
 				//if(Fragments[EvtRed->event%MERGE_DEPTH][0].event==22) cout << endl << Fragments[EvtRed->event%MERGE_DEPTH].size() << endl;
				MergedTree->Fill() ;
					
				if(Fragments[EvtRed->event%MERGE_DEPTH].size()<4*nCOBO+nVXI)
					cout << "Incomplete event found: merging event " << Fragments[EvtRed->event%MERGE_DEPTH][0].event << " with " << Fragments[EvtRed->event%MERGE_DEPTH].size() << " fragments at entry #" << i << endl;
				else if(Fragments[EvtRed->event%MERGE_DEPTH].size()>4*nCOBO+nVXI)
        {
          int cpt=0;
          for(int frg=0;frg<Fragments[EvtRed->event%MERGE_DEPTH].size();frg++)
            if((int)(Fragments[EvtRed->event%MERGE_DEPTH][frg].CoboAsad.size())) cpt++;
          if(cpt>4*nCOBO+nVXI && cpt!=4*nCOBO+nVXI+2)
          {
            //cout << "Duplicated fragments	in event: " << Fragments[EvtRed->event%MERGE_DEPTH][0].event << " with " << Fragments[EvtRed->event%MERGE_DEPTH].size() << " fragments at entry #" << i << endl;
				    //cout << "CoBo list: ";
            hdup->Reset();
            for(int frg=0;frg<Fragments[EvtRed->event%MERGE_DEPTH].size();frg++)
            {
             //if(Fragments[EvtRed->event%MERGE_DEPTH][frg].CoboAsad.size()) 
             //cout << (int)(Fragments[EvtRed->event%MERGE_DEPTH][frg].CoboAsad.size()) << "(" << (int)(Fragments[EvtRed->event%MERGE_DEPTH][frg].CoboAsad[0].globalchannelid>>11) << ") ";//.globalchannelid/(4*4*68)) << " ";
              hdup->Fill((int)(Fragments[EvtRed->event%MERGE_DEPTH][frg].CoboAsad[0].globalchannelid>>11));
              Cprob->cd();
              Cprob->SetTitle(Form("event %d",Fragments[EvtRed->event%MERGE_DEPTH][0].event));
              hdup->Draw();
              Cprob->Update();
              
            }
            //cout << endl;
          }
        }
        Fragments[EvtRed->event%MERGE_DEPTH].clear();
				
				ReducedEvent->CoboAsad.clear();
				Fragments[EvtRed->event%MERGE_DEPTH].push_back(*EvtRed);
			}
			else Fragments[EvtRed->event%MERGE_DEPTH].push_back(*EvtRed);
		}
		else Fragments[EvtRed->event%MERGE_DEPTH].push_back(*EvtRed);
			
	}
  cout << "Initial TTree scanned successfully" << endl;
	
	for(int i=0;i<MERGE_DEPTH;i++)
	{	
    		if(!(i%(MERGE_DEPTH/10)))
    		{
      		cout << "flushing... " << i*100./MERGE_DEPTH << " % done       \r";
		 	cout.flush();
		}
		if(Fragments[i].size()>=4*nCOBO)
		{
			ReducedEvent->event=Fragments[i][0].event;
			ReducedEvent->timestamp=Fragments[i][0].timestamp;
			for(unsigned int fr=0;fr<Fragments[i].size();fr++)
				for(unsigned int subfr=0;subfr<Fragments[i][fr].CoboAsad.size();subfr++)
					ReducedEvent->CoboAsad.push_back(Fragments[i][fr].CoboAsad[subfr]);
			
			Fragments[i].clear();
			MergedTree->Fill();
		
			ReducedEvent->CoboAsad.clear();
		}
		else if(!Fragments[i].empty())
		{
			cout << "Skipping event " << Fragments[i][0].event << ": contains " << Fragments[i].size() << " fragments instead of " << nCOBO*4+nVXI << endl;
			Fragments[i].clear();
		}
	}
	
	for(int i=0;i<MergedTree->GetEntries();i++)
	{
		MergedTree->GetEntry(i);
		for(unsigned int fr=0;fr<ReducedEvent->CoboAsad.size();fr++)
			chans2->Fill(ReducedEvent->event);
	}

  for(int i=0;i<NB_COBO+nVXI; i++)
    cout << i << "\t";
  cout << endl;
  for(int i=0;i<NB_COBO+nVXI; i++)
    cout << INFO.FrameCounter[i] << "\t";
  cout << endl;

	MergedTree->Write();
	TreeFile->Close();
	f->Close();
	
	
// 	delete f;
// 	delete ReducedEvent;
// 	delete TreeFile;		
// 	delete MergedTree;
// 	delete chans;
// 	delete chans2;
}

#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"
#include "TF1.h"
#include "TDirectory.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TStyle.h"
#include "TChain.h"
#include <iostream>
#include <fstream>
#include "TSystem.h"
#include "TROOT.h"

void create_MyHist(){

  TChain *tree = new TChain("ak4pfchs/t");
  Char_t filechain[1024];

  Int_t NofFiles = 100; //number of JRA*.root files for the MC sample

  for(Int_t kk=1; kk<=NofFiles; kk++) 
  {
      sprintf(filechain,"/Location_of_ntuples/JRA_%d.root",kk);
      tree->Add(filechain);
  }

  std::vector<float> *tnpus=0;	
  tree->SetBranchAddress("tnpus",&tnpus);

  TH1F *pileup = new TH1F("pileup","",100,0.,100.);

  cout << "Entries = " << tree->GetEntries() << endl; 

  for(int i=0; i<tree->GetEntries(); i++)
  {
	tree->GetEntry(i);
	if(i%1000000==0) cout << "Processed " << i << " / " << tree->GetEntries() << " events" << endl;
	pileup->Fill(tnpus->at(12));
  }

  TFile *fout = new TFile("MyMCPUHisto.root ","RECREATE");
  fout->cd();
  pileup->Write();

}

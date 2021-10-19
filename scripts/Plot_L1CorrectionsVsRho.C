#include "TF1.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TH3I.h"
#include "TTree.h"
#include "TFile.h"
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
#include "TMath.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TLatex.h"

void Plot_L1CorrectionsVsRho(){

// Script to plot the L1 correction factors as a function of rho for a set of corrections (e.g. for a specific year and jet collection)
// First run jet_draw_corrections_x.cc for the L1 corrections (see first lines of the code and the README)
// This will produce a root file, this will be the input for this code
// The eta bins in this code should match what you ran in the jet_draw_corrections_x.cc code. In this example code the input root
// files have the histograms for:
// Corr. vs rho for |eta|: 0-0.087, 0.261-0.348, 0.609-0.696, 0.957-1.044, 1.218-1.305, 1.479-1.566, 1.740-1.83, 2.043-2.172, 2.172-2.322, 
//			  2.322-2.5, 2.5-2.65, 2.65-2.853, 2.853-2.964, 2.964-3.139, 3.489-3.664
// In the jet_draw_corrections_x.cc you also fix the pt to a constant value, for instance 100 GeV

TPaveText *cms1 = new TPaveText(0.17,0.95,0.47,0.98,"NDC");
cms1->AddText("CMS Simulation #bf{#it{Work in Progress}}");
cms1->SetFillColor(0);
cms1->SetBorderSize(0);
cms1->SetTextSize(0.03);

TPaveText *cms2 = new TPaveText(0.91,0.95,0.97,0.98,"NDC");
cms2->AddText("#bf{13 TeV}");
cms2->SetFillColor(0);
cms2->SetBorderSize(0);
cms2->SetTextSize(0.03);

char cname_vsEta[1024];
char cname_vsPt[1024];

char Etabins[15][100]={"|#eta| < 0.087", "0.261 < |#eta| < 0.348", "0.609 < |#eta| < 0.696", "0.957 < |#eta| < 1.044", "1.218 < |#eta| < 1.305", "1.479 < |#eta| < 1.566", "1.740 < |#eta| < 1.83", "2.043 < |#eta| < 2.172", "2.172 < |#eta| < 2.322", "2.322 < |#eta| < 2.500","2.500 < |#eta| < 2.650", "2.650 < |#eta| < 2.853", "2.853 < |#eta| < 2.964", "2.964 < |#eta| < 3.139", "3.489 < |#eta| < 3.664"};

TFile *f1 = new TFile("Corrections_ak4pfchs_positive_eta.root");
TFile *f2 = new TFile("Corrections_ak4pfchs_negative_eta.root");

//Corrections Vs Rho

TCanvas *canvas_vsRho = new TCanvas("canvas_vsRho","canvas_vsRho",800,800);
canvas_vsRho->SetTickx(1);
canvas_vsRho->SetTicky(1);
canvas_vsRho->SetRightMargin(0.02);
canvas_vsRho->SetLeftMargin(0.12);
canvas_vsRho->SetTopMargin(0.05);
canvas_vsRho->SetBottomMargin(0.1);

for(int c=0; c<=14; c++)
{
	TString hstr; 
	hstr.Form("RhoSF_%d",c);
	TH1F *cc1_pos = (TH1F*)f1->Get(hstr);
	TH1F *cc1_neg = (TH1F*)f2->Get(hstr);
	cc1_pos->SetStats(0);
	cc1_neg->SetStats(0);
	cc1_pos->SetTitle("");
	cc1_neg->SetTitle("");
	cc1_pos->GetXaxis()->SetTitle("#rho [GeV]");
      	cc1_pos->GetYaxis()->SetTitle("L1 Corr. Factor");
      	cc1_pos->GetYaxis()->SetTitleOffset(1.);
      	cc1_pos->GetYaxis()->SetTitleSize(0.058);
      	cc1_pos->GetXaxis()->SetTitleSize(0.05);
      	cc1_pos->GetXaxis()->SetTitleOffset(0.80);
	cc1_pos->GetXaxis()->SetRangeUser(0.,70.);
	cc1_neg->GetXaxis()->SetRangeUser(0.,70.);
	cc1_pos->SetLineColor(kBlack);
	cc1_neg->SetLineColor(14);
	cc1_pos->SetLineWidth(2);
	cc1_neg->SetLineWidth(2);
	cc1_neg->SetLineStyle(9);	

	TPaveText * pave = new TPaveText(0.55,0.85,0.85,0.9,"NDC");
      	pave->AddText(Etabins[c]); 
	pave->AddText("p_{T}^{reco} = 100 GeV"); //or whatever value you used in the jet_draw_corrections_x.cc code     
      	pave->SetFillColor(0);
      	pave->SetShadowColor(0);
      	pave->SetBorderSize(0);
      	pave->SetTextSize(0.04);

	canvas_vsRho->cd();
      	cc1_pos->Draw("hist");
      	cc1_neg->Draw("hist same");
      	pave->Draw();

	TLegend *l = new TLegend(0.5,0.65,0.8,0.85,"");
	l->SetBorderSize(0);
	l->SetTextSize(0.025);
	l->AddEntry(cc1_pos,"set of corrections (MC sample) (#eta>0)","L");
	l->AddEntry(cc1_neg,"set of corrections (MC sample) (#eta<0)","L");
	l->AddEntry((TObject*)0,"AK4 CHS jets","");
	l->Draw();

	cms1->Draw();
	cms2->Draw();

	sprintf(cname_vsPt, "L1CorrectionVsRho_%d.png", c);
	canvas_vsRho->SaveAs(cname_vsRho);	
}







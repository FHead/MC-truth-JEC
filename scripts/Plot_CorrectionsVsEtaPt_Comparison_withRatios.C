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

void Plot_CorrectionsVsEtaPt_Comparison_withRatios(){

// Plotting script to compare the correction factors as a function of pT and eta for 2 different set of corrections, set1 and set2 
// (e.g. for 2 different years or 2 different jet collections)
// First run jet_draw_corrections_x.cc (see README)
// This will produce a root file, this will be the input for this code
// The eta and pT bins in this code should match what you ran in the jet_draw_corrections_x.cc code. In this example code the input root
// files have the histograms for:
// Corr. vs eta for 15,30,50,100,200,500,750,1000,2000 GeV
// Corr. vs pT for |eta|: 0-0.087, 0.261-0.348, 0.609-0.696, 0.957-1.044, 1.218-1.305, 1.479-1.566, 1.740-1.83, 2.043-2.172, 2.172-2.322, 
//			  2.322-2.5, 2.5-2.65, 2.65-2.853, 2.853-2.964, 2.964-3.139, 3.489-3.664

TPaveText *cms1 = new TPaveText(0.15,0.955,0.45,0.985,"NDC");
cms1->AddText("CMS Simulation #bf{#it{Work in Progress}}");
cms1->SetFillColor(0);
cms1->SetBorderSize(0);
cms1->SetTextSize(0.04);

TPaveText *cms2 = new TPaveText(0.915,0.955,0.975,0.985,"NDC");
cms2->AddText("#bf{13 TeV}");
cms2->SetFillColor(0);
cms2->SetBorderSize(0);
cms2->SetTextSize(0.04);

char cname_vsEta[1024];
char cname_vsPt[1024];

char Etabins[15][100]={"|#eta| < 0.087", "0.261 < |#eta| < 0.348", "0.609 < |#eta| < 0.696", "0.957 < |#eta| < 1.044", "1.218 < |#eta| < 1.305", "1.479 < |#eta| < 1.566", "1.740 < |#eta| < 1.83", "2.043 < |#eta| < 2.172", "2.172 < |#eta| < 2.322", "2.322 < |#eta| < 2.500","2.500 < |#eta| < 2.650", "2.650 < |#eta| < 2.853", "2.853 < |#eta| < 2.964", "2.964 < |#eta| < 3.139", "3.489 < |#eta| < 3.664"};

char Ptbins[9][100]={"p_{T}^{reco} = 15 GeV", "p_{T}^{reco} = 30 GeV", "p_{T}^{reco} = 50 GeV", "p_{T}^{reco} = 100 GeV", "p_{T}^{reco} = 200 GeV", "p_{T}^{reco} = 500 GeV", "p_{T}^{reco} = 750 GeV", "p_{T}^{reco} = 1000 GeV", "p_{T}^{reco} = 2000 GeV"};


TFile *f1 = new TFile("Corrections_ak4pfchs_set1_positive_eta.root");
TFile *f2 = new TFile("Corrections_ak4pfchs_set1_negative_eta.root");

TFile *f3 = new TFile("Corrections_ak4pfchs_set2_positive_eta.root");
TFile *f4 = new TFile("Corrections_ak4pfchs_set2_negative_eta.root");


//Corrections Vs Eta

TCanvas *canvas_vsEta = new TCanvas("canvas_vsEta","canvas_vsEta",800,1000);
canvas_vsEta->cd();
canvas_vsEta->Draw();
TPad *main_panel_vsEta = new TPad("main_panel_vsEta", "main_panel_vsEta", 0, 0.25, 1, 0.95);
main_panel_vsEta->SetTickx(1);
main_panel_vsEta->SetTicky(1);
main_panel_vsEta->SetRightMargin(0.02);
main_panel_vsEta->SetLeftMargin(0.12);
main_panel_vsEta->SetTopMargin(0.05);
main_panel_vsEta->SetBottomMargin(0.);
main_panel_vsEta->Draw();
TPad *ratio_panel_vsEta = new TPad("ratio_panel_vsEta", "ratio_panel_vsEta", 0, 0., 1, 0.25);
ratio_panel_vsEta->SetTickx(1);
ratio_panel_vsEta->SetTicky(1);
ratio_panel_vsEta->SetRightMargin(0.02);
ratio_panel_vsEta->SetLeftMargin(0.12);
ratio_panel_vsEta->SetTopMargin(0.);
ratio_panel_vsEta->SetBottomMargin(0.2);
ratio_panel_vsEta->Draw();

for(int c=0; c<=8; c++)
{
	TString hstr; 
	hstr.Form("EtaSF_%d",c);
	TH1F *cc1 = (TH1F*)f1->Get(hstr);
	TH1F *cc2 = (TH1F*)f3->Get(hstr);
	cc1->SetStats(0);
	cc2->SetStats(0);
	cc1->SetTitle("");
	cc2->SetTitle("");
	cc1->GetXaxis()->SetTitle("#eta");
      	cc1->GetYaxis()->SetTitle("Corr. Factor");
      	cc1->GetYaxis()->SetTitleOffset(1.);
      	cc1->GetYaxis()->SetTitleSize(0.058);
      	cc1->GetXaxis()->SetTitleSize(0.058);
      	cc1->GetXaxis()->SetTitleOffset(0.80);
      	cc1->GetYaxis()->SetRangeUser(0.9,2.1);
	cc1->SetLineColor(kBlack);
	cc2->SetLineColor(kRed);
	cc1->SetLineWidth(2);
	cc2->SetLineWidth(2);

	TPaveText * pave = new TPaveText(0.4,0.85,0.7,0.9,"NDC");
      	pave->AddText(Ptbins[c]);      
      	pave->SetFillColor(0);
      	pave->SetShadowColor(0);
      	pave->SetBorderSize(0);
      	pave->SetTextSize(0.04);

	main_panel_vsEta->cd();
      	cc1->Draw("hist");
	cc2->Draw("hist same");
      	pave->Draw();

	TLegend *l = new TLegend(0.3,0.65,0.6,0.85,"");
	l->SetBorderSize(0);
	l->SetTextSize(0.04);
	l->AddEntry(cc1,"set 1","L");
	l->AddEntry(cc2,"set 2","L");
	l->AddEntry((TObject*)0,"AK4 CHS jets","");
	l->Draw();

	cms1->Draw();
	cms2->Draw();

	ratio_panel_vsEta->cd();
	ratio_panel_vsEta->SetGridy(1);
	TH1F *h_ratio = (TH1F*)cc1->Clone();
	h_ratio->Divide(cc2);

	h_ratio->GetYaxis()->SetRangeUser(0.971, 1.029);
	h_ratio->GetYaxis()->SetTitle("set1 / set2");
	h_ratio->GetXaxis()->SetTitle("#eta");
      	h_ratio->GetYaxis()->SetTitleOffset(0.6);
      	h_ratio->GetYaxis()->SetTitleSize(0.1);
      	h_ratio->GetXaxis()->SetTitleSize(0.12);
	h_ratio->GetYaxis()->SetLabelSize(0.1);
      	h_ratio->GetXaxis()->SetLabelSize(0.1);
      	h_ratio->GetXaxis()->SetTitleOffset(0.7);
	h_ratio->GetXaxis()->SetTickLength(0.08);
	h_ratio->GetYaxis()->SetNdivisions(210);
	h_ratio->SetMarkerStyle(8);
	h_ratio->SetMarkerColor(kBlack);
	h_ratio->Draw("P");

	sprintf(cname_vsEta, "CorrectionVsEta_set1_vs_set2_%d.png", c);
	canvas_vsEta->SaveAs(cname_vsEta);
	
}



//Corrections Vs Pt

TCanvas *canvas_vsPt = new TCanvas("canvas_vsPt","canvas_vsPt",800,1000);
canvas_vsPt->cd()->SetLogx(1);
canvas_vsPt->Draw();
TPad *main_panel_vsPt = new TPad("main_panel_vsPt", "main_panel_vsPt", 0, 0.25, 1, 0.95);
main_panel_vsPt->SetTickx(1);
main_panel_vsPt->SetTicky(1);
main_panel_vsPt->SetRightMargin(0.02);
main_panel_vsPt->SetLeftMargin(0.12);
main_panel_vsPt->SetTopMargin(0.05);
main_panel_vsPt->SetBottomMargin(0.);
main_panel_vsPt->Draw();
TPad *ratio_panel_vsPt = new TPad("ratio_panel_vsPt", "ratio_panel_vsPt", 0, 0., 1, 0.25);
ratio_panel_vsPt->SetTickx(1);
ratio_panel_vsPt->SetTicky(1);
ratio_panel_vsPt->SetRightMargin(0.02);
ratio_panel_vsPt->SetLeftMargin(0.12);
ratio_panel_vsPt->SetTopMargin(0.);
ratio_panel_vsPt->SetBottomMargin(0.25);
ratio_panel_vsPt->Draw();

for(int c=0; c<=14; c++)
{
	TString hstr; 
	hstr.Form("PtSF_%d",c);
	TH1F *cc1_pos = (TH1F*)f1->Get(hstr);
	TH1F *cc2_pos = (TH1F*)f3->Get(hstr);
	TH1F *cc1_neg = (TH1F*)f2->Get(hstr);
	TH1F *cc2_neg = (TH1F*)f4->Get(hstr);
	cc1_pos->SetStats(0);
	cc2_pos->SetStats(0);
	cc1_neg->SetStats(0);
	cc2_neg->SetStats(0);
	cc1_pos->SetTitle("");
	cc2_pos->SetTitle("");
	cc1_neg->SetTitle("");
	cc2_neg->SetTitle("");
	cc1_pos->GetXaxis()->SetTitle("p_{T}^{reco} [GeV]");
      	cc1_pos->GetYaxis()->SetTitle("Corr. Factor");
      	cc1_pos->GetYaxis()->SetTitleOffset(1.);
      	cc1_pos->GetYaxis()->SetTitleSize(0.058);
      	cc1_pos->GetXaxis()->SetTitleSize(0.05);
      	cc1_pos->GetXaxis()->SetTitleOffset(0.80);
	cc1_pos->GetXaxis()->SetRangeUser(15.,4000.);
	cc1_pos->SetLineColor(kBlack);
	cc2_pos->SetLineColor(kRed);
	cc1_neg->SetLineColor(14);
	cc2_neg->SetLineColor(kRed+2);
	cc1_pos->SetLineWidth(2);
	cc2_pos->SetLineWidth(2);
	cc1_neg->SetLineWidth(2);
	cc2_neg->SetLineWidth(2);
	cc1_neg->SetLineStyle(9);
	cc2_neg->SetLineStyle(9);	

	TPaveText * pave = new TPaveText(0.55,0.85,0.85,0.9,"NDC");
      	pave->AddText(Etabins[c]);      
      	pave->SetFillColor(0);
      	pave->SetShadowColor(0);
      	pave->SetBorderSize(0);
      	pave->SetTextSize(0.04);

	main_panel_vsPt->cd()->SetLogx(1);
      	cc1_pos->Draw("hist");
	cc2_pos->Draw("hist same");
      	cc1_neg->Draw("hist same");
	cc2_neg->Draw("hist same");
      	pave->Draw();

	TLegend *l = new TLegend(0.4,0.5,0.8,0.85,"");
	l->SetBorderSize(0);
	l->SetTextSize(0.04);
	l->AddEntry(cc1_pos,"set1 (#eta>0)","L");
	l->AddEntry(cc2_pos,"set2 (#eta>0)","L");
	l->AddEntry((TObject*)0,"","");
	l->AddEntry(cc1_neg,"set1 (#eta<0)","L");
	l->AddEntry(cc2_neg,"set2 (#eta<0)","L");

	l->AddEntry((TObject*)0,"AK4 CHS jets","");
	l->Draw();

	cms1->Draw();
	cms2->Draw();

	ratio_panel_vsPt->cd()->SetLogx(1);
	ratio_panel_vsPt->SetGridy(1);
	TH1F *h_ratio_pos = (TH1F*)cc1_pos->Clone();
	h_ratio_pos->Divide(cc2_pos);
	TH1F *h_ratio_neg = (TH1F*)cc1_neg->Clone();
	h_ratio_neg->Divide(cc2_neg);

	h_ratio_pos->GetYaxis()->SetRangeUser(0.971, 1.029);
	h_ratio_pos->GetYaxis()->SetTitle("set1 / set2");
	h_ratio_pos->GetXaxis()->SetTitle("p_{T}^{reco} [GeV]");
      	h_ratio_pos->GetYaxis()->SetTitleOffset(0.6);
      	h_ratio_pos->GetYaxis()->SetTitleSize(0.1);
      	h_ratio_pos->GetXaxis()->SetTitleSize(0.12);
	h_ratio_pos->GetYaxis()->SetLabelSize(0.1);
      	h_ratio_pos->GetXaxis()->SetLabelSize(0.1);
      	h_ratio_pos->GetXaxis()->SetTitleOffset(0.7);
	h_ratio_pos->GetXaxis()->SetTickLength(0.08);
	h_ratio_pos->GetYaxis()->SetNdivisions(210);
	h_ratio_pos->SetMarkerStyle(8);
	h_ratio_pos->SetMarkerColor(kBlack);
	h_ratio_pos->GetXaxis()->SetRangeUser(15.,4000.);
	h_ratio_pos->Draw("P");
	h_ratio_neg->SetMarkerStyle(29);
	h_ratio_neg->SetMarkerColor(14);
	h_ratio_neg->Draw("P same");

	TLegend *lratio = new TLegend(0.8,0.35,0.9,0.55,"");
	//lratio->SetBorderSize(0);
	lratio->SetTextSize(0.08);
	lratio->AddEntry(h_ratio_pos,"#eta>0","P");
	lratio->AddEntry(h_ratio_neg,"#eta<0","P");
	lratio->Draw();

	sprintf(cname_vsPt, "CorrectionVsPt_set1_vs_set2_%d.png", c);
	canvas_vsPt->SaveAs(cname_vsPt);	
}

}

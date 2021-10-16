#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TStyle.h>

TF1* FindBestGaussianCoreFit(TH1D* histo)
{

	double mean = histo->GetMean();
	double rms = histo->GetRMS();
	bool quiet_mode = true; //minimum prints on terminal
	
	int HalfMaxBinLow = histo->FindFirstBinAbove(histo->GetMaximum()/2);
	int HalfMaxBinHigh = histo->FindLastBinAbove(histo->GetMaximum()/2);
	double WidthAtHalfMaximum = 0.5*(histo->GetBinCenter(HalfMaxBinHigh) - histo->GetBinCenter(HalfMaxBinLow));
    	double Xmax  = histo->GetXaxis()->GetBinCenter(histo->GetMaximumBin());

	TF1 *gausTF1;

	double Pvalue = 0;
	double ChiSquare;
	double ndf;

	double rms_step_plus;	
	double RangeLow = 0.;
	double RangeUp = 5.;
	double meanForRange;
	double spreadForRange;

	double PvalueBest = 0;
	double RangeLowBest = 0;
	double RangeUpBest = 0;
	double ChiSquareBest;
	double ndfBest;
	double StepMinusBest;
	double StepPlusBest;

	if(Xmax < mean) meanForRange = Xmax;
	else meanForRange = mean; //because some entries with LARGE errors sometimes falsely become the maximum

	if(WidthAtHalfMaximum < rms && WidthAtHalfMaximum>0) spreadForRange = WidthAtHalfMaximum;
	else spreadForRange = rms; //because WHF does not take into account weights and sometimes it turns LARGE

	double rms_step_minus = 2.2;
	while (rms_step_minus>0.5)
	{ 
		RangeLow = meanForRange - rms_step_minus*spreadForRange;
//		rms_step_plus = 2.2;
		rms_step_plus = rms_step_minus;  

		while ( rms_step_plus>0.5  )
		{	
			RangeUp = meanForRange + rms_step_plus*spreadForRange;
			if(quiet_mode)	histo->Fit("gaus","0Q","0",RangeLow, RangeUp);
			else 		histo->Fit("gaus","0","0",RangeLow, RangeUp);
			gausTF1 = (TF1*)	histo->GetListOfFunctions()->FindObject("gaus");
			ChiSquare = gausTF1->GetChisquare();
			ndf       = gausTF1->GetNDF();
			Pvalue = TMath::Prob(ChiSquare, ndf);			
	
			if (Pvalue > PvalueBest)
			{
				PvalueBest = Pvalue;
				RangeLowBest = RangeLow;
				RangeUpBest = RangeUp;
				ndfBest = ndf;
				ChiSquareBest = ChiSquare;
				StepMinusBest = rms_step_minus; 
				StepPlusBest = rms_step_plus; 
				meanForRange = gausTF1->GetParameter(1);
			}

			if(!quiet_mode)
			{
				cout << "\n\nFitting range used: [Mean - " << rms_step_minus  << " sigma,  Mean + " << rms_step_plus << " sigma ] "<< endl;
				cout << "ChiSquare = " << ChiSquare << " NDF = " << ndf << " Prob =  " << Pvalue << "  Best Prob so far = " << PvalueBest << endl;
			}
			rms_step_plus = rms_step_plus - 0.1;
		}
		rms_step_minus = rms_step_minus - 0.1;
	}

	if (quiet_mode) histo->Fit("gaus","0Q","0",RangeLowBest, RangeUpBest);
	else 
	{
		histo->Fit("gaus","0","0",RangeLowBest, RangeUpBest);
		cout << "\n\n\nMean =  " << mean << "  Xmax = " << Xmax << "  RMS = " << rms << "  WidthAtHalfMax = " << WidthAtHalfMaximum <<  endl;
		cout << "Fit found!" << endl;
		cout << "Final fitting range used: [Mean(Xmax) - " << StepMinusBest << " rms(WHF), Mean(Xmax) + " << StepPlusBest << " rms(WHF) ] "<< endl;
		cout << "ChiSquare = " << ChiSquareBest << " NDF = " << ndfBest << " Prob =  " << PvalueBest << "\n\n" << endl;
	}	
	gausTF1 = (TF1*)	histo->GetListOfFunctions()->FindObject("gaus");

	return gausTF1;
}

void Plot_GausFit_Offset(){

TCanvas *c = new TCanvas();
gStyle->SetPadTickX(1);
gStyle->SetPadTickY(1);

TFile *f = new TFile("Step2/output_ak4pfchs.root");
f->cd();

TH2D *h = (TH2D*)f->Get("p_offresVsrefpt_bb_tnpu0_9");	//bb = |η|<1.3, ei = 1.3 < |η| < 2.5 , eo = 2.5 < |η| < 3.0 , ff = 3.0 < |η| < 5.0

for(unsigned int i=1; i<=h->GetNbinsX(); i++)
{
	cout <<"pT bin = "<<i<<" ---> "<<" Low Edge = "<<h->GetXaxis()->GetBinLowEdge(i)<<" Upper Edge = "<<h->GetXaxis()->GetBinUpEdge(i)<<endl;
}

c->cd();

TH1D *histo = h->ProjectionY("histo",15,15); //bin n. 15 corresponds to 15-17 GeV (see the above printouts)
histo->Rebin(2);
histo->SetTitle("V|#eta|< 1.3 , 0 #leq #mu < 10");
histo->GetXaxis()->SetTitle("Offset (GeV)");
histo->GetYaxis()->SetTitle("Entries (weighted)");
histo->GetYaxis()->SetTitleOffset(1.3);
histo->SetMarkerSize(0.7);
histo->SetMarkerColor(kBlue);
histo->SetMarkerStyle(8);
histo->Draw();

TF1 *g1;
g1=FindBestGaussianCoreFit(histo);
g1->Draw("same");
cout<<"Prob = "<<g1->GetProb()<<endl;
cout<<"Val = "<<g1->GetParameter(1)<<endl;
cout<<"Valerr = "<<g1->GetParError(1)<<endl;

gStyle->SetOptFit(1111);
//gStyle->SetOptStat(11);

TLegend *l = new TLegend(0.15,0.5,0.4,0.7,"");
l->SetBorderSize(0);
l->SetTextSize(0.04);
TLegendEntry *l3 = l->AddEntry((TObject*)0,"#font[22]{p_{T}^{Gen} : 15 - 17 GeV}","");
TLegendEntry *l1 = l->AddEntry((TObject*)0,"Offset Distribution","");
TLegendEntry *l2 = l->AddEntry((TObject*)0,"Gaussian Fit","");
l1->SetTextColor(kBlue);
l2->SetTextColor(kRed);
l->Draw();

cout<<"Arithmetic mean = "<<histo->GetMean()<<endl;
cout<<"Error of Arithmetic mean = "<<histo->GetMeanError()<<endl;
cout<<"RMS = "<<histo->GetRMS()<<endl;
cout<<"Error of RMS = "<<histo->GetRMSError()<<endl;

double x,q;
q=0.5;
histo->ComputeIntegral();
histo->GetQuantiles(1, &x, &q);
cout<<"Median = "<<x<<endl;

c->SaveAs("Offset_distribution.png");
}

import glob, os, ROOT
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetOptFit(0)
from array import array
import numpy as np
from tdrstyle_JERC import *
import tdrstyle_JERC as TDR
TDR.extraText  = "Simulation"
TDR.extraText2  = "Preliminary"

class L1OffsetVsPt():
    def __init__(self, eta):
	self.doCleaning = True
        self.inputPath = "InputFiles/"
        self.outputPath = "Pdfs/"
        self.fname  = self.inputPath+"L1ClosureVsPt_AK4CHS_default.root" #-> L1OffsetVsPt_AK4CHS_default.root for uncorrected offset
        self.years = ["UL16APV", "UL16nonAPV", "UL17", "UL18"]
        self.mubins = ["0","10","20","30", "40", "50"] 
        self.Hists = {
                "0": {"legend": "0 #leq #mu < 10",
                        "color":  634,
                        "marker": ROOT.kFullCircle,
                        "msize":  1.0,
                        "hname":  "histograms/OffMeantnpuRef_" + eta + "_0",
                      },
                "10": {"legend": "10 #leq #mu < 20",
                        "color":  802,
                        "marker": ROOT.kFullCircle,
                        "msize":  1.0,
                        "hname":  "histograms/OffMeantnpuRef_" + eta + "_1",
                      },
                "20": {"legend": "20 #leq #mu < 30",
                        "color":  402,
                        "marker": ROOT.kFullCircle,
                        "msize":  1.0,
                        "hname":  "histograms/OffMeantnpuRef_" + eta + "_2",
                      },
                "30": {"legend": "30 #leq #mu < 40",
                        "color":  822,
                        "marker": ROOT.kFullCircle,
                        "msize":  1.0,
                        "hname":  "histograms/OffMeantnpuRef_" + eta + "_3",
                      },
		"40": {"legend": "40 #leq #mu < 50",
                        "color":  418,
                        "marker": ROOT.kFullCircle,
                        "msize":  1.0,
                        "hname":  "histograms/OffMeantnpuRef_" + eta + "_4",
                      },
		"50": {"legend": "50 #leq #mu < 60",
                        "color":  842,
                        "marker": ROOT.kFullCircle,
                        "msize":  1.0,
                        "hname":  "histograms/OffMeantnpuRef_" + eta + "_5",
                      },
            }
        
    def Plot(self, eta):
        PlotXMin = 11
        PlotXMax = 5500
        PlotYMin = -10
        PlotYMax = 30
        for year in self.years:
	    TDR.cms_lumi_TeV = TDR.commonScheme["legend"][year]+" Legacy"
            TDR.extraText3.append("#bf{Anti-k_{T} (R = 0.4), PF+CHS}")

            if eta=="BB": 
		TDR.extraText3.append("#bf{0.0<|#eta|<1.3}")
	    elif eta=="EI":
		TDR.extraText3.append("#bf{1.3<|#eta|<2.5}")
	    elif eta=="EO":
		TDR.extraText3.append("#bf{2.5<|#eta|<3.0}")
	    elif eta=="FF":
		TDR.extraText3.append("#bf{3.0<|#eta|<5.0}")
			
            f_ = ROOT.TFile(self.fname.replace("default",year))
            canv = tdrCanvas("L1OffsetVsPt"+year, PlotXMin, PlotXMax, PlotYMin, PlotYMax, "p_{T}^{ptcl}[GeV]", "Offset [GeV]", square=kSquare, isExtraSpace=True)
            canv.SetLogx(True)
            leg = tdrLeg(0.60,0.70,0.89,0.89, textSize=0.04)
	    for bin in self.mubins:
		#for UL16 show bins of mu up to 40 (no events in data with mu>40 and for the APV MC we have a mu<40 cut in the JEC derivation)
		if ((bin=="40" or bin=="50") and (year=="UL16APV" or year=="UL16nonAPV")): continue
                color  = self.Hists[bin]["color"]
                marker = self.Hists[bin]["marker"]
                self.Hists[bin]["hist"] = f_.Get(self.Hists[bin]["hname"])
                self.Hists[bin]["hist"].SetDirectory(0)
                self.Hists[bin]["hist"].SetMarkerSize(self.Hists[bin]["msize"])
		if self.doCleaning:
                    for x in range(1,self.Hists[bin]["hist"].GetNbinsX()+1):
                    	if self.Hists[bin]["hist"].GetBinCenter(x)<15: self.Hists[bin]["hist"].SetBinContent(x,-999)
			if self.Hists[bin]["hist"].GetBinError(x)>2: self.Hists[bin]["hist"].SetBinContent(x,-999)                  	
                tdrDraw(self.Hists[bin]["hist"], "P", marker=marker, mcolor=color )
                leg.AddEntry(self.Hists[bin]["hist"], self.Hists[bin]["legend"], "lp")

            f_.Close()
            canv.SaveAs(self.outputPath+"L1ClosureVsPt_AK4CHS_"+eta+"_"+year+".pdf") #-> L1OffsetVsPt_AK4CHS_ for uncorrected offset
	    canv.Close()
	    del TDR.extraText3[:]

def main():
    etabins = ["BB", "EI", "EO", "FF"]
    for eta in etabins:
    	L1OffsetVsPt(eta).Plot(eta)

if __name__ == '__main__':
    main()

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

class L1L2L3ClosureVsPt():
    def __init__(self):
        self.doCleaning = True
        self.inputPath = "InputFiles/"
        self.outputPath = "Pdfs/"
        self.fname  = self.inputPath+"L1L2L3ClosureVsPt_AK4CHS_default.root"
        self.years = ["UL16APV", "UL16nonAPV","UL17","UL18"]
        self.etabins = ["1.3","2.5","3.0","5.0"]
        self.Hists = {
            "1.3": {"legend": "0.0 < |#eta| < 1.3",
                    "color":  ROOT.kGreen+3,
                    "marker": ROOT.kFullCircle,
                    "msize":  1.0,
                    "hname":  "ak4pfchs/ClosureVsRefPt_JetEta0to1.3",
                  },
            "2.5": {"legend": "1.3 < |#eta| < 2.5",
                    "color":  ROOT.kBlue-4,
                    "marker": ROOT.kFullStar,
                    "msize":  1.3,
                    "hname":  "ak4pfchs/ClosureVsRefPt_JetEta1.3to2.5",
            },
            "3.0": {"legend": "2.5 < |#eta| < 3.0",
                    "color":  ROOT.kRed+1,
                    "marker": ROOT.kFullTriangleUp,
                    "msize":  1.0,
                    "hname":  "ak4pfchs/ClosureVsRefPt_JetEta2.5to3",
            },
            "5.0": {"legend": "3.0 < |#eta| < 5.0",
                    "color":  ROOT.kOrange+1,
                    "marker": ROOT.kFullTriangleDown,
                    "msize":  1.0,
                    "hname":  "ak4pfchs/ClosureVsRefPt_JetEta3to5",
            },
        }


    def Plot(self):
        PlotXMin = 11
        PlotXMax = 5500
        PlotYMin = 0.96
        PlotYMax = 1.04
        for year in self.years:
            TDR.cms_lumi_TeV = TDR.commonScheme["legend"][year]+" Legacy"
            TDR.extraText3 = []
            TDR.extraText3.append("#bf{Anti-k_{T} (R = 0.4), PF+CHS}")
            canv = tdrCanvas("L1L2L3ClosureVsPt"+year, PlotXMin, PlotXMax, PlotYMin, PlotYMax, "p_{T}^{ptcl}[GeV]", "Response", square=kSquare, isExtraSpace=True)
            canv.SetLogx(True)
            leg = tdrLeg(0.60,0.73,0.89,0.90, textSize=0.04)
            lines = {}
            f_ = ROOT.TFile(self.fname.replace("default",year))
            for shift in [+0.01, 0.00, -0.01, +0.001, -0.001]:
                lines[shift] = ROOT.TLine(PlotXMin, 1+shift, PlotXMax, 1+shift)
                lines[shift].SetLineWidth(1)
                lines[shift].SetLineStyle(ROOT.kDotted if shift != 0 else ROOT.kDashed)
                lines[shift].SetLineColor(ROOT.kBlack)
                lines[shift].Draw("same")
            for bin in self.etabins:
                color  = self.Hists[bin]["color"]
                marker = self.Hists[bin]["marker"]
                self.Hists[bin]["hist"] = f_.Get(self.Hists[bin]["hname"])
                self.Hists[bin]["hist"].SetDirectory(0)
                self.Hists[bin]["hist"].SetMarkerSize(self.Hists[bin]["msize"])
                if self.doCleaning:
                    for x in range(1,self.Hists[bin]["hist"].GetNbinsX()+1):
                        if self.Hists[bin]["hist"].GetBinCenter(x)<15: self.Hists[bin]["hist"].SetBinContent(x,0)
                        if self.Hists[bin]["hist"].GetBinError(x)>0.015: self.Hists[bin]["hist"].SetBinContent(x,0)
                tdrDraw(self.Hists[bin]["hist"], "P", marker=marker, mcolor=color )
                leg.AddEntry(self.Hists[bin]["hist"], self.Hists[bin]["legend"], "lp")

            f_.Close()
            canv.SaveAs(self.outputPath+"L1L2L3ClosureVsPt_"+year+".pdf")

def main():
    L1L2L3ClosureVsPt().Plot()

if __name__ == '__main__':
    main()

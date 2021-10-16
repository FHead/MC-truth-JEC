# MC-truth-JEC

<!-- MarkdownTOC depth=0 -->

- [Overview](#overview)
- [Instructions on how to run ntuples from MINIAOD](#ntuples-miniaod)
- [Instructions on how to derive MC-truth JEC](#JEC)
	- [Histograms for PU Reweighting](#PU-reweight)
	- [Pileup Offset Corrections (L1)](#L1)
	- [Relative & Absolute Corrections (L2L3)](#L2L3)

<!-- /MarkdownTOC -->

<a name="overview"></a>
## Overview

The code contained in this package is used for creating and analyzing the L1FastJet and L2Relative MC truth jet energy corrections (JEC). The code is used by the Jet Energy Resolution and Corrections (JERC) subgroup. This is a dedicated workflow for the MC-truth jet energy corrections used by the Athens group. The main code is located at https://github.com/cms-jet/JetMETAnalysis .

<a name="ntuples-miniaod"></a>
## Instructions on how to run ntuples from MINIAOD

Setup your CMSSW : 10_6_X is used for Run2 Ultra-legacy processing and Phase 2 L1T TDR samples (10_6_17 for V15 PUPPI tune) , see also here https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookWhichRelease#DifferentReleases .

mkdir JEC_MCsamples/ \
cd JEC_MCsamples/ \
tcsh \
setenv SCRAM_ARCH slc7_amd64_gcc700 \
cmsrel CMSSW_10_6_17 \
cd CMSSW_10_6_17/src \
cmsenv \
git-cms-init \
git clone https://github.com/izisopou/MC-truth-JEC.git 

Before compiling you need to remove the unnecessary folder MC-truth-JEC/ from this directory $CMSSW_BASE/src/MC-truth-JEC/JetMETAnalysis/JetAnalyzers/src/ . That is because only 2 folders in between the 2 src/ folders should exist in order for scram to work. To do this:

cd $CMSSW_BASE/src/ \
mv -f MC-truth-JEC/* . \
rm -rf MC-truth-JEC 

Then compile:

cd $CMSSW_BASE/src/ \
scram b -j 4 

Once compiling the codes is done with no problems:

cd $CMSSW_BASE/src/JetMETAnalysis/JetAnalyzers/test/

Before submitting jobs to crab run a local test first:

In the code run_JRA_cfg_ilias.py:

1)  In L51 put the correct Global Tag. Find the sample in DAS, click on it and then click "Configs".
2)	In L69 put the number of events to process (something small as a test, for example 100 or 1000). \
3)	In L79 put one of the .root files of the sample in DAS for the test run to process. Find the sample in DAS and click in the label "Files". Then take one of the root files and add root://cms-xrd-global.cern.ch/ in front of the path.

This test will produce a file named JRA.root in the directory you are in, containing these 100 or 1000 events you processed. You can change the name of this output file in L96.

To run the test:

cmsenv \
voms-proxy-init -voms cms \
cmsRun run_JRA_cfg_ilias.py

If there are no errors and the JRA.root is produced correctly (open it to check the tree inside it) then you are good to go.

Submit jobs to crab:

cd $CMSSW_BASE/src/JetMETAnalysis/JetAnalyzers/test/

In the code run_JRA_cfg_ilias.py:

1)	In L69 put as number of events -1 (this means run all the events in the sample).
2)	In L79 it will not matter what you have written because it will instead read the input config file from the custom_crab.py code so no need to change anything here.

As the code custom_crab_JEC.py is:

1)	It will create a folder MyWorkArea/name_related_to_sample/ in the directory you are in (test/).
2)	The output JRA files will be in the output LFN directory (in this case in /eos/cms/store/group/phys_jetmet/ilias/test/ --> Attention: Do not put the /eos/cms/ in front in the LFN directory)

voms-proxy-init -voms cms \
crab submit -c custom_crab_JEC.py

To check the status of jobs in crab:

crab status -d MyWorkArea/name_related_to_sample/

To resubmit jobs if some have failed:

crab resubmit -d MyWorkArea/name_related_to_sample/

When all jobs are finished, the output JRA root files, based on the above custom_crab_JEC.py, will be located in a directory with this format:\
/eos/cms/store/group/phys_jetmet/ilias/test/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/name/yymmdd_XXXXXX/0000/

These JRA_*.root files are the input ntuples for the MC-truth jet energy corrections.

<a name="JEC"></a>
## Instructions on how to derive MC-truth JEC

Setup your CMSSW : 10_5_X is used for Run 2 ultra-legacy calibrations studies , see here https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookWhichRelease#DifferentReleases .

mkdir JEC/ \
cd JEC/ \
tcsh \
setenv SCRAM_ARCH slc7_amd64_gcc700 \
cmsrel CMSSW_10_5_0 \
cd CMSSW_10_5_0/src \
cmsenv \
git-cms-init \
git clone https://github.com/izisopou/MC-truth-JEC.git \
mv -f MC-truth-JEC/* . \
rm -rf MC-truth-JEC \
scram b -j 4

<a name="#PU-reweight"></a>
### Histograms for PU Reweighting

In order to later do PU reweighting when deriving and applying the corrections you need to have 2 input root files with the mu (pileup) distribution; one for data and one for the MC. Note that they should have the same binning (we usually use 100 bins from 0 to 100).

To produce the root file for data:

You need to take the files that are located in the JERC Protolab : \
https://gitlab.cern.ch/cms-jetmet/JERCProtoLab/-/tree/master/macros/lumi_PU/InputFiles 

cd $CMSSW_BASE/src/ \
cmsenv \
pileupCalc.py -i Cert_\*.txt --inputLumiJSON pileup_latest_UL\*.txt --calcMode true --minBiasXsec 69200 --maxPileupBin 100 --numPileupBins 100  MyDataPUHisto.root 

where Cert_\*.txt is the JSON file for the corresponding epoch you want to process (e.g. Cert_271036-284044_13TeV_Legacy2016_Collisions16_JSON_nonAPV.txt for the non-APV UL16 data) and pileup_latest_UL\*.txt for the corresponding year.

To produce the root file for MC:

cd $CMSSW_BASE/src/scripts/ \
root -l \
[0] .x create_MyHist.C


<a name="#L1"></a>
### Pileup Offset Corrections (L1)


<a name="#L2L3"></a>
### Relative & Absolute Corrections (L2L3)




















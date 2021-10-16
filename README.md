# MC-truth-JEC

<!-- MarkdownTOC depth=0 -->

- [Overview](#overview)
- [Instructions on how to run ntuples from MINIAOD](#ntuples-miniaod)
- [Instructions on how to create pileup histograms for PU Reweighting](#PU-reweight)
- [Instructions on how to derive MC-truth JEC](#JEC)
	- [Pileup Offset Corrections (L1)](#L1)
	- [Relative & Absolute Corrections (L2L3)](#L2L3)

<!-- /MarkdownTOC -->

<a name="overview"></a>
## Overview

The code contained in this package is used for creating and analyzing the L1FastJet and L2Relative MC truth jet energy corrections (JEC). The code is used by the Jet Energy Resolution and Corrections (JERC) subgroup. This is a dedicated workflow for the MC-truth jet energy corrections used by the Athens group. The main code is located at https://github.com/cms-jet/JetMETAnalysis .

<a name="ntuples-miniaod"></a>
## Instructions on how to run ntuples from MINIAOD

mkdir JEC_MCsamples/ \
cd JEC_MCsamples/ \
tcsh \
setenv SCRAM_ARCH slc7_amd64_gcc700 \
cmsrel CMSSW_10_6_17 \
cd CMSSW_10_6_17/src \
cmsenv \
git-cms-init \
git clone https://github.com/izisopou/MC-truth-JEC.git \
scram b -j 4 \

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

1)	It will create a folder UL2016/crab_Summer20UL16APVv2_FlatPU_V15_recipe/ in the directory you are in (test/).
2)	The output JRA files will be in the output LFN directory (in this case in /eos/cms/store/group/phys_jetmet/ilias/MCsamples/Summer20UL16APVv2/FlatPU_V15_recipe/ --> Attention: Do not put the /eos/cms/ in front in the LFN directory)

voms-proxy-init -voms cms \
crab submit -c custom_crab_JEC.py

To check the status of jobs in crab:

crab status -d UL2016/crab_Summer20UL16APVv2_FlatPU_V15_recipe/

To resubmit jobs if some have failed:

crab resubmit -d UL2016/crab_Summer20UL16APVv2_FlatPU_V15_recipe/

When all jobs are finished, the output JRA root files in eos will be the input ntuples for the JECs.


<a name="PU-reweight"></a>
## Instructions on how to create pileup histograms for PU Reweighting

mkdir JEC/ \
cd JEC/ \
tcsh \
setenv SCRAM_ARCH slc7_amd64_gcc700 \
cmsrel CMSSW_10_5_0 \
cd CMSSW_10_5_0/src \
cmsenv \
git-cms-init \
git clone https://github.com/izisopou/MC-truth-JEC.git \






















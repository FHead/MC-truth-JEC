***** Instructions on how to run ntuples from MINIAOD for the MC-truth JECs *****

mkdir JEC_MCsamples/
cd JEC_MCsamples/
tcsh
setenv SCRAM_ARCH slc7_amd64_gcc700
cmsrel CMSSW_10_6_17
cd CMSSW_10_6_17/src
cmsenv
git-cms-init
git clone -b izisopou-patch-MINIAOD https://github.com/izisopou/JetMETAnalysis.git

Before you compile do the following:

1)	Open the code: $CMSSW_BASE/src/JetMETAnalysis/JetAnalyzers/bin/jet_correction_analyzer_x.cc
	and between L21 and L22 (under the command #include "xrootd/XProtocol/XProtocol.hh") add:
	#include "xrootd/XrdCl/XrdClXRootDResponses.hh"
	Save the code

2)	Open the code: $CMSSW_BASE/src/JetMETAnalysis/JetUtilities/src/classes_def.xml
	In L52 replace the number "329505606" with "538640849"
	In L6 replace the number "2622915190" with "3423243310"
	Save the code

Then compile:

cd $CMSSW_BASE/src
scram b -j 4

cp /afs/cern.ch/work/i/izisopou/public/JEC_MCsamples/CMSSW_10_6_17/src/MINIAOD_codes/Defaults_cff.py JetMETAnalysis/JetAnalyzers/python/Defaults_cff.py


For the V15 PUPPI tune recipe:

cp /afs/cern.ch/work/i/izisopou/public/JEC_MCsamples/CMSSW_10_6_17/src/MINIAOD_codes/addAlgorithm.py JetMETAnalysis/JetAnalyzers/python/addAlgorithm.py	
cp /afs/cern.ch/work/i/izisopou/public/JEC_MCsamples/CMSSW_10_6_17/src/MINIAOD_codes/customizePuppiTune_cff_V15.py JetMETAnalysis/JetAnalyzers/python/customizePuppiTune_cff_V15.py

!!!
With the codes as they are the PUPPI V15 recipe is applied.
!!!

Codes in order to submit jobs in crab:

cp /afs/cern.ch/work/i/izisopou/public/JEC_MCsamples/CMSSW_10_6_17/src/MINIAOD_codes/run_JRA_cfg_ilias.py JetMETAnalysis/JetAnalyzers/test/
cp /afs/cern.ch/work/i/izisopou/public/JEC_MCsamples/CMSSW_10_6_17/src/MINIAOD_codes/custom_crab_JEC.py JetMETAnalysis/JetAnalyzers/test/

In the code run_JRA_cfg_ilias.py in L51 put the correct Global Tag that you will find written in DAS if you click in the sample and then click "Configs".


Before submitting jobs to crab run a local test first:

cd $CMSSW_BASE/src/JetMETAnalysis/JetAnalyzers/test/

In the code run_JRA_cfg_ilias.py:

1)	In L69 put the number of events to process (something small as a test, for example 100 or 1000).
2)	In L79 put one of the .root files of the sample in DAS for the test run to process. You will find the sample in DAS and click in the 		label "Files". Then take one of the root files and add root://cms-xrd-global.cern.ch/ in front of the path.

This test will produce a file named JRA.root in the directory you are in, containing these 100 or 1000 events you processed. You can change the name of this output file in L96.

To run the test:

cmsenv
voms-proxy-init -voms cms
cmsRun run_JRA_cfg_ilias.py

If there are no errors and the JRA.root is produced correctly (open it to check the tree inside it) then you are good to go.


Submit jobs to crab:

cd $CMSSW_BASE/src/JetMETAnalysis/JetAnalyzers/test/

In the code run_JRA_cfg_ilias.py:

1)	In L69 put as number of events -1 (this means run all the events in the sample).
2)	In L79 it will not matter what you have written because it will instead read the input file from the custom_crab.py code so no need 		to change anything here.

As the code custom_crab_JEC.py is:

1)	It will create a folder UL2016/crab_Summer20UL16APVv2_FlatPU_V15_recipe/ in the directory you are in.
2)	The output JRA files will be in the output LFN directory (in this case in /eos/cms/store/group/phys_jetmet/ilias/MCsamples/Summer20UL16APVv2/FlatPU_V15_recipe/  --> Attention: Do not put the /eos/cms/ in front in the LFN directory)

voms-proxy-init -voms cms
crab submit -c custom_crab_JEC.py

To check the status of jobs in crab:

crab status -d UL2016/crab_Summer20UL16APVv2_FlatPU_V15_recipe/

To resubmit jobs if some have failed:

crab resubmit -d UL2016/crab_Summer20UL16APVv2_FlatPU_V15_recipe/

When all jobs are finished, the output JRA root files in eos will be the input ntuples for the JECs.






***** Instructions on how to derive JECs *****

mkdir JEC/
cd JEC/
tcsh
setenv SCRAM_ARCH slc7_amd64_gcc700
cmsrel CMSSW_10_5_0
cd CMSSW_10_5_0/src
cmsenv
git-cms-init
git clone -b izisopou-patch-NewFittingApproaches https://github.com/izisopou/JetMETAnalysis.git


!!!!!!!!!!!!!! Need to update this, put all codes in github, along with the condor/ shell scripts !!!!!!!!!!!!!!


1) Instructions on how to produce the histograms needed for PU Reweighting 






















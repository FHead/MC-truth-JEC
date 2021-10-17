# MC-truth-JEC

<!-- MarkdownTOC depth=0 -->

- [Overview](#overview)
- [Instructions on how to run ntuples from MINIAOD](#ntuples-miniaod)
- [Instructions on how to derive MC-truth JEC and plot closures](#JEC)
	- [Histograms for PU Reweighting](#PU-reweight)
	- [Pileup Offset Corrections (L1)](#L1)
	- [Relative & Absolute Corrections (L2L3)](#L2L3)
- [Instructions on how to plot the correction factors](#Corr-Factors)

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

Before compiling you need to remove the unnecessary folder "MC-truth-JEC" from this directory $CMSSW_BASE/src/MC-truth-JEC/JetMETAnalysis/JetAnalyzers/src/ . That is because only 2 folders in between the 2 src/ folders should exist in order for scram to work. To do this:

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
## Instructions on how to derive MC-truth JEC and plot closures

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

***Notice (useful for later)*** 
1) In all the steps of the MC-truth JECs you will be submitting jobs to condor. Once you submit them you can check their status by doing: \
condor_q \
For each job you submit, when it is finished, a corresponding root file should appear in eos. Additionally, in the directory $CMSSW_BASE/src/condor/Log/ for each job 3 files will appear: err,log,out. Check the err ones to see if there was an error in your codes that made the jobs crash. If you have a bug somewhere then the output root files in eos will not be created or they will be created empty, so you can also check them by going to the eos directory and doing ls -lh to see the size of the files and if they have closed properly. If the code does not have any bugs then these files should be a few MB each. \
If there are no bugs and the root files have been created correctly then it happens that condor did not run all jobs (due to technical issues related to condor and not you). Therefore you should always check how many output root files were created (by doing ls | wc -l): they should be the same number as the jobs you submitted. If they are fewer then you can resubmit the jobs (./SubmitStep*.sh as you did in the first time) until all root files are processed. 

2) Every time you do any changes to the codes in $CMSSW_BASE/src/JetMETAnalysis/ you should then compile the codes doing *scram b -j 4*

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

cd $CMSSW_BASE/src/ \
cmsenv \
cd condor/ \

Copy the MyMCPUHisto.root and MyDataPUHisto.root to $CMSSW_BASE/src/condor/Files/

First, you need to determine what jet collection you want to process. The codes in condor/ are written for ak4pfchs but you can easily change for ak8pfchs, ak4puppi, ak8puppi. Go to the $CMSSW_BASE/src/condor/ListRunLumi.cpp script and in L20 change to the jet collection you want. Then do:

make

This will change the unreadable file RunListRunLumi appropriately.

Make sure that in the script $CMSSW_BASE/src/condor/Setup_CMSSW.sh the correct path to the condor/ folder is written.

Determine the input MC samples and output files in eos: \
Open the $CMSSW_BASE/src/condor/Setup_FileLocation.sh code and write the correct paths to the NoPUFiles (NoPU or EpsilonPU MC) and WithPUFiles (FlatPU MC). Moreover, write the output directories in eos where the output root files in the 4 steps of the MC-truth JECs will appear. 

**Step1**

In the first step the events between the NoPU/EpsilonPU and the FlatPU sample are matched one by one: 

./SubmitStep1.sh

This will submit jobs to condor. Once they are done then some text files will appear in the Step1Output in eos, where all the events that are matched are written.

./HarvestStep1.sh

This will take as input the txt files in the Step1Output from eos and will create a file named "MatchedFiles" in the directory $CMSSW_BASE/src/condor/Files/ . In that file all the events that are matched between the NoPU/EpsilonPU and the FlatPU sample will be listed.

**Step2**

In this step, the jets from the matched events will be also matched and the offset will be calculated: offset = jetpT(FlatPU) - jetpT(NoPU/EpsilonPU).\
First do:

./RunPrepareStep2Submission 1 > SubmitStep2.sh

This takes as input the $CMSSW_BASE/src/condor/Files/MatchedFiles file and rewrites the SubmitStep2.sh code. Open the SubmitStep2.sh code to see how it has changed. It should list the directories for the MC samples where the JRA_\*.root are located. In L3 you should write the full path of where the Setup_FileLocation.sh is located. Then, replace L11 with:

echo "+JobFlavour = testmatch" >> $SubmissionFile

and immediately below add the line:

echo "+request_cpus=3">>$SubmissionFile

The above lines will help the jobs to run quicker.

The SubmitStep2.sh code calls the Step2PUMatching.sh one so open it and modify it appropriately. In turn, the Step2PUMatching.sh code calls the $CMSSW_BASE/src/JetMETAnalysis/JetAnalyzers/bin/jet_match_x.cc. In L40 of the Step2PUMatching.sh make sure it is false (we do not apply any corrections, we want to derive them).

Once you have modified the above codes in your liking, submit the jobs:

./SubmitStep2.sh

In this particular step not all root files are usually created when all jobs are done so resubmit with the above command until all root files in eos are created.

Then, open the HarvestStep2.sh code, which in turn calls the $CMSSW_BASE/src/JetMETAnalysis/JetAnalyzers/bin/jet_synchfit_x.cc one. In this code, in L425-443 you determine what fit function should be used (L1Complex, L1Simple, L1SemiSimple) so comment out 2 of them and leave the one to be used. In L640-648 you also determine the range of the 2D fit. In the HarvestStep2.sh code in L14 you define if you want to use ak4 or ak8 in the jet_synchfit_x.cc code.

./HarvestStep2.sh

This code does 2 things. First it hadds the Step2Output files in eos and then it will create the **ParallelMCL1_L1FastJet_AK4PFchs.txt** file in the directory $CMSSW_BASE/src/condor/Files/ (or the directory you have specified in L9 of the HarvestStep2.sh code), along with some plots concerning the 2D fits.

This hadded root file contains information for the offset (before you apply any correction to the MC). To plot it do:

cd $CMSSW_BASE/src/ \
cmsenv \
jet_synchplot_x -inputDir ./ -algo1 ak4pfchs -algo2 ak4pfchs -outDir ./ -outputFormat .png -fixedRange false -tdr true -npvRhoNpuBinWidth 10 -NBinsNpvRhoNpu 5

where the -inputDir will be the directory where the hadded output_ak4pfchs.root file is located. In the -outDir many .png files will be created, the ones that depict the offset vs pT for the 4 different eta regions and different mu bins are the OffMeantnpuRef_\*.png. The above code calls the $CMSSW_BASE/src/JetMETAnalysis/JetUtilities/src/SynchFittingProcedure.hh. In it, in L430-529 you determine how the offset will be calculated (mean, median or mode) and in L894-1072 how to plot the histograms. 

You are basically done with the L1 corrections, since you have derived the L1 JEC text file. However, you should check the quality of these L1 corrections and if they actually remove the pileup as intended. You have already plotted the offset (should be non-zero), and you need to do that again, applying now this text file you have produced. For that, you need to repeat the Step2 procedure but now in the Step2PUMatching.sh code in L40 make sure it is true and immediately below add the line -JECpar ParallelMCL1_L1FastJet_AK4PFchs.txt. Then sumbit the jobs again with ./SubmitStep2.sh but make sure *the output root files are in a different location as before so as to not overwrite anything!* When the jobs are done you should **not** run the HarvestStep2.sh code because you do not want to derive any corrections now. You only need to hadd the root files in eos and now use this new hadded root file as input in order to plot the offset again. The offset now should be ~0 since you have applied the ParallelMCL1_L1FastJet_AK4PFchs.txt file in the PU sample.

When you plot the offset, you can use the mean, median or mode (gaussian fits) for the offset distribution (SynchFittingProcedure.hh). To replicate what this code does in order to check by hand the gaussian fits point-by-point:

cd $CMSSW_BASE/src/scripts/ \
root -l \
[0] .x Plot_GausFit_Offset.C

<a name="#L2L3"></a>
### Relative & Absolute Corrections (L2L3)

Edit the $CMSSW_BASE/src/condor/Setup_FileLocation.sh code with the input WithPUFiles which can be either the FlatPU MC that was used for the L1 derivation or the PremixedPU MC. The NoPUFiles is not used for the L2L3 corrections.

**Step3**

In this step the L1 correction txt file is applied to the PU MC and the response histos in bins of eta and pT are created. The SubmitStep3.sh code calls the Step3ApplyL1.sh code which in turn calls the jet_response_analyzer_x.cc code. For PUPPI jets that we do not derive L1 corrections, comment out L19-28 in the Step3ApplyL1.sh code so that the step where the L1 corrections are applied to the MC is skipped.

./SubmitStep3.sh

When all jobs are done edit the HarvestStep3.sh code, which calls the code $CMSSW_BASE/src/JetMETAnalysis/JetUtilities/src/L2Creator.cc. When standard+Gaussian options is specified in L20 of HarvestStep3.sh then the inverse of the median response will be fitted with this function (L1076 of L2Creator.cc) for all 82 eta bins. In L1141-1177 the initial parameters for all the fits are specified. Since in some of the eta bins the fits fail to converge, you can change the set of the initial parameters for specific eta bins in L402-490.

./HarvestStep3.sh

This will hadd the output root file of Step3Output and then do the fits in order to derive the L2L3 corrections, thus creating the **ParallelMCL1_L2Relative_AK4PFchs.txt** file in the directory $CMSSW_BASE/src/condor/Files/, along with the file l2.root that contains the graphs with all fits. When running this command the fit probability for all fits will be printed so you will have to identify which fits have failed, modify the L2Creator.cc code, and rerun until all fits converge and the final txt file has the best quality. 

**Step4**

In this step both JEC text files (L1 + L2L3) are applied to the MC and the response is calculated and plotted as a function of eta and pt. 

<a name="#Corr-Factors"></a>
## Instructions on how to plot the correction factors

















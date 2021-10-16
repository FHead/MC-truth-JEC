#!/bin/sh --login

#BSUB -q 1nh
export DISPLAY=localhost0.0
WorkDir=$1
NoPUFile=$2
PUFile=$3
Output=$4
ID=$5

source $WorkDir/Setup_CMSSW.sh

if [[ $NoPUFile != *":"* ]]; then
   cp $NoPUFile NoPUFile.root
else
   hadd -f -k NoPUFile.root `echo $NoPUFile | tr ':' ' '`
fi

if [[ $PUFile != *":"* ]]; then
   cp $PUFile PUFile.root
else
   hadd -f -k PUFile.root `echo $PUFile | tr ':' ' '`
fi

cp $WorkDir/Files/My*.root .
cp $WorkDir/Files/*.txt .	#this is for when you have created the L1 txt file and you rerun SubmitStep2.sh to apply it and calculate the 					 corrected offset

echo Current directory content
ls
echo

jet_match_x \
   -basepath // \
   -samplePU PUFile.root \
   -sampleNoPU NoPUFile.root \
   -algo1 ak4pfchs \
   -algo2 ak4pfchs \
   -iftest false \
   -maxEvts 10000000 \
   -ApplyJEC false \
   -outputPath ./ \
   -npvRhoNpuBinWidth 10 \	#bin width of mu binning
   -NBinsNpvRhoNpu 9 \		#number of mu bins
   -MCPUReWeighting MyMCPUHisto.root \
   -DataPUReWeighting MyDataPUHisto.root \
   -useweight false \
   -nrefmax 3 \
   -doNotSave false

mv output_ak4pfchs.root ${Output}/Result_${ID}.root

# After you run SubmitStep2.sh and then HarvestStep2.sh to derive the L1 corrections and create the L1 txt file, then rerun SubmitStep2.sh to
# calculate the corrected offset. When you do that replace -ApplyJEC false with:
#
# -ApplyJEC true \
# -JECpar ParallelMCL1_L1FastJet_AK4PFchs.txt \
#
# Note that the -JECpar command should be *exactly* below the -ApplyJEC one



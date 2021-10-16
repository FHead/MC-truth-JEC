#!/bin/sh

source $CMSSW_BASE/src/condor/Setup_FileLocation.sh

hadd -k -f $Step3Output/Merged.root $Step3Output/JRA_*root

jet_l2_correction_x \
   -input $Step3Output/Merged.root \
   -algs ak4pfchsl1 \
   -era ParallelMCL1 \
   -output l2.root \
   -outputDir Files/ \
   -makeCanvasVariable AbsCorVsJetPt:JetEta \
   -l2l3 true \
   -batch true \
   -histMet median \
   -delphes false \
   -maxFitIter 30 \
   -l2calofit DynamicMin \
   -l2pffit standard+Gaussian \
   -ptclipfit true \

#rename the output text file
mv Files/ParallelMCL1_L2Relative_AK4PFchsl1.txt Files/ParallelMCL1_L2Relative_AK4PFchs.txt

#For splines instead of fitting function
#  -l2pffit splineAkima \
#  -ptclip 8

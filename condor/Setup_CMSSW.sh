export SCRAM_ARCH=slc7_amd64_gcc700
cd $CMSSW_BASE/src/condor/
eval `scramv1 runtime -sh`
cd - > /dev/null


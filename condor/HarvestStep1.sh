#!/bin/sh

source $CMSSW_BASE/src/condor/Setup_FileLocation.sh

cat $Step1Output/*/*txt | ./RunMatchFiles > Files/MatchedFiles





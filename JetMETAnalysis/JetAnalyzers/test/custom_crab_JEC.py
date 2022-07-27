from WMCore.Configuration import Configuration
config = Configuration()
config.section_("General")
config.General.requestName = 'name_related_to_sample'
config.General.workArea = 'MyWorkArea'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'run_JRA_cfg_MCtruth.py'
config.JobType.allowUndistributedCMSSW = True
#config.JobType.inputFiles = ['']

config.section_("Data")
config.Data.inputDataset = '/QCD_Pt-15to7000_TuneCP5_Flat2018_13TeV_pythia8/RunIISummer20UL16MiniAODv2-106X_mcRun2_asymptotic_v17-v1/MINIAODSIM'
config.Data.inputDBS = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 2
config.Data.publication = False
config.Data.publishDBS = 'https://cmsweb.cern.ch/dbs/prod/phys03/DBSWriter/'
config.Data.outputDatasetTag = 'name'
config.Data.outLFNDirBase = '/store/group/phys_jetmet/ilias/test/'
config.Data.ignoreLocality = True

config.section_("Site")
config.Site.blacklist = ['T3_US_TAMU']
config.Site.blacklist = ['T3_US_UCR']
config.Site.whitelist = ['T2_IT_Bari']
config.Site.storageSite = 'T2_CH_CERN'

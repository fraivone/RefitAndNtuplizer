import sys
import os
import difflib
import subprocess
import CRABClient
from WMCore.Configuration import Configuration 
from CRABAPI.RawCommand import crabCommand
import argparse
from argparse import RawTextHelpFormatter
from pathlib import Path
import time
baseDirectory = Path(__file__).parents[1]
baseDirectory = os.path.abspath(baseDirectory)
print (baseDirectory)


parser = argparse.ArgumentParser(
        description='''Scripts that: \n\t-Submit jobs for making GEMCommonNtuples''',
        epilog="""Typical exectuion\n\t python3  P5Data_crabConfig.py  --RunList 348773 --Dataset Express""",
        formatter_class=RawTextHelpFormatter
)
parser.add_argument('--RunList','-rl', type=int,help="run(s) to be ntuplized, space separated",required=True,nargs='*')
parser.add_argument('--Dataset','-d',nargs='?',choices=['Express', 'Prompt','ZeroBias','ZMu','MinimumBias'],help='Dataset to be used (check availability on DAS)',required=True)

args = parser.parse_args()
if args.Dataset == 'Express':
    inputDataset= ['/ExpressPhysics/Run2022C-Express-v1/FEVT']
    globalTag = '124X_dataRun3_Express_v5'
    unitsPerJob = 20
    fileSplitting = "FileBased"
elif args.Dataset == 'Prompt':
    globalTag = '124X_dataRun3_Prompt_v4'
    inputDataset = ['/Muon/Run2022C-PromptReco-v1/AOD']
    unitsPerJob = 20
    fileSplitting = "FileBased"
elif args.Dataset == 'ZeroBias':
    globalTag = '124X_dataRun3_Prompt_v4'
    inputDataset = ["/ZeroBias/Run2022C-PromptReco-v1/AOD"]#['/ZeroBias'+str(i)+'/Run2022B-PromptReco-v1/AOD' for i in range(20)]
    unitsPerJob = 1
    fileSplitting = "FileBased"
elif args.Dataset == 'ZMu':
    globalTag = '130X_dataRun3_Prompt_v3'
    inputDataset = ["/Muon0/Run2023C-ZMu-PromptReco-v3/RAW-RECO","/Muon1/Run2023C-ZMu-PromptReco-v3/RAW-RECO"]
    unitsPerJob = 20
    fileSplitting = "FileBased"
elif args.Dataset == 'MinimumBias':
    globalTag = '124X_dataRun3_Prompt_v4'
    inputDataset = ['/MinimumBias/Run2022B-PromptReco-v1/AOD']
    unitsPerJob = 1
    fileSplitting = "FileBased"

config = Configuration()

config.section_("General")
config.General.workArea = "{}/CRAB_SUB/".format(baseDirectory)
config.General.transferOutputs = True
config.General.transferLogs = True

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
# misalignement
#config.JobType.psetName = "{}/test/muDpgNtuples_cfg_misalignement.py".format(baseDirectory)
config.JobType.psetName = "{}/test/muDpgNtuples_cfg.py".format(baseDirectory)
config.JobType.allowUndistributedCMSSW = True
config.JobType.pyCfgParams = ['isMC=False', 'storeAMCStatus=True','storeOHStatus=True','nEvents=-1','globalTag='+str(globalTag)]
#config.JobType.maxMemoryMB = 5000

config.section_("Data")
config.Data.inputDBS = 'global'
config.Data.splitting = fileSplitting
config.Data.unitsPerJob = unitsPerJob
config.Data.publication = False
config.Data.outLFNDirBase = '/store/group/dpg_gem/comm_gem/P5_Commissioning/2022/GEMCommonNtuples'
config.Data.allowNonValidInputDataset = True

config.section_("Site")
config.Site.storageSite = 'T2_CH_CERN'


run_list = args.RunList


print("Submitting runs= ",run_list)
print("Dataset = " ,inputDataset)
print("GlobalTag = ",globalTag)
print(config)


for run_number in run_list:
    for indataset in inputDataset:
        ## Check dataset exsistance
        das_query = ' dasgoclient -query="dataset run='+str(run_number)+'"'
        availble_datasets = subprocess.check_output(das_query,shell=True).decode("utf-8").split('\n')
        if indataset in availble_datasets:
            print(f"Dataset {indataset} exists for run {run_number}")
        else:
            print(f"Dataset {indataset} not avilable for run {run_number}\n\t NO JOB(S) SUBMITTED")
            print(f"Maybe you meant {difflib.get_close_matches(indataset,availble_datasets)}\n")
            time.sleep(2)
            continue

        outputName = str(run_number)+'_'+str(args.Dataset)
        crab_folderName = str(run_number)+'_'+indataset.split("/")[1]
        config.Data.inputDataset = indataset
        config.Data.outputDatasetTag = outputName
        ## misalignement
        #config.JobType.inputFiles = ['/afs/cern.ch/user/f/fivone/Documents/NTuplizer/CollisionsRun3/CMSSW_12_4_6/src/MuDPGAnalysis/MuonDPGNtuples/test/GEM_1cm_xshift.db']
        config.General.requestName = crab_folderName#+"_misalignement_1cm_x"
        config.Data.runRange = str(run_number)
        crabCommand('submit', config = config)
sys.exit(0)

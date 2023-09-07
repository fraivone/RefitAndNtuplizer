import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras
from Configuration.AlCa.GlobalTag import GlobalTag

import os
import subprocess
import sys


"""
OPTIONS SUMMARY
* globalTag       --> GT of the run 
* nEvents         --> Number of events to be processed
* displacement    --> Track propagation displacement: propagate the muon tracks not on GE11 ROB but on the virtual surface of ROB moved closed to the IP by <displacement>
* STA             --> Process CSC hltDigis and build standalong tracks. Only works for RPC Monitor
* isMC            --> MC dataset
* reUnpack        --> gem digis not stored in prompt reco. In case you need to access the GEM digis on a RAW-RECO dataset (e.g. reading OH or AMC status) the reunpakc has to be triggered. The RPCMonitor already contains the digis so it's not needed there.
* storeOHStatus   --> Enables the filler for the OHStatus
* storeAMCStatus  --> Enables the filler for the AMCStatus
* GE21            --> Forces the runpacking of gemDigi and then the construction of GE21 rechits which is turned off by default

"""


options = VarParsing.VarParsing()

options.register('globalTag',
                 '130X_dataRun3_Prompt_v3', #'124X_dataRun3_Prompt_v4',
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Global Tag")

options.register('nEvents',
                 -1, #to run on a sub-sample
                 #-1, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Maximum number of processed events")

options.register('displacement',
                 0, ## propagate the track at GEM RO board + displacement
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.float,
                 "Maximum number of processed events")

options.register('STA',
                 False, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Process STA from RPCMonitor")

options.register('isMC',
                 False, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Dataset is MC")

options.register('reUnpack',
                 False, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "enables reprocessing of digis: i.e OHStatus is not stored in RECO datesets, but can be extracted by re-unpacking data from a RAW dataset.")

options.register('storeOHStatus',
                 True, #default value,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Save OH status info from unpacker")

options.register('storeAMCStatus',
                 True, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Save AMC status info from unpacker")

options.register('GE21',
                 False, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "enables storing of GE21 rechits, disabled by default in CMSSW: i.e when running on a RAW dataset it's possible to reprocess digi and build GE21 rechits and save them in the ntuples")

options.register('inputFolder',
                 #/eos/cms/store/
                 #"/eos/cms/store/data/Run2022D/Muon/RAW-RECO/ZMu-PromptReco-v2/000/357/734/00000/",
                 "/eos/cms/store/group/dpg_gem/comm_gem/reRECO/Muon/crab_gemReReco_hv_re3/230213_023322/0000/",
                 #"/eos/cms/store/group/dpg_gem/comm_gem/reRECO/SingleMuon/GEM-reRECO-GEM-only__Run2022B-ZMu-PromptReco-v1__RAW-RECO/220721_151149/0000/",
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "EOS folder with input files")

options.register('secondaryInputFolder',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "EOS folder with input files for secondary files")

options.register('ntupleName',
                 'MuDPGNtuple', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Name for output ntuple")

options.parseArguments()

process = cms.Process("MUNTUPLES",eras.Run3)#Run2_2018)

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
#SkipEvent = cms.untracked.vstring('ProductNotFound')
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.nEvents))


#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.GlobalTag.globaltag = cms.string(options.globalTag)
process.GlobalTag = GlobalTag(process.GlobalTag, options.globalTag, '')

process.source = cms.Source("PoolSource",
        fileNames = cms.untracked.vstring(),
        secondaryFileNames = cms.untracked.vstring()
)

if "eos/cms" in options.inputFolder:
    #files = subprocess.check_output(['xrdfs', 'root://eoscms.cern.ch/', 'ls', options.inputFolder]) ## Did work with CMSSW 11XX, not anymore w CMSSW 12
    files = os.listdir(options.inputFolder)
    #process.source.fileNames = ["file:"+options.inputFolder + f for f in files if "07a64f0e-25eb-40b6-b2a6-e8971a4e0ce8.root" in f]
    process.source.fileNames = ["root://cms-xrd-global.cern.ch//store/data/Run2023D/Muon0/RAW-RECO/ZMu-PromptReco-v1/000/370/293/00000/2244179c-9f4c-4fec-b1d0-97650a4d3091.root"]

elif "/xrd/" in options.inputFolder:
    files = subprocess.check_output(['xrdfs', 'root://cms-xrdr.sdfarm.kr/', 'ls', options.inputFolder])
    process.source.fileNames = ["root://cms-xrdr.sdfarm.kr//" +f for f in files.split()]

else:
    files = subprocess.check_output(['ls', options.inputFolder])
    process.source.fileNames = ["file://" + options.inputFolder + "/" + f for f in files.split()]

if options.secondaryInputFolder != "" :
    files = subprocess.check_output(["ls", options.secondaryInputFolder])
    process.source.secondaryFileNames = ["file://" + options.secondaryInputFolder + "/" + f for f in files.split()]


if options.STA == False:
    process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string(options.ntupleName+".root")
    )
else:
    process.TFileService = cms.Service('TFileService',
                                   fileName = cms.string(options.ntupleName+"_STA.root")
    )

from RecoMuon.Configuration.RecoMuon_cff import *
#from RecoVertex.BeamSpotProducer.BeamSpot_cff import offlineBeamSpot

process.load('Configuration/StandardSequences/GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('RecoLocalMuon.GEMRecHit.gemRecHits_cfi')
process.load("TrackingTools/TransientTrack/TransientTrackBuilder_cfi")
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi')
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi')
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi')
process.load('RecoMuon.StandAloneMuonProducer.standAloneMuons_cfi')
process.load('RecoMuon.Configuration.RecoMuonPPonly_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cff")
process.load('MuDPGAnalysis.MuonDPGNtuples.muNtupleProducer_cfi')

process.standAloneMuons = process.standAloneMuons.clone()
process.standAloneMuons.STATrajBuilderParameters.FilterParameters.EnableGEMMeasurement = cms.bool(False)
process.standAloneMuons.STATrajBuilderParameters.BWFilterParameters.EnableGEMMeasurement = cms.bool(False)

process.standAloneMuons.STATrajBuilderParameters.FilterParameters.CSCRecSegmentLabel = cms.InputTag("hltCscSegments")
process.standAloneMuons.STATrajBuilderParameters.FilterParameters.RPCRecSegmentLabel = cms.InputTag("hltRpcRecHits")
process.standAloneMuons.STATrajBuilderParameters.FilterParameters.DTRecSegmentLabel = cms.InputTag("hltDt4DSegments")
process.standAloneMuons.STATrajBuilderParameters.BWFilterParameters.RPCRecSegmentLabel = cms.InputTag("hltRpcRecHits")
process.standAloneMuons.STATrajBuilderParameters.BWFilterParameters.CSCRecSegmentLabel = cms.InputTag("hltCscSegments")
process.standAloneMuons.STATrajBuilderParameters.BWFilterParameters.DTRecSegmentLabel = cms.InputTag("hltDt4DSegments")
ancientMuonSeed.EnableDTMeasurement = False
ancientMuonSeed.CSCRecSegmentLabel = "hltCscSegments"
muonstandalonereco = cms.Sequence(process.offlineBeamSpot + standAloneMuonSeeds * process.standAloneMuons)


process.muNtupleProducer.isMC = cms.bool(options.isMC)
process.muNtupleProducer.storeOHStatus = cms.bool(options.storeOHStatus)
process.muNtupleProducer.storeAMCStatus = cms.bool(options.storeAMCStatus)
process.muNtupleProducer.STA = cms.bool(options.STA)
process.muNtupleProducer.displacement = cms.double(options.displacement)

if options.reUnpack and options.GE21:
    process.gemRecHits.ge21Off = cms.bool(not options.GE21) ## user selection GE21 = True means "store GE21 rechits"
    process.p = cms.Path(
        process.muonGEMDigis *
        process.gemRecHits *
        process.muNtupleProducer)
elif options.reUnpack:
    process.p = cms.Path(
        process.muonGEMDigis *
        process.muNtupleProducer)
elif options.STA:
    #process.muNtupleProducer.muonTag = cms.untracked.InputTag("TestSTA")

    # TO STORE OUTPUT FILE
    #process.gino = cms.OutputModule("PoolOutputModule", outputCommands = cms.untracked.vstring("keep *_*_*_*"), fileName=cms.untracked.string("out.root"))
    process.p = cms.Path(
        muonstandalonereco *
        process.muNtupleProducer
        )
    # TO STORE OUTPUT FILE
    #process.this_is_the_end = cms.EndPath(process.gino)
else:
    process.p = cms.Path(
        process.muNtupleProducer)

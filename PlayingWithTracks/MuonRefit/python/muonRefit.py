import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras
from Configuration.AlCa.GlobalTag import GlobalTag

from RecoMuon.Configuration.RecoMuon_cff import *

process = cms.Process("PIPPO",eras.Run3)

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
process.load('PlayingWithTracks.MuonRefit.MuonRefit_cfi')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')


process.GlobalTag = GlobalTag(process.GlobalTag,"130X_dataRun3_Prompt_v3", '')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False))
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50)
)

process.source = cms.Source("PoolSource",
#                            fileNames = cms.untracked.vstring("file:/lustre/cms/store/data/Run2023C/Muon0/MINIAOD/PromptReco-v4/000/368/410/00000/8db2c708-256c-4e30-8d0d-2fbbf78952e1.root")
#                            fileNames =cms.untracked.vstring("file:myaod_numEvent100.root")
                            fileNames =cms.untracked.vstring("file:/eos/cms/store/group/dpg_gem/comm_gem/StreamP5/myraw-reco_numEvent500.root")
)



process.output = cms.OutputModule("PoolOutputModule",
                                  outputCommands=cms.untracked.vstring(
                                      "keep *_*_*_*",#"drop *_*_*_*",
                                  ),
                                  
                                  fileName=cms.untracked.string(
                                      'output_Revit.root')


)

process.p = cms.Path(process.MuonRefitProducer)
process.outpath = cms.EndPath(process.output)

import FWCore.ParameterSet.Config as cms
from RecoMuon.TrackingTools.MuonServiceProxy_cff import MuonServiceProxy

muNtupleProducer = cms.EDAnalyzer("MuNtupleProducer",
                                  MuonServiceProxy,
                                  ph1DtDigiTag = cms.untracked.InputTag("muonDTDigis"),
                                  ph2DtDigiTag = cms.untracked.InputTag("none"),

                                  ph1DtSegmentTag = cms.untracked.InputTag("dt4DSegments"),        
                                  ph2DtSegmentTag = cms.untracked.InputTag("none"),

                                  ph1DTtTrigMode = cms.untracked.string('DTTTrigSyncFromDB'),
                                  isMC = cms.bool(False),
                                  gemDigiTag = cms.untracked.InputTag("hltMuonGEMDigis"),
                                  gemOHStatusTag = cms.untracked.InputTag("hltmuonGEMDigis", "OHStatus"),
                                  gemAMCStatusTag = cms.untracked.InputTag("hltmuonGEMDigis", "AMCStatus"),
                                  #gemDigiTag = cms.untracked.InputTag("simMuonGEMDigis"),
                                  gemRecHitTag = cms.untracked.InputTag("gemRecHits"),
                                  gemSegmentTag = cms.untracked.InputTag("gemSegments"),
                                  cscSegmentTag = cms.untracked.InputTag("cscSegments"),
                                  muonTag = cms.untracked.InputTag("muons"),
                                  standalonemuonTag = cms.untracked.InputTag("standAloneMuons","UpdatedAtVtx"),
                                  tcdsTag = cms.untracked.InputTag("tcdsDigis","tcdsRecord","RECO"),
                                  
                                  gemSimHitTag = cms.untracked.InputTag("g4SimHits","MuonGEMHits"),
                                  muonSimTag = cms.untracked.InputTag("muons"),
                                  genParticlesTag = cms.untracked.InputTag("genParticles"),
                                  primaryVerticesTag = cms.untracked.InputTag("offlinePrimaryVertices"),
                                                                 tracks=cms.untracked.InputTag("muons"),
                                  verbose = cms.bool(False),
                                  # Refitting exluding GEM? (1=layer2 excluded, 2=layer1 excluded, 3-=exclude both, 0=exclude none)
                                  refitOption=cms.int32(3),
                                  MuonTransformer = cms.PSet(DoPredictionsOnly = cms.bool(False),
                                                              Fitter = cms.string('KFFitterForRefitInsideOut'),
                                                              #        TrackerRecHitBuilder = cms.string('WithTrackAngleAndTemplate'),                                                   
                                                              TrackerRecHitBuilder = cms.string('WithTrackAngle'),
                                                              Smoother = cms.string('KFSmootherForRefitInsideOut'),
                                                              MuonRecHitBuilder = cms.string('MuonRecHitBuilder'),
                                                              MTDRecHitBuilder = cms.string('MTDRecHitBuilder'),
                                                              RefitDirection = cms.string('alongMomentum'),
                                                              RefitRPCHits = cms.bool(True),
                                                              RefitGEMHits = cms.bool(True),
                                                              Propagator = cms.string('SmartPropagatorAnyRKOpposite')
                                                            ),
                                  ph1DTtTrigModeConfig = cms.untracked.PSet(vPropWire = cms.double(24.4),
                                                                            doTOFCorrection = cms.bool(False),
                                                                            tofCorrType = cms.int32(2),
                                                                            wirePropCorrType = cms.int32(0),
                                                                            doWirePropCorrection = cms.bool(False),
                                                                            doT0Correction = cms.bool(True),
                                                                            tTrigLabel = cms.string(''),
                                                                            t0Label = cms.string(''),
                                                                            debug = cms.untracked.bool(False)
                                                                        )
)


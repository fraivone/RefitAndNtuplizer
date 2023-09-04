import FWCore.ParameterSet.Config as cms
from RecoMuon.TrackingTools.MuonServiceProxy_cff import MuonServiceProxy

MuonRefitProducer = cms.EDProducer("MuonRefit",
                               tracks=cms.untracked.InputTag("muons"),
                               francesco=cms.string("Malavitoso"),
                               verbose=cms.bool(True),
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
                                                       )

)
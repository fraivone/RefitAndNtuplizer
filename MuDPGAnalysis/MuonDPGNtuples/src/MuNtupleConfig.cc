/** \class MuNtupleConfig MuNtupleConfig.cc MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleConfig.cc
 *  
 * Helper class to handle :
 * - configuration parameters for edm::ParameterSet
 * - DB information from edm::EventSetup
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleConfig.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Run.h"

#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "CalibMuon/DTDigiSync/interface/DTTTrigSyncFactory.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "TString.h"
#include "TRegexp.h"

MuNtupleConfig::MuNtupleConfig(const edm::ParameterSet & config,
			       edm::ConsumesCollector && collector) 
{ 
  m_trackingGeomToken = collector.esConsumes<>();
  m_ttbToken = collector.esConsumes<>(edm::ESInputTag("", "TransientTrackBuilder"));

  m_gemGeomToken = collector.esConsumes<edm::Transition::BeginRun>();
  m_cscGeomToken = collector.esConsumes<edm::Transition::BeginRun>();

  edm::InputTag none = edm::InputTag("none");
  
  m_inputTags["ph1DtDigiTag"] = config.getUntrackedParameter<edm::InputTag>("ph1DtDigiTag", none);
  m_inputTags["ph2DtDigiTag"] = config.getUntrackedParameter<edm::InputTag>("ph2DtDigiTag", none);

  m_inputTags["ph1DtSegmentTag"] = config.getUntrackedParameter<edm::InputTag>("ph1DtSegmentTag", none);
  m_inputTags["ph2DtSegmentTag"] = config.getUntrackedParameter<edm::InputTag>("ph2DtSegmentTag", none);

 // if (m_inputTags["ph1DtSegmentTag"].label() != "none")
  //   m_dtSyncs[PhaseTag::PH1] = DTTTrigSyncFactory::get()->create(config.getUntrackedParameter<std::string>("ph1DTtTrigMode"),
  //   							 config.getUntrackedParameter<edm::ParameterSet>("ph1DTtTrigModeConfig"));

  // if (m_inputTags["ph2DtSegmentTag"].label() != "none")
  //   m_dtSyncs[PhaseTag::PH2] = DTTTrigSyncFactory::get()->create(config.getUntrackedParameter<std::string>("ph2DTtTrigMode"),
  //   							 config.getUntrackedParameter<edm::ParameterSet>("ph2DTtTrigModeConfig"));

  m_inputTags["gemDigiTag"] = config.getUntrackedParameter<edm::InputTag>("gemDigiTag", none);
  
  m_inputTags["gemRecHitTag"] = config.getUntrackedParameter<edm::InputTag>("gemRecHitTag", none);

  m_inputTags["gemOHStatusTag"] = config.getUntrackedParameter<edm::InputTag>("gemOHStatusTag", none);

  m_inputTags["gemAMCStatusTag"] = config.getUntrackedParameter<edm::InputTag>("gemAMCStatusTag", none);

  m_inputTags["gemSegmentTag"] = config.getUntrackedParameter<edm::InputTag>("gemSegmentTag",none);

  m_inputTags["cscSegmentTag"] = config.getUntrackedParameter<edm::InputTag>("cscSegmentTag",none);

  m_inputTags["muonTag"] = config.getUntrackedParameter<edm::InputTag>("muonTag",none);
  
  m_inputTags["standalonemuonTag"] = config.getUntrackedParameter<edm::InputTag>("standalonemuonTag",none);
  
  m_inputTags["tcdsTag"] = config.getUntrackedParameter<edm::InputTag>("tcdsTag",none);

  m_inputTags["primaryVerticesTag"] = config.getUntrackedParameter<edm::InputTag>("primaryVerticesTag",none);

  m_inputTags["gemSimHitTag"] = config.getUntrackedParameter<edm::InputTag>("gemSimHitTag",none);
  
  m_inputTags["muonSimTag"] = config.getUntrackedParameter<edm::InputTag>("muonSimTag",none);
  
  m_inputTags["genParticlesTag"] = config.getUntrackedParameter<edm::InputTag>("genParticlesTag",none);

  m_muonSP = std::make_unique<MuonServiceProxy>(config.getParameter<edm::ParameterSet>("ServiceParameters"), std::move(collector));

  // MuonRefit
  edm::ParameterSet muonTransformerParam = config.getParameter<edm::ParameterSet>("MuonTransformer");
  theMuonTransformer = std::make_unique<MuonTransformer>(muonTransformerParam,collector);

}

void MuNtupleConfig::getES(const edm::EventSetup & environment) 
{
  m_muonSP->update(environment);
  m_trackingGeometry = environment.getHandle(m_trackingGeomToken);
  m_transientTrackBuilder = environment.getHandle(m_ttbToken);
  theMuonTransformer->setServices(environment);
}

void MuNtupleConfig::getES(const edm::Run &run, const edm::EventSetup & environment) 
{
 
  // if (m_inputTags["ph1DtSegmentTag"].label() != "none")
  //   m_dtSyncs[PhaseTag::PH1]->setES(environment);

  // if (m_inputTags["ph2DtSegmentTag"].label() != "none")
  //   m_dtSyncs[PhaseTag::PH2]->setES(environment);

  m_cscGeometry = environment.getHandle(m_cscGeomToken);
  m_gemGeometry = environment.getHandle(m_gemGeomToken);

  

}



/** \class MuNtupleProducer MuNtupleProducer.cc MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleProducer.cc
 *
 * Steering class: the edm::EDAnalyzer for MuDPGNtuple prdouction
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "MuDPGAnalysis/MuonDPGNtuples/plugins/MuNtupleProducer.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/MuonReco/interface/MuonFwd.h"
//#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleEventFiller.h"
// #include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleDTDigiFiller.h"
// #include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleDTSegmentFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMDigiFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMRecHitFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMOHStatusFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMAMCStatusFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMSimHitFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMSegmentFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMMuonFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMStandAloneMuonFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMRefitMuonFiller.h"

#include <iostream>

MuNtupleProducer::MuNtupleProducer(const edm::ParameterSet &config)
{

  usesResource("TFileService");
  edm::Service<TFileService> fileService;
  bool isMC = static_cast<bool>(config.getParameter<bool>("isMC"));
  bool storeOHStatus = static_cast<bool>(config.getParameter<bool>("storeOHStatus"));
  bool storeAMCStatus = static_cast<bool>(config.getParameter<bool>("storeAMCStatus"));
  bool RunOnSTA = static_cast<bool>(config.getParameter<bool>("STA"));
  float displacement = static_cast<double>(config.getParameter<double>("displacement"));

  m_tree = std::shared_ptr<TTree>(fileService->make<TTree>("MuDPGTree", "Mu DPG Tree"));
  m_config = std::make_shared<MuNtupleConfig>(MuNtupleConfig(config, consumesCollector()));

  m_fillers.push_back(std::make_unique<MuNtupleEventFiller>(consumesCollector(), m_config, m_tree, "event"));


  if (false)
      m_fillers.push_back(std::make_unique<MuNtupleGEMDigiFiller>(consumesCollector(), m_config, m_tree, "gemDigi"));
  if (storeOHStatus)
    m_fillers.push_back(std::make_unique<MuNtupleGEMOHStatusFiller>(consumesCollector(), m_config, m_tree, "gemOHStatus"));
  if (storeAMCStatus)
    m_fillers.push_back(std::make_unique<MuNtupleGEMAMCStatusFiller>(consumesCollector(), m_config, m_tree, "gemAMCStatus"));

  m_fillers.push_back(std::make_unique<MuNtupleGEMRecHitFiller>(consumesCollector(), m_config, m_tree, "gemRecHit"));

  // m_fillers.push_back(std::make_unique<MuNtupleGEMSegmentFiller>(consumesCollector(), m_config, m_tree, "gemSegment"));

  if (RunOnSTA)
    m_fillers.push_back(std::make_unique<MuNtupleGEMStandAloneMuonFiller>(consumesCollector(), m_config, m_tree, "mu",displacement));
  else
    m_fillers.push_back(std::make_unique<MuNtupleGEMRefitMuonFiller>(consumesCollector(), m_config, m_tree, "mu_refit",displacement));
  
  

  if (isMC)
    m_fillers.push_back(std::make_unique<MuNtupleGEMSimHitFiller>(consumesCollector(), m_config, m_tree, "gemSimHit"));
}

void MuNtupleProducer::beginJob()
{

  for (const auto &filler : m_fillers)
  {
    filler->initialize();
    filler->clear();
  }
}

void MuNtupleProducer::beginRun(const edm::Run &run, const edm::EventSetup &environment)
{

  m_config->getES(run, environment);
}

void MuNtupleProducer::endJob()
{

  m_tree->GetCurrentFile()->Write();
}

void MuNtupleProducer::analyze(const edm::Event &ev, const edm::EventSetup &environment)
{

  m_config->getES(environment);

  for (const auto &filler : m_fillers)
  {
    filler->fill(ev);
  }

  m_tree->Fill();
}

// define this as a plug-in
DEFINE_FWK_MODULE(MuNtupleProducer);

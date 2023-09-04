#ifndef MuNtuple_MuNtupleProducer_h
#define MuNtuple_MuNtupleProducer_h

/** \class MuNtupleProducer MuNtupleProducer.h MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleProducer.h
 *  
 * Steering class: the edm::EDAnalyzer for MuDPGNtuple prdouction
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleBaseFiller.h"
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleConfig.h"

#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"

//#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"

#include <memory>

class MuNtupleProducer : public edm::one::EDAnalyzer<edm::one::SharedResources,edm::one::WatchRuns>
{
 public:

  /// Constructor
  MuNtupleProducer(const edm::ParameterSet &);
  
  /// Fill ntuples event by event
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;

  /// Configure event setup
  virtual void beginRun(const edm::Run&, const edm::EventSetup&) override;

  /// Empty, needed by interface
  virtual void endRun(const edm::Run&, const edm::EventSetup&) override { };

  /// General "una tantum" configuration
  virtual void beginJob() override;

  /// Write ntuples to file
  virtual void endJob() override;
  
 private:
  
  /// Pointer to the TTree
  std::shared_ptr<TTree> m_tree;

  /// Ponter to the configuration 
  std::shared_ptr<MuNtupleConfig> m_config;

  /// The container with all the fillers
  std::vector<std::unique_ptr<MuNtupleBaseFiller>> m_fillers;
  
};

#endif

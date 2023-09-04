#ifndef MuNtuple_MuNtupleGEMStandAloneMuonFiller_h
#define MuNtuple_MuNtupleGEMStandAloneMuonFiller_h

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleBaseFiller.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonIsolation.h"
#include "DataFormats/MuonReco/interface/MuonPFIsolation.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/GEMRecHit/interface/GEMSegment.h"
#include "DataFormats/CSCRecHit/interface/CSCSegment.h"
#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"

#include <vector>
#include <fstream>
#include "TClonesArray.h"
#include "TVectorF.h"
#include "TFile.h"

class MuNtupleGEMStandAloneMuonFiller : public MuNtupleBaseFiller
{

 public:

  /// Constructor
  MuNtupleGEMStandAloneMuonFiller(edm::ConsumesCollector && collector,
		     const std::shared_ptr<MuNtupleConfig> config, 
		     std::shared_ptr<TTree> tree, 
             const std::string & label,
             float displacement);
  
  ///Destructor
  virtual ~MuNtupleGEMStandAloneMuonFiller();
  
  /// Intialize function : setup tree branches etc ... 
  virtual void initialize() final;
  
  /// Clear branches before event filling 
  virtual void clear() final;
 
  virtual void fill(const edm::Event & ev) final;

  bool verbose = false;
  
 private:

  edm::EDGetTokenT<std::vector<reco::Track>>  m_muToken;
  

  TVectorF m_nullVecF;

  std::vector<float> m_pt;        // STAmuon pT [GeV/c]
  std::vector<float> m_normChi2;  // STAmuon normalizedChi2
  std::vector<float> m_phi;       // STAmuon phi [rad]
  std::vector<float> m_eta;       // STAmuon eta
  std::vector<short> m_charge;    // STAmuon charge

  std::vector<bool>  m_isGEM;
  std::vector<bool>  m_isCSC;
  std::vector<bool>  m_isDT;
  std::vector<bool>  m_isRPC;
  std::vector<bool> m_isME11;
  std::vector<bool> m_isME21;
  std::vector<int> m_STA_nHits;
  std::vector<int> m_nME11hits;
  std::vector<int> m_nME1hits;
  std::vector<int> m_nME2hits;
  std::vector<int> m_nME3hits;
  std::vector<int> m_nME4hits;

  std::vector<bool> m_propagated_isGEM;
  std::vector<bool> m_propagated_isCSC;
  std::vector<bool> m_propagated_isDT;
  std::vector<bool> m_propagated_isRPC;
  std::vector<bool> m_propagated_isME11;
  std::vector<bool> m_propagated_isME21;
  std::vector<int> m_propagated_STA_nHits;
  std::vector<int> m_propagated_nME11hits;
  std::vector<int> m_propagated_nME1hits;
  std::vector<int> m_propagated_nME2hits;
  std::vector<int> m_propagated_nME3hits;
  std::vector<int> m_propagated_nME4hits;

  std::vector<int> m_propagated_region;
  std::vector<int> m_propagated_station;
  std::vector<int> m_propagated_layer;
  std::vector<int> m_propagated_chamber;
  std::vector<int> m_propagated_etaP;

  std::vector<float> m_propagated_pt;
  std::vector<float> m_propagated_phi;
  std::vector<float> m_propagated_eta;
  std::vector<float> m_propagated_charge;
  std::vector<float> m_propagated_TrackNormChi2;

  std::vector<float> m_propagatedLoc_x;
  std::vector<float> m_propagatedLoc_y;
  std::vector<float> m_propagatedLoc_z;
  std::vector<float> m_propagatedLoc_r;
  std::vector<float> m_propagatedLoc_phi;
  std::vector<float> m_propagatedLoc_dirX;
  std::vector<float> m_propagatedLoc_dirY;
  std::vector<float> m_propagatedLoc_dirZ;
  std::vector<float> m_propagatedLoc_errX;
  std::vector<float> m_propagatedLoc_errY;

  std::vector<float> m_propagatedGlb_x;
  std::vector<float> m_propagatedGlb_y;
  std::vector<float> m_propagatedGlb_z;
  std::vector<float> m_propagatedGlb_r;
  std::vector<float> m_propagatedGlb_phi;
  std::vector<float> m_propagatedGlb_errX;
  std::vector<float> m_propagatedGlb_errY;
  std::vector<float> m_propagatedGlb_phierr;
  std::vector<float> m_propagatedGlb_rerr;

  std::vector<float> m_propagated_EtaPartition_centerX;
  std::vector<float> m_propagated_EtaPartition_centerY;
  std::vector<float> m_propagated_EtaPartition_phiMax;
  std::vector<float> m_propagated_EtaPartition_phiMin;
  std::vector<float> m_propagated_EtaPartition_rMax;
  std::vector<float> m_propagated_EtaPartition_rMin;

  std::vector<float> m_propagated_Innermost_x;
  std::vector<float> m_propagated_Innermost_y;
  std::vector<float> m_propagated_Innermost_z;

  std::vector<float> m_propagated_Outermost_x;
  std::vector<float> m_propagated_Outermost_y;
  std::vector<float> m_propagated_Outermost_z;

  float m_displacement;
  const TrajectoryStateOnSurface dest_state;

  int nTrackHits = 0;
  int track_region = 0;
  
  unsigned int m_nMuons; 
  unsigned int nME1_hits = 0;
  unsigned int nME11_hits = 0;
  unsigned int nME2_hits = 0;
  unsigned int nME3_hits = 0;
  unsigned int nME4_hits = 0;
  
  bool isCSC = false;
  bool isDT = false;
  bool isRPC = false;
  bool isGEM = false;
  bool isME11 = false;
  bool isME21 = false;
  
};


#endif

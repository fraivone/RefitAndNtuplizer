#ifndef MuNtuple_MuNtupleGEMSimHitFiller_h
#define MuNtuple_MuNtupleGEMSimHitFiller_h

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleBaseFiller.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"

#include "DataFormats/PatCandidates/interface/Muon.h"

#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include <vector>

class MuNtupleGEMSimHitFiller : public MuNtupleBaseFiller
{

 public:

  //Constructor
  MuNtupleGEMSimHitFiller(edm::ConsumesCollector && collector,
			  const std::shared_ptr<MuNtupleConfig> config,
			  std::shared_ptr<TTree> tree, const std::string & label);


  //Destructor
  virtual ~MuNtupleGEMSimHitFiller();

  virtual void initialize() final; 
  
  virtual void clear() final;

  virtual void fill(const edm::Event & ev) final;

 private:
  
  edm::EDGetTokenT<edm::PSimHitContainer> m_simhit_token_;
  edm::ESGetToken<GEMGeometry, MuonGeometryRecord> geomToken_;
  //edm::EDGetTokenT<edm::View<pat::Muon> > muons_token_;
  edm::EDGetTokenT<edm::View<reco::GenParticle> > genParticles_token_;

  std::vector<float>  m_simhit_TOF;
  std::vector<float>  m_simhit_energyLoss;
  std::vector<int>  m_simhit_particleType;
  std::vector<int>  m_simhit_detUnitId;
  std::vector<int>  m_simhit_trackId;
  std::vector<unsigned short> m_simhit_processType;

  std::vector<int>  m_simhit_eventId;
  std::vector<int>  m_simhit_bunchCrossing;

  std::vector<float>  m_simhit_loc_r;
  std::vector<float>  m_simhit_loc_phi;
  std::vector<float>  m_simhit_loc_x;
  std::vector<float>  m_simhit_loc_y;
  std::vector<float>  m_simhit_loc_z;
  
  std::vector<float>  m_simhit_g_r;
  std::vector<float>  m_simhit_g_x;
  std::vector<float>  m_simhit_g_y;
  std::vector<float>  m_simhit_g_z;
  std::vector<float>  m_simhit_g_phi;

  /*std::vector<int> m_musim_pdgId;
  std::vector<int> m_musim_motherPdgId;
  std::vector<int> m_musim_simFlavour;
  std::vector<int> m_musim_simType;
  std::vector<int> m_musim_simBX;*/

  std::vector<int> genParticle_PdgId;
  std::vector<float> genParticle_Pt;
  std::vector<float> genParticle_Eta;
  std::vector<float> genParticle_Phi;
  std::vector<float> genParticle_vx;
  std::vector<float> genParticle_vy;
  std::vector<float> genParticle_vz;
  std::vector<int> genParticle_MotherPdgId;
  std::vector<int> genParticle_GrandMotherPdgId;

};

#endif

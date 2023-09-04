#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMSimHitFiller.h"

#include "FWCore/Framework/interface/Event.h"
// #include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"

MuNtupleGEMSimHitFiller::MuNtupleGEMSimHitFiller(edm::ConsumesCollector && collector,
						 const std::shared_ptr<MuNtupleConfig> config,
						 std::shared_ptr<TTree> tree, const std::string & label) :

  MuNtupleBaseFiller(config, tree, label)
{

  /*edm::InputTag &muSimTag = m_config->m_inputTags["muonSimTag"];
    if (muSimTag.label() != "none") muons_token_ = collector.consumes<edm::View<pat::Muon> >(muSimTag);*/

  edm::InputTag &genParticlesTag = m_config->m_inputTags["genParticlesTag"];
  if (genParticlesTag.label() != "none") genParticles_token_ = collector.consumes<edm::View<reco::GenParticle> >(genParticlesTag);

  edm::InputTag &iTag = m_config->m_inputTags["gemSimHitTag"];
  if (iTag.label() != "none") m_simhit_token_ = collector.consumes<edm::PSimHitContainer>(iTag);
  
}

MuNtupleGEMSimHitFiller::~MuNtupleGEMSimHitFiller()
{

};

void MuNtupleGEMSimHitFiller::initialize()
{

  m_tree->Branch((m_label + "_particleType").c_str(), &m_simhit_particleType);
  m_tree->Branch((m_label + "_energyLoss").c_str(), &m_simhit_energyLoss);
  m_tree->Branch((m_label + "_TOF").c_str(), &m_simhit_TOF);
  m_tree->Branch((m_label + "_detUnitId").c_str(), &m_simhit_detUnitId);
  m_tree->Branch((m_label + "_trackId").c_str(), &m_simhit_trackId);
  m_tree->Branch((m_label + "_eventId").c_str(), &m_simhit_eventId);
  m_tree->Branch((m_label + "_processType").c_str(), &m_simhit_processType);
  m_tree->Branch((m_label + "_bunchCrossing").c_str(), &m_simhit_bunchCrossing);

  m_tree->Branch((m_label + "_loc_r").c_str(), &m_simhit_loc_r);
  m_tree->Branch((m_label + "_loc_phi").c_str(), &m_simhit_loc_phi);
  m_tree->Branch((m_label + "_loc_x").c_str(), &m_simhit_loc_x);
  m_tree->Branch((m_label + "_loc_y").c_str(), &m_simhit_loc_y);
  m_tree->Branch((m_label + "_loc_z").c_str(), &m_simhit_loc_z);

  m_tree->Branch((m_label + "_g_r").c_str(), &m_simhit_g_r);
  m_tree->Branch((m_label + "_g_phi").c_str(), &m_simhit_g_phi);
  m_tree->Branch((m_label + "_g_x").c_str(), &m_simhit_g_x);
  m_tree->Branch((m_label + "_g_y").c_str(), &m_simhit_g_y);
  m_tree->Branch((m_label + "_g_z").c_str(), &m_simhit_g_z);
  
  /*m_tree->Branch((m_label + "_simPdgId").c_str(), &m_musim_pdgId);
  m_tree->Branch((m_label + "_simMotherPdgId").c_str(), &m_musim_motherPdgId);
  m_tree->Branch((m_label + "_simFlavour").c_str(), &m_musim_simFlavour);
  m_tree->Branch((m_label + "_simType").c_str(), &m_musim_simType);
  m_tree->Branch((m_label + "_simBX").c_str(), &m_musim_simBX);*/

  m_tree->Branch(("genParticle_PdgId"), &genParticle_PdgId);
  m_tree->Branch(("genParticle_Pt"), &genParticle_Pt);
  m_tree->Branch(("genParticle_Eta"), &genParticle_Eta);
  m_tree->Branch(("genParticle_Phi"), &genParticle_Phi);
  m_tree->Branch(("genParticle_vx"), &genParticle_vx);
  m_tree->Branch(("genParticle_vy"), &genParticle_vy);
  m_tree->Branch(("genParticle_vz"), &genParticle_vz);
  m_tree->Branch(("genParticle_MotherPdgId"), &genParticle_MotherPdgId);
  m_tree->Branch(("genParticle_GrandMotherPdgId"), &genParticle_GrandMotherPdgId);
  
};

void MuNtupleGEMSimHitFiller::clear()
{

  m_simhit_particleType.clear();
  m_simhit_energyLoss.clear();
  m_simhit_TOF.clear();
  m_simhit_detUnitId.clear();
  m_simhit_trackId.clear();
  m_simhit_eventId.clear();
  m_simhit_processType.clear();
  m_simhit_bunchCrossing.clear();

  m_simhit_loc_r.clear();
  m_simhit_loc_phi.clear();
  m_simhit_loc_x.clear();
  m_simhit_loc_y.clear();
  m_simhit_loc_z.clear();
 
  m_simhit_g_r.clear();
  m_simhit_g_phi.clear();
  m_simhit_g_x.clear();
  m_simhit_g_y.clear();
  m_simhit_g_z.clear();

  /*m_musim_pdgId.clear();
  m_musim_motherPdgId.clear();
  m_musim_simFlavour.clear();
  m_musim_simType.clear();
  m_musim_simBX.clear();*/

  genParticle_PdgId.clear();
  genParticle_Pt.clear();
  genParticle_Eta.clear();
  genParticle_Phi.clear();
  genParticle_vx.clear();
  genParticle_vy.clear();
  genParticle_vz.clear();
  genParticle_MotherPdgId.clear();
  genParticle_GrandMotherPdgId.clear();

};

void MuNtupleGEMSimHitFiller::fill(const edm::Event & ev)
{

  clear();
  
  //edm::Handle<edm::View<pat::Muon>> muons;
  //ev.getByToken(muons_token_,muons);

  edm::Handle<edm::PSimHitContainer> GE11Hits;
  //ev.getByLabel(edm::InputTag("g4SimHits","MuonGEMHits"),GE11Hits);
  ev.getByToken(m_simhit_token_,GE11Hits);
  
  edm::Handle< edm::View<reco::GenParticle> > genParticles;
  ev.getByToken(genParticles_token_, genParticles);

  const auto gem = m_config->m_gemGeometry;
  
  for(edm::PSimHitContainer::const_iterator simHit = GE11Hits->begin(); simHit != GE11Hits->end(); ++simHit)
    {
      m_simhit_particleType.push_back(simHit->particleType());
      m_simhit_energyLoss.push_back(simHit->energyLoss());
      m_simhit_TOF.push_back(simHit->timeOfFlight());
      m_simhit_detUnitId.push_back(simHit->detUnitId());
      m_simhit_trackId.push_back(simHit->trackId());
      m_simhit_eventId.push_back(simHit->eventId().event());
      m_simhit_processType.push_back(simHit->processType());
      m_simhit_bunchCrossing.push_back(simHit->eventId().bunchCrossing());

      GEMDetId gem_id = GEMDetId(simHit->detUnitId());
      
      LocalPoint localPosition = simHit->entryPoint();

      const BoundPlane& surface = gem->idToDet(gem_id)->surface();
      GlobalPoint&& globalPosition = surface.toGlobal(localPosition);

      m_simhit_loc_r.push_back(localPosition.perp());
      m_simhit_loc_phi.push_back(localPosition.phi());
      m_simhit_loc_x.push_back(localPosition.x());
      m_simhit_loc_y.push_back(localPosition.y());
      m_simhit_loc_z.push_back(localPosition.z());

      m_simhit_g_r.push_back(globalPosition.perp());
      m_simhit_g_phi.push_back(globalPosition.phi());
      m_simhit_g_x.push_back(globalPosition.x());
      m_simhit_g_y.push_back(globalPosition.y());
      m_simhit_g_z.push_back(globalPosition.z());
    }

  /*for(edm::View<pat::Muon>::const_iterator mu = muons->begin(); mu != muons->end(); ++mu)
    {
      
      m_musim_pdgId.push_back(mu->simPdgId());
      m_musim_motherPdgId.push_back(mu->simMotherPdgId());
      m_musim_simFlavour.push_back(mu->simFlavour());
      m_musim_simType.push_back(mu->simType());
      m_musim_simBX.push_back(mu->simBX());
      
      }*/

  for(edm::View<reco::GenParticle>::const_iterator gp = genParticles->begin(); gp != genParticles->end(); ++gp)
    {

	  genParticle_PdgId.push_back(gp->pdgId());
	  genParticle_Pt.push_back(gp->pt());
	  genParticle_Eta.push_back(gp->eta());
	  genParticle_Phi.push_back(gp->phi());
	  genParticle_vx.push_back(gp->vx());
	  genParticle_vy.push_back(gp->vy());
	  genParticle_vz.push_back(gp->vz());
	  if (gp->numberOfMothers())
	    {
	      genParticle_MotherPdgId.push_back(gp->mother(0)->pdgId());
	      if(gp->mother(0)->mother(0))
		{
		  genParticle_GrandMotherPdgId.push_back(gp->mother(0)->mother(0)->pdgId());
		}
	      else
		{
		  genParticle_GrandMotherPdgId.push_back(-99);
		}
	    }
	  else
	    {
	      genParticle_MotherPdgId.push_back(-99);
		genParticle_GrandMotherPdgId.push_back(-99);
	    }
    }
};

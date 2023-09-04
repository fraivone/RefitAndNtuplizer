
#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMStandAloneMuonFiller.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
// #include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Ref.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonReco/interface/MuonChamberMatch.h"
#include "DataFormats/MuonReco/interface/MuonSegmentMatch.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"
#include "DataFormats/GEMRecHit/interface/GEMSegment.h"
#include "DataFormats/GEMRecHit/interface/GEMSegmentCollection.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/CSCRecHit/interface/CSCSegment.h"
#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"

#include "DataFormats/GeometryCommonDetAlgo/interface/ErrorFrameTransformer.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixStateInfo.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

#include "TVectorF.h"
#include "TFile.h"

MuNtupleGEMStandAloneMuonFiller::MuNtupleGEMStandAloneMuonFiller(edm::ConsumesCollector && collector,
					     const std::shared_ptr<MuNtupleConfig> config, 
					     std::shared_ptr<TTree> tree, 
               const std::string & label,
               float displacement) : 
MuNtupleBaseFiller(config, tree, label), m_nullVecF()
{

  edm::InputTag & muonTag = m_config->m_inputTags["standalonemuonTag"];
  if (muonTag.label() != "none") m_muToken = collector.consumes<std::vector<reco::Track>>(muonTag);
  m_displacement = displacement;
  
}

MuNtupleGEMStandAloneMuonFiller::~MuNtupleGEMStandAloneMuonFiller() 
{ 

}

void MuNtupleGEMStandAloneMuonFiller::initialize()
{

  // STA Track info
  m_tree->Branch((m_label + "_nMuons").c_str(), &m_nMuons);
  m_tree->Branch((m_label + "_pt").c_str(), &m_pt);
  m_tree->Branch((m_label + "_phi").c_str(), &m_phi);
  m_tree->Branch((m_label + "_eta").c_str(), &m_eta);
  m_tree->Branch((m_label + "_charge").c_str(), &m_charge);
  m_tree->Branch((m_label + "_normChi2").c_str(), &m_normChi2);
  // STA Track Hits info
  m_tree->Branch((m_label + "_isGEM").c_str(), &m_isGEM);
  m_tree->Branch((m_label + "_isCSC").c_str(), &m_isCSC);
  m_tree->Branch((m_label + "_isME11").c_str(), &m_isME11);
  m_tree->Branch((m_label + "_isME21").c_str(), &m_isME21);
  m_tree->Branch((m_label + "_nSTAHits").c_str(), &m_STA_nHits);
  m_tree->Branch((m_label + "_isRPC").c_str(), &m_isRPC);
  m_tree->Branch((m_label + "_isDT").c_str(), &m_isDT);
  m_tree->Branch((m_label + "_nME11hits").c_str(), &m_nME11hits);
  m_tree->Branch((m_label + "_nME1hits").c_str(), &m_nME1hits);
  m_tree->Branch((m_label + "_nME2hits").c_str(), &m_nME2hits);
  m_tree->Branch((m_label + "_nME3hits").c_str(), &m_nME3hits);
  m_tree->Branch((m_label + "_nME4hits").c_str(), &m_nME4hits);

  //Propagated tracks info
  m_tree->Branch((m_label + "_propagated_pt").c_str(), &m_propagated_pt);
  m_tree->Branch((m_label + "_propagated_phi").c_str(), &m_propagated_phi);
  m_tree->Branch((m_label + "_propagated_eta").c_str(), &m_propagated_eta);
  m_tree->Branch((m_label + "_propagated_charge").c_str(), &m_propagated_charge);
  m_tree->Branch((m_label + "_propagated_TrackNormChi2").c_str(), &m_propagated_TrackNormChi2);
  m_tree->Branch((m_label + "_propagated_nSTAHits").c_str(), &m_propagated_STA_nHits);

  m_tree->Branch((m_label + "_propagated_isCSC").c_str(), &m_propagated_isCSC);
  m_tree->Branch((m_label + "_propagated_isDT").c_str(), &m_propagated_isDT);
  m_tree->Branch((m_label + "_propagated_isGEM").c_str(), &m_propagated_isGEM);
  m_tree->Branch((m_label + "_propagated_isRPC").c_str(), &m_propagated_isRPC);
  m_tree->Branch((m_label + "_propagated_isME11").c_str(), &m_propagated_isME11);
  m_tree->Branch((m_label + "_propagated_isME21").c_str(), &m_propagated_isME21);
  m_tree->Branch((m_label + "_propagated_nME11hits").c_str(), &m_propagated_nME11hits);
  m_tree->Branch((m_label + "_propagated_nME1hits").c_str(), &m_propagated_nME1hits);
  m_tree->Branch((m_label + "_propagated_nME2hits").c_str(), &m_propagated_nME2hits);
  m_tree->Branch((m_label + "_propagated_nME3hits").c_str(), &m_propagated_nME3hits);
  m_tree->Branch((m_label + "_propagated_nME4hits").c_str(), &m_propagated_nME4hits);

  m_tree->Branch((m_label + "_propagated_region").c_str(), &m_propagated_region);
  m_tree->Branch((m_label + "_propagated_station").c_str(), &m_propagated_station);
  m_tree->Branch((m_label + "_propagated_layer").c_str(), &m_propagated_layer);
  m_tree->Branch((m_label + "_propagated_chamber").c_str(), &m_propagated_chamber);
  m_tree->Branch((m_label + "_propagated_etaP").c_str(), &m_propagated_etaP);

  m_tree->Branch((m_label + "_propagatedLoc_x").c_str(), &m_propagatedLoc_x);
  m_tree->Branch((m_label + "_propagatedLoc_y").c_str(), &m_propagatedLoc_y);
  m_tree->Branch((m_label + "_propagatedLoc_z").c_str(), &m_propagatedLoc_z);
  m_tree->Branch((m_label + "_propagatedLoc_r").c_str(), &m_propagatedLoc_r);
  m_tree->Branch((m_label + "_propagatedLoc_phi").c_str(), &m_propagatedLoc_phi);
  m_tree->Branch((m_label + "_propagatedLoc_errX").c_str(), &m_propagatedLoc_errX);
  m_tree->Branch((m_label + "_propagatedLoc_errY").c_str(), &m_propagatedLoc_errY);
  m_tree->Branch((m_label + "_propagatedLoc_dirX").c_str(), &m_propagatedLoc_dirX);
  m_tree->Branch((m_label + "_propagatedLoc_dirY").c_str(), &m_propagatedLoc_dirY);
  m_tree->Branch((m_label + "_propagatedLoc_dirZ").c_str(), &m_propagatedLoc_dirZ);

  m_tree->Branch((m_label + "_propagatedGlb_x").c_str(), &m_propagatedGlb_x);
  m_tree->Branch((m_label + "_propagatedGlb_y").c_str(), &m_propagatedGlb_y);
  m_tree->Branch((m_label + "_propagatedGlb_z").c_str(), &m_propagatedGlb_z);
  m_tree->Branch((m_label + "_propagatedGlb_r").c_str(), &m_propagatedGlb_r);
  m_tree->Branch((m_label + "_propagatedGlb_phi").c_str(), &m_propagatedGlb_phi);
  m_tree->Branch((m_label + "_propagatedGlb_errX").c_str(), &m_propagatedGlb_errX);
  m_tree->Branch((m_label + "_propagatedGlb_errY").c_str(), &m_propagatedGlb_errY);
  m_tree->Branch((m_label + "_propagatedGlb_errR").c_str(), &m_propagatedGlb_rerr);
  m_tree->Branch((m_label + "_propagatedGlb_errPhi").c_str(), &m_propagatedGlb_phierr);

  m_tree->Branch((m_label + "_propagated_EtaPartition_centerX").c_str(), &m_propagated_EtaPartition_centerX);
  m_tree->Branch((m_label + "_propagated_EtaPartition_centerY").c_str(), &m_propagated_EtaPartition_centerY);
  m_tree->Branch((m_label + "_propagated_EtaPartition_rMax").c_str(), &m_propagated_EtaPartition_rMax);
  m_tree->Branch((m_label + "_propagated_EtaPartition_rMin").c_str(), &m_propagated_EtaPartition_rMin);
  m_tree->Branch((m_label + "_propagated_EtaPartition_phiMax").c_str(), &m_propagated_EtaPartition_phiMax);
  m_tree->Branch((m_label + "_propagated_EtaPartition_phiMin").c_str(), &m_propagated_EtaPartition_phiMin);

  m_tree->Branch((m_label + "_propagated_Innermost_x").c_str(), &m_propagated_Innermost_x);
  m_tree->Branch((m_label + "_propagated_Innermost_y").c_str(), &m_propagated_Innermost_y);
  m_tree->Branch((m_label + "_propagated_Innermost_z").c_str(), &m_propagated_Innermost_z);
  m_tree->Branch((m_label + "_propagated_Outermost_x").c_str(), &m_propagated_Outermost_x);
  m_tree->Branch((m_label + "_propagated_Outermost_y").c_str(), &m_propagated_Outermost_y);
  m_tree->Branch((m_label + "_propagated_Outermost_z").c_str(), &m_propagated_Outermost_z);

  
  
}

void MuNtupleGEMStandAloneMuonFiller::clear()
{

  // Track info
  m_nMuons = 0;
  m_pt.clear();
  m_normChi2.clear();
  m_phi.clear();
  m_eta.clear();
  m_charge.clear();
  m_isGEM.clear();
  m_isCSC.clear();
  m_isDT.clear();
  m_isRPC.clear();
  m_isME11.clear();
  m_isME21.clear();
  m_STA_nHits.clear();
  m_nME11hits.clear();
  m_nME1hits.clear();
  m_nME2hits.clear();
  m_nME3hits.clear();
  m_nME4hits.clear();

  //Propagated tracks info
  m_propagated_pt.clear();
  m_propagated_phi.clear();
  m_propagated_eta.clear();
  m_propagated_charge.clear();
  m_propagated_TrackNormChi2.clear();
  m_propagated_STA_nHits.clear();

  m_propagated_isCSC.clear();
  m_propagated_isDT.clear();
  m_propagated_isGEM.clear();
  m_propagated_isRPC.clear();
  m_propagated_isME11.clear();
  m_propagated_isME21.clear();
  m_propagated_nME11hits.clear();
  m_propagated_nME1hits.clear();
  m_propagated_nME2hits.clear();
  m_propagated_nME3hits.clear();
  m_propagated_nME4hits.clear();

  m_propagated_region.clear();
  m_propagated_station.clear();
  m_propagated_layer.clear();
  m_propagated_chamber.clear();
  m_propagated_etaP.clear();

  m_propagatedLoc_x.clear();
  m_propagatedLoc_y.clear();
  m_propagatedLoc_z.clear();
  m_propagatedLoc_r.clear();
  m_propagatedLoc_phi.clear();
  m_propagatedLoc_errX.clear();
  m_propagatedLoc_errY.clear();
  m_propagatedLoc_dirX.clear();
  m_propagatedLoc_dirY.clear();
  m_propagatedLoc_dirZ.clear();

  m_propagatedGlb_x.clear();
  m_propagatedGlb_y.clear();
  m_propagatedGlb_z.clear();
  m_propagatedGlb_r.clear();
  m_propagatedGlb_phi.clear();
  m_propagatedGlb_errX.clear();
  m_propagatedGlb_errY.clear();
  m_propagatedGlb_rerr.clear();
  m_propagatedGlb_phierr.clear();

  m_propagated_EtaPartition_centerX.clear();
  m_propagated_EtaPartition_centerY.clear();
  m_propagated_EtaPartition_rMax.clear();
  m_propagated_EtaPartition_rMin.clear();
  m_propagated_EtaPartition_phiMax.clear();
  m_propagated_EtaPartition_phiMin.clear();

  m_propagated_Innermost_x.clear();
  m_propagated_Innermost_y.clear();
  m_propagated_Innermost_z.clear();
  m_propagated_Outermost_x.clear();
  m_propagated_Outermost_y.clear();
  m_propagated_Outermost_z.clear();
      
}

void MuNtupleGEMStandAloneMuonFiller::fill(const edm::Event & ev)
{

  clear();

  auto STAmuons = conditionalGet<std::vector<reco::Track>>(ev, m_muToken, "StandAloneMuonCollection");
   
  edm::ESHandle<Propagator>&& propagator_any = m_config->m_muonSP->propagator("SteppingHelixPropagatorAny");
  if (not propagator_any.isValid()) {
    std::cout<< "Any Propagator is invalid" << std::endl;
    return;
  }

    
  edm::ESHandle<GEMGeometry> gem = m_config->m_gemGeometry;
  if (not gem.isValid()) {
    std::cout << "GEMGeometry is invalid" << std::endl;
    return;
  }

  // edm::ESHandle<CSCGeometry> csc = m_config->m_cscGeometry;
  // if (not csc.isValid()) {
  //   std::cout << "CSCGeometry is invalid" << std::endl;
  //   return;
  // }

  edm::ESHandle<TransientTrackBuilder> transient_track_builder = m_config->m_transientTrackBuilder;
  if (not transient_track_builder.isValid()) {
    std::cout << "TransientTrack is invalid" << std::endl;
    return;
  }

  if (STAmuons.isValid()) {
    for (const auto & track : (*STAmuons)){
      m_nMuons++; // one more muon in the event
      track_region = track.outerZ() > 0 ? 1 : -1; // track belongs to region 1 or -1

      // default track details
      nTrackHits = 0;
      isCSC = false;
      isDT = false;
      isRPC = false;
      isGEM = false;
      isME11 = false;
      isME21 = false;
      nME11_hits = 0;
      nME1_hits = 0;
      nME2_hits = 0;
      nME3_hits = 0;
      nME4_hits = 0;
      // end default track details 
           
      const reco::HitPattern &p = track.hitPattern();
      // Loop on the hits of this track
      for (int i = 0; i < p.numberOfAllHits(reco::HitPattern::TRACK_HITS); i++) {
        uint32_t hit = p.getHitPattern(reco::HitPattern::TRACK_HITS, i);
        if (p.validHitFilter(hit) ){
          nTrackHits++;
          
          // Hit in the muon system
          if (p.getHitType(hit)==0){
            if (verbose) std::cout << "\t\tvalid hit found in the muon system "<< std::endl;
                       
            // CSC Hits
            if ( p.getSubStructure(hit) == 2 ){ 
              isCSC = true;
              int CSC_station = p.getMuonStation(hit);                
              int CSC_ring = reco::HitPattern::getCSCRing(hit);
              if(CSC_station == 1 && ((CSC_ring == 1) || (CSC_ring == 4)) ) {
                isME11 = true;
                nME11_hits++;
              }
              if(CSC_station == 2 && CSC_ring == 1) isME21 = true;
              if(CSC_station == 1) nME1_hits++;
              if(CSC_station == 2) nME2_hits++;
              if(CSC_station == 3) nME3_hits++;
              if(CSC_station == 4) nME4_hits++;
              } // end hit is CSC
            
            if ( p.getSubStructure(hit) == 1 ) isDT = true;
            if ( p.getSubStructure(hit) == 3 ) isRPC = true;
            if ( p.getSubStructure(hit) == 4 ) isGEM = true;
          } // end Hit in the muon system
        else if(verbose) std::cout << "valid hit NOT in the muon system "<< std::endl;
        }// Hit is valid
      } // end Loop on the hits of this track
      if (verbose) std::cout<<"\tMuon number "<<m_nMuons<<", in the region = "<< track_region <<", contains "<<nTrackHits<<" hits"<<std::endl;
      if (verbose) std::cout<<"\tisDT "<<isDT<<" isCSC "<< isCSC <<" isME21 "<< isME21 <<" isME11 "<<isME11<<" isRPC "<< isRPC << " isGEM" << isGEM <<std::endl;
      
      
      // Fill track info
      m_pt.push_back(track.pt());
      m_normChi2.push_back(track.normalizedChi2());
      m_phi.push_back(track.phi());
      m_eta.push_back(track.eta());
      m_charge.push_back(track.charge());
      m_isGEM.push_back(isGEM);
      m_isCSC.push_back(isCSC);
      m_isDT.push_back(isDT);
      m_isRPC.push_back(isRPC);
      m_isME11.push_back(isME11);
      m_isME21.push_back(isME21);
      m_STA_nHits.push_back(nTrackHits);
      m_nME11hits.push_back(nME11_hits);
      m_nME1hits.push_back(nME1_hits);
      m_nME2hits.push_back(nME2_hits);
      m_nME3hits.push_back(nME3_hits);
      m_nME4hits.push_back(nME4_hits);

      // Propagation
      const reco::TransientTrack&& transient_track = transient_track_builder->build(track);
      if (not transient_track.isValid()) {
		    std::cout<<"failed to build TransientTrack" << std::endl;
        continue;
		    }
      const auto&& start_state = transient_track.innermostMeasurementState();      
      for (const GEMRegion* gem_region : gem->regions()){
        if (gem_region->region() != track_region) continue; // don't propagate a track in the opposite endcap
        // GEMRegion gem_region = GEMRegion(region);
        for (const GEMStation* station : gem_region->stations()){
          for (const GEMSuperChamber* super_chamber : station->superChambers()){
            for (const GEMChamber* chamber : super_chamber->chambers()){
              for (const GEMEtaPartition* eta_partition : chamber->etaPartitions()){
              
                const BoundPlane& bound_plane = eta_partition->surface();
                // PROPAGATION ON ETAP SURFACE
                // The Z of the dest_state is fixed one the boundplane. x,y are actually evaluated by the propagator at that Z
                // const auto& dest_state = propagator_any->propagate(start_state,bound_plane);
                //END PROPAGATION ON ETAP SURFACE

                // PROPAGATION IN THE DRIFT GAP
                BoundPlane& etaPSur_translated_to_drift = const_cast<BoundPlane&>(bound_plane);

                int ch = eta_partition->id().chamber();
                int re = eta_partition->id().region();
                if (re == -1){
                  etaPSur_translated_to_drift.move(GlobalVector(0.,0.,m_displacement));
                  const auto& dest_state = propagator_any->propagate(start_state,etaPSur_translated_to_drift);
                  etaPSur_translated_to_drift.move(GlobalVector(0.,0.,-m_displacement));
                }
                else if (re == 1)  {
                  etaPSur_translated_to_drift.move(GlobalVector(0.,0.,-m_displacement));
                  const auto& dest_state = propagator_any->propagate(start_state,etaPSur_translated_to_drift);
                  etaPSur_translated_to_drift.move(GlobalVector(0.,0.,m_displacement));
                }
                else{
                  std::cout<<"Error region is neither +1 or -1"<<std::endl;
                  const auto& dest_state = propagator_any->propagate(start_state,etaPSur_translated_to_drift);
                }
                
                // if (ch % 2 == 0)
                //     {
                //         displacement = -0.55*re;
                //     }
                // if (ch % 2 == 1)
                //     {
                //         displacement = 0.55*re;
                //



                
                // END PROPAGATION IN THE DRIFT GAP
                

                if (not dest_state.isValid()){
                  if (verbose) std::cout << "failed to propagate" << std::endl;
                  continue;
                } // propagation destination is invalid

                const GlobalPoint&& dest_global_pos = dest_state.globalPosition();
                const LocalPoint&& local_point = eta_partition->toLocal(dest_global_pos);
                const LocalPoint local_point_2d(local_point.x(), local_point.y(), 0.0f);

                if (eta_partition->surface().bounds().inside(local_point_2d)) 
                {
                  const GEMDetId&& gem_id = eta_partition->id();
                  if (verbose) std::cout<<"STAMuon n"<<m_nMuons<<" propagated to region "<<gem_id.region()<<" station "<<gem_id.station()<<" chamber "<<gem_id.chamber()<<" layer "<<gem_id.layer()<<" etaP "<<gem_id.roll()<<std::endl;
                  // X,Y FROM QC8 Code
                  double xx = dest_state.curvilinearError().matrix()(3,3);
                  double yy = dest_state.curvilinearError().matrix()(4,4);
                  double xy = dest_state.curvilinearError().matrix()(4,3);
                  double dest_glob_error_x = sqrt(0.5*(xx+yy-sqrt((xx-yy)*(xx-yy)+4*xy*xy)));
                  double dest_glob_error_y = sqrt(0.5*(xx+yy+sqrt((xx-yy)*(xx-yy)+4*xy*xy)));
                  // R,Phi From https://github.com/cms-sw/cmssw/blob/f77e926a1e98b3d9f1144caf3b83cb3667e23786/DQMOffline/Muon/src/GEMEfficiencyAnalyzer.cc
                  const LocalPoint&& dest_local_pos = chamber->toLocal(dest_global_pos);
                  const LocalError&& dest_local_err = dest_state.localError().positionError();
                  const GlobalError& dest_global_err = ErrorFrameTransformer().transform(dest_local_err, eta_partition->surface());
                  const double dest_global_r_err = std::sqrt(dest_global_err.rerr(dest_global_pos));
                  const double dest_global_phi_err = std::sqrt(dest_global_err.phierr(dest_global_pos));
                  // Track info
                  m_propagated_pt.push_back(track.pt());
                  m_propagated_phi.push_back(track.phi());
                  m_propagated_eta.push_back(track.eta());
                  m_propagated_charge.push_back(track.charge());
                  m_propagated_TrackNormChi2.push_back(track.normalizedChi2());
                  m_propagated_isCSC.push_back(isCSC);
                  m_propagated_isDT.push_back(isDT);
                  m_propagated_isGEM.push_back(isGEM);
                  m_propagated_isRPC.push_back(isRPC);
                  m_propagated_isME11.push_back(isME11);
                  m_propagated_isME21.push_back(isME21);
                  m_propagated_nME11hits.push_back(nME11_hits);
                  m_propagated_nME1hits.push_back(nME1_hits);
                  m_propagated_nME2hits.push_back(nME2_hits);
                  m_propagated_nME3hits.push_back(nME3_hits);
                  m_propagated_nME4hits.push_back(nME4_hits);
                  m_propagated_STA_nHits.push_back(nTrackHits);
                  // propagation info
                  m_propagated_region.push_back(gem_id.region());
                  m_propagated_station.push_back(gem_id.station());
                  m_propagated_layer.push_back(gem_id.layer());
                  m_propagated_chamber.push_back(gem_id.chamber());
                  m_propagated_etaP.push_back(gem_id.roll());

                  m_propagatedGlb_x.push_back(dest_global_pos.x());
                  m_propagatedGlb_y.push_back(dest_global_pos.y());
                  m_propagatedGlb_z.push_back(dest_global_pos.z());
                  m_propagatedGlb_r.push_back(dest_global_pos.perp());
                  m_propagatedGlb_phi.push_back(dest_global_pos.phi());
                                                                  
                  m_propagatedLoc_x.push_back(dest_local_pos.x());
                  m_propagatedLoc_phi.push_back(dest_local_pos.phi());
                  m_propagatedLoc_r.push_back(dest_local_pos.perp());
                  m_propagatedLoc_y.push_back(dest_local_pos.y());
                  m_propagatedLoc_dirX.push_back(dest_state.localDirection().x());
                  m_propagatedLoc_dirY.push_back(dest_state.localDirection().y());
                  m_propagatedLoc_dirZ.push_back(dest_state.localDirection().z());
                                                                  
                  m_propagatedLoc_errX.push_back(dest_local_err.xx());
                  m_propagatedLoc_errY.push_back(dest_local_err.yy());
                  m_propagatedGlb_errX.push_back(dest_glob_error_x);
                  m_propagatedGlb_errY.push_back(dest_glob_error_y);
                  m_propagatedGlb_rerr.push_back(dest_global_r_err);
                  m_propagatedGlb_phierr.push_back(dest_global_phi_err);
                                                                  
                  m_propagated_Innermost_x.push_back(transient_track.innermostMeasurementState().globalPosition().x());
                  m_propagated_Innermost_y.push_back(transient_track.innermostMeasurementState().globalPosition().y());
                  m_propagated_Innermost_z.push_back(transient_track.innermostMeasurementState().globalPosition().z());
                  m_propagated_Outermost_x.push_back(transient_track.outermostMeasurementState().globalPosition().x());
                  m_propagated_Outermost_y.push_back(transient_track.outermostMeasurementState().globalPosition().y());
                  m_propagated_Outermost_z.push_back(transient_track.outermostMeasurementState().globalPosition().z());
                                                                  
                  m_propagated_EtaPartition_centerX.push_back(eta_partition->position().x());
                  m_propagated_EtaPartition_centerY.push_back(eta_partition->position().y());
                  m_propagated_EtaPartition_rMin.push_back(eta_partition->surface().rSpan().first);
                  m_propagated_EtaPartition_rMax.push_back(eta_partition->surface().rSpan().second);
                  m_propagated_EtaPartition_phiMin.push_back(eta_partition->surface().phiSpan().first);
                  m_propagated_EtaPartition_phiMax.push_back(eta_partition->surface().phiSpan().second);                                                        
                }
       
              }// end Loop on eta partitions
            }// end Loop on chambers
          } // end Loop on super chambers
        }// end Loop on GEM stations
      }// end Loop on GEM regions
      if (verbose) std::cout<<std::endl;
      
    }// end loop through tracks
    if (verbose) std::cout<<"Found "<<m_nMuons<<" STA tracks in this event\n"<<std::endl;
  } // Collection STA Muons is valid
}

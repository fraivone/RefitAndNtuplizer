#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleGEMSegmentFiller.h"

#include "FWCore/Framework/interface/Event.h"
// #include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "TrackingTools/GeomPropagators/interface/StraightLinePlaneCrossing.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"

#include "TClonesArray.h"

MuNtupleGEMSegmentFiller::MuNtupleGEMSegmentFiller(edm::ConsumesCollector && collector,
						 const std::shared_ptr<MuNtupleConfig> config,
						 std::shared_ptr<TTree> tree, const std::string & label) :
  MuNtupleBaseFiller(config, tree, label),  m_nullVecF()
{
  edm::InputTag & iTag = m_config->m_inputTags["gemSegmentTag"];
  if (iTag.label() != "none") m_gemSegmentToken = collector.consumes<GEMSegmentCollection>(iTag);

}

MuNtupleGEMSegmentFiller::~MuNtupleGEMSegmentFiller()
{

};

void MuNtupleGEMSegmentFiller::initialize()
{
  
  m_tree->Branch((m_label + "_nSegments").c_str(), &m_nSegments, (m_label + "_nSegments/i").c_str());

  m_tree->Branch((m_label + "_region").c_str(),   &m_seg_region);
  m_tree->Branch((m_label + "_ring").c_str(),  &m_seg_ring);
  m_tree->Branch((m_label + "_station").c_str(), &m_seg_station);

  m_tree->Branch((m_label + "_posLoc_x").c_str(), &m_seg_posLoc_x);
  m_tree->Branch((m_label + "_posLoc_y").c_str(), &m_seg_posLoc_y);
  m_tree->Branch((m_label + "_posLoc_z").c_str(), &m_seg_posLoc_z);
  m_tree->Branch((m_label + "_dirLoc_x").c_str(), &m_seg_dirLoc_x);
  m_tree->Branch((m_label + "_dirLoc_y").c_str(), &m_seg_dirLoc_y);
  m_tree->Branch((m_label + "_dirLoc_z").c_str(), &m_seg_dirLoc_z);

  m_tree->Branch((m_label + "_posGlb_x").c_str(), &m_seg_posGlb_x);
  m_tree->Branch((m_label + "_posGlb_y").c_str(), &m_seg_posGlb_y);
  m_tree->Branch((m_label + "_posGlb_z").c_str(), &m_seg_posGlb_z);

  m_tree->Branch((m_label + "_time").c_str(), &m_seg_time);
  m_tree->Branch((m_label + "_time_err").c_str(), &m_seg_time_err);
  m_tree->Branch((m_label + "_chi2").c_str(), &m_seg_chi2);

  m_tree->Branch((m_label + "_posGlb_phi").c_str(), &m_seg_posGlb_phi);
  m_tree->Branch((m_label + "_posGlb_eta").c_str(), &m_seg_posGlb_eta);

  m_tree->Branch((m_label + "_dirGlb_phi").c_str(), &m_seg_dirGlb_phi);
  m_tree->Branch((m_label + "_dirGlb_eta").c_str(), &m_seg_dirGlb_eta);
  

}

void MuNtupleGEMSegmentFiller::clear()
{
  m_nSegments = 0;

  m_seg_region.clear();
  m_seg_ring.clear();
  m_seg_station.clear();
  
  m_seg_posLoc_x.clear();
  m_seg_posLoc_y.clear();
  m_seg_posLoc_z.clear();
  m_seg_dirLoc_x.clear();
  m_seg_dirLoc_y.clear();
  m_seg_dirLoc_z.clear();
  
  m_seg_posGlb_x.clear();
  m_seg_posGlb_y.clear();
  m_seg_posGlb_z.clear();
  
  m_seg_time.clear();
  m_seg_time_err.clear();
  m_seg_chi2.clear();

  m_seg_posGlb_phi.clear();
  m_seg_posGlb_eta.clear();
  m_seg_dirGlb_phi.clear();
  m_seg_dirGlb_eta.clear();

  
}

void MuNtupleGEMSegmentFiller::fill(const edm::Event & ev)
{

  clear();
  
  auto segments = conditionalGet<GEMSegmentCollection>(ev, m_gemSegmentToken,"GEMSegmentCollection");

  if (segments.isValid())
    {
      
      for(auto range_iter = segments->id_begin(); range_iter != segments->id_end(); range_iter++)
	{
	  const auto range = segments->get(*range_iter);

	  for(auto segment = range.first; segment != range.second; ++segment)
	    {
	      auto region   = (*range_iter).region();
	      auto ring  = (*range_iter).ring();
	      auto station = (*range_iter).station();
	      
	      m_seg_region.push_back(region);
	      m_seg_ring.push_back(ring);
	      m_seg_station.push_back(station);
	      
	      auto pos = segment->localPosition();
	      auto dir = segment->localDirection();
	      
	      m_seg_posLoc_x.push_back(pos.x());
	      m_seg_posLoc_y.push_back(pos.y());
	      m_seg_posLoc_z.push_back(pos.z());

	      m_seg_dirLoc_x.push_back(dir.x());
	      m_seg_dirLoc_y.push_back(dir.y());
	      m_seg_dirLoc_z.push_back(dir.z());
	      
	      const GeomDet * geomDet = m_config->m_trackingGeometry->idToDet(segment->geographicalId());
	      auto posGlb = geomDet->toGlobal(pos);
	      auto dirGlb = geomDet->toGlobal(dir);
	      
	      m_seg_posGlb_x.push_back(posGlb.x());
	      m_seg_posGlb_y.push_back(posGlb.y());
	      m_seg_posGlb_z.push_back(posGlb.z());
	      m_seg_posGlb_phi.push_back(posGlb.phi());
	      m_seg_posGlb_eta.push_back(posGlb.eta());

	      m_seg_dirGlb_phi.push_back(dirGlb.phi());
	      m_seg_dirGlb_eta.push_back(dirGlb.eta());

	      auto time = segment->bunchX();
	      //auto time_err = segment->timeErr();

	      auto chi2 = segment->chi2();

	      m_seg_time.push_back(time);
	      m_seg_time_err.push_back(0);
	      m_seg_chi2.push_back(chi2);
	      
	      ++m_nSegments;
	    }
	}
    }

  return;

}


#ifndef MuNtuple_MuNtupleGEMSegmentFiller_h
#define MuNtuple_MuNtupleGEMSegmentFiller_h

#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleBaseFiller.h"

#include "DataFormats/GEMRecHit/interface/GEMSegmentCollection.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "TVectorF.h"

#include <vector>


class MuNtupleGEMSegmentFiller : public MuNtupleBaseFiller
{

 public:
  
  /// Constructor
  MuNtupleGEMSegmentFiller(edm::ConsumesCollector && collector,
			  const std::shared_ptr<MuNtupleConfig> config,
			  std::shared_ptr<TTree> tree, const std::string & label);
			  

  //Destructor
  virtual ~MuNtupleGEMSegmentFiller();

  /// Intialize function : setup tree branches etc ...
  virtual void initialize() final;

  /// Clear branches before event filling
  virtual void clear() final;

  /// Fill tree branches for a given event
  virtual void fill(const edm::Event & ev) final;
  
 private:

  /// Enum to activate "flavour-by-flavour"
  /// changes in the filling logic
  //Tag m_tag;

  /// The default TVercorF for empty vectors
  TVectorF m_nullVecF;

  /// The digi token
  edm::EDGetTokenT<GEMSegmentCollection> m_gemSegmentToken;

  /// The variables holding
  /// all digi related information

  unsigned int m_nSegments;
  
  std::vector<short> m_seg_region;   
  std::vector<short> m_seg_ring;  
  std::vector<short> m_seg_station;
  
  std::vector<float> m_seg_posLoc_x; // position x in local coordinates (float in cm)
  std::vector<float> m_seg_posLoc_y; // position y in local coordinates (float in cm)
  std::vector<float> m_seg_posLoc_z; // position z in local coordinates (float in cm)
  std::vector<float> m_seg_dirLoc_x; // direction x in local coordinates (float)
  std::vector<float> m_seg_dirLoc_y; // direction y in local coordinates (float)
  std::vector<float> m_seg_dirLoc_z; // direction z in local coordinates (float)
  
  std::vector<float> m_seg_posGlb_x; // position x in global coordinates (float in cm)
  std::vector<float> m_seg_posGlb_y;// position y in global coordinates (float in cm
  std::vector<float> m_seg_posGlb_z; // position z in global coordinates (float in cm)

  std::vector<float> m_seg_posGlb_phi; // position phi in global coordinates (float in radians [-pi:pi])
  std::vector<float> m_seg_posGlb_eta; // position eta in global coordinates (float)
  std::vector<float> m_seg_dirGlb_phi; // position phi in global coordinates (float in radians [-pi:pi])
  std::vector<float> m_seg_dirGlb_eta; // position eta in global coordinates (float)

  std::vector<float> m_seg_time;
  std::vector<float> m_seg_time_err;
  std::vector<double> m_seg_chi2;


};

#endif
  

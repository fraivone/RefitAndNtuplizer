#ifndef MuNtuple_MuNtupleGEMOHStatusFiller_h
#define MuNtuple_MuNtupleGEMOHStatusFiller_h


#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleBaseFiller.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "DataFormats/GEMDigi/interface/GEMOHStatusCollection.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/GEMDigi/interface/GEMAMC.h"

#include <vector>
class MuNtupleGEMOHStatusFiller : public MuNtupleBaseFiller
{
 public: 

  //Constructor
  MuNtupleGEMOHStatusFiller(edm::ConsumesCollector && collector,
                      const std::shared_ptr<MuNtupleConfig> config,
                      std::shared_ptr<TTree> tree, const std::string & label);

  
  //Destructor
  virtual ~MuNtupleGEMOHStatusFiller();

  /// Intialize function : setup tree branches etc ...
  virtual void initialize() final;

  /// Clear branches before event filling
  virtual void clear() final;

  /// Fill tree branches for a given events
  virtual void fill(const edm::Event & ev) final;
  

 private:
  
  
  /// The OHStatus token
  edm::EDGetToken m_gemOHStatusToken;
  edm::ESGetToken<GEMGeometry, MuonGeometryRecord> geomToken_;
  /// The variables holding
  /// all OHStatus related information

  std::vector<short> m_OHStatus_region;
  std::vector<short> m_OHStatus_station;
  std::vector<short> m_OHStatus_chamber;
  std::vector<short> m_OHStatus_chamberType;
  std::vector<short> m_OHStatus_layer;
  std::vector<uint32_t> m_OHStatus_VFATMasked;
  std::vector<uint32_t> m_OHStatus_VFATZS;
  std::vector<uint32_t> m_OHStatus_VFATMissing;
  std::vector<uint32_t> m_OHStatus_existVFAT;
  std::vector<uint16_t> m_OHStatus_errors;
  std::vector<uint8_t> m_OHStatus_warnings;
  

};

#endif


  

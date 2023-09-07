#ifndef MuNtuple_MuNtupleGEMAMCStatusFiller_h
#define MuNtuple_MuNtupleGEMAMCStatusFiller_h


#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleBaseFiller.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "DataFormats/GEMDigi/interface/GEMAMCStatusCollection.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/GEMDigi/interface/GEMAMC.h"

#include <vector>
class MuNtupleGEMAMCStatusFiller : public MuNtupleBaseFiller
{
 public: 

  //Constructor
  MuNtupleGEMAMCStatusFiller(edm::ConsumesCollector && collector,
                      const std::shared_ptr<MuNtupleConfig> config,
                      std::shared_ptr<TTree> tree, const std::string & label);

  
  //Destructor
  virtual ~MuNtupleGEMAMCStatusFiller();

  /// Intialize function : setup tree branches
  virtual void initialize() final;

  /// Clear branches before event filling
  virtual void clear() final;

  /// Fill tree branches for a given events
  virtual void fill(const edm::Event & ev) final;
  

 private:
  edm::EDGetToken gemAMCStatus_token;
  edm::ESGetToken<GEMGeometry, MuonGeometryRecord> geomToken_;

  std::vector<Int_t> m_AMCStatus_FEDId;
  std::vector<Int_t> m_AMCStatus_slot;
  // std::vector<uint32_t> m_AMCStatus_DAV_List;
  // std::vector<uint32_t> m_OHStatus_linkTO;
  

};

#endif


  

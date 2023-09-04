#include "MuDPGAnalysis/MuonDPGNtuples/src/MuNtupleEventFiller.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

//#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

MuNtupleEventFiller::MuNtupleEventFiller(edm::ConsumesCollector && collector,
					 const std::shared_ptr<MuNtupleConfig> config, 
					 std::shared_ptr<TTree> tree, const std::string & label) : 
  MuNtupleBaseFiller(config, tree, label)
{
  edm::InputTag & tcdsTag = m_config->m_inputTags["tcdsTag"];
  if (tcdsTag.label() != "none") m_TCDStoken = collector.consumes<TCDSRecord>(tcdsTag);
}


MuNtupleEventFiller::~MuNtupleEventFiller() 
{

};

void MuNtupleEventFiller::initialize()
{
  
  m_tree->Branch((m_label + "_runNumber").c_str(), &m_runNumber, (m_label + "_runNumber/I").c_str());
  m_tree->Branch((m_label + "_lumiBlock").c_str(), &m_lumiBlock, (m_label + "_lumiBlock/I").c_str());
  m_tree->Branch((m_label + "_eventNumber").c_str(), &m_eventNumber, (m_label + "_eventNumber/L").c_str());
  m_tree->Branch((m_label + "_bunchCrossing").c_str(), &m_bunchCrossing, (m_label + "_bunchCrossing/I").c_str());
  m_tree->Branch((m_label + "_orbitNumber").c_str(), &m_orbitNumber, (m_label + "_orbitNumber/I").c_str());
  m_tree->Branch((m_label + "_1stLast_L1A").c_str(), &m_L1A_1_Diff, (m_label + "_1stLast_L1A/I").c_str());
  m_tree->Branch((m_label + "_2ndLast_L1A").c_str(), &m_L1A_2_Diff, (m_label + "_2ndLast_L1A/I").c_str());
  m_tree->Branch((m_label + "_3rdLast_L1A").c_str(), &m_L1A_3_Diff, (m_label + "_3rdLast_L1A/I").c_str());
  m_tree->Branch((m_label + "_4thLast_L1A").c_str(), &m_L1A_4_Diff, (m_label + "_4thLast_L1A/I").c_str());
      
}

void MuNtupleEventFiller::clear()
{

  m_runNumber   = MuNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_lumiBlock   = MuNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_eventNumber = MuNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_bunchCrossing = MuNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_orbitNumber = MuNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_L1A_1_Diff = MuNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_L1A_2_Diff = MuNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_L1A_3_Diff = MuNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_L1A_4_Diff = MuNtupleBaseFiller::DEFAULT_INT_VAL_POS;
    
}

void MuNtupleEventFiller::fill(const edm::Event & ev)
{

  clear();
  

  auto tcdsData = conditionalGet<TCDSRecord>(ev, m_TCDStoken, "tcdsRecord");

  m_runNumber   = ev.run();
  m_lumiBlock   = ev.getLuminosityBlock().luminosityBlock();
  m_eventNumber = ev.eventAuxiliary().event();
  m_bunchCrossing = ev.bunchCrossing();
  m_orbitNumber = ev.orbitNumber();

  if(tcdsData.isValid()){
  m_L1A_1_Diff = 3564 * (tcdsData->getOrbitNr() - tcdsData->getL1aHistoryEntry(0).getOrbitNr()) + tcdsData->getBXID() - tcdsData->getL1aHistoryEntry(0).getBXID();
  
  m_L1A_2_Diff = 3564 * (tcdsData->getOrbitNr() - tcdsData->getL1aHistoryEntry(1).getOrbitNr()) + tcdsData->getBXID() - tcdsData->getL1aHistoryEntry(1).getBXID();
  
  m_L1A_3_Diff = 3564 * (tcdsData->getOrbitNr() - tcdsData->getL1aHistoryEntry(2).getOrbitNr()) + tcdsData->getBXID() - tcdsData->getL1aHistoryEntry(2).getBXID();
  
  m_L1A_4_Diff = 3564 * (tcdsData->getOrbitNr() - tcdsData->getL1aHistoryEntry(3).getOrbitNr()) + tcdsData->getBXID() - tcdsData->getL1aHistoryEntry(3).getBXID();
  }
  return;

}

// -*- C++ -*-
//
// Package:    MuonGEM/MuonRefit
// Class:      MuonRefit
//
/**\class MuonRefit MuonRefit.cc MuonGEM/MuonRefit/plugins/MuonRefit.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  mmaggi
//         Created:  Thu, 13 Jul 2023 08:39:32 GMT
//
//

// system include files
#include <memory>
#include <vector>

// user include files
#include "PlayingWithTracks/MuonRefit/interface/MuonTransformer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"

#include "FWCore/Framework/interface/ConsumesCollector.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
//
// class declaration
//

class MuonRefit : public edm::stream::EDProducer<> {
public:
  explicit MuonRefit(const edm::ParameterSet&);
  ~MuonRefit() override;
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
private:
  void beginStream(edm::StreamID) override;
  void produce(edm::Event&, const edm::EventSetup&) override;
  void endStream() override;
  
  //void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //void endRun(edm::Run const&, edm::EventSetup const&) override;
  //void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  
  // ----------member data ---------------------------
  edm::EDGetTokenT<reco::MuonCollection> muonToken_;
  std::string theBranchLabel;
  bool theVerboseOption;
  int theRefitOption;
  //  std::unique_ptr<TrackTransformerBase> theMuonTransformer;
  std::unique_ptr<MuonTransformer> theMuonTransformer;
  TrajectoryStateCombiner tsoscomb;
  edm::ESGetToken<GEMGeometry, MuonGeometryRecord> geometry_token_;
  std::string thePropagatorName;
  edm::ESGetToken<Propagator, TrackingComponentsRecord> thePropagatorToken;


};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MuonRefit::MuonRefit(const edm::ParameterSet& iConfig) 
  : muonToken_(consumes<reco::MuonCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tracks"))),
    theBranchLabel(iConfig.getParameter<std::string>("francesco")),
    theVerboseOption(iConfig.getParameter<bool>("verbose")),
    theRefitOption(iConfig.getParameter<int>("refitOption"))
{
  edm::ParameterSet muonTransformerParam = iConfig.getParameter<edm::ParameterSet>("MuonTransformer");

  theMuonTransformer =
    std::make_unique<MuonTransformer>(muonTransformerParam,consumesCollector());
  
  thePropagatorName = muonTransformerParam.getParameter<std::string>("Propagator");

  thePropagatorToken = esConsumes(edm::ESInputTag("", thePropagatorName));

  geometry_token_ = esConsumes<GEMGeometry, MuonGeometryRecord>();
  //register your products
  produces<std::vector<Trajectory>>(theBranchLabel).setBranchAlias(theBranchLabel);;
  /* Examples
     produces<ExampleData2>();
     
     //if do put with a label
  produces<ExampleData2>("label");
 
  //if you want to put into the Run
  produces<ExampleData2,InRun>();
  */
  //now do what ever other initialization is needed
}

MuonRefit::~MuonRefit() {
  // do anything here that needs to be done at destruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//

// ------------ method called to produce the data  ------------
void MuonRefit::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  if (theVerboseOption)
    std::cout <<" \n++++++++++++\nNew Event "<<iEvent.id().event()<<std::endl;
  edm::ESHandle<GEMGeometry> gemg = iSetup.getHandle(geometry_token_);
  const GEMGeometry*  mgeom = &*gemg;
  std::unique_ptr< std::vector<Trajectory> > collection( new std::vector<Trajectory> );  

  // and already reserve some space for the new data, to control the size
  // of your executible's memory use.
  // const int size = iEvent.get(muonToken_).size();
  // collection->reserve( size );
  
  
  theMuonTransformer->setServices(iSetup);
  
  edm::ESHandle<Propagator> prop = iSetup.getHandle(thePropagatorToken);
  int  muonCount = 0;
  for (const auto& muon : iEvent.get(muonToken_)) {
    muonCount += 1;
    int charge = muon.charge();
    if(!muon.globalTrack().isNull()) {
      if (theVerboseOption)
        std::cout <<"\n-----------\nNew GlobalTrack Muon with charge "<<charge<<" pt="<<muon.pt()<<std::endl;
      
      const reco::TrackRef outerTrackRef = muon.globalTrack();
      //      std::cout <<" Check if we have rechits"<<std::endl;
      auto recHitMuBeg = outerTrackRef->recHitsBegin();
      auto recHitMuEnd = outerTrackRef ->recHitsEnd();
      //      std::cout <<" Loop over rechits"<<std::endl;
      for (auto rh=recHitMuBeg;rh<recHitMuEnd;rh++){
        DetId detId = (*rh)->geographicalId();
	      if (detId.det() == DetId::Muon && detId.subdetId() == 4){
          GEMDetId gid(detId.rawId());
          if (theVerboseOption)
            std::cout<<" GEM Layer "<<gid.layer()<<" coord. "<<(*rh)->localPosition().x()<<" "<<(*rh)->localPosition().y()<<" "<<(*rh)->localPosition().z()<<std::endl;
	      }
      } // Loop on all GEM Hits in the current global track
    
      const reco::Track track = *muon.globalTrack().get();
      // here I'm refitting and I use the python option to decide if to include or exclude the GEM SYSTEM
      //      auto const& trajectoriesSM = theMuonTransformer->transform(track);
      // here I'm refitting excluding layer 1 (1=layer2 excluded, 2=layer1 excluded, 3-=exclude both, 0=exclude none)
      if (theVerboseOption)
        std::cout<<"Reftitting with option " << theRefitOption << std::endl;
      auto const& trajectoriesSM = theMuonTransformer->muontransform(track,theRefitOption);
      if (trajectoriesSM.size() > 0){
        auto traj = trajectoriesSM.front();
        // collection->push_back(traj);
        std::vector<TrajectoryMeasurement> mss = traj.measurements();
        // let's determine the last tsos on tracker (the one nearest to muon)
        // let's determine the first tsos on muon (except GEM)
        TrajectoryStateOnSurface lastTrackerTsos;
        TrajectoryStateOnSurface firstMuonTsos;
        double lastTrackerPos = 0.0;
        double firstMuonPos = 100000.0;
        bool hasM=false;
        bool hasT=false;
        if (theVerboseOption)
          std::cout<<"\n Muon "<<muonCount<<" looping on the measurements of the refitted track" << std::endl;
        for (auto ims : mss){
	
	      TrajectoryStateOnSurface tsos = tsoscomb(ims.forwardPredictedState(), ims.backwardPredictedState());
	      const TransientTrackingRecHit* hit = &(*ims.recHit());
	      DetId detId = hit->geographicalId();
	      
        if (tsos.isValid() && detId.det() == DetId::Tracker) {
	        double tsosPos = sqrt(tsos.globalPosition().x() * tsos.globalPosition().x() + tsos.globalPosition().y() * tsos.globalPosition().y()); 
          double tsoszPos = tsos.globalPosition().z();
	        if (tsosPos > lastTrackerPos) {
	          lastTrackerTsos = tsos;
	          lastTrackerPos = tsosPos;
	          hasT=true;
	          if (theVerboseOption)
              std::cout <<"\tNew last on Tracker R = " <<tsosPos<<" z = "<<tsoszPos<<std::endl;
	        }
	      } // end Current measurement belongs to the Tracker 
        
        else if (tsos.isValid() && detId.det() == DetId::Muon && detId.subdetId() == 4){
	        if (theVerboseOption){
            std::cout << " GEM "<< GEMDetId(detId.rawId()) << " TrajectoryMeasurement TSOS localPosition"
                      << " x: " << tsos.localPosition().x() << " y: " << tsos.localPosition().y()
                      << " z: " << tsos.localPosition().z();
	          std::cout << " GEM     measurement "
                      << " x: " << hit->localPosition().x() << " y: " << hit->localPosition().y()
                      << " z: " << hit->localPosition().z() << std::endl;
          }
	      } // end Current measurement belongs to the Muon/GEM 

        else if (tsos.isValid() && detId.det() == DetId::Muon && detId.subdetId() != 4){	  
	        double tsosPos = sqrt(tsos.globalPosition().x() * tsos.globalPosition().x() + tsos.globalPosition().y() * tsos.globalPosition().y());
          double tsoszPos = tsos.globalPosition().z();
          if (tsosPos < firstMuonPos) {
            firstMuonTsos = tsos;
            firstMuonPos = tsosPos;
	          hasM=true;
            if (theVerboseOption)
	            std::cout <<"\tNew first on Muon R = " <<tsosPos<<" z = "<<tsoszPos<<std::endl;
          }	  
	      } // end Current measurement belongs to the Muon/nonGEM

        }// end loop on the measurements of the transformed track
        if (theVerboseOption)
          std::cout<<" LoopCompleted\n" << std::endl;
        
        if (hasM && hasT) {
          // here I start from the tracker and I propagte to GEM system to discover which detid is involved...
          int muon_reg = track.outerZ() > 0 ? 1 : -1;
          if (theVerboseOption)
            std::cout <<" Muon "<<muonCount<<" refitted track goes through Tr and Muon\tOriginalMuon pt ="<<muon.pt()<<std::endl;
          for (auto ch : mgeom->chambers() ){
            GEMDetId GEMId=ch->id();
            if (GEMId.region() == muon_reg){ 
              const BoundPlane& bp  = mgeom->idToDet(GEMId)->surface();  
              TrajectoryStateOnSurface toGemTr = prop->propagate(lastTrackerTsos,bp);
              TrajectoryStateOnSurface toGemMu = prop->propagate(firstMuonTsos,bp);
              
              if (toGemTr.isValid() && toGemMu.isValid()){
                TrajectoryStateOnSurface toGem = tsoscomb(toGemTr,toGemMu);	    
                // Checking the chambers extrapolation taking into account 2 sigma errors on the extrapolation
                if (toGem.isValid() && bp.bounds().inside(toGem.localPosition(),toGem.localError().positionError(),2.) ){
                  if (theVerboseOption){
                    std::cout <<" Valid GEM propagation in "<<GEMId<<std::endl;
                    std::cout <<" Co local "<<toGem.localPosition()<<" global "<<toGem.globalPosition()<<std::endl;
                    std::cout <<" Tr local "<<toGemTr.localPosition()<<" global "<<toGemTr.globalPosition()<<std::endl;
                    std::cout <<" Mu local "<<toGemMu.localPosition()<<" global "<<toGemMu.globalPosition()<<std::endl;
                  }
                } // end propagation is on the chamber within 2sigma error
            
              } // propagation Tr2GEM and  Mu2GEM is valid
            } // end Chamber in compatible region
          } // end loop an all chambers

        } // end track passes through Tracker && Muon
      } // end refit track exists
    } // end muon global track is non Null
    else {
      if (theVerboseOption)
        std::cout<<"\n Muon "<<muonCount<<" was not a global muon" << std::endl;
    }
  
  } // end loop on all muons
  if (theVerboseOption)
    std::cout <<"\nClosing Event \n++++++++++++ "<<std::endl;
  iEvent.put(std::move(collection),theBranchLabel);
  
  

}

// ------------ method called once each stream before processing any runs, lumis or events  ------------
void MuonRefit::beginStream(edm::StreamID) {
  // please remove this method if not needed
}

// ------------ method called once each stream after processing all runs, lumis and events  ------------
void MuonRefit::endStream() {
  // please remove this method if not needed
}

// ------------ method called when starting to processes a run  ------------
/*
void
MuonRefit::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void
MuonRefit::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void
MuonRefit::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void
MuonRefit::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MuonRefit::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MuonRefit);

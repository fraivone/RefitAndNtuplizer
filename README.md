# MuonDPGNtuples

Repository to host common ntuples developed and maintained by the CMS muon DPGs.
Works with CMSSW 13. For crab submission use crab-dev

## Installation:
### Download 
You may want to use a specific CMSSW  version (from now on referred as CMSSW_XXXX) and global tag based on the data you are about to Ntuplize:
- For **P5 data** check  [Global Tags for Conditions Data ](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions)
- For **MC data** check the production parameters

```
cmsrel CMSSW_13_0_6
cd CMSSW_13_0_6
cmsenv

git clone --branch main git@github.com:fraivone/RefitAndNtuplizer.git
```

## Some info
**DataFormats/GEMDigi** contains the code to add relevant info to the `GEMOHStatus.h` and `GEMAMCStatus.h` so that I can store the DAQ status in the ntuples
**MuDPGAnalysis/MuonDPGNtuples** is the standard ntuplizer set up to work with the track refitter written by Marcello.
**PlayingWithTracks/MuonRefit** contains the class that takes care of refitting a global track with
    1. `gemcase 0` GEM included
    1. `gemcase 1` GEM Layer 1 Included
    1. `gemcase 2` GEM Layer 2 Included 
    1. `gemcase 3` GEM excluded
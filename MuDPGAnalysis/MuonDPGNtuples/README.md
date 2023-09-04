# MuonDPGNtuples

Repository to host common ntuples developed and maintained by the CMS muon DPGs.
Works with CMSSW 12. For crab submission use crab-dev

## Installation:
### Download 
You may want to use a specific CMSSW  version (from now on referred as CMSSW_XXXX) and global tag based on the data you are about to Ntuplize:
- For **P5 data** check  [Global Tags for Conditions Data ](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideFrontierConditions)
- For **MC data** check the production parameters

```
cmsrel CMSSW_XXXX 
cd CMSSW_XXXX/src/ 
cmsenv

git clone --branch GEMOHStatusCMSSW_12_6_X git@github.com:gem-dpg-pfa/MuonDPGNTuples.git MuDPGAnalysis/MuonDPGNtuples
```

### Compile
```
scram b -j 5
```
## How to run the Ntuplzier with CRAB
- You have to authenticate yourself by mean of the grid certificate ( check the  [Twiki](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookStartingGrid#Using_your_grid_certificate) )
- Source `/cvmfs/cms.cern.ch/common/crab-setup.sh`
- Execute `python3  ./CRAB_SUB/crabConfig.py --RunList <space spearated list of runs to ntuplize> --Dataset <Express or Prompt>` (express data available as soon as the run ends, prompt data available within 48h from the end of the run)
- Monitor the status of your jobs

## Possible issues
- crab jobs fail because some of the data are not completely transferred.

In these cases it is worth waiting ~24h and try again. If the jobs keep failing email the computing Computing Tools team

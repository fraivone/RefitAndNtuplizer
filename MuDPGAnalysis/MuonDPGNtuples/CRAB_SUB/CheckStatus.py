import glob
from CRABAPI.RawCommand import crabCommand
from CRABClient.UserUtilities import setConsoleLogLevel
from CRABClient.ClientUtilities import LOGLEVEL_MUTE
setConsoleLogLevel(LOGLEVEL_MUTE)

folders = glob.glob("crab_*")

print(f'{"folder":>50}{"finished":>10}{"transferring":>15}{"running":>10}{"idle":>10}{"unsubmitted":>15}{"failed":>10}{"%finished":>15}{"total":>10}{"submitted":>14}')
for f in folders:
    res = crabCommand('status', dir = f)

    failed = res["jobsPerStatus"].get("failed",0)
    finished = res["jobsPerStatus"].get("finished",0)
    running = res["jobsPerStatus"].get("running",0)
    idle = res["jobsPerStatus"].get("idle",0)
    unsubmitted = res["jobsPerStatus"].get("unsubmitted",0)
    transferring = res["jobsPerStatus"].get("transferring",0)
    tot = failed + finished + running + idle + unsubmitted + transferring
    submitted = len(res["jobList"])
    successrate = round(float(finished/submitted)*100,2) if submitted!=0 else 0
    
    print(f'{f:>50}{finished:>10}{transferring:>15}{running:>10}{idle:>10}{unsubmitted:>15}{failed:>10}{successrate:>15}{tot:>10}{submitted:>14}')

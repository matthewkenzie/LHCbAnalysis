#######################
##   1.) SKIMMING    ##
#######################

# Run Skimming on Cambridge batch system
#bin/Bc2Dmunu_Skimming -c dat/skimming_cfg_mc.dat   -j -D sub_jobs_mc -q CONDOR
#bin/Bc2Dmunu_Skimming -c dat/skimming_cfg_data.dat -j -D sub_jobs_data -s 1000000 -q CONDOR

# Data jobs have been split into different files so hadd the subjobs
#hadd -ff sub_jobs_data/Data2011.root sub_jobs_data/Data2011_j*.root
#hadd -ff sub_jobs_data/Data2012.root sub_jobs_data/Data2012_j*.root
#hadd -ff sub_jobs_data/Data2015.root sub_jobs_data/Data2015_j*.root
#hadd -ff sub_jobs_data/Data2016.root sub_jobs_data/Data2016_j*.root
#hadd -ff sub_jobs_data/Data2017.root sub_jobs_data/Data2017_j*.root

#######################
##   1.) ANALYSIS    ##
#######################

# make some plots
bin/Bc2Dmunu_SkimmingPlotter -c dat/analysis_cfg.dat -l 50000

# train the BDT
bin/Bc2Dmunu_BDTTrain -c dat/bdttraining_cfg.dat -l 50000

# evaluate the BDT (i.e. run the analysis)
bin/Bc2Dmunu_Analysis -c dat/analysis_cfg.dat -l 50000

# the output file in root/AnalysisOut.root
# now contains all the skimmed events along with
# a branch called bu_rejection_bdtoutput
# this can be used to think about where the optimum
# cut should be placed

# can make some plots in html format
python ../scripts/make_html.py
cp -r plots ~/public_html/Bc2Dmunu

# you can now view all the plots at
# www.hep.phy.cam.ac.uk/~<user_name>/Bc2Dmunu
# for an example go to:
# http://www.hep.phy.cam.ac.uk/~mkenzie/browser/lhcb/Bc2Dmunu

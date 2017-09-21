#
# 1.) Run the preselection on lxplus
# 2.) Run the PID correction on lxplus (use Meerkat - PIDGen)
# 3.) Train BDT
#       - bin/Bs2KstKst_BDTTraining -c dat/bdttraining_cfg.dat -o root/TrainingOut.root
# 4.) PID + BDT cut opt
#       - bin/Bs2KstKst_Analysis -c dat/analysis_cfg.dat (read weights recreate AnalysisOut.root file and datasets file)
#       - bin/Bs2KstKst_BDTAndPIDCutOptimisation
# 5.) Run the analysis selection
#       - bin/Bs2KstKst_Analysis -c dat/analysis_cfg.dat
# 6.) Run Fit
#     a) Bs2KstKst_PrepareWorkspace (flags mult_cands and makes workspace)
#     b) Bs2KstKst_RunFit (runs the fit)
#     c) Bs2KstKst_AddSWeightsToTree


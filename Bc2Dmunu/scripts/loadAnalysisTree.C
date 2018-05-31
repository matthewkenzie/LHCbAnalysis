void loadAnalysisTree(){

  TChain *t = new TChain("AnalysisTree");
  t->Add("anal_jobs/Data2011.root");

  TCut pid = "K_minus_ProbNNk>0.6 && K_minus_ProbNNpi<0.4 && Pi_plus_ProbNNk<0.4 && Pi_plus_ProbNNpi>0.6 && Mu_plus_ProbNNmu>0.6 && Mu_plus_ProbNNk<0.4 && Mu_plus_ProbNNpi<0.4";
  TCut bdt1 = "comb_rejection_bdtoutput>0.";
  TCut bdt2 = "bu_rejection_bdtoutput>0.";
  TCut dcs = "(K_minus_ID*Mu_plus_ID)>0";

  TCut all = pid+bdt1+bdt2+dcs;

  t->Draw("D0_M","itype>0"+all);
  new TCanvas();
  t->Draw("B_plus_MCORR","itype>0"+all);


}

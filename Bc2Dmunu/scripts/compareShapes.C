TCanvas* plot(TTree *t, TString name, TCut add_cut=""){

  TCut dcs = "(K_minus_ID*Mu_plus_ID)>0";

  TH1F *b_all = new TH1F("b_all","; B+ MCORR;",100,2000,15000);
  TH1F *d_all = new TH1F("d_all","; D0 M;",100,1785,1944);

  TH1F *b_cf = new TH1F("b_cf","; B+ MCORR;",100,2000,15000);
  TH1F *d_cf = new TH1F("d_cf","; D0 M;",100,1785,1944);

  TH1F *b_dcs = new TH1F("b_dcs","; B+ MCORR;",100,2000,15000);
  TH1F *d_dcs = new TH1F("d_dcs","; D0 M;",100,1785,1944);

  t->Draw( "B_plus_MCORR>>b_all" , add_cut , "goff" );
  t->Draw( "D0_M>>d_all" , add_cut, "goff" );

  t->Draw( "B_plus_MCORR>>b_cf", (!dcs)+add_cut , "goff" );
  t->Draw( "D0_M>>d_cf" , (!dcs)+add_cut, "goff" );

  t->Draw( "B_plus_MCORR>>b_dcs", dcs+add_cut , "goff" );
  t->Draw( "D0_M>>d_dcs", dcs+add_cut , "goff" );

  b_all->SetName( "b_all"+name );
  d_all->SetName( "d_all"+name );
  b_cf->SetName( "b_cf"+name );
  d_cf->SetName( "d_cf"+name );
  b_dcs->SetName( "b_dcs"+name );
  d_dcs->SetName( "d_dcs"+name );

  b_all->SetLineColor(kBlack);
  d_all->SetLineColor(kBlack);
  b_cf->SetLineColor(kRed);
  d_cf->SetLineColor(kRed);
  b_dcs->SetLineColor(kBlue);
  d_dcs->SetLineColor(kBlue);

  TLegend *leg = new TLegend(0.45,0.7,0.7,0.9);
  leg->SetHeader(name);
  double dcs_frac = 100.*b_dcs->Integral() / b_all->Integral();
  double cf_frac = 100.*b_cf->Integral() / b_all->Integral();

  leg->AddEntry(b_dcs, Form("DCS (%4.1f%%)",dcs_frac), "L");
  leg->AddEntry(b_cf, Form("CF (%4.1f%%)",cf_frac), "L");
  leg->AddEntry(b_all, "Sum", "L");

  TCanvas *c = new TCanvas("c"+name,"c",1600,1200);
  c->Divide(2,2);
  c->cd(1);
  b_all->Draw("HIST");
  b_cf->Draw("HISTsame");
  b_dcs->Draw("HISTsame");
  b_all->Draw("HISTsame");
  leg->Draw("same");
  c->cd(3);
  d_all->Draw("HIST");
  d_cf->Draw("HISTsame");
  d_dcs->Draw("HISTsame");
  d_all->Draw("HISTsame");
  c->cd(2);
  TH1F *b_cf_norm = (TH1F*)b_cf->Clone("b_cf_norm");
  b_cf_norm->Scale(1./b_cf_norm->Integral());
  b_cf_norm->Draw("HIST");
  TH1F *b_dcs_norm = (TH1F*)b_dcs->Clone("b_dcs_norm");
  b_dcs_norm->Scale(1./b_dcs_norm->Integral());
  b_dcs_norm->Draw("HISTsame");
  c->cd(4);
  TH1F *d_cf_norm = (TH1F*)d_cf->Clone("d_cf_norm");
  d_cf_norm->Scale(1./d_cf_norm->Integral());
  d_cf_norm->Draw("HIST");
  TH1F *d_dcs_norm = (TH1F*)d_dcs->Clone("d_dcs_norm");
  d_dcs_norm->Scale(1./d_dcs_norm->Integral());
  d_dcs_norm->Draw("HISTsame");

  c->Print("plots/CompShapes/pdf/"+name+".pdf");
  c->Print("plots/CompShapes/png/"+name+".png");

  return c;

}

void compareShapes(){


  TChain *t = new TChain("AnalysisTree");
  t->Add("anal_jobs/Data2011.root");
  t->Add("anal_jobs/Data2012.root");
  t->SetEntries(10000);
  //TFile *f = TFile::Open("anal_jobs/Data2011.root");
  //TTree *t = (TTree*)f->Get("AnalysisTree");

  TCut pid = "K_minus_ProbNNk>0.6 && K_minus_ProbNNpi<0.4 && Pi_plus_ProbNNk<0.4 && Pi_plus_ProbNNpi>0.6 && Mu_plus_ProbNNmu>0.6 && Mu_plus_ProbNNk<0.4 && Mu_plus_ProbNNpi<0.4";
  TCut bdt1 = "comb_rejection_bdtoutput>0.";
  TCut bdt2 = "bu_rejection_bdtoutput>0.";
  TCut sb   = "D0_M<1830 || D0_M>1900";

  TCanvas *c1 = plot(t, "all", "");
  TCanvas *c2 = plot(t, "pid", pid);
  TCanvas *c3 = plot(t, "bdt1", bdt1);
  TCanvas *c4 = plot(t, "bdt2", bdt2);
  TCanvas *c5 = plot(t, "pid_bdt2", pid+bdt2);

  TCanvas *c6 = plot(t, "all_sb", sb);
  TCanvas *c7 = plot(t, "pid_sb", sb+pid);
  TCanvas *c8 = plot(t, "bdt1_sb", sb+bdt1);
  TCanvas *c9 = plot(t, "bdt2_sb", sb+bdt2);
  TCanvas *c10 = plot(t, "pid_bdt2_sb", sb+pid+bdt2);



}

#include <iostream>
#include <vector>
#include <cmath>

#include "TROOT.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TAxis.h"

#include "RooPlot.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooAbsPdf.h"
#include "RooFitResult.h"
#include "RooMsgService.h"

#include "Utils.h"
#include "RunCutOptimisation.h"

using namespace std;
using namespace RooFit;
using namespace Bs2KstKst;
using namespace Utils;

RooPlot* plotSimple(RooWorkspace *w, TString var, TString data, TString pdf) {

  RooPlot *plot = w->var(var)->frame();
  if (w->data(data)) w->data(data)->plotOn(plot);
  if (w->pdf(pdf))   w->pdf(pdf)->plotOn(plot);
  return plot;
}

void fitAndSetConst(RooWorkspace *w, TString pdf, TString data) {

  if (!w->pdf(pdf) || !w->data(data)){
    cerr << "Bs2KstKst::MultiDimCutOpt::fitAndSetConst() -- NOT VALID" << endl;
    exit(1);
  }
  w->pdf(pdf)->fitTo(*w->data(data));
  RooArgSet *params = (RooArgSet*)w->pdf(pdf)->getParameters(*w->set("observables"));
  params->setAttribAll("Constant");
  w->defineSet(pdf+"_params",*params);
  delete params;

}

void getExpectationFromFit( TString year, int misIdType, int &nSig, int &nBkg, int &nMis ) {

  TFile *inFile = TFile::Open("root/CutOptimisation/CutOptimisationDatasets.root");
  RooWorkspace *w = (RooWorkspace*)inFile->Get("w");
  w->Print();

  w->factory("sig_mc_mean[5350,5450]");
  w->factory("CBShape::sig_mc_cb1( B_s0_DTF_B_s0_M, sig_mc_mean, sig_mc_sigma1[20,5,200], sig_mc_alpha_1[0.5,0.,5], sig_mc_n_1[0.5,0.,10] )");
  w->factory("CBShape::sig_mc_cb2( B_s0_DTF_B_s0_M, sig_mc_mean, sig_mc_sigma2[20,5,200], sig_mc_alpha_2[-0.5,-5.,0], sig_mc_n_2[0.5,0.,10] )");
  w->factory("SUM::sig_mc_pdf( sig_mc_f1[0.6,0.,1.]*sig_mc_cb1, sig_mc_cb2 )");

  w->factory("bd_mc_mean[5200,5350]");
  w->factory("CBShape::bd_mc_cb1( B_s0_DTF_B_s0_M, bd_mc_mean, bd_mc_sigma1[20,10,200], bd_mc_alpha_1[0.5,0.,5], bd_mc_n_1[0.5,0.,10] )");
  w->factory("CBShape::bd_mc_cb2( B_s0_DTF_B_s0_M, bd_mc_mean, bd_mc_sigma2[20,10,200], bd_mc_alpha_2[-0.5,-5.,0], bd_mc_n_2[0.5,0.,10] )");
  w->factory("SUM::bd_mc_pdf( bd_mc_f1[0.6,0.,1.]*bd_mc_cb1, bd_mc_cb2 )");

  w->factory("rho_mc_mean[5250,5350]");
  w->factory("CBShape::rho_mc_cb1( B_s0_DTF_B_s0_M, rho_mc_mean, rho_mc_sigma1[20,10,200], rho_mc_alpha1[0.5,0.,5.], rho_mc_n1[0.01,0.,40.])");
  w->factory("CBShape::rho_mc_cb2( B_s0_DTF_B_s0_M, rho_mc_mean, rho_mc_sigma2[20,10,200], rho_mc_alpha2[-0.5,-5.,0.], rho_mc_n2[0.01,0.,40.])");
  w->factory("SUM::rho_mc_pdf( rho_mc_f1[0.2,0.,1.]*rho_mc_cb1, rho_mc_cb2)");

  w->factory("phi_mc_mean[5100,5350]");
  w->factory("CBShape::phi_mc_cb1( B_s0_DTF_B_s0_M, phi_mc_mean, phi_mc_sigma1[100,0,400], phi_mc_alpha1[0.5,0.,5.], phi_mc_n1[0.01,0.,100.])");
  w->factory("CBShape::phi_mc_cb2( B_s0_DTF_B_s0_M, phi_mc_mean, phi_mc_sigma2[100,0,400], phi_mc_alpha2[-0.5,-5.,0.], phi_mc_n2[0.1,0.,100.])");
  w->factory("SUM::phi_mc_pdf( phi_mc_f1[0.6,0.,1.]*phi_mc_cb1, phi_mc_cb2 )");

  w->factory("lb_mc_mean[5300,5650]");
  w->factory("CBShape::lb_mc_cb1( B_s0_DTF_B_s0_M, lb_mc_mean, lb_mc_sigma1[100,0,400], lb_mc_alpha1[0.5,0.,5.], lb_mc_n1[0.01,0.,100.])");
  w->factory("CBShape::lb_mc_cb2( B_s0_DTF_B_s0_M, lb_mc_mean, lb_mc_sigma2[100,0,400], lb_mc_alpha2[0.5,0.,5.], lb_mc_n2[0.1,0.,100.])");
  w->factory("SUM::lb_mc_pdf( lb_mc_f1[0.6,0.,1.]*lb_mc_cb1, lb_mc_cb2 )");

  // background and total pdf
  w->factory("ArgusBG::part_reco_pdf( B_s0_DTF_B_s0_M, part_reco_m0[5226], part_reco_c[-10,-100.,0.], part_reco_p[0.4,0.,1.] )");
  w->factory("Exponential::bkg_pdf( B_s0_DTF_B_s0_M, bkg_exp_p1[-0.02,-0.04,0.] )");

  w->factory("SUM::pdf( bkg_y[0,200e3]*bkg_pdf, part_reco_y[0,200e3]*part_reco_pdf, sig_mc_y[0,100e3]*sig_mc_pdf, bd_mc_y[0,10e3]*bd_mc_pdf, rho_mc_y[0,50e3]*rho_mc_pdf, phi_mc_y[0,50e3]*phi_mc_pdf, lb_mc_y[0,50e3]*lb_mc_pdf)");

  w->Print();

  fitAndSetConst(w, "sig_mc_pdf" , Form("Bs2Kst0Kst0_MC%s",year.Data()) );
  fitAndSetConst(w, "bd_mc_pdf"  , Form("Bd2Kst0Kst0_MC%s",year.Data()) );
  fitAndSetConst(w, "rho_mc_pdf" , Form("Bd2RhoKst0_MC%s",year.Data())  );
  fitAndSetConst(w, "phi_mc_pdf" , Form("Bd2PhiKst0_MC%s",year.Data())  );
  fitAndSetConst(w, "lb_mc_pdf"  , Form("Lb2pKpipi_MC%s",year.Data())   );

  w->var("sig_mc_mean")->setConstant(false);

  RooFitResult *fitRes = w->pdf("pdf")->fitTo(*w->data(Form("Data%s",year.Data())),Save());
  fitRes->SetName("full_data_fit_res");
  w->import(*fitRes);

  RooArgSet *part_reco_params = (RooArgSet*)w->pdf("part_reco_pdf")->getParameters(*w->set("observables"));
  w->defineSet("part_reco_params",*part_reco_params);

  RooArgSet *bkg_params = (RooArgSet*)w->pdf("bkg_pdf")->getParameters(*w->set("observables"));
  w->defineSet("bkg_params",*bkg_params);

  RooArgSet *params = (RooArgSet*)w->pdf("pdf")->getParameters(*w->set("observables"));
  w->defineSet("params",*params);

  if ( misIdType == 1 ) {
    w->factory("sum::misid_mc_y(rho_mc_y,phi_mc_y,lb_mc_y)");
  }
  else if ( misIdType == 2 ) {
    w->factory("sum::misid_mc_y(rho_mc_y)");
  }
  else if ( misIdType == 3 ) {
    w->factory("sum::misid_mc_y(phi_mc_y)");
  }
  else if ( misIdType == 4 ) {
    w->factory("sum::misid_mc_y(lb_mc_y)");
  }
  else if ( misIdType == 5 ) {
    w->factory("sum::misid_mc_y(rho_mc_y,phi_mc_y)");
  }
  else {
    cout << "ERROR -- not a valid misIdType " << misIdType << endl;
  }
  w->writeToFile(Form("root/CutOptimisation/CutOptCache%s.root",year.Data()));
  inFile->Close();
  delete inFile;

  nSig = int( round( w->var("sig_mc_y")->getVal() ) );
  nBkg = int( round( w->var("bkg_y")->getVal() ) );
  nMis = int( round( w->function("misid_mc_y")->getVal() ) );

}

void plotExpectationFit(TString year) {

  TString cachefilename = Form("root/CutOptimisation/CutOptCache%s.root",year.Data());
  TFile *cacheFile = TFile::Open(cachefilename);
  RooWorkspace *w = (RooWorkspace*)cacheFile->Get("w");

  TCanvas *canv = new TCanvas("shapes","shapes",1600,1200);
  canv->Divide(3,2);
  RooPlot* sig_mc_plot = plotSimple(w, "B_s0_DTF_B_s0_M",Form("Bs2Kst0Kst0_MC%s",year.Data()), "sig_mc_pdf");
  RooPlot *bd_mc_plot  = plotSimple(w, "B_s0_DTF_B_s0_M",Form("Bd2Kst0Kst0_MC%s",year.Data()), "bd_mc_pdf");
  RooPlot *rho_mc_plot = plotSimple(w, "B_s0_DTF_B_s0_M",Form("Bd2RhoKst0_MC%s",year.Data()) , "rho_mc_pdf");
  RooPlot *phi_mc_plot = plotSimple(w, "B_s0_DTF_B_s0_M",Form("Bd2PhiKst0_MC%s",year.Data()) , "phi_mc_pdf");
  RooPlot *lb_mc_plot = plotSimple(w, "B_s0_DTF_B_s0_M", Form("Lb2pKpipi_MC%s",year.Data())  , "lb_mc_pdf");

  canv->cd(1);
  sig_mc_plot->Draw();
  canv->cd(2);
  bd_mc_plot->Draw();
  canv->cd(3);
  rho_mc_plot->Draw();
  canv->cd(4);
  phi_mc_plot->Draw();
  canv->cd(5);
  lb_mc_plot->Draw();

  canv->Update();
  canv->Modified();
  canv->Print(Form("plots/CutOptimisation/pdf/shapes_%s.pdf",year.Data()));

  TCanvas *c = new TCanvas();
  RooPlot *plot = w->var("B_s0_DTF_B_s0_M")->frame();
  TLegend *leg = new TLegend(0.65,0.5,0.92,0.92);
  TPaveText *label = new TPaveText(0.2,0.84,0.4,0.9,"ndc");
  label->SetFillColor(0);
  label->SetShadowColor(0);
  label->SetLineWidth(1);
  label->SetLineColor(1);
  label->SetTextSize(0.08);
  label->SetTextAlign(11);
  if ( year=="all" ) label->AddText("2011+2012");
  else label->AddText(year);
  TPaveText *params = new TPaveText(0.24,0.7,0.4,0.84,"ndc");
  params->SetFillColor(0);
  params->SetShadowColor(0);
  params->SetLineWidth(0);
  params->SetTextSize(0.03);
  params->SetTextAlign(11);
  params->AddText(Form("%-5s = %6.2f #pm %6.2f","N_{sig}",w->var("sig_mc_y")->getVal(),w->var("sig_mc_y")->getError()));
  params->AddText(Form("%-5s = %6.2f #pm %6.2f","N_{bkg}",w->var("bkg_y")->getVal(),w->var("bkg_y")->getError()));
  params->AddText(Form("%-5s = %6.2f #pm %6.2f","N_{mis}",w->function("misid_mc_y")->getVal(),w->function("misid_mc_y")->getPropagatedError(*((RooFitResult*)w->obj("full_data_fit_res")))));
  //TPaveText *cuts = new TPaveText(0.3,0.56,0.46,0.7,"ndc");
  //cuts->SetFillColor(0);
  //cuts->SetShadowColor(0);
  //cuts->SetLineWidth(0);
  //cuts->SetTextSize(0.03);
  //cuts->SetTextAlign(13);
  //if (cutvals) {
    //cuts->AddText(Form("%-5s > %4.2f","bdt  ",cutvals[0]));
    //cuts->AddText(Form("%-5s > %4.2f","PIDK ",cutvals[1]));
    //cuts->AddText(Form("%-5s < %4.2f","PID#pi",-1.*cutvals[2]));
    //cuts->AddText(Form("%-5s > %4.2f","ProbNNk ",cutvals[1]));
    //cuts->AddText(Form("%-5s > %4.2f","ProbNNpiKp",cutvals[2]));
  //}

  w->data(Form("Data%s",year.Data()))->plotOn(plot);
  leg->AddEntry(plot->getObject(plot->numItems()-1), "Data", "LEP");

  w->pdf("pdf")->plotOn(plot,Components("bkg_pdf"),LineColor(kGreen+1),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "Combinatorial", "L");

  w->pdf("pdf")->plotOn(plot,Components("part_reco_pdf"),LineColor(kGreen+4),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "Part. reco.", "L");

  w->pdf("pdf")->plotOn(plot,Components("sig_mc_pdf"),LineColor(kRed),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "B_{s}#rightarrow(K^{+}#pi^{-})(K^{-}#pi^{+})", "L");

  w->pdf("pdf")->plotOn(plot,Components("bd_mc_pdf"),LineColor(kMagenta),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "B_{d}#rightarrow(K^{+}#pi^{-})(K^{-}#pi^{+})", "L");

  w->pdf("pdf")->plotOn(plot,Components("rho_mc_pdf"),LineColor(kOrange),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "B_{d}#rightarrow(#pi^{+}#pi^{-})(K^{-}#pi^{+})", "L");

  w->pdf("pdf")->plotOn(plot,Components("phi_mc_pdf"),LineColor(kYellow+1),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "B_{d}#rightarrow(K^{+}K^{-})(K^{-}#pi^{+})", "L");

  w->pdf("pdf")->plotOn(plot,Components("lb_mc_pdf"),LineColor(kViolet+1),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "#Lambda_{b(d)}#rightarrow(p^{+}#pi^{-})(K(#pi)^{-}#pi^{+})", "L");

  w->pdf("pdf")->plotOn(plot);
  leg->AddEntry(plot->getObject(plot->numItems()-1), "Total", "L");

  plot->GetXaxis()->SetTitleOffset(0.8);
  plot->GetYaxis()->SetTitleOffset(0.8);
  plot->GetYaxis()->SetRangeUser(0.,plot->GetMaximum()*1.2);
  plot->Draw();
  leg->Draw("same");
  params->Draw("same");
  //if (cutvals) cuts->Draw("same");
  label->Draw("same");
  c->Update();
  c->Modified();
  c->Print(Form("plots/CutOptimisation/pdf/fit%s.pdf",year.Data()));

  cacheFile->Close();
  delete cacheFile;
}

int main(int argc, char **argv) {

  RooMsgService::instance().setGlobalKillBelow(FATAL);
  RooMsgService::instance().setSilentMode(true);

  gROOT->ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C");
  system("mkdir -p root/CutOptimisation");
  system("mkdir -p plots/CutOptimisation/pdf");

  bool loadFromCache = false;
  TString fname = "root/AnalysisOut.root";
  TString tname = "AnalysisTree";

  vector<TString> years;
  years.push_back("2011");
  years.push_back("2012");

  for (int i=0; i<years.size(); i++) {
    TString year = years[i];

    cout << "YEAR: " << year << endl;

    // need to do the prelim fit to get the idea of numbers
    int nExpSignalEvents     = -1;
    int nExpBackgroundEvents = -1;
    int nExpMisIDEvents      = -1;
    getExpectationFromFit( year, 2, nExpSignalEvents, nExpBackgroundEvents, nExpMisIDEvents );
    plotExpectationFit( year );

    // now do the optimisation
    RunCutOptimisation optimiser( fname, tname );

    // fom types to run
    vector<int> fomTypes;
    fomTypes.push_back(0);
    fomTypes.push_back(1);
    fomTypes.push_back(2);

    // numbers from fit guess
    optimiser.setExpNSignalEvents( nExpSignalEvents );
    optimiser.setExpNBackgroundEvents( nExpBackgroundEvents );
    optimiser.setExpNMisIDEvents( nExpMisIDEvents );

    TString sigString;
    TString bkgString;
    TString misString;

    if ( year=="2011" ) {
      sigString = "(itype==-70)";
      bkgString = "(itype==71) && (B_s0_DTF_B_s0_M>5600 && B_s0_DTF_B_s0_M<5800)";
      misString = "(itype==-77)";
    }
    else if (year=="2012") {
      sigString = "(itype==-80)";
      bkgString = "(itype==81) && (B_s0_DTF_B_s0_M>5600 && B_s0_DTF_B_s0_M<5800)";
      misString = "(itype==-87)";
    }
    else if (year=="all") {
      sigString = "(itype==-70 || itype==-80)";
      bkgString = "(itype==71 || itype==81) && (B_s0_DTF_B_s0_M>5600 && B_s0_DTF_B_s0_M<5800)";
      misString = "(itype==-77 || itype==-87)";
    }
    else {
      cout << "Year: " << year << " not recognised" << endl;
      exit(1);
    }

    optimiser.setSignalSelString    ( sigString );
    optimiser.setBackgroundSelString( bkgString );
    optimiser.setMisIDSelString     ( misString );

    // BDT cut optimisation
    optimiser.optimise( "bdtoutput", fomTypes, 100, -1., 1., "_"+year );

    // PID cut optimisation
    //optimiser.optimise( "min_kaon_ProbNNk_corr", fomTypes, 100, 0, 1, "_"+year );
    //optimiser.optimise( "min_kaon_ProbNNKpi_corr", fomTypes, 100, 0, 1, "_"+year );
    //optimiser.optimise( "min_pion_ProbNNpi_corr", fomTypes, 100, 0, 1, "_"+year );
    //optimiser.optimise( "min_pion_ProbNNpiK_corr", fomTypes, 100, 0, 1, "_"+year );
  }
}

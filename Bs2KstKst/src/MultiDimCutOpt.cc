#include "TROOT.h"
#include "TStyle.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2D.h"
#include "TMath.h"
#include "TCut.h"
#include "TLegend.h"
#include "TPaveText.h"

#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooAbsPdf.h"
#include "RooDataHist.h"
#include "RooAbsReal.h"
#include "RooFitResult.h"

#include "Utils.h"

#include "MultiDimCutOpt.h"

using namespace std;
using namespace RooFit;
using namespace Utils;

Bs2KstKst::MultiDimCutOpt::MultiDimCutOpt():
  runAll(true),
  run2011(false),
  run2012(false),
  misIdType(1),
  n_reducs(0),
  scanpoints1d(10),
  scanpoints2d(10)
{
}

void Bs2KstKst::MultiDimCutOpt::setup(TString _infilename, TString _outfilename, TString _cachefilename) {

  infilename    = _infilename;
  outfilename   = _outfilename;
  cachefilename = _cachefilename;

  outFile = TFile::Open(outfilename,"RECREATE");
  w = new RooWorkspace("w","w");

  gROOT->ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C");

  system("mkdir -p plots/MultiDimCutOpt/png");
  system("mkdir -p plots/MultiDimCutOpt/pdf");
  system("mkdir -p plots/MultiDimCutOpt/C");

}

Bs2KstKst::MultiDimCutOpt::~MultiDimCutOpt(){
}

void Bs2KstKst::MultiDimCutOpt::makeInitialDatasets(){

  w->factory("B_s0_DTF_B_s0_M[5000,5800]");
  w->var("B_s0_DTF_B_s0_M")->setBins(100); // IMPORTANT THIS
  w->factory("bdtoutput[-1.,1.]");
  //w->factory("max_pion_PIDK[-999.,0.]");
  //w->factory("min_kaon_PIDK[0.,999.]");
  w->factory("max_pion_V3ProbNNKpi_corr[0.,1.]");
  w->factory("min_kaon_V3ProbNNKpi_corr[0.,1.]");
  w->factory("itype[-999,999]");

  RooArgSet *observables = new RooArgSet();
  observables->add(*w->var("B_s0_DTF_B_s0_M"));
  observables->add(*w->var("bdtoutput"));
  observables->add(*w->var("max_pion_V3ProbNNKpi_corr"));
  observables->add(*w->var("min_kaon_V3ProbNNKpi_corr"));
  //observables->add(*w->var("max_pion_PIDK"));
  //observables->add(*w->var("min_kaon_PIDK"));
  observables->add(*w->var("itype"));
  w->defineSet("observables",*observables);

  TFile *tempFile = TFile::Open(infilename);
  TTree *inTree = (TTree*)tempFile->Get("AnalysisTree");

  TString data_cut;
  TString sig_mc_cut;
  TString bd_mc_cut;
  TString phi_mc_cut;
  TString rho_mc_cut;
  TString lb_mc_cut;

  if ( runAll || ( run2011 && run2012 ) ) {
    data_cut   = "itype==71 || itype==81";
    sig_mc_cut = "itype==-70 || itype==-80";
    bd_mc_cut  = "itype==-74 || itype==-84";
    phi_mc_cut = "itype==-75 || itype==-85";
    rho_mc_cut = "itype==-77 || itype==-87";
    lb_mc_cut  = "itype==-78 || itype==79 || itype==-88 || itype==-89";
  }
  if ( run2011 && !run2012 ) {
    data_cut   = "itype==71" ;
    sig_mc_cut = "itype==-70";
    bd_mc_cut  = "itype==-74";
    phi_mc_cut = "itype==-75";
    rho_mc_cut = "itype==-77";
    lb_mc_cut  = "itype==-78 || itype==79";
  }
  if ( run2012 && !run2011 ) {

    data_cut   = "itype==81" ;
    sig_mc_cut = "itype==-80";
    bd_mc_cut  = "itype==-84";
    phi_mc_cut = "itype==-85";
    rho_mc_cut = "itype==-87";
    lb_mc_cut  = "itype==-88 || itype==89";
  }

  print( "MultiDimCutOpt::makeInitialDatasets()" , "Making datasets..." );

  RooDataSet *full_data = new RooDataSet("full_data","Full Data",*observables,Import(*inTree),Cut(data_cut));
  w->import(*full_data);
  delete full_data;
  print( "" , "\t Data" );

  RooDataSet *full_sig_mc = new RooDataSet("full_sig_mc","Full Sig MC",*observables,Import(*inTree),Cut(sig_mc_cut));
  w->import(*full_sig_mc);
  delete full_sig_mc;
  print( "" , "\t Sig" );

  RooDataSet *full_bd_mc = new RooDataSet("full_bd_mc","Full Bd->KstKst MC",*observables,Import(*inTree),Cut(bd_mc_cut));
  w->import(*full_bd_mc);
  delete full_bd_mc;
  print( "" , "\t Bd" );

  RooDataSet *full_phi_mc = new RooDataSet("full_phi_mc","Full Bd->RhoKst MC",*observables,Import(*inTree),Cut(phi_mc_cut));
  w->import(*full_phi_mc);
  delete full_phi_mc;
  print( "" , "\t Phi" );

  RooDataSet *full_rho_mc = new RooDataSet("full_rho_mc","Full Bd->RhoKst MC",*observables,Import(*inTree),Cut(rho_mc_cut));
  w->import(*full_rho_mc);
  delete full_rho_mc;
  print( "" , "\t Rho" );

  RooDataSet *full_lb_mc = new RooDataSet("full_lb_mc","Full Bd->RhoKst MC",*observables,Import(*inTree),Cut(lb_mc_cut));
  w->import(*full_lb_mc);
  delete full_lb_mc;
  print( "" , "\t Lb" );

  TString ext="";
  if ( run2011 && !run2012 ) ext="2011";
  if ( run2012 && !run2011 ) ext="2012";
  w->writeToFile(cachefilename);

  w->Print();
}

void Bs2KstKst::MultiDimCutOpt::loadWorkspace(){

  TFile *inFile = TFile::Open(cachefilename);
  w = (RooWorkspace*)( (RooWorkspace*)inFile->Get("w") )->Clone();
  inFile->Close();
}

RooDataSet* Bs2KstKst::MultiDimCutOpt::getDatasetFromWS( RooWorkspace *ws, TString name2011, TString name2012, TString newname )
{

  RooDataSet *ret_data;
  if ( run2011 )      ret_data = (RooDataSet*)ws->data(name2011);
  else if ( run2012 ) ret_data = (RooDataSet*)ws->data(name2012);
  else  {
    ret_data = (RooDataSet*)ws->data(name2011);
    ret_data->append( * (RooDataSet*)ws->data(name2012) );
  }
  ret_data->SetName( newname );
  return ret_data;

}

void Bs2KstKst::MultiDimCutOpt::loadDatasetsFromFile( TString fname ) {

  TFile *loadFile = TFile::Open( fname );
  RooWorkspace *loadWS = (RooWorkspace*)loadFile->Get("w");

  RooDataSet *full_data = getDatasetFromWS( loadWS, "Data2011", "Data2012", "full_data" );
  w->import( *full_data );

  RooDataSet *full_sig_mc = getDatasetFromWS( loadWS, "Bs2Kst0Kst0_MC2011", "Bs2Kst0Kst0_MC2012", "full_sig_mc" );
  w->import( *full_sig_mc );

  RooDataSet *full_bd_mc = getDatasetFromWS( loadWS, "Bd2Kst0Kst0_MC2011", "Bd2Kst0Kst0_MC2012", "full_bd_mc" );
  w->import( *full_bd_mc );

  RooDataSet *full_phi_mc = getDatasetFromWS( loadWS, "Bs2PhiKst0_MC2011", "Bs2PhiKst0_MC2012", "full_phi_mc" );
  w->import( *full_phi_mc );

  RooDataSet *full_rho_mc = getDatasetFromWS( loadWS, "Bd2RhoKst0_MC2011", "Bd2RhoKst0_MC2012", "full_rho_mc" );
  w->import( *full_rho_mc );

  RooDataSet *full_lb_mc_ppi = getDatasetFromWS( loadWS, "Lb2ppipipi_MC2011", "Lb2ppipipi_MC2012", "full_lb_mc_ppi" );
  RooDataSet *full_lb_mc_pk = getDatasetFromWS( loadWS, "Lb2pKpipi_MC2011",  "Lb2pKpipi_MC2012",  "full_lb_mc_pk" );

  full_lb_mc_ppi->append( *full_lb_mc_pk );
  full_lb_mc_ppi->SetName("full_lb_mc");
  w->import( *full_lb_mc_ppi );

  loadFile->Close();
}

RooPlot* Bs2KstKst::MultiDimCutOpt::getSimplePlot(TString var, TString data, TString pdf){

  RooPlot *plot = w->var(var)->frame();
  if (w->data(data)) w->data(data)->plotOn(plot);
  if (w->pdf(pdf))   w->pdf(pdf)->plotOn(plot);
  return plot;
}

void Bs2KstKst::MultiDimCutOpt::fitAndSetConst(TString pdf, TString data) {

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

void Bs2KstKst::MultiDimCutOpt::makePDFs(){

  print( "MultiDimCutOpt::makePDFs()" , "Making pdfs..." );

  w->factory("sig_mc_mean[5350,5450]");
  w->factory("CBShape::sig_mc_cb1( B_s0_DTF_B_s0_M, sig_mc_mean, sig_mc_sigma1[20,5,200], sig_mc_alpha_1[0.5,0.,5], sig_mc_n_1[0.5,0.,10] )");
  w->factory("CBShape::sig_mc_cb2( B_s0_DTF_B_s0_M, sig_mc_mean, sig_mc_sigma2[20,5,200], sig_mc_alpha_2[-0.5,-5.,0], sig_mc_n_2[0.5,0.,10] )");
  w->factory("SUM::sig_mc_pdf( sig_mc_f1[0.6,0.,1.]*sig_mc_cb1, sig_mc_cb2 )");
  w->pdf("sig_mc_pdf")->fitTo(*w->data("full_sig_mc"));

  w->factory("bd_mc_mean[5200,5350]");
  w->factory("CBShape::bd_mc_cb1( B_s0_DTF_B_s0_M, bd_mc_mean, bd_mc_sigma1[20,10,200], bd_mc_alpha_1[0.5,0.,5], bd_mc_n_1[0.5,0.,10] )");
  w->factory("CBShape::bd_mc_cb2( B_s0_DTF_B_s0_M, bd_mc_mean, bd_mc_sigma2[20,10,200], bd_mc_alpha_2[-0.5,-5.,0], bd_mc_n_2[0.5,0.,10] )");
  w->factory("SUM::bd_mc_pdf( bd_mc_f1[0.6,0.,1.]*bd_mc_cb1, bd_mc_cb2 )");
  w->pdf("bd_mc_pdf")->fitTo(*w->data("full_bd_mc"));

  w->factory("rho_mc_mean[5250,5350]");
  w->factory("CBShape::rho_mc_cb1( B_s0_DTF_B_s0_M, rho_mc_mean, rho_mc_sigma1[20,10,200], rho_mc_alpha1[0.5,0.,5.], rho_mc_n1[0.01,0.,40.])");
  w->factory("CBShape::rho_mc_cb2( B_s0_DTF_B_s0_M, rho_mc_mean, rho_mc_sigma2[20,10,200], rho_mc_alpha2[-0.5,-5.,0.], rho_mc_n2[0.01,0.,40.])");
  w->factory("SUM::rho_mc_pdf( rho_mc_f1[0.2,0.,1.]*rho_mc_cb1, rho_mc_cb2)");
  w->pdf("rho_mc_pdf")->fitTo(*w->data("full_rho_mc"));

  w->factory("phi_mc_mean[5100,5350]");
  w->factory("CBShape::phi_mc_cb1( B_s0_DTF_B_s0_M, phi_mc_mean, phi_mc_sigma1[100,0,400], phi_mc_alpha1[0.5,0.,5.], phi_mc_n1[0.01,0.,100.])");
  w->factory("CBShape::phi_mc_cb2( B_s0_DTF_B_s0_M, phi_mc_mean, phi_mc_sigma2[100,0,400], phi_mc_alpha2[-0.5,-5.,0.], phi_mc_n2[0.1,0.,100.])");
  w->factory("SUM::phi_mc_pdf( phi_mc_f1[0.6,0.,1.]*phi_mc_cb1, phi_mc_cb2 )");
  w->pdf("phi_mc_pdf")->fitTo(*w->data("full_phi_mc"));

  w->factory("lb_mc_mean[5300,5650]");
  w->factory("CBShape::lb_mc_cb1( B_s0_DTF_B_s0_M, lb_mc_mean, lb_mc_sigma1[100,0,400], lb_mc_alpha1[0.5,0.,5.], lb_mc_n1[0.01,0.,100.])");
  w->factory("CBShape::lb_mc_cb2( B_s0_DTF_B_s0_M, lb_mc_mean, lb_mc_sigma2[100,0,400], lb_mc_alpha2[0.5,0.,5.], lb_mc_n2[0.1,0.,100.])");
  w->factory("SUM::lb_mc_pdf( lb_mc_f1[0.6,0.,1.]*lb_mc_cb1, lb_mc_cb2 )");
  w->pdf("lb_mc_pdf")->fitTo(*w->data("full_lb_mc"));

  fitAndSetConst("sig_mc_pdf","full_sig_mc");
  fitAndSetConst("bd_mc_pdf","full_bd_mc");
  fitAndSetConst("rho_mc_pdf","full_rho_mc");
  fitAndSetConst("phi_mc_pdf","full_phi_mc");
  fitAndSetConst("lb_mc_pdf","full_lb_mc");

  // background and total pdf
  w->factory("ArgusBG::part_reco_pdf( B_s0_DTF_B_s0_M, part_reco_m0[5226], part_reco_c[-10,-100.,0.], part_reco_p[0.4,0.,1.] )");
  w->factory("Exponential::bkg_pdf( B_s0_DTF_B_s0_M, bkg_exp_p1[-0.02,-0.04,0.] )");

  w->factory("SUM::pdf( bkg_y[0,200e3]*bkg_pdf, part_reco_y[0,200e3]*part_reco_pdf, sig_mc_y[0,100e3]*sig_mc_pdf, bd_mc_y[0,10e3]*bd_mc_pdf, rho_mc_y[0,50e3]*rho_mc_pdf, phi_mc_y[0,50e3]*phi_mc_pdf, lb_mc_y[0,50e3]*lb_mc_pdf)");

  w->var("sig_mc_mean")->setConstant(false);

  RooFitResult *fitRes = w->pdf("pdf")->fitTo(*w->data("full_data"),Save());
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

  TString ext="";
  if ( run2011 && !run2012 ) ext="2011";
  if ( run2012 && !run2011 ) ext="2012";

    w->writeToFile(cachefilename);

}

void Bs2KstKst::MultiDimCutOpt::plotShapes(){

  TCanvas *canv = new TCanvas("shapes","shapes",1600,1200);
  canv->Divide(3,2);
  RooPlot* sig_mc_plot = getSimplePlot("B_s0_DTF_B_s0_M","full_sig_mc","sig_mc_pdf");
  RooPlot *bd_mc_plot  = getSimplePlot("B_s0_DTF_B_s0_M","full_bd_mc","bd_mc_pdf");
  RooPlot *rho_mc_plot = getSimplePlot("B_s0_DTF_B_s0_M","full_rho_mc","rho_mc_pdf");
  RooPlot *phi_mc_plot = getSimplePlot("B_s0_DTF_B_s0_M","full_phi_mc","phi_mc_pdf");
  RooPlot *lb_mc_plot = getSimplePlot("B_s0_DTF_B_s0_M","full_lb_mc","lb_mc_pdf");

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

  if ( runAll || ( run2011 && run2012 ) ) {
    canv->Print("plots/MultiDimCutOpt/pdf/shapes_all.pdf");
    canv->Print("plots/MultiDimCutOpt/png/shapes_all.png");
    canv->Print("plots/MultiDimCutOpt/C/shapes_all.C");
  }
  if ( run2011 && !run2012 ) {
    canv->Print("plots/MultiDimCutOpt/pdf/shapes_2011.pdf");
    canv->Print("plots/MultiDimCutOpt/png/shapes_2011.png");
    canv->Print("plots/MultiDimCutOpt/C/shapes_2011.C");
  }
  if ( run2012 && !run2011 ) {
    canv->Print("plots/MultiDimCutOpt/pdf/shapes_2012.pdf");
    canv->Print("plots/MultiDimCutOpt/png/shapes_2012.png");
    canv->Print("plots/MultiDimCutOpt/C/shapes_2012.C");
  }
}

void Bs2KstKst::MultiDimCutOpt::save(){

  outFile->cd();
  w->Write();
  outFile->Close();
}

void Bs2KstKst::MultiDimCutOpt::plot(RooAbsData *data, RooAbsPdf *pdf, TString ext, double *cutvals){

  TCanvas *canv = new TCanvas();
  RooPlot *plot = w->var("B_s0_DTF_B_s0_M")->frame();
  TLegend *leg = new TLegend(0.65,0.5,0.92,0.92);
  TPaveText *label = new TPaveText(0.2,0.84,0.4,0.9,"ndc");
  label->SetFillColor(0);
  label->SetShadowColor(0);
  label->SetLineWidth(1);
  label->SetLineColor(1);
  label->SetTextSize(0.08);
  label->SetTextAlign(11);
  if ( runAll || (run2011 && run2012) ) label->AddText("2011+2012");
  if ( run2011 && !run2012 ) label->AddText("2011");
  if ( run2012 && !run2011 ) label->AddText("2012");
  TPaveText *params = new TPaveText(0.24,0.7,0.4,0.84,"ndc");
  params->SetFillColor(0);
  params->SetShadowColor(0);
  params->SetLineWidth(0);
  params->SetTextSize(0.03);
  params->SetTextAlign(11);
  params->AddText(Form("%-5s = %6.2f #pm %6.2f","N_{sig}",w->var("sig_mc_y")->getVal(),w->var("sig_mc_y")->getError()));
  params->AddText(Form("%-5s = %6.2f #pm %6.2f","N_{bkg}",w->var("bkg_y")->getVal(),w->var("bkg_y")->getError()));
  params->AddText(Form("%-5s = %6.2f #pm %6.2f","N_{mis}",w->function("misid_mc_y")->getVal(),w->function("misid_mc_y")->getPropagatedError(*((RooFitResult*)w->obj("full_data_fit_res")))));
  TPaveText *cuts = new TPaveText(0.3,0.56,0.46,0.7,"ndc");
  cuts->SetFillColor(0);
  cuts->SetShadowColor(0);
  cuts->SetLineWidth(0);
  cuts->SetTextSize(0.03);
  cuts->SetTextAlign(13);
  if (cutvals) {
    cuts->AddText(Form("%-5s > %4.2f","bdt  ",cutvals[0]));
    //cuts->AddText(Form("%-5s > %4.2f","PIDK ",cutvals[1]));
    //cuts->AddText(Form("%-5s < %4.2f","PID#pi",-1.*cutvals[2]));
    cuts->AddText(Form("%-5s > %4.2f","ProbNNk ",cutvals[1]));
    cuts->AddText(Form("%-5s > %4.2f","ProbNNpiKp",cutvals[2]));
  }

  data->plotOn(plot);
  leg->AddEntry(plot->getObject(plot->numItems()-1), "Data", "LEP");

  pdf->plotOn(plot,Components("bkg_pdf"),LineColor(kGreen+1),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "Combinatorial", "L");

  pdf->plotOn(plot,Components("part_reco_pdf"),LineColor(kGreen+4),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "Part. reco.", "L");

  pdf->plotOn(plot,Components("sig_mc_pdf"),LineColor(kRed),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "B_{s}#rightarrow(K^{+}#pi^{-})(K^{-}#pi^{+})", "L");

  pdf->plotOn(plot,Components("bd_mc_pdf"),LineColor(kMagenta),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "B_{d}#rightarrow(K^{+}#pi^{-})(K^{-}#pi^{+})", "L");

  pdf->plotOn(plot,Components("rho_mc_pdf"),LineColor(kOrange),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "B_{d}#rightarrow(#pi^{+}#pi^{-})(K^{-}#pi^{+})", "L");

  pdf->plotOn(plot,Components("phi_mc_pdf"),LineColor(kYellow+1),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "B_{d}#rightarrow(K^{+}K^{-})(K^{-}#pi^{+})", "L");

  pdf->plotOn(plot,Components("lb_mc_pdf"),LineColor(kViolet+1),LineStyle(kDashed));
  leg->AddEntry(plot->getObject(plot->numItems()-1), "#Lambda_{b(d)}#rightarrow(p^{+}#pi^{-})(K(#pi)^{-}#pi^{+})", "L");

  pdf->plotOn(plot);
  leg->AddEntry(plot->getObject(plot->numItems()-1), "Total", "L");

  plot->GetXaxis()->SetTitleOffset(0.8);
  plot->GetYaxis()->SetTitleOffset(0.8);
  plot->GetYaxis()->SetRangeUser(0.,plot->GetMaximum()*1.2);
  plot->Draw();
  leg->Draw("same");
  params->Draw("same");
  if (cutvals) cuts->Draw("same");
  label->Draw("same");
  canv->Update();
  canv->Modified();
  if ( runAll || ( run2011 && run2012 ) ) {
    canv->Print(Form("plots/MultiDimCutOpt/pdf/fit%s_all.pdf",ext.Data()));
    canv->Print(Form("plots/MultiDimCutOpt/png/fit%s_all.png",ext.Data()));
    canv->Print(Form("plots/MultiDimCutOpt/C/fit%s_all.C",ext.Data()));
  }
  if ( run2011 && !run2012 ) {
    canv->Print(Form("plots/MultiDimCutOpt/pdf/fit%s_2011.pdf",ext.Data()));
    canv->Print(Form("plots/MultiDimCutOpt/png/fit%s_2011.png",ext.Data()));
    canv->Print(Form("plots/MultiDimCutOpt/C/fit%s_2011.C",ext.Data()));
  }
  if ( run2012 && !run2011 ) {
    canv->Print(Form("plots/MultiDimCutOpt/pdf/fit%s_2012.pdf",ext.Data()));
    canv->Print(Form("plots/MultiDimCutOpt/png/fit%s_2012.png",ext.Data()));
    canv->Print(Form("plots/MultiDimCutOpt/C/fit%s_2012.C",ext.Data()));
  }

}

double Bs2KstKst::MultiDimCutOpt::fitAndReduce(const double *cutvs) {

  RooMsgService::instance().setGlobalKillBelow(FATAL);
  RooMsgService::instance().setSilentMode(true);

  double bdtoutput_cut = cutvs[0];
  double min_kaon_V3ProbNNKpi_corr_cut = cutvs[1];
  double max_pion_V3ProbNNKpi_corr_cut = cutvs[2];
  TString cutString = Form(" ( (bdtoutput>%f) && (min_kaon_V3ProbNNKpi_corr>%f) && (max_pion_V3ProbNNKpi_corr<%f) )",bdtoutput_cut,min_kaon_V3ProbNNKpi_corr_cut,max_pion_V3ProbNNKpi_corr_cut);
  //double min_kaon_PIDK_cut = cutvs[1];
  //double max_pion_PIDK_cut = cutvs[2];
  //TString cutString = Form(" ( (bdtoutput>%f) && (min_kaon_PIDK>%f) && (max_pion_PIDK<%f) )",bdtoutput_cut,min_kaon_PIDK_cut,max_pion_PIDK_cut);
  return fitAndReduce(cutString);
}

double Bs2KstKst::MultiDimCutOpt::fitAndReduce(TString cut){

  RooDataHist *dat = reduce(cut);
  RooAbsPdf *pdf = (RooAbsPdf*)w->pdf("pdf");
  pdf->fitTo(*dat);
  //plot(dat,pdf);

  double nsig = w->var("sig_mc_y")->getVal();
  double nbkg = w->var("bkg_y")->getVal();
  double nrho = w->var("rho_mc_y")->getVal();

  double fom = -1.*(nsig/(TMath::Sqrt(nsig+nbkg)) * (nsig/nrho));
  return fom;
}

void Bs2KstKst::MultiDimCutOpt::run(){


  //double start_vals[3] = {-0.3, 2., -2.};
  double start_vals[3] = {-0.3, 0., 0.};
  double test_vals[3] = { start_vals[0], start_vals[1], start_vals[2] };
  double prev_vals[3] = { start_vals[0], start_vals[1], start_vals[2] };

  double step_vals[3] = {0.2, 0.1, 0.1};

  double threshold = 5.;
  double diff = threshold*2.;

  double prev_fom = 0.;

  std::vector<std::vector<double> > result;

  // just do 1D for now
  int p=0;
  while ( diff>threshold ){
    double fom = fitAndReduce(test_vals);
    vector<double> res;
    res.push_back(test_vals[0]);
    res.push_back(test_vals[1]);
    res.push_back(test_vals[2]);
    res.push_back(fom);
    result.push_back(res);

    cout << Form("%-5d",p) << Form(" (%3.1f,%3.1f,%3.1f) ",test_vals[0],test_vals[1],test_vals[2]) << " -- " << Form("%6.3f",fom) << endl;

    int direction;
    if ( test_vals[0] < prev_vals[0] ) {
      direction = -1;
    }
    else {
      direction = 1;
    }

    // if theres a switch
    if ( fom > prev_fom ) {
      direction *= -1;
      step_vals[0] *= 0.25;
    }
    test_vals[0] += direction*step_vals[0];
    diff = TMath::Abs( fom - prev_fom );
    prev_fom = fom;
    prev_vals[0] = test_vals[0];
    prev_vals[1] = test_vals[1];
    prev_vals[2] = test_vals[2];
    p++;
  }
  //
  //
  TGraph *gr = new TGraph();
  std::sort(result.begin(),result.end());
  for (unsigned int p=0; p<result.size(); p++){
    gr->SetPoint(p,result[p][0],result[p][3]);
  }
  TCanvas *canv = new TCanvas();
  gr->Draw("ALP");
  canv->Update();
  canv->Modified();
  canv->Print("plots/MultiDimCutOpt/pdf/min.pdf");
}

RooDataHist* Bs2KstKst::MultiDimCutOpt::reduce(TString cut_expr){

  n_reducs++;

  RooDataSet *full = (RooDataSet*)w->data("full_data");
  RooDataSet *reduced = (RooDataSet*)full->reduce(SelectVars(RooArgSet(*w->var("B_s0_DTF_B_s0_M"))),Cut(cut_expr.Data()),Name(Form("dset%d",n_reducs)));
  RooDataHist *binned = reduced->binnedClone(Form("dhist%d",n_reducs));
  delete reduced;
  return binned;
}

void Bs2KstKst::MultiDimCutOpt::getEventEstimates(TTree *tree) {

  //w->var("B_s0_DTF_B_s0_M")->setBins(100);
  //RooDataSet *data = (RooDataSet*)w->data("full_data")->reduce(SelectVars(RooArgSet(*w->var("B_s0_DTF_B_s0_M"))),Name("full_data_red"));
  //RooDataHist *binned = (RooDataHist*)data->binnedClone("full_data_hist");
  //w->var("sig_mc_mean")->setConstant(false);
  //w->pdf("pdf")->fitTo(*binned);
  double cut_vals[3] = {-1.0,0.0,0.0};
  w->Print("v");
  plot(w->data("full_data"),w->pdf("pdf"),"full",cut_vals);
  w->var("B_s0_DTF_B_s0_M")->setRange("window",5300,5500);
  RooAbsReal *integral = w->pdf("bkg_pdf")->createIntegral(RooArgSet(*w->var("B_s0_DTF_B_s0_M")),NormSet(RooArgSet(*w->var("B_s0_DTF_B_s0_M"))),Range("window"));
  cout << "INTEGRAL: " << integral->getVal() << " -- " << integral->getVal()*w->var("bkg_y")->getVal() << endl;

  double sigEst = w->var("sig_mc_y")->getVal();
  double bkgEst = w->var("bkg_y")->getVal()*integral->getVal();
  double misEst = w->function("misid_mc_y")->getVal();

  double sigTree = tree->GetEntries(getSigSel());
  double bkgTree = tree->GetEntries(getBkgSel());
  double misTree = tree->GetEntries(getMisSel());

  sigNorm = sigEst / sigTree;
  bkgNorm = bkgEst / bkgTree;
  misNorm = misEst / misTree;

}

TCut Bs2KstKst::MultiDimCutOpt::getCut(double *cutvals) {

  double bdtoutput_cut = cutvals[0];
  double min_kaon_V3ProbNNKpi_corr_cut = cutvals[1];
  double max_pion_V3ProbNNKpi_corr_cut = cutvals[2];
  TString cutString = Form(" ( (bdtoutput>%f) && (min_kaon_V3ProbNNKpi_corr>%f) && (max_pion_V3ProbNNKpi_corr<%f) )",bdtoutput_cut,min_kaon_V3ProbNNKpi_corr_cut,max_pion_V3ProbNNKpi_corr_cut);

  //double min_kaon_PIDK_cut = cutvals[1];
  //double min_pion_PIDK_cut = cutvals[2];
  //TString cutString = Form(" ( (bdtoutput>%f) && (min_kaon_PIDK>%f) && (max_pion_PIDK<%f) )",bdtoutput_cut,min_kaon_PIDK_cut,-1.*min_pion_PIDK_cut);

  TCut cut = TCut(cutString);
  return cut;
}

TCut Bs2KstKst::MultiDimCutOpt::getBkgSel(){

  if ( runAll || ( run2011 && run2012 ) ) {
    return TCut("( (itype==71 || itype==81) && (B_s0_DTF_B_s0_M>5600) )");
  }
  if ( run2011 && !run2012 ) {
    return TCut("( (itype==71) && (B_s0_DTF_B_s0_M>5600) )");
  }
  if ( run2012 && !run2011 ) {
    return TCut("( (itype==81) && (B_s0_DTF_B_s0_M>5600) )");
  }
  cout << "ERROR -- This shouldn't be possible" << endl;
  exit(1);
  return TCut("");
}

TCut Bs2KstKst::MultiDimCutOpt::getSigSel(){
  if ( runAll || ( run2011 && run2012 ) ) {
    return TCut("( (itype==-71 || itype==-81) )");
  }
  if ( run2011 && !run2012 ) {
    return TCut("( (itype==-71) )");
  }
  if ( run2012 && !run2011 ) {
    return TCut("( (itype==-81) )");
  }
  cout << "ERROR -- This shouldn't be possible" << endl;
  exit(1);
  return TCut("");
}

TCut Bs2KstKst::MultiDimCutOpt::getMisSel(){
  if ( runAll || ( run2011 && run2012 ) ) {
    if ( misIdType == 1 ) return TCut("( (itype==-76 || itype==-86 || itype==-77 || itype==-87 || itype==-78 || itype==-88 || itype==-79 || itype==-89) )");
    if ( misIdType == 2 ) return TCut("( (itype==-77 || itype==-87) )");
    if ( misIdType == 3 ) return TCut("( (itype==-76 || itype==-86) )");
    if ( misIdType == 4 ) return TCut("( (itype==-78 || itype==-88 || itype==-79 || itype==-89) )");
    if ( misIdType == 5 ) return TCut("( (itype==-76 || itype==-86 || itype==-77 || itype==-87) )");
    return TCut("( (itype==-77 || itype==-87) )");
  }
  if ( run2011 && !run2012 ) {
    if ( misIdType == 1 ) return TCut("( (itype==-76 || itype==-77 || itype==-78 || itype==-79) )");
    if ( misIdType == 2 ) return TCut("( (itype==-77) )");
    if ( misIdType == 3 ) return TCut("( (itype==-76) )");
    if ( misIdType == 4 ) return TCut("( (itype==-78 || itype==-79) )");
    if ( misIdType == 5 ) return TCut("( (itype==-76 || itype==-77) )");
    return TCut("( (itype==-77) )");
  }
  if ( run2012 && !run2011 ) {
    if ( misIdType == 1 ) return TCut("( (itype==-86 || itype==-87 || itype==-88 || itype==-89) )");
    if ( misIdType == 2 ) return TCut("( (itype==-87) )");
    if ( misIdType == 3 ) return TCut("( (itype==-86) )");
    if ( misIdType == 4 ) return TCut("( (itype==-88 || itype==-89) )");
    if ( misIdType == 5 ) return TCut("( (itype==-86 || itype==-87) )");
    return TCut("( (itype==-87) )");
  }
  cout << "ERROR -- This shouldn't be possible" << endl;
  exit(1);
  return TCut("");
}

double Bs2KstKst::MultiDimCutOpt::getFOM(double *cutvals) {

  TCut cutString = getCut(cutvals);
  TCut sigSel = getSigSel();
  TCut bkgSel = getBkgSel();
  TCut misSel = getMisSel();

  double nSig = tree->GetEntries(cutString+sigSel) * sigNorm;
  double nBkg = tree->GetEntries(cutString+bkgSel) * bkgNorm;
  double nMis = tree->GetEntries(cutString+misSel) * misNorm;

  double fom = -999.;
  double sigmaMis = 0.5;

  if (fomType==0)        fom = nSig/(TMath::Sqrt(nSig+nBkg));                                   // S/sqrt(S+B)
  else if (fomType==1)   fom = nSig/(TMath::Sqrt(nSig+nBkg+nMis));                              // S/sqrt(S+B+M)
  else if (fomType==2)   fom = nSig/(TMath::Sqrt(nSig+nBkg+(1.+sigmaMis*sigmaMis)*nMis));       // S/sqrt(S+B+M+systM)
  else if (fomType==3)   fom = nSig/(TMath::Sqrt(nSig+nBkg+(1.+2.*sigmaMis*sigmaMis)*nMis));    // S/sqrt(S+B+M+2systM)
  else if (fomType==4)   fom = (nSig/(TMath::Sqrt(nSig+nBkg+nMis)) * (nSig/(nSig+nBkg+nMis)));  // S/sqrt(S+B+M) * Purity
  if (fom != fom) fom=0; // nan protect

  cout << "Scan: " << Form("( %4.2f , %4.2f , %4.2f )",cutvals[0],cutvals[1],cutvals[2]) << " -- " << Form("%6.3f",fom) << endl;
  return fom;
}

Bs2KstKst::Scan1DResult Bs2KstKst::MultiDimCutOpt::scan1D( int varInd, double lowVal, double highVal, int npoints, double *cutvals ){
  TGraph *gr = new TGraph();
  double valAtFOMmax=-999.;
  double fomMax=-999.;
  double step_size = (highVal-lowVal)/npoints;
  for (int p=0; p<npoints; p++){
    double val = lowVal + (p*step_size);
    cutvals[varInd] = val;
    double fom = getFOM(cutvals);
    gr->SetPoint(p,val,fom);
    if (fom > fomMax) {
      valAtFOMmax = val;
      fomMax = fom;
    }
  }
  return Scan1DResult(gr, valAtFOMmax, fomMax);

}

Bs2KstKst::Scan2DResult Bs2KstKst::MultiDimCutOpt::scan2D( int varInd1, int varInd2, double lowVal1, double highVal1, int npoints1, double lowVal2, double highVal2, int npoints2, double *cutvals ) {

  TH2D *pidH = new TH2D("h","h",npoints1,lowVal1,highVal1,npoints2,lowVal2,highVal2);
  pidH->SetDirectory(0);
  for (int xbin=1; xbin<=pidH->GetNbinsX(); xbin++){
    for (int ybin=1; ybin<=pidH->GetNbinsY(); ybin++){
      double xval = pidH->GetXaxis()->GetBinLowEdge(xbin);
      double yval = pidH->GetYaxis()->GetBinLowEdge(ybin);
      cutvals[varInd1] = xval;
      cutvals[varInd2] = yval;
      double fom = getFOM(cutvals);
      pidH->SetBinContent(xbin,ybin,fom);
      pidH->Draw("colz");
      gPad->Update();
      gPad->Modified();
    }
  }
  // find max
  int xbin_opt;
  int ybin_opt;
  int zbin_opt;
  pidH->GetBinXYZ(pidH->GetMaximumBin(),xbin_opt,ybin_opt,zbin_opt);
  double opt1 = pidH->GetXaxis()->GetBinLowEdge(xbin_opt);
  double opt2 = pidH->GetYaxis()->GetBinLowEdge(ybin_opt);
  return Scan2DResult( pidH, opt1, opt2, pidH->GetMaximum() );


}

void Bs2KstKst::MultiDimCutOpt::runSimple(int type) {

  // figure of merit type:
  // // 1 = S/sqrt(S+B)
  // // 2 = S/sqrt(S+B+M)
  // // 3 = S/sqrt(S+B+sigma*M)
  fomType = type;

  TFile *inFile = TFile::Open(infilename);
  tree = (TTree*)inFile->Get("AnalysisTree");
  TCanvas *canv = new TCanvas("opt","opt",1200,600);
  canv->Divide(2,1);

  // fit data with no cuts and compare to tree values to get expected number of each type
  getEventEstimates(tree);

  double cut_vals[3] = { 0.0, 0., 0.};

  // 1D scan and draw
  Scan1DResult scan1dRes = scan1D( 0, -1., 1., scanpoints1d, cut_vals );
  canv->cd(1);
  gPad->SetRightMargin(0.15);
  setAxes(scan1dRes.gr, "bdtoutput cut", "FOM");
  scan1dRes.gr->Draw("ALP");
  canv->Update();
  canv->Modified();

  cout << "Running PID scan at bdtoutput cut value = " << scan1dRes.maxX << endl;

  gStyle->SetPalette(1);

  cut_vals[0] = scan1dRes.maxX;

  // 2D scan and draw
  canv->cd(2);
  gPad->SetLeftMargin(0.05);
  gPad->SetRightMargin(0.2);
  Scan2DResult scan2dRes = scan2D( 1, 2, 0., 1. , scanpoints2d, 0., 1., scanpoints2d, cut_vals);
  setAxes(scan2dRes.hist, "min_kaon_DLLK cut", "min_pion_DLLK cut", "FOM");
  scan2dRes.hist->Draw("colz");
  canv->Update();
  canv->Modified();

  // redo 1D scan as check
  cut_vals[1] = scan2dRes.maxX1;
  cut_vals[2] = scan2dRes.maxX2;
  Scan1DResult scan1dResCheck = scan1D( 0, -1., 1., scanpoints1d, cut_vals );
  canv->cd(1);
  setAxes(scan1dResCheck.gr, "bdtoutput cut", "FOM");
  scan1dResCheck.gr->SetLineColor(kRed);
  scan1dResCheck.gr->SetMarkerColor(kRed);
  scan1dRes.gr->GetYaxis()->SetRangeUser(0.,TMath::Max(scan1dRes.maxY,scan1dResCheck.maxY)*1.2);
  scan1dResCheck.gr->Draw("LPsame");
  TLegend *leg = new TLegend(0.16,0.75,0.42,0.92);
  leg->SetFillColor(0);
  leg->SetTextSize(0.04);
  leg->AddEntry(scan1dRes.gr,"Before PID Cuts","LP");
  leg->AddEntry(scan1dResCheck.gr,"After PID Cuts","LP");
  leg->Draw("same");
  canv->Update();
  canv->Modified();

  double opts_vals[3] = { scan1dResCheck.maxX, scan2dRes.maxX1, scan2dRes.maxX2 };

  cout << "Suggested values: " << endl;
  cout << "\t" << "bdtoutput:     " << opts_vals[0] << endl;
  cout << "\t" << "min_kaon_V3ProbNNKpi_corr: " << opts_vals[1] << endl;
  cout << "\t" << "max_pion_V3ProbNNKpi_corr: " << opts_vals[2] << endl;
  //cout << "\t" << "min_kaon_DLLK: " << opts_vals[1] << endl;
  //cout << "\t" << "min_pion_DLLK: " << opts_vals[2] << endl;

  if ( runAll || ( run2011 && run2012 ) ) {
    canv->Print(Form("plots/MultiDimCutOpt/pdf/scan%d_all.pdf",fomType));
    canv->Print(Form("plots/MultiDimCutOpt/png/scan%d_all.png",fomType));
    canv->Print(Form("plots/MultiDimCutOpt/C/scan%d_all.C",fomType));
  }
  if ( run2011 && !run2012 ) {
    canv->Print(Form("plots/MultiDimCutOpt/pdf/scan%d_2011.pdf",fomType));
    canv->Print(Form("plots/MultiDimCutOpt/png/scan%d_2011.png",fomType));
    canv->Print(Form("plots/MultiDimCutOpt/C/scan%d_2011.C",fomType));
  }
  if ( run2012 && !run2011 ) {
    canv->Print(Form("plots/MultiDimCutOpt/pdf/scan%d_2012.pdf",fomType));
    canv->Print(Form("plots/MultiDimCutOpt/png/scan%d_2012.png",fomType));
    canv->Print(Form("plots/MultiDimCutOpt/C/scan%d_2012.C",fomType));
  }

  scan1dRes.gr->SetName(Form("FirstScan1D%d",fomType));
  scan1dResCheck.gr->SetName(Form("SecondScan1D%d",fomType));
  scan2dRes.hist->SetName(Form("Scan2D%d",fomType));
  outFile->cd();
  scan1dRes.gr->Write();
  scan1dResCheck.gr->Write();
  scan2dRes.hist->Write();

  // plot resulting decision
  //RooDataSet *data = (RooDataSet*)w->data("full_data")->reduce(SelectVars(RooArgSet(*w->var("B_s0_DTF_B_s0_M"))),Name("result_data_red"),Cut(Form("bdtoutput>%4.2f && min_kaon_PIDK>%5.3f && max_pion_PIDK<%5.3f",opts_vals[0],opts_vals[1],opts_vals[2])));
  RooDataSet *data = (RooDataSet*)w->data("full_data")->reduce(SelectVars(RooArgSet(*w->var("B_s0_DTF_B_s0_M"))),Name("result_data_red"),Cut(Form("bdtoutput>%4.2f && min_kaon_V3ProbNNKpi_corr>%5.3f && max_pion_V3ProbNNKpi_corr<%5.3f",opts_vals[0],opts_vals[1],opts_vals[2])));
  RooDataHist *binned = (RooDataHist*)data->binnedClone("result_data_hist");
  w->var("sig_mc_mean")->setConstant(false);
  w->pdf("pdf")->fitTo(*binned);
  plot(binned,w->pdf("pdf"),Form("res%d",fomType),opts_vals);

  inFile->Close();

}

void Bs2KstKst::MultiDimCutOpt::drawAtCut(TString name, double bdt_cut, double kaon_cut, double pion_cut) {

  if (pion_cut>0) pion_cut*=-1;

  //RooDataSet *data = (RooDataSet*)w->data("full_data")->reduce(SelectVars(RooArgSet(*w->var("B_s0_DTF_B_s0_M"))),Name(Form("%s_data_red",name.Data())),Cut(Form("bdtoutput>%4.2f && min_kaon_PIDK>%5.3f && max_pion_PIDK<%5.3f",bdt_cut,kaon_cut,pion_cut)));
  RooDataSet *data = (RooDataSet*)w->data("full_data")->reduce(SelectVars(RooArgSet(*w->var("B_s0_DTF_B_s0_M"))),Name(Form("%s_data_red",name.Data())),Cut(Form("bdtoutput>%4.2f && min_kaon_V3ProbNNKpi_corr>%5.3f && max_pion_V3ProbNNKpi_corr<%5.3f",bdt_cut,kaon_cut,pion_cut)));
  RooDataHist *binned = (RooDataHist*)data->binnedClone(Form("%s_data_hist",name.Data()));
  w->var("sig_mc_mean")->setConstant(false);
  w->pdf("pdf")->fitTo(*binned);
  double cvals[3] = { bdt_cut, kaon_cut, -1.*pion_cut };
  plot(binned,w->pdf("pdf"),name.Data(),cvals);


}

void Bs2KstKst::MultiDimCutOpt::setAxes(TGraph *obj, TString xtitle, TString ytitle){

  obj->GetXaxis()->SetTitle(xtitle);
  obj->GetXaxis()->SetTitleSize(0.8*obj->GetXaxis()->GetTitleSize());
  obj->GetXaxis()->SetTitleOffset(0.8*obj->GetXaxis()->GetTitleOffset());
  obj->GetYaxis()->SetTitle(ytitle);
  obj->GetYaxis()->SetTitleSize(0.8*obj->GetYaxis()->GetTitleSize());
  obj->GetYaxis()->SetTitleOffset(0.8*obj->GetYaxis()->GetTitleOffset());

}

void Bs2KstKst::MultiDimCutOpt::setAxes(TH1 *obj, TString xtitle, TString ytitle){

  obj->GetXaxis()->SetTitle(xtitle);
  obj->GetXaxis()->SetTitleSize(0.8*obj->GetXaxis()->GetTitleSize());
  obj->GetXaxis()->SetTitleOffset(0.8*obj->GetXaxis()->GetTitleOffset());
  obj->GetYaxis()->SetTitle(ytitle);
  obj->GetYaxis()->SetTitleSize(0.8*obj->GetYaxis()->GetTitleSize());
  obj->GetYaxis()->SetTitleOffset(0.8*obj->GetYaxis()->GetTitleOffset());

}

void Bs2KstKst::MultiDimCutOpt::setAxes(TH1 *obj, TString xtitle, TString ytitle, TString ztitle){

  obj->GetXaxis()->SetTitle(xtitle);
  obj->GetXaxis()->SetTitleSize(0.8*obj->GetXaxis()->GetTitleSize());
  obj->GetXaxis()->SetTitleOffset(0.8*obj->GetXaxis()->GetTitleOffset());
  obj->GetYaxis()->SetTitle(ytitle);
  obj->GetYaxis()->SetTitleSize(0.8*obj->GetYaxis()->GetTitleSize());
  obj->GetYaxis()->SetTitleOffset(0.8*obj->GetYaxis()->GetTitleOffset());
  obj->GetZaxis()->SetTitle(ztitle);
  obj->GetZaxis()->SetTitleSize(0.1);
  obj->GetZaxis()->SetTitleOffset(0.65);

}

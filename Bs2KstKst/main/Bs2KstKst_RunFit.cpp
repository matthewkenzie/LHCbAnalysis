#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TLine.h"
#include "TBox.h"
#include "TAxis.h"
#include "TColor.h"
#include "TMath.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TMatrixDSym.h"

#include "RooWorkspace.h"
#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooAbsData.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooSimultaneous.h"
#include "RooCategory.h"
#include "RooFitResult.h"
#include "RooHist.h"
#include "RooLinkedList.h"
#include "RooCmdArg.h"
#include "RooIpatia2.h"
#include "RooAddPdf.h"
#include "RooCBShape.h"
#include "RooArgusBG.h"
#include "RooExponential.h"

#include "RooStats/SPlot.h"

#include "MassFitPlotter.h"

using namespace std;
using namespace RooFit;
using namespace TMath;
using namespace Bs2KstKst;

void fit( RooWorkspace *w, TString pdf, TString data ) {

  assert( w->pdf(pdf) );
  assert( w->data(data) );

  RooFitResult *fr = w->pdf(pdf)->fitTo( *w->data(data), Save() );
  RooArgSet *parameters = w->pdf(pdf)->getParameters(w->set("observables"));
  w->saveSnapshot( Form("%s_fit",pdf.Data()),*parameters );
  fr->SetName( Form("%s_fr",pdf.Data()) );
  w->import(*fr);

}

void fitToObs( RooWorkspace *w, TString pdf, TString data, TString obs ) {

  assert( w->pdf(pdf) );
  assert( w->data(data) );

  RooArgSet *obsSet = new RooArgSet( *w->var(obs) );
  RooFitResult *fr = w->pdf(pdf)->fitTo( *w->data(data), Save() );
  RooArgSet *parameters = w->pdf(pdf)->getParameters(obsSet);
  w->saveSnapshot( Form("%s_fit",pdf.Data()),*parameters );
  fr->SetName( Form("%s_fr",pdf.Data()) );
  w->import(*fr);
  delete obsSet;

}

void RunMCFits( RooWorkspace *w ) {
  fit( w, "bs2kstkst_mc_pdf", "Bs2KstKst" );
  fit( w, "bd2kstkst_mc_pdf", "Bd2KstKst" );
  fit( w, "bd2phikst_mc_pdf", "Bd2PhiKst" );
  fit( w, "bs2phikst_mc_pdf", "Bs2PhiKst" );
  fit( w, "bd2rhokst_mc_pdf", "Bd2RhoKst" );
  fit( w, "lb2pkpipi_mc_pdf", "Lb2pKpipi" );
  //fit( w, "lb2ppipipi_mc_pdf", "Lb2ppipipi" );
}

void RunBkgFits( RooWorkspace *w) {
  fitToObs( w, "bkg_self_pdf"          , "Combinatorial", "B_s0_DTF_B_s0_M_forCombinatorial" );
  w->var("bkg_exp_p1_constraint_mu")->setVal( w->var("bkg_exp_p1")->getVal() );
  w->var("bkg_exp_p1_constraint_sigma")->setVal( w->var("bkg_exp_p1")->getError() );
  w->var("bkg_exp_p1")->setConstant(true);
  w->var("bs2kstkst_mu")->setVal(5371.7109); // where we eventually fit to
  w->var("bs2kstkst_mu")->setConstant(true);
  fitToObs( w, "part_reco_bkg_self_pdf", "PartReco"     , "B_s0_DTF_B_s0_M_forPartReco" );
  w->var("part_reco_c_constraint_mu")->setVal( w->var("part_reco_c")->getVal() );
  w->var("part_reco_c_constraint_sigma")->setVal( w->var("part_reco_c")->getError() );
  w->var("part_reco_p_constraint_mu")->setVal( w->var("part_reco_p")->getVal() );
  w->var("part_reco_p_constraint_sigma")->setVal( w->var("part_reco_p")->getError() );
  w->var("bkg_exp_p1")->setConstant(false);
  w->var("bs2kstkst_mu")->setConstant(false);
}

void PlotMCFitResults( MassFitPlotter *plotter ) {
  plotter->plot( "B_s0_DTF_B_s0_M", "Bs2KstKst", "bs2kstkst_mc_pdf" );
  plotter->plot( "B_s0_DTF_B_s0_M", "Bd2KstKst", "bd2kstkst_mc_pdf" );
  plotter->plot( "B_s0_DTF_B_s0_M", "Bd2PhiKst", "bd2phikst_mc_pdf" );
  plotter->plot( "B_s0_DTF_B_s0_M", "Bs2PhiKst", "bs2phikst_mc_pdf" );
  plotter->plot( "B_s0_DTF_B_s0_M", "Bd2RhoKst", "bd2rhokst_mc_pdf" );
  plotter->plot( "B_s0_DTF_B_s0_M", "Lb2pKpipi", "lb2pkpipi_mc_pdf" );
  //plotter->plot( "B_s0_DTF_B_s0_M", "Lb2ppipipi", "lb2ppipipi_mc_pdf" );
}

void PlotBkgFitResults( MassFitPlotter *plotter ) {
  plotter->plot( "B_s0_DTF_B_s0_M_forCombinatorial", "Combinatorial", "bkg_self_pdf" );
  plotter->plot( "B_s0_DTF_B_s0_M_forPartReco"     , "PartReco"     , "part_reco_bkg_self_pdf" );
  plotter->makeBkgPlot( "bkgs_fit" );
}

void RunDataFit( RooWorkspace *w ) {

  // load mc fit results
  w->loadSnapshot("bs2kstkst_mc_pdf_fit");
  w->loadSnapshot("bd2kstkst_mc_pdf_fit");
  w->loadSnapshot("bd2phikst_mc_pdf_fit");
  w->loadSnapshot("bs2phikst_mc_pdf_fit");
  w->loadSnapshot("bd2rhokst_mc_pdf_fit");
  w->loadSnapshot("lb2pkpipi_mc_pdf_fit");
  //w->loadSnapshot("lb2ppipipi_mc_pdf_fit");
  w->loadSnapshot("bkg_self_pdf_fit");
  w->loadSnapshot("part_reco_bkg_self_pdf_fit");

  // set relevant parameters constant
  ((RooArgSet*)w->set("bs2kstkst_mc_pdf_params"))->setAttribAll("Constant");
  ((RooArgSet*)w->set("bd2kstkst_mc_pdf_params"))->setAttribAll("Constant");
  ((RooArgSet*)w->set("bd2phikst_mc_pdf_params"))->setAttribAll("Constant");
  ((RooArgSet*)w->set("bs2phikst_mc_pdf_params"))->setAttribAll("Constant");
  ((RooArgSet*)w->set("bd2rhokst_mc_pdf_params"))->setAttribAll("Constant");
  ((RooArgSet*)w->set("lb2pkpipi_mc_pdf_params"))->setAttribAll("Constant");
  //((RooArgSet*)w->set("lb2ppipipi_mc_pdf_params"))->setAttribAll("Constant");
  //((RooArgSet*)w->set("part_reco_pdf_params"))->setAttribAll("Constant");
  //((RooArgSet*)w->set("bkg_pdf_params"))->setAttribAll("Constant");

  // release other parameters we want to
  w->var("bs2kstkst_mu")->setConstant(false);
  w->var("bs2kstkst_sigma")->setConstant(false);
  w->var("bd2kstkst_mu")->setConstant(false);
  w->var("bd2kstkst_sigma")->setConstant(false);
  w->var("bd2phikst_mu")->setConstant(false);
  w->var("bd2phikst_sigma")->setConstant(false);
  w->var("bkg_exp_p1")->setConstant(false);
  //w->var("bs2phikst_mu")->setConstant(false);
  //w->var("bs2phikst_sigma")->setConstant(false);
  //w->var("bd2rhokst_mu")->setConstant(false);
  //w->var("bd2rhokst_sigma")->setConstant(false);

  // now fit and save

  // constrained
  RooArgSet *constraints = new RooArgSet();
  constraints->add( *w->var("yield_ratio_bs2phikst_o_bd2phikst") );
  constraints->add( *w->var("yield_ratio_bd2rhokst_o_bd2phikst") );
  constraints->add( *w->var("bkg_exp_p1") );
  constraints->add( *w->var("part_reco_c") );
  constraints->add( *w->var("part_reco_p") );
  w->defineSet( "constrained_vars", *constraints);
  delete constraints;
  RooFitResult *fr = w->pdf("constrained_pdf")->fitTo( *w->data("DataCombined"), Constrain( *w->set("constrainted_vars") ), Extended(), Save() );
  RooArgSet *parameters = w->pdf("constrained_pdf")->getParameters(w->set("observables"));
  w->saveSnapshot("constrained_pdf_fit",*parameters);
  fr->SetName("constrained_pdf_fr");
  w->import(*fr);
  // not constrained
  //RooFitResult *fr = w->pdf("pdf")->fitTo( *w->data("DataCombined"), Extended(), Save() );
  //RooArgSet *parameters = w->pdf("pdf")->getParameters(w->set("observables"));
  //w->saveSnapshot("pdf_fit",*parameters);
  //fr->SetName("pdf_fr");
  //w->import(*fr);

}

void PlotDataFitResult( RooWorkspace *w, MassFitPlotter *plotter ) {

  //w->loadSnapshot("pdf_fit");
  w->loadSnapshot("constrained_pdf_fit");

  plotter->makeDataPlot( "DataFit", "DataCombined", "All" );
  plotter->makeDataPlot( "DataFit", "DataCombined", "HadronTOS2011" );
  plotter->makeDataPlot( "DataFit", "DataCombined", "GlobalTIS2011" );
  plotter->makeDataPlot( "DataFit", "DataCombined", "HadronTOS2012" );
  plotter->makeDataPlot( "DataFit", "DataCombined", "GlobalTIS2012" );

}

void CalcSWeights( RooWorkspace *w ) {

  //w->loadSnapshot( "pdf_fit" );
  w->loadSnapshot( "constrained_pdf_fit" );

  // we know there is a problem with the sWeights in categories
  // given the yields can float we just fix everything else
  // and redo it in each category - sigh!
  //
  // the sWeights do not like getting RooPDFs from the WS so
  // have to make a new ones - sigh!

  RooIpatia2 *bs2kstkst_mc_pdf = new RooIpatia2("bs2kstkst_mc_pdf","bs2kstkst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bs2kstkst_l"),*w->var("bs2kstkst_zeta"),*w->var("bs2kstkst_fb"),*w->var("bs2kstkst_sigma"),*w->var("bs2kstkst_mu"),*w->var("bs2kstkst_a"),*w->var("bs2kstkst_n"),*w->var("bs2kstkst_a2"),*w->var("bs2kstkst_n2"));
  RooIpatia2 *bd2kstkst_mc_pdf = new RooIpatia2("bd2kstkst_mc_pdf","bd2kstkst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2kstkst_l"),*w->var("bd2kstkst_zeta"),*w->var("bd2kstkst_fb"),*w->var("bd2kstkst_sigma"),*w->var("bd2kstkst_mu"),*w->var("bd2kstkst_a"),*w->var("bd2kstkst_n"),*w->var("bd2kstkst_a2"),*w->var("bd2kstkst_n2"));
  RooIpatia2 *bd2rhokst_mc_pdf = new RooIpatia2("bd2rhokst_mc_pdf","bd2rhokst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2rhokst_l"),*w->var("bd2rhokst_zeta"),*w->var("bd2rhokst_fb"),*w->var("bd2rhokst_sigma"),*w->var("bd2rhokst_mu"),*w->var("bd2rhokst_a"),*w->var("bd2rhokst_n"),*w->var("bd2rhokst_a2"),*w->var("bd2rhokst_n2"));
  RooIpatia2 *bd2phikst_mc_pdf = new RooIpatia2("bd2phikst_mc_pdf","bd2phikst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2phikst_l"),*w->var("bd2phikst_zeta"),*w->var("bd2phikst_fb"),*w->var("bd2phikst_sigma"),*w->var("bd2phikst_mu"),*w->var("bd2phikst_a"),*w->var("bd2phikst_n"),*w->var("bd2phikst_a2"),*w->var("bd2phikst_n2"));
  RooIpatia2 *bs2phikst_mc_pdf = new RooIpatia2("bs2phikst_mc_pdf","bs2phikst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bs2phikst_l"),*w->var("bs2phikst_zeta"),*w->var("bs2phikst_fb"),*w->var("bs2phikst_sigma"),*w->var("bs2phikst_mu"),*w->var("bs2phikst_a"),*w->var("bs2phikst_n"),*w->var("bs2phikst_a2"),*w->var("bs2phikst_n2"));
  RooIpatia2 *lb2pkpipi_mc_pdf = new RooIpatia2("lb2pkpipi_mc_pdf","lb2pkpipi_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("lb2pkpipi_l"),*w->var("lb2pkpipi_zeta"),*w->var("lb2pkpipi_fb"),*w->var("lb2pkpipi_sigma"),*w->var("lb2pkpipi_mu"),*w->var("lb2pkpipi_a"),*w->var("lb2pkpipi_n"),*w->var("lb2pkpipi_a2"),*w->var("lb2pkpipi_n2"));
  //RooCBShape *lb2ppipipi_mc_pdf = new RooCBShape( "lb2ppipipi_mc_pdf", "lb2ppipipi_mc_pdf", *w->var("B_s0_DTF_B_s0_M"), *w->var("lb2ppipipi_mean"), *w->var("lb2ppipipi_sigma"), *w->var("lb2ppipipi_alpha"), *w->var("lb2ppipipi_n") );
  RooArgusBG *part_reco_pdf = new RooArgusBG( "part_reco_pdf", "part_reco_pdf", *w->var("B_s0_DTF_B_s0_M"), *w->function("part_reco_m0"), *w->var("part_reco_c"), *w->var("part_reco_p") );
  RooExponential *bkg_pdf = new RooExponential( "bkg_pdf", "bkg_pdf", *w->var("B_s0_DTF_B_s0_M"), *w->var("bkg_exp_p1") );

  // load mc fit results
  w->loadSnapshot("bs2kstkst_mc_pdf_fit");
  w->loadSnapshot("bd2kstkst_mc_pdf_fit");
  w->loadSnapshot("bd2phikst_mc_pdf_fit");
  w->loadSnapshot("bs2phikst_mc_pdf_fit");
  w->loadSnapshot("bd2rhokst_mc_pdf_fit");
  w->loadSnapshot("lb2pkpipi_mc_pdf_fit");
  //w->loadSnapshot("lb2ppipipi_mc_pdf_fit");

  // pdfs
  RooCategory *cat = (RooCategory*)w->cat("DataCat");
  for (int c=0; c<cat->numTypes(); c++) {

    cat->setIndex(c);
    RooArgList *yields = new RooArgList();
    yields->add( *w->var(Form("bs2kstkst_y_%s",cat->getLabel())) );
    yields->add( *w->var(Form("bd2kstkst_y_%s",cat->getLabel())) );
    yields->add( *w->var(Form("bd2phikst_y_%s",cat->getLabel())) ); // constrained
    yields->add( *w->function(Form("bs2phikst_y_%s",cat->getLabel())) );
    yields->add( *w->function(Form("bd2rhokst_y_%s",cat->getLabel())) );
    yields->add( *w->function(Form("lb2pkpipi_y_%s",cat->getLabel())) );
    yields->add( *w->var(Form("part_reco_y_%s",cat->getLabel())) );
    yields->add( *w->var(Form("bkg_y_%s",cat->getLabel())) );

    RooArgList *pdfs = new RooArgList();
    pdfs->add( *bs2kstkst_mc_pdf );
    pdfs->add( *bd2kstkst_mc_pdf );
    pdfs->add( *bd2phikst_mc_pdf );
    pdfs->add( *bs2phikst_mc_pdf );
    pdfs->add( *bd2rhokst_mc_pdf );
    pdfs->add( *lb2pkpipi_mc_pdf );
    pdfs->add( *part_reco_pdf );
    pdfs->add( *bkg_pdf );

    RooAddPdf *pdf = new RooAddPdf( "pdf","", *pdfs, *yields );

    RooArgSet obsAndCats( *w->set("observables"), *w->set("categories") );
    RooArgSet *nonyields = pdf->getParameters( RooArgSet( *yields, obsAndCats ) );
    // need to change the actual floating parameters for the sWeights to get rid of the yield ratios
    yields->remove( *w->function(Form("bs2phikst_y_%s",cat->getLabel())) );
    yields->remove( *w->function(Form("bd2rhokst_y_%s",cat->getLabel())) );
    yields->remove( *w->function(Form("lb2pkpipi_y_%s",cat->getLabel())) );

    TString dsetName;
    if ( cat->getLabel() == TString("HadronTOS2011") ) dsetName = "Data2011HadronTOS";
    if ( cat->getLabel() == TString("GlobalTIS2011") ) dsetName = "Data2011GlobalTIS";
    if ( cat->getLabel() == TString("HadronTOS2012") ) dsetName = "Data2012HadronTOS";
    if ( cat->getLabel() == TString("GlobalTIS2012") ) dsetName = "Data2012GlobalTIS";

    RooDataSet *data = (RooDataSet*)w->data(dsetName);

    nonyields->setAttribAll("Constant");

    cout << "Fixed for sPlot:" << endl;
    nonyields->Print("v");
    cout << "Float for sPlot:" << endl;
    yields->Print("v");

    pdf->fitTo(*data, Extended() );

    RooStats::SPlot *sData = new RooStats::SPlot(Form("s%s",dsetName.Data()),"sData", *data, pdf, *yields, RooArgSet(*w->var("eventNumber")) );

    w->import(*data, Rename(Form("%sSWeights",dsetName.Data())) );

    // make the projected dataset
    RooDataSet *dataSW = new RooDataSet(Form("%s_sw",data->GetName()), Form("%s_sw",data->GetTitle()), data, RooArgSet( *w->var("B_s0_DTF_B_s0_M"), *w->var("eventNumber"), *w->var(Form("bs2kstkst_y_%s_sw",cat->getLabel()))), 0, Form("bs2kstkst_y_%s_sw",cat->getLabel()) );
    w->import(*dataSW);

  }

  w->writeToFile("root/MassFit/MassFitResultWSWeights.root");

}

void PrintFitResult( RooWorkspace *w ) {

  TString fr_fname = "plots/MassFit/fitres.tex";
  cout << "Saving fit result to file: " << fr_fname << endl;
  RooFitResult *fr = (RooFitResult*)w->obj("constrained_pdf_fr");
  RooArgList res = fr->floatParsFinal();
  TMatrixDSym corrMat = fr->correlationMatrix();

  // print fit res parameters
  ofstream fr_file;
  fr_file.open( fr_fname.Data() );

  RooRealVar *parg;
  TIterator *iter = res.createIterator();
  fr_file << "\\hline" << endl;
  fr_file << Form("%-40s &  %9s  &       %9s  \\\\","Parameter","Value","Error") << endl;
  fr_file << "\\hline" << endl;
  while ( (parg = (RooRealVar*)iter->Next() ) ) {
    fr_file << Form("%-40s & $%9.4f$ &  $\\pm %9.4f$ \\\\",parg->GetName(), parg->getVal(), parg->getError() ) << endl;
  }
  fr_file << "\\hline" << endl;

  fr_file.close();

  // print corr mat
  //cout << Form("%2s  ","");
  //for ( int i=0; i<corrMat.GetNcols(); i++ ) {
    //cout << Form("%5s  ","");
  //}
  //cout << endl;

  //for ( int j=0; j<corrMat.GetNrows(); j++ ) {
    //cout << Form("%2d  ",j);
    //for ( int i=0; i<corrMat.GetNcols(); i++ ) {
      //cout << Form("%5.3f  ",corrMat[i][j]);
    //}
    //cout << endl;
  //}
}

int main() {

  gROOT->ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C");
  gROOT->ProcessLine(".x ~/lhcbStyle.C");
  system("mkdir -p plots/MassFit/png");
  system("mkdir -p plots/MassFit/pdf");
  system("mkdir -p plots/MassFit/C");

  TFile *inf = TFile::Open("root/MassFit/MassFitWorkspaceWithPDFs.root");
  RooWorkspace *w = (RooWorkspace*)inf->Get("w");
  RunMCFits( w );
  RunBkgFits( w );
  RunDataFit( w );
  w->writeToFile("root/MassFit/MassFitResult.root");
  inf->Close();

  TFile *tf = TFile::Open("root/MassFit/MassFitResult.root");
  RooWorkspace *ws = (RooWorkspace*)tf->Get("w");
  MassFitPlotter *plotter = new MassFitPlotter( ws, "MassFit" );
  PlotMCFitResults( plotter );
  PlotBkgFitResults( plotter );
  PlotDataFitResult( ws, plotter );
  CalcSWeights( ws );
  PrintFitResult( ws );
  tf->Close();
  //

  return 0;
}

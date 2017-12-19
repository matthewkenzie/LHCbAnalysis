#include <iostream>

#include "TROOT.h"
#include "TFile.h"
#include "TMath.h"

#include "RooWorkspace.h"
#include "RooAbsReal.h"
#include "RooAbsPdf.h"
#include "RooRealVar.h"
#include "RooProduct.h"
#include "RooCategory.h"
#include "RooFitResult.h"

#include "MassFitPlotter.h"

using namespace std;
using namespace TMath;
using namespace RooFit;
using namespace Bs2KstKst;

double getIntegratedPdf(RooWorkspace *w, TString prefix, TString range) {

  RooRealVar *mass = (RooRealVar*)w->var("B_s0_DTF_B_s0_M");
  if ( prefix.Contains("2") ) prefix += "_mc";

  RooAbsPdf *pdf = (RooAbsPdf*)w->pdf(Form("%s_pdf",prefix.Data()));
  RooAbsReal *i_sig = pdf->createIntegral( *mass, NormSet(*mass), Range("signal") );

  return i_sig->getVal();
}

pair<double,double> getYieldAndErr(RooWorkspace *w, TString prefix, bool realVar=true) {

  double yield = 0.;
  double yield_err = 0.;
  RooCategory *cat = (RooCategory*)w->cat("DataCat");
  RooFitResult *fr = (RooFitResult*)w->obj("constrained_pdf_fr");

  for ( int c=0; c<cat->numTypes(); c++ ) {
    cat->setIndex(c);
    if ( realVar ) {
      RooRealVar *var = (RooRealVar*)w->var(Form("%s_y_%s",prefix.Data(),cat->getLabel()));
      yield += var->getVal();
      yield_err += (var->getError()*var->getError());
    }
    else {
      RooRealVar *prod = (RooRealVar*)w->var(Form("yield_ratio_%s_o_bd2phikst",prefix.Data()));
      RooRealVar *var  = (RooRealVar*)w->var(Form("bd2phikst_y_%s",cat->getLabel()));
      double val = prod->getVal()*var->getVal();
      double err = val * TMath::Sqrt( TMath::Power(prod->getError()/prod->getVal() ,2) + TMath::Power(var->getError()/var->getVal() ,2) );
      yield += val;
      yield_err += err;
    }
  }

  yield_err = Sqrt( yield_err );
  double integral = getIntegratedPdf( w, prefix, "signal");
  cout << Form("%-20s",prefix.Data()) << " " << Form("%4.0f",yield) << " +/- " << Form("%-4.0f",yield_err) << " " << Form("%6.1f",integral*yield) << " " << Form("%4.1f%%",integral*100.) << endl;
  return pair<double,double>(yield,yield_err);
}

int main() {

  gROOT->ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C");
  system("mkdir -p plots/Paper/png");
  system("mkdir -p plots/Paper/pdf");
  system("mkdir -p plots/Paper/C");

  TFile *tf = TFile::Open("root/MassFit/MassFitResultWSWeights.root");
  RooWorkspace *w = (RooWorkspace*)tf->Get("w");
  w->var("B_s0_DTF_B_s0_M")->SetTitle("m(#it{K}^{#plus}#it{#pi}^{#minus}#it{K}^{#minus}#it{#pi}^{#plus}) [MeV/#it{c}^{2}]");
  w->var("B_s0_DTF_B_s0_M")->setUnit("");

  MassFitPlotter *plotter = new MassFitPlotter( w, "Paper");
  plotter->titleOnLog = true;
  plotter->logRangeMin = 4.;
  plotter->logRangeMax = 2.e3;
  plotter->xtitle = "#it{m}(#it{K}^{#plus}#it{#pi}^{#minus}#it{K}^{#minus}#it{#pi}^{#plus}) [MeV/#it{c}^{2}]";
  plotter->ytitle = "Candidates / ( 8 MeV/#it{c}^{2} )";
  plotter->chooseColorScheme(2);

  w->loadSnapshot("constrained_pdf_fit");
  plotter->makeDataPlot( "DataFit", "DataCombined", "All", true );

  // make the yield tables
  //
  RooRealVar *mass = (RooRealVar*)w->var("B_s0_DTF_B_s0_M");
  mass->setRange("signal", w->var("bs2kstkst_mu")->getVal()-60., w->var("bs2kstkst_mu")->getVal()+60.);

  getYieldAndErr(w,"bs2kstkst");
  getYieldAndErr(w,"bd2kstkst");
  getYieldAndErr(w,"bd2phikst");
  getYieldAndErr(w,"bs2phikst",false);
  getYieldAndErr(w,"bd2rhokst",false);
  getYieldAndErr(w,"lb2pkpipi",false);
  getYieldAndErr(w,"part_reco");
  getYieldAndErr(w,"bkg");

  return 0;
}

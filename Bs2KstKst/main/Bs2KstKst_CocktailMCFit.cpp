#include <iostream>
#include <map>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TCanvas.h"

#include "RooWorkspace.h"
#include "RooFitResult.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooAbsPdf.h"
#include "RooArgSet.h"
#include "RooCategory.h"
#include "RooPlot.h"
#include "RooDataSet.h"

#include "MassFitPlotter.h"

using namespace std;
using namespace RooFit;
using namespace Bs2KstKst;

double getWeight( RooWorkspace *w, TString dname, TString pname, TString catName ) {

  int catIndex=-1;
  if ( catName == "HadronTOS2011" ) catIndex = 0;
  else if ( catName == "GlobalTIS2011" ) catIndex = 1;
  else if ( catName == "HadronTOS2012" ) catIndex = 2;
  else if ( catName == "GlobalTIS2012" ) catIndex = 3;
  else assert(0);
  double evs = w->data(dname)->sumEntries( Form("DataCat==%d",catIndex) );
  double yield;
  if ( w->var(pname+"_y_"+catName) ) yield = w->var(pname+"_y_"+catName)->getVal();
  else if ( w->function(pname+"_y_"+catName) ) yield = w->function(pname+"_y_"+catName)->getVal();
  else assert(0);
  return yield / evs;
}

void addWeightedMCToCocktail(RooWorkspace *w, TString dsetname, TString pdfname) {

  cout << "Adding weights to dataset for: " << dsetname << endl;
  RooDataSet *dset = (RooDataSet*)w->data(dsetname);

  vector<double> weights;
  for ( int i=0; i<w->cat("DataCat")->numTypes(); i++ ) {
    w->cat("DataCat")->setIndex(i);
    weights.push_back( getWeight( w, dsetname, pdfname, w->cat("DataCat")->getLabel() ) );
  }

  for ( int ev=0; ev<dset->numEntries(); ev++ ) {
    const RooArgSet *vals = dset->get(ev);
    w->var("B_s0_DTF_B_s0_M")->setVal( vals->getRealValue("B_s0_DTF_B_s0_M") );
    w->cat("DataCat")->setIndex( vals->getCatIndex("DataCat") );
    w->data("Cocktail")->add( RooArgSet( *w->var("B_s0_DTF_B_s0_M"), *w->cat("DataCat") ), weights[w->cat("DataCat")->getIndex()] );
  }

}

void addToyFromPdfToCocktail(RooWorkspace *w, TString pdfname, TString yieldname, TString catname) {

  double nevs = w->var(yieldname)->getVal();
  RooDataSet *toy = w->pdf(pdfname)->generate( RooArgSet( *w->var("B_s0_DTF_B_s0_M") ), int(floor(nevs+0.5)), Extended() );

  for ( int ev=0; ev<toy->numEntries(); ev++ ) {

    const RooArgSet *vals = toy->get(ev);
    w->var("B_s0_DTF_B_s0_M")->setVal( vals->getRealValue("B_s0_DTF_B_s0_M") );
    w->cat("DataCat")->setLabel(catname);
    w->data("Cocktail")->add( RooArgSet( *w->var("B_s0_DTF_B_s0_M"), *w->cat("DataCat") ), 1. );
  }

}

void makeCocktailMC(RooWorkspace *w) {

  w->factory("weight[0.,100.]");
  w->var("weight")->removeRange();
  RooDataSet *cocktail = new RooDataSet( "Cocktail", "Cocktail", RooArgSet(*w->var("B_s0_DTF_B_s0_M"),*w->cat("DataCat"),*w->var("weight")), WeightVar("weight") );
  w->import( *cocktail );
  addWeightedMCToCocktail(w, "Bs2KstKst", "bs2kstkst" );
  addWeightedMCToCocktail(w, "Bd2KstKst", "bd2kstkst" );
  addWeightedMCToCocktail(w, "Bs2PhiKst", "bs2phikst" );
  addWeightedMCToCocktail(w, "Bd2PhiKst", "bd2phikst" );
  addWeightedMCToCocktail(w, "Bd2RhoKst", "bd2rhokst" );
  addWeightedMCToCocktail(w, "Lb2pKpipi", "lb2pkpipi" );
  addToyFromPdfToCocktail(w, "part_reco_pdf", "part_reco_y_HadronTOS2011", "HadronTOS2011" );
  addToyFromPdfToCocktail(w, "part_reco_pdf", "part_reco_y_GlobalTIS2011", "GlobalTIS2011" );
  addToyFromPdfToCocktail(w, "part_reco_pdf", "part_reco_y_HadronTOS2012", "HadronTOS2012" );
  addToyFromPdfToCocktail(w, "part_reco_pdf", "part_reco_y_GlobalTIS2012", "GlobalTIS2012" );
  addToyFromPdfToCocktail(w, "bkg_pdf_HadronTOS2011", "bkg_y_HadronTOS2011", "HadronTOS2011" );
  addToyFromPdfToCocktail(w, "bkg_pdf_GlobalTIS2011", "bkg_y_GlobalTIS2011", "GlobalTIS2011" );
  addToyFromPdfToCocktail(w, "bkg_pdf_HadronTOS2012", "bkg_y_HadronTOS2012", "HadronTOS2012" );
  addToyFromPdfToCocktail(w, "bkg_pdf_GlobalTIS2012", "bkg_y_GlobalTIS2012", "GlobalTIS2012" );

}

void fitCocktailMC(RooWorkspace *w) {

  // load mc fit results
  w->loadSnapshot("bs2kstkst_mc_pdf_fit");
  w->loadSnapshot("bd2kstkst_mc_pdf_fit");
  w->loadSnapshot("bd2phikst_mc_pdf_fit");
  w->loadSnapshot("bs2phikst_mc_pdf_fit");
  w->loadSnapshot("bd2rhokst_mc_pdf_fit");
  w->loadSnapshot("lb2pkpipi_mc_pdf_fit");
  w->loadSnapshot("lb2ppipipi_mc_pdf_fit");

  // set relevant parameters constant
  ((RooArgSet*)w->set("bs2kstkst_mc_pdf_params"))->setAttribAll("Constant");
  ((RooArgSet*)w->set("bd2kstkst_mc_pdf_params"))->setAttribAll("Constant");
  ((RooArgSet*)w->set("bd2phikst_mc_pdf_params"))->setAttribAll("Constant");
  ((RooArgSet*)w->set("bs2phikst_mc_pdf_params"))->setAttribAll("Constant");
  ((RooArgSet*)w->set("bd2rhokst_mc_pdf_params"))->setAttribAll("Constant");
  ((RooArgSet*)w->set("lb2pkpipi_mc_pdf_params"))->setAttribAll("Constant");
  ((RooArgSet*)w->set("lb2ppipipi_mc_pdf_params"))->setAttribAll("Constant");

  // release other parameters we want to
  w->var("bs2kstkst_mu")->setConstant(false);
  w->var("bs2kstkst_sigma")->setConstant(false);
  w->var("bd2kstkst_mu")->setConstant(false);
  w->var("bd2kstkst_sigma")->setConstant(false);
  //w->var("bd2phikst_mu")->setConstant(false);
  //w->var("bd2phikst_sigma")->setConstant(false);
  //w->var("bs2phikst_mu")->setConstant(false);
  //w->var("bs2phikst_sigma")->setConstant(false);
  //w->var("bd2rhokst_mu")->setConstant(false);
  //w->var("bd2rhokst_sigma")->setConstant(false);

  // now fit and save
  RooFitResult *fr = w->pdf("constrained_pdf")->fitTo( *w->data("Cocktail"), Constrain( RooArgSet(*w->var("yield_ratio_bs2phikst_o_bd2phikst"),*w->var("yield_ratio_bd2rhokst_o_bd2phikst")) ), Extended(), SumW2Error(false), Save() );
  RooArgSet *parameters = w->pdf("constrained_pdf")->getParameters(w->set("observables"));
  w->saveSnapshot("cocktail_mc_fit",*parameters);
  fr->SetName("cocktail_mc_fr");
  w->import(*fr);
}

int main(int argc, char* argv[]) {

  TFile *wf = TFile::Open("root/MassFitResultWSWeights.root");
  RooWorkspace *w = (RooWorkspace*)wf->Get("w");

  w->Print();

  RooFitResult *fr = (RooFitResult*)w->obj("constrained_pdf_fr");
  fr->Print("v");

  fr->floatParsFinal().Print("v");

  w->loadSnapshot("constrained_pdf_fit");

  makeCocktailMC(w);

  fitCocktailMC(w);

  MassFitPlotter *plotter = new MassFitPlotter( w, "CocktailMCFit" );
  plotter->makeDataPlot( "CocktailMCFit", "Cocktail", "All" );
  plotter->makeDataPlot( "CocktailMCFit", "Cocktail", "HadronTOS2011" );
  plotter->makeDataPlot( "CocktailMCFit", "Cocktail", "GlobalTIS2011" );
  plotter->makeDataPlot( "CocktailMCFit", "Cocktail", "HadronTOS2012" );
  plotter->makeDataPlot( "CocktailMCFit", "Cocktail", "GlobalTIS2012" );

  delete wf;

  return 0;
}

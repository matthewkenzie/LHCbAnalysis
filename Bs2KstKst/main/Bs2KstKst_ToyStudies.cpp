#include <iostream>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TString.h"
#include "TLine.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TH1D.h"
#include "TRandom3.h"

#include "RooWorkspace.h"
#include "RooPlot.h"
#include "RooMsgService.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooAbsData.h"
#include "RooDataSet.h"
#include "RooAbsPdf.h"
#include "RooSimultaneous.h"
#include "RooIpatia2.h"
#include "RooAddPdf.h"
#include "RooArgusBG.h"
#include "RooExponential.h"
#include "RooCBShape.h"
#include "RooPolynomial.h"
#include "RooProdPdf.h"

#include "RooStats/SPlot.h"

#include "boost/lexical_cast.hpp"

#include "MassFitPlotter.h"

using namespace std;
using namespace TMath;
using namespace RooFit;
using namespace Bs2KstKst;

RooIpatia2 *createNewIpatiaFromWorkspace(RooWorkspace *w, TString fitName, TString compName, RooRealVar *obs, RooArgList *pStore) {

  assert( pStore->getSize()==0 );

  w->loadSnapshot(fitName);

  RooRealVar *l     = new RooRealVar( Form("%s_l_clone",compName.Data())       , "l"    , w->var(Form("%s_l",compName.Data()))->getVal() );
  RooRealVar *zeta  = new RooRealVar( Form("%s_zeta_clone",compName.Data())    , "zeta" , w->var(Form("%s_zeta",compName.Data()))->getVal() );
  RooRealVar *fb    = new RooRealVar( Form("%s_fb_clone",compName.Data())      , "fb"   , w->var(Form("%s_fb",compName.Data()))->getVal() );
  RooRealVar *sigma = new RooRealVar( Form("%s_sigma_clone",compName.Data())   , "sigma", w->var(Form("%s_sigma",compName.Data()))->getVal() );
  RooRealVar *mu    = new RooRealVar( Form("%s_mu_clone",compName.Data())      , "mu"   , w->var(Form("%s_mu",compName.Data()))->getVal() );
  RooRealVar *a     = new RooRealVar( Form("%s_a_clone",compName.Data())       , "a"    , w->var(Form("%s_a",compName.Data()))->getVal() );
  RooRealVar *n     = new RooRealVar( Form("%s_n_clone",compName.Data())       , "n"    , w->var(Form("%s_n",compName.Data()))->getVal() );
  RooRealVar *a2    = new RooRealVar( Form("%s_a2_clone",compName.Data())      , "a2"   , w->var(Form("%s_a2",compName.Data()))->getVal() );
  RooRealVar *n2    = new RooRealVar( Form("%s_n2_clone",compName.Data())      , "n2"   , w->var(Form("%s_n2",compName.Data()))->getVal() );

  pStore->add(*l);
  pStore->add(*zeta);
  pStore->add(*fb);
  pStore->add(*sigma);
  pStore->add(*mu);
  pStore->add(*a);
  pStore->add(*n);
  pStore->add(*a2);
  pStore->add(*n2);

  RooIpatia2 *pdf = new RooIpatia2( Form("%s_pdf_clone",compName.Data()), "pdf", *obs, *l, *zeta, *fb, *sigma, *mu, *a, *n, *a2, *n2 );
  return pdf;
}

RooExponential *createNewBkgFromWorkspace(RooWorkspace *w, TString fitName, RooRealVar *obs, RooArgList *pStore) {

  assert( pStore->getSize()==0 );

  w->loadSnapshot(fitName);

  RooRealVar *p = new RooRealVar( "bkg_exp_p1_clone", "p1", w->var("bkg_exp_p1")->getVal() );
  pStore->add(*p);

  RooExponential *pdf = new RooExponential( "bkg_pdf_clone", "pdf", *obs, *p );
  return pdf;
}

RooArgusBG *createNewPartRecoFromWorkspace(RooWorkspace *w, TString fitName, RooRealVar *obs, RooArgList *pStore) {

  assert( pStore->getSize()==0 );

  w->loadSnapshot(fitName);

  RooRealVar *m0 = new RooRealVar( "part_reco_m0_clone", "p1", w->function("part_reco_m0")->getVal() );
  RooRealVar *c  = new RooRealVar( "part_reco_c_clone", "p1", w->var("part_reco_c")->getVal() );
  RooRealVar *p  = new RooRealVar( "part_reco_p_clone", "p1", w->var("part_reco_p")->getVal() );
  pStore->add(*m0);
  pStore->add(*c);
  pStore->add(*p);

  RooArgusBG *pdf = new RooArgusBG( "part_reco_clone", "pdf", *obs, *m0, *c, *p );
  return pdf;

}

void redoSWeights( RooWorkspace *w ) {

  RooRealVar *mass = (RooRealVar*)w->var("B_s0_DTF_B_s0_M");

  RooArgList *bs2kstkstParams = new RooArgList();
  RooIpatia2 *bs2kstkst = createNewIpatiaFromWorkspace(w,"constrained_pdf_fit","bs2kstkst",mass,bs2kstkstParams);

  RooArgList *bd2kstkstParams = new RooArgList();
  RooIpatia2 *bd2kstkst = createNewIpatiaFromWorkspace(w,"constrained_pdf_fit","bd2kstkst",mass,bd2kstkstParams);

  RooArgList *bd2phikstParams = new RooArgList();
  RooIpatia2 *bd2phikst = createNewIpatiaFromWorkspace(w,"constrained_pdf_fit","bd2phikst",mass,bd2phikstParams);

  RooArgList *bs2phikstParams = new RooArgList();
  RooIpatia2 *bs2phikst = createNewIpatiaFromWorkspace(w,"constrained_pdf_fit","bs2phikst",mass,bs2phikstParams);

  RooArgList *bd2rhokstParams = new RooArgList();
  RooIpatia2 *bd2rhokst = createNewIpatiaFromWorkspace(w,"constrained_pdf_fit","bd2rhokst",mass,bd2rhokstParams);

  RooArgList *lb2pkpipiParams = new RooArgList();
  RooIpatia2 *lb2pkpipi = createNewIpatiaFromWorkspace(w,"constrained_pdf_fit","lb2pkpipi",mass,lb2pkpipiParams);

  RooArgList *partrecParams = new RooArgList();
  RooArgusBG *partrec = createNewPartRecoFromWorkspace(w,"constrained_pdf_fit",mass,partrecParams);

  RooArgList *bkgParams = new RooArgList();
  RooExponential *bkg = createNewBkgFromWorkspace(w,"constrained_pdf_fit",mass,bkgParams);

  RooRealVar *bs2kstkst_y = new RooRealVar("bs2kstkst_y","bs2kstkst_y",w->var("bs2kstkst_y_HadronTOS2012")->getVal());
  RooRealVar *bd2kstkst_y = new RooRealVar("bd2kstkst_y","bd2kstkst_y",w->var("bd2kstkst_y_HadronTOS2012")->getVal());
  RooRealVar *bd2phikst_y = new RooRealVar("bd2phikst_y","bd2phikst_y",w->var("bd2phikst_y_HadronTOS2012")->getVal());
  RooRealVar *bs2phikst_y = new RooRealVar("bs2phikst_y","bs2phikst_y",w->function("bs2phikst_y_HadronTOS2012")->getVal());
  RooRealVar *bd2rhokst_y = new RooRealVar("bd2rhokst_y","bd2rhokst_y",w->function("bd2rhokst_y_HadronTOS2012")->getVal());
  RooRealVar *lb2pkpipi_y = new RooRealVar("lb2pkpipi_y","lb2pkpipi_y",w->function("lb2pkpipi_y_HadronTOS2012")->getVal());
  RooRealVar *part_reco_y = new RooRealVar("part_reco_y","part_reco_y",w->var("part_reco_y_HadronTOS2012")->getVal());
  RooRealVar *bkg_y = new RooRealVar("bkg_y","bkg_y",w->var("bkg_y_HadronTOS2012")->getVal());

  RooArgList *pdfs = new RooArgList();
  pdfs->add(*bs2kstkst);
  pdfs->add(*bd2kstkst);
  pdfs->add(*bd2phikst);
  pdfs->add(*bs2phikst);
  pdfs->add(*bd2rhokst);
  pdfs->add(*lb2pkpipi);
  pdfs->add(*partrec);
  pdfs->add(*bkg);

  RooArgList *yields = new RooArgList();
  yields->add(*bs2kstkst_y);
  yields->add(*bd2kstkst_y);
  yields->add(*bd2phikst_y);
  yields->add(*bs2phikst_y);
  yields->add(*bd2rhokst_y);
  yields->add(*lb2pkpipi_y);
  yields->add(*part_reco_y);
  yields->add(*bkg_y);

  RooAddPdf  *pdf   = new RooAddPdf("pdf","pdf",*pdfs,*yields);
  RooDataSet *data  = (RooDataSet*)w->data("Data2012HadronTOS");

  RooPlot *plot = mass->frame();
  data->plotOn(plot);
  pdf->plotOn(plot);

  //cout << "Yields: " << endl;
  //yields->Print("v");
  yields->setAttribAll("Constant",false);

  RooStats::SPlot *sData = new RooStats::SPlot( "sData", "sData", *data, pdf, *yields );
  w->import(*data,Rename("Data2012HadronTOS_WSW"));
  //data->Print("v");

  //cout << "Yields: " << endl;
  //yields->Print("v");

  RooDataSet *dataSW = new RooDataSet( "sw", "sw", data, RooArgSet(*mass, w->var("bs2kstkst_y_sw")), 0, "bs2kstkst_y_sw" );

  TFile *oldf = TFile::Open("root/MassFit/MassFitResultWSWeights.root");
  RooWorkspace *ows = (RooWorkspace*)oldf->Get("w");
  RooDataSet *odata = (RooDataSet*)ows->data("Data2012HadronTOS_sw");

  RooPlot *plot2 = mass->frame();
  dataSW->plotOn(plot2);
  odata->plotOn(plot2, LineColor(kRed), MarkerColor(kRed));

  cout << "Check: " << bs2kstkst_y->getVal() << " " << dataSW->sumEntries() << " " << odata->sumEntries() << endl;

  TLine *l = new TLine();
  l->SetLineWidth(3);
  l->SetLineColor(kBlue);

  TCanvas *c = new TCanvas("c","c",800,1200);
  c->Divide(1,2);
  c->cd(1);
  plot->Draw();
  c->cd(2);
  plot2->Draw();
  l->DrawLine(5000,0,5800,0);
  c->Update();
  c->Print("c.pdf");

  return;
}

void shiftYields( RooWorkspace *w, TString startingSnapshot, TString saveSnapshot, int nsigma ) {

  // shift or randomize the yields around by nsigma
  // if nsigma=0 then randomize them

  w->loadSnapshot(startingSnapshot);

  RooArgList *yieldVars = new RooArgList();
  yieldVars->add( *w->var("yield_ratio_bs2phikst_o_bd2phikst") );
  yieldVars->add( *w->var("yield_ratio_bd2rhokst_o_bd2phikst") );
  yieldVars->add( *w->var("yield_ratio_lb2pkpipi_o_bd2phikst") );

  RooCategory *cat = (RooCategory*)w->cat("DataCat");
  for ( int c=0; c<cat->numTypes(); c++ ){
    cat->setIndex(c);
    yieldVars->add( *w->var(Form("bs2kstkst_y_%s",cat->getLabel())) );
    yieldVars->add( *w->var(Form("bd2kstkst_y_%s",cat->getLabel())) );
    yieldVars->add( *w->var(Form("bd2phikst_y_%s",cat->getLabel())) );
    yieldVars->add( *w->var(Form("part_reco_y_%s",cat->getLabel())) );
    yieldVars->add( *w->var(Form("bkg_y_%s",cat->getLabel())) );
  }
  //yieldVars->Print("v");

  for ( int i=0; i < yieldVars->getSize(); i++ ) {
    RooRealVar *param = (RooRealVar*)yieldVars->at(i);
    cout << Form("%-20s",param->GetName()) << " " << param->getVal() << " +/- " << param->getError() << endl;
    if ( nsigma==0 ) { // randomize it
      TRandom3 rand;
      rand.SetSeed(0); // important otherwise you get the same toy every time
      double val = rand.Gaus( param->getVal(), param->getError() );
      param->setVal(val);
      param->setConstant();
    }
    else {
      param->setVal( param->getVal() + (nsigma*param->getError()) );
      param->setConstant();
    }
  }
  RooArgSet *params = w->pdf("constrained_pdf")->getParameters(w->set("observables"));
  w->saveSnapshot( saveSnapshot, *params );

  //cout << "Check: " << endl;
  //cout << "My params" << endl;
  //params->Print("v");
  //w->loadSnapshot(startingSnapshot);
  //cout << "Starting params" << endl;
  //params->Print("v");
  //w->loadSnapshot(saveSnapshot);
  //cout << "Shifted params" << endl;
  //params->Print("v");

  // now need to put the original values back in case anything else needs doing elsewhere
  w->loadSnapshot(startingSnapshot);
}

void makeDCBShape( RooWorkspace *w, MassFitPlotter *plotter, TString name, TString plotWData="" ){

  w->var(Form("%s_mu",name.Data()))->setConstant(false);
  w->var(Form("%s_sigma",name.Data()))->setConstant(false);
  w->var(Form("%s_a",name.Data()))->setConstant(false);
  w->var(Form("%s_a",name.Data()))->setRange(-10,10);
  w->var(Form("%s_n",name.Data()))->setConstant(false);
  w->var(Form("%s_n",name.Data()))->setRange(1,10);
  w->var(Form("%s_a2",name.Data()))->setConstant(false);
  w->var(Form("%s_a2",name.Data()))->setRange(-10,10);
  w->var(Form("%s_n2",name.Data()))->setConstant(false);
  w->var(Form("%s_n2",name.Data()))->setRange(1,10);
  w->factory(Form("%s_f[0.2,0.,1.]",name.Data()));

  RooCBShape *cb1 = new RooCBShape(Form("%s_cb1_pdf",name.Data()),Form("%s_cb1_pdf",name.Data()),*w->var("B_s0_DTF_B_s0_M"), *w->var(Form("%s_mu",name.Data())), *w->var(Form("%s_sigma",name.Data())), *w->var(Form("%s_a",name.Data())), *w->var(Form("%s_n",name.Data())) );
  RooCBShape *cb2 = new RooCBShape(Form("%s_cb2_pdf",name.Data()),Form("%s_cb2_pdf",name.Data()),*w->var("B_s0_DTF_B_s0_M"), *w->var(Form("%s_mu",name.Data())), *w->var(Form("%s_sigma",name.Data())), *w->var(Form("%s_a2",name.Data())), *w->var(Form("%s_n2",name.Data())) );
  RooAddPdf *cb = new RooAddPdf(Form("%s_cb_pdf",name.Data()),Form("%s_cb_pdf",name.Data()),*cb1,*cb2,*w->var(Form("%s_f",name.Data())));

  w->import( *cb );

  if (plotWData!="") {
    RooFitResult *fr = w->pdf(Form("%s_cb_pdf",name.Data()))->fitTo( *w->data(plotWData), Save() );
    RooArgSet *parameters = w->pdf(Form("%s_cb_pdf",name.Data()))->getParameters(RooArgSet(*w->var("B_s0_DTF_B_s0_M")));
    w->saveSnapshot( Form("%s_cb_pdf_fit",name.Data()), *parameters );
    plotter->plot( "B_s0_DTF_B_s0_M", plotWData, Form("%s_cb_pdf",name.Data() ) );
  }
}

void makePolyShape( RooWorkspace *w, TString name, int order ) {

  RooArgList *coeffs = new RooArgList();
  for (int i=0; i<order; i++){
    w->factory(Form("bkg_p%d_p%d[-1.65e-4,-0.001,0.02]",order,i+1));
    coeffs->add( *w->var(Form("bkg_p%d_p%d",order,i+1)) );
  }
  RooPolynomial *poly = new RooPolynomial( name, name, *w->var("B_s0_DTF_B_s0_M"), *coeffs );
  w->import( *poly );

}

void makeFitAndPlotTotalShape(RooWorkspace *w, MassFitPlotter *plotter, bool switchDCbs, bool switchBkgs) {

  TString newName;
  if ( switchDCbs && switchBkgs ) newName = "swcb_swbkg";
  else if ( switchDCbs ) newName = "swcb";
  else if ( switchBkgs ) newName = "swbkg";

  RooCategory *cat = (RooCategory*)w->cat("DataCat");
  for ( int i=0; i < cat->numTypes(); i++ ) {
    cat->setIndex(i);

    RooArgList *yields = new RooArgList();
    yields->add(*w->var( Form("bkg_y_%s"      , cat->getLabel()) ));
    yields->add(*w->var( Form("part_reco_y_%s", cat->getLabel()) ));
    yields->add(*w->var( Form("bs2kstkst_y_%s", cat->getLabel()) ));
    yields->add(*w->var( Form("bd2kstkst_y_%s", cat->getLabel()) ));
    yields->add(*w->var( Form("bd2phikst_y_%s", cat->getLabel()) ));
    yields->add(*w->function( Form("bs2phikst_y_%s", cat->getLabel()) ));
    yields->add(*w->function( Form("bd2rhokst_y_%s", cat->getLabel()) ));
    yields->add(*w->function( Form("lb2pkpipi_y_%s", cat->getLabel()) ));

    RooArgList *pdfs   = new RooArgList();
    if ( switchBkgs ) {
      pdfs->add(*w->pdf("bkg_p1_pdf" ));
    }
    else{
      pdfs->add(*w->pdf("bkg_pdf" ));
    }
    pdfs->add(*w->pdf("part_reco_pdf" ));
    if ( switchDCbs ) {
      pdfs->add(*w->pdf("bs2kstkst_cb_pdf" ));
      pdfs->add(*w->pdf("bd2kstkst_cb_pdf" ));
      pdfs->add(*w->pdf("bd2phikst_cb_pdf" ));
      pdfs->add(*w->pdf("bs2phikst_cb_pdf" ));
      pdfs->add(*w->pdf("bd2rhokst_cb_pdf" ));
      pdfs->add(*w->pdf("lb2pkpipi_cb_pdf" ));
    }
    else {
      pdfs->add(*w->pdf("bs2kstkst_mc_pdf" ));
      pdfs->add(*w->pdf("bd2kstkst_mc_pdf" ));
      pdfs->add(*w->pdf("bd2phikst_mc_pdf" ));
      pdfs->add(*w->pdf("bs2phikst_mc_pdf" ));
      pdfs->add(*w->pdf("bd2rhokst_mc_pdf" ));
      pdfs->add(*w->pdf("lb2pkpipi_mc_pdf" ));
    }

    RooAddPdf *pdf = new RooAddPdf( Form("%s_pdf_%s",newName.Data(),cat->getLabel()), "pdf" , *pdfs, *yields);
    w->import(*pdf);
    delete pdf;

    RooArgSet *prodpdfs = new RooArgSet();
    prodpdfs->add( *w->pdf(Form("%s_pdf_%s",newName.Data(),cat->getLabel())) );
    prodpdfs->add( *w->pdf("yield_ratio_bs2phikst_o_bd2phikst_constraint") );
    prodpdfs->add( *w->pdf("yield_ratio_bd2rhokst_o_bd2phikst_constraint") );
    if (!switchBkgs ) prodpdfs->add( *w->pdf("bkg_exp_p1_constraint") );
    prodpdfs->add( *w->pdf("part_reco_c_constraint") );
    prodpdfs->add( *w->pdf("part_reco_p_constraint") );
    RooProdPdf *cpdf = new RooProdPdf( Form("constrained_%s_pdf_%s",newName.Data(),cat->getLabel()), "constrained_pdf", *prodpdfs );
    w->import(*cpdf);
    delete cpdf;

  }
  RooSimultaneous *cpdf = new RooSimultaneous( Form("constrained_%s_pdf",newName.Data()), Form("constrained_%s_pdf",newName.Data()), *w->cat("DataCat") );
  for ( int i=0; i < cat->numTypes(); i++ ) {
    cat->setIndex(i);
    cpdf->addPdf( *w->pdf( Form("constrained_%s_pdf_%s", newName.Data(), cat->getLabel() )), cat->getLabel() );
  }
  w->import(*cpdf);
  delete cpdf;

  // now do the fit
  if ( switchDCbs ) {
    w->loadSnapshot("bs2kstkst_cb_pdf_fit");
    w->loadSnapshot("bd2kstkst_cb_pdf_fit");
    w->loadSnapshot("bd2phikst_cb_pdf_fit");
    w->loadSnapshot("bs2phikst_cb_pdf_fit");
    w->loadSnapshot("bd2rhokst_cb_pdf_fit");
    w->loadSnapshot("lb2pkpipi_cb_pdf_fit");
  }
  else {
    w->loadSnapshot("bs2kstkst_mc_pdf_fit");
    w->loadSnapshot("bd2kstkst_mc_pdf_fit");
    w->loadSnapshot("bd2phikst_mc_pdf_fit");
    w->loadSnapshot("bs2phikst_mc_pdf_fit");
    w->loadSnapshot("bd2rhokst_mc_pdf_fit");
    w->loadSnapshot("lb2pkpipi_mc_pdf_fit");
  }
  if ( !switchBkgs ) {
    w->loadSnapshot("bkg_self_pdf_fit");
  }
  w->loadSnapshot("part_reco_bkg_self_pdf_fit");

  // set relevant parameters constant
  RooArgSet* allParams = w->pdf(Form("constrained_%s_pdf",newName.Data()))->getParameters(RooArgSet(*w->var("B_s0_DTF_B_s0_M")));
  //allParams->Print("v");
  allParams->setAttribAll("Constant");
  // and release the others
  w->var("bs2kstkst_mu")->setConstant(false);
  w->var("bs2kstkst_sigma")->setConstant(false);
  w->var("bd2kstkst_mu")->setConstant(false);
  w->var("bd2kstkst_sigma")->setConstant(false);
  w->var("bd2phikst_mu")->setConstant(false);
  w->var("bd2phikst_sigma")->setConstant(false);
  if ( !switchBkgs ) w->var("bkg_exp_p1")->setConstant(false);
  else w->var("bkg_p1_p1")->setConstant(false);
  w->var("part_reco_c")->setConstant(false);
  w->var("part_reco_p")->setConstant(false);
  w->var("yield_ratio_bs2phikst_o_bd2phikst")->setConstant(false);
  w->var("yield_ratio_bd2rhokst_o_bd2phikst")->setConstant(false);
  w->var("yield_ratio_lb2pkpipi_o_bd2phikst")->setConstant(false);
  for ( int i=0; i < cat->numTypes(); i++ ) {
    cat->setIndex(i);
    w->var(Form("bs2kstkst_y_%s",cat->getLabel()))->setConstant(false);
    w->var(Form("bd2kstkst_y_%s",cat->getLabel()))->setConstant(false);
    w->var(Form("bd2phikst_y_%s",cat->getLabel()))->setConstant(false);
    w->var(Form("part_reco_y_%s",cat->getLabel()))->setConstant(false);
    w->var(Form("bkg_y_%s",cat->getLabel()))->setConstant(false);
  }
  // now do the fit
  RooFitResult *fr = w->pdf(Form("constrained_%s_pdf",newName.Data()))->fitTo( *w->data("DataCombined"), Constrain( *w->set("constrained_vars")), Extended(), Save() );
  RooArgSet *parameters = w->pdf(Form("constrained_%s_pdf",newName.Data()))->getParameters(w->set("observables"));
  w->saveSnapshot( Form("constrained_%s_pdf_fit",newName.Data()), *parameters);
  fr->SetName(Form("constrained_%s_pdf_fr",newName.Data()));
  w->import(*fr);

  plotter->plot( "B_s0_DTF_B_s0_M", "DataCombined", Form("constrained_%s_pdf",newName.Data()), 2, true );

}

void reParamFit( RooWorkspace *w, MassFitPlotter *plotter ) {

  makeDCBShape( w, plotter, "bs2kstkst", "Bs2KstKst" );
  makeDCBShape( w, plotter, "bd2kstkst", "Bd2KstKst" );
  makeDCBShape( w, plotter, "bd2phikst", "Bd2PhiKst" );
  makeDCBShape( w, plotter, "bs2phikst", "Bs2PhiKst" );
  makeDCBShape( w, plotter, "bd2rhokst", "Bd2RhoKst" );
  makeDCBShape( w, plotter, "lb2pkpipi", "Lb2pKpipi" );

  makePolyShape( w, "bkg_p0_pdf", 0 );
  makePolyShape( w, "bkg_p1_pdf", 1 );

  makeFitAndPlotTotalShape( w, plotter, true, false ); // just switch for DCBs
  makeFitAndPlotTotalShape( w, plotter, false, true ); // just switch bkg
  makeFitAndPlotTotalShape( w, plotter, true, true  ); // switch both

  // now fit each shape to data
  // and then calculate sWeights for each new set
}

map<ULong64_t, double> computeSWeightsFromFit( RooWorkspace *w, TString fitName, bool doCompPlot=false, TString compPlotName="c", bool useCBs=false, bool usePoly=false, const vector<double> &scales=vector<double>()) {

  // this is what we return
  // it gives the sweight from the "new pdf" given an eventNumber
  map<ULong64_t, double> sweightMap;

  w->loadSnapshot( fitName );

  RooAbsPdf *bs2kstkst_mc_pdf, *bd2kstkst_mc_pdf, *bd2rhokst_mc_pdf, *bd2phikst_mc_pdf, *bs2phikst_mc_pdf, *lb2pkpipi_mc_pdf;

  if ( useCBs ) {
    bs2kstkst_mc_pdf = (RooCBShape*)w->pdf("bs2kstkst_cb_pdf");
    bd2kstkst_mc_pdf = (RooCBShape*)w->pdf("bd2kstkst_cb_pdf");
    bd2rhokst_mc_pdf = (RooCBShape*)w->pdf("bd2rhokst_cb_pdf");
    bd2phikst_mc_pdf = (RooCBShape*)w->pdf("bd2phikst_cb_pdf");
    bs2phikst_mc_pdf = (RooCBShape*)w->pdf("bs2phikst_cb_pdf");
    lb2pkpipi_mc_pdf = (RooCBShape*)w->pdf("lb2pkpipi_cb_pdf");
  }
  else {
    bs2kstkst_mc_pdf = new RooIpatia2("bs2kstkst_mc_pdf","bs2kstkst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bs2kstkst_l"),*w->var("bs2kstkst_zeta"),*w->var("bs2kstkst_fb"),*w->var("bs2kstkst_sigma"),*w->var("bs2kstkst_mu"),*w->var("bs2kstkst_a"),*w->var("bs2kstkst_n"),*w->var("bs2kstkst_a2"),*w->var("bs2kstkst_n2"));
    bd2kstkst_mc_pdf = new RooIpatia2("bd2kstkst_mc_pdf","bd2kstkst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2kstkst_l"),*w->var("bd2kstkst_zeta"),*w->var("bd2kstkst_fb"),*w->var("bd2kstkst_sigma"),*w->var("bd2kstkst_mu"),*w->var("bd2kstkst_a"),*w->var("bd2kstkst_n"),*w->var("bd2kstkst_a2"),*w->var("bd2kstkst_n2"));
    bd2rhokst_mc_pdf = new RooIpatia2("bd2rhokst_mc_pdf","bd2rhokst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2rhokst_l"),*w->var("bd2rhokst_zeta"),*w->var("bd2rhokst_fb"),*w->var("bd2rhokst_sigma"),*w->var("bd2rhokst_mu"),*w->var("bd2rhokst_a"),*w->var("bd2rhokst_n"),*w->var("bd2rhokst_a2"),*w->var("bd2rhokst_n2"));
    bd2phikst_mc_pdf = new RooIpatia2("bd2phikst_mc_pdf","bd2phikst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2phikst_l"),*w->var("bd2phikst_zeta"),*w->var("bd2phikst_fb"),*w->var("bd2phikst_sigma"),*w->var("bd2phikst_mu"),*w->var("bd2phikst_a"),*w->var("bd2phikst_n"),*w->var("bd2phikst_a2"),*w->var("bd2phikst_n2"));
    bs2phikst_mc_pdf = new RooIpatia2("bs2phikst_mc_pdf","bs2phikst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bs2phikst_l"),*w->var("bs2phikst_zeta"),*w->var("bs2phikst_fb"),*w->var("bs2phikst_sigma"),*w->var("bs2phikst_mu"),*w->var("bs2phikst_a"),*w->var("bs2phikst_n"),*w->var("bs2phikst_a2"),*w->var("bs2phikst_n2"));
    lb2pkpipi_mc_pdf = new RooIpatia2("lb2pkpipi_mc_pdf","lb2pkpipi_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("lb2pkpipi_l"),*w->var("lb2pkpipi_zeta"),*w->var("lb2pkpipi_fb"),*w->var("lb2pkpipi_sigma"),*w->var("lb2pkpipi_mu"),*w->var("lb2pkpipi_a"),*w->var("lb2pkpipi_n"),*w->var("lb2pkpipi_a2"),*w->var("lb2pkpipi_n2"));
  }
  RooArgusBG *part_reco_pdf = new RooArgusBG( "part_reco_pdf", "part_reco_pdf", *w->var("B_s0_DTF_B_s0_M"), *w->function("part_reco_m0"), *w->var("part_reco_c"), *w->var("part_reco_p") );

  RooAbsPdf *bkg_pdf;
  if ( usePoly ) {
    bkg_pdf = new RooPolynomial( "bkg_pdf", "bkg_pdf", *w->var("B_s0_DTF_B_s0_M"), RooArgList(*w->var("bkg_p1_p1")) );
  }
  else {
    bkg_pdf = new RooExponential( "bkg_pdf", "bkg_pdf", *w->var("B_s0_DTF_B_s0_M"), *w->var("bkg_exp_p1") );
  }

  w->loadSnapshot( fitName );

  TCanvas *canv;
  if (doCompPlot) {
    canv = new TCanvas("c","c",1600,1000);
    canv->Divide(4,3);
  }

  RooCategory *cat = (RooCategory*)w->cat("DataCat");
  for ( int c=0; c<cat->numTypes(); c++ ) {

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
    bs2kstkst_mc_pdf->Print("v");
    bd2kstkst_mc_pdf->Print("v");
    bd2phikst_mc_pdf->Print("v");
    bs2phikst_mc_pdf->Print("v");
    bd2rhokst_mc_pdf->Print("v");
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
    yields->remove( *w->function(Form("bs2phikst_y_%s",cat->getLabel())) );
    yields->remove( *w->function(Form("bd2rhokst_y_%s",cat->getLabel())) );
    yields->remove( *w->function(Form("lb2pkpipi_y_%s",cat->getLabel())) );

    TString dsetName;
    if ( cat->getLabel() == TString("HadronTOS2011") ) dsetName = "Data2011HadronTOS";
    if ( cat->getLabel() == TString("GlobalTIS2011") ) dsetName = "Data2011GlobalTIS";
    if ( cat->getLabel() == TString("HadronTOS2012") ) dsetName = "Data2012HadronTOS";
    if ( cat->getLabel() == TString("GlobalTIS2012") ) dsetName = "Data2012GlobalTIS";

    RooDataSet *oldData = (RooDataSet*)w->data(dsetName);
    RooDataSet *data = new RooDataSet( Form("n%s",dsetName.Data()), oldData->GetTitle(), oldData, RooArgSet( *w->var("B_s0_DTF_B_s0_M"), *w->var("eventNumber")) );
    //data->Print("v");
    //pdf->Print("v");

    nonyields->setAttribAll("Constant");
    yields->setAttribAll("Constant");

    RooStats::SPlot *sData = new RooStats::SPlot(Form("s%s",dsetName.Data()),"sData", *data, pdf, *yields, RooArgSet(*w->var("eventNumber")) );

    //cout << "Yields: " << endl;
    //cout << "bs2kstkst: " << w->var(Form("bs2kstkst_y_%s",cat->getLabel()))->getVal() << " " << sData->GetYieldFromSWeight(Form("bs2kstkst_y_%s",cat->getLabel())) << endl;
    //cout << "bd2kstkst: " << w->var(Form("bd2kstkst_y_%s",cat->getLabel()))->getVal() << " " << sData->GetYieldFromSWeight(Form("bd2kstkst_y_%s",cat->getLabel())) << endl;
    //cout << "bd2phikst: " << w->var(Form("bd2phikst_y_%s",cat->getLabel()))->getVal() << " " << sData->GetYieldFromSWeight(Form("bd2phikst_y_%s",cat->getLabel())) << endl;
    //cout << "part_reco: " << w->var(Form("part_reco_y_%s",cat->getLabel()))->getVal() << " " << sData->GetYieldFromSWeight(Form("part_reco_y_%s",cat->getLabel())) << endl;
    //cout << "bkg: " << w->var(Form("bkg_y_%s",cat->getLabel()))->getVal() << " " << sData->GetYieldFromSWeight(Form("bkg_y_%s",cat->getLabel())) << endl;

    RooDataSet *dataSW = new RooDataSet(Form("%s_swN",data->GetName()), Form("%s_sw",data->GetTitle()), data, RooArgSet( *w->var("B_s0_DTF_B_s0_M"), *w->var("eventNumber"), *w->var(Form("bs2kstkst_y_%s_sw",cat->getLabel()))), 0, Form("bs2kstkst_y_%s_sw",cat->getLabel()) );

    // now put these sWeights in a map to the event number
    for ( int i=0; i<dataSW->numEntries(); i++ ){
      RooArgSet  *obs = (RooArgSet*)dataSW->get(i);
      double      mass          = obs->getRealValue("B_s0_DTF_B_s0_M");
      ULong64_t   eventNumber   = boost::lexical_cast<ULong64_t>( obs->getRealValue("eventNumber") );
      double      weight        = dataSW->weight();
      if ( !scales.empty() ) weight  *= scales[c];

      if (sweightMap.find(eventNumber)!=sweightMap.end() ) {
        cout << "Something went wrong. Event number " << eventNumber << " is already in the map. I'm outta here!" << endl;
        exit(1);
      }
      sweightMap[eventNumber] = weight;
    }

    if ( doCompPlot ) {
      TH1D *oldSWs = new TH1D(Form("oldSWs%d",c), "oldSWs",100,-0.6,1.2);
      TH1D *newSWs = new TH1D(Form("newSWs%d",c), "newSWs",100,-0.6,1.2);
      RooDataSet *dataOriginal = (RooDataSet*)w->data(Form("%s_sw",dsetName.Data()));

      for ( int i=0; i<dataOriginal->numEntries(); i++ ) {
        RooArgSet *obs = (RooArgSet*)dataSW->get(i);

        double      mass          = obs->getRealValue("B_s0_DTF_B_s0_M");
        ULong64_t   eventNumber   = boost::lexical_cast<ULong64_t>( obs->getRealValue("eventNumber") );
        double      weight        = dataSW->weight();
        if ( !scales.empty() ) weight  *= scales[c];
        newSWs->Fill(weight);

        RooArgSet *oObs = (RooArgSet*)dataOriginal->get(i);

        double      omass          = oObs->getRealValue("B_s0_DTF_B_s0_M");
        ULong64_t   oeventNumber   = boost::lexical_cast<ULong64_t>( oObs->getRealValue("eventNumber") );
        double      oweight        = dataOriginal->weight();
        oldSWs->Fill(oweight);

        //if ( i%1000==0 ) {
          //cout << i << " " << eventNumber << " " << mass << " " << weight << " " << sData->GetSWeight(i, Form("bs2kstkst_y_%s",cat->getLabel()) ) << endl;
          //cout << i << " " << oeventNumber << " " << omass << " " << oweight << " " << endl;
        //}
      }

      RooPlot *plot = w->var("B_s0_DTF_B_s0_M")->frame();
      plot->SetTitle(cat->getLabel());
      plot->GetXaxis()->SetTitleSize(0.06);
      plot->GetYaxis()->SetTitleSize(0.06);
      data->plotOn(plot);
      pdf->plotOn(plot,LineWidth(1));

      RooPlot *plot2 = w->var("B_s0_DTF_B_s0_M")->frame();
      plot2->SetTitle("");
      plot2->GetXaxis()->SetTitleSize(0.06);
      plot2->GetYaxis()->SetTitleSize(0.06);
      if ( !scales.empty() ) dataSW->plotOn( plot2, Rescale(scales[c]) );
      else dataSW->plotOn(plot2);
      dataOriginal->plotOn(plot2, LineColor(kRed), MarkerColor(kRed), MarkerStyle(kOpenCircle),LineWidth(1));

      //RooPlot *plot3 = w->var(Form("bs2kstkst_y_%s_sw",cat->getLabel()))->frame(Range(-1,2.));
      //plot3->SetTitle("");
      //plot3->GetXaxis()->SetTitleSize(0.06);
      //plot3->GetYaxis()->SetTitleSize(0.06);
      //dataSW->plotOn(plot3);
      //dataOriginal->plotOn(plot3, LineColor(kRed), MarkerColor(kRed), MarkerStyle(kOpenCircle),LineWidth(1));
      newSWs->SetTitle("");
      newSWs->GetXaxis()->SetTitleSize(0.06);
      newSWs->GetYaxis()->SetTitleSize(0.06);
      newSWs->SetLineWidth(1);
      oldSWs->SetMarkerColor(kRed);
      oldSWs->SetLineColor(kRed);
      oldSWs->SetMarkerStyle(kOpenCircle);
      oldSWs->SetLineWidth(1);

      TLine *l = new TLine();
      l->SetLineColor(kBlue);
      l->SetLineWidth(3);

      TLatex *lat = new TLatex();
      lat->SetNDC();
      lat->SetTextSize(0.05);

      canv->cd(c+1);
      plot->Draw();
      lat->DrawLatex(0.6,0.8,cat->getLabel());
      canv->cd(c+5);
      plot2->Draw();
      l->DrawLine(5000,0,5800,0);
      canv->cd(c+9);
      //plot3->Draw();
      newSWs->Draw("LEP");
      oldSWs->Draw("LEPsame");
      canv->Update();

      delete newSWs;
      delete oldSWs;
      delete l;
      delete dataSW;
    }
  }

  delete bs2kstkst_mc_pdf;
  delete bd2kstkst_mc_pdf;
  delete bd2phikst_mc_pdf;
  delete bs2phikst_mc_pdf;
  delete bd2rhokst_mc_pdf;
  delete lb2pkpipi_mc_pdf;
  delete bkg_pdf;

  if ( doCompPlot ) {
    canv->Print(Form("plots/Toys/pdf/%s.pdf",compPlotName.Data()));
    canv->Print(Form("plots/Toys/png/%s.png",compPlotName.Data()));
    canv->Print(Form("plots/Toys/C/%s.C",compPlotName.Data()));
  }
  return sweightMap;
}

map<ULong64_t, double> getSubsetWeights( RooWorkspace *w, MassFitPlotter *plotter, TString dsetName, TString label, TString plotName ) {

  assert( w->data(dsetName) );
  //w->data(dsetName)->Print("v");

  //w->set("constrained_vars")->Print("v");
  RooArgSet *parameters = w->pdf("constrained_pdf")->getParameters(w->set("observables"));
  //parameters->Print("v");
  parameters->setAttribAll("Constant");
  // and release the others
  w->var("bs2kstkst_mu")->setConstant(false);
  w->var("bs2kstkst_sigma")->setConstant(false);
  w->var("bd2kstkst_mu")->setConstant(false);
  w->var("bd2kstkst_sigma")->setConstant(false);
  w->var("bd2phikst_mu")->setConstant(false);
  w->var("bd2phikst_sigma")->setConstant(false);
  w->var("bkg_exp_p1")->setConstant(false);
  w->var("part_reco_c")->setConstant(false);
  w->var("part_reco_p")->setConstant(false);
  w->var("yield_ratio_bs2phikst_o_bd2phikst")->setConstant(false);
  w->var("yield_ratio_bd2rhokst_o_bd2phikst")->setConstant(false);
  w->var("yield_ratio_lb2pkpipi_o_bd2phikst")->setConstant(false);
  RooCategory *cat = w->cat("DataCat");
  for ( int i=0; i < cat->numTypes(); i++ ) {
    cat->setIndex(i);
    w->var(Form("bs2kstkst_y_%s",cat->getLabel()))->setConstant(false);
    w->var(Form("bd2kstkst_y_%s",cat->getLabel()))->setConstant(false);
    w->var(Form("bd2phikst_y_%s",cat->getLabel()))->setConstant(false);
    w->var(Form("part_reco_y_%s",cat->getLabel()))->setConstant(false);
    w->var(Form("bkg_y_%s",cat->getLabel()))->setConstant(false);
  }
  w->loadSnapshot("constrained_pdf_fit");
  RooFitResult *fr = w->pdf("constrained_pdf")->fitTo( *w->data(dsetName), Constrain( *w->set("constrained_vars") ), Extended(), Save() );
  w->saveSnapshot(Form("constrained_%s_pdf_fit",label.Data()), *parameters);
  fr->SetName(Form("constrained_%s_pdf_fr",label.Data()));
  w->import(*fr);
  plotter->makeDataPlot( plotName, dsetName, "All" );

  vector<double> dataFracs;
  for (int i=0; i < cat->numTypes(); i++ ) {
    dataFracs.push_back( w->data("DataCombined")->sumEntries(Form("DataCat==%d",i)) / w->data(dsetName)->sumEntries(Form("DataCat==%d",i)) );
  }
  //cout << "Data fraction: " << w->data(dsetName)->numEntries() << " / " << w->data("DataCombined")->numEntries() <<  " = " << dataFrac << endl;

  return computeSWeightsFromFit( w, Form("constrained_%s_pdf_fit",label.Data()), true, plotName+"Comp", false,false, dataFracs );
}

void addVariantSWeightsToTree( map<ULong64_t, double> &sweightMap, TString branchName, TString fileName ) {

  TFile *tf = new TFile(fileName,"UPDATE");
  TTree *tree = (TTree*)tf->Get("AnalysisTree");

  // add branch
  double newWeight;
  TBranch *brNewWeight = tree->Branch( branchName, &newWeight, branchName+"/D" );
  // set event branch
  ULong64_t eventNumber;
  tree->SetBranchAddress( "eventNumber", &eventNumber );

  Long64_t nentries = tree->GetEntries();
  for ( int i=0; i<nentries; i++ ) {
    tree->GetEntry(i);
    if ( sweightMap.find( eventNumber ) == sweightMap.end() ) {
      cout << "Looking for the new sweight to go into branch " << branchName << " but I can't find the relevant eventNumber " << eventNumber << " in the map. I'm outta here!" << endl;
      exit(1);
    }
    newWeight = sweightMap[eventNumber];
    // fill the new branch
    brNewWeight->Fill();
  }
  tree->Write();
  delete tf;
}

// this method is for adding all the sweights at once
void addVariantSWeightsToTree( map< TString, map<ULong64_t, double> > &sweightMap, TString fileName ) {

  TFile *tf = new TFile(fileName,"UPDATE");
  TTree *tree = (TTree*)tf->Get("AnalysisTree");

  map<TString, TBranch*> branchHolder;
  map<TString, double >  varHolder;

  for ( auto br=sweightMap.begin(); br != sweightMap.end(); br++ ) {
    TString branchName = br->first;
    varHolder[branchName] = -999.; // initialise
    branchHolder[branchName] = tree->Branch( branchName, &varHolder[branchName], branchName+"/D" );
  }

  ULong64_t eventNumber;
  tree->SetBranchAddress( "eventNumber", &eventNumber );

  Long64_t nentries = tree->GetEntries();
  for ( int i=0; i<nentries; i++ ) {
    tree->GetEntry(i);

    for ( auto br=branchHolder.begin(); br!=branchHolder.end(); br++ ) {
      TString branchName = br->first;
      if ( sweightMap[branchName].find(eventNumber) == sweightMap[branchName].end() ) {
        cout << "Looking for the new sweight to go into branch " << branchName << " but I can't find the relevant eventNumber " << eventNumber << " in the map. I'm outta here!" << endl;
        exit(1);
      }
      varHolder[branchName] = sweightMap[branchName][eventNumber];
      // fill the branch
      branchHolder[branchName]->Fill();
    }
  }
  tree->Write();
  delete tf;
}

void trimTree( TString oldName, TString newName ) {

  cout << "Trimming the tree" << endl;
  TFile *oldfile = new TFile(oldName);
  TTree *oldtree = (TTree*)oldfile->Get("AnalysisTree");

  bool pass_all;
  int itype;

  // turn off all branches
  oldtree->SetBranchStatus("*",0);
  // turn back on branches we want
  oldtree->SetBranchStatus("B_s0_DTF_B_s0_M", 1);
  oldtree->SetBranchStatus("B_s0_DTF_B_s0_MERR", 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST1_M", 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST2_M", 1);
  oldtree->SetBranchStatus("B_s0_DTF_B_s0_CosTheta1", 1);
  oldtree->SetBranchStatus("B_s0_DTF_B_s0_CosTheta2", 1);
  oldtree->SetBranchStatus("B_s0_DTF_B_s0_Phi1", 1);
  oldtree->SetBranchStatus("B_s0_DTF_TAU", 1);
  oldtree->SetBranchStatus("B_s0_DTF_TAUERR", 1);
  oldtree->SetBranchStatus("B_s0_TAGDECISION_OS", 1);
  oldtree->SetBranchStatus("B_s0_TAGOMEGA_OS", 1);
  oldtree->SetBranchStatus("B_s0_SS_nnetKaon_DEC", 1);
  oldtree->SetBranchStatus("B_s0_SS_nnetKaon_PROB", 1);
  oldtree->SetBranchStatus("B_s0_L0Global_TIS", 1);
  oldtree->SetBranchStatus("B_s0_L0HadronDecision_TOS", 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST1_K_PX" , 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST1_K_PY" , 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST1_K_PZ" , 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST2_K_PX" , 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST2_K_PY" , 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST2_K_PZ" , 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST2_PI_PX" , 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST2_PI_PY" , 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST2_PI_PZ" , 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST1_PI_PX" , 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST1_PI_PY" , 1);
  oldtree->SetBranchStatus("B_s0_DTF_KST1_PI_PZ" , 1);
  oldtree->SetBranchStatus("eventNumber", 1);
  oldtree->SetBranchStatus("sweight", 1);
  oldtree->SetBranchStatus("itype", 1);
  oldtree->SetBranchStatus("pass_all", 1);
  oldtree->SetBranchStatus("category", 1);

  oldtree->SetBranchAddress("itype", &itype);
  oldtree->SetBranchAddress("pass_all", &pass_all);

  TFile *newfile = new TFile(newName,"RECREATE");
  TTree *newtree = oldtree->CloneTree(0);

  Long64_t nentries = oldtree->GetEntries();
  for ( Long64_t i=0; i<nentries; i++ ) {
    oldtree->GetEntry(i);
    if (i%10000==0){
      cout << "\t" << i << " / " << nentries << endl;
    }
    if ( itype>0 && pass_all ) newtree->Fill();
  }
  //newtree->Print();
  newtree->AutoSave();
  delete oldfile;
  delete newfile;
}

int main(int argc, char **argv) {

  bool runTrimming     = true;
  bool runChecks       = false;
  bool runStatToys     = false;
  int  nStatToys       = 20;
  bool runYieldShift   = true;
  bool runYieldToys    = false;
  int  nYieldToys      = 100;
  int  jobn            = -1;
  bool runReParam      = true;
  bool runSubsetRefit  = true;
  bool putSWBackInTree = true;

  TString trimmedFileName = "root/AnalysisOutTrimmedSmall.root";

  if (argc > 1){
    // we want to run a toy file
    runYieldShift = false;
    runYieldToys = true;
    runReParam = false;
    runSubsetRefit = false;

    nYieldToys = boost::lexical_cast<int>( argv[1] );
    jobn       = boost::lexical_cast<int>( argv[2] );

    trimmedFileName = Form("root/AnalysisOutTrimmedJob%d.root",jobn);
  }

  if ( runTrimming ) trimTree( "root/AnalysisOutWSWeights.root", trimmedFileName );

  //gROOT->ProcessLine(".x ~/lhcbStyle.C");
  gROOT->ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C");
  system("mkdir -p plots/Toys/png");
  system("mkdir -p plots/Toys/pdf");
  system("mkdir -p plots/Toys/C");

  RooMsgService::instance().setGlobalKillBelow(FATAL);
  RooMsgService::instance().setSilentMode(true);

  TFile *inf = TFile::Open("root/MassFit/MassFitResultWSWeights.root");
  RooWorkspace *w = (RooWorkspace*)inf->Get("w");

  MassFitPlotter *plotter = new MassFitPlotter( w, "Toys" );

  // do some check plots to make sure things looks fairly reasonable
  if ( runChecks ) {
    w->loadSnapshot("constrained_pdf_fit");
    plotter->makeDataPlot( "DataFit", "DataCombined", "All" );
    // check we get the same sWeights
    computeSWeightsFromFit( w, "constrained_pdf_fit", true, "DataComp");
  }

  // place to store all our new sweights which we eventually put back into trimmed tree
  map< TString, map<ULong64_t, double> > sweightStore;

  // throw a few toys stat variation toys
  if ( runStatToys ) {
    cout << "Running Stat Toys" << endl;
    for (int i=0; i<nStatToys; i++) {
      cout << "Toy " << i+1 << " / " << nStatToys << endl;
      // generate a toy
      RooDataSet *toy = w->pdf("constrained_pdf")->generate(RooArgSet(*w->var("B_s0_DTF_B_s0_M"),*w->cat("DataCat")),w->data("DataCombined")->numEntries(),Extended());
      TString toyName = TString(Form("Toy%d",i));
      toy->SetName(toyName);
      w->import(*toy);
      // fix the same and sensible starting values
      w->loadSnapshot("constrained_pdf_fit");
      // fit the toy
      RooFitResult *fr = w->pdf("constrained_pdf")->fitTo( *toy, Constrain( *w->set("constrained_vars") ), Extended(), Save() );
      // save the fit result
      RooArgSet *parameters = w->pdf("constrained_pdf")->getParameters(w->set("observables"));
      w->saveSnapshot( Form("toy%d_constrained_pdf_fit",i), *parameters );
      fr->SetName( Form("toy%d_constrained_pdf_fr",i) );
      w->import(*fr);
      // plot the toy
      plotter->makeDataPlot( Form("Toy%dFit",i), Form("Toy%d",i), "All" );
      // compute sWeights for this version of the PDF
      //map<ULong64_t, double> sweightMap = computeSWeightsFromFit( w, Form("toy%d_constrained_pdf_fit",i), true, Form("Toy%dComp",i) );
      //addVariantSWeightsToTree( sweightMap, Form("stat_toy%d_sw",i), trimmedFileName );
      sweightStore[Form("stat_toy%d_sw",i)] = computeSWeightsFromFit( w, Form("toy%d_constrained_pdf_fit",i), true, Form("Toy%dComp",i) );
    }
  }

  // yield shift systematic
  if ( runYieldShift ) {

    cout << "Running yield shift systematic" << endl;
    // shift all up by 1 sigma
    shiftYields( w, "constrained_pdf_fit", "yield_shift_up1sigma",  1 );
    // plot
    w->loadSnapshot( "yield_shift_up1sigma" );
    plotter->makeDataPlot( "ShiftUp1", "DataCombined", "All" );
    // compute sWeights for this verion
    sweightStore["shift_yield_up1_sw"] = computeSWeightsFromFit( w, "yield_shift_up1sigma", true, "ShiftUp1Comp" );
    w->loadSnapshot( "constrained_pdf_fit" );

    // shift all down by 1 sigma
    shiftYields( w, "constrained_pdf_fit", "yield_shift_dn1sigma", -1 );
    // plot
    w->loadSnapshot( "yield_shift_dn1sigma" );
    plotter->makeDataPlot( "ShiftDn1", "DataCombined", "All" );
    // compute sWeights for this version
    sweightStore["shift_yield_dn1_sw"] = computeSWeightsFromFit( w, "yield_shift_dn1sigma", true, "ShiftDn1Comp" );
    w->loadSnapshot( "constrained_pdf_fit" );
  }

  // then yield randomisation systematic
  if ( runYieldToys ) {
    cout << "Running yield randomisation systematic" << endl;
    for ( int i=0; i<nYieldToys; i++ ) {
      cout << "Toy " << i << " / " << nYieldToys << endl;
      // randomize yield
      shiftYields( w, "constrained_pdf_fit", Form("yield_random_toy%d",i), 0 );
      // compute sWeights
      sweightStore[ Form("random_yield_toy%d_sw",i) ] = computeSWeightsFromFit( w, Form("yield_random_toy%d",i) );
      w->loadSnapshot( "constrained_pdf_fit" );
    }
  }

  // then do change the parametrisations e.g. Gaussian (or CB) instead of Ipatia2 and flat bkg instead of exp
  if ( runReParam ) {
    cout << "Running reparam fit" << endl;
    // make DCB signal shapes, fit them and plot them
    makeDCBShape( w, plotter, "bs2kstkst", "Bs2KstKst" );
    makeDCBShape( w, plotter, "bd2kstkst", "Bd2KstKst" );
    makeDCBShape( w, plotter, "bd2phikst", "Bd2PhiKst" );
    makeDCBShape( w, plotter, "bs2phikst", "Bs2PhiKst" );
    makeDCBShape( w, plotter, "bd2rhokst", "Bd2RhoKst" );
    makeDCBShape( w, plotter, "lb2pkpipi", "Lb2pKpipi" );

    // make alternate background shapes
    makePolyShape( w, "bkg_p0_pdf", 0 );
    makePolyShape( w, "bkg_p1_pdf", 1 );

    // make total shape, fit to data and plot
    makeFitAndPlotTotalShape( w, plotter, true, false ); // just switch for DCBs
    makeFitAndPlotTotalShape( w, plotter, false, true ); // just switch bkg
    makeFitAndPlotTotalShape( w, plotter, true, true  ); // switch both

    // extract the sweights for each (problematic as this expects RooIpatia etc. so needs fixing)
    sweightStore["altsigmodel_sw"] = computeSWeightsFromFit( w, "constrained_swcb_pdf_fit", false, "c", true, false );
    sweightStore["altbkgmodel_sw"] = computeSWeightsFromFit( w, "constrained_swbkg_pdf_fit", false, "c", false, true );
    //sweightStore["altsigandbkgmodel_sw"] = computeSWeightsFromFit( w, "constrained_swcb_swbkg_pdf_fit", false, "c", true, true );
  }

  // then an uncertainty itself for the sWeights. e.g. fit on sub samples and recompute (polarity, cos(theta) bins, year)
  if ( runSubsetRefit ) {
    cout << "Running subset fits" << endl;
    sweightStore["magup_sw"]      = getSubsetWeights(w, plotter, "DataCombMagUp", "magup", "MagUpFit" );
    sweightStore["magdn_sw"]      = getSubsetWeights(w, plotter, "DataCombMagDn", "magdn", "MagDnFit" );
    sweightStore["phi0_sw"]       = getSubsetWeights(w, plotter, "DataCombPhi0" , "phi0" , "Phi0Fit"  );
    sweightStore["phi1_sw"]       = getSubsetWeights(w, plotter, "DataCombPhi1" , "phi1" , "Phi1Fit"  );
    sweightStore["phi2_sw"]       = getSubsetWeights(w, plotter, "DataCombPhi2" , "phi2" , "Phi2Fit"  );
    sweightStore["phi3_sw"]       = getSubsetWeights(w, plotter, "DataCombPhi3" , "phi3" , "Phi3Fit"  );
    sweightStore["costheta0_sw"]  = getSubsetWeights(w, plotter, "DataCombCosTheta0" , "costheta0" , "CosTheta0Fit"  );
    sweightStore["costheta1_sw"]  = getSubsetWeights(w, plotter, "DataCombCosTheta1" , "costheta1" , "CosTheta1Fit"  );
    sweightStore["costheta2_sw"]  = getSubsetWeights(w, plotter, "DataCombCosTheta2" , "costheta2" , "CosTheta2Fit"  );
    sweightStore["costheta3_sw"]  = getSubsetWeights(w, plotter, "DataCombCosTheta3" , "costheta3" , "CosTheta3Fit"  );
  }

  // put the new sweights back in the tree
  if ( putSWBackInTree ) addVariantSWeightsToTree( sweightStore, trimmedFileName );

  inf->Close();

  return 0;

}

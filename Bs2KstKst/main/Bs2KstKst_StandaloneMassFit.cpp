#include <iostream>

#include "TFile.h"
#include "TCanvas.h"

#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooConstVar.h"
#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooExponential.h"
#include "RooGaussian.h"

#include "RooStats/SPlot.h"

#include "RooIpatia2.h"

using namespace std;
using namespace RooFit;

void makePdf( RooWorkspace *w ) {

  // bkg
  w->factory("Exponential::bkg_pdf( B_s0_DTF_B_s0_M, bkg_exp_p1[-0.002,-0.004,0.] )");

  // sig
  w->factory("bs2kstkst_l       [ -5    ,-20    ,0]");
  w->factory("bs2kstkst_zeta    [ 1.e-5           ]");
  w->factory("bs2kstkst_fb      [ 0.              ]");
  w->factory("bs2kstkst_sigma   [ 15    ,10   ,100]");
  w->factory("bs2kstkst_mu      [ 5350  ,5400     ]");
  w->factory("bs2kstkst_a       [ 2.5  , 0    ,10 ]");
  w->factory("bs2kstkst_n       [ 2.5  , 0    ,10 ]");
  w->factory("bs2kstkst_a2      [ 2.5  , 0    ,10 ]");
  w->factory("bs2kstkst_n2      [ 2.5  , 0    ,10 ]");

  w->var("bs2kstkst_zeta")->setConstant(true);
  w->var("bs2kstkst_fb")->setConstant(true);

  RooIpatia2 *pdf = new RooIpatia2("bs2kstkst_pdf","bs2kstkst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bs2kstkst_l"),*w->var("bs2kstkst_zeta"),*w->var("bs2kstkst_fb"),*w->var("bs2kstkst_sigma"),*w->var("bs2kstkst_mu"),*w->var("bs2kstkst_a"),*w->var("bs2kstkst_n"),*w->var("bs2kstkst_a2"),*w->var("bs2kstkst_n2"));

  w->import(*pdf);
  delete pdf;

  //w->factory("Gaussian::bs2kstkst_pdf( B_s0_DTF_B_s0_M, bs2kstkst_mean[5320,5380], bs2kstkst_sigma[1,20] )");


  // other sig
  w->factory("Gaussian::bd2kstkst_pdf( B_s0_DTF_B_s0_M, bd2kstkst_mean[5250,5300], bd2kstkst_sigma[1,20] )");

  RooArgList *pdfs = new RooArgList();
  RooArgList *yields = new RooArgList();

  pdfs->add( *w->pdf("bkg_pdf") );
  pdfs->add( *w->pdf("bs2kstkst_pdf") );
  pdfs->add( *w->pdf("bd2kstkst_pdf") );

  w->factory( "bkg_y[1e3,1e5]" );
  w->factory( "bs2kstkst_y[0,20e3]" );
  w->factory( "bd2kstkst_y[0,3000]" );

  yields->add( *w->var("bkg_y") );
  yields->add( *w->var("bs2kstkst_y") );
  yields->add( *w->var("bd2kstkst_y") );

  // total
  RooAddPdf *addpdf = new RooAddPdf( "pdf","pdf", *pdfs, *yields);
  w->import(*addpdf);
  delete addpdf;
  //w->factory("SUM::pdf( bkg_y[1e3,1e5]*bkg_pdf, bs2kstkst_y[0,20e3]*bs2kstkst_pdf, bd2kstkst_y[0,3000]*bd2kstkst_pdf )");

}

void fitIpatia( RooWorkspace *w, TString pdf_name, TString data_name ) {

  RooRealVar *mass = (RooRealVar*)w->var("B_s0_DTF_B_s0_M");
  RooDataSet *data = (RooDataSet*)w->data(data_name);

  RooRealVar  *l       = new RooRealVar ( Form("%s_l",pdf_name.Data())    ,"", -5, -20, -1.);
  RooConstVar *zeta    = new RooConstVar( Form("%s_zeta",pdf_name.Data()) ,"",0.              );
  RooConstVar *fb      = new RooConstVar( Form("%s_fb",pdf_name.Data())   ,"",0.              );
  RooRealVar  *sigma   = new RooRealVar ( Form("%s_sigma",pdf_name.Data()),"",15    ,10   ,20 );
  RooRealVar  *mu      = new RooRealVar ( Form("%s_mu",pdf_name.Data())   ,"",5350  ,5380     );
  RooRealVar  *a       = new RooRealVar ( Form("%s_a",pdf_name.Data())    ,"",2.5  , 0    ,10 );
  RooRealVar  *n       = new RooRealVar ( Form("%s_n",pdf_name.Data())    ,"",2.5  , 0    ,10 );
  RooRealVar  *a2      = new RooRealVar ( Form("%s_a2",pdf_name.Data())   ,"",2.5  , 0    ,10 );
  RooRealVar  *n2      = new RooRealVar ( Form("%s_n2",pdf_name.Data())   ,"",2.5  , 0    ,10 );

  RooIpatia2 *pdf = new RooIpatia2( Form("%s_pdf",pdf_name.Data()), "pdf",*mass,*l,*zeta,*fb,*sigma,*mu,*a,*n,*a2,*n2);

  pdf->fitTo(*data);
  w->import(*pdf);

}

int main (int argc, char **argv) {

  TFile *tf = TFile::Open("tmp/DataSets.root");
  RooWorkspace *w = (RooWorkspace*)tf->Get("w");

  RooDataSet *Data = (RooDataSet*)w->data("Data2011")->Clone("Data");
  Data->append( *((RooDataSet*)w->data("Data2012")) );

  RooDataSet *Bs2Kst0Kst0_MC = (RooDataSet*)w->data("Bs2Kst0Kst0_MC2011")->Clone("Bs2KstKst0_MC");
  Bs2Kst0Kst0_MC->append( *((RooDataSet*)w->data("Bs2Kst0Kst0_MC2012")) );

  RooDataSet *Bs2Kst0Kst01430_MC = (RooDataSet*)w->data("Bs2Kst0Kst01430_MC2011")->Clone("Bs2KstKst0_MC");
  Bs2Kst0Kst01430_MC->append( *((RooDataSet*)w->data("Bs2Kst0Kst01430_MC2012")) );

  RooDataSet *Bs2Kst01430Kst01430_MC = (RooDataSet*)w->data("Bs2Kst01430Kst01430_MC2011")->Clone("Bs2KstKst0_MC");
  Bs2Kst01430Kst01430_MC->append( *((RooDataSet*)w->data("Bs2Kst01430Kst01430_MC2012")) );

  RooDataSet *Bd2Kst0Kst0_MC = (RooDataSet*)w->data("Bd2Kst0Kst0_MC2011")->Clone("Bs2KstKst0_MC");
  Bd2Kst0Kst0_MC->append( *((RooDataSet*)w->data("Bd2Kst0Kst0_MC2012")) );

  RooDataSet *Bd2PhiKst0_MC = (RooDataSet*)w->data("Bd2PhiKst0_MC2011")->Clone("Bs2KstKst0_MC");
  Bd2PhiKst0_MC->append( *((RooDataSet*)w->data("Bd2PhiKst0_MC2012")) );

  RooDataSet *Bs2PhiKst0_MC = (RooDataSet*)w->data("Bs2PhiKst0_MC2011")->Clone("Bs2KstKst0_MC");
  Bs2PhiKst0_MC->append( *((RooDataSet*)w->data("Bs2PhiKst0_MC2012")) );

  RooDataSet *Bd2RhoKst0_MC = (RooDataSet*)w->data("Bd2RhoKst0_MC2011")->Clone("Bs2KstKst0_MC");
  Bd2RhoKst0_MC->append( *((RooDataSet*)w->data("Bd2RhoKst0_MC2012")) );

  RooDataSet *Lb2ppipipi_MC = (RooDataSet*)w->data("Lb2ppipipi_MC2011")->Clone("Bs2KstKst0_MC");
  Lb2ppipipi_MC->append( *((RooDataSet*)w->data("Lb2ppipipi_MC2012")) );

  RooDataSet *Lb2pKpipi_MC = (RooDataSet*)w->data("Lb2pKpipi_MC2011")->Clone("Bs2KstKst0_MC");
  Lb2pKpipi_MC->append( *((RooDataSet*)w->data("Lb2pKpipi_MC2012")) );


  w->import(*Data);
  w->import(*Bs2Kst0Kst0_MC);
  w->import(*Bs2Kst0Kst01430_MC);
  w->import(*Bs2Kst01430Kst01430_MC);
  w->import(*Bd2Kst0Kst0_MC);
  w->import(*Bd2PhiKst0_MC);
  w->import(*Bs2PhiKst0_MC);
  w->import(*Bd2RhoKst0_MC);
  w->import(*Lb2ppipipi_MC);
  w->import(*Lb2pKpipi_MC);

  RooRealVar *mass = (RooRealVar*)w->var("B_s0_DTF_B_s0_M");

  fitIpatia( w, "bs2kstkst_mc", "Bs2KstKst0_MC");

  // Make the PDF here
  RooRealVar *p1 = new RooRealVar("p1","p1",-0.002,-0.004,0.);
  RooExponential *exp = new RooExponential("exp","exp",*mass,*p1);
  //RooRealVar *m1 = new RooRealVar("m1","m1",5320,5380);
  //RooRealVar *s1 = new RooRealVar("s1","s1",1,20);
  //RooGaussian *sig = new RooGaussian("sig","sig",*mass,*m1,*s1);
  RooRealVar *m2 = new RooRealVar("m2","m2",5320,5380);
  RooRealVar *s2 = new RooRealVar("s2","s2",1,20);
  RooGaussian *sig_bd = new RooGaussian("sig_bd","sig_bd",*mass,*m2,*s2);

  //
  RooRealVar *bs2kstkst_l       = new RooRealVar( "bs2kstkst_l"    ,"", -5, -20, -1.);
  RooConstVar *bs2kstkst_zeta   = new RooConstVar( "bs2kstkst_zeta","",0.              );
  RooConstVar *bs2kstkst_fb     = new RooConstVar( "bs2kstkst_fb"  ,"",0.              );
  RooRealVar *bs2kstkst_sigma   = new RooRealVar( "bs2kstkst_sigma","",15    ,10   ,20 );
  RooRealVar *bs2kstkst_mu      = new RooRealVar( "bs2kstkst_mu"   ,"",5350  ,5380     );
  RooRealVar *bs2kstkst_a       = new RooRealVar( "bs2kstkst_a"    ,"",2.5  , 0    ,10 );
  RooRealVar *bs2kstkst_n       = new RooRealVar( "bs2kstkst_n"    ,"",2.5  , 0    ,10 );
  RooRealVar *bs2kstkst_a2      = new RooRealVar( "bs2kstkst_a2"   ,"",2.5  , 0    ,10 );
  RooRealVar *bs2kstkst_n2      = new RooRealVar( "bs2kstkst_n2"   ,"",2.5  , 0    ,10 );

  RooIpatia2 *sig = new RooIpatia2("sig","sig",*mass,*bs2kstkst_l,*bs2kstkst_zeta,*bs2kstkst_fb,*bs2kstkst_sigma,*bs2kstkst_mu,*bs2kstkst_a,*bs2kstkst_n,*bs2kstkst_a2,*bs2kstkst_n2);

  RooRealVar *bkg_y = new RooRealVar("bkg_y","bkg_y",10e3,10e5);
  RooRealVar *sig_y = new RooRealVar("sig_y","sig_y",0,20e3);
  RooRealVar *sig_bd_y = new RooRealVar("sig_bd_y","sig_bd_y",0,3000);

  RooArgList *pdfs = new RooArgList();
  RooArgList *yields = new RooArgList();

  pdfs->add( *exp );
  pdfs->add( *sig );
  pdfs->add( *sig_bd );

  yields->add( *bkg_y );
  yields->add( *sig_y );
  yields->add( *sig_bd_y );

  RooAddPdf *pdf = new RooAddPdf("pdf","pdf",*pdfs,*yields);

  pdf->fitTo(*Data, Extended() );

  RooPlot *plot = mass->frame();
  Data->plotOn(plot);
    // set fit params constant;
  pdf->plotOn(plot);

  TCanvas *c = new TCanvas();
  plot->Draw();
  c->Print("tmp/mass.pdf");

  // Plots Kst Ms with no sweights
  TCanvas *c1 = new TCanvas("c1","c1",800,1200);
  c1->Divide(1,2);
  c1->cd(1);
  RooPlot *c1p1 = w->var("B_s0_DTF_KST1_M")->frame();
  Data->plotOn(c1p1);
  c1p1->Draw();
  c1->cd(2);
  RooPlot *c1p2 = w->var("B_s0_DTF_KST2_M")->frame();
  Data->plotOn(c1p2);
  c1p2->Draw();
  c1->Print("tmp/nosw.pdf");

  // set fit params constant
  p1->setConstant(true);
  //m1->setConstant(true);
  //s1->setConstant(true);
  bs2kstkst_l->setConstant(true);
  //bs2kstkst_zeta->setConstant(true);
  //bs2kstkst_fb->setConstant(true);
  bs2kstkst_sigma->setConstant(true);
  bs2kstkst_mu->setConstant(true);
  bs2kstkst_a->setConstant(true);
  bs2kstkst_n->setConstant(true);
  bs2kstkst_a2->setConstant(true);
  bs2kstkst_n2->setConstant(true);
  m2->setConstant(true);
  s2->setConstant(true);

  RooStats::SPlot *sData = new RooStats::SPlot("sData","sData", *Data, pdf, *yields);

  w->import(*sData);
  w->import(*Data,Rename("Data_wsweights"));

  RooDataSet *swdata = new RooDataSet("Data_wsweights", "Data", Data, *Data->get(), 0 , "sig_y_sw");
  // Plots Kst Ms with no sweights
  TCanvas *c2 = new TCanvas("c2","c2",800,1200);
  c2->Divide(1,2);
  c2->cd(1);
  RooPlot *c2p1 = w->var("B_s0_DTF_KST1_M")->frame();
  swdata->plotOn(c2p1);
  c2p1->Draw();
  c2->cd(2);
  RooPlot *c2p2 = w->var("B_s0_DTF_KST2_M")->frame();
  swdata->plotOn(c2p2);
  c2p2->Draw();
  c2->Print("tmp/withsw.pdf");


  tf->Close();
  return 0;
}

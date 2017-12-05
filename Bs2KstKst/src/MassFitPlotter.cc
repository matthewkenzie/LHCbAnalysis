#include <iostream>

#include "TROOT.h"
#include "TLine.h"
#include "TBox.h"
#include "TAxis.h"
#include "TColor.h"
#include "TMath.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include "TPad.h"

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

#include "RooStats/SPlot.h"

#include "MassFitPlotter.h"

using namespace std;
using namespace RooFit;
using namespace TMath;

Bs2KstKst::MassFitPlotter::MassFitPlotter(RooWorkspace *_w, TString _outdirname):
  w(_w),
  outdirname(_outdirname)
{
  gROOT->ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C");
  system(Form("mkdir -p plots/%s/pdf",outdirname.Data()));
  system(Form("mkdir -p plots/%s/png",outdirname.Data()));
  system(Form("mkdir -p plots/%s/C",outdirname.Data()));
  chooseColorScheme(0);
}

Bs2KstKst::MassFitPlotter::~MassFitPlotter(){}

void Bs2KstKst::MassFitPlotter::chooseColorScheme( int i) {

  lineColor.clear();
  lineStyle.clear();
  lineWidth.clear();
  fillColor.clear();
  fillStyle.clear();
  drawOpt.clear();

  if ( i==0 ) {
    lineColor.push_back(kRed-3);
    lineColor.push_back(kMagenta-3);
    lineColor.push_back(kCyan-3);
    lineColor.push_back(kOrange-3);
    lineColor.push_back(kViolet+1);
    lineColor.push_back(kBlue-7);
    lineColor.push_back(kTeal-7);
    lineColor.push_back(kGreen+1);

    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);

    lineWidth.push_back(2);
    lineWidth.push_back(2);
    lineWidth.push_back(2);
    lineWidth.push_back(2);
    lineWidth.push_back(2);
    lineWidth.push_back(2);
    lineWidth.push_back(2);
    lineWidth.push_back(2);

    fillColor.push_back(kRed-3);
    fillColor.push_back(kMagenta-3);
    fillColor.push_back(kCyan-3);
    fillColor.push_back(kOrange-3);
    fillColor.push_back(kViolet+1);
    fillColor.push_back(kBlue-7);
    fillColor.push_back(kTeal-7);
    fillColor.push_back(kGreen+1);

    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);

    drawOpt.push_back("F");
    drawOpt.push_back("F");
    drawOpt.push_back("F");
    drawOpt.push_back("F");
    drawOpt.push_back("F");
    drawOpt.push_back("F");
    drawOpt.push_back("F");
    drawOpt.push_back("F");
    drawOpt.push_back("F");
  }
  else if ( i==1 ) {
    lineColor.push_back( TColor::GetColor("#006d2c") );
    lineColor.push_back( TColor::GetColor("#2ca25f") );
    lineColor.push_back( TColor::GetColor("#66c2a4") );
    lineColor.push_back( TColor::GetColor("#b2e2e2") );
    lineColor.push_back( TColor::GetColor("#bdc9e1") );
    lineColor.push_back( TColor::GetColor("#74a9cf") );
    lineColor.push_back( TColor::GetColor("#2b8cbe") );
    lineColor.push_back( TColor::GetColor("#045a8d") );

    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);

    lineWidth.push_back(3);
    lineWidth.push_back(3);
    lineWidth.push_back(3);
    lineWidth.push_back(3);
    lineWidth.push_back(3);
    lineWidth.push_back(3);
    lineWidth.push_back(3);
    lineWidth.push_back(3);

    fillColor.push_back( TColor::GetColor("#006d2c") );
    fillColor.push_back( TColor::GetColor("#2ca25f") );
    fillColor.push_back( TColor::GetColor("#66c2a4") );
    fillColor.push_back( TColor::GetColor("#b2e2e2") );
    fillColor.push_back( TColor::GetColor("#bdc9e1") );
    fillColor.push_back( TColor::GetColor("#74a9cf") );
    fillColor.push_back( TColor::GetColor("#2b8cbe") );
    fillColor.push_back( TColor::GetColor("#045a8d") );

    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);

    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
  }
  else if ( i==2 ) {
    //lineColor.push_back( TColor::GetColor("#016c59") );
    //lineColor.push_back( TColor::GetColor("#d0d1e6") );
    //lineColor.push_back( TColor::GetColor("#67a9cf") );
    //lineColor.push_back( TColor::GetColor("#014636") );
    //lineColor.push_back( TColor::GetColor("#ece2f0") );
    //lineColor.push_back( TColor::GetColor("#3690c0") );
    //lineColor.push_back( TColor::GetColor("#02818a") );
    //lineColor.push_back( TColor::GetColor("#a6bddb") );

    lineColor.push_back(1);
    lineColor.push_back(1);
    lineColor.push_back(1);
    lineColor.push_back(1);
    lineColor.push_back(1);
    lineColor.push_back(1);
    lineColor.push_back(1);
    lineColor.push_back(1);

    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);
    lineStyle.push_back(1);

    lineWidth.push_back(1);
    lineWidth.push_back(1);
    lineWidth.push_back(1);
    lineWidth.push_back(1);
    lineWidth.push_back(1);
    lineWidth.push_back(1);
    lineWidth.push_back(1);
    lineWidth.push_back(1);

    fillColor.push_back( TColor::GetColor("#016c59") );
    fillColor.push_back( TColor::GetColor("#d0d1e6") );
    fillColor.push_back( TColor::GetColor("#67a9cf") );
    fillColor.push_back( TColor::GetColor("#014636") );
    fillColor.push_back( TColor::GetColor("#ece2f0") );
    fillColor.push_back( TColor::GetColor("#3690c0") );
    fillColor.push_back( TColor::GetColor("#02818a") );
    fillColor.push_back( TColor::GetColor("#a6bddb") );

    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);
    fillStyle.push_back(1001);

    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
    drawOpt.push_back("LF");
  }
  else {
    cout << "Color scheme " << i << " not recognised" << endl;
    exit(1);
  }
}

void Bs2KstKst::MassFitPlotter::plot( TString var, TString data, TString pdf, int resid, bool project ) {


  if ( pdf!="" ) w->loadSnapshot(Form("%s_fit",pdf.Data()));

  RooPlot *plot = w->var(var)->frame();
  w->data(data)->plotOn(plot);
  if ( pdf!="" ) {
    if (project) w->pdf(pdf)->plotOn(plot, ProjWData(*w->cat("DataCat"),*w->data(data)) );
    else w->pdf(pdf)->plotOn(plot);
  }

  TString name = "v" + var + "_d" + data;
  if ( pdf!="" ) name += "_p" + pdf;

  if ( pdf=="" ) resid = 0;

  makePlot( plot, name, resid );
}

void Bs2KstKst::MassFitPlotter::makeBkgPlot( TString outfName ) {
  w->loadSnapshot("part_reco_bkg_self_pdf_fit");

  RooPlot *plot = w->var("B_s0_DTF_B_s0_M_forPartReco")->frame();

  w->data("PartReco")->plotOn(plot);
  w->pdf("part_reco_bkg_self_pdf")->plotOn(plot, Components("bkg_pr_pdf"), LineColor(kRed), LineStyle(kDashed) );
  w->pdf("part_reco_bkg_self_pdf")->plotOn(plot, Components("part_reco_self_pdf"), LineColor(kBlue+2), LineStyle(kDashed) );
  w->pdf("part_reco_bkg_self_pdf")->plotOn(plot);

  makePlot( plot, outfName, 2 );
}

void Bs2KstKst::MassFitPlotter::makeDataPlot( TString outfName, TString dsetName, TString catName, bool paperStyle ) {

  //w->loadSnapshot("pdf_fit");
  //w->loadSnapshot("constrained_pdf_fit");

  // make a legend
  TLegend *leg = new TLegend(0.56,0.28,0.92,0.92);
  leg->SetFillColor(0);
  leg->SetLineColor(0);

  // make a plot
  RooPlot *plot = w->var("B_s0_DTF_B_s0_M")->frame();
  if ( xtitle!="" ) plot->GetXaxis()->SetTitle(xtitle);
  if ( ytitle!="" ) plot->GetYaxis()->SetTitle(ytitle);

  // keep this command arg as they all have it
  RooCmdArg proj = ProjWData(*w->cat("DataCat"),*w->data(dsetName));

  // figure out the Data and PDF slices
  RooCmdArg *slice = 0;
  RooCmdArg *cut = 0;
  if ( catName != "All" ) {
    slice = new RooCmdArg( Slice(*w->cat("DataCat"),catName) );
    cut   = new RooCmdArg( Cut("DataCat==DataCat::"+catName) );
  }

  // component lists
  TString bkgComps;
  TString precComps;
  //if ( catName != "All" ) {
    //bkgComps = "bkg_pdf_" + catName;
    //precComps = "part_reco_pdf_" + catName;
  //}
  //else {
    //bkgComps  = "bkg_pdf_HadronTOS2011,bkg_pdf_HadronTOS2012,bkg_pdf_GlobalTIS2011,bkg_pdf_GlobalTIS2012";
    //precComps = "part_reco_pdf_HadronTOS2011,part_reco_pdf_HadronTOS2012,part_reco_pdf_GlobalTIS2011,part_reco_pdf_GlobalTIS2012";
  //}
  bkgComps = "bkg_pdf";
  precComps = "part_reco_pdf";

  // data invisible to start things off
  RooLinkedList data_inv;
  if ( cut ) data_inv.Add( cut );
  data_inv.Add( new RooCmdArg( Invisible() ) );
  w->data(dsetName)->plotOn(plot, data_inv );

  // Bs->KstKst
  RooLinkedList bs2kstkst;
  bs2kstkst.Add( new RooCmdArg( LineColor(lineColor[0]) ) );
  bs2kstkst.Add( new RooCmdArg( LineStyle(lineStyle[0]) ) );
  bs2kstkst.Add( new RooCmdArg( LineWidth(lineWidth[0]) ) );
  bs2kstkst.Add( new RooCmdArg( FillColor(fillColor[0]) ) );
  bs2kstkst.Add( new RooCmdArg( FillStyle(fillStyle[0]) ) );
  bs2kstkst.Add( new RooCmdArg( DrawOption(drawOpt[0]) ) );
  bs2kstkst.Add( new RooCmdArg( Components( bkgComps + "," + precComps + ",lb2pkpipi_mc_pdf,bs2phikst_mc_pdf,bd2rhokst_mc_pdf,bd2phikst_mc_pdf,bd2kstkst_mc_pdf,bs2kstkst_mc_pdf" ) ) );
  bs2kstkst.Add( &proj );
  if ( slice ) bs2kstkst.Add( slice );
  w->pdf("pdf")->plotOn(plot, bs2kstkst );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "#it{B}^{0}_{#it{s}} #rightarrow (#it{K}^{#plus}#it{#pi}^{#minus})(#it{K}^{#minus}#it{#pi}^{#plus})", "F" );

  // Bd->KstKst
  RooLinkedList bd2kstkst;
  bd2kstkst.Add( new RooCmdArg( LineColor(lineColor[1]) ) );
  bd2kstkst.Add( new RooCmdArg( LineStyle(lineStyle[1]) ) );
  bd2kstkst.Add( new RooCmdArg( LineWidth(lineWidth[1]) ) );
  bd2kstkst.Add( new RooCmdArg( FillColor(fillColor[1]) ) );
  bd2kstkst.Add( new RooCmdArg( FillStyle(fillStyle[1]) ) );
  bd2kstkst.Add( new RooCmdArg( DrawOption(drawOpt[1]) ) );
  bd2kstkst.Add( new RooCmdArg( Components( bkgComps + "," + precComps + ",lb2pkpipi_mc_pdf,bs2phikst_mc_pdf,bd2rhokst_mc_pdf,bd2phikst_mc_pdf,bd2kstkst_mc_pdf" ) ) );
  if ( slice ) bd2kstkst.Add( slice );
  bd2kstkst.Add( &proj );
  w->pdf("pdf")->plotOn(plot, bd2kstkst );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "#it{B}^{0}_{#it{d}} #rightarrow (#it{K}^{#plus}#it{#pi}^{#minus})(#it{K}^{#minus}#it{#pi}^{#plus})", "F" );

  // Bd->PhiKst
  RooLinkedList bd2phikst;
  bd2phikst.Add( new RooCmdArg( LineColor(lineColor[2]) ) );
  bd2phikst.Add( new RooCmdArg( LineStyle(lineStyle[2]) ) );
  bd2phikst.Add( new RooCmdArg( LineWidth(lineWidth[2]) ) );
  bd2phikst.Add( new RooCmdArg( FillColor(fillColor[2]) ) );
  bd2phikst.Add( new RooCmdArg( FillStyle(fillStyle[2]) ) );
  bd2phikst.Add( new RooCmdArg( DrawOption(drawOpt[2]) ) );
  bd2phikst.Add( new RooCmdArg( Components( bkgComps + "," + precComps + ",lb2pkpipi_mc_pdf,bs2phikst_mc_pdf,bd2rhokst_mc_pdf,bd2phikst_mc_pdf" ) ) );
  if ( slice ) bd2phikst.Add( slice );
  bd2phikst.Add( &proj );
  w->pdf("pdf")->plotOn(plot, bd2phikst );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "#it{B}^{0}_{#it{d}} #rightarrow (#it{K}^{#plus}#it{#pi}^{#minus})(#it{K}^{#minus}#it{K}^{#plus})", "F" );

  // Bd->RhoKst
  RooLinkedList bd2rhokst;
  bd2rhokst.Add( new RooCmdArg( LineColor(lineColor[3]) ) );
  bd2rhokst.Add( new RooCmdArg( LineStyle(lineStyle[3]) ) );
  bd2rhokst.Add( new RooCmdArg( LineWidth(lineWidth[3]) ) );
  bd2rhokst.Add( new RooCmdArg( FillColor(fillColor[3]) ) );
  bd2rhokst.Add( new RooCmdArg( FillStyle(fillStyle[3]) ) );
  bd2rhokst.Add( new RooCmdArg( DrawOption(drawOpt[3]) ) );
  bd2rhokst.Add( new RooCmdArg( Components( bkgComps + "," + precComps + ",lb2pkpipi_mc_pdf,bs2phikst_mc_pdf,bd2rhokst_mc_pdf" ) ) );
  if ( slice ) bd2rhokst.Add( slice );
  bd2rhokst.Add( &proj );
  w->pdf("pdf")->plotOn(plot, bd2rhokst );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "#it{B}^{0}_{#it{d}}#rightarrow (#it{K}^{#plus}#it{#pi}^{#minus})(#it{#pi}^{#minus}#it{#pi}^{#plus})", "F" );

  // Bs->PhiKst
  RooLinkedList bs2phikst;
  bs2phikst.Add( new RooCmdArg( LineColor(lineColor[4]) ) );
  bs2phikst.Add( new RooCmdArg( LineStyle(lineStyle[4]) ) );
  bs2phikst.Add( new RooCmdArg( LineWidth(lineWidth[4]) ) );
  bs2phikst.Add( new RooCmdArg( FillColor(fillColor[4]) ) );
  bs2phikst.Add( new RooCmdArg( FillStyle(fillStyle[4]) ) );
  bs2phikst.Add( new RooCmdArg( DrawOption(drawOpt[4]) ) );
  bs2phikst.Add( new RooCmdArg( Components( bkgComps + "," + precComps + ",lb2pkpipi_mc_pdf,bs2phikst_mc_pdf" ) ) );
  if ( slice ) bs2phikst.Add( slice );
  bs2phikst.Add( &proj );
  w->pdf("pdf")->plotOn(plot, bs2phikst );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "#it{B}^{0}_{#it{s}} #rightarrow (#it{K}^{#plus}#it{#pi}^{#minus})(#it{K}^{#minus}#it{K}^{#plus})", "F" );

  // Lb->pKpipi
  RooLinkedList lb2ppipipi;
  lb2ppipipi.Add( new RooCmdArg( LineColor(lineColor[5]) ) );
  lb2ppipipi.Add( new RooCmdArg( LineStyle(lineStyle[5]) ) );
  lb2ppipipi.Add( new RooCmdArg( LineWidth(lineWidth[5]) ) );
  lb2ppipipi.Add( new RooCmdArg( FillColor(fillColor[5]) ) );
  lb2ppipipi.Add( new RooCmdArg( FillStyle(fillStyle[5]) ) );
  lb2ppipipi.Add( new RooCmdArg( DrawOption(drawOpt[5]) ) );
  lb2ppipipi.Add( new RooCmdArg( Components( bkgComps + "," + precComps + ",lb2pkpipi_mc_pdf" ) ) );
  if ( slice ) lb2ppipipi.Add( slice );
  lb2ppipipi.Add( &proj );
  w->pdf("pdf")->plotOn(plot, lb2ppipipi );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "#it{#Lambda}^{0}_{#it{b}}#rightarrow (#it{p}#it{#pi}^{#minus})(#it{K}^{#minus}#it{#pi}^{#plus})", "F" );

  // part reco
  RooLinkedList prec;
  prec.Add( new RooCmdArg( LineColor(lineColor[6]) ) );
  prec.Add( new RooCmdArg( LineStyle(lineStyle[6]) ) );
  prec.Add( new RooCmdArg( LineWidth(lineWidth[6]) ) );
  prec.Add( new RooCmdArg( FillColor(fillColor[6]) ) );
  prec.Add( new RooCmdArg( FillStyle(fillStyle[6]) ) );
  prec.Add( new RooCmdArg( DrawOption(drawOpt[6]) ) );
  prec.Add( new RooCmdArg( Components( bkgComps + "," + precComps ) ) );
  if ( slice ) prec.Add( slice );
  prec.Add( &proj );
  w->pdf("pdf")->plotOn(plot, prec );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "Partially reconstructed", "F" );

  // bkg
  RooLinkedList bkg;
  bkg.Add( new RooCmdArg( LineColor(lineColor[7]) ) );
  bkg.Add( new RooCmdArg( LineStyle(lineStyle[7]) ) );
  bkg.Add( new RooCmdArg( LineWidth(lineWidth[7]) ) );
  bkg.Add( new RooCmdArg( FillColor(fillColor[7]) ) );
  bkg.Add( new RooCmdArg( FillStyle(fillStyle[7]) ) );
  bkg.Add( new RooCmdArg( DrawOption(drawOpt[7]) ) );
  bkg.Add( new RooCmdArg( Components( bkgComps ) ) );
  if ( slice ) bkg.Add( slice );
  bkg.Add( &proj );
  w->pdf("pdf")->plotOn(plot, bkg);
  leg->AddEntry( plot->getObject(plot->numItems()-1), "Combinatorial", "F" );

  // full pdf over the top
  RooLinkedList pdf;
  pdf.Add( new RooCmdArg( LineWidth(3) ) );
  pdf.Add( new RooCmdArg( LineStyle(1) ) );
  pdf.Add( new RooCmdArg( LineColor(kBlue) ) );
  if ( slice ) pdf.Add( slice );
  pdf.Add( &proj );
  //w->pdf("pdf")->plotOn(plot, pdf);
  w->pdf("constrained_pdf")->plotOn(plot, pdf);
  leg->AddEntry( plot->getObject(plot->numItems()-1), "Total Model" , "L" );

  // redraw data on top
  RooLinkedList data;
  if ( cut ) data.Add( cut );
  w->data(dsetName)->plotOn(plot, data);
  leg->AddEntry( plot->getObject(plot->numItems()-1), "Data", "LEP" );

  // title
  TString label = "All Categories Combined";
  if ( catName == "HadronTOS2011" ) label = "2011 Hadron TOS";
  if ( catName == "GlobalTIS2011" ) label = "2011 Global TIS";
  if ( catName == "HadronTOS2012" ) label = "2012 Hadron TOS";
  if ( catName == "GlobalTIS2012" ) label = "2012 Global TIS";

  // file name
  TString fname = outfName+catName;

  // setup the PAPER style
  int resid = 2;
  int pRedPull = 3;
  TString plotLabel = "#splitline{Simultaneous Mass Fit}{"+label+"}";
  if ( paperStyle ) {
    resid = 0;
    pRedPull = -1;
    plotLabel = "LHCb";
  }

  // plot it
  makePlot( plot, fname, resid, leg, plotLabel, pRedPull );
}

void Bs2KstKst::MassFitPlotter::makePlot( RooPlot *plot, TString fname, int resid, TLegend *leg, TString pTitle, int pRedPull, const RooArgList *params ) {

  plot->GetXaxis()->SetTitleSize(0.08);
  plot->GetYaxis()->SetTitleSize(0.08);
  plot->GetXaxis()->SetTitleOffset(0.8);
  plot->GetYaxis()->SetTitleOffset(0.9);

  // legend
  if ( leg ) {
    leg->SetFillColor(0);
    leg->SetLineColor(0);
  }

  // residual hist
  RooHist *underHist;
  if (resid==1) underHist = plot->residHist();
  if (resid==2) underHist = plot->pullHist();

  // title
  TPaveText *title;
  if ( pTitle != "" ) {
    title = new TPaveText(0.15,0.8,0.47,0.92,"ndc");
    title->SetFillColor(0);
    title->SetShadowColor(0);
    title->SetLineColor(0);
    title->AddText(pTitle);
  }

  // parameter values
  TPaveText *pNames;
  TPaveText *pVals;
  TPaveText *pErrs;
  if ( params ) {
    double top     = pTitle == "" ? 0.9 : 0.8;
    double bottom  = top-(0.05*params->getSize());
    double pBoxX   = 0.22;
    pNames = new TPaveText(pBoxX,bottom,pBoxX+0.10,top,"ndc");
    pVals  = new TPaveText(pBoxX+0.10,bottom,pBoxX+0.19,top,"ndc");
    pErrs  = new TPaveText(pBoxX+0.19,bottom,pBoxX+0.23,top,"ndc");
    pNames->SetFillColor(0);
    pNames->SetShadowColor(0);
    pNames->SetLineColor(0);
    pNames->SetTextSize(0.03);
    pNames->SetTextAlign(11);
    pVals->SetFillColor(0);
    pVals->SetShadowColor(0);
    pVals->SetLineColor(0);
    pVals->SetTextSize(0.03);
    pVals->SetTextAlign(11);
    pErrs->SetFillColor(0);
    pErrs->SetShadowColor(0);
    pErrs->SetLineColor(0);
    pErrs->SetTextSize(0.03);
    pErrs->SetTextAlign(11);
    RooRealVar *parg;
    TIterator *iter = params->createIterator();
    while ((parg = (RooRealVar*)iter->Next())){
	    pNames->AddText(Form("%-10s",parg->GetTitle()));
      pVals->AddText(Form("= %7.1f ",parg->getVal()));
      pErrs->AddText(Form("#pm %-5.1f",parg->getError()));
    }
  }

  TCanvas *c;
  TCanvas *clog;
  TPad *upperPad;
  TPad *lowerPad;

  if ( resid==0 ) {
    c = new TCanvas("c","c",800,600);
    c->SetBottomMargin(0.2);
    plot->Draw();
    if ( leg ) leg->Draw("same");
    if ( pTitle != "" ) title->Draw("same");
    if ( params ) {
      pNames->Draw("same");
      pVals->Draw("same");
      pErrs->Draw("same");
    }
  }
  else {
    c = new TCanvas("c","c",800,800);
    upperPad = new TPad("c_upper","",0.,0.33,1.,1.);
    lowerPad = new TPad("c_lower","",0.,0.,1.,0.33);
    c->cd();
    upperPad->Draw();
    lowerPad->Draw();
    // under hist style
    underHist->GetXaxis()->SetRangeUser(plot->GetXaxis()->GetXmin(), plot->GetXaxis()->GetXmax());
    underHist->GetXaxis()->SetTitle(plot->GetXaxis()->GetTitle());
    if (resid==1) underHist->GetYaxis()->SetTitle("Residual");
    if (resid==2) underHist->GetYaxis()->SetTitle("Pull");
    underHist->GetXaxis()->SetLabelSize(0.12);
    underHist->GetYaxis()->SetLabelSize(0.12);
    underHist->GetXaxis()->SetTitleSize(0.2);
    underHist->GetXaxis()->SetTitleOffset(0.7);
    underHist->GetYaxis()->SetTitleSize(0.18);
    underHist->GetYaxis()->SetTitleOffset(0.38);
    // canv style and plot
    plot->GetXaxis()->SetTitle("");
    upperPad->SetBottomMargin(0.1);
    upperPad->cd();
    plot->Draw();
    if ( leg ) leg->Draw("same");
    if ( pTitle != "" ) title->Draw("same");
    if ( params ) {
      pNames->Draw("same");
      pVals->Draw("same");
      pErrs->Draw("same");
    }
    c->cd();
    lowerPad->SetTopMargin(0.05);
    lowerPad->SetBottomMargin(0.35);
    lowerPad->cd();
    underHist->Draw("AP");
    if ( resid==2 ) { // pull hist
      double ymin = underHist->GetYaxis()->GetXmin();
      double ymax = underHist->GetYaxis()->GetXmax();
      double yrange = Max( Abs( ymin ), Abs( ymax ) );
      underHist->GetYaxis()->SetRangeUser( -1.*yrange, 1.*yrange );

      double xmin = plot->GetXaxis()->GetXmin();
      double xmax = plot->GetXaxis()->GetXmax();

      TColor *mycol3sig = gROOT->GetColor( kGray );
      mycol3sig->SetAlpha(0.5);
      TColor *mycol2sig = gROOT->GetColor( kGray+1 );
      mycol2sig->SetAlpha(0.5);
      TColor *mycol1sig = gROOT->GetColor( kGray+2 );
      mycol1sig->SetAlpha(0.5);

      TBox box3sig;
      box3sig.SetFillColor( mycol3sig->GetNumber() );
      //box3sig.SetFillColorAlpha( kGray, 0.5 );
      box3sig.SetFillStyle(1001);
      box3sig.DrawBox( xmin, -3., xmax, 3.);
      TBox box2sig;
      box2sig.SetFillColor( mycol2sig->GetNumber() );
      //box2sig.SetFillColorAlpha( kGray+1, 0.5 );
      box2sig.SetFillStyle(1001);
      box2sig.DrawBox( xmin, -2., xmax, 2.);
      TBox box1sig;
      box1sig.SetFillColor( mycol1sig->GetNumber() );
      //box1sig.SetFillColorAlpha( kGray+2, 0.5 );
      box1sig.SetFillStyle(1001);
      box1sig.DrawBox( xmin, -1., xmax, 1.);

      TLine lineErr;
      lineErr.SetLineWidth(1);
      lineErr.SetLineColor(kBlue-9);
      lineErr.SetLineStyle(2);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),1.,plot->GetXaxis()->GetXmax(),1.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),-1.,plot->GetXaxis()->GetXmax(),-1.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),2.,plot->GetXaxis()->GetXmax(),2.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),-2.,plot->GetXaxis()->GetXmax(),-2.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),3.,plot->GetXaxis()->GetXmax(),3.);
      lineErr.DrawLine(plot->GetXaxis()->GetXmin(),-3.,plot->GetXaxis()->GetXmax(),-3.);
    }
    // line at zero
    TLine line;
    line.SetLineWidth(3);
    line.SetLineColor(kBlue);
    line.DrawLine(plot->GetXaxis()->GetXmin(),0.,plot->GetXaxis()->GetXmax(),0.);
    underHist->Draw("Psame");
    // draw red pull points if requested
    if (pRedPull>-1) {
      RooHist *redPull = new RooHist();
      int newp=0;
      for (int p=0; p<underHist->GetN(); p++) {
        double x,y;
        underHist->GetPoint(p,x,y);
        if ( TMath::Abs(y)>pRedPull ) {
          redPull->SetPoint(newp,x,y);
          redPull->SetPointError(newp,0.,0.,underHist->GetErrorYlow(p),underHist->GetErrorYhigh(p));
          newp++;
        }
      }
      redPull->SetLineWidth(underHist->GetLineWidth());
      redPull->SetMarkerStyle(underHist->GetMarkerStyle());
      redPull->SetMarkerSize(underHist->GetMarkerSize());
      redPull->SetLineColor(kRed);
      redPull->SetMarkerColor(kRed);
      redPull->Draw("Psame");
    }
  }
  c->Update();
  c->Modified();
  c->Print( Form("plots/%s/pdf/%s.pdf",outdirname.Data(),fname.Data()) );
  c->Print( Form("plots/%s/png/%s.png",outdirname.Data(),fname.Data()) );
  //c->Print( Form("plots/%s/C/%s.C",outdirname.Data(),fname.Data()) ); // seems to cause seg fault
  //

  if (resid==0) {
    clog = new TCanvas("clog","clog",800,600);
    clog->SetBottomMargin(0.2);
    RooPlot *logplot = (RooPlot*)plot->Clone();
    logRangeMin = logRangeMin > 0 ? logRangeMin : 1.;
    logRangeMax = logRangeMax > 0 ? logRangeMax : plot->GetMaximum()*2.;
    cout << "YO: " << logRangeMin << " " << logRangeMax << endl;
    logplot->GetYaxis()->SetRangeUser(logRangeMin, logRangeMax);
    logplot->Draw();
    if ( pTitle != "" && titleOnLog ) title->Draw("same");
    clog->SetLogy();
    clog->Update();
    clog->Modified();
    clog->Print( Form("plots/%s/pdf/%s_log.pdf",outdirname.Data(),fname.Data()) );
    clog->Print( Form("plots/%s/png/%s_log.png",outdirname.Data(),fname.Data()) );
  }
  else {
    clog = new TCanvas("clog","clog",800,800);
    clog->cd();
    TPad *upperPadLog = new TPad("c_upper_log","",0.0,0.33,1.,1.);
    TPad *lowerPadLog = (TPad*)lowerPad->Clone("c_lower_log");
    upperPadLog->SetBottomMargin(0.1);
    RooPlot *logplot = (RooPlot*)plot->Clone();
    logplot->GetYaxis()->SetRangeUser(1.,plot->GetMaximum()*2.);
    upperPadLog->cd();
    logplot->Draw();
    upperPadLog->SetLogy();
    clog->cd();
    upperPadLog->Draw();
    lowerPadLog->Draw();
    clog->Update();
    clog->Modified();
    clog->Print( Form("plots/%s/pdf/%s_log.pdf",outdirname.Data(),fname.Data()) );
    clog->Print( Form("plots/%s/png/%s_log.png",outdirname.Data(),fname.Data()) );
  }

  delete c;
  delete clog;
}


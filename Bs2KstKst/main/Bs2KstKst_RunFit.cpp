#include <iostream>

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

#include "RooStats/SPlot.h"

using namespace std;
using namespace RooFit;
using namespace TMath;

void makePlot( RooPlot *plot, TString fname, int resid=0, TLegend *leg=0, TString pTitle="", int pRedPull=-1, const RooArgList *params=0 ) {

  plot->GetXaxis()->SetTitleOffset(0.8);
  plot->GetYaxis()->SetTitleOffset(0.75);

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
    TPad *upperPad = new TPad(Form("%s_upper",c->GetName()),"",0.,0.33,1.,1.);
    TPad *lowerPad = new TPad(Form("%s_lower",c->GetName()),"",0.,0.,1.,0.33);
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
  c->Print( Form("plots/MassFit/pdf/%s.pdf",fname.Data()) );
  c->Print( Form("plots/MassFit/png/%s.png",fname.Data()) );
  //c->Print( Form("plots/MassFit/C/%s.C",fname.Data()) ); // seems to cause seg fault
  delete c;
}

void plot( RooWorkspace *w, TString var, TString data, TString pdf="", int resid=2 ) {

  w->loadSnapshot(Form("%s_fit",pdf.Data()));

  RooPlot *plot = w->var(var)->frame();
  w->data(data)->plotOn(plot);
  if ( pdf!="" ) w->pdf(pdf)->plotOn(plot);

  TString name = "v" + var + "_d" + data + "_p" + pdf;
  makePlot( plot, name, 2 );
}

void fit( RooWorkspace *w, TString pdf, TString data ) {

  assert( w->pdf(pdf) );
  assert( w->data(data) );

  RooFitResult *fr = w->pdf(pdf)->fitTo( *w->data(data), Save() );
  RooArgSet *parameters = w->pdf(pdf)->getParameters(w->set("observables"));
  w->saveSnapshot( Form("%s_fit",pdf.Data()),*parameters );
  fr->SetName( Form("%s_fr",pdf.Data()) );
  w->import(*fr);

}

void RunMCFits( RooWorkspace *w ) {
  fit( w, "bs2kstkst_mc_pdf", "Bs2KstKst" );
  fit( w, "bd2kstkst_mc_pdf", "Bd2KstKst" );
  fit( w, "bd2phikst_mc_pdf", "Bd2PhiKst" );
  fit( w, "bs2phikst_mc_pdf", "Bs2PhiKst" );
  fit( w, "bd2rhokst_mc_pdf", "Bd2RhoKst" );
  fit( w, "lb2pkpipi_mc_pdf", "Lb2pKpipi" );
  fit( w, "lb2ppipipi_mc_pdf", "Lb2ppipipi" );
}

void PlotMCFitResults( RooWorkspace *w ) {
  plot( w, "B_s0_DTF_B_s0_M", "Bs2KstKst", "bs2kstkst_mc_pdf" );
  plot( w, "B_s0_DTF_B_s0_M", "Bd2KstKst", "bd2kstkst_mc_pdf" );
  plot( w, "B_s0_DTF_B_s0_M", "Bd2PhiKst", "bd2phikst_mc_pdf" );
  plot( w, "B_s0_DTF_B_s0_M", "Bs2PhiKst", "bs2phikst_mc_pdf" );
  plot( w, "B_s0_DTF_B_s0_M", "Bd2RhoKst", "bd2rhokst_mc_pdf" );
  plot( w, "B_s0_DTF_B_s0_M", "Lb2pKpipi", "lb2pkpipi_mc_pdf" );
  plot( w, "B_s0_DTF_B_s0_M", "Lb2ppipipi", "lb2ppipipi_mc_pdf" );

}

void RunDataFit( RooWorkspace *w ) {

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

  // constrained
  RooFitResult *fr = w->pdf("constrained_pdf")->fitTo( *w->data("DataCombined"), Constrain( RooArgSet(*w->var("yield_ratio_bs2phikst_o_bd2phikst"),*w->var("yield_ratio_bd2rhokst_o_bd2phikst")) ), Extended(), Save() );
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

void makeDataPlot( RooWorkspace *w, TString catName="All" ) {

  //w->loadSnapshot("pdf_fit");
  w->loadSnapshot("constrained_pdf_fit");

  // make a legend
  TLegend *leg = new TLegend(0.6,0.3,0.89,0.89);
  leg->SetFillColor(0);
  leg->SetLineColor(0);

  // make a plot
  RooPlot *plot = w->var("B_s0_DTF_B_s0_M")->frame();

  // keep this command arg as they all have it
  RooCmdArg proj = ProjWData(*w->cat("DataCat"),*w->data("DataCombined"));

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
  if ( catName != "All" ) {
    bkgComps = "bkg_pdf_" + catName;
    //precComps = "part_reco_pdf_" + catName;
  }
  else {
    bkgComps  = "bkg_pdf_HadronTOS2011,bkg_pdf_HadronTOS2012,bkg_pdf_GlobalTIS2011,bkg_pdf_GlobalTIS2012";
    //precComps = "part_reco_pdf_HadronTOS2011,part_reco_pdf_HadronTOS2012,part_reco_pdf_GlobalTIS2011,part_reco_pdf_GlobalTIS2012";
  }
  precComps = "part_reco_pdf";

  // data invisible to start things off
  RooLinkedList data_inv;
  if ( cut ) data_inv.Add( cut );
  data_inv.Add( new RooCmdArg( Invisible() ) );
  w->data("DataCombined")->plotOn(plot, data_inv );

  // Bs->KstKst
  RooLinkedList bs2kstkst;
  bs2kstkst.Add( new RooCmdArg( LineColor(kRed-3) ) );
  bs2kstkst.Add( new RooCmdArg( FillColor(kRed-3) ) );
  bs2kstkst.Add( new RooCmdArg( DrawOption("F") ) );
  bs2kstkst.Add( new RooCmdArg( Components( bkgComps + "," + precComps + ",lb2pkpipi_mc_pdf,bs2phikst_mc_pdf,bd2rhokst_mc_pdf,bd2phikst_mc_pdf,bd2kstkst_mc_pdf,bs2kstkst_mc_pdf" ) ) );
  bs2kstkst.Add( &proj );
  if ( slice ) bs2kstkst.Add( slice );
  w->pdf("pdf")->plotOn(plot, bs2kstkst );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "B_{s} #rightarrow (K^{+}#pi^{-})(K^{-}#pi^{+})", "F" );

  // Bd->KstKst
  RooLinkedList bd2kstkst;
  bd2kstkst.Add( new RooCmdArg( LineColor(kMagenta-3) ) );
  bd2kstkst.Add( new RooCmdArg( FillColor(kMagenta-3) ) );
  bd2kstkst.Add( new RooCmdArg( DrawOption("F") ) );
  bd2kstkst.Add( new RooCmdArg( Components( bkgComps + "," + precComps + ",lb2pkpipi_mc_pdf,bs2phikst_mc_pdf,bd2rhokst_mc_pdf,bd2phikst_mc_pdf,bd2kstkst_mc_pdf" ) ) );
  if ( slice ) bd2kstkst.Add( slice );
  bd2kstkst.Add( &proj );
  w->pdf("pdf")->plotOn(plot, bd2kstkst );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "B_{d} #rightarrow (K^{+}#pi^{-})(K^{-}#pi^{+})", "F" );

  // Bd->PhiKst
  RooLinkedList bd2phikst;
  bd2phikst.Add( new RooCmdArg( LineColor(kCyan-3) ) );
  bd2phikst.Add( new RooCmdArg( FillColor(kCyan-3) ) );
  bd2phikst.Add( new RooCmdArg( DrawOption("F") ) );
  bd2phikst.Add( new RooCmdArg( Components( bkgComps + "," + precComps + ",lb2pkpipi_mc_pdf,bs2phikst_mc_pdf,bd2rhokst_mc_pdf,bd2phikst_mc_pdf" ) ) );
  if ( slice ) bd2phikst.Add( slice );
  bd2phikst.Add( &proj );
  w->pdf("pdf")->plotOn(plot, bd2phikst );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "B_{d} #rightarrow (K^{+}K^{-})(K^{-}#pi^{+})", "F" );

  // Bd->RhoKst
  RooLinkedList bd2rhokst;
  bd2rhokst.Add( new RooCmdArg( LineColor(kOrange-3) ) );
  bd2rhokst.Add( new RooCmdArg( FillColor(kOrange-3) ) );
  bd2rhokst.Add( new RooCmdArg( DrawOption("F") ) );
  bd2rhokst.Add( new RooCmdArg( Components( bkgComps + "," + precComps + ",lb2pkpipi_mc_pdf,bs2phikst_mc_pdf,bd2rhokst_mc_pdf" ) ) );
  if ( slice ) bd2rhokst.Add( slice );
  bd2rhokst.Add( &proj );
  w->pdf("pdf")->plotOn(plot, bd2rhokst );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "B_{d}#rightarrow (#pi^{+}#pi^{-})(K^{-}#pi^{+})", "F" );

  // Bs->PhiKst
  RooLinkedList bs2phikst;
  bs2phikst.Add( new RooCmdArg( LineColor(kViolet+1) ) );
  bs2phikst.Add( new RooCmdArg( FillColor(kViolet+1) ) );
  bs2phikst.Add( new RooCmdArg( DrawOption("F") ) );
  bs2phikst.Add( new RooCmdArg( Components( bkgComps + "," + precComps + ",lb2pkpipi_mc_pdf,bs2phikst_mc_pdf" ) ) );
  if ( slice ) bs2phikst.Add( slice );
  bs2phikst.Add( &proj );
  w->pdf("pdf")->plotOn(plot, bs2phikst );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "B_{s} #rightarrow (K^{+}K^{-})(K^{-}#pi^{+})", "F" );

  // Lb->pKpipi
  RooLinkedList lb2ppipipi;
  lb2ppipipi.Add( new RooCmdArg( LineColor(kBlue-7) ) );
  lb2ppipipi.Add( new RooCmdArg( FillColor(kBlue-7) ) );
  lb2ppipipi.Add( new RooCmdArg( DrawOption("F") ) );
  lb2ppipipi.Add( new RooCmdArg( Components( bkgComps + "," + precComps + ",lb2pkpipi_mc_pdf" ) ) );
  if ( slice ) lb2ppipipi.Add( slice );
  lb2ppipipi.Add( &proj );
  w->pdf("pdf")->plotOn(plot, lb2ppipipi );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "#Lambda_{b}#rightarrow (p^{+}#pi^{-})(K^{-}#pi^{+})", "F" );

  // part reco
  RooLinkedList prec;
  prec.Add( new RooCmdArg( LineColor(kTeal-7) ) );
  prec.Add( new RooCmdArg( FillColor(kTeal-7) ) );
  prec.Add( new RooCmdArg( DrawOption("F") ) );
  prec.Add( new RooCmdArg( Components( bkgComps + "," + precComps ) ) );
  if ( slice ) prec.Add( slice );
  prec.Add( &proj );
  w->pdf("pdf")->plotOn(plot, prec );
  leg->AddEntry( plot->getObject(plot->numItems()-1), "Part reco", "F" );

  // bkg
  RooLinkedList bkg;
  bkg.Add( new RooCmdArg( LineColor(kGreen+1) ) );
  bkg.Add( new RooCmdArg( FillColor(kGreen+1) ) );
  bkg.Add( new RooCmdArg( DrawOption("F") ) );
  bkg.Add( new RooCmdArg( Components( bkgComps ) ) );
  if ( slice ) bkg.Add( slice );
  bkg.Add( &proj );
  w->pdf("pdf")->plotOn(plot, bkg);
  leg->AddEntry( plot->getObject(plot->numItems()-1), "Combinatorial", "F" );

  // full pdf over the top
  RooLinkedList pdf;
  if ( slice ) pdf.Add( slice );
  pdf.Add( &proj );
  //w->pdf("pdf")->plotOn(plot, pdf);
  w->pdf("constrained_pdf")->plotOn(plot, pdf);
  leg->AddEntry( plot->getObject(plot->numItems()-1), "Total PDF" , "L" );

  // redraw data on top
  RooLinkedList data;
  if ( cut ) data.Add( cut );
  w->data("DataCombined")->plotOn(plot, data);
  leg->AddEntry( plot->getObject(plot->numItems()-1), "Data", "LEP" );

  // title
  TString label = "All Categories Combined";
  if ( catName == "HadronTOS2011" ) label = "2011 Hadron TOS";
  if ( catName == "GlobalTIS2011" ) label = "2011 Global TIS";
  if ( catName == "HadronTOS2012" ) label = "2012 Hadron TOS";
  if ( catName == "GlobalTIS2012" ) label = "2012 Global TIS";

  // file name
  TString fname = "DataFit"+catName;

  // plot it
  makePlot( plot, fname, 2, leg, "#splitline{Simultaneous Mass Fit}{"+label+"}", 3 );
}

void PlotDataFitResult( RooWorkspace *w ) {

  //w->loadSnapshot("pdf_fit");
  w->loadSnapshot("constrained_pdf_fit");

  makeDataPlot( w, "All" );
  makeDataPlot( w, "HadronTOS2011" );
  makeDataPlot( w, "GlobalTIS2011" );
  makeDataPlot( w, "HadronTOS2012" );
  makeDataPlot( w, "GlobalTIS2012" );

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
  RooCBShape *lb2pkpipi_mc_cb1  = new RooCBShape( "lb2pkpipi_mc_cb1", "lb2pkpipi_mc_cb1", *w->var("B_s0_DTF_B_s0_M"), *w->var("lb2pkpipi_mean"), *w->var("lb2pkpipi_sigma1"), *w->var("lb2pkpipi_alpha1"), *w->var("lb2pkpipi_n1") );
  RooCBShape *lb2pkpipi_mc_cb2  = new RooCBShape( "lb2pkpipi_mc_cb2", "lb2pkpipi_mc_cb2", *w->var("B_s0_DTF_B_s0_M"), *w->var("lb2pkpipi_mean"), *w->var("lb2pkpipi_sigma2"), *w->var("lb2pkpipi_alpha2"), *w->var("lb2pkpipi_n2") );
  RooAddPdf  *lb2pkpipi_mc_pdf  = new RooAddPdf( "lb2pkpipi_mc_pdf", "lb2pkpipi_mc_pdf", RooArgList( *lb2pkpipi_mc_cb1, *lb2pkpipi_mc_cb2 ), RooArgList( *w->var("lb2pkpipi_f1") ) );
  RooCBShape *lb2ppipipi_mc_pdf = new RooCBShape( "lb2ppipipi_mc_pdf", "lb2ppipipi_mc_pdf", *w->var("B_s0_DTF_B_s0_M"), *w->var("lb2ppipipi_mean"), *w->var("lb2ppipipi_sigma"), *w->var("lb2ppipipi_alpha"), *w->var("lb2ppipipi_n") );
  RooArgusBG *part_reco_pdf = new RooArgusBG( "part_reco_pdf", "part_reco_pdf", *w->var("B_s0_DTF_B_s0_M"), *w->var("part_reco_m0"), *w->var("part_reco_c"), *w->var("part_reco_p") );

  // load mc fit results
  w->loadSnapshot("bs2kstkst_mc_pdf_fit");
  w->loadSnapshot("bd2kstkst_mc_pdf_fit");
  w->loadSnapshot("bd2phikst_mc_pdf_fit");
  w->loadSnapshot("bs2phikst_mc_pdf_fit");
  w->loadSnapshot("bd2rhokst_mc_pdf_fit");
  w->loadSnapshot("lb2pkpipi_mc_pdf_fit");
  w->loadSnapshot("lb2ppipipi_mc_pdf_fit");

  // pdfs
  RooCategory *cat = (RooCategory*)w->cat("DataCat");
  for (int c=0; c<cat->numTypes(); c++) {

    cat->setIndex(c);
    RooArgList *yields = new RooArgList();
    yields->add( *w->var(Form("bs2kstkst_y_%s",cat->getLabel())) );
    yields->add( *w->var(Form("bd2kstkst_y_%s",cat->getLabel())) );
    yields->add( *w->var(Form("bd2phikst_y_%s",cat->getLabel())) ); // constrained
    //yields->add( *w->var(Form("bd2phikst_y_%s",cat->getLabel())) );
    //yields->add( *w->var(Form("bd2rhokst_y_%s",cat->getLabel())) );
    yields->add( *w->function(Form("bs2phikst_y_%s",cat->getLabel())) );
    yields->add( *w->function(Form("bd2rhokst_y_%s",cat->getLabel())) );
    yields->add( *w->var(Form("lb2pkpipi_y_%s",cat->getLabel())) );
    //yields->add( *w->var(Form("lb2ppipipi_y_%s",cat->getLabel())) );
    yields->add( *w->var(Form("part_reco_y_%s",cat->getLabel())) );
    yields->add( *w->var(Form("bkg_y_%s",cat->getLabel())) );

    RooArgList *pdfs = new RooArgList();
    pdfs->add( *bs2kstkst_mc_pdf );
    pdfs->add( *bd2kstkst_mc_pdf );
    pdfs->add( *bd2phikst_mc_pdf );
    pdfs->add( *bs2phikst_mc_pdf );
    pdfs->add( *bd2rhokst_mc_pdf );
    pdfs->add( *lb2pkpipi_mc_pdf );
    //pdfs->add( *lb2ppipipi_mc_pdf );
    pdfs->add( *part_reco_pdf );
    pdfs->add( *w->pdf(Form("bkg_pdf_%s",cat->getLabel())) );

    RooAddPdf *pdf = new RooAddPdf( "pdf","", *pdfs, *yields );

    RooArgSet obsAndCats( *w->set("observables"), *w->set("categories") );
    RooArgSet *nonyields = pdf->getParameters( RooArgSet( *yields, obsAndCats ) );
    // need to change the actualy floating parameters for the sWeights to get rid of the yield ratios
    yields->remove( *w->function(Form("bs2phikst_y_%s",cat->getLabel())) );
    yields->remove( *w->function(Form("bd2rhokst_y_%s",cat->getLabel())) );

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

  w->writeToFile("root/MassFitResultWSWeights.root");

}

int main() {

  gROOT->ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C");
  system("mkdir -p plots/MassFit/png");
  system("mkdir -p plots/MassFit/pdf");
  system("mkdir -p plots/MassFit/C");

  TFile *inf = TFile::Open("root/MassFitWorkspaceWithPDFs.root");
  RooWorkspace *w = (RooWorkspace*)inf->Get("w");
  RunMCFits( w );
  RunDataFit( w );
  w->writeToFile("root/MassFitResult.root");
  inf->Close();

  TFile *tf = TFile::Open("root/MassFitResult.root");
  RooWorkspace *ws = (RooWorkspace*)tf->Get("w");
  PlotMCFitResults( ws );
  PlotDataFitResult( ws );
  CalcSWeights( ws );
  tf->Close();
  //

  return 0;
}

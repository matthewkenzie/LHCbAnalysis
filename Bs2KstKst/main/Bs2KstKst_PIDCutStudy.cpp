#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TEfficiency.h"
#include "TLatex.h"
#include "TLine.h"
#include "TMath.h"

using namespace std;

void drawDist( TTree *tree, TString name, TString var, TString rhokst_mc, TString kstkst_mc, TString bkg, int bins, float xmin, float xmax, bool log=false ) {

  TH1F *h_rhokst = new TH1F( Form("h_rhokst_%s",name.Data()), "", bins, xmin, xmax );
  TH1F *h_kstkst = new TH1F( Form("h_kstkst_%s",name.Data()), "", bins, xmin, xmax );
  TH1F *h_bkg    = new TH1F( Form("h_bkg_%s",name.Data()), "", bins, xmin, xmax );

  tree->Draw( Form("%s >> h_rhokst_%s",var.Data(),name.Data()), rhokst_mc , "goff" );
  tree->Draw( Form("%s >> h_kstkst_%s",var.Data(),name.Data()), kstkst_mc , "goff" );
  tree->Draw( Form("%s >> h_bkg_%s",var.Data(),name.Data()), bkg , "goff" );

  h_rhokst->SetLineColor(kRed);
  h_kstkst->SetLineColor(kBlue);
  h_bkg->SetLineColor(kBlack);

  h_rhokst->SetLineWidth(2);
  h_kstkst->SetLineWidth(2);
  h_bkg->SetLineWidth(2);

  h_kstkst->GetXaxis()->SetTitle( name );
  h_kstkst->GetXaxis()->SetTitleOffset( 0.8 );

  h_rhokst->Scale(1./h_rhokst->Integral() );
  h_kstkst->Scale(1./h_kstkst->Integral() );
  h_bkg->Scale(1./h_bkg->Integral() );

  TLegend *leg = new TLegend( 0.4, 0.7, 0.7, 0.9 );
  leg->SetFillColor(0);
  leg->AddEntry( h_kstkst, "B_{s}^{0} #rightarrow K^{*0}K^{*0}", "L" );
  leg->AddEntry( h_rhokst, "B_{s}^{0} #rightarrow #rho^{0}K^{*0}", "L" );
  leg->AddEntry( h_bkg, "Background", "L" );

  h_kstkst->GetYaxis()->SetRangeUser(0.0001, 1.05* TMath::Max( TMath::Max( h_kstkst->GetMaximum(),h_rhokst->GetMaximum()), h_bkg->GetMaximum()));

  TCanvas *c = new TCanvas();
  h_kstkst->Draw("HIST");
  h_rhokst->Draw("HISTsame");
  h_bkg->Draw("HISTsame");
  leg->Draw("same");
  if (log) c->SetLogy();
  c->Update();
  c->Modified();
  c->Print(Form("plots/PIDStudy/pdf/%s.pdf",name.Data()));

}

void drawRoc( TTree *tree, TString xtitle, TString var, TString sig_cut, TString bkg_cut, int bins, float xmin, float xmax, TString op=">", TString legPos="R" ) {

  // dummy hist to get bin center cut value
  TH1F *h_dum = new TH1F("h_dum","", bins, xmin, xmax);
  h_dum->GetXaxis()->SetTitle( xtitle );
  h_dum->GetYaxis()->SetTitle( "Efficiency" );
  //h_dum->GetXaxis()->SetTitleOffset(0.8);
  //h_dum->GetYaxis()->SetTitleOffset(0.78);
  h_dum->GetXaxis()->SetTitleSize(0.06);
  h_dum->GetYaxis()->SetTitleSize(0.06);
  h_dum->GetYaxis()->SetRangeUser(0.,1.05);

  TEfficiency *sigEff = new TEfficiency( "sigEff", "eff;"+xtitle+";Efficiency", bins, xmin, xmax );
  TEfficiency *bkgEff = new TEfficiency( "bkgEff", "eff;"+xtitle+";Efficiency", bins, xmin, xmax );
  TGraphAsymmErrors *effRat = new TGraphAsymmErrors();
  TGraph *roc = new TGraph();

  int sigEntries = tree->GetEntries( sig_cut );
  int bkgEntries = tree->GetEntries( bkg_cut );

  // also draw 4 specific values on the ROC curve
  int nSpecVals = 4;
  int spacing = bins/(nSpecVals+1);
  int iSpecVal = 0;
  TGraph *specGr = new TGraph();
  vector<float> specVals;

  for ( int bin=1; bin<h_dum->GetNbinsX()+1; bin++ ) {

    float cut_val = h_dum->GetBinCenter(bin);
    int sigPass;
    int bkgPass;
    if ( var.Contains(":") ) {
      TString var1 = var;
      TString var2 = var;
      var1.Remove( var1.First(":"), var1.Length() );
      var2.Remove( 0, var2.First(":")+1 );
      sigPass = tree->GetEntries( Form( "(%s) && (%s %s %8.6f) && (%s %s %8.6f)", sig_cut.Data(), var1.Data(), op.Data(), cut_val, var2.Data(), op.Data(), cut_val) );
      bkgPass = tree->GetEntries( Form( "(%s) && (%s %s %8.6f) && (%s %s %8.6f)", bkg_cut.Data(), var1.Data(), op.Data(), cut_val, var2.Data(), op.Data(), cut_val) );

    }
    else {
      sigPass = tree->GetEntries( Form( "(%s) && (%s %s %8.6f)", sig_cut.Data(), var.Data(), op.Data(), cut_val) );
      bkgPass = tree->GetEntries( Form( "(%s) && (%s %s %8.6f)", bkg_cut.Data(), var.Data(), op.Data(), cut_val) );
    }

    sigEff->SetTotalEvents( bin, sigEntries );
    sigEff->SetPassedEvents( bin, sigPass );

    bkgEff->SetTotalEvents( bin, bkgEntries );
    bkgEff->SetPassedEvents( bin, bkgPass );

    double effR = bkgEff->GetEfficiency(bin)/sigEff->GetEfficiency(bin);
    double effRErrLow = effR * TMath::Sqrt ( TMath::Power( bkgEff->GetEfficiencyErrorLow(bin) / bkgEff->GetEfficiency(bin) , 2 ) + TMath::Power( sigEff->GetEfficiencyErrorLow(bin) / sigEff->GetEfficiency(bin) , 2 ) );
    double effRErrUp = effR * TMath::Sqrt ( TMath::Power( bkgEff->GetEfficiencyErrorUp(bin) / bkgEff->GetEfficiency(bin) , 2 ) + TMath::Power( sigEff->GetEfficiencyErrorUp(bin) / sigEff->GetEfficiency(bin) , 2 ) );

    effRat->SetPoint( bin-1, cut_val, effR );
    effRat->SetPointError( bin-1, h_dum->GetBinWidth(bin)/2., h_dum->GetBinWidth(bin)/2., effRErrLow, effRErrUp );

    roc->SetPoint( bin-1, sigEff->GetEfficiency(bin), 1.-bkgEff->GetEfficiency(bin) );

    // specific values on roc curve
    for ( int j=0; j<nSpecVals; j++ ) {
      if ( bin == (j+1)*spacing ) {
        specVals.push_back( cut_val );
        specGr->SetPoint( iSpecVal, sigEff->GetEfficiency(bin),  1.-bkgEff->GetEfficiency(bin) );
        iSpecVal++;
      }
    }
  }

  sigEff->SetLineColor(kBlue);
  sigEff->SetLineWidth(2);
  sigEff->SetMarkerColor(kBlue);
  sigEff->SetMarkerSize(0.5);

  bkgEff->SetLineColor(kRed);
  bkgEff->SetLineWidth(2);
  bkgEff->SetMarkerColor(kRed);
  bkgEff->SetMarkerSize(0.5);

  effRat->SetLineColor(kBlack);
  effRat->SetLineWidth(2);
  effRat->SetMarkerColor(kBlack);
  effRat->SetMarkerSize(0.5);

  TLegend *leg;
  if (legPos=="R") {
    leg = new TLegend(0.6,0.7,0.9,0.9);
  }
  else {
    leg = new TLegend(0.2,0.2,0.5,0.5);
  }
  leg->SetFillColor(0);
  leg->AddEntry(sigEff,"B_{s}^{0} #rightarrow K^{*0}K^{*0} MC","LEP");
  leg->AddEntry(bkgEff,"B_{s}^{0} #rightarrow #rho^{0}K^{*0} MC", "LEP");
  leg->AddEntry(effRat,"Ratio of efficiencies", "LEP");

  TLine *line = new TLine();
  line->SetLineColor( kGreen+3 );
  line->SetLineWidth( 2 );
  line->SetLineStyle( kDashed );

  TCanvas *c1 = new TCanvas("c1","c1",800,600);
  c1->cd();
  h_dum->Draw("AXIS");
  for ( int i=0; i<specVals.size(); i++ ) line->DrawLine( specVals[i], 0., specVals[i], 1.05 );
  sigEff->Draw("PEsame");
  bkgEff->Draw("PEsame");
  effRat->Draw("PEsame");
  leg->Draw("same");
  c1->Update();
  c1->Modified();
  c1->Print(Form("plots/PIDStudy/pdf/eff_%s.pdf",xtitle.Data()));

  TCanvas *c2 = new TCanvas("c2","c2",800,600);
  c2->cd();
  TH1F *dumRoc = new TH1F("dumRoc","",100,0,1);
  dumRoc->GetXaxis()->SetTitle( "K^{*0}K^{*0} Effciency" );
  dumRoc->GetYaxis()->SetTitle( "#rho^{0}K^{*0} Rejection" );
  dumRoc->GetXaxis()->SetTitleSize(0.06);
  dumRoc->GetYaxis()->SetTitleSize(0.06);
  dumRoc->GetYaxis()->SetRangeUser(0.,1.1);

  roc->SetLineWidth(3);

  specGr->SetMarkerColor(kGreen+3);
  specGr->SetLineColor(kGreen+3);
  specGr->SetLineWidth(2);
  specGr->SetMarkerStyle(33);
  specGr->SetMarkerSize(2.);

  dumRoc->Draw("AXIS");
  roc->Draw("Lsame");
  specGr->Draw("Psame");

  // label the specGr points
  TLatex *lat = new TLatex();
  lat->SetLineColor(0);
  lat->SetTextColor(kGreen+3);
  lat->SetTextSize(0.04);

  double x,y;
  for ( int p=0; p<specGr->GetN(); p++ ) {
    specGr->GetPoint(p,x,y);
    lat->DrawLatex(x+0.01,y+0.01, Form("%4.2f",specVals[p]) );
  }

  TLegend *leg2 = new TLegend(0.2,0.2,0.5,0.5);
  leg2->SetFillColor(0);
  leg2->AddEntry(roc,"ROC Curve","L");
  leg2->Draw("same");

  c2->Update();
  c2->Modified();
  c2->Print(Form("plots/PIDStudy/pdf/eff_%s_roc.pdf",xtitle.Data()));

  delete h_dum;
  delete sigEff;
  delete bkgEff;
  delete effRat;
  delete roc;
  delete dumRoc;
  delete c1;
  delete c2;
}

int main() {

  system("mkdir -p plots/PIDStudy/pdf");
  gROOT->ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C");
  TH1::SetDefaultSumw2();

  TFile *tf = TFile::Open("root/AnalysisOut.root");
  TTree *tree = (TTree*)tf->Get("AnalysisTree");

  TString pt_cut = " && (Kplus_P<100000 && Kminus_P<100000 && Piplus_P<100000 && Piminus_P<100000)";
  TString rhokst_mc = "(itype==-77 || itype==-87)";
  TString kstkst_mc = "(itype==-70 || itype==-80)";
  TString bkg = "(itype==71 || itype==81) && (B_s0_DTF_B_s0_M>5600 && B_s0_DTF_B_s0_M<5800)";

  //drawDist( tree, "Kminus_V3ProbNNk_corr"  , "Kminus_V3ProbNNk_corr"  , rhokst_mc, kstkst_mc, bkg, 100, 0, 1, true );
  //drawDist( tree, "Kplus_V3ProbNNk_corr"   , "Kplus_V3ProbNNk_corr"   , rhokst_mc, kstkst_mc, bkg, 100, 0, 1, true );
  //drawDist( tree, "Kminus_V3ProbNNpi_corr" , "Kminus_V3ProbNNpi_corr" , rhokst_mc, kstkst_mc, bkg, 100, 0, 1, true );
  //drawDist( tree, "Kplus_V3ProbNNpi_corr"  , "Kplus_V3ProbNNpi_corr"  , rhokst_mc, kstkst_mc, bkg, 100, 0, 1, true );
  //drawDist( tree, "Kminus_V3ProbNNKpi_corr", "Kminus_V3ProbNNKpi_corr", rhokst_mc+pt_cut, kstkst_mc+pt_cut, bkg+pt_cut, 100, 0, 1, true );
  //drawDist( tree, "Kplus_V3ProbNNKpi_corr" , "Kplus_V3ProbNNKpi_corr" , rhokst_mc+pt_cut, kstkst_mc+pt_cut, bkg+pt_cut, 100, 0, 1, true );
  drawDist( tree, "min_kaon_V3ProbNNKpi_corr", "min_kaon_V3ProbNNKpi_corr", rhokst_mc+pt_cut, kstkst_mc+pt_cut, bkg+pt_cut, 100, 0, 1, true );
  //drawDist( tree, "max_kaon_V3ProbNNKpi_corr", "min_kaon_V3ProbNNKpi_corr", rhokst_mc+pt_cut, kstkst_mc+pt_cut, bkg+pt_cut, 100, 0, 1, true );

  //drawRoc( tree, "Kminus_V3ProbNNk_corr"   , "Kminus_V3ProbNNk_corr"  , kstkst_mc, rhokst_mc, 50, 0, 1 );
  //drawRoc( tree, "Kplus_V3ProbNNk_corr"    , "Kplus_V3ProbNNk_corr"   , kstkst_mc, rhokst_mc, 50, 0, 1 );
  //drawRoc( tree, "Kminus_V3ProbNNpi_corr"  , "Kminus_V3ProbNNpi_corr" , kstkst_mc, rhokst_mc, 50, 0, 1 , "<");
  //drawRoc( tree, "Kplus_V3ProbNNpi_corr"   , "Kplus_V3ProbNNpi_corr"  , kstkst_mc, rhokst_mc, 50, 0, 1 , "<");
  //drawRoc( tree, "Kminus_V3ProbNNKpi_corr" , "Kminus_V3ProbNNKpi_corr", kstkst_mc+pt_cut, rhokst_mc+pt_cut, 50, 0, 1 );
  //drawRoc( tree, "Kplus_V3ProbNNKpi_corr"  , "Kplus_V3ProbNNKpi_corr" , kstkst_mc+pt_cut, rhokst_mc+pt_cut, 50, 0, 1 );

  //drawRoc( tree, "min_kaon_V3ProbNNk_corr", "min_kaon_V3ProbNNk_corr", kstkst_mc, rhokst_mc, 50, 0, 1 );
  //drawRoc( tree, "max_kaon_V3ProbNNpi_corr", "max_kaon_V3ProbNNpi_corr", kstkst_mc, rhokst_mc, 50, 0, 1, "<", "L" );
  //drawRoc( tree, "max_kaon_V3ProbNNKpi_corr", "max_kaon_V3ProbNNKpi_corr", kstkst_mc+pt_cut, rhokst_mc+pt_cut, 50, 0, 1 );
  drawRoc( tree, "min_kaon_V3ProbNNKpi_corr", "min_kaon_V3ProbNNKpi_corr", kstkst_mc+pt_cut, rhokst_mc+pt_cut, 50, 0, 1, ">", "L" );


  return 0;
}

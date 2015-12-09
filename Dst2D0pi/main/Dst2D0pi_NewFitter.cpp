#include <iostream>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TBox.h"
#include "TLine.h"
#include "TAxis.h"
#include "TColor.h"
#include "TMath.h"

#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooWorkspace.h"
#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooHist.h"

#include "Utils.h"

using namespace std;
using namespace RooFit;
using namespace TMath;
using namespace Utils;

void makeDataset( TString fname, TString tname, TString outfname ) {

  RooWorkspace *w = new RooWorkspace("w","w");

  w->factory( "Dst_M[1950.,2070.]" );
  w->factory( "D0_M[1810.,1920.]" );
  w->factory( "D0_LTIME_ps[0.00,5.]" );

  RooArgSet *observables = new RooArgSet();
  observables->add( *w->var("Dst_M") );
  observables->add( *w->var("D0_M") );
  observables->add( *w->var("D0_LTIME_ps") );
  w->defineSet("observables", *observables);

  w->var("Dst_M")->setBins(240);
  w->var("D0_M")->setBins(220);
  w->var("D0_LTIME_ps")->setBins(200);

  double Dst_M = -999.;
  double D0_M = -999.;
  double D0_LTIME_ps = -999.;

  TFile *tf = TFile::Open(fname);
  TTree *tree = (TTree*)tf->Get(tname);

  tree->SetBranchAddress( "Dst_M", &Dst_M );
  tree->SetBranchAddress( "D0_M" , &D0_M );
  tree->SetBranchAddress( "D0_LTIME_ps", &D0_LTIME_ps );

  RooDataSet *data = new RooDataSet("Data","Data",*observables);
  RooDataHist *dataH = new RooDataHist("DataHist","Data",*observables);

  for ( int ev=0; ev<tree->GetEntries(); ev++) {

    tree->GetEntry(ev);

    if ( ev%10000 == 0 ) cout << ev << " / " << tree->GetEntries() << endl;

    if ( Dst_M < w->var("Dst_M")->getMin() || Dst_M > w->var("Dst_M")->getMax() ) continue;
    if ( D0_M < w->var("D0_M")->getMin() || D0_M > w->var("D0_M")->getMax() ) continue;
    if ( D0_LTIME_ps < w->var("D0_LTIME_ps")->getMin() || D0_LTIME_ps > w->var("D0_LTIME_ps")->getMax() ) continue;

    w->var("Dst_M")->setVal(Dst_M);
    w->var("D0_M")->setVal(D0_M);
    w->var("D0_LTIME_ps")->setVal(D0_LTIME_ps);

    data->add( *observables );
    dataH->add( *observables );

  }
  tf->Close();

  w->import(*data);
  w->import(*dataH);

  w->writeToFile(outfname);

}

void makeFit( TString inf, TString outf ) {

  TFile *tf = TFile::Open( inf );
  RooWorkspace *w = (RooWorkspace*)tf->Get("w");

  //w->factory( "Gaussian::dst_mass1( Dst_M, dst_mean[2005,2015], dst_sigma1[1,20] )" );
  //w->factory( "Gaussian::dst_mass2( Dst_M, dst_mean, dst_sigma2[3,50] )" );
  //w->factory( "Gaussian::dst_mass3( Dst_M, dst_mean, dst_sigma3[5,200] )" );
  //w->factory( "SUM::dst_mass( dst_f[0.1,1.]*dst_mass1, dst_f2[0.1,1.]*dst_mass2, dst_mass3 )" );
  //w->factory( "SUM::dst_mass_sig( dst_f[0.1,1.]*dst_mass1, dst_f2[0.1,1.]*dst_mass2, dst_mass3 )" );
  w->factory( "Gaussian::dst_mass1( Dst_M, dst_mean[2005,2015], dst_sigma1[1,20] )" );
  w->factory( "CBShape::dst_mass2( Dst_M, dst_mean, dst_sigma2[1,20], dst_alpha[0.1,10.], dst_n1[0.1,10.] )" );
  w->factory( "SUM::dst_mass_sig( dst_f[0.1,1.]*dst_mass1, dst_mass2 )" );
  w->factory( "Bernstein::dst_mass_bkg( Dst_M, {1.,dst_p0[0.,1.]} )" );
  w->factory( "SUM::dst_mass( dst_mass_sy[0,10e8]*dst_mass_sig, dst_mass_by[0,10e2]*dst_mass_bkg )" );

  //w->factory( "Gaussian::d0_mass1( D0_M, d0_mean[1862,1868], d0_sigma1[1,20] )" );
  //w->factory( "Gaussian::d0_mass2( D0_M, d0_mean, d0_sigma2[3,50] )" );
  //w->factory( "Gaussian::d0_mass3( D0_M, d0_mean, d0_sigma3[5,200] )" );
  //w->factory( "SUM::d0_mass( d0_f[0.1,1.]*d0_mass1, d0_f2[0.1,1.]*d0_mass2, d0_mass3 )" );
  //w->factory( "SUM::d0_mass( d0_f[0.1,1.]*d0_mass1, d0_f2[0.1,1.]*d0_mass2, d0_mass3 )" );
  w->factory( "Gaussian::d0_mass1( D0_M, d0_mean[1862,1868], d0_sigma1[1,20] )" );
  w->factory( "CBShape::d0_mass2( D0_M, d0_mean, d0_sigma2[1,20], d0_alpha[0.1,10.], d0_n1[0.1,10.] )" );
  w->factory( "SUM::d0_mass_sig( d0_f[0.1,1.]*d0_mass1, d0_mass2 )" );
  w->factory( "Bernstein::d0_mass_bkg( D0_M, {1.,d0_p0[0.,1.]} )" );
  w->factory( "SUM::d0_mass( d0_mass_sy[0,10e8]*d0_mass_sig, d0_mass_by[0,10e1]*d0_mass_bkg )" );

  w->factory( "d0_tau[0,1000.]" );
  w->factory( "expr::d0_e( '-1/@0', d0_tau)" );
  w->factory( "Exponential::d0_t( D0_LTIME_ps, d0_e )" );

  w->pdf("dst_mass")->fitTo( *w->data("Data") , Range(1960,2060) );
  w->pdf("d0_mass") ->fitTo( *w->data("Data") , Range(1820,1910) );
  w->pdf("d0_t")    ->fitTo( *w->data("Data") , Range(0.25,5.) );

  tf->Close();

  w->writeToFile(outf);


}


void plot( TString var, TString data, TString pdf, double low=-1, double high=-1 ) {

  TFile *tf = TFile::Open( "root/FitOut.root" );
  RooWorkspace *w = (RooWorkspace*)tf->Get("w");
  TCanvas *canv = new TCanvas("c","c",800,800);
  TPad *upperPad = new TPad(Form("%s_upper",canv->GetName()),"",0.,0.33,1.,1.);
  TPad *lowerPad = new TPad(Form("%s_lower",canv->GetName()),"",0.,0.,1.,0.33);
  canv->cd();
  upperPad->Draw();
  lowerPad->Draw();

  if ( low < 0 ) low = w->var(var)->getMin();
  if ( high < 0 ) high = w->var(var)->getMax();
  RooPlot *plot = w->var(var)->frame(Range(low,high));
  w->data(data)->plotOn(plot);
  w->pdf(pdf)->plotOn(plot);

  RooHist *underHist = plot->pullHist();
  underHist->GetXaxis()->SetRangeUser(plot->GetXaxis()->GetXmin(), plot->GetXaxis()->GetXmax());
  underHist->GetXaxis()->SetTitle(plot->GetXaxis()->GetTitle());
  underHist->GetYaxis()->SetTitle("Pull");
  underHist->GetXaxis()->SetLabelSize(0.12);
  underHist->GetYaxis()->SetLabelSize(0.12);
  underHist->GetXaxis()->SetTitleSize(0.2);
  underHist->GetXaxis()->SetTitleOffset(0.7);
  underHist->GetYaxis()->SetTitleSize(0.18);
  underHist->GetYaxis()->SetTitleOffset(0.38);

  plot->GetXaxis()->SetTitle("");
  upperPad->SetBottomMargin(0.1);
  upperPad->cd();
  plot->Draw();

  canv->cd();
  lowerPad->SetTopMargin(0.05);
  lowerPad->SetBottomMargin(0.35);
  lowerPad->cd();
  underHist->Draw("AP");

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

  TLine line;
  line.SetLineWidth(3);
  line.SetLineColor(kBlue);
  line.DrawLine(plot->GetXaxis()->GetXmin(),0.,plot->GetXaxis()->GetXmax(),0.);
  underHist->Draw("Psame");

  RooHist *redPull = new RooHist();
  int newp=0;
  for (int p=0; p<underHist->GetN(); p++) {
    double x,y;
    underHist->GetPoint(p,x,y);
    if ( TMath::Abs(y)>3 ) {
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

  canv->Print(Form("tmp/%s.pdf",var.Data()));
  tf->Close();

}

int main(int argc, char **argv) {

  gROOT->ProcessLine(".x ~/bin/lhcbStyle.C");
  //makeDataset( "root/AnalysisOutEos.root", "AnalysisTree", "root/DataSet.root" );
  //

  //
  makeFit( "root/DataSet.root", "root/FitOut.root" );

  plot( "Dst_M",       "Data", "dst_mass" , 1960, 2060 ); //, w->var("Dst_M")->getMin(), w->var("Dst_M")->getMax() );
  plot( "D0_M",        "Data", "d0_mass"  , 1820, 1910 ); //, w->var("D0_M")->getMin(), w->var("D0_M")->getMax() );
  plot( "D0_LTIME_ps", "Data", "d0_t"     , 0., 5.); //, w->var("D0_LTIME_ps")->getMin(), w->var("D0_LTIME_ps")->getMax() );
}



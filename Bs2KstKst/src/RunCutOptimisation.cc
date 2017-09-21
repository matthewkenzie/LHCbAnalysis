#include <iostream>

#include "TROOT.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TH1F.h"
#include "TLegend.h"

#include "RunCutOptimisation.h"

using namespace std;

Bs2KstKst::RunCutOptimisation::RunCutOptimisation(TString fname, TString tname):
  nExpSigEvs(1000),
  nExpBkgEvs(10000),
  nExpMisEvs(100)
{
  inf = new TFile( fname );
  tree = (TTree*)inf->Get(tname);
  gROOT->ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C");
  system("mkdir -p plots/CutOptimisation/pdf");

}

Bs2KstKst::RunCutOptimisation::~RunCutOptimisation(){
  inf->Close();
  delete inf;
}

void Bs2KstKst::RunCutOptimisation::printOptResult( vector<TGraph*> curves, vector<int> fomTypes, TString var) {

  cout << "OPTIMISATION RESULT: " << endl;
  cout << "\t Var: " << var << endl;
  for ( int i=0; i<curves.size(); i++ ) {
    double x,y;
    float max=0.;
    float cut_val=-999;
    for ( int p=0; p<curves[i]->GetN(); p++ ) {
      curves[i]->GetPoint(p,x,y);
      if ( y>max ) {
        max = y;
        cut_val = x;
      }
    }
    cout << "\t FOM: " << fomTypes[i] << " : " << cut_val << endl;
  }
}

void Bs2KstKst::RunCutOptimisation::plotOptCurves( vector<TGraph*> curves, vector<int> fomTypes, TString var, float xmin, float xmax, TString plotext) {

  int colors[4] = { kBlack, kBlue, kRed, kGreen+2 };

  // find max
  double max=0.;
  for (int i=0; i<curves.size(); i++) {
    double x,y;
    for ( int p=0; p<curves[i]->GetN(); p++ ) {
      curves[i]->GetPoint(p,x,y);
      max = TMath::Max(max,y);
    }
  }

  TH1F dum("dum","dum",1,xmin,xmax);
  dum.GetXaxis()->SetTitle(var);
  dum.GetYaxis()->SetTitle("FOM");
  dum.GetXaxis()->SetTitleOffset(0.8);
  dum.GetYaxis()->SetTitleOffset(0.8);
  dum.GetYaxis()->SetRangeUser(0.,max*1.05);

  TCanvas *canv = new TCanvas();
  dum.Draw("HIST");

  TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
  leg->SetFillColor(0);

  for ( int i=0; i<curves.size(); i++ ) {
    curves[i]->SetLineColor(colors[i]);
    curves[i]->SetLineWidth(2);
    leg->AddEntry( curves[i], getFOMName( fomTypes[i] ), "L" );
    curves[i]->Draw("Lsame");
  }

  leg->Draw("same");
  canv->Update();
  canv->Modified();
  canv->Print(Form("plots/CutOptimisation/pdf/%s%s.pdf",var.Data(),plotext.Data()));

}

TString Bs2KstKst::RunCutOptimisation::getFOMName( int type ) {

  if (type==0) return "S/#sqrt{S+B}";
  else if (type==1) return "S/#sqrt{S+B+M}";
  else if (type==2) return "S/(S+B+M) #times S/#sqrt{S+B+M}";
  else return "";
}

float Bs2KstKst::RunCutOptimisation::getFOM( int fomType, float nSig, float nBkg, float nMis ) {

  float fom = -999.;

  // S/sqrt(S+B)
  if (fomType==0)        fom = nSig/(TMath::Sqrt(nSig+nBkg));
  // S/sqrt(S+B+M)
  else if (fomType==1)   fom = nSig/(TMath::Sqrt(nSig+nBkg+nMis));
  // S/sqrt(S+B+M) * Purity
  else if (fomType==2)   fom = (nSig/(TMath::Sqrt(nSig+nBkg+nMis)) * (nSig/(nSig+nBkg+nMis)));

  if ( fom!=fom ) fom=0; // nan protect
  return fom;

}

void Bs2KstKst::RunCutOptimisation::optimise( TString var, vector<int> fomTypes, int nsteps, float min, float max, TString plotext, TString operation)
{
  TH1F hdum("hdum","",nsteps,min,max);

  // figure out normalisations
  //
  // events with no cuts from tree
  int nSigNoCuts = tree->GetEntries( sigSel );
  int nBkgNoCuts = tree->GetEntries( bkgSel );
  int nMisNoCuts = tree->GetEntries( misSel );

  // normalisation numbers (use the a priori expectation)
  float sigNorm = nExpSigEvs / nSigNoCuts ;
  float bkgNorm = nExpBkgEvs / nBkgNoCuts ;
  float misNorm = nExpMisEvs / nMisNoCuts ;

  vector<TGraph*> res;
  for (int i=0; i<fomTypes.size(); i++) {
    res.push_back( new TGraph() );
  }

  for (int step=0; step<nsteps; step++) {

    float cut_val = hdum.GetBinLowEdge( step+1 );

    TString cut_string = Form( "(%s %s %6.4f)", var.Data(), operation.Data(), cut_val );

    int nSigAfterCut = tree->GetEntries( sigSel + " && " + cut_string );
    int nBkgAfterCut = tree->GetEntries( bkgSel + " && " + cut_string );
    int nMisAfterCut = tree->GetEntries( misSel + " && " + cut_string );

    float nExpSigAfterCut = sigNorm * nSigAfterCut;
    float nExpBkgAfterCut = bkgNorm * nBkgAfterCut;
    float nExpMisAfterCut = misNorm * nMisAfterCut;

    for (int i=0; i<fomTypes.size(); i++) {
      float fom = getFOM( fomTypes[i], nExpSigAfterCut, nExpBkgAfterCut, nExpMisAfterCut );
      res[i]->SetPoint(step,cut_val,fom);
    }
  }

  plotOptCurves( res, fomTypes, var, min, max, plotext );
  printOptResult( res, fomTypes, var );

}

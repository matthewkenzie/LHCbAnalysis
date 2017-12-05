#include <iostream>

#include "TROOT.h"
#include "TFile.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "MassFitPlotter.h"

using namespace std;
using namespace TMath;
using namespace Bs2KstKst;

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

  return 0;
}

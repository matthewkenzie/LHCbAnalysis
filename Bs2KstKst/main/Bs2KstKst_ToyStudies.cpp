#include <iostream>
#include <string>

#include "TFile.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TString.h"

#include "RooWorkspace.h"
#include "RooPlot.h"
#include "RooMsgService.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooAbsData.h"
#include "RooDataSet.h"
#include "RooAbsPdf.h"
#include "RooSimultaneous.h"

#include "MassFitPlotter.h"

using namespace std;
using namespace TMath;
using namespace RooFit;
using namespace Bs2KstKst;

int main() {
  
  gROOT->ProcessLine(".x ~/lhcbStyle.C");
  system("mkdir -p plots/Toys/png");
  system("mkdir -p plots/Toys/pdf");
  system("mkdir -p plots/Toys/C");

  //RooMsgService::instance().setGlobalKillBelow(FATAL);
  //RooMsgService::instance().setSilentMode(true);

  TFile *inf = TFile::Open("root/MassFit/MassFitResultWSWeights.root");
  RooWorkspace *w = (RooWorkspace*)inf->Get("w");
  w->Print();
  
  // do some check plots to make sure things looks fairly reasonable
  w->loadSnapshot("constrained_pdf_fit");
  MassFitPlotter *plotter = new MassFitPlotter( w, "Toys" );
  plotter->makeDataPlot( "DataFit", "DataCombined", "All" );

  RooAbsPdf *pdf = (RooAbsPdf*)w->pdf("constrained_pdf");
  pdf->Print();

  // throw a few toys for practise
  int nToys = 10;
  for (int i=0; i<nToys; i++) {
    cout << "Toy " << i+1 << " / " << nToys << endl;
    RooDataSet *toy = pdf->generate(RooArgSet(*w->var("B_s0_DTF_B_s0_M"),*w->cat("DataCat")),w->data("DataCombined")->numEntries(),Extended());
    toy->Print("v");
    TString toyName = TString(Form("Toy%d",i));
    toy->SetName(toyName);
    w->import(*toy);
    w->loadSnapshot("constrained_pdf_fit");
    //plotter->plot( "B_s0_DTF_B_s0_M", toyName );
    pdf->fitTo( *toy, Constrain( *w->set("constrained_vars") ), Extended(), Save() );
    plotter->makeDataPlot( Form("Toy%dFit",i), Form("Toy%d",i), "All" );
  }
    
  inf->Close();

  return 0;

}

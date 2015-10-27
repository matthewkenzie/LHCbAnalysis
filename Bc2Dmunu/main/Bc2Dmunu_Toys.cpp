#include <iostream>
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooHistPdf.h"
#include "RooArgList.h"
#include "RooDataSet.h"

using namespace std;
using namespace RooFit;

int main(int argc, char **argv) {

  system("mkdir -p plots/Toys/pdf");
  system("mkdir -p plots/Toys/png");
  system("mkdir -p plots/Toys/C");

  if (argc!=2) {
    cout << "Pass filename as first argument" << endl;
    exit(1);
  }
  TFile *tf = TFile::Open( argv[1] );
  RooWorkspace *w = (RooWorkspace*)tf->Get("w");

  double lumi = 25.0;
  double scale = 3000./lumi;

  double pdg_d_kpi_fav = 3.88e-2;
  double pdg_d_kpi_sup = 1.38e-4;

  double exp_bu_fav = w->var("sig_y")->getVal() * scale;
  double exp_bu_sup = exp_bu_fav * ( pdg_d_kpi_sup/pdg_d_kpi_fav );

  double exp_bc = exp_bu_fav*(0.01)*(0.3)*(1./500.)*(25./35.); // (Vub/Vcb)^2 * 0.3 ( as Bc goes c->s 70%) * f(Bc/Bu)~1/500 * trigger (25/35)

  cout << "Obs Bu Fav: " << exp_bu_fav << endl;
  cout << "Exp Bu Sup: " << exp_bu_sup << endl;
  cout << "Exp Bc    : " << exp_bc << endl;

  RooHistPdf *bu_pdf = (RooHistPdf*)w->pdf("bu2dmunu_mc_pdf");
  RooHistPdf *bc_pdf = (RooHistPdf*)w->pdf("bc2dmunu_mc_pdf");

  RooRealVar *bu_y       = new RooRealVar("bu_y","bu_y", exp_bu_sup,0,10e4);
  RooRealVar *bc_y       = new RooRealVar("bc_y","bc_y", exp_bc,-10e4,10e4);
  //RooRealVar *frac_buobc = new RooRealVar("frac","", (exp_bc/exp_bu_sup),0.,0.1);
  //RooFormulaVar *bc_y = new RooFormulaVar("bc_y","bc_y","@0*@1",RooArgList(*bu_y,*frac_buobc));

  RooAddPdf *pdf = new RooAddPdf("pdf","",RooArgList(*bu_pdf,*bc_pdf),RooArgList(*bu_y,*bc_y));

  RooDataSet* dset = pdf->generate( RooArgSet(*w->var("Bplus_Mcorr")), exp_bc+exp_bu_sup, Extended() );
  pdf->fitTo(*dset,Extended());

  TCanvas *c = new TCanvas();
  RooPlot *p = w->var("Bplus_Mcorr")->frame();
  dset->plotOn(p);
  pdf->plotOn(p);
  p->Draw();
  c->Print("plots/Toys/pdf/pdf.pdf");
  c->Print("plots/Toys/png/pdf.png");
  c->Print("plots/Toys/C/pdf.C");

  TH1F *frac_err = new TH1F("f_err","",100,0.,1.5);
  TH1F *err = new TH1F("err","",100,50,80);
  TH1F *val = new TH1F("bc_y","",100,-200,500);
  frac_err->Fill( bc_y->getError() / bc_y->getVal() );
  err->Fill( bc_y->getError() );
  val->Fill( bc_y->getVal() );
  //frac_err->Fill( frac_buobc->getError() / frac_buobc->getVal() );

  const int nToys = 2000.;
  for ( int i=0; i<nToys; i++ ) {
    bu_y->setVal( exp_bu_sup );
    bc_y->setVal( exp_bc );
    //frac_buobc->setVal( exp_bc/exp_bu_sup );
    RooDataSet *dset = pdf->generate( RooArgSet(*w->var("Bplus_Mcorr")), exp_bc+exp_bu_sup, Extended() );
    pdf->fitTo(*dset,Extended());
    //frac_err->Fill( frac_buobc->getError() / frac_buobc->getVal() );
    frac_err->Fill( bc_y->getError() / bc_y->getVal() );
    err->Fill( bc_y->getError() );
    val->Fill( bc_y->getVal() );
  }

  TCanvas *c2 = new TCanvas();
  frac_err->Fit("gaus");
  frac_err->Draw("HIST");
  c2->Print("plots/Toys/pdf/toys.pdf");
  c2->Print("plots/Toys/png/toys.png");
  c2->Print("plots/Toys/C/toys.C");

  TCanvas *c3 = new TCanvas();
  err->Fit("gaus");
  err->Draw("HIST");
  c3->Print("plots/Toys/pdf/toys_err.pdf");
  c3->Print("plots/Toys/png/toys_err.png");
  c3->Print("plots/Toys/C/toys_err.C");

  TCanvas *c4 = new TCanvas();
  val->Fit("gaus");
  val->Draw("HIST");
  c4->Print("plots/Toys/pdf/toys_val.pdf");
  c4->Print("plots/Toys/png/toys_val.png");
  c4->Print("plots/Toys/C/toys_val.C");

  TFile *outf = new TFile("root/ToysOut.root","recreate");
  w->Write();
  frac_err->Write();
  err->Write();
  val->Write();
  outf->Close();
  tf->Close();
}

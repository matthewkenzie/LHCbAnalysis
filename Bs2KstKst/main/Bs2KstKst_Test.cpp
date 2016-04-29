#include <iostream>

#include "TFile.h"
#include "TMatrixD.h"

#include "RooWorkspace.h"
#include "RooIpatia2.h"
#include "RooAbsPdf.h"
#include "RooFitResult.h"
#include "RooAddPdf.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooConstVar.h"

#include "RooStats/SPlot.h"
#include "RooStats/RooStatsUtils.h"

using namespace std;
using namespace RooFit;

int main() {

  TFile *tf = TFile::Open("root/MassFitResult.root");
  RooWorkspace *w = (RooWorkspace*)tf->Get("w");

  RooDataSet *data = (RooDataSet*)w->data("Data2012HadronTOS");
  //w->loadSnapshot("bs2kstkst_mc_pdf_fit");

  //RooRealVar *bs2kstkst_l      = new RooRealVar("bs2kstkst_l"      , "", -5., -20., 0.);
  //RooConstVar *bs2kstkst_zeta  = new RooConstVar("bs2kstkst_zeta" , "", 0.);
  //RooConstVar *bs2kstkst_fb    = new RooConstVar("bs2kstkst_fb"   , "", 0.);
  //RooRealVar *bs2kstkst_sigma  = new RooRealVar("bs2kstkst_sigma"  , "", 15, 10, 100);
  //RooRealVar *bs2kstkst_mu     = new RooRealVar("bs2kstkst_mu"     , "", 5350, 5400 );
  //RooRealVar *bs2kstkst_a      = new RooRealVar("bs2kstkst_a"      , "", 2.5,0,10);
  //RooRealVar *bs2kstkst_n      = new RooRealVar("bs2kstkst_n"      , "", 2.5,0,10);
  //RooRealVar *bs2kstkst_a2     = new RooRealVar("bs2kstkst_a2"     , "", 2.5,0,10);
  //RooRealVar *bs2kstkst_n2     = new RooRealVar("bs2kstkst_n2"     , "", 2.5,0,10);

  //RooIpatia2 *sig = new RooIpatia2("sig","",*w->var("B_s0_DTF_B_s0_M"), *bs2kstkst_l, *bs2kstkst_zeta, *bs2kstkst_fb, *bs2kstkst_sigma, *bs2kstkst_mu, *bs2kstkst_a, *bs2kstkst_n, *bs2kstkst_a2, *bs2kstkst_n2);
  //RooAbsPdf *sig = (RooAbsPdf*)w->pdf("bs2kstkst_mc_pdf");
  RooIpatia2 *sig = new RooIpatia2("bs2kstkst_mc_pdf","bs2kstkst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bs2kstkst_l"),*w->var("bs2kstkst_zeta"),*w->var("bs2kstkst_fb"),*w->var("bs2kstkst_sigma"),*w->var("bs2kstkst_mu"),*w->var("bs2kstkst_a"),*w->var("bs2kstkst_n"),*w->var("bs2kstkst_a2"),*w->var("bs2kstkst_n2"));

  RooAbsPdf *bkg = (RooAbsPdf*)w->pdf("bkg_pdf_HadronTOS2012");

  RooRealVar *sY = (RooRealVar*)w->var("bs2kstkst_y_HadronTOS2012");
  RooRealVar *bY = (RooRealVar*)w->var("bkg_y_HadronTOS2012");

  cout << sig << bkg << sY << bY << endl;

  RooAddPdf *pdf = new RooAddPdf("test","test", RooArgList(*sig,*bkg), RooArgList(*sY,*bY) );

  pdf->fitTo(*data, Extended() );

  // my sw
  double syVal = sY->getVal();
  double byVal = bY->getVal();

  // loop events
  int numevents = data->numEntries();

  sY->setVal(0.);
  bY->setVal(0.);

  RooArgSet *pdfvars = pdf->getVariables();

  vector<double> fsvals;
  vector<double> fbvals;

  for ( int ievt=0; ievt<numevents; ievt++ ) {

    RooStats::SetParameters(data->get(ievt), pdfvars);

    sY->setVal(1.);
    double f_s = pdf->getVal( RooArgSet(*w->var("B_s0_DTF_B_s0_M")) );
    fsvals.push_back(f_s);
    sY->setVal(0.);

    bY->setVal(1.);
    double f_b = pdf->getVal( RooArgSet(*w->var("B_s0_DTF_B_s0_M")) );
    fbvals.push_back(f_b);
    bY->setVal(0.);

    //cout << f_s << " " << f_b << endl;

  }

  TMatrixD covInv(2,2);
  covInv[0][0] = 0;
  covInv[0][1] = 0;
  covInv[1][0] = 0;
  covInv[1][1] = 0;

  for ( int ievt=0; ievt<numevents; ievt++ ) {
    data->get(ievt);
    double dsum=0;
    dsum += fsvals[ievt] * syVal;
    dsum += fbvals[ievt] * byVal;

    covInv[0][0] += fsvals[ievt]*fsvals[ievt] / (dsum*dsum);
    covInv[0][1] += fsvals[ievt]*fbvals[ievt] / (dsum*dsum);
    covInv[1][0] += fbvals[ievt]*fsvals[ievt] / (dsum*dsum);
    covInv[1][1] += fbvals[ievt]*fbvals[ievt] / (dsum*dsum);

  }

  covInv.Print();

  cout << covInv.Determinant() << endl;

  TMatrixD covMatrix(TMatrixD::kInverted,covInv);

  covMatrix.Print();

  RooStats::SPlot *sD = new RooStats::SPlot("sD","sD",*data,pdf,RooArgSet(*sY,*bY),RooArgSet(*w->var("eventNumber")));
}

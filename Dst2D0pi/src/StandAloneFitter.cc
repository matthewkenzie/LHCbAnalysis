#include "StandAloneFitter.h"

#include "TH2F.h"
#include "TPaveText.h"
#include "TLegend.h"

#include "RooAbsPdf.h"
#include "RooHistPdf.h"
#include "RooArgList.h"

using namespace std;
using namespace RooFit;

Dst2D0pi::StandAloneFitter::StandAloneFitter():
  isMassFit(false),
  haveYields(false),
  isIPCHI2Fit(false),
  signalWindowMin(-1),
  signalWindowMax(-1),
  nSigInWindow(-1),
  nBkgInWindow(-1),
  ipCutSet(false),
  ipCut(-99),
  haveFinalYields(false),
  nPrompt(-1),
  nSec(-1),
  nBkg(-1)
{
  w = new RooWorkspace("w","w");
  p = new StandAlonePlotter(w);
}

Dst2D0pi::StandAloneFitter::~StandAloneFitter(){}

void Dst2D0pi::StandAloneFitter::run(){

  cout << "Fitter" << endl;

  dataFile = new TFile("nTuples/NewFiles/TurboOut_hadd.root");
  dataTree = (TTree*)dataFile->Get("TeslaTuple/DecayTree");

  mcFile = new TFile("nTuples/NewFiles/MCOut_hadd.root");
  mcTree = (TTree*)mcFile->Get("MCTuple/DecayTree");

  cuts = "(Dst_L0HadronDecision_TOS && Dst_Hlt1CalibTrackingKPiDecision_TOS && Dst_Hlt2CharmHadDstp2D0Pip_D02KmPip_LTUNBTurboDecision_TOS) && (Dst_M-D0_M) > 144 && (Dst_M-D0_M) < 147 && (D0_LTIME*1000>0.25) && (D0_LTIME*1000<4.)";
  // as above but no trigger
  mccuts_prompt = "(Dst_M-D0_M) > 144 && (Dst_M-D0_M) < 147 && (D0_LTIME*1000>0.25) && (D0_LTIME*1000<4.)";
  mccuts_sec    = "(Dst_M-D0_M) > 144 && (Dst_M-D0_M) < 147 && (D0_LTIME*1000>0.25) && (D0_LTIME*1000<4.)";

  // D0 mass fit
  D0MassFit();

  // Integrate n_sig, n_bkg in signal window
  SetSignalWindow( 1840, 1890 );
  GetYieldsInMassWindow();

  // IP chi2 fit
  D0IPChi2Fit();

  // Integrate n_sig, n_prompt, n_sec in ip chi2 cut
  SetIPCut(1.);
  GetYieldsFromIPCut();

  // Lifetime fit
  D0LifetimeFit();

  // Print some stuff
  cout << " --------- After D0 Mass Fit ---------- " << endl;
  cout << "\t nSigInWindow: " << nSigInWindow << endl;
  cout << "\t nBkgInWindow: " << nBkgInWindow << endl;
  cout << " --------- After IP chi2 Fit --------- " << endl;
  cout << "\t nPrompt: " << nPrompt << endl;
  cout << "\t nSec:    " << nSec << endl;
  cout << "\t nBkg:    " << nBkg << endl;
  cout << " --------- After lifetime Fit --------- " << endl;
  cout << "\t nPrompt: " << w->var("n_prompt")->getVal() << endl;
  cout << "\t nSec:    " << w->function("n_sec")->getVal() << endl;
  cout << "\t nbkg:    " << w->var("n_bkg")->getVal() << endl;
  cout << "\t tau:     " << w->var("D0_tau")->getVal() << endl;
  cout << "\t Quote:   " << Form("t = ( %6.2f +/- %4.2f ) fs", w->var("D0_tau")->getVal()*1000., w->var("D0_tau")->getError()*1000.) << endl;
  cout << " ------------------------------------- " << endl;

  w->writeToFile("root/StandAloneFitterOut.root");
}

void Dst2D0pi::StandAloneFitter::D0MassFit() {

  // make variable
  w->factory( "D0_M[1810,1920]" );
  w->var("D0_M")->setBins(2200);
  w->var("D0_M")->setUnit("MeV");
  w->var("D0_M")->SetTitle("m(D0)");
  RooRealVar *D0_M = (RooRealVar*)w->var("D0_M");

  // make dataset
  TH1F *D0_M_th1f = new TH1F( "D0_M_th1f", "", D0_M->getBins(), D0_M->getMin(), D0_M->getMax() );
  dataTree->Draw( "D0_M >> D0_M_th1f", cuts, "goff" );
  RooDataHist *D0_M_dh = new RooDataHist( "D0_M_dh", "", RooArgList( *D0_M ), Import( *D0_M_th1f ) );
  w->import( *D0_M_dh );
  delete D0_M_dh;

  // mass pdf
  // sig
  w->factory( "Gaussian::D0_M_gaus1( D0_M, D0_M_mean[1810,1920], D0_M_sigma1[10,0,100] )" );
  w->factory( "Gaussian::D0_M_gaus2( D0_M, D0_M_mean, D0_M_sigma2[10,0,100] )" );
  w->factory( "CBShape::D0_M_cb( D0_M, D0_M_mean, D0_M_sigma3[10,0,100], D0_M_alpha[1,-10,10], D0_M_n[3.]  )" );
  w->factory( "SUM::D0_M_sig( D0_M_f1[0.2,0.,1.]*D0_M_gaus1, D0_M_cb )" );
  // bkg
  w->factory( "Exponential::D0_M_bkg( D0_M, D0_M_e1[-0.0002,-0.01,0.] )" );
  // pdf
  w->factory( "SUM::D0_M_pdf( n_bkg[5000,10,10e5]*D0_M_bkg, n_sig[15e3,10,20e5]*D0_M_sig )" );
  // fit
  w->pdf("D0_M_pdf")->fitTo( *w->data("D0_M_dh"), Extended() );

  // plot
  PlotComponent pc_data( "D0_M_dh", "Data" );
  pc_data.setDefaultDataStyle();
  pc_data.binning = 110;

  PlotComponent pc_bkg( "D0_M_pdf:D0_M_bkg", "Combinatorial PDF" );
  pc_bkg.setDashedLine( kRed );

  PlotComponent pc_sig( "D0_M_pdf:D0_M_sig", "Signal PDF" );
  pc_sig.setDashedLine( kGreen+2 );

  PlotComponent pc_pdf( "D0_M_pdf", "Total PDF" );
  pc_pdf.setSolidLine( kBlue );

  vector<PlotComponent> plotComps;
  plotComps.push_back(pc_data);
  plotComps.push_back(pc_bkg);
  plotComps.push_back(pc_sig);
  plotComps.push_back(pc_pdf);

  p->pResidType = 2;
  p->pTitle = "D0 Mass Fit";
  p->pDrawLog = true;
  p->pRedPull = 3;

  //p->drawBox( 1840, 1890, kBlue-7 );
  //p->drawBox( D0_M->getMin(), 1840, kRed-7 );
  //p->drawBox( 1890, D0_M->getMax(), kRed-7 );

  p->plot( "D0_M", plotComps, "mass" );

  isMassFit = true;
}

void Dst2D0pi::StandAloneFitter::GetYieldsInMassWindow( ) {

  assert( isMassFit );
  assert( signalWindowSet );

  w->var("D0_M")->setRange( "SignalMassWindow", signalWindowMin, signalWindowMax );

  RooAbsReal *sig_integral = w->pdf("D0_M_sig")->createIntegral( *w->var("D0_M"), NormSet( *w->var("D0_M") ), Range( "SignalMassWindow" ) );
  RooAbsReal *bkg_integral = w->pdf("D0_M_bkg")->createIntegral( *w->var("D0_M"), NormSet( *w->var("D0_M") ), Range( "SignalMassWindow" ) );

  nSigInWindow = sig_integral->getVal()*w->var("n_sig")->getVal();
  nBkgInWindow = bkg_integral->getVal()*w->var("n_bkg")->getVal();

  // cross check it
  RooAbsReal *mass_integral = w->pdf("D0_M_pdf")->createIntegral( *w->var("D0_M"), NormSet( *w->var("D0_M") ), Range( "SignalMassWindow" ) );
  double nEvsInWindow = mass_integral->getVal()*( w->var("n_sig")->getVal() + w->var("n_bkg")->getVal() );

  assert( TMath::Abs( nEvsInWindow - nSigInWindow - nBkgInWindow ) < 0.01 );

  haveYields = true;

}

void Dst2D0pi::StandAloneFitter::D0IPChi2Fit() {

  assert( isMassFit );
  assert( haveYields );

  // make variable
  w->factory( "D0_LOGIPCHI2[-10,15]" );
  w->var("D0_LOGIPCHI2")->setBins(2500);
  w->var("D0_LOGIPCHI2")->setUnit("");
  w->var("D0_LOGIPCHI2")->SetTitle("log(IP #chi^{2}) (D0)");
  RooRealVar *D0_LOGIPCHI2 = (RooRealVar*)w->var("D0_LOGIPCHI2");

  // make datasets
  // data
  TH1F *D0_LOGIPCHI2_th1f = new TH1F( "D0_LOGIPCHI2_th1f", "", D0_LOGIPCHI2->getBins(), D0_LOGIPCHI2->getMin(), D0_LOGIPCHI2->getMax() );
  TString window_cuts = cuts + TString( Form(" && (D0_M > %6.1f && D0_M < %6.1f)", signalWindowMin, signalWindowMax) );
  dataTree->Draw( "TMath::Log(D0_MINIPCHI2) >> D0_LOGIPCHI2_th1f", cuts, "goff" );
  RooDataHist *D0_LOGIPCHI2_dh = new RooDataHist( "D0_LOGIPCHI2_dh", "", RooArgList( *D0_LOGIPCHI2 ), Import( *D0_LOGIPCHI2_th1f ) );
  w->import( *D0_LOGIPCHI2_dh );
  delete D0_LOGIPCHI2_dh;
  // data in mass sidebands (to get combinatorial shape)
  TH1F *D0_LOGIPCHI2_th1f_sb = new TH1F( "D0_LOGIPCHI2_th1f_sb", "", D0_LOGIPCHI2->getBins(), D0_LOGIPCHI2->getMin(), D0_LOGIPCHI2->getMax() );
  TString sb_cuts = cuts + TString( Form(" && (D0_M < %6.1f || D0_M > %6.1f) ",signalWindowMin,signalWindowMax) );
  dataTree->Draw( "TMath::Log(D0_MINIPCHI2) >> D0_LOGIPCHI2_th1f_sb", sb_cuts, "goff" );
  RooDataHist *D0_LOGIPCHI2_dh_sb = new RooDataHist( "D0_LOGIPCHI2_dh_sb", "", RooArgList( *D0_LOGIPCHI2 ), Import( *D0_LOGIPCHI2_th1f_sb ) );
  w->import( *D0_LOGIPCHI2_dh_sb );
  delete D0_LOGIPCHI2_dh_sb;
  // mc prompt and secondary
  TH1F *D0_LOGIPCHI2_th1f_prompt_mc = new TH1F("D0_LOGIPCHI2_th1f_prompt_mc","",D0_LOGIPCHI2->getBins(), D0_LOGIPCHI2->getMin(), D0_LOGIPCHI2->getMax());
  TH1F *D0_LOGIPCHI2_th1f_sec_mc = new TH1F("D0_LOGIPCHI2_th1f_sec_mc","",D0_LOGIPCHI2->getBins(),D0_LOGIPCHI2->getMin(),D0_LOGIPCHI2->getMax());
  mcTree->Draw( "TMath::Log(D0_MINIPCHI2) >> D0_LOGIPCHI2_th1f_prompt_mc", mccuts_prompt, "goff" );
  mcTree->Draw( "TMath::Log(D0_MINIPCHI2) >> D0_LOGIPCHI2_th1f_sec_mc", mccuts_sec, "goff" );
  RooDataHist *D0_LOGIPCHI2_dh_prompt_mc = new RooDataHist("D0_LOGIPCHI2_dh_prompt_mc","",RooArgList(*w->var("D0_LOGIPCHI2")), Import(*D0_LOGIPCHI2_th1f_prompt_mc));
  RooDataHist *D0_LOGIPCHI2_dh_sec_mc = new RooDataHist("D0_LOGIPCHI2_dh_sec_mc","",RooArgList(*w->var("D0_LOGIPCHI2")), Import(*D0_LOGIPCHI2_th1f_sec_mc));
  w->import( *D0_LOGIPCHI2_dh_prompt_mc );
  w->import( *D0_LOGIPCHI2_dh_sec_mc );
  delete D0_LOGIPCHI2_dh_prompt_mc;
  delete D0_LOGIPCHI2_dh_sec_mc;

  // prompt
  w->factory( "Gaussian::D0_LOGIPCHI2_prompt_g1 (D0_LOGIPCHI2, D0_LOGIPCHI2_prompt_m[0.4,1.2], D0_LOGIPCHI2_prompt_s1[1,0.1,2])" );
  w->factory( "Gaussian::D0_LOGIPCHI2_prompt_g2 (D0_LOGIPCHI2, D0_LOGIPCHI2_prompt_m, D0_LOGIPCHI2_prompt_s2[1,0.1,8])" );
  w->factory( "BifurGauss::D0_LOGIPCHI2_prompt_bf( D0_LOGIPCHI2, D0_LOGIPCHI2_prompt_m, D0_LOGIPCHI2_prompt_sL[5.,1.,10], D0_LOGIPCHI2_prompt_sR[1,0.1,5])" );
  w->factory( "CBShape::D0_LOGIPCHI2_prompt_cb( D0_LOGIPCHI2, D0_LOGIPCHI2_prompt_m, D0_LOGIPCHI2_prompt_s3[5.,1.,10], D0_LOGIPCHI2_prompt_alpha[1,-10.,10], D0_LOGIPCHI2_prompt_n[3.])" );
  w->factory( "SUM::D0_LOGIPCHI2_prompt_pdf( D0_LOGIPCHI2_f1[0.2,0.,1.]*D0_LOGIPCHI2_prompt_g1, D0_LOGIPCHI2_f2[0.2,0.,1.]*D0_LOGIPCHI2_prompt_g2, D0_LOGIPCHI2_prompt_bf)" );
  //w->factory( "SUM::D0_LOGIPCHI2_prompt_pdf( D0_LOGIPCHI2_f1[0.2,0.,1.]*D0_LOGIPCHI2_prompt_g1, D0_LOGIPCHI2_prompt_cb)" );
  // sec
  w->factory( "alpha[1,0.5,10.]");
  w->factory( "prod::D0_LOGIPCHI2_sec_s1( alpha , D0_LOGIPCHI2_prompt_s1 )" );
  w->factory( "prod::D0_LOGIPCHI2_sec_s2( alpha , D0_LOGIPCHI2_prompt_s2 )" );
  w->factory( "prod::D0_LOGIPCHI2_sec_s3( alpha , D0_LOGIPCHI2_prompt_s3 )" );
  w->factory( "prod::D0_LOGIPCHI2_sec_sL( alpha , D0_LOGIPCHI2_prompt_sL )" );
  w->factory( "prod::D0_LOGIPCHI2_sec_sR( alpha , D0_LOGIPCHI2_prompt_sR )" );
  w->factory( "Gaussian::D0_LOGIPCHI2_sec_g1( D0_LOGIPCHI2, D0_LOGIPCHI2_sec_m[1,10], D0_LOGIPCHI2_sec_s1)" );
  w->factory( "Gaussian::D0_LOGIPCHI2_sec_g2( D0_LOGIPCHI2, D0_LOGIPCHI2_sec_m, D0_LOGIPCHI2_sec_s2)" );
  w->factory( "BifurGauss::D0_LOGIPCHI2_sec_bf( D0_LOGIPCHI2, D0_LOGIPCHI2_sec_m, D0_LOGIPCHI2_sec_sL, D0_LOGIPCHI2_sec_sR)" );
  w->factory( "CBShape::D0_LOGIPCHI2_sec_cb( D0_LOGIPCHI2, D0_LOGIPCHI2_sec_m, D0_LOGIPCHI2_sec_s3, D0_LOGIPCHI2_sec_alpha[1,-10.,10], D0_LOGIPCHI2_sec_n[3.])" );
  //w->factory( "SUM::D0_LOGIPCHI2_sec_pdf( D0_LOGIPCHI2_f1*D0_LOGIPCHI2_sec_g1, D0_LOGIPCHI2_f2*D0_LOGIPCHI2_sec_g2, D0_LOGIPCHI2_sec_bf)" );
  w->factory( "SUM::D0_LOGIPCHI2_sec_pdf( D0_LOGIPCHI2_f1*D0_LOGIPCHI2_sec_g1, D0_LOGIPCHI2_sec_cb)" );
  // bkg
  TH1F *D0_LOGIPCHI2_th1f_sb_rb = (TH1F*)D0_LOGIPCHI2_th1f_sb->Clone(Form("%s_rb",D0_LOGIPCHI2_th1f_sb->GetName()));
  D0_LOGIPCHI2_th1f_sb_rb->Rebin(25);
  RooDataHist *D0_LOGIPCHI2_dh_sb_rb = new RooDataHist("D0_LOGIPCHI2_dh_sb_rb","",RooArgList(*w->var("D0_LOGIPCHI2")), Import(*D0_LOGIPCHI2_th1f_sb_rb));
  RooHistPdf *D0_LOGIPCHI2_bkg_pdf = new RooHistPdf("D0_LOGIPCHI2_bkg_pdf","",RooArgList(*w->var("D0_LOGIPCHI2")), *D0_LOGIPCHI2_dh_sb_rb);
  w->import( *D0_LOGIPCHI2_dh_sb_rb );
  w->import( *D0_LOGIPCHI2_bkg_pdf );

  // the pdf
  w->var("n_bkg")->setVal( nBkgInWindow );
  w->var("n_sig")->setVal( nSigInWindow );
  w->var("n_bkg")->setConstant(true);
  w->var("n_sig")->setConstant(true);
  w->factory(Form("n_prompt[5e3,10e2,%f]",0.95*w->var("n_sig")->getVal()));
  w->factory( "expr::n_sec( \"@0-@1\",n_sig,n_prompt )");

  // fit to data
  w->var("n_sig")->setConstant(true);
  w->var("n_bkg")->setConstant(true);
  w->factory("SUM::D0_LOGIPCHI2_pdf( n_bkg*D0_LOGIPCHI2_bkg_pdf, n_prompt*D0_LOGIPCHI2_prompt_pdf, n_sec*D0_LOGIPCHI2_sec_pdf )");
  w->pdf("D0_LOGIPCHI2_pdf")->fitTo( *w->data("D0_LOGIPCHI2_dh"), Extended() );

  // plot
  PlotComponent pc_data( "D0_LOGIPCHI2_dh", "Data" );
  pc_data.setDefaultDataStyle();
  pc_data.binning = 250;

  PlotComponent pc_bkg( "D0_LOGIPCHI2_pdf:D0_LOGIPCHI2_bkg_pdf", "Combinatorial PDF" );
  pc_bkg.setDashedLine( kRed );

  PlotComponent pc_sec( "D0_LOGIPCHI2_pdf:D0_LOGIPCHI2_sec_pdf", "Secondary PDF" );
  pc_sec.setDashedLine( kMagenta+2 );

  PlotComponent pc_prompt( "D0_LOGIPCHI2_pdf:D0_LOGIPCHI2_prompt_pdf", "Prompt PDF" );
  pc_prompt.setDashedLine( kGreen+2 );

  PlotComponent pc_pdf ( "D0_LOGIPCHI2_pdf", "Total PDF" );
  pc_pdf.setSolidLine( kBlue );

  vector<PlotComponent> plotComps;
  plotComps.push_back( pc_data );
  plotComps.push_back( pc_bkg );
  plotComps.push_back( pc_sec );
  plotComps.push_back( pc_prompt );
  plotComps.push_back( pc_pdf );

  p->pTitle = "D0 log(IP #chi^{2}) Fit";
  p->pDrawLog = true;
  p->pResidType = 2;
  p->plot ( "D0_LOGIPCHI2", plotComps, "logipchi2" );

  isIPCHI2Fit = true;
}

void Dst2D0pi::StandAloneFitter::GetYieldsFromIPCut() {

  assert( isIPCHI2Fit );
  assert( ipCutSet );

  w->var("D0_LOGIPCHI2")->setRange( "IPCutWindow", w->var("D0_LOGIPCHI2")->getMin(), ipCut );

  RooAbsReal *prompt_integral = w->pdf("D0_LOGIPCHI2_prompt_pdf")->createIntegral( *w->var("D0_LOGIPCHI2"), NormSet( *w->var("D0_LOGIPCHI2") ), Range( "IPCutWindow" ) );
  RooAbsReal *sec_integral = w->pdf("D0_LOGIPCHI2_sec_pdf")->createIntegral( *w->var("D0_LOGIPCHI2"), NormSet( *w->var("D0_LOGIPCHI2") ), Range( "IPCutWindow" ) );
  RooAbsReal *bkg_integral = w->pdf("D0_LOGIPCHI2_bkg_pdf")->createIntegral( *w->var("D0_LOGIPCHI2"), NormSet( *w->var("D0_LOGIPCHI2") ), Range( "IPCutWindow" ) );

  nPrompt = prompt_integral->getVal() * w->var("n_prompt")->getVal();
  nSec = sec_integral->getVal() * w->function("n_sec")->getVal();
  nBkg = bkg_integral->getVal() * w->var("n_bkg")->getVal();

  // cross check it
  RooAbsReal *ip_integral = w->pdf("D0_LOGIPCHI2_pdf")->createIntegral( *w->var("D0_LOGIPCHI2"), NormSet( *w->var("D0_LOGIPCHI2") ), Range( "IPCutWindow" ) );
  double nEvs = ip_integral->getVal() * ( w->var("n_sig")->getVal() + w->var("n_bkg")->getVal() );

  assert( TMath::Abs( nEvs - nPrompt - nSec - nBkg ) < 0.01 );

  haveFinalYields = true;

}

void Dst2D0pi::StandAloneFitter::D0LifetimeFit() {

  assert( isMassFit );
  assert( haveYields );
  assert( isIPCHI2Fit );
  assert( haveFinalYields );

  // make variable
  w->factory("D0_t[0.0,5.0]");
  w->var("D0_t")->setBins(2000);
  w->var("D0_t")->setUnit("ps");
  w->var("D0_t")->SetTitle("t(D0)");
  RooRealVar *D0_t = (RooRealVar*)w->var("D0_t");

  // set fit range
  w->var("D0_t")->setRange( "t_fit_range", 0.25, 4. );

  // make datasets
  //
  TString lifetime_cuts = cuts + TString( Form(" && (D0_M > %6.1f && D0_M < %6.1f) && (TMath::Log( D0_MINIPCHI2 ) < %5.3f)", signalWindowMin, signalWindowMax, ipCut) );
  TH1F *D0_t_th1f = new TH1F( "D0_t_th1f", "", D0_t->getBins(), D0_t->getMin(), D0_t->getMax() );
  dataTree->Draw( "D0_LTIME*1000 >> D0_t_th1f", lifetime_cuts, "goff" );
  RooDataHist *D0_t_dh = new RooDataHist( "D0_t_dh", "", RooArgList( *D0_t ), Import( *D0_t_th1f ) );
  w->import( *D0_t_dh );
  delete D0_t_dh;
  // sideband cut not including IP Chi2 cut which shouldn't bias lifetime
  TString sb_cuts = cuts + TString( Form(" && (D0_M < %6.1f || D0_M > %6.1f)", signalWindowMin, signalWindowMax) );
  //TString sb_cuts = cuts + TString( Form(" && (D0_M < %6.1f || D0_M > %6.1f) && (TMath::Log( D0_MINIPCHI2 ) < %5.3f)", signalWindowMin, signalWindowMax, ipCut) );
  TH1F *D0_t_th1f_sb = new TH1F( "D0_t_th1f_sb", "", D0_t->getBins(), D0_t->getMin(), D0_t->getMax() );
  dataTree->Draw( "D0_LTIME*1000 >> D0_t_th1f_sb", sb_cuts, "goff" );
  RooDataHist *D0_t_dh_sb = new RooDataHist( "D0_t_dh_sb", "", RooArgList( *D0_t ), Import( *D0_t_th1f_sb ) );
  w->import( *D0_t_dh_sb );
  delete D0_t_dh_sb;
  TH1F *D0_t_th1f_sec_mc = new TH1F( "D0_t_th1f_sec_mc","", D0_t->getBins(), D0_t->getMin(), D0_t->getMax() );
  mcTree->Draw(" D0_LTIME*1000 >> D0_t_th1f_sec_mc", mccuts_sec, "goff" );
  RooDataHist *D0_t_dh_sec_mc = new RooDataHist( "D0_t_dh_sec_mc", "", RooArgList( *D0_t ), Import( *D0_t_th1f_sec_mc ) );
  w->import( *D0_t_dh_sec_mc );
  delete D0_t_dh_sec_mc;

  cout << mcTree->GetEntries(mccuts_sec) << endl;
  cout << D0_t_th1f_sec_mc->GetEntries() << endl;
  cout << w->data("D0_t_dh_sec_mc")->sumEntries() << endl;

  // try and get lifetime shape for the combinatorial
  w->factory( "Exponential::D0_t_bkg_e1( D0_t, D0_t_bkg_lambda1[-0.8,-10.,-0.1] )" );
  w->factory( "Exponential::D0_t_bkg_e2( D0_t, D0_t_bkg_lambda2[-4., -10.,-2.] )" );
  w->factory( "Exponential::D0_t_bkg_e3( D0_t, D0_t_bkg_lambda3[-20.,-40.,-6.] )" );
  w->factory( "SUM::D0_t_bkg_pdf( D0_t_bkg_f1[0.5,0.01,1.]*D0_t_bkg_e1, D0_t_bkg_f2[0.5,0.01,1.]*D0_t_bkg_e2, D0_t_bkg_e3 )" );
  w->pdf("D0_t_bkg_pdf")->fitTo( *w->data("D0_t_dh_sb") , Range("t_fit_range") );

  PlotComponent pc_bkg_sb( "D0_t_dh_sb", "Data in Sideband" );
  pc_bkg_sb.setDefaultDataStyle();
  pc_bkg_sb.binning = 100;

  PlotComponent pc_bkg_pdf( "D0_t_bkg_pdf", "Combinatorial PDF" );
  pc_bkg_pdf.setSolidLine( kBlue );

  vector<PlotComponent> bkgPlotComps;
  bkgPlotComps.push_back( pc_bkg_sb );
  bkgPlotComps.push_back( pc_bkg_pdf );

  p->pTitle = "D0 t Combinatorial Fit";
  p->plot( "D0_t", bkgPlotComps, "bkg_ltime" );

  // freeze background params
  RooArgSet *params = (RooArgSet*)w->pdf("D0_t_bkg_pdf")->getParameters( RooArgSet( *w->var("D0_t") ) );
  params->setAttribAll("Constant");

  // get lifetime shape for secondaries from MC
  w->factory( "Exponential::D0_t_sec_e1( D0_t, D0_t_sec_lambda1[-100.,0] )" );
  w->factory( "Exponential::D0_t_sec_e2( D0_t, D0_t_sec_lambda2[-20.,0] )" );
  w->factory( "SUM::D0_t_sec_pdf( D0_t_sec_f1[0.8,0.1,1.]*D0_t_sec_e1, D0_t_sec_e2 )" );
  w->pdf( "D0_t_sec_pdf" )->fitTo( *w->data("D0_t_dh_sec_mc"), Range("t_fit_range") );

  PlotComponent pc_sec_mc( "D0_t_dh_sec_mc", "Secondary MC" );
  pc_sec_mc.setDefaultDataStyle();
  pc_sec_mc.binning = 100;

  PlotComponent pc_sec_pdf( "D0_t_sec_pdf", "Secondary PDF" );
  pc_sec_pdf.setSolidLine( kBlue );

  vector<PlotComponent> secPlotComps;
  secPlotComps.push_back(pc_sec_mc);
  secPlotComps.push_back(pc_sec_pdf);

  p->pTitle = "D0 t Secondary Fit";
  p->pTitleX = 0.3;
  p->plot( "D0_t", secPlotComps, "sec_ltime" );

  // freeze secondary params
  RooArgSet *sec_params = (RooArgSet*)w->pdf("D0_t_sec_pdf")->getParameters( RooArgSet( *w->var("D0_t") ) );
  sec_params->setAttribAll("Constant");

  // prompt pdf
  w->factory( "D0_tau[0.408,0.,1.]" );
  w->factory( "expr::D0_prompt_lambda(\"-1/@0\",D0_tau)");
  w->factory( "Exponential::D0_t_prompt_pdf( D0_t, D0_prompt_lambda )" );

  // set yields to where they should be after the IP cut
  w->var("n_prompt")->setConstant(false);
  w->var("n_sig")->setConstant(false);
  w->var("n_bkg")->setConstant(false);
  w->var("n_prompt")->setVal( nPrompt );
  w->var("n_sig")->setVal( nPrompt+nSec );
  w->var("n_bkg")->setVal( nBkg );
  w->var("n_prompt")->setConstant(true);
  w->var("n_sig")->setConstant(true);
  w->var("n_bkg")->setConstant(true);

  // roofit struggles with the normalisation in the range
  w->factory( Form("fPrompt[%6.4f,0.,1.]", nPrompt/(nPrompt+nSec+nBkg)) );
  w->factory( Form("fSec[%6.4f,0.,0.2]", nSec/(nPrompt+nSec+nBkg)) );
  w->var("fPrompt")->setConstant(true);
  w->var("fSec")->setConstant(true);

  // sum pdf
  w->factory( "SUM::D0_t_pdf( fPrompt*D0_t_prompt_pdf, fSec*D0_t_sec_pdf, D0_t_bkg_pdf )" );
  //w->factory( "SUM::D0_t_pdf( n_prompt*D0_t_prompt_pdf, n_sec*D0_t_sec_pdf, n_bkg*D0_t_bkg_pdf )" );

  // fit
  w->pdf("D0_t_pdf")->fitTo( *w->data("D0_t_dh"), Range( "t_fit_range" ) );

  PlotComponent pc_data_invis( "D0_t_dh", "Data" );
  pc_data_invis.setDefaultDataStyle();
  pc_data_invis.noleg = true;
  pc_data_invis.binning = 100;

  PlotComponent pc_bkg( "D0_t_pdf:D0_t_bkg_pdf", "Combinatorial PDF" );
  pc_bkg.setSolidLine( kRed );

  PlotComponent pc_sec( "D0_t_pdf:D0_t_sec_pdf", "Secondary PDF" );
  pc_sec.setSolidLine( kMagenta+2 );

  PlotComponent pc_prompt( "D0_t_pdf:D0_t_prompt_pdf", "Prompt PDF" );
  pc_prompt.setSolidLine( kGreen+2 );

  PlotComponent pc_pdf( "D0_t_pdf", "Total PDF" );
  pc_pdf.setSolidLine( kBlue );

  PlotComponent pc_data( "D0_t_dh", "Data" );
  pc_data.setDefaultDataStyle();
  pc_data.binning = 100;

  vector<PlotComponent> plotComps;
  plotComps.push_back( pc_data_invis );
  plotComps.push_back( pc_bkg );
  plotComps.push_back( pc_sec );
  plotComps.push_back( pc_prompt );
  plotComps.push_back( pc_pdf );
  plotComps.push_back( pc_data );

  p->pTitle = "D0 Lifetime Fit" ;
  p->pTitleX = 0.3;
  p->plot( "D0_t", plotComps, "lifetime" );

}

#include "BasicFitter.h"
#include "RooCBShape.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooAbsData.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooFitResult.h"
#include "RooMinimizer.h"
#include "RooAbsReal.h"
#include "TROOT.h"
#include "TColor.h"
#include "TMath.h"
#include "TH1F.h"
#include "TF1.h"

using namespace std;
using namespace RooFit;

Bc2Dmunu::BasicFitter::BasicFitter(TString wsname,TString name, bool verbose, bool debug):
  FitterBase(wsname,name,verbose,debug)
{}

Bc2Dmunu::BasicFitter::~BasicFitter(){}

void Bc2Dmunu::BasicFitter::addObsVars(){

  addObsVar("Bplus_Mcorr",            "m_{corr} (B^{+})", "MeV/c^{2}", 5000.,8000.);
  addObsVar("Bplus_M"    ,            "m(B^{+})", "MeV/c^{2}", 2000., 8000. );
  addObsVar("bu_rejection_bdtoutput", "BDT", "", float(-0.25),float(1.));
}

void Bc2Dmunu::BasicFitter::addCuts(){

  //addCut("bu_rejection_bdtoutput",float(-0.25), float(1.))
  //addCut("Bplus_Mcorr", double(5000.), double(8000.));
  //addCut("bdtoutput",float(0.),float(1.));
  //addCut("min_kaon_ProbNNk",double(0.5),double(1.));
  //addCut("min_pion_ProbNNpiKp",double(0.),double(1.));

}

void Bc2Dmunu::BasicFitter::addDatasets(){

  // Data
  addDataset("Data12",                "Data12",                80 );
  addDataset("Data11",                "Data11",                70 );

  // MC
  addDataset("Bc2Dmunu12",           "Bc2Dmunu12",           -20);
  addDataset("Bu2Dmunu12",         "Bu2Dmunu12",         -29);
  addDataset("Bu2Dmunu11",           "Bu2Dmunu11",           -19);
}

void Bc2Dmunu::BasicFitter::constructPdfs(){
  //makeBc2DmunuPdf();
  //makeBc2DstmunuPdf();
  //makeBu2DmunuPdf();
  //makeBu2DstmunuPdf();
  //makeCombinatorialPdf();
  //makeTotalPdf();
  makeMassPdfs();
}

void Bc2Dmunu::BasicFitter::run(){

  // in M corr
  //Bu2DmunuFit();
  //Bc2DmunuFit();
  //Bc2DstmunuFit();
  //DataFit();
  MassFit();
  RunToys();

}

void Bc2Dmunu::BasicFitter::makePlots(){

  PlotComponent pc_data( "Data", "Data" );
  pc_data.setDefaultDataStyle();

  PlotComponent pc_data_invis( "Data", "Data Invis" );
  pc_data_invis.noleg = true;

  PlotComponent pc_bkg( "pdf:comb_pdf", "Combinatorial" );
  pc_bkg.setSolidFill(kRed-7);

  PlotComponent pc_sig( "pdf", "Signal" );
  pc_sig.setSolidFill(kBlue-7);

  PlotComponent pc_tot( "pdf", "Total" );
  pc_tot.setSolidLine(kBlue-7);
  pc_tot.noleg = true;

  vector<PlotComponent> plotComps;
  plotComps.push_back(pc_data);
  plotComps.push_back(pc_sig);
  plotComps.push_back(pc_bkg);
  plotComps.push_back(pc_tot);
  plotComps.push_back(pc_data_invis);

  setTitle( "Mass Fit" );
  setDrawLog(false);
  setResidType(2);
  //plot("Bplus_Mcorr", plotComps, "fit");

}

void Bc2Dmunu::BasicFitter::makeBu2DmunuPdf() {

  RooDataSet  *mc_dset  = (RooDataSet*)w->data("Bc2Dmunu");
  RooDataHist *mc_dhist = new RooDataHist("Bc2DmunuHist","",RooArgSet(*w->var("Bplus_Mcorr")),*mc_dset);
  RooHistPdf  *mc_pdf   = new RooHistPdf("bc2dmunu_mc_pdf","",RooArgSet(*w->var("Bplus_Mcorr")),*mc_dhist);
  w->import( *mc_dhist );
  w->import( *mc_pdf   );
  //w->factory( "CBShape::bu2dmunu_mc_cb1( Bplus_Mcorr, m1[4000.,8000.], s1[0.,500.], alpha1[0.5,0.,10.], n1[0.01,0.,10.] )" );
  //w->factory( "CBShape::bu2dmunu_mc_cb2( Bplus_Mcorr, m2[4000.,8000.], s2[0.,500.], alpha2[-0.5,-10.,0.], n2[0.01,0.,10.] )" );
  //w->factory( "SUM::bu2dmunu_mc_pdf( f1[0.6,0.,1.]*bu2dmunu_mc_cb1, bu2dmunu_mc_cb2 )" );
  defineParamSet("bc2dmunu_mc_pdf");
}

void Bc2Dmunu::BasicFitter::makeBu2DstmunuPdf() {
  return;
}

void Bc2Dmunu::BasicFitter::makeBc2DmunuPdf() {
  RooDataSet  *mc_dset  = (RooDataSet*)w->data("Bu2Dmunu");
  RooDataHist *mc_dhist = new RooDataHist("Bu2DmunuHist","",RooArgSet(*w->var("Bplus_Mcorr")),*mc_dset);
  RooHistPdf  *mc_pdf   = new RooHistPdf("bu2dmunu_mc_pdf","",RooArgSet(*w->var("Bplus_Mcorr")),*mc_dhist);
  w->import( *mc_dhist );
  w->import( *mc_pdf   );
  //w->factory( "CBShape::bu2dmunu_mc_cb1( Bplus_Mcorr, m1[4000.,8000.], s1[0.,500.], alpha1[0.5,0.,10.], n1[0.01,0.,10.] )" );
  //w->factory( "CBShape::bu2dmunu_mc_cb2( Bplus_Mcorr, m2[4000.,8000.], s2[0.,500.], alpha2[-0.5,-10.,0.], n2[0.01,0.,10.] )" );
  //w->factory( "SUM::bu2dmunu_mc_pdf( f1[0.6,0.,1.]*bu2dmunu_mc_cb1, bu2dmunu_mc_cb2 )" );
  defineParamSet("bu2dmunu_mc_pdf");
}

void Bc2Dmunu::BasicFitter::makeBc2DstmunuPdf() {
  RooDataSet  *mc_dset  = (RooDataSet*)w->data("Bc2Dstmunu");
  RooDataHist *mc_dhist = new RooDataHist("Bc2DstmunuHist","",RooArgSet(*w->var("Bplus_Mcorr")),*mc_dset);
  RooHistPdf  *mc_pdf   = new RooHistPdf("bc2dstmunu_mc_pdf","",RooArgSet(*w->var("Bplus_Mcorr")),*mc_dhist);
  w->import( *mc_dhist );
  w->import( *mc_pdf   );
  //w->factory( "CBShape::bu2dmunu_mc_cb1( Bplus_Mcorr, m1[4000.,8000.], s1[0.,500.], alpha1[0.5,0.,10.], n1[0.01,0.,10.] )" );
  //w->factory( "CBShape::bu2dmunu_mc_cb2( Bplus_Mcorr, m2[4000.,8000.], s2[0.,500.], alpha2[-0.5,-10.,0.], n2[0.01,0.,10.] )" );
  //w->factory( "SUM::bu2dmunu_mc_pdf( f1[0.6,0.,1.]*bu2dmunu_mc_cb1, bu2dmunu_mc_cb2 )" );
  defineParamSet("bc2dstmunu_mc_pdf");
}

void Bc2Dmunu::BasicFitter::makeCombinatorialPdf() {
  w->factory( "Exponential::comb_pdf( Bplus_Mcorr, e1[-0.002,-0.01,0.] )" );
  defineParamSet("comb_pdf");
}

void Bc2Dmunu::BasicFitter::makeTotalPdf() {
  addParameter("bkg_y", 0, 500e4);
  addParameter("sig_y", 0, 500e4);
  w->factory( "SUM::pdf( bkg_y * comb_pdf, sig_y * bu2dmunu_mc_pdf )" );
  defineParamSet("pdf");
  defineYieldSet("pdf");
}

void Bc2Dmunu::BasicFitter::Bu2DmunuFit() {
  fit("bu2dmunu_mc_pdf", "Bu2Dmunu");
  plot("Bplus_Mcorr", "Bu2Dmunu", "bu2dmunu_mc_pdf", 2);
  //freeze("bu2dmunu_mc_pdf");
}

void Bc2Dmunu::BasicFitter::Bc2DmunuFit() {
  fit("bc2dmunu_mc_pdf", "Bc2Dmunu");
  plot("Bplus_Mcorr", "Bc2Dmunu", "bc2dmunu_mc_pdf", 2);
}

void Bc2Dmunu::BasicFitter::Bc2DstmunuFit() {
  fit("bc2dstmunu_mc_pdf", "Bc2Dstmunu");
  plot("Bplus_Mcorr", "Bc2Dstmunu", "bc2dstmunu_mc_pdf", 2);
}

void Bc2Dmunu::BasicFitter::DataFit() {
  fit("pdf", "Data");
  plot("Bplus_Mcorr", "Data", "pdf",2);
}

void Bc2Dmunu::BasicFitter::makeMassPdfs() {

  // for Bu
  //w->factory( "CBShape::bu_mc_mass_pdf( Bplus_M, bu_mass_m[4000,2000,8000], bu_mass_s[100,0,2000], bu_mass_alpha[1,0,10], bu_mass_n[0.01,0,10] )" );
  //defineParamSet("bu_mc_mass_pdf");
  makeTemplatePDF( "bu_mc_mass_pdf", "Bplus_M", "Bu2Dmunu" );
  //makeDoubleCBPDF( "bu_mc_mass_pdf_cb", "Bplus_M" );
  //makeGaussianPDF( "bu_mc_mass_pdf_gaus", "Bplus_M" );
  //w->var( "bu_mc_mass_pdf_gaus_mean" )->setConstant(false);
  //w->var( "bu_mc_mass_pdf_gaus_mean" )->setVal(4000.);
  //w->var( "bu_mc_mass_pdf_gaus_mean" )->setRange(3800,4200);
  //w->var( "bu_mc_mass_pdf_gaus_sigma" )->setRange(0,500);
  //makeSumPDF( "bu_mc_mass_pdf", "bu_mc_mass_pdf_gaus", "bu_mc_mass_pdf_cb" );
  //makeDisjointGaussianSumPDF( "bu_mc_mass_pdf", "Bplus_M", 5 );

  // for Bc use template
  makeTemplatePDF( "bc_mc_mass_pdf", "Bplus_M", "Bc2Dmunu" );
  // for Bc to Dst
  makeTemplatePDF( "bc_dst_mc_mass_pdf", "Bplus_M", "Bc2Dstmunu" );
  // for Data
  //w->factory( "Gaussian::add_gaus1( Bplus_M, g1_m[3500,4000], g1_s[50,0,200] ) " );
  //w->factory( "Gaussian::add_gaus2( Bplus_M, g2_m[5200,5400], g2_s[50,0,200] ) " );
  //w->factory( "SUM::mass_pdf( bu_y[0,10e5]*bu_mc_mass_pdf, g1_y[0,10e4]*add_gaus1, g2_y[0,10e4]*add_gaus2 )" );
  //w->factory( "SUM::mass_pdf( bu_y[0,10e5]*bu_mc_mass_pdf, g1_y[0,10e4]*add_gaus1 )" );
}

void Bc2Dmunu::BasicFitter::MassFit() {

  //fit( "bu_mc_mass_pdf", "Bu2Dmunu" );
  plot( "Bplus_M" , "Bu2Dmunu", "bu_mc_mass_pdf", 2);
  //freeze( "bu_mc_mass_pdf" );
  plot( "Bplus_M" , "Bc2Dmunu", "bc_mc_mass_pdf", 2);
  plot( "Bplus_M" , "Bc2Dstmunu", "bc_dst_mc_mass_pdf", 2);

  //fit( "mass_pdf", "Data" );
  //plot( "Bplus_M", "Data", "mass_pdf", 2);
}

void Bc2Dmunu::BasicFitter::RunToys() {

  cout << "HERE" << endl;
  print( " ---------- RUNNING TOYS --------" );

  // estimate Bu suppressed number
  double current_lumi = 25.0;
  double lumi_scale = 3000./current_lumi;

  double pdg_d_kpi_fav = 3.88e-2; // D->Kpi fav BR
  double pdg_d_kpi_sup = 1.38e-4; // D->Kpi sup BR

  double exp_bu_fav = w->data("Data")->numEntries() * lumi_scale;
  double exp_bu_sup = exp_bu_fav * ( pdg_d_kpi_sup / pdg_d_kpi_fav );

  double exp_bc = exp_bu_fav*(0.01)*(0.3)*(1./500.)*(25./35.)*(90./24.); // (Vub/Vcb)^2 * 0.3 ( as Bc goes c->s 70%) * f(Bc/Bu)~1/500 * trigger (25/35) * bdtoutput and mass cut (90/24)

  print( Form("See    %10.2f events in Bu favoured data",exp_bu_fav) );
  print( Form("Expect %10.2f events in Bu suppressed data",exp_bu_sup) );
  print( Form("Expect %10.2f events in Bc signal data", exp_bc) );

  // make joint pdf

  addParameter( "bu_y", exp_bu_sup, 0, 10e6 );
  addParameter( "bc_y", exp_bc    , 0, 10e4 );

  RooAddPdf *pdf = new RooAddPdf( "toy_pdf", "", RooArgList( *w->pdf("bu_mc_mass_pdf"), *w->pdf("bc_mc_mass_pdf") ), RooArgList( *w->var("bu_y"), *w->var("bc_y") ) );
  w->import(*pdf);
  defineParamSet("toy_pdf");
  defineYieldSet("toy_pdf");

  RooDataSet *toy = pdf->generate( RooArgSet(*w->var("Bplus_M")), exp_bu_sup+exp_bc, Extended() );
  toy->SetName("Toy");
  w->import(*toy);

  // plot joint pdf
  PlotComponent pc_toy( "Toy", "Toy" );
  pc_toy.setDefaultDataStyle();

  PlotComponent pc_tot( "toy_pdf", "Total PDF" );
  pc_tot.setSolidLine(kBlue);

  // mainpulate colors
  TColor *myRed = gROOT->GetColor(kRed-7);
  myRed->SetAlpha(0.6);
  TColor *myBlue = gROOT->GetColor(kBlue-7);
  myBlue->SetAlpha(0.6);

  PlotComponent pc_bu( "toy_pdf:bu_mc_mass_pdf", "B^{+} Background" );
  pc_bu.setSolidFill(kRed-7);

  PlotComponent pc_bc( "toy_pdf:bc_mc_mass_pdf", "B_{c}^{+} Signal" );
  pc_bc.setSolidFill(kBlue-7);

  PlotComponent pc_toy_inv( "Toy", "Toy" );
  pc_toy_inv.setDefaultDataStyle();
  pc_toy_inv.noleg = true ;

  vector<PlotComponent> plotComps;
  plotComps.push_back(pc_toy);
  plotComps.push_back(pc_bu);
  plotComps.push_back(pc_bc);
  plotComps.push_back(pc_tot);
  plotComps.push_back(pc_toy_inv);

  //setTitle( "Mass Fit" );
  setDrawLog(true);
  //setResidType(2);

  plot("Bplus_M", plotComps, "toys_pdf");

  TH1F *th1fSig = new TH1F("sig","Significance; Significance; Probability",100,0,30);
  TH1F *nllRat  = new TH1F("nll","Log Likelihood Ratio; Log Likelihood Ratio; Probability", 100, 0, 1000);
  TH1F *bcVal   = new TH1F("bcy","Number of Bc events; Fitted Bc Yield; Probability", 100, 0, 250 );
  TH1F *bcDiff  = new TH1F("bcdiff","Significance; Significance; Probability",100,0,10);
  th1fSig->Sumw2();
  nllRat->Sumw2();
  bcVal->Sumw2();
  bcDiff->Sumw2();
  th1fSig->GetXaxis()->SetTitleOffset(0.8);
  nllRat->GetXaxis()->SetTitleOffset(0.8);
  bcVal->GetXaxis()->SetTitleOffset(0.8);
  bcDiff->GetXaxis()->SetTitleOffset(0.8);
  //th1fSig->GetYaxis()->SetTitleOffset(0.8);
  //nllRat->GetYaxis()->SetTitleOffset(0.8);
  //bcVal->GetYaxis()->SetTitleOffset(0.8);
  th1fSig->GetXaxis()->SetTitleSize(0.07);
  nllRat->GetXaxis()->SetTitleSize(0.07);
  bcVal->GetXaxis()->SetTitleSize(0.07);
  bcDiff->GetXaxis()->SetTitleSize(0.07);
  th1fSig->GetYaxis()->SetTitleSize(0.07);
  nllRat->GetYaxis()->SetTitleSize(0.07);
  bcVal->GetYaxis()->SetTitleSize(0.07);
  bcDiff->GetYaxis()->SetTitleSize(0.07);

  // now really run the toys
  const int nToys = 10000.;
  for (int i=0; i<nToys; i++ ) {
    w->var("bu_y")->setConstant(false);
    w->var("bc_y")->setConstant(false);
    w->var("bu_y")->setVal(exp_bu_sup);
    w->var("bc_y")->setVal(exp_bc);
    RooDataSet *dset = pdf->generate( RooArgSet(*w->var("Bplus_M")), exp_bu_sup+exp_bc, Extended() );

    RooAbsReal *minnll = pdf->createNLL( *dset, Extended() );
    RooMinimizer minim(*minnll);

    // fit s+b
    minim.minimize("Minuit","minimize");
    double minnllSB = minnll->getVal();
    bcVal->Fill( w->var("bc_y")->getVal() );

    // different of bc y from zero
    double diff = w->var("bc_y")->getVal() / w->var("bc_y")->getError();
    bcDiff->Fill(diff);

    // fit b only
    w->var("bc_y")->setVal(1.e-20);
    w->var("bc_y")->setConstant(true);
    minim.minimize("Minuit","minimize");
    double minnllBonly = minnll->getVal();

    double minnllDiff = -2.*( minnllSB - minnllBonly );
    double signif = TMath::Sqrt( minnllDiff ) ;

    cout << Form("%-5d   %10.2f  %10.2f  %6.2f   %5.3f  %10.2f  %10.2f",i,minnllSB, minnllBonly, minnllDiff,signif, w->var("bu_y")->getVal(), w->var("bc_y")->getVal()) << endl;

    th1fSig->Fill( signif );
    nllRat->Fill( minnllDiff );

    delete dset;
    delete minnll;
  }

  th1fSig->Scale(1./th1fSig->Integral());
  nllRat->Scale(1./nllRat->Integral());
  bcVal->Scale(1./bcVal->Integral());
  bcDiff->Scale(1./bcDiff->Integral());

  TF1 *f = new TF1("bcDiffF","gaus",bcDiff->GetBinLowEdge(1),bcDiff->GetBinLowEdge(bcDiff->GetNbinsX()+1));
  bcDiff->Fit(f);

  TCanvas *canv = createCanvas(800,800);
  canv->SetLeftMargin(0.18);
  th1fSig->Draw("HIST");
  canv->Print(Form("plots/%s/pdf/toys.pdf",fitterName.Data()));
  canv->Print(Form("plots/%s/png/toys.png",fitterName.Data()));
  canv->Print(Form("plots/%s/C/toys.C",fitterName.Data()));

  TCanvas *canv2 = createCanvas(800,800);
  canv2->SetLeftMargin(0.18);
  nllRat->Draw("HIST");
  canv2->Print(Form("plots/%s/pdf/toys_nll.pdf",fitterName.Data()));
  canv2->Print(Form("plots/%s/png/toys_nll.png",fitterName.Data()));
  canv2->Print(Form("plots/%s/C/toys_nll.C",fitterName.Data()));

  TCanvas *canv3 = createCanvas(800,800);
  canv3->SetLeftMargin(0.18);
  bcVal->Draw("HIST");
  canv3->Print(Form("plots/%s/pdf/toys_bcy.pdf",fitterName.Data()));
  canv3->Print(Form("plots/%s/png/toys_bcy.png",fitterName.Data()));
  canv3->Print(Form("plots/%s/C/toys_bcy.C",fitterName.Data()));

  TCanvas *canv4 = createCanvas(800,800);
  canv4->SetLeftMargin(0.18);
  bcDiff->Draw("HIST");
  f->Draw("Lsame");
  canv4->Print(Form("plots/%s/pdf/toys_bcdiff.pdf",fitterName.Data()));
  canv4->Print(Form("plots/%s/png/toys_bcdiff.png",fitterName.Data()));
  canv4->Print(Form("plots/%s/C/toys_bcdiff.C",fitterName.Data()));


}

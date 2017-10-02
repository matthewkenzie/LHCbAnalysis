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
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooCategory.h"
#include "RooAbsData.h"
#include "RooDataSet.h"
#include "RooAbsPdf.h"
#include "RooSimultaneous.h"
#include "RooIpatia2.h"
#include "RooAddPdf.h"
#include "RooArgusBG.h"
#include "RooExponential.h"

#include "RooStats/SPlot.h"

#include "boost/lexical_cast.hpp"

#include "MassFitPlotter.h"

using namespace std;
using namespace TMath;
using namespace RooFit;
using namespace Bs2KstKst;

void computeSWeightsFromFit( RooWorkspace *w, TString fit_name ) {

  w->loadSnapshot( "constrained_pdf_fit" );

  RooIpatia2 *bs2kstkst_mc_pdf = new RooIpatia2("bs2kstkst_mc_pdf","bs2kstkst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bs2kstkst_l"),*w->var("bs2kstkst_zeta"),*w->var("bs2kstkst_fb"),*w->var("bs2kstkst_sigma"),*w->var("bs2kstkst_mu"),*w->var("bs2kstkst_a"),*w->var("bs2kstkst_n"),*w->var("bs2kstkst_a2"),*w->var("bs2kstkst_n2"));
  RooIpatia2 *bd2kstkst_mc_pdf = new RooIpatia2("bd2kstkst_mc_pdf","bd2kstkst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2kstkst_l"),*w->var("bd2kstkst_zeta"),*w->var("bd2kstkst_fb"),*w->var("bd2kstkst_sigma"),*w->var("bd2kstkst_mu"),*w->var("bd2kstkst_a"),*w->var("bd2kstkst_n"),*w->var("bd2kstkst_a2"),*w->var("bd2kstkst_n2"));
  RooIpatia2 *bd2rhokst_mc_pdf = new RooIpatia2("bd2rhokst_mc_pdf","bd2rhokst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2rhokst_l"),*w->var("bd2rhokst_zeta"),*w->var("bd2rhokst_fb"),*w->var("bd2rhokst_sigma"),*w->var("bd2rhokst_mu"),*w->var("bd2rhokst_a"),*w->var("bd2rhokst_n"),*w->var("bd2rhokst_a2"),*w->var("bd2rhokst_n2"));
  RooIpatia2 *bd2phikst_mc_pdf = new RooIpatia2("bd2phikst_mc_pdf","bd2phikst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2phikst_l"),*w->var("bd2phikst_zeta"),*w->var("bd2phikst_fb"),*w->var("bd2phikst_sigma"),*w->var("bd2phikst_mu"),*w->var("bd2phikst_a"),*w->var("bd2phikst_n"),*w->var("bd2phikst_a2"),*w->var("bd2phikst_n2"));
  RooIpatia2 *bs2phikst_mc_pdf = new RooIpatia2("bs2phikst_mc_pdf","bs2phikst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bs2phikst_l"),*w->var("bs2phikst_zeta"),*w->var("bs2phikst_fb"),*w->var("bs2phikst_sigma"),*w->var("bs2phikst_mu"),*w->var("bs2phikst_a"),*w->var("bs2phikst_n"),*w->var("bs2phikst_a2"),*w->var("bs2phikst_n2"));
  RooIpatia2 *lb2pkpipi_mc_pdf = new RooIpatia2("lb2pkpipi_mc_pdf","lb2pkpipi_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("lb2pkpipi_l"),*w->var("lb2pkpipi_zeta"),*w->var("lb2pkpipi_fb"),*w->var("lb2pkpipi_sigma"),*w->var("lb2pkpipi_mu"),*w->var("lb2pkpipi_a"),*w->var("lb2pkpipi_n"),*w->var("lb2pkpipi_a2"),*w->var("lb2pkpipi_n2"));
  RooArgusBG *part_reco_pdf = new RooArgusBG( "part_reco_pdf", "part_reco_pdf", *w->var("B_s0_DTF_B_s0_M"), *w->function("part_reco_m0"), *w->var("part_reco_c"), *w->var("part_reco_p") );
  RooExponential *bkg_pdf = new RooExponential( "bkg_pdf", "bkg_pdf", *w->var("B_s0_DTF_B_s0_M"), *w->var("bkg_exp_p1") );

  w->loadSnapshot( "constrained_pdf_fit" );

  RooCategory *cat = (RooCategory*)w->cat("DataCat");
  for ( int c=0; c<cat->numTypes(); c++ ) {

    cat->setIndex(c);

    RooArgList *yields = new RooArgList();
    yields->add( *w->var(Form("bs2kstkst_y_%s",cat->getLabel())) );
    yields->add( *w->var(Form("bd2kstkst_y_%s",cat->getLabel())) );
    yields->add( *w->var(Form("bd2phikst_y_%s",cat->getLabel())) ); // constrained
    yields->add( *w->function(Form("bs2phikst_y_%s",cat->getLabel())) );
    yields->add( *w->function(Form("bd2rhokst_y_%s",cat->getLabel())) );
    yields->add( *w->function(Form("lb2pkpipi_y_%s",cat->getLabel())) );
    yields->add( *w->var(Form("part_reco_y_%s",cat->getLabel())) );
    yields->add( *w->var(Form("bkg_y_%s",cat->getLabel())) );

    RooArgList *pdfs = new RooArgList();
    pdfs->add( *bs2kstkst_mc_pdf );
    pdfs->add( *bd2kstkst_mc_pdf );
    pdfs->add( *bd2phikst_mc_pdf );
    pdfs->add( *bs2phikst_mc_pdf );
    pdfs->add( *bd2rhokst_mc_pdf );
    pdfs->add( *lb2pkpipi_mc_pdf );
    pdfs->add( *part_reco_pdf );
    pdfs->add( *bkg_pdf );

    RooAddPdf *pdf = new RooAddPdf( "pdf","", *pdfs, *yields );

    RooArgSet obsAndCats( *w->set("observables"), *w->set("categories") );
    RooArgSet *nonyields = pdf->getParameters( RooArgSet( *yields, obsAndCats ) );
    yields->remove( *w->function(Form("bs2phikst_y_%s",cat->getLabel())) );
    yields->remove( *w->function(Form("bd2rhokst_y_%s",cat->getLabel())) );
    yields->remove( *w->function(Form("lb2pkpipi_y_%s",cat->getLabel())) );

    TString dsetName;
    if ( cat->getLabel() == TString("HadronTOS2011") ) dsetName = "Data2011HadronTOS";
    if ( cat->getLabel() == TString("GlobalTIS2011") ) dsetName = "Data2011GlobalTIS";
    if ( cat->getLabel() == TString("HadronTOS2012") ) dsetName = "Data2012HadronTOS";
    if ( cat->getLabel() == TString("GlobalTIS2012") ) dsetName = "Data2012GlobalTIS";

    RooDataSet *oldData = (RooDataSet*)w->data(dsetName);
    RooDataSet *data = new RooDataSet( Form("n%s",dsetName.Data()), oldData->GetTitle(), oldData, RooArgSet( *w->var("B_s0_DTF_B_s0_M"), *w->var("eventNumber")) );
    data->Print("v");
    pdf->Print("v");

    nonyields->setAttribAll("Constant");
    yields->setAttribAll("Constant");
    //yields->setAttribAll("Constant");
    //pdf->fitTo( *data, Extended() );

    RooStats::SPlot *sData = new RooStats::SPlot(Form("s%s",dsetName.Data()),"sData", *data, pdf, *yields, RooArgSet(*w->var("eventNumber")) );

    cout << "Yields: " << endl;
    cout << "bs2kstkst: " << w->var(Form("bs2kstkst_y_%s",cat->getLabel()))->getVal() << " " << sData->GetYieldFromSWeight(Form("bs2kstkst_y_%s",cat->getLabel())) << endl;
    cout << "bd2kstkst: " << w->var(Form("bd2kstkst_y_%s",cat->getLabel()))->getVal() << " " << sData->GetYieldFromSWeight(Form("bd2kstkst_y_%s",cat->getLabel())) << endl;
    cout << "bd2phikst: " << w->var(Form("bd2phikst_y_%s",cat->getLabel()))->getVal() << " " << sData->GetYieldFromSWeight(Form("bd2phikst_y_%s",cat->getLabel())) << endl;
    cout << "part_reco: " << w->var(Form("part_reco_y_%s",cat->getLabel()))->getVal() << " " << sData->GetYieldFromSWeight(Form("part_reco_y_%s",cat->getLabel())) << endl;
    cout << "bkg: " << w->var(Form("bkg_y_%s",cat->getLabel()))->getVal() << " " << sData->GetYieldFromSWeight(Form("bkg_y_%s",cat->getLabel())) << endl;

    RooDataSet *dataSW = new RooDataSet(Form("%s_swN",data->GetName()), Form("%s_sw",data->GetTitle()), data, RooArgSet( *w->var("B_s0_DTF_B_s0_M"), *w->var("eventNumber"), *w->var(Form("bs2kstkst_y_%s_sw",cat->getLabel()))), 0, Form("bs2kstkst_y_%s_sw",cat->getLabel()) );

    RooDataSet *dataInvolved = (RooDataSet*)w->data(Form("%s_sw",dsetName.Data()));

    for ( int i=0; i<10; i++ ) {
      RooArgSet *obs = (RooArgSet*)dataSW->get(i);

      double      mass          = obs->getRealValue("B_s0_DTF_B_s0_M");
      ULong64_t   eventNumber   = boost::lexical_cast<ULong64_t>( obs->getRealValue("eventNumber") );
      double      weight        = dataSW->weight();

      RooArgSet *oObs = (RooArgSet*)dataInvolved->get(i);

      double      omass          = oObs->getRealValue("B_s0_DTF_B_s0_M");
      ULong64_t   oeventNumber   = boost::lexical_cast<ULong64_t>( oObs->getRealValue("eventNumber") );
      double      oweight        = dataInvolved->weight();


      cout << i << " " << eventNumber << " " << mass << " " << weight << " " << sData->GetSWeight(i, Form("bs2kstkst_y_%s",cat->getLabel()) ) << endl;
      cout << i << " " << oeventNumber << " " << omass << " " << oweight << " " << endl;
    }
  }
}

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

  computeSWeightsFromFit( w, "constrained_pdf_fit");

  return 0;

  // throw a few toys for practise
  int nToys = 4;
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

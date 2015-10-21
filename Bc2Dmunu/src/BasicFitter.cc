#include "BasicFitter.h"
#include "RooCBShape.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"

using namespace std;
using namespace RooFit;

Bc2Dmunu::BasicFitter::BasicFitter(TString wsname,TString name, bool verbose, bool debug):
  FitterBase(wsname,name,verbose,debug)
{}

Bc2Dmunu::BasicFitter::~BasicFitter(){}

void Bc2Dmunu::BasicFitter::addObsVars(){

  addObsVar("Bplus_Mcorr",            "m_{corr} (B^{+})", "MeV", 3500.,8000.);
}

void Bc2Dmunu::BasicFitter::addCuts(){

  //addCut("bdtoutput",float(0.),float(1.));
  //addCut("min_kaon_ProbNNk",double(0.5),double(1.));
  //addCut("min_pion_ProbNNpiKp",double(0.),double(1.));

}

void Bc2Dmunu::BasicFitter::addDatasets(){

  // Data
  addDataset("Data",                "Data",                80 );

  // MC
  addDataset("Bc2Dmunu",           "Bc2Dmunu",           -80);
  addDataset("Bc2Dstmunu",         "Bc2Dstmunu",         -81);
  addDataset("Bu2Dmunu",           "Bu2Dmunu",           -82);
}

void Bc2Dmunu::BasicFitter::constructPdfs(){
  makeBc2DmunuPdf();
  makeBc2DstmunuPdf();
  makeBu2DmunuPdf();
  makeBu2DstmunuPdf();
  makeCombinatorialPdf();
  makeTotalPdf();
}

void Bc2Dmunu::BasicFitter::run(){

  // do the MC part
  Bu2DmunuFit();
  DataFit();

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
  plot("Bplus_Mcorr", plotComps, "fit");

}

void Bc2Dmunu::BasicFitter::makeBu2DmunuPdf() {

  w->factory( "CBShape::bu2dmunu_mc_cb1( Bplus_Mcorr, m1[4000.,8000.], s1[0.,500.], alpha1[0.5,0.,10.], n1[0.01,0.,10.] )" );
  w->factory( "CBShape::bu2dmunu_mc_cb2( Bplus_Mcorr, m2[4000.,8000.], s2[0.,500.], alpha2[-0.5,-10.,0.], n2[0.01,0.,10.] )" );
  w->factory( "SUM::bu2dmunu_mc_pdf( f1[0.6,0.,1.]*bu2dmunu_mc_cb1, bu2dmunu_mc_cb2 )" );
  defineParamSet("bu2dmunu_mc_pdf");
}

void Bc2Dmunu::BasicFitter::makeBu2DstmunuPdf() {

  w->factory( "CBShape::bu2dstmunu_mc_cb1( Bplus_Mcorr, dst_m1[4000.,8000.], s1, alpha1, n1 )" );
  w->factory( "CBShape::bu2dstmunu_mc_cb2( Bplus_Mcorr, dst_m2[4000.,8000.], s2, alpha2, n2 )" );
  w->factory( "SUM::bu2dstmunu_mc_pdf( dst_f1[0.6,0.,1.]*bu2dstmunu_mc_cb1, bu2dstmunu_mc_cb2 )" );
  defineParamSet("bu2dmunu_mc_pdf");
}

void Bc2Dmunu::BasicFitter::makeBc2DmunuPdf() {
  return;
}

void Bc2Dmunu::BasicFitter::makeBc2DstmunuPdf() {
  return;
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
  freeze("bu2dmunu_mc_pdf");
}

void Bc2Dmunu::BasicFitter::DataFit() {
  fit("pdf", "Data");
  plot("Bplus_Mcorr", "Data", "pdf",2);
}

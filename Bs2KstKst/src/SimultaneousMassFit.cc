//
// ratio of branching fractions:
//  B (B_s -> phi Kst) / B (B0 -> phi Kst) = 0.113 +- 0.024 (stat) +- 0.013 (syst) +- 0.009 (fd/fs)
//
// in quadrature = 0.113 +- 0.0287

#include "SimultaneousMassFit.h"
#include "RooExpAndGauss.h"
#include "RooPhysBkg.h"
#include "RooIpatia2.h"
#include "RooCBShape.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooSimultaneous.h"
#include "RooStats/SPlot.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooCategory.h"

using namespace std;
using namespace RooFit;

Bs2KstKst::SimultaneousMassFit::SimultaneousMassFit(TString wsname,TString name, bool verbose, bool debug):
  FitterBase(wsname,name,verbose,debug)
{}

Bs2KstKst::SimultaneousMassFit::~SimultaneousMassFit(){}

void Bs2KstKst::SimultaneousMassFit::addObsVars(){

  addObsVar("B_s0_DTF_B_s0_M",            "m(K^{+}#pi^{-}K^{-}#pi^{+})", "MeV",  5000.,5800.);
  //addObsVar("B_s0_DTF_B_s0_MERR",         "m(K^{+}#pi^{-}K^{-}#pi^{+})", "MeV",  0.,100.);
  addObsVar("B_s0_DTF_KST1_M",            "m(K^{-}#pi^{+})",             "MeV",  750., 1700.);
  addObsVar("B_s0_DTF_KST2_M",            "m(K^{-}#pi^{+})",             "MeV",  750., 1700.);
  addObsVar("B_s0_DTF_B_s0_CosTheta1",    "cos(#theta_{1})",             "",    -1., 1.);
  addObsVar("B_s0_DTF_B_s0_CosTheta2",    "cos(#theta_{2})",             "",    -1., 1.);
  addObsVar("B_s0_DTF_B_s0_Phi1",         "#Phi",                        "rad", -3.14,3.14);
  addObsVar("B_s0_DTF_TAU",               "t",                           "fs",   0., 50.);
  addObsVar("B_s0_DTF_TAUERR",            "#sigma_{t}",                  "fs",   0., 0.5);
  addObsVar("B_s0_TAGDECISION_OS",        "TAG OS DEC",                  "",    -5, 5); // must pass right type (int)
  addObsVar("B_s0_TAGOMEGA_OS",           "TAG OS OMEGA",                "",     0., 1.);
  addObsVar("B_s0_SS_nnetKaon_DEC",       "TAG SS Kaon DEC",             "",     short(-5),short(5)); // must pass right type (int)
  addObsVar("B_s0_SS_nnetKaon_PROB",      "TAG SS Kaon PROB",            "",     float(0.),float(1.));
  //addObsVar("B_s0_L0Global_TIS",          "L0 Global TIS",               "",     false );
  //addObsVar("B_s0_L0Global_TOS",          "L0 Global TOS",               "",     false );
  //addObsVar("B_s0_L0HadronDecision_TIS",  "L0 Hadron TIS",               "",     false );
  //addObsVar("B_s0_L0HadronDecision_TOS",  "L0 Hadron TOS",               "",     false );
}

void Bs2KstKst::SimultaneousMassFit::addCuts(){

  //addCut("bdtoutput",float(0.),float(1.));
  //addCut("min_kaon_ProbNNk",double(0.5),double(1.));
  //addCut("min_pion_ProbNNpiKp",double(0.),double(1.));

}

void Bs2KstKst::SimultaneousMassFit::addDatasets(){

  // Data
  addDataset("Data",                "Data",                    71, 81);
  addRequirement("Data",       "pass_bdt", true);
  addRequirement("Data",       "pass_pid", true);

  // These are the simultaneous dsets
  addDataset("Data2011HadronTOS",   "Data (2011) Hadron TOS",  71);
  addDataset("Data2011GlobalTIS",   "Data (2011) Global TIS",  71);
  addDataset("Data2012HadronTOS",   "Data (2012) Hadron TOS",  81);
  addDataset("Data2012GlobalTIS",   "Data (2012) Global TIS",  81);

  // 2011 && L0_Hadron_TOS
  addRequirement("Data2011HadronTOS",   "pass_bdt", true);
  addRequirement("Data2011HadronTOS",   "pass_pid", true);
  addRequirement("Data2011HadronTOS",   "B_s0_L0HadronDecision_TOS", true  );

  // 2011 && L0_Global_TIS && !L0_Hadron_TOS
  addRequirement("Data2011GlobalTIS",   "pass_bdt", true);
  addRequirement("Data2011GlobalTIS",   "pass_pid", true);
  addRequirement("Data2011GlobalTIS",   "B_s0_L0Global_TIS",         true );
  addRequirement("Data2011GlobalTIS",   "B_s0_L0HadronDecision_TOS", false  );

  // 2012 && L0_Hadron_TOS
  addRequirement("Data2012HadronTOS",   "pass_bdt", true);
  addRequirement("Data2012HadronTOS",   "pass_pid", true);
  addRequirement("Data2012HadronTOS",   "B_s0_L0HadronDecision_TOS", true  );

  // 2012 && && L0_Global_TIS !L0_Hadron_TOS
  addRequirement("Data2012GlobalTIS",   "pass_bdt", true);
  addRequirement("Data2012GlobalTIS",   "pass_pid", true);
  addRequirement("Data2012GlobalTIS",   "B_s0_L0Global_TIS",         true );
  addRequirement("Data2012GlobalTIS",   "B_s0_L0HadronDecision_TOS", false  );

  // the combined dataset
  addCategory( "DataCat", "2011HadronTOS", "2011GlobalTIS", "2012HadronTOS", "2012GlobalTIS" );
  map<TString,TString> catDataMap;
  catDataMap["2011HadronTOS"] = "Data2011HadronTOS";
  catDataMap["2011GlobalTIS"] = "Data2011GlobalTIS";
  catDataMap["2012HadronTOS"] = "Data2012HadronTOS";
  catDataMap["2012GlobalTIS"] = "Data2012GlobalTIS";
  addCombDataset("DataCombined", "Data Combined", "DataCat", catDataMap);

  //addRequirement("Data2011", "bdtoutput",float(-0.04),float(1.));
  //addRequirement("Data2011", "min_kaon_ProbNNk",double(0.52),double(1.));
  //addRequirement("Data2011", "min_pion_ProbNNpiKp",double(0.1),double(1.));
  //addRequirement("Data2011", "B_s0_MMERRoMM",double(0.),double(0.0035));
  //addRequirement("Data2012", "bdtoutput",float(-0.04),float(1.));
  //addRequirement("Data2012", "min_kaon_ProbNNk",double(0.52),double(1.));
  //addRequirement("Data2012", "min_pion_ProbNNpiKp",double(0.1),double(1.));
  //addRequirement("Data2012", "B_s0_MMERRoMM",double(0.),double(0.0035));
  //addRequirement("Data",     "bdtoutput",float(-0.04),float(1.));
  //addRequirement("Data",     "min_kaon_ProbNNk",double(0.52),double(1.));
  //addRequirement("Data",     "min_pion_ProbNNpiKp",double(0.1),double(1.));
  //addRequirement("Data",     "B_s0_MMERRoMM",double(0.),double(0.0035));

  // MC
  addDataset("Bs2KstKst",           "Bs2KstKst",           -70, -80);
  addDataset("Bs2KstKst1430",       "Bs2KstKst1430",       -71, -81);
  addDataset("Bs2Kst1430Kst1430",   "Bs2Kst1430Kst1430",   -72, -82);
  addDataset("Bs2KpiKpiPhaseSpace", "Bs2KpiKpiPhaseSpace", -73, -83);
  addDataset("Bd2KstKst",           "Bd2Kst0Kst0",         -74, -84);
  addDataset("Bd2PhiKst",           "Bd2PhiKst0",          -75, -85);
  addDataset("Bs2PhiKst",           "Bs2PhiKst0",          -76, -86);
  addDataset("Bd2RhoKst",           "Bd2RhoKst0",          -77, -87);
  addDataset("Lb2pKpipi",           "Lb2pKpipi",           -78, -88);
  addDataset("Lb2ppipipi",          "Lb2ppipipi",          -79, -89);

}

void Bs2KstKst::SimultaneousMassFit::constructPdfs(){
  makeBs2KstKstPdf();
  makeBd2KstKstPdf();
  makeBd2PhiKstPdf();
  makeBs2PhiKstPdf();
  makeBd2RhoKstPdf();
  makeLb2pKpipiPdf();
  makeLb2ppipipiPdf();
  makePartRecoPdf();
  makeCombinatorialPdf();
  makeTotalPdf();
}

void Bs2KstKst::SimultaneousMassFit::run(){

  // do the MC part
  Bs2KstKstFit();
  Bd2KstKstFit();
  Bd2PhiKstFit();
  Bs2PhiKstFit();
  Bd2RhoKstFit();
  Lb2pKpipiFit();
  Lb2ppipipiFit();

  DataFit();

  // Splot stuff
  //RooDataSet *data = (RooDataSet*)w->data("Data");
  //RooAbsPdf *pdf = w->pdf("constrained_pdf");

  //w->loadSnapshot("constrained_pdf_fit");
  //RooArgList *syields = new RooArgList();
  //syields->add( *w->var("bs2kstkst_y") );

  //RooStats::SPlot *sData = new RooStats::SPlot("Data_sfit", "Data_sfit", *data, pdf, *syields);
  //w->import(*sData);
  //w->import(*data,Rename("Data_wsweights"));

  //RooDataSet *swdata = new RooDataSet("Data_wsweights_proj_bs2kstkst_y","Data sweight proj bs2kstkst_y", data, *data->get(), 0, "bs2kstkst_y_sw");
  //w->import(*swdata);

  //RooArgSet *syields = new RooArgSet();
  //syields->add(*w->var("bkg_y"       ));
  //syields->add(*w->var("part_reco_y" ));
  //syields->add(*w->var("bs2kstkst_y"  ));
  //syields->add(*w->var("bd2kstkst_y" ));
  //syields->add(*w->var("bd2phikst_y" ));
  ////syields->add(*w->function("bs2phikst_y" ));
  //syields->add(*w->var("bd2rhokst_y" ));
  //syields->add(*w->var("lb2pkpipi_y" ));
  //syields->add(*w->var("lb2ppipipi_y"));

  //RooArgSet *nonsyields = w->pdf("constrained_pdf")->getParameters( RooArgSet( *w->set("observables"), *syields) );

  //w->defineSet("syields", *syields);
  //w->defineSet("nonsyields", *nonsyields);

  //sfit("constrained_pdf","Data","syields","nonsyields");

  //sfit("pdf","Data");

  if ( w->pdf("constrained_pdf") ) {
    sfit("constrained_pdf","Data");
  }
  else {
    sfit("pdf","Data");
  }
  sproject("Data",   "bs2kstkst_y");

}

void Bs2KstKst::SimultaneousMassFit::makePlots(){
  //makeInitialFitPlots();
  return;
  makeDataPlot();
  makeSolidDataPlot();
  makeSWeightPlots();
}

void Bs2KstKst::SimultaneousMassFit::makeInitialFitPlots() {
  plot("B_s0_DTF_B_s0_M","Bd2KstKst","bd2kstkst_mc_pdf",2);
  plot("B_s0_DTF_B_s0_M","Bs2KstKst","bs2kstkst_mc_pdf",2);
  plot("B_s0_DTF_B_s0_M","Bd2PhiKst","bd2phikst_mc_pdf",2);
  plot("B_s0_DTF_B_s0_M","Bs2PhiKst","bs2phikst_mc_pdf",2);
  plot("B_s0_DTF_B_s0_M","Bd2RhoKst","bd2rhokst_mc_pdf",2);
  plot("B_s0_DTF_B_s0_M","Lb2pKpipi","lb2pkpipi_mc_pdf",2);
  plot("B_s0_DTF_B_s0_M","Lb2ppipipi","lb2ppipipi_mc_pdf",2);
  plot("B_s0_DTF_B_s0_M","Lb2ppipipi","lb2ppipipi_mc_pdf",2);
  plot("B_s0_DTF_B_s0_M","Data","constrained_pdf",2);
}

void Bs2KstKst::SimultaneousMassFit::makeDataPlot(){

  if ( verbose || debug ) {
    cout << "Making data plot" << endl;
  }

  vector<PlotComponent> plotComps;

  PlotComponent pc_data( "Data", "Data" );
  pc_data.setDefaultDataStyle();

  PlotComponent pc_data_invis( "Data", "Data Invis" );
  pc_data_invis.noleg = true;

  PlotComponent pc_pdf( "pdf", "Total PDF" );
  pc_pdf.setSolidLine(kBlue);

  PlotComponent pc_pdf_bkg( "pdf:bkg_pdf", "Background (Exp)" );
  pc_pdf_bkg.setDashedLine(kGreen+1);

  PlotComponent pc_pdf_part_reco( "pdf:part_reco_pdf", "Partially reco" );
  pc_pdf_part_reco.setDashedLine(kGreen+7);

  PlotComponent pc_pdf_sig( "pdf:bs2kstkst_mc_pdf", "B_{s} #rightarrow (K^{+}#pi^{-})(K^{-}#pi^{+})" );
  pc_pdf_sig.setDashedLine(kRed);

  PlotComponent pc_pdf_sig_bd( "pdf:bd2kstkst_mc_pdf", "B_{d} #rightarrow (K^{+}#pi^{-})(K^{-}#pi^{+})" );
  pc_pdf_sig_bd.setDashedLine(kMagenta);

  PlotComponent pc_pdf_sig_phikst( "pdf:bd2phikst_mc_pdf", "B_{d} #rightarrow (K^{+}K^{-})(K^{-}#pi^{+})");
  pc_pdf_sig_phikst.setDashedLine(kYellow+1);

  PlotComponent pc_pdf_sig_bsphikst( "pdf:bs2phikst_mc_pdf", "B_{s} #rightarrow (K^{+}K^{-})(K^{-}#pi^{+})");
  pc_pdf_sig_bsphikst.setDashedLine(kBlack);

  PlotComponent pc_pdf_sig_rhokst( "pdf:bd2rhokst_mc_pdf", "B_{d}#rightarrow (#pi^{+}#pi^{-})(K^{-}#pi^{+})");
  pc_pdf_sig_rhokst.setDashedLine(kOrange+1);

  PlotComponent pc_pdf_sig_pkpipi( "pdf:lb2pkpipi_mc_pdf", "#Lambda_{b}#rightarrow (p^{+}#pi^{-})(K^{-}#pi^{+})");
  pc_pdf_sig_pkpipi.setDashedLine(kViolet+1);

  PlotComponent pc_pdf_sig_ppipipi( "pdf:lb2ppipipi_mc_pdf", "#Lambda_{b}#rightarrow (p^{+}#pi^{-})(#pi^{-}#pi^{+})");
  pc_pdf_sig_ppipipi.setDashedLine(kBlue-7);

  plotComps.push_back(pc_data);
  plotComps.push_back(pc_pdf_bkg);
  plotComps.push_back(pc_pdf_part_reco);
  plotComps.push_back(pc_pdf_sig_bd);
  plotComps.push_back(pc_pdf_sig_phikst);
  plotComps.push_back(pc_pdf_sig_bsphikst);
  plotComps.push_back(pc_pdf_sig_rhokst);
  plotComps.push_back(pc_pdf_sig_pkpipi);
  plotComps.push_back(pc_pdf_sig_ppipipi);
  plotComps.push_back(pc_pdf_sig);
  plotComps.push_back(pc_pdf);
  plotComps.push_back(pc_data_invis);

  // set up yield list
  w->var("bkg_y")->SetTitle("N_{comb.}");
  w->var("part_reco_y")->SetTitle("N_{part. rec.}");
  w->var("bs2kstkst_y")->SetTitle("N_{B_{s}#rightarrow(K#pi)(K#pi)}");
  w->var("bd2kstkst_y")->SetTitle("N_{B_{d}#rightarrow(K#pi)(K#pi)}");
  w->var("bd2rhokst_y")->SetTitle("N_{B_{d}#rightarrow(K#pi)(#pi#pi)}");
  w->var("bd2phikst_y")->SetTitle("N_{B_{d}#rightarrow(K#pi)(KK)}");
  w->function("bs2phikst_y")->SetTitle("N_{B_{s}#rightarrow(K#pi)(KK)}");
  w->var("lb2pkpipi_y")->SetTitle("N_{#Lambda_{b}#rightarrow(p#pi)(K#pi)}");
  w->var("lb2ppipipi_y")->SetTitle("N_{#Lambda_{b}#rightarrow(p#pi)(#pi#pi)}");

  RooArgList *ordered_params = new RooArgList();
  ordered_params->add( *w->var("bkg_y") );
  ordered_params->add( *w->var("part_reco_y") );
  ordered_params->add( *w->var("bd2kstkst_y") );
  ordered_params->add( *w->var("bd2rhokst_y") );
  ordered_params->add( *w->var("bd2phikst_y") );
  ordered_params->add( *w->function("bs2phikst_y") );
  ordered_params->add( *w->var("lb2pkpipi_y") );
  ordered_params->add( *w->var("lb2ppipipi_y") );
  ordered_params->add( *w->var("bs2kstkst_y") );

  setTitle("SimultaneousMassFit");
  setDrawLog(true);
  setResidType(2);
  setPBoxX(0.23);

  plot("B_s0_DTF_B_s0_M", plotComps, "fullfit", ordered_params);

}

void Bs2KstKst::SimultaneousMassFit::makeSolidDataPlot(){

  if ( verbose || debug ) {
    cout << "Making alternate data plot" << endl;
  }

  vector<PlotComponent> plotComps;

  PlotComponent pc_data( "Data", "Data" );
  pc_data.setDefaultDataStyle();

  PlotComponent pc_data_invis( "Data", "Data Invis" );
  pc_data_invis.noleg = true;

  PlotComponent pc_pdf( "pdf", "Total PDF" );
  pc_pdf.setSolidLine(kBlue+1);
  //pc_pdf.invisible = true;

  PlotComponent pc_pdf_bkg( "pdf:bkg_pdf", "Background (Exp)" );
  pc_pdf_bkg.setSolidFill(kGreen+1);

  PlotComponent pc_pdf_part_reco( "pdf:bkg_pdf,part_reco_pdf", "Partially reco" );
  pc_pdf_part_reco.setSolidFill(kTeal-7);

  PlotComponent pc_pdf_sig( "pdf:bkg_pdf,part_reco_pdf,lb2pkpipi_mc_pdf,lb2ppipipi_mc_pdf,bd2rhokst_mc_pdf,bs2phikst_mc_pdf,bd2phikst_mc_pdf,bd2kstkst_mc_pdf,bs2kstkst_mc_pdf", "B_{s} #rightarrow (K^{+}#pi^{-})(K^{-}#pi^{+})" );
  pc_pdf_sig.setSolidFill(kRed-3);

  PlotComponent pc_pdf_sig_bd( "pdf:bkg_pdf,part_reco_pdf,lb2pkpipi_mc_pdf,lb2ppipipi_mc_pdf,bd2rhokst_mc_pdf,bs2phikst_mc_pdf,bd2phikst_mc_pdf,bd2kstkst_mc_pdf", "B_{d} #rightarrow (K^{+}#pi^{-})(K^{-}#pi^{+})" );
  pc_pdf_sig_bd.setSolidFill(kMagenta-3);

  PlotComponent pc_pdf_sig_phikst( "pdf:bkg_pdf,part_reco_pdf,lb2pkpipi_mc_pdf,lb2ppipipi_mc_pdf,bd2rhokst_mc_pdf,bs2phikst_mc_pdf,bd2phikst_mc_pdf", "B_{d} #rightarrow (K^{+}K^{-})(K^{-}#pi^{+})");
  pc_pdf_sig_phikst.setSolidFill(kCyan-3);

  PlotComponent pc_pdf_sig_bsphikst( "pdf:bkg_pdf,part_reco_pdf,lb2pkpipi_mc_pdf,lb2ppipipi_mc_pdf,bs2phikst_mc_pdf", "B_{s} #rightarrow (K^{+}K^{-})(K^{-}#pi^{+})");
  pc_pdf_sig_bsphikst.setSolidFill(kGray+1);

  PlotComponent pc_pdf_sig_rhokst( "pdf:bkg_pdf,part_reco_pdf,lb2pkpipi_mc_pdf,lb2ppipipi_mc_pdf,bs2phikst_mc_pdf,bd2rhokst_mc_pdf", "B_{d}#rightarrow (#pi^{+}#pi^{-})(K^{-}#pi^{+})");
  pc_pdf_sig_rhokst.setSolidFill(kOrange-3);

  PlotComponent pc_pdf_sig_pkpipi( "pdf:bkg_pdf,part_reco_pdf,lb2pkpipi_mc_pdf", "#Lambda_{b}#rightarrow (p^{+}#pi^{-})(K^{-}#pi^{+})");
  pc_pdf_sig_pkpipi.setSolidFill(kViolet+1);

  PlotComponent pc_pdf_sig_ppipipi( "pdf:bkg_pdf,part_reco_pdf,lb2pkpipi_mc_pdf,lb2ppipipi_mc_pdf", "#Lambda_{b}#rightarrow (p^{+}#pi^{-})(#pi^{-}#pi^{+})");
  pc_pdf_sig_ppipipi.setSolidFill(kBlue-7);

  plotComps.push_back(pc_data);
  plotComps.push_back(pc_pdf_sig);
  plotComps.push_back(pc_pdf_sig_bd);
  plotComps.push_back(pc_pdf_sig_phikst);
  plotComps.push_back(pc_pdf_sig_rhokst);
  plotComps.push_back(pc_pdf_sig_bsphikst);
  plotComps.push_back(pc_pdf_sig_pkpipi);
  plotComps.push_back(pc_pdf_sig_ppipipi);
  plotComps.push_back(pc_pdf_part_reco);
  plotComps.push_back(pc_pdf_bkg);
  plotComps.push_back(pc_pdf);
  plotComps.push_back(pc_data_invis);

  // set up yield list
  w->var("bkg_y")->SetTitle("N_{comb.}");
  w->var("part_reco_y")->SetTitle("N_{part. rec.}");
  w->var("bs2kstkst_y")->SetTitle("N_{B_{s}#rightarrow(K#pi)(K#pi)}");
  w->var("bd2kstkst_y")->SetTitle("N_{B_{d}#rightarrow(K#pi)(K#pi)}");
  w->var("bd2rhokst_y")->SetTitle("N_{B_{d}#rightarrow(K#pi)(#pi#pi)}");
  w->var("bd2phikst_y")->SetTitle("N_{B_{d}#rightarrow(K#pi)(KK)}");
  w->function("bs2phikst_y")->SetTitle("N_{B_{s}#rightarrow(K#pi)(KK)}");
  w->var("lb2pkpipi_y")->SetTitle("N_{#Lambda_{b}#rightarrow(p#pi)(K#pi)}");
  w->var("lb2ppipipi_y")->SetTitle("N_{#Lambda_{b}#rightarrow(p#pi)(#pi#pi)}");

  RooArgList *ordered_params = new RooArgList();
  ordered_params->add( *w->var("bkg_y") );
  ordered_params->add( *w->var("part_reco_y") );
  ordered_params->add( *w->var("bd2kstkst_y") );
  ordered_params->add( *w->var("bd2rhokst_y") );
  ordered_params->add( *w->var("bd2phikst_y") );
  ordered_params->add( *w->function("bs2phikst_y") );
  ordered_params->add( *w->var("lb2pkpipi_y") );
  ordered_params->add( *w->var("lb2ppipipi_y") );
  ordered_params->add( *w->var("bs2kstkst_y") );

  setTitle("SimultaneousMassFit");
  setDrawLog(false);
  setResidType(2);
  setPBoxX(0.23);

  plot("B_s0_DTF_B_s0_M", plotComps, "fullfit_solid", ordered_params);

}

void Bs2KstKst::SimultaneousMassFit::makeSWeightPlots() {

  // set up plot stuff
  w->var("B_s0_DTF_KST1_M")->setBins(40);
  w->var("B_s0_DTF_KST2_M")->setBins(40);
  w->var("B_s0_DTF_B_s0_CosTheta1")->setBins(40);
  w->var("B_s0_DTF_B_s0_CosTheta2")->setBins(40);
  w->var("B_s0_DTF_B_s0_Phi1")->setBins(40);
  w->var("B_s0_DTF_TAU")->setRange(0.,10.);
  w->var("B_s0_DTF_TAU")->setBins(40);
  w->var("B_s0_DTF_TAUERR")->setRange(0.,0.08);
  w->var("B_s0_DTF_TAUERR")->setBins(40);
  w->var("B_s0_SS_nnetKaon_DEC")->setRange(-1.5,1.5);
  w->var("B_s0_SS_nnetKaon_DEC")->setBins(3);
  w->var("B_s0_SS_nnetKaon_PROB")->setRange(0.,0.5);
  w->var("B_s0_SS_nnetKaon_PROB")->setBins(40);
  w->var("B_s0_TAGDECISION_OS")->setRange(-1.5,1.5);
  w->var("B_s0_TAGDECISION_OS")->setBins(3);
  w->var("B_s0_TAGOMEGA_OS")->setRange(0.,0.5);
  w->var("B_s0_TAGOMEGA_OS")->setBins(40);

  plot( "B_s0_DTF_B_s0_CosTheta1", "Data_wsweights_proj_bs2kstkst_y" , "", 0, "", false);
  plot( "B_s0_DTF_B_s0_CosTheta2", "Data_wsweights_proj_bs2kstkst_y" , "", 0, "", false);
  plot( "B_s0_DTF_B_s0_Phi1",      "Data_wsweights_proj_bs2kstkst_y" , "", 0, "", false);
  plot( "B_s0_DTF_KST1_M",         "Data_wsweights_proj_bs2kstkst_y" , "", 0, "", false);
  plot( "B_s0_DTF_KST2_M",         "Data_wsweights_proj_bs2kstkst_y" , "", 0, "", false);

  plotMultiCanv( 5, "sweighted_proj_1" );

  plot( "B_s0_DTF_TAU",            "Data_wsweights_proj_bs2kstkst_y" , "", 0, "", false);
  plot( "B_s0_TAGDECISION_OS",     "Data_wsweights_proj_bs2kstkst_y" , "", 0, "", false);
  plot( "B_s0_SS_nnetKaon_DEC",    "Data_wsweights_proj_bs2kstkst_y" , "", 0, "", false);
  plot( "B_s0_DTF_TAUERR",         "Data_wsweights_proj_bs2kstkst_y" , "", 0, "", false);
  plot( "B_s0_TAGOMEGA_OS",        "Data_wsweights_proj_bs2kstkst_y" , "", 0, "", false);
  plot( "B_s0_SS_nnetKaon_PROB",   "Data_wsweights_proj_bs2kstkst_y" , "", 0, "", false);

  plotMultiCanv( 6, "sweighted_proj_2" );

  plot2D( "B_s0_DTF_KST1_M", "B_s0_DTF_KST2_M", "Data_wsweights_proj_bs2kstkst_y" );
  plot2D( "B_s0_DTF_KST1_M", "B_s0_DTF_KST2_M", "Data" );
  plot2D( "B_s0_DTF_KST1_M", "B_s0_DTF_KST2_M", "Bs2KstKst"           );
  plot2D( "B_s0_DTF_KST1_M", "B_s0_DTF_KST2_M", "Bs2KstKst1430"       );
  plot2D( "B_s0_DTF_KST1_M", "B_s0_DTF_KST2_M", "Bs2Kst1430Kst1430"   );
  plot2D( "B_s0_DTF_KST1_M", "B_s0_DTF_KST2_M", "Bs2KpiKpiPhaseSpace" );
}

void Bs2KstKst::SimultaneousMassFit::Bs2KstKstFit() {
  fit("bs2kstkst_mc_pdf", "Bs2KstKst");
  plot("B_s0_DTF_B_s0_M","Bs2KstKst","bs2kstkst_mc_pdf",2);
  freeze("bs2kstkst_mc_pdf");
  w->var("bs2kstkst_mu")->setConstant(false);
  w->var("bs2kstkst_sigma")->setConstant(false);
  w->var("bs2kstkst_zeta")->setConstant(true);
  w->var("bs2kstkst_fb")->setConstant(true);
  //addConstraint("bs2kstkst_l");
  //addConstraint("bs2kstkst_n");
  //addConstraint("bs2kstkst_a");
  //addConstraint("bs2kstkst_n2");
  //addConstraint("bs2kstkst_a2");
}

void Bs2KstKst::SimultaneousMassFit::Bd2KstKstFit() {
  fit("bd2kstkst_mc_pdf", "Bd2KstKst");
  plot("B_s0_DTF_B_s0_M","Bd2KstKst","bd2kstkst_mc_pdf",2);
  freeze("bd2kstkst_mc_pdf");
  w->var("bd2kstkst_mu")->setConstant(false);
  w->var("bd2kstkst_sigma")->setConstant(false);
  w->var("bd2kstkst_zeta")->setConstant(true);
  w->var("bd2kstkst_fb")->setConstant(true);
  //addConstraint("bd2kstkst_l");
  //addConstraint("bd2kstkst_n");
  //addConstraint("bd2kstkst_a");
  //addConstraint("bd2kstkst_n2");
  //addConstraint("bd2kstkst_a2");
}

void Bs2KstKst::SimultaneousMassFit::Bd2PhiKstFit() {
  fit("bd2phikst_mc_pdf", "Bd2PhiKst");
  plot("B_s0_DTF_B_s0_M","Bd2PhiKst","bd2phikst_mc_pdf",2);
  freeze("bd2phikst_mc_pdf");
  //w->var("bd2phikst_mu")->setConstant(false);
  //w->var("bd2phikst_sigma")->setConstant(false);

}

void Bs2KstKst::SimultaneousMassFit::Bs2PhiKstFit() {
  fit("bs2phikst_mc_pdf", "Bs2PhiKst");
  plot("B_s0_DTF_B_s0_M","Bs2PhiKst","bs2phikst_mc_pdf",2);
  freeze("bs2phikst_mc_pdf");
  //addConstraint("bs2phikst_mean");
}

void Bs2KstKst::SimultaneousMassFit::Bd2RhoKstFit() {
  fit("bd2rhokst_mc_pdf", "Bd2RhoKst");
  plot("B_s0_DTF_B_s0_M","Bd2RhoKst","bd2rhokst_mc_pdf",2);
  freeze("bd2rhokst_mc_pdf");
  //w->var("bd2rhokst_mu")->setConstant(false);
  //w->var("bd2rhokst_sigma")->setConstant(false);
}

void Bs2KstKst::SimultaneousMassFit::Lb2pKpipiFit() {
  fit("lb2pkpipi_mc_pdf", "Lb2pKpipi");
  plot("B_s0_DTF_B_s0_M","Lb2pKpipi","lb2pkpipi_mc_pdf",2);
  freeze("lb2pkpipi_mc_pdf");
  //w->var("lb2pkpipi_mean")->setConstant(false);
  //w->var("lb2pkpipi_sigma1")->setConstant(false);
  //w->var("lb2pkpipi_sigma2")->setConstant(false);
}

void Bs2KstKst::SimultaneousMassFit::Lb2ppipipiFit() {
  fit("lb2ppipipi_mc_pdf", "Lb2ppipipi");
  plot("B_s0_DTF_B_s0_M","Lb2ppipipi","lb2ppipipi_mc_pdf",2);
  freeze("lb2ppipipi_mc_pdf");
  //w->var("lb2ppipipi_mean")->setConstant(false);
  //w->var("lb2pkpipi_sigma")->setConstant(false);
}

void Bs2KstKst::SimultaneousMassFit::DataFit() {

  RooArgSet *constraints = (RooArgSet*)w->set("constraints");
  if ( constraints->getSize()>0 ) {
    RooProdPdf *constrainedPdf = new RooProdPdf("constrained_pdf","constrained_pdf",RooArgSet(RooArgSet(*w->pdf("pdf")),*constraints));
    w->import(*constrainedPdf);
    delete constrainedPdf;
    w->defineSet("constrained_pdf_params",*((RooArgSet*)w->set("pdf_params")));
    w->defineSet("constrained_pdf_yield_params",*((RooArgSet*)w->set("pdf_yield_params")));
    w->defineSet("constrained_pdf_nonyield_params",*((RooArgSet*)w->set("pdf_nonyield_params")));

    //if ( verbose || debug ) {

      //cout << "Constrained PDF params: " << endl;
      //w->set("constrained_pdf_params")->Print("v");

      //cout << "PDF non-yield params: " << endl;
      //w->set("pdf_nonyield_params")->Print("v");

      //cout << "PDF yield params: " << endl;
      //w->set("pdf_yield_params")->Print("v");

      //cout << "Constrained PDF non-yield params: " << endl;
      //w->set("constrained_pdf_nonyield_params")->Print("v");

      //cout << "Constrained PDF yield params: " << endl;
      //w->set("constrained_pdf_yield_params")->Print("v");

    //}

    fit("constrained_pdf","DataCombined",true);
    //plot("B_s0_DTF_B_s0_M","Data","constrained_pdf",2);
  }
  else {
    fit("pdf","DataCombined");
    //plot("B_s0_DTF_B_s0_M","Data","pdf",2);
  }


}


/// ----------------------------------------------------------------- ///
/// ----------------------------------------------------------------- ///
/// ---------------------------- PDFS ------------------------------- ///
/// ----------------------------------------------------------------- ///
/// ----------------------------------------------------------------- ///

// Bs2KstKst MC
void Bs2KstKst::SimultaneousMassFit::makeBs2KstKstPdf() {

  addParameter("bs2kstkst_l"       ,-5    ,-20    ,0  );
  addParameter("bs2kstkst_zeta"    ,0.             );
  addParameter("bs2kstkst_fb"      ,0.                );
  addParameter("bs2kstkst_sigma"   ,15    ,10     ,100);
  addParameter("bs2kstkst_mu"      ,5350  ,5400       );
  addParameter("bs2kstkst_a"       , 2.5  , 0     ,10 );
  addParameter("bs2kstkst_n"       , 2.5  , 0     ,10 );
  addParameter("bs2kstkst_a2"      , 2.5  , 0     ,10 );
  addParameter("bs2kstkst_n2"      , 2.5  , 0     ,10 );

  w->var("bs2kstkst_zeta")->setConstant(true);
  w->var("bs2kstkst_fb")->setConstant(true);

  RooIpatia2 *pdf = new RooIpatia2("bs2kstkst_mc_pdf","bs2kstkst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bs2kstkst_l"),*w->var("bs2kstkst_zeta"),*w->var("bs2kstkst_fb"),*w->var("bs2kstkst_sigma"),*w->var("bs2kstkst_mu"),*w->var("bs2kstkst_a"),*w->var("bs2kstkst_n"),*w->var("bs2kstkst_a2"),*w->var("bs2kstkst_n2"));

  w->import(*pdf);
  delete pdf;

  defineParamSet("bs2kstkst_mc_pdf");

}

// Bd2KstKst MC
void Bs2KstKst::SimultaneousMassFit::makeBd2KstKstPdf() {

  addParameter("bd2kstkst_l"       ,-5    ,-20    ,0  );
  addParameter("bd2kstkst_zeta"    ,0.             );
  addParameter("bd2kstkst_fb"      ,0.                );
  addParameter("bd2kstkst_sigma"   ,15    ,10     ,100);
  addParameter("bd2kstkst_mu"      ,5250  ,5300       );
  addParameter("bd2kstkst_a"       , 2.5  , 0     ,10 );
  addParameter("bd2kstkst_n"       , 2.5  , 0     ,10 );
  addParameter("bd2kstkst_a2"      , 2.5  , 0     ,10 );
  addParameter("bd2kstkst_n2"      , 2.5  , 0     ,10 );

  w->var("bd2kstkst_zeta")->setConstant(true);
  w->var("bd2kstkst_fb")->setConstant(true);

  RooIpatia2 *pdf = new RooIpatia2("bd2kstkst_mc_pdf","bd2kstkst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2kstkst_l"),*w->var("bd2kstkst_zeta"),*w->var("bd2kstkst_fb"),*w->var("bd2kstkst_sigma"),*w->var("bd2kstkst_mu"),*w->var("bd2kstkst_a"),*w->var("bd2kstkst_n"),*w->var("bd2kstkst_a2"),*w->var("bd2kstkst_n2"));

  w->import(*pdf);
  delete pdf;

  defineParamSet("bd2kstkst_mc_pdf");
}

// Bd2PhiKst MC
void Bs2KstKst::SimultaneousMassFit::makeBd2PhiKstPdf() {

  addParameter("bd2phikst_l"       ,-5    ,-40   ,-1  );
  addParameter("bd2phikst_zeta"    ,0.             );
  addParameter("bd2phikst_fb"      ,0.                );
  addParameter("bd2phikst_sigma"   ,15    ,10     ,100);
  addParameter("bd2phikst_mu"      ,5200  ,5270       );
  addParameter("bd2phikst_a"       , 2.5  , 0     ,10 );
  addParameter("bd2phikst_n"       , 2.5  , 0     ,40 );
  addParameter("bd2phikst_a2"      , 2.5  , 0     ,10 );
  addParameter("bd2phikst_n2"      , 2.5  , 0     ,10 );

  w->var("bd2phikst_zeta")->setConstant(true);
  w->var("bd2phikst_fb")->setConstant(true);

  RooIpatia2 *pdf = new RooIpatia2("bd2phikst_mc_pdf","bd2phikst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2phikst_l"),*w->var("bd2phikst_zeta"),*w->var("bd2phikst_fb"),*w->var("bd2phikst_sigma"),*w->var("bd2phikst_mu"),*w->var("bd2phikst_a"),*w->var("bd2phikst_n"),*w->var("bd2phikst_a2"),*w->var("bd2phikst_n2"));

  w->import(*pdf);
  delete pdf;

  defineParamSet("bd2phikst_mc_pdf");
}

// Bs2PhiKst MC
void Bs2KstKst::SimultaneousMassFit::makeBs2PhiKstPdf() {

  addParameter("bs2phikst_l"       ,-5    ,-40   ,-1  );
  addParameter("bs2phikst_zeta"    ,0.             );
  addParameter("bs2phikst_fb"      ,0.                );
  addParameter("bs2phikst_sigma"   ,15    ,10     ,100);
  addParameter("bs2phikst_mu"      ,5300  ,5400       );
  addParameter("bs2phikst_a"       , 2.5  , 0     ,10 );
  addParameter("bs2phikst_n"       , 2.5  , 0     ,40 );
  addParameter("bs2phikst_a2"      , 2.5  , 0     ,10 );
  addParameter("bs2phikst_n2"      , 2.5  , 0     ,10 );

  w->var("bs2phikst_zeta")->setConstant(true);
  w->var("bs2phikst_fb")->setConstant(true);

  RooIpatia2 *pdf = new RooIpatia2("bs2phikst_mc_pdf","bs2phikst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bs2phikst_l"),*w->var("bs2phikst_zeta"),*w->var("bs2phikst_fb"),*w->var("bs2phikst_sigma"),*w->var("bs2phikst_mu"),*w->var("bs2phikst_a"),*w->var("bs2phikst_n"),*w->var("bs2phikst_a2"),*w->var("bs2phikst_n2"));

  w->import(*pdf);
  delete pdf;

  //w->factory("bs2phikst_mean[5250,5350]");
  //w->factory("CBShape::bs2phikst_mc_cb1( B_s0_DTF_B_s0_M, bs2phikst_mean, bs2phikst_sigma1[100,0,400], bs2phikst_alpha1[0.5,0.1,10..], bs2phikst_n1[0.1,0.,10.])");
  //w->factory("CBShape::bs2phikst_mc_cb2( B_s0_DTF_B_s0_M, bs2phikst_mean, bs2phikst_sigma2[100,0,400], bs2phikst_alpha2[-0.5,-10.,-0.1.], bs2phikst_n2[0.1,0.,10.])");
  //w->factory("SUM::bs2phikst_mc_pdf( bs2phikst_f1[0.6,0.,1.]*bs2phikst_mc_cb1, bs2phikst_mc_cb2 )");
  defineParamSet("bs2phikst_mc_pdf");
}

// Bd2RhoKst MC
void Bs2KstKst::SimultaneousMassFit::makeBd2RhoKstPdf() {

  addParameter("bd2rhokst_l"       ,-5    ,-40   ,-1  );
  addParameter("bd2rhokst_zeta"    ,0.             );
  addParameter("bd2rhokst_fb"      ,0.                );
  addParameter("bd2rhokst_sigma"   ,15    ,10     ,100);
  addParameter("bd2rhokst_mu"      ,5300  ,5400       );
  addParameter("bd2rhokst_a"       , 2.5  , 0     ,10 );
  addParameter("bd2rhokst_n"       , 2.5  , 0     ,40 );
  addParameter("bd2rhokst_a2"      , 2.5  , 0     ,10 );
  addParameter("bd2rhokst_n2"      , 2.5  , 0     ,10 );

  w->var("bd2rhokst_zeta")->setConstant(true);
  w->var("bd2rhokst_fb")->setConstant(true);

  RooIpatia2 *pdf = new RooIpatia2("bd2rhokst_mc_pdf","bd2rhokst_mc_pdf",*w->var("B_s0_DTF_B_s0_M"),*w->var("bd2rhokst_l"),*w->var("bd2rhokst_zeta"),*w->var("bd2rhokst_fb"),*w->var("bd2rhokst_sigma"),*w->var("bd2rhokst_mu"),*w->var("bd2rhokst_a"),*w->var("bd2rhokst_n"),*w->var("bd2rhokst_a2"),*w->var("bd2rhokst_n2"));

  w->import(*pdf);
  delete pdf;

  defineParamSet("bd2rhokst_mc_pdf");
}

// Lb2pKpipi MC
void Bs2KstKst::SimultaneousMassFit::makeLb2pKpipiPdf() {

  w->factory("lb2pkpipi_mean[5450,5550]");
  w->factory("CBShape::lb2pkpipi_mc_cb1( B_s0_DTF_B_s0_M, lb2pkpipi_mean, lb2pkpipi_sigma1[10,0,200], lb2pkpipi_alpha1[0.04,0.,10.], lb2pkpipi_n1[0.1,0.,100.])");
  w->factory("CBShape::lb2pkpipi_mc_cb2( B_s0_DTF_B_s0_M, lb2pkpipi_mean, lb2pkpipi_sigma2[10,0,200], lb2pkpipi_alpha2[-0.04,-10.,0.], lb2pkpipi_n2[0.1,0.,100.])");
  w->factory("SUM::lb2pkpipi_mc_pdf( lb2pkpipi_f1[0.4,0.,1.]*lb2pkpipi_mc_cb1, lb2pkpipi_mc_cb2 )");
  defineParamSet("lb2pkpipi_mc_pdf");
}

// Lb2ppipipi MC
void Bs2KstKst::SimultaneousMassFit::makeLb2ppipipiPdf() {

  w->factory("lb2ppipipi_mean[5450,5600]");
  w->factory("CBShape::lb2ppipipi_mc_pdf( B_s0_DTF_B_s0_M, lb2ppipipi_mean, lb2ppipipi_sigma[10,0,200], lb2ppipipi_alpha[0.04,0.,10.], lb2ppipipi_n[5.,0.,10.])");
  defineParamSet("lb2ppipipi_mc_pdf");
}

void Bs2KstKst::SimultaneousMassFit::makePartRecoPdf() {

  //w->factory("Bernstein::part_reco_pdf( B_s0_DTF_B_s0_M, { bkg_bern_p0[1.], bkg_bern_p1[0.5,-1.,1.], bkg_bern_p2[0.5,-1.,1.] } )");
  //w->factory("ExpAndGaus::part_reco_pdf( B_s0_DTF_B_s0_M, part_reco_mbar(-0.05,0.05),
  w->factory("ArgusBG::part_reco_pdf( B_s0_DTF_B_s0_M, part_reco_m0[5226], part_reco_c[-10.0,-50.,-10.], part_reco_p[0.4,0.,1.] )");
  //w->importClassCode(RooPhysBkg::Class(),kTRUE);
  //w->factory("PhysBkg::part_reco_pdf( B_s0_DTF_B_s0_M, part_reco_m0[5200,5000,5400], part_reco_c[-20.,-50.,-10.], part_reco_s[20,1,100] )");
  defineParamSet("part_reco_pdf");
}

void Bs2KstKst::SimultaneousMassFit::makeCombinatorialPdf() {

  // make one for each category
  RooCategory *cat = (RooCategory*)w->cat("DataCat");
  for ( int i=0; i < cat->numTypes(); i++ ) {
    cat->setIndex(i);
    w->factory( Form("Exponential::bkg_pdf_%s( B_s0_DTF_B_s0_M, bkg_exp_p1_%s[-0.002,-0.004,0.] )", cat->getLabel(), cat->getLabel() ) );
    defineParamSet( Form("bkg_pdf_%s",cat->getLabel()) );
  }
}

void Bs2KstKst::SimultaneousMassFit::makeTotalPdf() {

  // constrain the bs->phikst / bd->phikst ratio
  addParameter("yield_ratio_bsobd_phikst",  0, 1.);
  addConstraint("yield_ratio_bsobd_phikst", 0.113, 0.0287);

  // make a yield for each category
  RooCategory *cat = (RooCategory*)w->cat("DataCat");
  for ( int i=0; i < cat->numTypes(); i++ ) {
    cat->setIndex(i);
    addParameter( Form("bkg_y_%s",       cat->getLabel()),  0, 200e3);
    addParameter( Form("part_reco_y_%s", cat->getLabel()),  0, 200e3);
    addParameter( Form("bs2kstkst_y_%s", cat->getLabel()),   0, 20e3);
    addParameter( Form("bd2kstkst_y_%s", cat->getLabel()),  0, 3000);
    addParameter( Form("bd2phikst_y_%s", cat->getLabel()),  0, 5000);
    // add bs2phikst yield as constrained ratio
    w->factory( Form("prod::bs2phikst_y_%s( yield_ratio_bsobd_phikst, bd2phikst_y_%s )", cat->getLabel(), cat->getLabel()) );
    //addParameter( Form("bs2phikst_y_%s", cat->getLabel()),  0, 250);
    addParameter( Form("bd2rhokst_y_%s", cat->getLabel()),  0, 10e4);
    addParameter( Form("lb2pkpipi_y_%s", cat->getLabel()),  0, 4000);
    addParameter( Form("lb2ppipipi_y_%s", cat->getLabel()), 0, 4000);
  }

  // construct the pdf for each category
  for ( int i=0; i < cat->numTypes(); i++ ) {
    cat->setIndex(i);

    RooArgList *yields = new RooArgList();
    yields->add(*w->var( Form("bkg_y_%s"      , cat->getLabel()) ));
    yields->add(*w->var( Form("part_reco_y_%s", cat->getLabel()) ));
    yields->add(*w->var( Form("bs2kstkst_y_%s", cat->getLabel()) ));
    yields->add(*w->var( Form("bd2kstkst_y_%s", cat->getLabel()) ));
    yields->add(*w->var( Form("bd2phikst_y_%s", cat->getLabel()) ));
    yields->add(*w->function( Form("bs2phikst_y_%s", cat->getLabel()) ));
    yields->add(*w->var( Form("bd2rhokst_y_%s", cat->getLabel()) ));
    yields->add(*w->var( Form("lb2pkpipi_y_%s", cat->getLabel()) ));
    yields->add(*w->var( Form("lb2ppipipi_y_%s", cat->getLabel()) ));

    RooArgList *pdfs   = new RooArgList();
    pdfs->add(*w->pdf( Form("bkg_pdf_%s", cat->getLabel()) ));
    pdfs->add(*w->pdf("part_reco_pdf" ));
    pdfs->add(*w->pdf("bs2kstkst_mc_pdf"  ));
    pdfs->add(*w->pdf("bd2kstkst_mc_pdf" ));
    pdfs->add(*w->pdf("bd2phikst_mc_pdf" ));
    pdfs->add(*w->pdf("bs2phikst_mc_pdf" ));
    pdfs->add(*w->pdf("bd2rhokst_mc_pdf" ));
    pdfs->add(*w->pdf("lb2pkpipi_mc_pdf" ));
    pdfs->add(*w->pdf("lb2ppipipi_mc_pdf"));

    RooAddPdf *pdf = new RooAddPdf( Form("pdf_%s",cat->getLabel()), "pdf" , *pdfs, *yields);
    w->import(*pdf);

    defineParamSet( Form("pdf_%s", cat->getLabel()) );
    defineYieldSet( Form("pdf_%s", cat->getLabel()) );

    delete pdf;
    delete yields;
    delete pdfs;

    // this is to catch the constraint
    ((RooArgSet*)w->set( Form("pdf_%s_yield_params", cat->getLabel()) ))->remove( *w->var("yield_ratio_bsobd_phikst") );
    ((RooArgSet*)w->set( Form("pdf_%s_nonyield_params", cat->getLabel()) ))->add( *w->var("yield_ratio_bsobd_phikst") );

  }
  // now make simultaneous pdf
  RooSimultaneous *pdf = new RooSimultaneous( "pdf", "pdf", *w->cat("DataCat") );
  for ( int i=0; i < cat->numTypes(); i++ ) {
    cat->setIndex(i);
    pdf->addPdf( *w->pdf( Form("pdf_%s", cat->getLabel() )), cat->getLabel() );
  }
  w->import(*pdf);

  defineParamSet("pdf");
  defineYieldSet("pdf");
  // this is to catch the constraint
  ((RooArgSet*)w->set("pdf_yield_params"))->remove( *w->var("yield_ratio_bsobd_phikst") );
  ((RooArgSet*)w->set("pdf_nonyield_params"))->add( *w->var("yield_ratio_bsobd_phikst") );

  delete pdf;

}

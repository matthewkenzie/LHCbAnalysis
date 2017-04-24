#include "MassVariablesPlotter.h"
#include "TMath.h"
#include "TLorentzVector.h"

using namespace std;
using namespace TMath;

Bs2KstKst::MassVariablesPlotter::MassVariablesPlotter(TString _name, const Variables_Analysis *_v):
  PlotterBase(_name,_v),
  v(_v),
  split(false)
{
  normalise = true;
  normalisation = 1.;
  system(Form("mkdir -p root/%s",_name.Data()));
  outfilename = Form("root/%s/%sOut.root",_name.Data(),_name.Data());
}

Bs2KstKst::MassVariablesPlotter::~MassVariablesPlotter(){}

void Bs2KstKst::MassVariablesPlotter::defineDrawingConfig(){

  // add these drawing options
  // -------------------------------------------- //
  // split years same plot
  TColor *blueFill = gROOT->GetColor(kBlue-4);
  blueFill->SetAlpha(0.4);
  TColor *redFill = gROOT->GetColor(kRed-4);
  redFill->SetAlpha(0.4);

  addDrawOpt("Bs2Kst0Kst0", "B_{s}^{0}#rightarrowK^{*0}K^{*0} MC", -70, -80 );
  setDrawOptDefaultFill(blueFill->GetNumber());

  addDrawOpt("Bs2KpiKpiPS", "B_{s}^{0}#rightarrow(K^{+}#pi^{-})(K^{-}#pi^{+}) PS MC", -73, -83 );
  setDrawOptDefaultFill(redFill->GetNumber());

  //addDrawOpt("Bd2RhoKst0", "B_{d}^{0}#rightarrow#rho^{0}K^{*0} MC", -77, -87 );
  //setDrawOptDefaultFill(redFill->GetNumber());

  addDrawOpt("SidebandData", "Data", 71, 81);
  setDrawOptDefaultLine(kBlack);

}

void Bs2KstKst::MassVariablesPlotter::defineHistograms(){

  // add these histograms
  //
  addHist("B_s0_DTF_B_s0_M", "m(K^{+}#pi^{-}K^{-}#pi^{+}) (MeV)", 100, 5200,5800, "R");
  addHist("B_s0_DTF_KST1_M", "m(K^{-}#pi^{+}) (MeV)", 50, 800, 1750, "R");
  addHist("B_s0_DTF_KST2_M", "m(K^{+}#pi^{-}) (MeV)", 50, 800, 1750, "R");

  addHist("M_KpKmPip" , "m(K^{+}K^{-}#pi^{+}) (MeV)", 100, 2000, 5600, "R");
  addHist("M_KpKmPim" , "m(K^{+}K^{-}#pi^{-}) (MeV)", 100, 2000, 5600, "R");
  addHist("M_KpPipPim", "m(K^{+}#pi^{+}#pi^{-}) (MeV)", 100, 0, 5600, "L");
  addHist("M_KmPipPim", "m(K^{-}#pi^{+}#pi^{-}) (MeV)", 100, 0, 5600, "L");

  addHist("M_KpKm"    , "m(K^{+}K^{-}) (MeV)", 100, 800, 5500, "R");
  addHist("M_PipPim"  , "m(#pi^{+}#pi^{-}) (MeV)", 100, 0, 6000, "R");

  addHist("B_s0_M_KpPimPimPip", "m(K^{+}#pi^{-}#pi^{-}#pi^{+})", 100, 5100, 5800, "R" );
  addHist("B_s0_M_PipPimKmPip", "m(#pi^{+}#pi^{-}K^{-}#pi^{+})", 100, 5100, 5800, "R" );
  addHist("B_s0_M_KpKmKmPip"  , "m(K^{+}K^{-}K^{-}#pi^{+})"    , 100, 5350, 5800, "R" );
  addHist("B_s0_M_KpPimKmKp"  , "m(K^{+}#pi^{-}K^{-}K^{+})"    , 100, 5350, 5800, "R" );

  //addHist("Kplus_V3ProbNNk"   , "Kplus_V3ProbNNk", 50, 0, 1.);
  //addHist("Kminus_V3ProbNNk"  , "Kminus_V3ProbNNk", 50, 0, 1.);
  //addHist("Piplus_V3ProbNNk"  , "Piplus_V3ProbNNk", 50, 0, 1.);
  //addHist("Piminus_V3ProbNNk" , "Piminus_V3ProbNNk", 50, 0, 1.);

  //addHist("Kplus_V3ProbNNpi"   , "Kplus_V3ProbNNpi", 50, 0, 1.);
  //addHist("Kminus_V3ProbNNpi"  , "Kminus_V3ProbNNpi", 50, 0, 1.);
  //addHist("Piplus_V3ProbNNpi"  , "Piplus_V3ProbNNpi", 50, 0, 1.);
  //addHist("Piminus_V3ProbNNpi" , "Piminus_V3ProbNNpi", 50, 0, 1.);

  //addHist("Kplus_V3ProbNNk_corr"   , "Kplus_V3ProbNNk_corr", 50, 0, 1.);
  //addHist("Kminus_V3ProbNNk_corr"  , "Kminus_V3ProbNNk_corr", 50, 0, 1.);
  //addHist("Piplus_V3ProbNNk_corr"  , "Piplus_V3ProbNNk_corr", 50, 0, 1.);
  //addHist("Piminus_V3ProbNNk_corr" , "Piminus_V3ProbNNk_corr", 50, 0, 1.);

  //addHist("Kplus_V3ProbNNpi_corr"   , "Kplus_V3ProbNNpi_corr", 50, 0, 1.);
  //addHist("Kminus_V3ProbNNpi_corr"  , "Kminus_V3ProbNNpi_corr", 50, 0, 1.);
  //addHist("Piplus_V3ProbNNpi_corr"  , "Piplus_V3ProbNNpi_corr", 50, 0, 1.);
  //addHist("Piminus_V3ProbNNpi_corr" , "Piminus_V3ProbNNpi_corr", 50, 0, 1.);

}

bool Bs2KstKst::MassVariablesPlotter::fillHistograms(){

  // don't fill if they don't pass the cuts
  if ( !v->pass_bdt ) return true;
  if ( !v->pass_pid ) return true;
  if ( v->pass_rhokst ) return true;
  if ( v->pass_massveto ) return true;

  double K_mass  = 493.677;
  double PI_mass = 139.57018;

  TLorentzVector kp_p4;
  TLorentzVector km_p4;
  TLorentzVector pip_p4;
  TLorentzVector pim_p4;

  km_p4.SetXYZM( v->B_s0_DTF_KST1_K_PX, v->B_s0_DTF_KST1_K_PY, v->B_s0_DTF_KST1_K_PZ, K_mass );
  pip_p4.SetXYZM( v->B_s0_DTF_KST1_PI_PX, v->B_s0_DTF_KST1_PI_PY, v->B_s0_DTF_KST1_PI_PZ, PI_mass );
  kp_p4.SetXYZM( v->B_s0_DTF_KST2_K_PX, v->B_s0_DTF_KST2_K_PY, v->B_s0_DTF_KST2_K_PZ, K_mass );
  pim_p4.SetXYZM( v->B_s0_DTF_KST2_PI_PX, v->B_s0_DTF_KST2_PI_PY, v->B_s0_DTF_KST2_PI_PZ, PI_mass );

  TLorentzVector kpkm_p4 = kp_p4 + km_p4;
  TLorentzVector pippim_p4 = pip_p4 + pim_p4;

  TLorentzVector kppippim_p4 = kp_p4 + pip_p4 + pim_p4;
  TLorentzVector kmpippim_p4 = km_p4 + pip_p4 + pim_p4;
  TLorentzVector kpkmpip_p4  = kp_p4 + km_p4 + pip_p4;
  TLorentzVector kpkmpim_p4  = kp_p4 + km_p4 + pim_p4;

  // fill hists now
  fillHist("B_s0_DTF_B_s0_M"   ,   v->B_s0_DTF_B_s0_M );
  fillHist("B_s0_DTF_KST1_M"   ,   v->B_s0_DTF_KST1_M );
  fillHist("B_s0_DTF_KST2_M"   ,   v->B_s0_DTF_KST2_M );

  fillHist("M_KpKmPip"         ,   v->M_KpKmPip );
  fillHist("M_KpKmPim"         ,   v->M_KpKmPim );
  fillHist("M_KpPipPim"        ,   v->M_KpPipPim );
  fillHist("M_KmPipPim"        ,   v->M_KmPipPim );

  fillHist("M_KpKm"            ,   v->M_KpKm );
  fillHist("M_PipPim"          ,   v->M_PipPim );

  fillHist("B_s0_M_KpPimPimPip",   v->B_s0_M_KpPimPimPip );
  fillHist("B_s0_M_PipPimKmPip",   v->B_s0_M_PipPimKmPip );
  fillHist("B_s0_M_KpKmKmPip"  ,   v->B_s0_M_KpKmKmPip   );
  fillHist("B_s0_M_KpPimKmKp"  ,   v->B_s0_M_KpPimKmKp   );

  //fillHist("Kplus_V3ProbNNk", v->Kplus_MC12TuneV3_ProbNNk );
  //fillHist("Kminus_V3ProbNNk", v->Kminus_MC12TuneV3_ProbNNk );
  //fillHist("Piplus_V3ProbNNk", v->Piplus_MC12TuneV3_ProbNNk );
  //fillHist("Piminus_V3ProbNNk", v->Piminus_MC12TuneV3_ProbNNk );

  //fillHist("Kplus_V3ProbNNpi", v->Kplus_MC12TuneV3_ProbNNpi );
  //fillHist("Kminus_V3ProbNNpi", v->Kminus_MC12TuneV3_ProbNNpi );
  //fillHist("Piplus_V3ProbNNpi", v->Piplus_MC12TuneV3_ProbNNpi );
  //fillHist("Piminus_V3ProbNNpi", v->Piminus_MC12TuneV3_ProbNNpi );

  //fillHist("Kplus_V3ProbNNk_corr", v->Kplus_V3ProbNNk_corr );
  //fillHist("Kminus_V3ProbNNk_corr", v->Kminus_V3ProbNNk_corr );
  //fillHist("Piplus_V3ProbNNk_corr", v->Piplus_V3ProbNNk_corr );
  //fillHist("Piminus_V3ProbNNk_corr", v->Piminus_V3ProbNNk_corr );

  //fillHist("Kplus_V3ProbNNpi_corr", v->Kplus_V3ProbNNpi_corr );
  //fillHist("Kminus_V3ProbNNpi_corr", v->Kminus_V3ProbNNpi_corr );
  //fillHist("Piplus_V3ProbNNpi_corr", v->Piplus_V3ProbNNpi_corr );
  //fillHist("Piminus_V3ProbNNpi_corr", v->Piminus_V3ProbNNpi_corr );

  return true;
}

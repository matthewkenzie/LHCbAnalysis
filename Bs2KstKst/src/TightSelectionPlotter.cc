#include "TightSelectionPlotter.h"
#include "TMath.h"

using namespace std;
using namespace TMath;

Bs2KstKst::TightSelectionPlotter::TightSelectionPlotter(TString _name, const Variables_Analysis *_v):
  PlotterBase(_name,_v),
  v(_v),
  split(false)
{
  normalise = true;
  normalisation = 1.;
  outfilename = Form("root/%sOut.root",_name.Data());
}

Bs2KstKst::TightSelectionPlotter::~TightSelectionPlotter(){}

void Bs2KstKst::TightSelectionPlotter::defineHistograms(){

  // add these histograms
  //
  addHist("B_s0_DTF_B_s0_M", "m(K^{+}#pi^{-}K^{-}#pi^{+}) (MeV)", 100, 5000,5800, "R");
  addHist("B_s0_DTF_KST1_M", "m(K^{-}#pi^{+}) (MeV)", 50, 750, 1050, "R");
  addHist("B_s0_DTF_KST2_M", "m(K^{+}#pi^{-}) (MeV)", 50, 750, 1050, "R");

  addHist("ln_B_s0_PT", "p_{T}(K^{+}#pi^{-}K^{-}#pi^{+}) (MeV)", 50, 6, 11, "L");
  addHist("ln_Kst_PT", "p_{T}(K^{-}#pi^{+}) (MeV)", 50, 6, 11, "L");
  addHist("ln_Kstb_PT", "p_{T}(K^{+}#pi^{-}) (MeV)", 50, 6, 11, "L");
  addHist("ln_max_track_PT", "p_{T} (max track) (MeV)", 50, 6, 11, "L");
  addHist("ln_min_track_PT", "p_{T} (min track) (MeV)", 50, 6, 11, "L");

  addHist("B_s0_ETA", "#eta(K^{+}#pi^{-}K^{-}#pi^{+}) (MeV)", 50, 1, 8, "R");
  addHist("Kst_ETA", "#eta(K^{-}#pi^{+}) (MeV)", 50, 1, 8, "R");
  addHist("Kstb_ETA", "#eta(K^{+}#pi^{-}) (MeV)", 50, 1, 8, "R");
  addHist("max_track_ETA", "#eta (max track) (MeV)", 50, 1, 8, "R");
  addHist("min_track_ETA", "#eta (min track) (MeV)", 50, 1, 8, "R");

  addHist("B_s0_ARCCOS_DIRA", "B_{s} ACos(DIRA)", 50,0.,0.025);
  addHist("B_s0_VTX_CHI2", "B_{s} Vtx #chi^{2}",50,0,40,"R");

}

void Bs2KstKst::TightSelectionPlotter::defineDrawingConfig(){

  // add these drawing options
  // -------------------------------------------- //
  // split years same plot
  if ( split ) {
    TColor *blueFill = gROOT->GetColor(kBlue-4);
    blueFill->SetAlpha(0.4);
    TColor *redFill = gROOT->GetColor(kRed-4);
    redFill->SetAlpha(0.4);

    addDrawOpt("mc_sig_2011",   "Signal MC 2011", -70);
    setDrawOptDefaultFill(blueFill->GetNumber());
    addDrawOpt("mc_sig_2012",   "Signal MC 2012", -80);
    setDrawOptDefaultFill(redFill->GetNumber());

    addDrawOpt("data_2011",   "Data 2011", 71);
    setDrawOptDefaultPoint(kBlue);
    addDrawOpt("data_2012",   "Data 2012", 81);
    setDrawOptDefaultPoint(kRed);

    // add this as a residual plot option
    addResidOpt(make_pair(2,0));
    addResidOpt(make_pair(3,1));
    setResidType(1);
    // -------------------------------------------- //
  }
  // -------------------------------------------- //
  // combined years
  else {
    TColor *greenFill = gROOT->GetColor(kGreen-3);
    greenFill->SetAlpha(0.4);

    addDrawOpt("mc_sig",   "Signal MC", -70, -80);
    setDrawOptDefaultFill(greenFill->GetNumber());

    addDrawOpt("data",   "Data", 71, 81);
    setDrawOptDefaultPoint(kGreen+3);

    addResidOpt(make_pair(1,0));
    setResidType(1);
  }
  // -------------------------------------------- //

}

bool Bs2KstKst::TightSelectionPlotter::fillHistograms(){

  // fill hists now
  fillHist("B_s0_DTF_B_s0_M",           v->B_s0_MM);
  fillHist("B_s0_DTF_KST1_M",            v->Kst_MM);
  fillHist("B_s0_DTF_KST2_M",           v->Kstb_MM);

  fillHist("ln_B_s0_PT",        TMath::Log(v->B_s0_PT));
  fillHist("ln_Kst_PT",         TMath::Log(v->Kst_PT));
  fillHist("ln_Kstb_PT",        TMath::Log(v->Kstb_PT));
  fillHist("ln_max_track_PT",   TMath::Log( TMath::Max( TMath::Max(v->Kplus_PT, v->Kminus_PT), TMath::Max(v->Piplus_PT, v->Piminus_PT) )));
  fillHist("ln_min_track_PT",   TMath::Log( TMath::Min( TMath::Min(v->Kplus_PT, v->Kminus_PT), TMath::Min(v->Piplus_PT, v->Piminus_PT) )));

  fillHist("B_s0_ETA",          TMath::Abs(v->B_s0_ETA));
  fillHist("Kst_ETA",           TMath::Abs(v->Kst_ETA));
  fillHist("Kstb_ETA",          TMath::Abs(v->Kstb_ETA));
  fillHist("max_track_ETA",     TMath::Max( TMath::Max(TMath::Abs(v->Kplus_ETA), TMath::Abs(v->Kminus_ETA)), TMath::Max(TMath::Abs(v->Piplus_ETA), TMath::Abs(v->Piminus_ETA)) ));
  fillHist("min_track_ETA",     TMath::Min( TMath::Min(TMath::Abs(v->Kplus_ETA), TMath::Abs(v->Kminus_ETA)), TMath::Min(TMath::Abs(v->Piplus_ETA), TMath::Abs(v->Piminus_ETA)) ));

  fillHist("B_s0_ARCCOS_DIRA",  TMath::ACos(v->B_s0_DIRA_OWNPV));
  fillHist("B_s0_VTX_CHI2",     v->B_s0_ENDVERTEX_CHI2);

  return true;
}

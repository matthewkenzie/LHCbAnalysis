#include "Plotter.h"
#include "TMath.h"

using namespace std;
using namespace TMath;

Dst2D0pi::Plotter::Plotter(TString _name, const Variables_Analysis *_v):
  PlotterBase(_name,_v),
  v(_v),
  split(false)
{
  normalise = true;
  normalisation = 1.;
  outfilename = Form("root/%sOut.root",_name.Data());
}

Dst2D0pi::Plotter::~Plotter(){}

void Dst2D0pi::Plotter::defineHistograms(){

  // add these histograms
  //

  addHist("Dst_M"          , "m(D^{#star}) [MeV/c^{2}]", 100, 1900,2100, "R");
  addHist("D0_M"           , "m(D^{0}) [MeV/c^{2}]", 100, 1800, 1950, "R");
  addHist("DstD0_Mdiff"    , "m(D^{#star}) - m(D^{0}) [MeV/c^{2}]", 100, 0,200, "L");
  addHist("D0_PT"          ,  "p_{T}(D^{0}) [MeV/c]", 100, 0, 40000, "R");
  addHist("D0_TAU"         , "t(D^{0}) [ns]" , 100, 0, 0.005, "R");
  addHist("D0_MINIPCHI2"   , "Min IP #chi^{2} (D^{0})", 100,-5,15,"R");
  addHist("D0_LOGMINIPCHI2", "Log Min IP #chi^{2} (D^{0})", 100,-5,15,"L");
  addHist("D0_ENDVERTEX_CHI2",  "Vtx #chi^{2} (D^{-})", 100, 0, 25, "R");
  addHist("D0_LOGENDVERTEX_CHI2", "Log Vtx #chi^{2} (D^{0})", 100, -5, 15, "R" );
  //addHist("Kminus_PIDK", "PIDK (K^{-})",   100, 0, 200, "R");
  //addHist("piplus_PIDK", "PIDK (#pi^{+})", 100, -200, 0, "L");

  addHist("nCandidate", "N_{C}", 20, 0, 20, "R");
}

void Dst2D0pi::Plotter::defineDrawingConfig(){

  // add these drawing options
  // -------------------------------------------- //
  // -------------------------------------------- //
  TColor *blueFill = gROOT->GetColor(kBlue-7);
  blueFill->SetAlpha(0.4);
  TColor *redFill = gROOT->GetColor(kRed-7);
  redFill->SetAlpha(0.4);
  TColor *greenFill = gROOT->GetColor(kGreen-3);
  greenFill->SetAlpha(0.4);

  //addDrawOpt("mc_Bu",   "MC B^{+}#rightarrow D^{0}#mu#nu", -82);
  //setDrawOptDefaultFill(blueFill->GetNumber());

  //addDrawOpt("mc_Bc",   "MC B^{+}_{c}#rightarrow D^{0}#mu#nu", -80);
  //setDrawOptDefaultFill(redFill->GetNumber());

  //addDrawOpt("mc_Bc_Dst", "MC B^{+}_{c}#rightarrow D^{*}#mu#nu", -81);
  //setDrawOptDefaultFill(greenFill->GetNumber());

  addDrawOpt("data",   "Data", 80);
  setDrawOptDefaultPoint(kBlack);

  //addResidOpt(make_pair(3,0));
  //setResidType(1);
  // -------------------------------------------- //

}

bool Dst2D0pi::Plotter::fillHistograms(){

  // fill hists now
  fillHist("Dst_M"       , v->Dst_M        );
  fillHist("D0_M"        , v->D0_M         );
  fillHist("DstD0_Mdiff" , (v->Dst_M-v->D0_M) );
  fillHist("D0_PT"       , v->D0_PT        );
  fillHist("D0_TAU"      , v->D0_TAU       );
  fillHist("D0_MINIPCHI2", v->D0_MINIPCHI2 );
  fillHist("D0_LOGMINIPCHI2", TMath::Log(v->D0_MINIPCHI2) );
  fillHist("D0_ENDVERTEX_CHI2",  v->D0_ENDVERTEX_CHI2 ) ;
  fillHist("D0_LOGENDVERTEX_CHI2", TMath::Log( v->D0_ENDVERTEX_CHI2 ) );
  //fillHist("Kminus_PIDK", v->Kminus_PIDK );
  //fillHist("piplus_PIDK", v->piplus_PIDK );
  fillHist("nCandidate", int(v->nCandidate) );
  return true;
}

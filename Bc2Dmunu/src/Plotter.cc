#include "Plotter.h"
#include "TMath.h"

using namespace std;
using namespace TMath;

Bc2Dmunu::Plotter::Plotter(TString _name, const Variables_Analysis *_v):
  PlotterBase(_name,_v),
  v(_v),
  split(false)
{
  normalise = true;
  normalisation = 1.;
  outfilename = Form("root/%sOut.root",_name.Data());
}

Bc2Dmunu::Plotter::~Plotter(){}

void Bc2Dmunu::Plotter::defineHistograms(){

  // add these histograms
  //

  addHist("B_plus_M"    , "m(B^{+}) [MeV]", 100, 2000,8000, "R");
  addHist("B_plus_MCORR", "m_{corr}(B^{+}) [MeV]", 100,2000,8000, "R");
  addHist("B_plus_LTIME",   "#tau(B^{+}) [ns]", 100,0,0.01,"R");
  addHist("D0_M"       , "m(D^{0}) [MeV]", 100, 1800, 1950, "R");
  addHist("D0_LOGIPCHI2", "Log Min IP #chi^{2} (D^{0})", 100,-5,15,"L");

  addHist("B_plus_LOGIPCHI2", "Log Min IP #chi^{2} (B^{+})", 100,-5,15,"L");
  addHist("B_plus_DIRA_OWNPV",   "DIRA (B^{+})", 100, 0.999,1.0,"L");

  addHist("B_plus_LOGENDVERTEX_CHI2", "Log vtx #chi^{2} (B^{+})", 100, -10, 2, "L");
  addHist("D0_LOGENDVERTEX_CHI2", "Log vtx #chi^{2} (D^{0})", 100, -10, 2, "L");

  addHist("B_plus_ENDVERTEX_CHI2", "Vtx #chi^{2} (B^{+})", 100, 0, 8, "L");
  addHist("D0_ENDVERTEX_CHI2", "Vtx #chi^{2} (D^{0})", 100, 0, 8, "L");

  addHist("K_minus_PIDK", "PIDK (K^{-})",   100, 0, 200, "R");
  addHist("Pi_plus_PIDK", "PIDK (#pi^{+})", 100, -200, 0, "L");
  addHist("K_minus_ProbNNK" , "NNK (K^{-})",    100, 0, 1, "R");
  addHist("Pi_plus_ProbNNK" , "NNK (#pi^{+})",  100, 0, 1, "L");
  addHist("K_minus_ProbNNKpi" , "NNKpi (K^{-})",    100, 0, 1, "R");
  addHist("Pi_plus_ProbNNpiK" , "NNpiK (#pi^{+})",  100, 0, 1, "L");
  addHist("K_minus_ProbNNpi" , "NNpi (K^{-})",    100, 0, 1, "R");
  addHist("Pi_plus_ProbNNpi" , "NNpi (#pi^{+})",  100, 0, 1, "L");
  addHist("K_minus_ProbNNghost" , "ProbNNghost (K^{-})",    100, 0, 1, "R");
  addHist("Pi_plus_ProbNNghost" , "ProbNNghost (#pi^{+})",  100, 0, 1, "L");

  addHist("K_minus_MIPCHI2PV", "MIN IP #chi^{2} (K^{-})"    , 100, 0, 150, "R");
  addHist("Pi_plus_MIPCHI2PV", "MIN IP #chi^{2} (#pi^{+})"  , 100, 0, 150, "R");
  addHist("Mu_plus_MIPCHI2PV", "MIN IP #chi^{2} (#mu^{+})"  , 100, 0, 200, "R");

  addHist("D0_PT",   "p_{T}(D^{0}) [MeV/c]", 100, 0, 10000, "R");

  addHist("nCandidate", "N_{C}", 20, 0, 20, "R");
}

void Bc2Dmunu::Plotter::defineDrawingConfig(){

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

  addDrawOpt("data",   "Data", 70);
  setDrawOptDefaultPoint(kBlack);

  //addResidOpt(make_pair(3,0));
  //addResidOpt(make_pair(4,0));
  //setResidType(1);
  // -------------------------------------------- //

}

bool Bc2Dmunu::Plotter::fillHistograms(){

  // fill hists now
  fillHist("B_plus_M"     , v->B_plus_M      );
  fillHist("B_plus_MCORR" , v->B_plus_MCORR  );
  fillHist("B_plus_LTIME"   , v->B_plus_LTIME    );
  fillHist("D0_M"        , v->D0_M         );
  fillHist("D0_PT"       , v->D0_PT        );
  fillHist("D0_LOGIPCHI2", TMath::Log(v->D0_IPCHI2_OWNPV) );
  fillHist("B_plus_LOGIPCHI2", TMath::Log(v->B_plus_IPCHI2_OWNPV) );
  fillHist("B_plus_DIRA_OWNPV", v->B_plus_DIRA_OWNPV );
  fillHist("B_plus_LOGENDVERTEX_CHI2", TMath::Log( v->B_plus_ENDVERTEX_CHI2 ) );
  fillHist("D0_LOGENDVERTEX_CHI2", TMath::Log( v->D0_ENDVERTEX_CHI2 ) );
  fillHist("B_plus_ENDVERTEX_CHI2",  v->B_plus_ENDVERTEX_CHI2 ) ;
  fillHist("D0_ENDVERTEX_CHI2",  v->D0_ENDVERTEX_CHI2 ) ;

  fillHist("K_minus_PIDK"  , v->K_minus_PIDK );
  fillHist("Pi_plus_PIDK"  , v->Pi_plus_PIDK );
  fillHist("K_minus_ProbNNK"   , v->K_minus_ProbNNk  );
  fillHist("Pi_plus_ProbNNK"   , v->Pi_plus_ProbNNk  );
  fillHist("K_minus_ProbNNKpi" , v->K_minus_ProbNNk*(1.-v->K_minus_ProbNNpi) );
  fillHist("Pi_plus_ProbNNpiK" , v->Pi_plus_ProbNNpi*(1.-v->Pi_plus_ProbNNk) );
  fillHist("K_minus_ProbNNpi"  , v->K_minus_ProbNNpi );
  fillHist("Pi_plus_ProbNNpi"  , v->Pi_plus_ProbNNpi );
  fillHist("K_minus_ProbNNghost" , v->K_minus_ProbNNghost);
  fillHist("Pi_plus_ProbNNghost" , v->Pi_plus_ProbNNghost);
  fillHist("K_minus_MIPCHI2PV", v->K_minus_MIPCHI2PV);
  fillHist("Pi_plus_MIPCHI2PV", v->Pi_plus_MIPCHI2PV);
  fillHist("Mu_plus_MIPCHI2PV", v->Mu_plus_MIPCHI2PV);

  return true;
}

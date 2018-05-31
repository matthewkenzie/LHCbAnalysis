#include "ComparisonPlotter.h"
#include "TMath.h"

using namespace std;
using namespace TMath;

Bc2Dmunu::ComparisonPlotter::ComparisonPlotter(TString _name, const Variables_Analysis *_v):
  PlotterBase(_name,_v),
  v(_v),
  split(false)
{
  normalise = true;
  normalisation = 1.;
  outfilename = Form("root/%sOut.root",_name.Data());
}

Bc2Dmunu::ComparisonPlotter::~ComparisonPlotter(){}

void Bc2Dmunu::ComparisonPlotter::defineHistograms(){

  // add these histograms
  //

  addHist("B_plus_M"    , "m(B^{+}) [MeV]", 100, 2000,8000, "R");
  addHist("B_plus_MCORR", "m_{corr}(B^{+}) [MeV]", 100,2000,8000, "R");
  addHist("B_plus_MCORRERR", "#sigma(m_{corr})(B^{+}) [MeV]", 100, 0, 3000, "R");
  addHist("B_plus_MCORRERR_perMCORR", "#sigma(m_{corr})/m_{corr}(B^{+}) ", 100, 0, 1, "R");
  addHist("B_plus_PT",  "p_{T}(B^{+}) [MeV]", 100, 0, 50000, "R");
  addHist("B_plus_LTIME",   "#tau(B^{+}) [ns]", 100,0,0.01,"R");
  addHist("D0_M"       , "m(D^{0}) [MeV]", 100, 1800, 1950, "R");
  addHist("D0_LOGIPCHI2", "Log Min IP #chi^{2} (D^{0})", 100,-5,15,"L");

  addHist("B_plus_LOGIPCHI2", "Log Min IP #chi^{2} (B^{+})", 100,-5,15,"L");
  addHist("B_plus_DIRA_OWNPV",   "DIRA (B^{+})", 100, 0.999,1.0,"L");
  addHist("B_plus_DIRA_ACOSOWNPV",   "ArcCos DIRA (B^{+})", 100, 0.,0.1,"R");

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

  addHist("K_minus_MIPCHI2PV", "MIN IP #chi^{2} (K^{-})"    , 100, 0, 200, "R");
  addHist("Pi_plus_MIPCHI2PV", "MIN IP #chi^{2} (#pi^{+})"  , 100, 0, 200, "R");
  addHist("Mu_plus_MIPCHI2PV", "MIN IP #chi^{2} (#mu^{+})"  , 100, 0, 400, "R");

  addHist("K_minus_LOGIPCHI2PV", "Log MIN IP #chi^{2} (K^{-})"    , 100, -5, 15, "R");
  addHist("Pi_plus_LOGIPCHI2PV", "Log MIN IP #chi^{2} (#pi^{+})"  , 100, -5, 15, "R");
  addHist("Mu_plus_LOGIPCHI2PV", "Log MIN IP #chi^{2} (#mu^{+})"  , 100, -5, 15, "R");

  addHist("Mu_plus_PT", "p_{T}(#mu^{+})", 100, 0, 20000, "R");

  addHist("D0_PT",   "p_{T}(D^{0}) [MeV/c]", 100, 0, 10000, "R");
  addHist("Log_D0_PT",   "Log(p_{T})(D^{0}) [MeV/c]", 100, 0, 20, "R");

//added here beyond default

//  addHist("B_plus_OWNPV_CHI2", "OwnPV #chi^{2} (B^{+})", 100,-5,100,"L");
//  addHist("B_plus_LOGOWNPV_CHI2", "Log Own PV #chi^{2} (B^{+})", 100,-5,15,"L");
//  addHist("B_plus_PT",   "p_{T}(B^{+}) [MeV/c]", 100, 0, 30000, "R");
  //addHist("B_plus_ISOLATION_BDT", "BDT(B^{+})", 100, -2.5, 1, "R");
  //addHist("B_plus_ISOLATION_BDT2", "BDT(B^{+})", 100, -2.5, 1, "R");
  //addHist("B_plus_ISOLATION_BDT3", "BDT(B^{+})", 100, -2.5, 1, "R");
//  addHist("D0_OWNPV_CHI2", "OwnPV #chi^{2} (D^{0})", 100,-5,100,"L");
//  addHist("D0_LOGOWNPV_CHI2", "Log Own PV #chi^{2} (D^{0})", 100,-5,15,"L");
//  addHist("D0_DIRA_OWNPV",   "DIRA (D^{0})", 100, 0.999,1.0,"L");

}

void Bc2Dmunu::ComparisonPlotter::defineDrawingConfig(){

  // add these drawing options
  // -------------------------------------------- //
  // -------------------------------------------- //
  TColor *blueFill = gROOT->GetColor(kBlue-7);
  blueFill->SetAlpha(0.4);
  TColor *redFill = gROOT->GetColor(kRed-7);
  redFill->SetAlpha(0.4);
  TColor *greenFill = gROOT->GetColor(kGreen-3);
  greenFill->SetAlpha(0.4);

  addDrawOpt("mc_Bu",   "MC B^{+}#rightarrow D^{0}#mu#nu", -29);
  setDrawOptDefaultFill(blueFill->GetNumber());

  addDrawOpt("mc_Bc",   "MC B^{+}_{c}#rightarrow D^{0}#mu#nu", -20);
  setDrawOptDefaultFill(redFill->GetNumber());

  addDrawOpt("data_comb", "Combinatorial", 11, 12);
  setDrawOptDefaultPoint(kBlack);

  //addResidOpt(make_pair(2,0));
  //addResidOpt(make_pair(2,1));
  //setResidType(1);
  // -------------------------------------------- //

}

bool Bc2Dmunu::ComparisonPlotter::fillHistograms(){

  if ( v->itype > 0 && v->D0_M>1840 && v->D0_M<1900 && (v->K_minus_ID*v->Mu_plus_ID)>0  ) return true;

  // fill hists now
  fillHist("B_plus_M"     , v->B_plus_M      );
  fillHist("B_plus_MCORR" , v->B_plus_MCORR  );
  fillHist("B_plus_MCORRERR" , v->B_plus_MCORRERR  );
  fillHist("B_plus_MCORRERR_perMCORR" , v->B_plus_MCORRERR/v->B_plus_MCORR  );
  fillHist("B_plus_PT" , v->B_plus_PT );
  fillHist("B_plus_LTIME"   , v->B_plus_LTIME    );
  fillHist("D0_M"        , v->D0_M         );
  fillHist("D0_PT"       , v->D0_PT        );
  fillHist("Log_D0_PT"       , TMath::Log(v->D0_PT)        );
  fillHist("D0_LOGIPCHI2", TMath::Log(v->D0_IPCHI2_OWNPV) );
  fillHist("B_plus_LOGIPCHI2", TMath::Log(v->B_plus_IPCHI2_OWNPV) );
  fillHist("B_plus_DIRA_OWNPV", v->B_plus_DIRA_OWNPV );
  fillHist("B_plus_DIRA_ACOSOWNPV", TMath::ACos(v->B_plus_DIRA_OWNPV) );
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
  fillHist("K_minus_LOGIPCHI2PV", TMath::Log(v->K_minus_MIPCHI2PV));
  fillHist("Pi_plus_LOGIPCHI2PV", TMath::Log(v->Pi_plus_MIPCHI2PV));
  fillHist("Mu_plus_LOGIPCHI2PV", TMath::Log(v->Mu_plus_MIPCHI2PV));
  fillHist("Mu_plus_PT"       , v->Mu_plus_PT );

  return true;
}


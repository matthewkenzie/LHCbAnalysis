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

  addHist("Bplus_M"    , "m(B^{+}) [MeV]", 100, 2000,8000, "R");
  addHist("Bplus_Mcorr", "m_{corr}(B^{+}) [MeV]", 100,2000,8000, "R");
  addHist("Bplus_TAU",   "#tau(B^{+}) [ns]", 100,0,0.01,"R");
  addHist("D0_M"       , "m(D^{0}) [MeV]", 100, 1800, 1950, "R");
  addHist("D0_LOGMINIPCHI2", "Log Min IP #chi^{2} (D^{0})", 100,-5,15,"L");

  addHist("Bplus_LOGMINIPCHI2", "Log Min IP #chi^{2} (B^{+})", 100,-5,15,"L");
  addHist("Bplus_DIRA_OWNPV",   "DIRA (B^{+})", 100, 0.999,1.0,"L");

  addHist("Bplus_LOGENDVERTEX_CHI2", "Log vtx #chi^{2} (B^{+})", 100, -10, 2, "L");
  addHist("D0_LOGENDVERTEX_CHI2", "Log vtx #chi^{2} (D^{0})", 100, -10, 2, "L");

  addHist("Bplus_ENDVERTEX_CHI2", "Vtx #chi^{2} (B^{+})", 100, 0, 8, "L");
  addHist("D0_ENDVERTEX_CHI2", "Vtx #chi^{2} (D^{0})", 100, 0, 8, "L");

  addHist("Kminus_PIDK", "PIDK (K^{-})",   100, 0, 200, "R");
  addHist("piplus_PIDK", "PIDK (#pi^{+})", 100, -200, 0, "L");
  addHist("Kminus_NNK" , "NNK (K^{-})",    100, 0, 1, "R");
  addHist("piplus_NNK" , "NNK (#pi^{+})",  100, 0, 1, "L");
  addHist("Kminus_NNKpi" , "NNKpi (K^{-})",    100, 0, 1, "R");
  addHist("piplus_NNKpi" , "NNKpi (#pi^{+})",  100, 0, 1, "L");
  addHist("Kminus_NNpi" , "NNpi (K^{-})",    100, 0, 1, "R");
  addHist("piplus_NNpi" , "NNpi (#pi^{+})",  100, 0, 1, "L");
  addHist("Kminus_ghost" , "ghost (K^{-})",    100, 0, 1, "R");
  addHist("piplus_ghost" , "ghost (#pi^{+})",  100, 0, 1, "L");

  addHist("Kminus_MINIPCHI2", "MIN IP #chi^{2} (K^{-})"    , 100, 0, 150, "R");
  addHist("piplus_MINIPCHI2", "MIN IP #chi^{2} (#pi^{+})"  , 100, 0, 150, "R");
  addHist("muplus_MINIPCHI2", "MIN IP #chi^{2} (#mu^{+})"  , 100, 0, 200, "R");

  addHist("D0_PT",   "p_{T}(D^{0}) [MeV/c]", 100, 0, 10000, "R");

  addHist("bu_rejection_bdtoutput", "BDT", 100, -1, 1., "R");

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

  addDrawOpt("mc_Bu",   "MC B^{+}#rightarrow D^{0}#mu#nu", -82);
  setDrawOptDefaultFill(blueFill->GetNumber());

  addDrawOpt("mc_Bc",   "MC B^{+}_{c}#rightarrow D^{0}#mu#nu", -80);
  setDrawOptDefaultFill(redFill->GetNumber());

  addDrawOpt("mc_Bc_Dst", "MC B^{+}_{c}#rightarrow D^{*}#mu#nu", -81);
  setDrawOptDefaultFill(greenFill->GetNumber());

  addDrawOpt("data_fav",   "Data (fav)", 80);
  setDrawOptDefaultPoint(kBlack);

  addDrawOpt("data_sup",   "Data (sup)", 81);
  setDrawOptDefaultPoint(kBlue);

  addResidOpt(make_pair(3,0));
  addResidOpt(make_pair(4,0));
  setResidType(1);
  // -------------------------------------------- //

}

bool Bc2Dmunu::Plotter::fillHistograms(){

  // fill hists now
  fillHist("Bplus_M"     , v->Bplus_M      );
  fillHist("Bplus_Mcorr" , v->Bplus_Mcorr  );
  fillHist("Bplus_TAU"   , v->Bplus_TAU    );
  fillHist("D0_M"        , v->D0_M         );
  fillHist("D0_PT"       , v->D0_PT        );
  fillHist("D0_LOGMINIPCHI2", TMath::Log(v->D0_MINIPCHI2) );
  fillHist("Bplus_LOGMINIPCHI2", TMath::Log(v->Bplus_MINIPCHI2) );
  fillHist("Bplus_DIRA_OWNPV", v->Bplus_DIRA_OWNPV );
  fillHist("Bplus_LOGENDVERTEX_CHI2", TMath::Log( v->Bplus_ENDVERTEX_CHI2 ) );
  fillHist("D0_LOGENDVERTEX_CHI2", TMath::Log( v->D0_ENDVERTEX_CHI2 ) );
  fillHist("Bplus_ENDVERTEX_CHI2",  v->Bplus_ENDVERTEX_CHI2 ) ;
  fillHist("D0_ENDVERTEX_CHI2",  v->D0_ENDVERTEX_CHI2 ) ;

  fillHist("Kminus_PIDK"  , v->Kminus_PIDK );
  fillHist("piplus_PIDK"  , v->piplus_PIDK );
  fillHist("Kminus_NNK"   , v->Kminus_NNK  );
  fillHist("piplus_NNK"   , v->piplus_NNK  );
  fillHist("Kminus_NNKpi" , v->Kminus_NNKpi);
  fillHist("piplus_NNKpi" , v->piplus_NNKpi);
  fillHist("Kminus_NNpi"  , v->Kminus_NNpi );
  fillHist("piplus_NNpi"  , v->piplus_NNpi );
  fillHist("Kminus_ghost" , v->Kminus_ghost);
  fillHist("piplus_ghost" , v->piplus_ghost);
  fillHist("Kminus_MINIPCHI2", v->Kminus_MINIPCHI2);
  fillHist("piplus_MINIPCHI2", v->piplus_MINIPCHI2);
  fillHist("muplus_MINIPCHI2", v->muplus_MINIPCHI2);

  fillHist("bu_rejection_bdtoutput", v->bu_rejection_bdtoutput);
  fillHist("nCandidate", int(v->nCandidate) );
  return true;
}

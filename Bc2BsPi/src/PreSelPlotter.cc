#include "PreSelPlotter.h"
#include "TMath.h"

using namespace std;
using namespace TMath;

Bc2BsPi::PreSelPlotter::PreSelPlotter(TString _name, const Variables_Analysis *_v):
  PlotterBase(_name,_v),
  v(_v),
  split(false)
{
  normalise = true;
  normalisation = 1.;
  outfilename = Form("root/%sOut.root",_name.Data());
}

Bc2BsPi::PreSelPlotter::~PreSelPlotter(){}

void Bc2BsPi::PreSelPlotter::defineHistograms(){

  // add these histograms
  //

  addHist("B_c_M"                   , "B_c_M"                  , 100, 5800, 6600, "R");
  addHist("B_c_MINIPCHI2"           , "B_c_MINIPCHI2"          , 100,0,20, "R");
  addHist("B_c_ENDVERTEX_CHI2NDOF"  , "B_c_ENDVERTEX_CHI2NDOF" , 100, 0, 6, "R");
  addHist("B_c_FDCHI2_OWNPV"        , "B_c_FDCHI2_OWNPV"       , 100, 0, 100, "R");
  addHist("B_c_DIRA_OWNPV"          , "B_c_DIRA_OWNPV"         , 100, 0.99,1, "R");
  addHist("B_c_P"                   , "B_c_P"                  , 100, 0, 400000, "R");
  addHist("B_c_PT"                  , "B_c_PT"                 , 100, 0, 30000, "R");
  addHist("B_c_B_s0_VTX_DIFF_R"     , "B_c_B_s0_VTX_DIFF_R"    , 100, 0, 100, "R");
  addHist("B_c_B_s0_VTX_DIFF_Z"     , "B_c_B_s0_VTX_DIFF_Z"    , 100, -50, 50, "R");

  addHist("B_s0_M"                  , "B_s0_M"                 , 100, 5200, 5500, "R");
  addHist("B_s0_P"                  , "B_s0_P"                 , 100, 0, 400000, "R");
  addHist("B_s0_PT"                 , "B_s0_PT"                , 100, 0, 30000, "R");

  addHist("Pibach_P"                , "Pibach_P"               , 100,0, 100000, "R");
  addHist("Pibach_PT"               , "Pibach_PT"              , 100,0,5000, "R");
  addHist("Pibach_PIDK"             , "Pibach_PIDK"            , 100, -100, 100, "R");
  addHist("Pibach_ProbNNk"          , "Pibach_ProbNNk"         , 100, 0, 1 , "R");
  addHist("Pibach_ProbNNpi"         , "Pibach_ProbNNpi"        , 100, 0, 1 , "L");
  addHist("Pibach_ProbNNpiK"        , "Pibach_ProbNNpiK"       , 100, 0, 1 , "L");
  addHist("Pibach_MIPCHI2PV"        , "Pibach_MIPCHI2PV"       , 100, 0, 50, "R");

  addHist("nCandidate"              , "N_{C}"                  , 50, 0, 50, "R");
}

void Bc2BsPi::PreSelPlotter::defineDrawingConfig(){

  // add these drawing options
  // -------------------------------------------- //
  // -------------------------------------------- //
  TColor *blueFill = gROOT->GetColor(kBlue-7);
  blueFill->SetAlpha(0.4);
  TColor *redFill = gROOT->GetColor(kRed-7);
  redFill->SetAlpha(0.4);
  TColor *greenFill = gROOT->GetColor(kGreen-3);
  greenFill->SetAlpha(0.4);

  addDrawOpt("mc_Bc",   "MC B_{c}^{+}#rightarrow[B_{s}^{0}#rightarrowJ/#psi#phi]#pi^{+}", -80);
  setDrawOptDefaultFill(blueFill->GetNumber());

  addDrawOpt("mc_Bs",   "MC B_{s}^{0}#rightarrowJ/#psi#phi", -81);
  setDrawOptDefaultFill(redFill->GetNumber());

  addDrawOpt("data",   "Data Run 1", 70, 80 );
  setDrawOptDefaultPoint(kBlack);

  addDrawOpt("data",   "Data Run 2", 90, 100 );
  setDrawOptPoint(kBlue,kOpenCircle);

  //addResidOpt(make_pair(3,0));
  //addResidOpt(make_pair(4,0));
  //setResidType(1);
  // -------------------------------------------- //

}

bool Bc2BsPi::PreSelPlotter::fillHistograms(){

  // fill hists now
  fillHist("B_c_M"                   , v->B_c_M );
  fillHist("B_c_MINIPCHI2"           , v->B_c_MINIPCHI2 );
  fillHist("B_c_ENDVERTEX_CHI2NDOF"  , v->B_c_ENDVERTEX_CHI2/v->B_c_ENDVERTEX_NDOF );
  fillHist("B_c_FDCHI2_OWNPV"        , v->B_c_FDCHI2_OWNPV );
  fillHist("B_c_DIRA_OWNPV"          , v->B_c_DIRA_OWNPV );
  fillHist("B_c_P"                   , v->B_c_P );
  fillHist("B_c_PT"                  , v->B_c_PT );
  fillHist("B_c_B_s0_VTX_DIFF_R"     , v->B_c_B_s0_VTX_DIFF_R );
  fillHist("B_c_B_s0_VTX_DIFF_Z"     , v->B_c_B_s0_VTX_DIFF_Z );

  fillHist("B_s0_M"                  , v->B_s0_M );
  fillHist("B_s0_P"                  , v->B_s0_P );
  fillHist("B_s0_PT"                 , v->B_s0_PT );

  fillHist("Pibach_P"                , v->Pibach_P );
  fillHist("Pibach_PT"               , v->Pibach_PT );
  fillHist("Pibach_PIDK"             , v->Pibach_PIDK );
  fillHist("Pibach_ProbNNk"          , v->Pibach_ProbNNk );
  fillHist("Pibach_ProbNNpi"         , v->Pibach_ProbNNpi );
  fillHist("Pibach_ProbNNpiK"        , v->Pibach_ProbNNpi*(1.-v->Pibach_ProbNNk) );
  fillHist("Pibach_MIPCHI2PV"        , v->Pibach_MIPCHI2PV );

  fillHist("nCandidate"              , int(v->nCandidate) );

  return true;
}

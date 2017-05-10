#include "PIDVariablesPlotter.h"
#include "TMath.h"
#include "TLorentzVector.h"

using namespace std;
using namespace TMath;

Bs2KstKst::PIDVariablesPlotter::PIDVariablesPlotter(TString _name, const Variables_Analysis *_v):
  PlotterBase(_name,_v),
  v(_v),
  split(false)
{
  normalise = true;
  normalisation = 1.;
  system(Form("mkdir -p root/%s",_name.Data()));
  outfilename = Form("root/%s/%sOut.root",_name.Data(),_name.Data());
}

Bs2KstKst::PIDVariablesPlotter::~PIDVariablesPlotter(){}

void Bs2KstKst::PIDVariablesPlotter::defineDrawingConfig(){

  // add these drawing options
  // -------------------------------------------- //
  // split years same plot
  TColor *blueFill = gROOT->GetColor(kBlue-4);
  blueFill->SetAlpha(0.4);
  TColor *redFill = gROOT->GetColor(kRed-4);
  redFill->SetAlpha(0.4);
  TColor *greenFill = gROOT->GetColor(kGreen-3);
  greenFill->SetAlpha(0.4);

  addDrawOpt("Bs2Kst0Kst0", "B_{s}^{0}#rightarrowK^{*0}K^{*0} MC", -70, -80 );
  setDrawOptDefaultFill(blueFill->GetNumber());

  //addDrawOpt("Bs2KpiKpiPS", "B_{s}^{0}#rightarrow(K^{+}#pi^{-})(K^{-}#pi^{+}) PS MC", -73, -83 );
  //setDrawOptDefaultFill(redFill->GetNumber());

  addDrawOpt("Bd2RhoKst0", "B_{d}^{0}#rightarrow#rho^{0}K^{*0} MC", -77, -87 );
  setDrawOptDefaultFill(redFill->GetNumber());

  addDrawOpt("Bd2PhiKst0", "B_{d}^{0}#rightarrow#phi K^{*0} MC", -75, -85 );
  setDrawOptDefaultFill(greenFill->GetNumber());

  addDrawOpt("SidebandData", "Data (sidebands)", 71, 81);
  setDrawOptDefaultLine(kBlack);

}

void Bs2KstKst::PIDVariablesPlotter::defineHistograms(){

  // add these histograms
  //
  addHist("Kplus_ProbNNk"   , "Kplus_ProbNNk", 50, 0, 1.);
  addHist("Kminus_ProbNNk"  , "Kminus_ProbNNk", 50, 0, 1.);
  addHist("Piplus_ProbNNk"  , "Piplus_ProbNNk", 50, 0, 1.);
  addHist("Piminus_ProbNNk" , "Piminus_ProbNNk", 50, 0, 1.);

  addHist("Kplus_ProbNNpi"   , "Kplus_ProbNNpi", 50, 0, 1.);
  addHist("Kminus_ProbNNpi"  , "Kminus_ProbNNpi", 50, 0, 1.);
  addHist("Piplus_ProbNNpi"  , "Piplus_ProbNNpi", 50, 0, 1.);
  addHist("Piminus_ProbNNpi" , "Piminus_ProbNNpi", 50, 0, 1.);

  addHist("Kplus_ProbNNKpi"  , "Kplus_ProbNNKpi", 50, 0, 1.);
  addHist("Kminus_ProbNNKpi" , "Kminus_ProbNNKpi", 50, 0, 1.);
  addHist("Piplus_ProbNNpiK" , "Piplus_ProbNNpiK", 50, 0, 1.);
  addHist("Piminus_ProbNNpiK", "Piminus_ProbNNpiK", 50, 0, 1.);

  addHist("min_kaon_ProbNNk", "min_kaon_ProbNNk", 50, 0, 1.);
  addHist("min_kaon_ProbNNKpi", "min_kaon_ProbNNKpi", 50, 0, 1.);
  addHist("min_pion_ProbNNpi", "min_pion_ProbNNpi", 50, 0, 1.);
  addHist("min_pion_ProbNNpiK", "min_pion_ProbNNpiK", 50, 0, 1.);
}

bool Bs2KstKst::PIDVariablesPlotter::fillHistograms(){

  // don't fill if they don't pass the cuts
  //if ( !v->pass_bdt ) return true;
  //if ( !v->pass_pid ) return true;
  //if ( v->pass_rhokst ) return true;
  //if ( v->pass_massveto ) return true;

  // fill hists now
  fillHist("Kplus_ProbNNk"     , v->Kplus_ProbNNk      );
  fillHist("Kminus_ProbNNk"    , v->Kminus_ProbNNk     );
  fillHist("Piplus_ProbNNk"    , v->Piplus_ProbNNk     );
  fillHist("Piminus_ProbNNk"   , v->Piminus_ProbNNk    );

  fillHist("Kplus_ProbNNpi"    , v->Kplus_ProbNNpi     );
  fillHist("Kminus_ProbNNpi"   , v->Kminus_ProbNNpi    );
  fillHist("Piplus_ProbNNpi"   , v->Piplus_ProbNNpi    );
  fillHist("Piminus_ProbNNpi"  , v->Piminus_ProbNNpi   );

  fillHist("Kplus_ProbNNKpi"   , v->Kplus_ProbNNKpi    );
  fillHist("Kminus_ProbNNKpi"  , v->Kminus_ProbNNKpi   );
  fillHist("Piplus_ProbNNpiK"  , v->Piplus_ProbNNpiK   );
  fillHist("Piminus_ProbNNpiK" , v->Piminus_ProbNNpiK  );

  fillHist("min_kaon_ProbNNk"  , v->min_kaon_ProbNNk   );
  fillHist("min_kaon_ProbNNKpi", v->min_kaon_ProbNNKpi );
  fillHist("min_pion_ProbNNpi" , v->min_pion_ProbNNpi  );
  fillHist("min_pion_ProbNNpiK", v->min_pion_ProbNNpiK );

  return true;
}

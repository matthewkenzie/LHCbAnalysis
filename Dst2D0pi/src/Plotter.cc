#include "Plotter.h"
#include "TMath.h"

using namespace std;
using namespace TMath;

Dst2D0pi::Plotter::Plotter(TString _name, const Variables_Analysis *_v):
  PlotterBase(_name,_v),
  v(_v)
{
  normalise = true;
  normalisation = 1.;
  outfilename = Form("root/%sOut.root",_name.Data());
}

Dst2D0pi::Plotter::~Plotter(){}

void Dst2D0pi::Plotter::defineHistograms(){

  // add these histograms
  //
    
  addHist("mDz"            , "m(D^{0}) [GeV/c^{2}]",  150,  1.8, 1.95, "R" );
  addHist("mDst"           , "m(D^{#star+}) [GeV/c^{2}]", 100, 1.9, 2.1, "R" );
  addHist("dm"             , "m(D^{#star+}) [GeV/c^{2}]", 100, 0.12, 0.16, "R" );
  addHist("Dst_Vtx_dist"   , "dist(D^{#star+})", 100, -100, 150, "R" );
}

void Dst2D0pi::Plotter::defineDrawingConfig(){

  // add these drawing options
  // -------------------------------------------- //
  // -------------------------------------------- //
  TColor *blueFill = gROOT->GetColor(kBlue-7);
  blueFill->SetAlpha(0.4);
 
  addDrawOpt("MC",  "MC D^{#star+}#rightarrow D^{0}#pi^{+}", -130);
  setDrawOptDefaultFill(blueFill->GetNumber());

  addDrawOpt("Data",   "Data", 130);
  setDrawOptDefaultPoint(kBlack);

  addResidOpt(make_pair(1,0));
  setResidType(1);
  // -------------------------------------------- //

}

bool Dst2D0pi::Plotter::fillHistograms(){

  // fill hists now
  fillHist("mDz"            , v->mDz  );
  fillHist("mDst"           , v->mDst );
  fillHist("dm"             , v->dm   );
  fillHist("Dst_Vtx_dist"   , v->Dst_Vtx_dist );
  
  return true;
}

#ifndef StandAlonePlotter_h
#define StandAlonePlotter_h

#include <iostream>
#include <vector>

#include "TString.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TROOT.h"
#include "TMath.h"
#include "TColor.h"
#include "TH1F.h"

#include "RooWorkspace.h"
#include "RooArgList.h"
#include "RooHist.h"

#include "FitterUtils.h"

class PlotComponent;
class DrawBox;

namespace Dst2D0pi{

  class StandAlonePlotter {

    public:

      StandAlonePlotter( RooWorkspace *_w, TString _pName="StandAlonePlotter" );
      ~StandAlonePlotter();

      void drawBox( double xmin, double xmax, int color );

      void plot( TString var, std::vector<PlotComponent> plotComps, TString fname, const RooArgList *params=NULL, double xlow=-1., double xhigh=-1. );

      TString pName;
      int pResidType;
      TString pTitle;
      double pTitleX;
      double pBoxX;
      bool pDrawLog;
      int pRedPull;

    private:

      RooWorkspace *w;

      std::vector<DrawBox> boxes;

      std::vector<TCanvas*> canvs;
      TCanvas *createCanvas(int w=800, int h=600);
  };

}

#endif

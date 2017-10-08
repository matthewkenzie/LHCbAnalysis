#ifndef Bs2KstKst_MassFitPlotter_h
#define Bs2KstKst_MassFitPlotter_h

#include <iostream>

#include "TString.h"
#include "TLegend.h"

#include "RooWorkspace.h"
#include "RooPlot.h"
#include "RooArgList.h"

namespace Bs2KstKst {

  class MassFitPlotter {

    public:

      MassFitPlotter(RooWorkspace *_w, TString _outdirname="MassFitPlotter");
      ~MassFitPlotter();

      void makePlot( RooPlot *plot, TString fname, int resid=0, TLegend *leg=0, TString pTitle="", int pRedPull=-1, const RooArgList *params=0 );

      void plot( TString var, TString data, TString pdf="", int resid=2, bool project=false );
      void makeDataPlot( TString outfName, TString dsetName="DataCombined", TString catName="All" );
      void makeBkgPlot( TString outfName );

    private:

      RooWorkspace *w;
      TString outdirname;
  };

}

#endif

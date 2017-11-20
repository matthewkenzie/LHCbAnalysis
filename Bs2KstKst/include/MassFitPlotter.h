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
      void makeDataPlot( TString outfName, TString dsetName="DataCombined", TString catName="All", bool paperStyle=false );
      void makeBkgPlot( TString outfName );
      void chooseColorScheme( int i=0 );

      bool titleOnLog=false;
      float logRangeMin=-1.;
      float logRangeMax=-1.;
      TString xtitle = "";
      TString ytitle = "";

    private:

      RooWorkspace *w;
      TString outdirname;
      std::vector<int> colorScheme;
  };

}

#endif

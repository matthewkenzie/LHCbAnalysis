/////////////////////////////////////
//                                 //
// MassVariablesPlotter.h               //
// Author: Matthew Kenzie          //
// Auto-generated                  //
// Will run the analysis chain     //
//                                 //
/////////////////////////////////////

#ifndef Bs2KstKst_MassVariablesPlotter_h
#define Bs2KstKst_MassVariablesPlotter_h

#include <iostream>
#include <vector>
#include <map>

#include "TString.h"
#include "TH1F.h"

#include "Analyser.h"
#include "PlotterBase.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class MassVariablesPlotter : public PlotterBase {

    public:

      MassVariablesPlotter(TString _name, const Variables_Analysis *_v);
      ~MassVariablesPlotter();

      void defineHistograms();
      void defineDrawingConfig();
      bool fillHistograms();

      const Variables_Analysis *v;

      bool split;

  };

}

#endif

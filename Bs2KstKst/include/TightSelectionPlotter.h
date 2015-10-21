/////////////////////////////////////
//                                 //
// TightSelectionPlotter.h               //
// Author: Matthew Kenzie          //
// Auto-generated                  //
// Will run the analysis chain     //
//                                 //
/////////////////////////////////////

#ifndef Bs2KstKst_TightSelectionPlotter_h
#define Bs2KstKst_TightSelectionPlotter_h

#include <iostream>
#include <vector>
#include <map>

#include "TString.h"
#include "TH1F.h"

#include "Analyser.h"
#include "PlotterBase.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class TightSelectionPlotter : public PlotterBase {

    public:

      TightSelectionPlotter(TString _name, const Variables_Analysis *_v);
      ~TightSelectionPlotter();

      void defineHistograms();
      void defineDrawingConfig();
      bool fillHistograms();

      const Variables_Analysis *v;

      bool split;

  };

}

#endif

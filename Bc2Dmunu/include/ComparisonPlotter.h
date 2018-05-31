////////////////////////////////////
//                                 //
// ComparisonPlotter.h               //
// Author: Matthew Kenzie          //
// Auto-generated                  //
// Will run the analysis chain     //
//                                 //
/////////////////////////////////////

#ifndef Bc2Dmunu_ComparisonPlotter_h
#define Bc2Dmunu_ComparisonPlotter_h

#include <iostream>
#include <vector>
#include <map>

#include "TString.h"
#include "TH1F.h"

#include "Analyser.h"
#include "PlotterBase.h"
#include "Variables_Analysis.h"

namespace Bc2Dmunu {

  class ComparisonPlotter : public PlotterBase {

    public:

      ComparisonPlotter(TString _name, const Variables_Analysis *_v);
      ~ComparisonPlotter();

      void defineHistograms();
      void defineDrawingConfig();
      bool fillHistograms();

      const Variables_Analysis *v;

      bool split;

  };

}

#endif

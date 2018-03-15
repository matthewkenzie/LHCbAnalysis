////////////////////////////////////
//                                 //
// AnalysisPlotter.h               //
// Author: Matthew Kenzie          //
// Auto-generated                  //
// Will run the analysis chain     //
//                                 //
/////////////////////////////////////

#ifndef Bc2Dmunu_AnalysisPlotter_h
#define Bc2Dmunu_AnalysisPlotter_h

#include <iostream>
#include <vector>
#include <map>

#include "TString.h"
#include "TH1F.h"

#include "Analyser.h"
#include "PlotterBase.h"
#include "Variables_Analysis.h"

namespace Bc2Dmunu {

  class AnalysisPlotter : public PlotterBase {

    public:

      AnalysisPlotter(TString _name, const Variables_Analysis *_v);
      ~AnalysisPlotter();

      void defineHistograms();
      void defineDrawingConfig();
      bool fillHistograms();

      const Variables_Analysis *v;

      bool split;

  };

}

#endif

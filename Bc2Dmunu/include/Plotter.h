////////////////////////////////////
//                                 //
// Plotter.h               //
// Author: Matthew Kenzie          //
// Auto-generated                  //
// Will run the analysis chain     //
//                                 //
/////////////////////////////////////

#ifndef Bc2Dmunu_Plotter_h
#define Bc2Dmunu_Plotter_h

#include <iostream>
#include <vector>
#include <map>

#include "TString.h"
#include "TH1F.h"

#include "Analyser.h"
#include "PlotterBase.h"
#include "Variables_Analysis.h"

namespace Bc2Dmunu {

  class Plotter : public PlotterBase {

    public:

      Plotter(TString _name, const Variables_Analysis *_v);
      ~Plotter();

      void defineHistograms();
      void defineDrawingConfig();
      bool fillHistograms();

      const Variables_Analysis *v;

      bool split;

  };

}

#endif

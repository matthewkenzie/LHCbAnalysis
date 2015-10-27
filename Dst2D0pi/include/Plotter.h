////////////////////////////////////
//                                 //
// Plotter.h               //
// Author: Matthew Kenzie          //
// Auto-generated                  //
// Will run the analysis chain     //
//                                 //
/////////////////////////////////////

#ifndef Dst2D0pi_Plotter_h
#define Dst2D0pi_Plotter_h

#include <iostream>
#include <vector>
#include <map>

#include "TString.h"
#include "TH1F.h"

#include "Analyser.h"
#include "PlotterBase.h"
#include "Variables_Analysis.h"

namespace Dst2D0pi {

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

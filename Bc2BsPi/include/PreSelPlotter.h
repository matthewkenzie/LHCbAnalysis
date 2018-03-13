////////////////////////////////////
//                                 //
// PreSelPlotter.h               //
// Author: Matthew Kenzie          //
// Auto-generated                  //
// Will run the analysis chain     //
//                                 //
/////////////////////////////////////

#ifndef Bc2BsPi_PreSelPlotter_h
#define Bc2BsPi_PreSelPlotter_h

#include <iostream>
#include <vector>
#include <map>

#include "TString.h"
#include "TH1F.h"

#include "Analyser.h"
#include "PlotterBase.h"
#include "Variables_Analysis.h"

namespace Bc2BsPi {

  class PreSelPlotter : public PlotterBase {

    public:

      PreSelPlotter(TString _name, const Variables_Analysis *_v);
      ~PreSelPlotter();

      void defineHistograms();
      void defineDrawingConfig();
      bool fillHistograms();

      const Variables_Analysis *v;

      bool split;

  };

}

#endif

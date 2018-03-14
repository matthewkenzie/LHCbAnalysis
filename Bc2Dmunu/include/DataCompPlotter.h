////////////////////////////////////
//                                 //
// DataCompPlotter.h               //
// Author: Matthew Kenzie          //
// Auto-generated                  //
// Will run the analysis chain     //
//                                 //
/////////////////////////////////////

#ifndef Bc2Dmunu_DataCompPlotter_h
#define Bc2Dmunu_DataCompPlotter_h

#include <iostream>
#include <vector>
#include <map>

#include "TString.h"
#include "TH1F.h"

#include "Analyser.h"
#include "SkimmingPlotter.h"
#include "Variables_Analysis.h"

namespace Bc2Dmunu {

  class DataCompPlotter : public SkimmingPlotter {

    public:

      DataCompPlotter(TString _name, const Variables_Analysis *_v);
      ~DataCompPlotter();

      void defineDrawingConfig();

  };

}

#endif

#ifndef Dst2D0pi_LifetimeFitter_h
#define Dst2D0pi_LifetimeFitter_h

#include "FitterBase.h"

namespace Dst2D0pi {

  class LifetimeFitter : public FitterBase {

    public:

      LifetimeFitter(TString wsname="w", TString name="LifetimeFitter", bool verbose=false, bool debug=false);
      ~LifetimeFitter();

      void addObsVars();
      void addCuts();
      void addDatasets();
      void constructPdfs();

      void run();
      void makePlots();

  };

}

#endif

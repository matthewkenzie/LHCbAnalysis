#ifndef Bc2Dmunu_TruthMatching_h
#define Bc2Dmunu_TruthMatching_h

#include "Analyser.h"
#include "Variables_Skimming.h"

namespace Bc2Dmunu {

  class TruthMatching : public Analyser {

    public:

      TruthMatching(TString _name, Variables_Skimming *_v);
      ~TruthMatching();

      bool AnalyseEvent();

      Variables_Skimming *v;
  };

}

#endif


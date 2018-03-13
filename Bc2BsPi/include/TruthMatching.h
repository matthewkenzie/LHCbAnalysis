#ifndef Bc2BsPi_TruthMatching_h
#define Bc2BsPi_TruthMatching_h

#include "Analyser.h"
#include "Variables_PreSel.h"

namespace Bc2BsPi {

  class TruthMatching : public Analyser {

    public:

      TruthMatching(TString _name, Variables_PreSel *_v);
      ~TruthMatching();

      bool AnalyseEvent();

      Variables_PreSel *v;
  };

}

#endif


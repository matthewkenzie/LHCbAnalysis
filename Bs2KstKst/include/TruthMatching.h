#ifndef Bs2KstKst_TruthMatching_h
#define Bs2KstKst_TruthMatching_h

#include "Analyser.h"
#include "Variables_PreSel.h"

namespace Bs2KstKst {

  class TruthMatching : public Analyser {

    public:

      TruthMatching(TString _name, Variables_PreSel *_v);
      ~TruthMatching();

      bool AnalyseEvent();

      Variables_PreSel *v;
  };

}

#endif


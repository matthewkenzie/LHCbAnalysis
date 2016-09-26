#ifndef Dst2D0pi_PreSelection_h
#define Dst2D0pi_PreSelection_h

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Dst2D0pi {

  class PreSelection : public Analyser {

    public:

      PreSelection(TString _name, Variables_Analysis *_v);
      ~PreSelection();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif

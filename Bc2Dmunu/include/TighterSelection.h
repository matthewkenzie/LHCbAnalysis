#ifndef Bc2Dmunu_TighterSelection_h
#define Bc2Dmunu_TighterSelection_h

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Bc2Dmunu {

  class TighterSelection : public Analyser {

    public:

      TighterSelection(TString _name, Variables_Analysis *_v);
      ~TighterSelection();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif

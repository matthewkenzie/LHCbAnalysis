#ifndef Bs2KstKst_PreSelection_h
#define Bs2KstKst_PreSelection_h

#include "Analyser.h"
#include "Variables_PreSel.h"

namespace Bs2KstKst {

  class PreSelection : public Analyser {

    public:

      PreSelection(TString _name, Variables_PreSel *_v);
      ~PreSelection();

      bool AnalyseEvent();

      Variables_PreSel *v;

  };

}

#endif

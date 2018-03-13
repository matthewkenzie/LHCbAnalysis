#ifndef Bc2BsPi_PreSelection_h
#define Bc2BsPi_PreSelection_h

#include "Analyser.h"
#include "Variables_PreSel.h"

namespace Bc2BsPi {

  class PreSelection : public Analyser {

    public:

      PreSelection(TString _name, Variables_PreSel *_v);
      ~PreSelection();

      bool AnalyseEvent();

      Variables_PreSel *v;

  };

}

#endif

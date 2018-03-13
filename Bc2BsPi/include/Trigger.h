#ifndef Bc2BsPi_Trigger_h
#define Bc2BsPi_Trigger_h

#include "Analyser.h"
#include "Variables_PreSel.h"

namespace Bc2BsPi {

  class Trigger : public Analyser {

    public:

      Trigger(TString _name, Variables_PreSel *_v);
      ~Trigger();

      bool AnalyseEvent();

      Variables_PreSel *v;

  };

}

#endif

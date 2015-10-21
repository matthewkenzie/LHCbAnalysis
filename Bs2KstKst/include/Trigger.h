#ifndef Bs2KstKst_Trigger_h
#define Bs2KstKst_Trigger_h

#include "Analyser.h"
#include "Variables_PreSel.h"

namespace Bs2KstKst {

  class Trigger : public Analyser {

    public:

      Trigger(TString _name, Variables_PreSel *_v);
      ~Trigger();

      bool AnalyseEvent();

      Variables_PreSel *v;

  };

}

#endif

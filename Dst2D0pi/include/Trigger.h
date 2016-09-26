#ifndef Dst2D0pi_Trigger_h
#define Dst2D0pi_Trigger_h

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Dst2D0pi {

  class Trigger : public Analyser {

    public:

      Trigger(TString _name, Variables_Analysis *_v);
      ~Trigger();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif

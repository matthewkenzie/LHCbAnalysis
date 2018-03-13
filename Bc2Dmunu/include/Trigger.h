#ifndef Bc2Dmunu_Trigger_h
#define Bc2Dmunu_Trigger_h

#include "Analyser.h"
#include "Variables_Skimming.h"

namespace Bc2Dmunu {

  class Trigger : public Analyser {

    public:

      Trigger(TString _name, Variables_Skimming *_v);
      ~Trigger();

      bool AnalyseEvent();

      Variables_Skimming *v;

  };

}

#endif

#ifndef Dst2D0pi_Selection_h
#define Dst2D0pi_Selection_h

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Dst2D0pi {

  class Selection : public Analyser {

    public:

      Selection(TString _name, Variables_Analysis *_v);
      ~Selection();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif

#ifndef Bc2Dmunu_Selection_h
#define Bc2Dmunu_Selection_h

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Bc2Dmunu {

  class Selection : public Analyser {

    public:

      Selection(TString _name, Variables_Analysis *_v);
      ~Selection();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif

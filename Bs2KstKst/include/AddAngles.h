#ifndef Bs2KstKst_AddAngles_h
#define Bs2KstKst_AddAngles_h

#include "TString.h"
#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class AddAngles : public Analyser {

    public:

      AddAngles(TString _name, Variables_Analysis *_v);
      ~AddAngles();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif


#ifndef Bs2KstKst_AddMassVars_h
#define Bs2KstKst_AddMassVars_h

#include "RooWorkspace.h"
#include "RooArgSet.h"
#include "RooDataSet.h"

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class AddMassVars : public Analyser {

    public:

      AddMassVars(TString _name, Variables_Analysis *_v);
      ~AddMassVars();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif

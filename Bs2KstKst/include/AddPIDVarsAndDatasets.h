#ifndef Bs2KstKst_AddPIDVarsAndDatasets_h
#define Bs2KstKst_AddPIDVarsAndDatasets_h

#include "RooWorkspace.h"
#include "RooArgSet.h"
#include "RooDataSet.h"

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class AddPIDVarsAndDatasets : public Analyser {

    public:

      AddPIDVarsAndDatasets(TString _name, Variables_Analysis *_v);
      ~AddPIDVarsAndDatasets();

      void Init();
      void Term();
      bool AnalyseEvent();

      Variables_Analysis *v;

    private:

      RooWorkspace *w;

      std::map<int, RooDataSet*> dsets;
      RooArgSet *observables;

  };

}

#endif

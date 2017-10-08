#ifndef Bs2KstKst_CutOnMass_h
#define Bs2KstKst_CutOnMass_h

#include "RooWorkspace.h"
#include "RooArgSet.h"
#include "RooDataSet.h"

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class CutOnMass : public Analyser {

    public:

      CutOnMass(TString _name, Variables_Analysis *_v);
      ~CutOnMass();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif

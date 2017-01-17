#ifndef Bs2KstKst_CutOnBDT_h
#define Bs2KstKst_CutOnBDT_h

#include "RooWorkspace.h"
#include "RooArgSet.h"
#include "RooDataSet.h"

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class CutOnBDT : public Analyser {

    public:

      CutOnBDT(TString _name, Variables_Analysis *_v);
      ~CutOnBDT();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif

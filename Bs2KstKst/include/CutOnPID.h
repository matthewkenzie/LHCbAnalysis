#ifndef Bs2KstKst_CutOnPID_h
#define Bs2KstKst_CutOnPID_h

#include "RooWorkspace.h"
#include "RooArgSet.h"
#include "RooDataSet.h"

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class CutOnPID : public Analyser {

    public:

      CutOnPID(TString _name, Variables_Analysis *_v);
      ~CutOnPID();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif

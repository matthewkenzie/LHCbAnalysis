#ifndef Bs2KstKst_CutOnBDTAndPID_h
#define Bs2KstKst_CutOnBDTAndPID_h

#include "RooWorkspace.h"
#include "RooArgSet.h"
#include "RooDataSet.h"

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class CutOnBDTAndPID : public Analyser {

    public:

      CutOnBDTAndPID(TString _name, Variables_Analysis *_v);
      ~CutOnBDTAndPID();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif

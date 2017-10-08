#ifndef Bs2KstKst_CutOnBDTPIDAndMass_h
#define Bs2KstKst_CutOnBDTPIDAndMass_h

#include "RooWorkspace.h"
#include "RooArgSet.h"
#include "RooDataSet.h"

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class CutOnBDTPIDAndMass : public Analyser {

    public:

      CutOnBDTPIDAndMass(TString _name, Variables_Analysis *_v);
      ~CutOnBDTPIDAndMass();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif

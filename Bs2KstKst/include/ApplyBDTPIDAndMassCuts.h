#ifndef Bs2KstKst_ApplyBDTPIDAndMassCuts_h
#define Bs2KstKst_ApplyBDTPIDAndMassCuts_h

#include <vector>
#include <map>

#include "RooWorkspace.h"
#include "RooArgSet.h"
#include "RooDataSet.h"

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class ApplyBDTPIDAndMassCuts : public Analyser {

    public:

      ApplyBDTPIDAndMassCuts(TString _name, Variables_Analysis *_v);
      ~ApplyBDTPIDAndMassCuts();

      bool AnalyseEvent();

      Variables_Analysis *v;

  };

}

#endif

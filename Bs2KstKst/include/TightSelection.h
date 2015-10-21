/////////////////////////////////////
//                                 //
// TightSelection.h               //
// Author: Matthew Kenzie          //
// Auto-generated                  //
// Will run the analysis chain     //
//                                 //
/////////////////////////////////////

#ifndef TightSelection_h
#define TightSelection_h

#include "TString.h"
#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {
  class TightSelection : public Analyser {

    public:

      TightSelection(TString _name, const Variables_Analysis *_v);
      ~TightSelection();

      virtual void Init();
      virtual void Term();
      virtual bool AnalyseEvent();

      const Variables_Analysis *v;

  };
}
#endif

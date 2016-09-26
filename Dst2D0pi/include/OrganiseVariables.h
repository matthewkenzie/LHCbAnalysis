#ifndef Dst2D0pi_OrganiseVariables_h
#define Dst2D0pi_OrganiseVariables_h

#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Dst2D0pi {

  class OrganiseVariables : public Analyser {

    public:

      OrganiseVariables(TString _name, Variables_Analysis *_v);
      ~OrganiseVariables();

      bool AnalyseEvent();

      Variables_Analysis *v;

    private:

      Double_t CalculateSignedVtxDist();

  };

}

#endif

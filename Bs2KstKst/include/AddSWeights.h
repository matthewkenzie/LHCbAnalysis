#ifndef Bs2KstKst_AddSWeights_h
#define Bs2KstKst_AddSWeights_h

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "Utils.h"
#include "Analyser.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class AddSWeights : public Analyser {

    public:

      AddSWeights(TString _name, Variables_Analysis *_v);
      ~AddSWeights();

      void Init();
      void Term();
      bool AnalyseEvent();

      Variables_Analysis *v;

    private:

      TFile *sWeightsFile;
      TTree *sWeightsTree;

      ULong64_t sWeightEvNumber;
      Double_t  sWeight;


  };
}
#endif

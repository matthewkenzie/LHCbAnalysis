#ifndef BuRejectionBDT_h
#define BuRejectionBDT_h

#include "TMVAWrapperBase.h"
#include "Variables_Analysis.h"

namespace Bc2Dmunu {

  class BuRejectionBDT : public TMVAWrapperBase {

    public:

      BuRejectionBDT(TString _name, Variables_Analysis *_v, TMVAWrapperBase::mode _rMode=kTrain);
      ~BuRejectionBDT();

      void setCategories();
      void setFactoryOptions();
      void setTrainingOptions();
      void setNumberOfBDTs();
      void setBDTCycling();
      void setInputVariables();
      void setSpectatorVariables();

      bool setEventValuesAndEvaluate();

      Variables_Analysis *v;
  };
}

#endif

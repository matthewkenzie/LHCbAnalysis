#ifndef BDTForPID_h
#define BDTForPID_h

#include "TMVAWrapperBase.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class BDTForPID : public TMVAWrapperBase {

    public:

      BDTForPID(TString _name, Variables_Analysis *_v, TMVAWrapperBase::mode _rMode=kTrain);
      ~BDTForPID();

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

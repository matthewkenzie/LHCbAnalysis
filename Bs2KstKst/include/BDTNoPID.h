#ifndef BDTNoPID_h
#define BDTNoPID_h

#include "TMVAWrapperBase.h"
#include "Variables_Analysis.h"

namespace Bs2KstKst {

  class BDTNoPID : public TMVAWrapperBase {

    public:

      BDTNoPID(TString _name, Variables_Analysis *_v, TMVAWrapperBase::mode _rMode=kTrain);
      ~BDTNoPID();

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

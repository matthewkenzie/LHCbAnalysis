#ifndef CombRejectionBDT_h
#define CombRejectionBDT_h

#include "TMVAWrapperBase.h"
#include "Variables_Analysis.h"

namespace Bc2Dmunu {

  class CombRejectionBDT : public TMVAWrapperBase {

    public:

      CombRejectionBDT(TString _name, Variables_Analysis *_v, TMVAWrapperBase::mode _rMode=kTrain);
      ~CombRejectionBDT();

      void setCategories();
      void setFactoryOptions();
      void setTrainingOptions();
      void setNumberOfBDTs();
      void setBDTCycling();
      void setInputVariables();
      void setSpectatorVariables();

      bool setEventValuesAndEvaluate();

      Variables_Analysis *v;

    private:
      double dmin(const double &v1, const double &v2, const double &v3);
      double dmax(const double &v1, const double &v2, const double &v3);

  };
}

#endif

#ifndef Bc2Dmunu_BasicFitter_h
#define Bc2Dmunu_BasicFitter_h

#include "FitterBase.h"

namespace Bc2Dmunu {

  class BasicFitter : public FitterBase {

    public:

      BasicFitter(TString wsname="w", TString name="BasicFitter", bool verbose=false, bool debug=false);
      ~BasicFitter();

      void addObsVars();
      void addCuts();
      void addDatasets();
      void constructPdfs();

      void run();
      void makePlots();

    private:

      void makeBc2DmunuPdf();
      void makeBc2DstmunuPdf();
      void makeBu2DmunuPdf();
      void makeBu2DstmunuPdf();
      void makeCombinatorialPdf();
      void makeTotalPdf();

      void Bu2DmunuFit();
      void DataFit();

  };

}
#endif


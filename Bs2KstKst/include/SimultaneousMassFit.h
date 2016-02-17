#ifndef Bs2KstKst_SimultaneousMassFit_h
#define Bs2KstKst_SimultaneousMassFit_h

#include "FitterBase.h"

namespace Bs2KstKst {

  class SimultaneousMassFit : public FitterBase {

    public:

      SimultaneousMassFit(TString wsname="w", TString name="SimultaneousMassFit", bool verbose=false, bool debug=false);
      ~SimultaneousMassFit();

      void addObsVars();
      void addCuts();
      void addDatasets();
      void constructPdfs();

      void run();
      void makePlots();

    private:

      void makeInitialFitPlots();
      void makeDataPlot();
      void makeSolidDataPlot();
      void makeSWeightPlots();

      void Bs2KstKstFit();
      void Bd2KstKstFit();
      void Bs2PhiKstFit();
      void Bd2PhiKstFit();
      void Bd2RhoKstFit();
      void Lb2pKpipiFit();
      void Lb2ppipipiFit();
      void DataFit();

      void makeBs2KstKstPdf();
      void makeBd2KstKstPdf();
      void makeBs2PhiKstPdf();
      void makeBd2PhiKstPdf();
      void makeBd2RhoKstPdf();
      void makeLb2pKpipiPdf();
      void makeLb2ppipipiPdf();

      void makePartRecoPdf();
      void makeCombinatorialPdf();

      void makeTotalPdf();

  };

}
#endif


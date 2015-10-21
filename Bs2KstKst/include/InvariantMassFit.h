#ifndef Bs2KstKst_InvariantMassFit_h
#define Bs2KstKst_InvariantMassFit_h

#include "FitterBase.h"

namespace Bs2KstKst {

  class InvariantMassFit : public FitterBase {

    public:

      InvariantMassFit(TString wsname="w", TString name="InvariantMassFit", bool verbose=false, bool debug=false);
      ~InvariantMassFit();

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


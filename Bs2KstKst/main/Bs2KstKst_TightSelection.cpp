#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "TightSelection.h"
#include "TightSelectionPlotter.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("RunEngine", argc, argv);

  // create the variables
  Bs2KstKst::Variables_Analysis *v = new Bs2KstKst::Variables_Analysis() ;

  // make the analysers
  Bs2KstKst::TightSelection        *tightSel             = new Bs2KstKst::TightSelection       ( "TightSelection" ,            v );
  Bs2KstKst::TightSelectionPlotter *tightSelPlotter      = new Bs2KstKst::TightSelectionPlotter( "TightSelectionPlotter" ,     v );
  Bs2KstKst::TightSelectionPlotter *tightSelPlotterSplit = new Bs2KstKst::TightSelectionPlotter( "TightSelectionPlotterSplit", v );

  tightSelPlotter->split = false;
  tightSelPlotterSplit->split = true;
  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( tightSel );
  runner.addAnalyser( tightSelPlotter );
  runner.addAnalyser( tightSelPlotterSplit );

  // run
  runner.run();

  // clean up
  delete v;
  delete tightSel;
  delete tightSelPlotter;

  return 0;

}

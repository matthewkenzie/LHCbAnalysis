#include "Utils.h"

#include "RunEngine.h"
#include "Variables_PreSel.h"
#include "Analyser.h"
#include "Trigger.h"
#include "TruthMatching.h"
#include "PreSelection.h"
#include "PreSelPlotter.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("RunEngine", argc, argv);

  // create the variables
  Bc2BsPi::Variables_PreSel *v = new Bc2BsPi::Variables_PreSel() ;

  // make the analysers
  Analyser *truthMatch = new Bc2BsPi::TruthMatching( "TruthMatching" , v );
  Analyser *triggerSel = new Bc2BsPi::Trigger      ( "Trigger"       , v );
  Analyser *presel     = new Bc2BsPi::PreSelection ( "PreSelection"  , v );
  //Analyser *plotter    = new Bc2BsPi::PreSelPlotter( "PreSelPlotter" , v );

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( truthMatch );
  runner.addAnalyser( triggerSel );
  runner.addAnalyser( presel     );
  //runner.addAnalyser( plotter    );

  // run
  runner.run();

  // clean up
  delete v;
  delete truthMatch;
  delete triggerSel;
  delete presel;
  //delete plotter;

  return 0;

}

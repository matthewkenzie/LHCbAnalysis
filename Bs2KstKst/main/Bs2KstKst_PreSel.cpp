#include "Utils.h"

#include "RunEngine.h"
#include "Variables_PreSel.h"
#include "Analyser.h"
#include "Trigger.h"
#include "TruthMatching.h"
#include "PreSelection.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("RunEngine", argc, argv);

  // create the variables
  Bs2KstKst::Variables_PreSel *v = new Bs2KstKst::Variables_PreSel() ;

  // make the analysers
  Analyser *truthMatch = new Bs2KstKst::TruthMatching( "TruthMatching" , v );
  Analyser *triggerSel = new Bs2KstKst::Trigger      ( "Trigger"       , v );
  Analyser *presel     = new Bs2KstKst::PreSelection ( "PreSelection"  , v );

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( truthMatch );
  runner.addAnalyser( triggerSel );
  runner.addAnalyser( presel     );

  // run
  runner.run();

  // clean up
  delete v;
  delete truthMatch;
  delete triggerSel;
  delete presel;

  return 0;

}

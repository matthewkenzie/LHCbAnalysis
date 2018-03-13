#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Skimming.h"
#include "TruthMatching.h"
#include "Trigger.h"
#include "Selection.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("Bc2DmunuSkimming", argc, argv);

  // create the variables
  Bc2Dmunu::Variables_Skimming *v = new Bc2Dmunu::Variables_Skimming() ;

  // make the analysers
  Bc2Dmunu::TruthMatching *truthMatching = new Bc2Dmunu::TruthMatching ( "TruthMatching", v );
  Bc2Dmunu::Trigger       *trigger       = new Bc2Dmunu::Trigger       ( "Trigger"      , v );
  Bc2Dmunu::Selection     *selection     = new Bc2Dmunu::Selection     ( "Selection"    , v );

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( truthMatching );
  runner.addAnalyser( trigger       );
  runner.addAnalyser( selection     );

  // run
  runner.run();

  // clean up
  delete v;
  delete truthMatching;
  delete trigger;
  delete selection;

  return 0;

}


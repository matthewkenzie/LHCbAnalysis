#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "CombRejectionBDT.h"
#include "TighterSelection.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("Bc2DmunuAnalysis", argc, argv);

  // create the variables
  Bc2Dmunu::Variables_Analysis *v = new Bc2Dmunu::Variables_Analysis() ;

  // make the analysers
  Bc2Dmunu::TighterSelection *sel      = new Bc2Dmunu::TighterSelection( "TighterSelection", v);
  Bc2Dmunu::CombRejectionBDT *bdtTrain = new Bc2Dmunu::CombRejectionBDT( "CombRejectionBDT", v );
  bdtTrain->setTrainMode();

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( sel );
  runner.addAnalyser( bdtTrain );

  // run
  runner.run();

  // clean up
  delete v;
  delete sel;
  delete bdtTrain;

  return 0;

}


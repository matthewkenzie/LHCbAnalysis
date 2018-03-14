#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "BuRejectionBDT.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("Bc2DmunuAnalysis", argc, argv);

  // create the variables
  Bc2Dmunu::Variables_Analysis *v = new Bc2Dmunu::Variables_Analysis() ;

  // make the analysers
  Bc2Dmunu::BuRejectionBDT    *bdt       = new Bc2Dmunu::BuRejectionBDT( "BuRejectionBDT", v );
  bdt->setEvalMode();

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( bdt );

  // run
  runner.run();

  // clean up
  delete v;
  delete bdt;

  return 0;

}


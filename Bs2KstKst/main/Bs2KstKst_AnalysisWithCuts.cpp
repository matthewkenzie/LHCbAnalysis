#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "BDTNoPID.h"
#include "CutOnBDTAndPID.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("Bs2KstKstAnalysisCuts", argc, argv);

  // create the variables
  Bs2KstKst::Variables_Analysis *v = new Bs2KstKst::Variables_Analysis() ;

  // make the analysers
  Bs2KstKst::CutOnBDTAndPID *cut = new Bs2KstKst::CutOnBDTAndPID( "BDTAndPIDCut" , v );

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( cut );

  // run
  runner.run();

  // clean up
  delete v;
  delete cut;

  return 0;

}


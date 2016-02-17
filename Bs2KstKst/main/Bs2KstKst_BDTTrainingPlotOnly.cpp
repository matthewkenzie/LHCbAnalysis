#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "BDTNoPID.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("RunEngine", argc, argv);

  // create the variables
  Bs2KstKst::Variables_Analysis *v = new Bs2KstKst::Variables_Analysis() ;

  // make the analysers
  Bs2KstKst::BDTNoPID *bdtTrainer = new Bs2KstKst::BDTNoPID     ( "BDTNoPID" , v );
  bdtTrainer->setPlotMode();

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( bdtTrainer );

  // run
  runner.run();

  // clean up
  delete v;
  delete bdtTrainer;

  return 0;

}

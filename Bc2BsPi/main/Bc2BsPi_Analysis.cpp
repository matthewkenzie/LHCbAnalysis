#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "Analyser.h"
#include "PreSelPlotter.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("RunEngine", argc, argv);

  // create the variables
  Bc2BsPi::Variables_Analysis *v = new Bc2BsPi::Variables_Analysis() ;

  // make the analysers
  Analyser *plotter    = new Bc2BsPi::PreSelPlotter( "PreSelPlotter" , v );

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( plotter    );

  // run
  runner.run();

  // clean up
  delete v;
  delete plotter;

  return 0;

}

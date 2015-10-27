#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
//#include "Trigger.h"
#include "Selection.h"
#include "Plotter.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("Dst2D0piAnalysis", argc, argv);

  // create the variables
  Dst2D0pi::Variables_Analysis *v = new Dst2D0pi::Variables_Analysis() ;

  // make the analysers
  //Dst2D0pi::Trigger   *trigger   = new Dst2D0pi::Trigger   ( "Trigger", v );
  Dst2D0pi::Selection *selection = new Dst2D0pi::Selection ( "Selection", v );
  Dst2D0pi::Plotter   *plotter   = new Dst2D0pi::Plotter   ( "Plotter", v );

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  //runner.addAnalyser( trigger );
  runner.addAnalyser( selection );
  runner.addAnalyser( plotter );

  // run
  runner.run();

  // clean up
  delete v;
  //delete trigger;
  delete selection;
  delete plotter;

  return 0;

}


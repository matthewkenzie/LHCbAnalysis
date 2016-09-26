#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "Trigger.h"
#include "OrganiseVariables.h"
#include "PreSelection.h"
//#include "Plotter.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("Dst2D0piAnalysis", argc, argv);

  // create the variables
  Dst2D0pi::Variables_Analysis *v = new Dst2D0pi::Variables_Analysis() ;

  // make the analysers
  Dst2D0pi::OrganiseVariables *orgvars = new Dst2D0pi::OrganiseVariables( "OrganiseVariables", v );
  Dst2D0pi::Trigger           *trigger = new Dst2D0pi::Trigger          ( "Trigger", v );
  Dst2D0pi::PreSelection      *presel  = new Dst2D0pi::PreSelection     ( "PreSelection", v );
  //Dst2D0pi::Plotter           *plotter = new Dst2D0pi::Plotter          ( "Plotter", v );

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( orgvars );
  runner.addAnalyser( trigger );
  runner.addAnalyser( presel );
  //runner.addAnalyser( plotter );

  // run
  runner.run();

  // clean up
  delete v;
  delete orgvars;
  delete trigger;
  delete presel;
  //delete plotter;

  return 0;

}


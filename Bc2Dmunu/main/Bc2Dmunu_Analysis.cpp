#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "Trigger.h"
#include "Selection.h"
#include "Plotter.h"
#include "BuRejectionBDT.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("Bc2DmunuAnalysis", argc, argv);

  // create the variables
  Bc2Dmunu::Variables_Analysis *v = new Bc2Dmunu::Variables_Analysis() ;

  // make the analysers
  Bc2Dmunu::Trigger   *trigger   = new Bc2Dmunu::Trigger   ( "Trigger", v );
  Bc2Dmunu::Selection *selection = new Bc2Dmunu::Selection ( "Selection", v );
  Bc2Dmunu::Plotter   *plotter   = new Bc2Dmunu::Plotter   ( "Plotter", v );
  //Bc2Dmunu::BuRejectionBDT *bdt  = new Bc2Dmunu::BuRejectionBDT( "BDT", v );
  //bdt->setEvalMode();

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( trigger );
  runner.addAnalyser( selection );
  runner.addAnalyser( plotter );
  //runner.addAnalyser( bdt );

  // run
  runner.run();

  // clean up
  delete v;
  delete trigger;
  delete selection;
  delete plotter;
  //delete bdt;

  return 0;

}


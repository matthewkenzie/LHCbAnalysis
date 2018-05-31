#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "TighterSelection.h"
#include "CombRejectionBDT.h"
#include "BuRejectionBDT.h"
#include "AnalysisPlotter.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("Bc2DmunuAnalysisDoubleSel", argc, argv);

  // create the variables
  Bc2Dmunu::Variables_Analysis *v = new Bc2Dmunu::Variables_Analysis() ;

  // make the analysers
  Bc2Dmunu::TighterSelection  *sel      = new Bc2Dmunu::TighterSelection( "TighterSelection", v);
  Bc2Dmunu::CombRejectionBDT  *bdtComb  = new Bc2Dmunu::CombRejectionBDT( "CombRejectionBDT", v );
  bdtComb->setEvalMode();
  Bc2Dmunu::BuRejectionBDT    *bdtB     = new Bc2Dmunu::BuRejectionBDT( "BuRejectionBDT", v );
  bdtB->setEvalMode();
  //Bc2Dmunu::AnalysisPlotter   *plotter  = new Bc2Dmunu::AnalysisPlotter( "AnalysisPlotter", v );

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( sel );
  runner.addAnalyser( bdtComb );
  runner.addAnalyser( bdtB );
  //runner.addAnalyser( plotter );

  // run
  runner.run();

  // clean up
  delete v;
  delete sel;
  delete bdtComb;
  delete bdtB;
  //delete plotter;

  return 0;

}



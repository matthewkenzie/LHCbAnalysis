#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "SkimmingPlotter.h"
#include "DataCompPlotter.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("Bc2DmunuSkimmingPlotter", argc, argv);

  // create the variables
  Bc2Dmunu::Variables_Analysis *v = new Bc2Dmunu::Variables_Analysis() ;

  // make the analysers
  Bc2Dmunu::SkimmingPlotter   *skimPlotter = new Bc2Dmunu::SkimmingPlotter( "SkimmingPlotter", v );
  Bc2Dmunu::DataCompPlotter   *dataPlotter = new Bc2Dmunu::DataCompPlotter( "DataCompPlotter", v );

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( skimPlotter );
  runner.addAnalyser( dataPlotter );

  // run
  runner.run();

  // clean up
  delete v;
  delete skimPlotter;
  delete dataPlotter;

  return 0;

}


#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "BDTNoPID.h"
#include "AddMassVars.h"
#include "AddPIDVarsAndDatasets.h"
#include "ApplyBDTAndPIDCuts.h"
#include "AddAngles.h"
#include "MassVariablesPlotter.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("Bs2KstKstAnalysis", argc, argv);

  // create the variables
  Bs2KstKst::Variables_Analysis *v = new Bs2KstKst::Variables_Analysis() ;

  // make the analysers
  Bs2KstKst::BDTNoPID              *bdtRunner = new Bs2KstKst::BDTNoPID     ( "BDTNoPID" , v );
  bdtRunner->setEvalMode();
  Bs2KstKst::AddMassVars           *addMass   = new Bs2KstKst::AddMassVars           ( "AddMassVars"   , v );
  Bs2KstKst::AddPIDVarsAndDatasets *addPid    = new Bs2KstKst::AddPIDVarsAndDatasets ( "AddPIDVars"    , v );
  Bs2KstKst::ApplyBDTAndPIDCuts    *bdtPid    = new Bs2KstKst::ApplyBDTAndPIDCuts    ( "ApplyBDTAndPID", v );
  Bs2KstKst::AddAngles             *addAng    = new Bs2KstKst::AddAngles             ( "AddAngles"     , v );
  Bs2KstKst::MassVariablesPlotter  *massPlot  = new Bs2KstKst::MassVariablesPlotter  ( "MassVariablesPlotter", v );

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( bdtRunner );
  runner.addAnalyser( addMass   );
  runner.addAnalyser( addPid    );
  runner.addAnalyser( bdtPid    );
  runner.addAnalyser( addAng    );
  runner.addAnalyser( massPlot  );

  // run
  runner.run();

  // clean up
  delete v;
  delete bdtRunner;
  delete addMass;
  delete addPid;
  delete bdtPid;
  delete addAng;
  delete massPlot;

  return 0;

}


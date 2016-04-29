#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "BDTNoPID.h"
#include "AddPIDVarsAndDatasets.h"
#include "ApplyBDTAndPIDCuts.h"
#include "AddAngles.h"
#include "AddSWeights.h"

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
  Bs2KstKst::AddPIDVarsAndDatasets *addVars   = new Bs2KstKst::AddPIDVarsAndDatasets ( "AddPIDVars"    , v );
  Bs2KstKst::ApplyBDTAndPIDCuts    *bdtPid    = new Bs2KstKst::ApplyBDTAndPIDCuts    ( "ApplyBDTAndPID", v );
  Bs2KstKst::AddAngles             *addAng    = new Bs2KstKst::AddAngles             ( "AddAngles"     , v );
  Bs2KstKst::AddSWeights           *addSW     = new Bs2KstKst::AddSWeights           ( "AddSWeights"   , v );

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( bdtRunner );
  runner.addAnalyser( addVars   );
  runner.addAnalyser( bdtPid    );
  runner.addAnalyser( addAng    );
  runner.addAnalyser( addSW     );

  // run
  runner.run();

  // clean up
  delete v;
  delete bdtRunner;
  delete addVars;
  delete bdtPid;
  delete addAng;
  delete addSW;

  return 0;

}


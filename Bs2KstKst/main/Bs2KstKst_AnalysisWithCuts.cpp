#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "BDTNoPID.h"
#include "AddPIDVarsAndDatasets.h"
#include "ApplyBDTPIDAndMassCuts.h"
#include "CutOnBDT.h"
#include "CutOnPID.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("Bs2KstKstAnalysis", argc, argv);

  // create the variables
  Bs2KstKst::Variables_Analysis *v = new Bs2KstKst::Variables_Analysis() ;

  // make the analysers
  Bs2KstKst::BDTNoPID               *bdtRunner = new Bs2KstKst::BDTNoPID     ( "BDTNoPID" , v );
  bdtRunner->setEvalMode();
  Bs2KstKst::AddPIDVarsAndDatasets  *addVars   = new Bs2KstKst::AddPIDVarsAndDatasets  ( "AddPIDVars"    , v );
  Bs2KstKst::ApplyBDTPIDAndMassCuts *bdtPid    = new Bs2KstKst::ApplyBDTPIDAndMassCuts ( "ApplyBDTPIDAndMass", v );
  Bs2KstKst::CutOnBDT               *bdtCut    = new Bs2KstKst::CutOnBDT               ( "BDTCut", v );
  Bs2KstKst::CutOnPID               *pidCut    = new Bs2KstKst::CutOnPID               ( "PIDCut", v );

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( bdtRunner );
  runner.addAnalyser( addVars   );
  runner.addAnalyser( bdtPid    );
  runner.addAnalyser( bdtCut    );
  runner.addAnalyser( pidCut    );

  // run
  runner.run();

  // clean up
  delete v;
  delete bdtRunner;
  delete addVars;
  delete bdtPid;
  delete bdtCut;
  delete pidCut;

  return 0;

}



#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "BDTNoPID.h"
#include "AddMassVars.h"
#include "AddPIDVarsAndDatasets.h"
#include "ApplyBDTPIDAndMassCuts.h"
#include "CutOnBDT.h"
#include "CutOnPID.h"
#include "CutOnMass.h"
#include "CutOnBDTPIDAndMass.h"

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
  Bs2KstKst::AddMassVars            *addMass   = new Bs2KstKst::AddMassVars            ( "AddMassVars"           , v ); // Adds mass variables (e.g. M_pPimKmPip )
  Bs2KstKst::AddPIDVarsAndDatasets  *addVars   = new Bs2KstKst::AddPIDVarsAndDatasets  ( "AddPIDVars"            , v );
  Bs2KstKst::ApplyBDTPIDAndMassCuts *bdtPid    = new Bs2KstKst::ApplyBDTPIDAndMassCuts ( "ApplyBDTPIDAndMassCuts", v );
  Bs2KstKst::CutOnBDT               *bdtCut    = new Bs2KstKst::CutOnBDT               ( "BDTCut"                , v );
  Bs2KstKst::CutOnPID               *pidCut    = new Bs2KstKst::CutOnPID               ( "PIDCut"                , v );
  Bs2KstKst::CutOnMass              *massCut   = new Bs2KstKst::CutOnMass              ( "MassCut"               , v );

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( bdtRunner );
  runner.addAnalyser( addVars   );
  runner.addAnalyser( bdtPid    );
  runner.addAnalyser( bdtCut    );
  runner.addAnalyser( pidCut    );
  runner.addAnalyser( massCut    );

  // run
  runner.run();

  // clean up
  delete v;
  delete bdtRunner;
  delete addVars;
  delete bdtPid;
  delete massCut;
  delete bdtCut;
  delete pidCut;

  return 0;

}



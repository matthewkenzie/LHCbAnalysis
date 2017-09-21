#include "Utils.h"

#include "RunEngine.h"
#include "Variables_Analysis.h"
#include "BDTNoPID.h"
#include "AddMassVars.h"
#include "AddPIDVarsAndDatasets.h"
#include "ApplyBDTPIDAndMassCuts.h"
#include "AddAngles.h"
#include "MassVariablesPlotter.h"
#include "PIDVariablesPlotter.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  RunEngine runner("Bs2KstKstAnalysis", argc, argv);

  // create the variables
  Bs2KstKst::Variables_Analysis *v = new Bs2KstKst::Variables_Analysis() ;

  // make the analysers
  Bs2KstKst::BDTNoPID               *bdtRunner = new Bs2KstKst::BDTNoPID              ( "BDTNoPID" , v );
  bdtRunner->setEvalMode();
  Bs2KstKst::AddMassVars            *addMass   = new Bs2KstKst::AddMassVars           ( "AddMassVars"           , v ); // Adds mass variables (e.g. M_pPimKmPip )
  Bs2KstKst::AddPIDVarsAndDatasets  *addPid    = new Bs2KstKst::AddPIDVarsAndDatasets ( "AddPIDVars"            , v ); // Adds PID variables  (e.g. Prob_NNpKpi )
  Bs2KstKst::ApplyBDTPIDAndMassCuts *applyCuts = new Bs2KstKst::ApplyBDTPIDAndMassCuts( "ApplyBDTPIDAndMassCuts", v ); // Applies the cuts by adding pass_ variables
  Bs2KstKst::AddAngles              *addAng    = new Bs2KstKst::AddAngles             ( "AddAngles"             , v ); // Adds the decay angles
  Bs2KstKst::MassVariablesPlotter   *massPlot  = new Bs2KstKst::MassVariablesPlotter  ( "MassVariablesPlotter"  , v ); // Makes some mass distribution plots
  Bs2KstKst::PIDVariablesPlotter    *pidPlot   = new Bs2KstKst::PIDVariablesPlotter   ( "PIDVariablesPlotter"   , v ); // Makes some PID distribution plots

  // pass variables to runner
  runner.setVariables( v );

  // pass analysers to runner
  runner.addAnalyser( bdtRunner );
  runner.addAnalyser( addMass   );
  runner.addAnalyser( addPid    );
  runner.addAnalyser( applyCuts );
  runner.addAnalyser( addAng    );
  runner.addAnalyser( massPlot  );
  runner.addAnalyser( pidPlot   );

  // run
  runner.run();

  // clean up
  delete v;
  delete bdtRunner;
  delete addMass;
  delete addPid;
  delete applyCuts;
  delete addAng;
  delete massPlot;
  delete pidPlot;

  return 0;

}


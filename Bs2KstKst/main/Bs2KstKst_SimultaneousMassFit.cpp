#include "Utils.h"

#include "FitEngine.h"
#include "FitterBase.h"
#include "SimultaneousMassFit.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  FitEngine fitEngine("Bs2KstKstSimultaneousMassFit", argc, argv);

  Bs2KstKst::SimultaneousMassFit *fitter = new Bs2KstKst::SimultaneousMassFit("w","SimultaneousMassFit", fitEngine.arg.verbose, fitEngine.arg.debug );

  fitEngine.setFitter( fitter );

  fitEngine.run();

  return 0;

}


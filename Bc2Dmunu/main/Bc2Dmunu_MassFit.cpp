#include "Utils.h"

#include "FitEngine.h"
#include "FitterBase.h"
#include "BasicFitter.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  FitEngine fitEngine("Bc2DmunuFit", argc, argv);

  Bc2Dmunu::BasicFitter *fitter = new Bc2Dmunu::BasicFitter("w","BasicFitter", fitEngine.arg.verbose, fitEngine.arg.debug );

  fitEngine.setFitter( fitter );

  fitEngine.run();

  return 0;

}


#include "Utils.h"

#include "FitEngine.h"
#include "FitterBase.h"
#include "LifetimeFitter.h"

using namespace std;
using namespace Utils;

int main(int argc, char **argv) {

  // create run engine
  FitEngine fitEngine("Dst2D0piLifetimeFitter", argc, argv);

  Dst2D0pi::LifetimeFitter *fitter = new Dst2D0pi::LifetimeFitter("w","LifetimeFitter", fitEngine.arg.verbose, fitEngine.arg.debug );

  fitEngine.setFitter( fitter );

  fitEngine.run();

  return 0;

}


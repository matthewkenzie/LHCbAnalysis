#include <iostream>

#include "TApplication.h"

#include "StandAloneFitter.h"

using namespace std;

int main(int argc, char **argv) {

  TApplication *theApp = new TApplication("TheApp",&argc,argv);
  Dst2D0pi::StandAloneFitter fitter;
  fitter.run();

  cout << "Done. Exit with Ctrl+C" << endl;
  theApp->Run();

  return 0;
}

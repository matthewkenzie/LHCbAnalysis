#include <iostream>
#include <string>

#include "TApplication.h"

#include "StandAloneFitter.h"

using namespace std;

void printHelp() {

  cout << "\t --makeDatasets    :  " << "Make the datasets only" << endl;
  cout << "\t --loadDatasets    :  " << "Perform the fit only" << endl;
  exit(0);

}

int main(int argc, char **argv) {

  bool makeDatasets=false;
  bool loadDatasets=false;

  // check options
  for (int i=0; i<argc; i++) {
    string opt = string(argv[i]);
    if (opt=="-h" || opt=="--help") printHelp();
    if (opt=="--makeDatasets") makeDatasets = true;
    if (opt=="--loadDatasets") loadDatasets = true;
  }

  TApplication *theApp = new TApplication("TheApp",&argc,argv);
  Dst2D0pi::StandAloneFitter fitter;
  fitter.run(makeDatasets,loadDatasets);

  cout << "Done. Exit with Ctrl+C" << endl;
  theApp->Run();

  return 0;
}

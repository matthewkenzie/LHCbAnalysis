#include <iostream>
#include <string>

#include "TApplication.h"

#include "StandAloneFitter.h"

using namespace std;

void printHelp() {

  cout << "\t --makeDatasets    :  " << "Make the datasets only" << endl;
  cout << "\t --loadDatasets    :  " << "Perform the fit only" << endl;
  cout << "\t --datafile        :  " << "File with data tree" << endl;
  cout << "\t --mcfile          :  " << "File with MC tree" << endl;
  exit(0);

}

int main(int argc, char **argv) {

  bool makeDatasets=false;
  bool loadDatasets=false;
  TString datafile;
  TString mcfile;

  // check options
  for (int i=0; i<argc; i++) {
    string opt = string(argv[i]);
    if (opt=="-h" || opt=="--help") printHelp();
    if (opt=="--makeDatasets") makeDatasets = true;
    if (opt=="--loadDatasets") loadDatasets = true;
    if (opt=="--datafile"    ) datafile = TString( argv[i+1] );
    if (opt=="--mcfile"      ) mcfile = TString( argv[i+1] );
  }

  TApplication *theApp = new TApplication("TheApp",&argc,argv);
  Dst2D0pi::StandAloneFitter fitter(datafile,mcfile);
  fitter.run(makeDatasets,loadDatasets);

  cout << "Done. Exit with Ctrl+C" << endl;
  theApp->Run();

  return 0;
}

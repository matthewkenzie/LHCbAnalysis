#include <iostream>
#include <vector>

#include "boost/filesystem.hpp"

#include "TROOT.h"
#include "TFile.h"
#include "TMath.h"
#include "TChain.h"
#include "TTree.h"

#include "Utils.h"
#include "FitEngine.h"

using namespace std;
using namespace Utils;

FitEngine::FitEngine(TString _name, int argc, char **argv):
  name(_name)
{
  // Start timer
  timer.Start();
  // Pass command line arguments
  arg.parseOptions(argc,argv);

  if ( arg.interactive ) theApp = new TApplication("App", &argc, argv);
  else gROOT->SetBatch();

}

FitEngine::~FitEngine(){
  timer.Stop();
  cout << "SUCCESS" << endl;
  cout << "Took: "; timer.Print();
}

void FitEngine::setFitter( FitterBase *fit ) {
  fitter = fit;
}

void FitEngine::run() {

  if ( ! fitter ) {

    error( "ERROR -- No fitter set in FitEngine" );
  }

  if ( arg.cached ) {
    if ( arg.cachedDataOnly ) {
      fitter->loadCachedData( arg.cachefile );
      fitter->constructPdfs();
    }
    else {
      fitter->loadCachedWorkspace( arg.cachefile );
    }
  }
  else {

    fitter->addObsVars();
    fitter->addCuts();
    fitter->addDatasets();
    fitter->makeDatasets();
    fitter->fillDatasets( arg.infilename, arg.intreename );
    fitter->makeCombinedDatasets();
    fitter->constructPdfs();
  }

  if ( ! arg.plotOnly ) fitter->run();

  fitter->makePlots();
  fitter->save( arg.outfilename );

  if ( arg.interactive ) {
    cout << "Exit with Ctrl+c" << endl;
    theApp->Run();
  }

}

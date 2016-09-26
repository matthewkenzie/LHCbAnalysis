#include "boost/program_options.hpp"

#include "TROOT.h"
#include "TStopwatch.h"
#include "TApplication.h"

#include "Utils.h"

#include "RunEngine.h"
#include "MultiDimCutOpt.h"

using namespace std;
using namespace Utils;
namespace po = boost::program_options;

bool loadFromCachce;
bool makeCache;
string infname;
string outfname;
string cachefile;
int scanPoints1D;
int scanPoints2D;
int misIdType;
int fomType;
string runType;
bool bdtOnly;
bool interactive;

void parseOptions(int argc, char **argv) {

  po::options_description desc("MultiDimCutOpt options");
  po::variables_map vm;

  desc.add_options()
    ("help,h",                "Print help and exit")
    ("infile,i",     po::value<string>(&infname)->default_value("root/AnalysisOut.root"),         "Name of input file")
    ("outfile,o",    po::value<string>(&outfname)->default_value("root/MultiDimCutOptOut.root"),  "Name of output file")
    ("cachecfile,c", po::value<string>(&cachefile)->default_value("root/MultiDimCutOptIn.root"),  "Name of cache file")
    ("loadCache,l",  po::bool_switch(&loadFromCachce)->default_value(false),                      "Load from cache")
    ("makeCache",    po::bool_switch(&makeCache)->default_value(false),                           "Make cache file")
    ("scanPoints1D", po::value<int>(&scanPoints1D)->default_value(10),                            "Number of 1D scan points")
    ("scanPoints2D", po::value<int>(&scanPoints2D)->default_value(10),                            "Number of 2D scan points")
    ("misIdType,m",  po::value<int>(&misIdType)->default_value(1),                                "The MisIdType, Options:\n"
                                                                                                  "  1: rho_mc_y + phi_mc_y + lb_mc_y\n"
                                                                                                  "  2: rho_mc_y\n"
                                                                                                  "  3: phi_mc_y\n"
                                                                                                  "  4: lb_mc_y\n"
                                                                                                  "  5: rho_mc_y + phi_mc_y\n")
    ("fomType,f",   po::value<int>(&fomType)->default_value(1),                                   "The fig of merit type. Options:\n"
                                                                                                  "  0: S/sqrt(S+B)              \n"
                                                                                                  "  1: S/sqrt(S+B+M)            \n"
                                                                                                  "  2: S/sqrt(S+B+sigma*M)      \n"
                                                                                                  "  3: S/sqrt(S+B+2sigma*M)     \n"
                                                                                                  "  4: S/sqrt(S+B+M) * S/(S+B+M)\n")
    ("runType,r",   po::value<string>(&runType)->default_value("all"),                            "The run type. Options:\n"
                                                                                                  "  all:  everything (default)   \n"
                                                                                                  "  2011: just 2011              \n"
                                                                                                  "  2012: just 2012              \n")
    ("bdtOnly,b",   po::bool_switch(&bdtOnly)->default_value(false),                              "Run the BDT optimisation only")
    ("interactive,I", po::bool_switch(&interactive)->default_value(false),                        "Run in interactive mode")
    ;

  po::store( po::parse_command_line( argc, argv, desc) , vm);
  notify(vm);

  if (vm.count("help")) {
    cout << desc << endl;
    exit(0);
  }

}

int main(int argc, char **argv) {

  parseOptions(argc,argv);

  TApplication *theApp;

  if ( interactive ) theApp = new TApplication("App", &argc, argv);
  else gROOT->SetBatch();

  Bs2KstKst::MultiDimCutOpt *cutOpt = new Bs2KstKst::MultiDimCutOpt();

  // set run type
  if ( runType == "all" ) {
    cutOpt->setRunAll(true);
  }
  else if ( runType == "2011" ) {
    cutOpt->setRunAll(false);
    cutOpt->setRun2011(true);
  }
  else if ( runType == "2012" ) {
    cutOpt->setRunAll(false);
    cutOpt->setRun2012(true);
  }
  else {
    cerr << "ERROR - no such runType: " << runType << endl;
    exit(1);
  }

  // setup files
  cutOpt->setup( infname , outfname, cachefile );

  // set mis id type
  cutOpt->setMisIdType( misIdType );

  if ( loadFromCachce ) {
    cutOpt->loadWorkspace();
  }
  else {
    cutOpt->makeInitialDatasets();
    //cutOpt->loadDatasetsFromFile( "root/MultiDimCutDatasets.root" );
    cutOpt->makePDFs();
    cutOpt->plotShapes();
  }

  if ( ! makeCache ) {
    cutOpt->setScanPoints1D( scanPoints1D );
    cutOpt->setScanPoints2D( scanPoints2D );
    if ( bdtOnly ) cutOpt->runBDTOptOnly( fomType );
    else           cutOpt->runSimple( fomType );
  }

  cutOpt->save();

  if ( interactive ) {
    cout << "Exit with Ctrl+c" << endl;
    theApp->Run();
  }

}


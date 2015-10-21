#include <iostream>

#include "TString.h"

#include "FitOptParser.h"

using namespace std;
namespace po = boost::program_options;
using namespace po;

FitOptParser::FitOptParser():
  desc("Allowed options")
{

  desc.add_options()
    ("help,h",                                                                          "Produce help message and exit")
    ("infilename,i",  value<string>(&infilename),                                       "Name of input file")
    ("intreename,t",  value<string>(&intreename)->default_value("AnalysisTree"),        "Name of input tree")
    ("outfilename,o", value<string>(&outfilename)->default_value("root/FitterOut.root"),"Name of output file")
    ("interactive,I", bool_switch(&interactive)->default_value(false),                  "Run in interactive mode")
    ("verbose,v",     bool_switch(&verbose)->default_value(false),                      "Run in verbose mode")
    ("debug,d",       bool_switch(&debug)->default_value(false),                        "Run in debug mode")
    ("cached,c",      bool_switch(&cached)->default_value(false),                       "Load cached workspace")
    ("cachefile",     value<string>(&cachefile)->default_value(""),                     "Cachefile name")
    ("cachedDataOnly",bool_switch(&cachedDataOnly)->default_value(false),               "Load cached data only")
    ("plotOnly",      bool_switch(&plotOnly)->default_value(false),                     "Only make the plots")
    ;
}

FitOptParser::~FitOptParser(){}

void FitOptParser::parseOptions(int argc, char **argv) {

  prog_name = string(argv[0]);

  store( parse_command_line( argc, argv, desc) , vm);
  notify(vm);

  if (vm.count("help")) {
    cout << desc << endl;
    exit(0);
  }
}


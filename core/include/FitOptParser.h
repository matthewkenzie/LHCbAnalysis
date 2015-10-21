#ifndef FitOptParser_h
#define FitOptParser_h

#include "TString.h"

#include "boost/program_options.hpp"

namespace po = boost::program_options;

class FitOptParser {

  public:
    FitOptParser();
    ~FitOptParser();

    void parseOptions(int argc, char **argv);

    std::string   infilename;
    std::string   intreename;
    std::string   outfilename;
    bool          interactive;
    bool          debug;
    bool          verbose;
    bool          cached;
    std::string   cachefile;
    bool          cachedDataOnly;
    bool          plotOnly;

    std::string   prog_name;

  private:

    po::options_description desc;
    po::variables_map vm;


};

#endif


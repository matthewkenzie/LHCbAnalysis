#include <iostream>
#include <fstream>
#include <vector>

#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/algorithm/string/predicate.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/filesystem.hpp"

#include "Utils.h"
#include "Config.h"

using namespace std;
using namespace boost;
using namespace Utils;

void Config::readDatFile(const std::string &datfile) {

  cfgDatFile = TMacro("config",datfile.c_str());

  ifstream df(datfile.c_str());

  print("Config::readDatFile()",Form("Reading file: %s",datfile.c_str()));

  string line;
  while (getline(df, line)) {
    cfgDatFile.AddLine(line.c_str());
    if ( starts_with( line, "#" ) ) continue;
    vector<string> line_elements;
    split( line_elements, line, is_any_of("\t ") );
    if ( line_elements.size() == 0 ) continue;
    if ( ! starts_with( line_elements[0] , "itype" ) ) continue;

    InputFileOptions opt;
    for (vector<string>::iterator el = line_elements.begin(); el != line_elements.end(); el ++) {

      vector<string> element_arg_val;
      split ( element_arg_val, *el, is_any_of("=") );

      if ( element_arg_val.size()!=2 ) continue;

      if ( element_arg_val[0] == "itype" ) {
        opt.itype = lexical_cast<int>(element_arg_val[1]);
      }

      if ( element_arg_val[0] == "sqrts" ) {
        opt.sqrts = lexical_cast<int>(element_arg_val[1]);
      }

      if ( element_arg_val[0] == "year" ) {
        opt.year = element_arg_val[1];
      }

      if ( element_arg_val[0] == "name" ) {
        opt.name = element_arg_val[1];
      }

      if ( element_arg_val[0] == "fname" ) {
        opt.addFile( element_arg_val[1] );
      }

      if ( element_arg_val[0] == "tname" ) {
        opt.addTree( element_arg_val[1] );
      }
    }
    // need to check if itype and name are already in there
    bool alreadyExists = false;
    for (vector<InputFileOptions>::iterator it = config.begin(); it != config.end(); it ++ )
    {
      if ( it->name == opt.name && it->itype == opt.itype ) // same name and itype so just add the files
      {
        it->addFile( opt.filenames[0] );
        it->addTree( opt.treenames[0] );
        alreadyExists = true;
        break; // dont need to keep looking
      }
      if ( it->name == opt.name && it->itype != opt.itype ) // same name but itype mismatch
      {
        error( Form("This name (%s) already exists in the map but with a different itype (%d). You tried itype (%d)", opt.name.Data(), it->itype, opt.itype) );
      }
      if ( it->name != opt.name && it->itype == opt.itype) // same itype but name mismatch
      {
        error( Form("This itype (%d) already exists in the map but with a different name (%s). You tried name (%s)",opt.itype,it->name.Data(),opt.name.Data()) );
      }
    }
    if ( !alreadyExists ) // then add it
    {
      config.push_back( opt );
    }
  } // end readlines loop
}

void Config::printConfig() {

  print("Config::printConfig()", Form("%-30s %-5s %-5s %-10s", "Name", "itype", "sqrts", "year") );

  for (vector<InputFileOptions>::iterator cIt = config.begin(); cIt != config.end(); cIt++ ){

    print("Config::printConfig()", Form("%-30s %-5d %-5d %-10s", cIt->name.Data(), cIt->itype, cIt->sqrts, cIt->year.Data()) );
    for (int f=0; f< cIt->filenames.size(); f++ ) {
      filesystem::path fn( cIt->filenames[f].Data() );
      filesystem::path tn( cIt->treenames[f].Data() );
      print(" ", Form("    %-80s : %s", fn.filename().string().c_str(), tn.string().c_str() ) );
    }

  }


}

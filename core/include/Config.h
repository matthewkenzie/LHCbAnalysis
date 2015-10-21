#ifndef Config_h
#define Config_h

#include <string>
#include <map>
#include <vector>

#include "TMacro.h"

#include "InputFileOptions.h"

class Config {

  public:

    void readDatFile(const std::string &datfile);
    void printConfig();

    //std::map<TString, InputFileOptions> config;
    std::vector<InputFileOptions> config;

    TMacro cfgDatFile;

};

#endif

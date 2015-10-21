#ifndef RunEngine_h
#define RunEngine_h

#include <vector>

#include "TString.h"
#include "TStopwatch.h"

#include "OptParser.h"
#include "Config.h"
#include "InputFileOptions.h"
#include "CreateBatchJob.h"

#include "EventCounter.h"
#include "Analyser.h"
#include "Variables.h"

class RunEngine {

  public:

    RunEngine(TString _name, int argc, char **argv);
    ~RunEngine();

    inline void addAnalyser(Analyser *analyser) {
      analysers.push_back(analyser);
      eventCounter.addAnalyser( analyser->name );
    }
    inline void setVariables(Variables *vars) { v = vars; isVariablesSet = true; }

    void run();

  private:

    TString name;
    OptParser arg;
    Config cfg;
    std::vector<int>     itypes;  // these guys get setup when we pass the config
    std::vector<TString> evnames; // these guys get setup when we pass the config
    std::vector<int>     sqrtss;  // these guys get setup when we pass the config
    std::vector<TString> years;   // these guys get setup when we pass the config

    std::vector<TString> infilenames;
    std::vector<TString> intreenames;

    TStopwatch timer;
    std::vector<Analyser*> analysers;
    Variables *v;

    EventCounter eventCounter;

    void createBatchJobs();
    void setupConfig();
    void setupEventCounter();

    void printProgressBar(Long64_t jentry, bool isDone=false, bool newLine=false);
    void printProgressBar(Long64_t jentry, Long64_t fEntry, Long64_t lEntry, bool isDone=false, bool newLine=false);
    void checkSetup();

    bool isVariablesSet;

};

#endif

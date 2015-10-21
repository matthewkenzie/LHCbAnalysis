#ifndef Analyser_h
#define Analyser_h

#include <iostream>

#include "TString.h"

#include "Variables.h"

class Analyser {

  public:

    Analyser(TString _name);
    virtual ~Analyser() = 0;

    virtual void Init();
    virtual void Term();
    virtual bool AnalyseEvent() = 0;

    TString name;

    Long64_t processedEvents;
    Long64_t passedEvents;

};

#endif

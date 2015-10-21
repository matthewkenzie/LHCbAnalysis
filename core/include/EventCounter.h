#ifndef EventCounter_h
#define EventCounter_h

#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <algorithm>

#include "TString.h"
#include "TH1I.h"
#include "TH2D.h"

class EventCounter {

  public:

    EventCounter();
    void addDataType( int itype, TString evName );
    void addAnalyser( TString anName );
    void Init();
    void countEvent(int itype, TString analyserName, bool isPass);
    void countEvent(TString evName, TString analyserName, bool isPass);
    void printShortSummary(TString evName);
    void printSummary();
    void setStyle(TH1* h, bool is2d=true);

    std::vector<int>         itypes;
    std::vector<TString>     evNames;
    std::map<int, TString>   typeToNameMap;
    std::map<TString, int>   nameToTypeMap;
    std::vector<TString>     analyserNames;
    std::vector<TString>     sortedNames;

    TH2I *hPass;
    TH2I *hFail;
    TH2I *hProc;
    TH2D *hEff;
    TH1I *hType;

};

#endif

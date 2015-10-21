#ifndef Variables_h
#define Variables_h

#include <iostream>
#include "TROOT.h"
#include "TTree.h"

class Variables {

  public:

    Variables();
    ~Variables();

    virtual void setInputBranches(TTree *tree) = 0;
    virtual void setOutputBranches(TTree *tree) = 0;

    //
    // these variables will always be used
    //
    int       itype;
    TString   evname;
    int       sqrts;
    TString   year;

    // this must be declared back here (for TMVAWrapperBase class)
    ULong64_t eventNumber;

};
#endif


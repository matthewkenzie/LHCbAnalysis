/* Abstract Base class for BDTs
 *
 */

#ifndef TMVAWrapperBase_h
#define TMVAWrapperBase_h

#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "Analyser.h"
#include "Variables.h"

#include "TFile.h"
#include "TString.h"
#include "TH1F.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"

class TMVAWrapperBase : public Analyser {

  public:

    enum mode {
      kTrain,
      kEval,
    };

    TMVAWrapperBase(TString _name, const Variables *_v, mode _rMode=kTrain);
    ~TMVAWrapperBase();

    void Init();
    void Term();
    bool AnalyseEvent();

    void addEvent(TString cat, bool isSig);
    void addSignalEvent(TString cat);
    void addBackgroundEvent(TString cat);
    float evaluateMVAValue(TString cat);

    // abstract functions that must be implemented
    virtual void setCategories() = 0;
    virtual void setFactoryOptions() = 0;
    virtual void setTrainingOptions() = 0;
    virtual void setNumberOfBDTs() = 0;
    virtual void setBDTCycling() = 0;
    virtual void setInputVariables() = 0;
    virtual void setSpectatorVariables() = 0;

    virtual bool setEventValuesAndEvaluate() = 0;

    void setOutFileName(TString name) { outfilename = name; }

    mode getMode() { return rMode; }
    void setMode(mode val) { rMode = val; }
    inline void setTrainMode() { rMode = kTrain; }
    inline void setEvalMode() { rMode = kEval; }
    
    const Variables *v;
    mode rMode;


  protected:

    TString outfilename;
    int numberOfBDTs;
    bool doBDTCycling;

    TString factoryOptions;
    TString trainingOptions;

    std::vector<TString> categories;
    std::vector<TString> varNames;
    std::map<TString,double> varMap;
    std::map<TString,float> readVarMap;

    void addVar(TString var);
    void setVal(TString var, double value);

  private:

    void checkInit();
    void initMVAFactories();
    void prepare();
    void train();
    void test();
    void evaluate();

    TFile *outFile;
    std::map<TString, std::vector<TMVA::Factory*> > factoryContainer;
    std::map<TString, std::vector<TMVA::Reader*> >  readerContainer;
    std::map<TString, std::vector<std::pair<int,int> > > sigEvCounts;
    std::map<TString, std::vector<std::pair<int,int> > > bkgEvCounts;

    TTree *trainingTree;
    int relBDT;
    bool isSigEvent;
    TString eventCat;
    std::vector<TBranch*> bdtoutBranches; // add a branch to the training on the fly for each BDT trained
    float bdtval; // float to store bdt val

    TString weightsFile;
    TString methodName;

    void createTrainingHistograms();
    void makePlot(TString cat, int b, TString type);
    TString getHistName(TString cat, int b, TString var, TString ext="");
    void setHistStyle(TH1F *sigTrain, TH1F *bkgTrain, TH1F *sigTest, TH1F* bkgTest);
    double getMaximum(TH1F *sigTrain, TH1F *bkgTrain, TH1F *sigTest, TH1F* bkgTest);
    TString types[4];
    std::map<TString, TH1F*> histStore;

};

#endif

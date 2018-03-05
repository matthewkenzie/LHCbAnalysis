#include "BuRejectionBDT.h"
#include "TString.h"
#include "TMath.h"

using namespace std;
using namespace TMVA;

Bc2Dmunu::BuRejectionBDT::BuRejectionBDT(TString _name, Variables_Analysis *_v, TMVAWrapperBase::mode _rMode):
  TMVAWrapperBase(_name, _v, _rMode),
  v(_v)
{}

Bc2Dmunu::BuRejectionBDT::~BuRejectionBDT(){}

void Bc2Dmunu::BuRejectionBDT::setCategories(){
  categories.push_back("2012");
}

void Bc2Dmunu::BuRejectionBDT::setNumberOfBDTs(){
  numberOfBDTs = 2;
}

void Bc2Dmunu::BuRejectionBDT::setBDTCycling(){
  doBDTCycling = true;
}

void Bc2Dmunu::BuRejectionBDT::setFactoryOptions(){
	factoryOptions = "";
  factoryOptions += "!V:";
	factoryOptions += "!Silent:";
	factoryOptions += "Color:";
	factoryOptions += "DrawProgressBar:";
  factoryOptions += "Transformations=G,D:";
	factoryOptions += "AnalysisType=Classification:";
}

void Bc2Dmunu::BuRejectionBDT::setTrainingOptions(){
  trainingOptions = "";
  trainingOptions += "!H:";
  trainingOptions += "!V:";
  //trainingOptions += "VarTransform=D,G:";
  trainingOptions += "NTrees=200:";
  trainingOptions += "BoostType=AdaBoost:";
  trainingOptions += "UseBaggedBoost:";
  trainingOptions += "nCuts=-1:";
  trainingOptions += "MinNodeSize=5:";
  trainingOptions += "MaxDepth=3:";
  trainingOptions += "NegWeightTreatment=IgnoreNegWeightsInTraining:";
}

void Bc2Dmunu::BuRejectionBDT::setInputVariables() {
  addVar("B_plus_LOGIPCHI2");
  addVar("B_plus_DIRA_OWNPV");
  addVar("B_plus_LTIME");
  addVar("D0_LOGIPCHI2");
  addVar("D0_PT");
  addVar("D0_ENDVERTEX_CHI2");
  addVar("B_plus_ENDVERTEX_CHI2");
}

void Bc2Dmunu::BuRejectionBDT::setSpectatorVariables(){
  return;
}

bool Bc2Dmunu::BuRejectionBDT::setEventValuesAndEvaluate() {

  // setup values
  //

  setVal("B_plus_LOGIPCHI2", TMath::Log( v->B_plus_IPCHI2_OWNPV ) );
  setVal("B_plus_DIRA_OWNPV"  , v->B_plus_DIRA_OWNPV );
  setVal("B_plus_LTIME"         , v->B_plus_LTIME );
  setVal("D0_LOGIPCHI2"   , TMath::Log( v->D0_IPCHI2_OWNPV ) );
  setVal("D0_PT"             , v->D0_PT );
  setVal("D0_ENDVERTEX_CHI2" , v->D0_ENDVERTEX_CHI2);
  setVal("B_plus_ENDVERTEX_CHI2", v->B_plus_ENDVERTEX_CHI2);

  // TRAINING
  if ( rMode == kTrain ) {

    // Signal is Bc -> D mu nu
    if ( v->itype == -80 ) {
      addSignalEvent("2012");
    }

    // Background is Bu -> D mu nu
    if ( v->itype == -82 ) {
      addBackgroundEvent("2012");
    }

  }
  else if ( rMode == kEval ) {
    cout << "NOT IMPLEMENTED" << endl;
    //v->bu_rejection_bdtoutput = evaluateMVAValue("2012");
  }
  else {
    cerr << "ERROR -- Bc2Dmunu::BuRejectionBDT::setEventValuesAndEvaluate() -- invalid run mode" << endl;
    exit(1);
  }

  return true;
}

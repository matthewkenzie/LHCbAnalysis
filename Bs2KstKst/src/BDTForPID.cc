#include "BDTForPID.h"
#include "TString.h"

using namespace std;
using namespace TMVA;

Bs2KstKst::BDTForPID::BDTForPID(TString _name, Variables_Analysis *_v, TMVAWrapperBase::mode _rMode):
  TMVAWrapperBase(_name, _v, _rMode),
  v(_v)
{}

Bs2KstKst::BDTForPID::~BDTForPID(){}

void Bs2KstKst::BDTForPID::setCategories(){
  categories.push_back("2011");
  categories.push_back("2012");
}

void Bs2KstKst::BDTForPID::setNumberOfBDTs(){
  numberOfBDTs = 2;
}

void Bs2KstKst::BDTForPID::setBDTCycling(){
  doBDTCycling = true;
}

void Bs2KstKst::BDTForPID::setFactoryOptions(){
	factoryOptions = "";
  factoryOptions += "!V:";
	factoryOptions += "!Silent:";
	factoryOptions += "Color:";
	factoryOptions += "DrawProgressBar:";
  factoryOptions += "Transformations=G,D:";
	factoryOptions += "AnalysisType=Classification:";
}

void Bs2KstKst::BDTForPID::setTrainingOptions(){
  trainingOptions = "";
  trainingOptions += "!H:";
  trainingOptions += "!V:";
  //trainingOptions += "VarTransform=D,G:";
  trainingOptions += "NTrees=800:";
  trainingOptions += "BoostType=AdaBoost:";
  trainingOptions += "UseBaggedBoost:";
  trainingOptions += "nCuts=20:";
  trainingOptions += "MinNodeSize=5:";
  trainingOptions += "MaxDepth=3:";
  trainingOptions += "NegWeightTreatment=IgnoreNegWeightsInTraining:";
}

void Bs2KstKst::BDTForPID::setInputVariables() {
  addVar("Kplus_PT");
  addVar("Kminus_PT");
  addVar("Kplus_ETA");
  addVar("Kminus_ETA");
  addVar("Kplus_ProbNNKpi");
  addVar("Kminus_ProbNNKpi");
  addVar("Piplus_ProbNNpiK");
  addVar("Piminus_ProbNNpiK");
}

void Bs2KstKst::BDTForPID::setSpectatorVariables(){
  return;
}

bool Bs2KstKst::BDTForPID::setEventValuesAndEvaluate() {

  TString year = v->year;

  // setup values
  //
  setVal("Kplus_PT", v->Kplus_PT);
  setVal("Kminus_PT", v->Kminus_PT);
  setVal("Kplus_ETA" , v->Kplus_ETA);
  setVal("Kminus_ETA", v->Kminus_ETA);
  setVal("Kplus_ProbNNKpi"  , v->Kplus_ProbNNKpi );
  setVal("Kminus_ProbNNKpi" , v->Kminus_ProbNNKpi );
  setVal("Piplus_ProbNNpiK" , v->Piplus_ProbNNpiK );
  setVal("Piminus_ProbNNpiK", v->Piminus_ProbNNpiK );

  // TRAINING
  if ( rMode == kTrain || rMode == kPlot ) {
    // Bs->KstKst MC
    if ( v->itype == -70 || v->itype == -80 ) {
      addSignalEvent(year);
    }
    // Bd->RhoKst MC
    if ( v->itype == -77 || v->itype == -87 ) {
      addBackgroundEvent(year);
    }
  }
  else if ( rMode == kEval ) {
    v->pidbdtoutput = evaluateMVAValue(year);
  }
  else {
    cerr << "ERROR -- Bs2KstKst::BDTForPID::setEventValuesAndEvaluate() -- invalid run mode" << endl;
    exit(1);
  }

  return true;
}


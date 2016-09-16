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
  //addVar("B_s0_DTF_B_s0_PT");
  //addVar("B_s0_DTF_KST1_PT");
  //addVar("B_s0_DTF_KST2_PT");
  //addVar("max_track_PT");
  //addVar("min_track_PT");
  //addVar("B_s0_ETA");
  //addVar("Kst_ETA");
  //addVar("Kstb_ETA");
  //addVar("max_track_ETA");
  //addVar("min_track_ETA");
  //addVar("B_s0_DIRA_OWNPV");
  //addVar("B_s0_ENDVERTEX_CHI2");
  addVar("Kplus_PT");
  addVar("Kminus_PT");
  //addVar("Piplus_PT");
  //addVar("Piminus_PT");
  addVar("Kplus_ETA");
  addVar("Kminus_ETA");
  //addVar("Piplus_ETA");
  //addVar("Piminus_ETA");
  //addVar("Kplus_V3ProbNNKpi_corr");
  //addVar("Kminus_V3ProbNNKpi_corr");
  //addVar("Piplus_V3ProbNNKpi_corr");
  //addVar("Piminus_V3ProbNNKpi_corr");
  addVar("Kplus_V3ProbNNk_corr");
  addVar("Kminus_V3ProbNNk_corr");
  addVar("Kplus_V3ProbNNpi_corr");
  addVar("Kminus_V3ProbNNpi_corr");
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
  //setVal("Piplus_PT", v->Piplus_PT);
  //setVal("Piminus_PT", v->Piminus_PT);
  setVal("Kplus_ETA" , v->Kplus_ETA);
  setVal("Kminus_ETA", v->Kminus_ETA);
  //setVal("Piplus_ETA", v->Piplus_ETA);
  //setVal("Piminus_ETA", v->Piminus_ETA);
  //setVal("Kplus_V3ProbNNKpi_corr"  , v->Kplus_V3ProbNNk_corr   * (1. - v->Kplus_V3ProbNNpi_corr   ) );
  //setVal("Kminus_V3ProbNNKpi_corr" , v->Kminus_V3ProbNNk_corr  * (1. - v->Kminus_V3ProbNNpi_corr  ) );
  //setVal("Piplus_V3ProbNNKpi_corr" , v->Piplus_V3ProbNNk_corr  * (1. - v->Piplus_V3ProbNNpi_corr  ) );
  //setVal("Piminus_V3ProbNNKpi_corr", v->Piminus_V3ProbNNk_corr * (1. - v->Piminus_V3ProbNNpi_corr ) );
  setVal("Kplus_V3ProbNNk_corr", v->Kplus_V3ProbNNk_corr);
  setVal("Kminus_V3ProbNNk_corr", v->Kminus_V3ProbNNk_corr);
  setVal("Kplus_V3ProbNNpi_corr", v->Kplus_V3ProbNNpi_corr);
  setVal("Kminus_V3ProbNNpi_corr", v->Kminus_V3ProbNNpi_corr);

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


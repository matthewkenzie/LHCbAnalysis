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
  // for now we only have one category of BDT as we only have Run 1 MC
  categories.push_back("Run1");
}

void Bc2Dmunu::BuRejectionBDT::setBDTCycling(){
  // if we want to implement the k-folding CV technique we can do so here
  doBDTCycling = true;
}

void Bc2Dmunu::BuRejectionBDT::setNumberOfBDTs(){
  // if we are implementing k-folding this is the number of folds e.g. 5
  numberOfBDTs = 2;
}

void Bc2Dmunu::BuRejectionBDT::setFactoryOptions(){
	factoryOptions = "";
  factoryOptions += "!V:";
	factoryOptions += "!Silent:";
	factoryOptions += "Color:";
	factoryOptions += "DrawProgressBar:";
  //factoryOptions += "Transformations=G,D:";
	factoryOptions += "AnalysisType=Classification:";
}

void Bc2Dmunu::BuRejectionBDT::setTrainingOptions(){
  trainingOptions = "";
  trainingOptions += "!H:";
  trainingOptions += "!V:";
  //trainingOptions += "VarTransform=D,G:";
  trainingOptions += "NTrees=400:";
  trainingOptions += "BoostType=AdaBoost:";
  trainingOptions += "UseBaggedBoost:";
  trainingOptions += "nCuts=-1:";
  trainingOptions += "MinNodeSize=5:";
  trainingOptions += "MaxDepth=3:";
  trainingOptions += "NegWeightTreatment=IgnoreNegWeightsInTraining:";
}

void Bc2Dmunu::BuRejectionBDT::setInputVariables() {
  addVar("B_plus_LOGPT");
  addVar("B_plus_LOGIPCHI2");
  addVar("B_plus_ACOS_DIRA_OWNPV");
  addVar("B_plus_LOGFDCHI2");
  addVar("D0_LOGPT");
  addVar("D0_LOGIPCHI2");
  addVar("Mu_plus_LOGPT");
  addVar("Mu_plus_LOGIPCHI2");
}

void Bc2Dmunu::BuRejectionBDT::setSpectatorVariables(){
  return;
}

bool Bc2Dmunu::BuRejectionBDT::setEventValuesAndEvaluate() {

  // setup values
  //
  if ( v->B_plus_LTIME<0 ) return false;

  setVal("B_plus_LOGPT"            , TMath::Log( v->B_plus_PT )           );
  setVal("B_plus_LOGIPCHI2"        , TMath::Log( v->B_plus_IPCHI2_OWNPV ) );
  setVal("B_plus_ACOS_DIRA_OWNPV"  , TMath::ACos( v->B_plus_DIRA_OWNPV )  );
  setVal("B_plus_LOGFDCHI2"        , TMath::Log( v->B_plus_FDCHI2_OWNPV ) );
  setVal("D0_LOGPT"                , TMath::Log( v->D0_PT )               );
  setVal("D0_LOGIPCHI2"            , TMath::Log( v->D0_IPCHI2_OWNPV )     );
  setVal("Mu_plus_LOGPT"           , TMath::Log( v->Mu_plus_PT )          );
  setVal("Mu_plus_LOGIPCHI2"       , TMath::Log( v->Mu_plus_MIPCHI2PV )   );

  // TRAINING
  if ( rMode == kTrain || rMode == kPlot ) {

    // Signal is Bc -> D mu nu
    if ( v->itype == -20 ) {
      addSignalEvent("Run1");
    }

    // Background is Bu -> D mu nu
    if ( v->itype == -29 ) {
      addBackgroundEvent("Run1");
    }

  }
  else if ( rMode == kEval ) {
    v->comb_rejection_bdtoutput = evaluateMVAValue("Run1");
  }
  else {
    cerr << "ERROR -- Bc2Dmunu::BuRejectionBDT::setEventValuesAndEvaluate() -- invalid run mode" << endl;
    exit(1);
  }

  return true;
}

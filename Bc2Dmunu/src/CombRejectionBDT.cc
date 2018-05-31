#include "CombRejectionBDT.h"
#include "TString.h"
#include "TMath.h"

using namespace std;
using namespace TMVA;

Bc2Dmunu::CombRejectionBDT::CombRejectionBDT(TString _name, Variables_Analysis *_v, TMVAWrapperBase::mode _rMode):
  TMVAWrapperBase(_name, _v, _rMode),
  v(_v)
{}

Bc2Dmunu::CombRejectionBDT::~CombRejectionBDT(){}

void Bc2Dmunu::CombRejectionBDT::setCategories(){
  // for now we only have one category of BDT as we only have Run 1 MC
  categories.push_back("Run1");
}

void Bc2Dmunu::CombRejectionBDT::setBDTCycling(){
  // if we want to implement the k-folding CV technique we can do so here
  doBDTCycling = true;
}

void Bc2Dmunu::CombRejectionBDT::setNumberOfBDTs(){
  // if we are implementing k-folding this is the number of folds e.g. 5
  numberOfBDTs = 2;
}

void Bc2Dmunu::CombRejectionBDT::setFactoryOptions(){
	factoryOptions = "";
  factoryOptions += "!V:";
	factoryOptions += "!Silent:";
	factoryOptions += "Color:";
	factoryOptions += "DrawProgressBar:";
  factoryOptions += "Transformations=G,D:";
	factoryOptions += "AnalysisType=Classification:";
}

void Bc2Dmunu::CombRejectionBDT::setTrainingOptions(){
  trainingOptions = "";
  trainingOptions += "!H:";
  trainingOptions += "!V:";
  trainingOptions += "VarTransform=D,G:";
  trainingOptions += "NTrees=800:";
  trainingOptions += "BoostType=AdaBoost:";
  trainingOptions += "UseBaggedBoost:";
  trainingOptions += "nCuts=-1:";
  trainingOptions += "MinNodeSize=5:";
  trainingOptions += "MaxDepth=3:";
  trainingOptions += "NegWeightTreatment=IgnoreNegWeightsInTraining:";
}

double Bc2Dmunu::CombRejectionBDT::dmax( const double &v1, const double &v2, const double &v3 ){
  return TMath::Max( TMath::Max( v1,v2), v3);
}

double Bc2Dmunu::CombRejectionBDT::dmin( const double &v1, const double &v2, const double &v3 ){
  return TMath::Min( TMath::Min( v1,v2), v3);
}


void Bc2Dmunu::CombRejectionBDT::setInputVariables() {
  addVar("B_plus_PT");
  addVar("B_plus_ACOS_DIRA_OWNPV");
  addVar("B_plus_ENDVERTEX_CHI2");
  addVar("B_plus_IPCHI2");
  addVar("D0_PT");
  addVar("D0_ACOS_DIRA_OWNPV");
  addVar("D0_ENDVERTEX_CHI2");
  addVar("D0_IPCHI2");
  addVar("MAX_TRACK_PT");
  addVar("MIN_TRACK_PT");
  addVar("MIN_TRACK_IPCHI2");
  addVar("MAX_TRACK_IPCHI2");
  addVar("MAX_TRACK_CHI2NDOF");
  addVar("MAX_TRACK_CHI2NDOF");
  addSpectator("D0_M");
  addSpectator("B_plus_M");
  addSpectator("B_plus_MCORR");
}

void Bc2Dmunu::CombRejectionBDT::setSpectatorVariables(){
  return;
}

bool Bc2Dmunu::CombRejectionBDT::setEventValuesAndEvaluate() {

  // setup values
  //
  if ( v->B_plus_LTIME<0 ) return false;

  setVal("B_plus_PT"              ,  v->B_plus_PT             );
  setVal("B_plus_ACOS_DIRA_OWNPV" , TMath::ACos( v->B_plus_DIRA_OWNPV ));
  setVal("B_plus_ENDVERTEX_CHI2"  , TMath::Log( v->B_plus_ENDVERTEX_CHI2/v->B_plus_ENDVERTEX_NDOF ) );
  setVal("B_plus_IPCHI2"          , TMath::Log( v->B_plus_IPCHI2_OWNPV                       ) );
  setVal("D0_PT"                  , v->D0_PT );
  setVal("D0_ACOS_DIRA_OWNPV"     , TMath::ACos( v->D0_DIRA_OWNPV ) );
  setVal("D0_ENDVERTEX_CHI2"      , TMath::Log( v->D0_ENDVERTEX_CHI2/v->D0_ENDVERTEX_NDOF ) );
  setVal("D0_IPCHI2"              , TMath::Log( v->D0_IPCHI2_OWNPV   ) );
  setVal("MAX_TRACK_PT"           , dmax( v->K_minus_PT, v->Pi_plus_PT, v->Mu_plus_PT)   );
  setVal("MIN_TRACK_PT"           , dmin( v->K_minus_PT, v->Pi_plus_PT, v->Mu_plus_PT)   );
  setVal("MIN_TRACK_IPCHI2"       , TMath::Log(dmax( v->K_minus_MIPCHI2PV, v->Pi_plus_MIPCHI2PV, v->Mu_plus_MIPCHI2PV)   ));
  setVal("MAX_TRACK_IPCHI2"       , TMath::Log(dmin( v->K_minus_MIPCHI2PV, v->Pi_plus_MIPCHI2PV, v->Mu_plus_MIPCHI2PV)   ));
  setVal("MAX_TRACK_CHI2NDOF"     , TMath::Log(dmax( v->K_minus_TRCHI2DOF, v->Pi_plus_TRCHI2DOF, v->Mu_plus_TRCHI2DOF)   ));
  setVal("MAX_TRACK_CHI2NDOF"     , TMath::Log(dmin( v->K_minus_TRCHI2DOF, v->Pi_plus_TRCHI2DOF, v->Mu_plus_TRCHI2DOF)   ));

  setVal("D0_M", v->D0_M);
  setVal("B_plus_M", v->B_plus_M);
  setVal("B_plus_MCORR", v->B_plus_MCORR);

  // TRAINING
  if ( rMode == kTrain || rMode == kPlot ) {

    // Signal is Bc -> D mu nu
    if ( v->itype == -20 ) {
      addSignalEvent("Run1");
    }

    // Background is Comb
    if ( v->itype > 0 ) {
      if ( (v->D0_M>1900) ) // just use upper sideband
        addBackgroundEvent("Run1");
    }

  }
  else if ( rMode == kEval ) {
    v->bu_rejection_bdtoutput = evaluateMVAValue("Run1");
  }
  else {
    cerr << "ERROR -- Bc2Dmunu::CombRejectionBDT::setEventValuesAndEvaluate() -- invalid run mode" << endl;
    exit(1);
  }

  return true;
}

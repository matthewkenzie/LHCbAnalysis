#include "BDTNoPID.h"
#include "TString.h"
#include "TMath.h"

using namespace std;
using namespace TMVA;
using namespace TMath;

Bs2KstKst::BDTNoPID::BDTNoPID(TString _name, Variables_Analysis *_v, TMVAWrapperBase::mode _rMode):
  TMVAWrapperBase(_name, _v, _rMode),
  v(_v)
{}

Bs2KstKst::BDTNoPID::~BDTNoPID(){}

void Bs2KstKst::BDTNoPID::setCategories(){
  categories.push_back("2011");
  categories.push_back("2012");
}

void Bs2KstKst::BDTNoPID::setNumberOfBDTs(){
  numberOfBDTs = 4;
}

void Bs2KstKst::BDTNoPID::setBDTCycling(){
  doBDTCycling = true;
}

void Bs2KstKst::BDTNoPID::setFactoryOptions(){
	factoryOptions = "";
  factoryOptions += "!V:";
	factoryOptions += "!Silent:";
	factoryOptions += "Color:";
	factoryOptions += "DrawProgressBar:";
  factoryOptions += "Transformations=G,D:";
	factoryOptions += "AnalysisType=Classification:";
}

void Bs2KstKst::BDTNoPID::setTrainingOptions(){
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

void Bs2KstKst::BDTNoPID::setInputVariables() {
  //addVar("B_s0_DTF_B_s0_PT");
  //addVar("B_s0_DTF_KST1_PT");
  //addVar("B_s0_DTF_KST2_PT");
  addVar("B_s0_PT");
  addVar("Kst_PT");
  addVar("Kstb_PT");
  addVar("max_track_PT");
  addVar("min_track_PT");
  addVar("B_s0_ETA");
  addVar("Kst_ETA");
  addVar("Kstb_ETA");
  addVar("max_track_ETA");
  addVar("min_track_ETA");
  addVar("B_s0_DIRA_OWNPV");
  addVar("B_s0_ENDVERTEX_CHI2");
}

void Bs2KstKst::BDTNoPID::setSpectatorVariables(){
  addSpectator("B_s0_DTF_B_s0_M");
  return;
}

bool Bs2KstKst::BDTNoPID::setEventValuesAndEvaluate() {

  TString year = v->year;

  // setup values
  // spectator
  setVal("B_s0_DTF_B_s0_M", v->B_s0_DTF_B_s0_M);

  //
  // PT
  //setVal("B_s0_DTF_B_s0_PT",v->B_s0_DTF_B_s0_PT);
  //setVal("B_s0_DTF_KST1_PT",v->B_s0_DTF_KST1_PT);
  //setVal("B_s0_DTF_KST2_PT",v->B_s0_DTF_KST2_PT);
  setVal("B_s0_PT",v->B_s0_PT);
  setVal("Kst_PT",v->Kst_PT);
  setVal("Kstb_PT",v->Kstb_PT);

  // pt order
  //double pts[4]  = { v->B_s0_DTF_KST1_K_PT , v->B_s0_DTF_KST2_K_PT , v->B_s0_DTF_KST1_PI_PT , v->B_s0_DTF_KST2_PI_PT };
  double pts[4]  = { v->Kplus_PT , v->Kminus_PT , v->Piplus_PT , v->Piminus_PT };
  double etas[4] = { Abs(v->Kplus_ETA) , Abs(v->Kminus_ETA) , Abs(v->Piplus_ETA) , Abs(v->Piminus_ETA) };
  int max=-1;
  int min=-1;
  double maxpt=-1.e10;
  double minpt = 1.e10;
  for (int i=0; i<4; i++) {
    if ( pts[i]>maxpt ) {
      maxpt = pts[i];
      max = i;
    }
    if ( pts[i]<minpt ) {
      minpt = pts[i];
      min = i;
    }
  }

  setVal("max_track_PT",maxpt);
  setVal("min_track_PT",minpt);

  setVal("B_s0_ETA",Abs(v->B_s0_ETA));
  setVal("Kst_ETA", Abs(v->Kst_ETA));
  setVal("Kstb_ETA",Abs(v->Kstb_ETA));
  setVal("max_track_ETA",etas[max]);
  setVal("min_track_ETA",etas[min]);

  setVal("B_s0_DIRA_OWNPV",v->B_s0_DIRA_OWNPV);
  setVal("B_s0_ENDVERTEX_CHI2",v->B_s0_ENDVERTEX_CHI2);


  // TRAINING
  if ( rMode == kTrain || rMode == kPlot ) {
    // MC only
    if ( v->itype < 0 ) {
      addSignalEvent(year);
    }
    // Data only
    if ( v->itype > 0 ) {
      if ( v->B_s0_DTF_B_s0_M <= 5600 ) return false;
      addBackgroundEvent(year);
    }
  }
  else if ( rMode == kEval ) {
    v->bdtoutput = evaluateMVAValue(year);
  }
  else {
    cerr << "ERROR -- Bs2KstKst::BDTNoPID::setEventValuesAndEvaluate() -- invalid run mode" << endl;
    exit(1);
  }

  return true;
}

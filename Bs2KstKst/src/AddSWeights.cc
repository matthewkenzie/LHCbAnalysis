#include "AddSWeights.h"

using namespace std;
using namespace Utils;

Bs2KstKst::AddSWeights::AddSWeights(TString _name, Variables_Analysis *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::AddSWeights::~AddSWeights(){}

void Bs2KstKst::AddSWeights::Init() {

  print( name+"::Init()", "Initialising Analyser: "+name );

  // Open sWeights file
  TString sWeightsFileName = "root/InvariantMassFitOut.root";
  sWeightsFile = TFile::Open(sWeightsFileName);
  if ( !sWeightsFile ) {
    error( "No sWeights file found: "+sWeightsFileName );
  }
  // get tree w sWeights
  sWeightsTree = (TTree*)sWeightsFile->Get("trees/Data_wsweights");
  if ( !sWeightsTree ){
    error( "No sWeights tree found: trees/Data" );
  }
  // set branches
  sWeightsTree->SetBranchAddress( "eventNumber", &sWeightEvNumber );
  sWeightsTree->SetBranchAddress( "bs2kstkst_y_sw", &sWeight );
}

void Bs2KstKst::AddSWeights::Term() {
  sWeightsFile->Close();
}

bool Bs2KstKst::AddSWeights::AnalyseEvent(){

  // Add sWeights into tree
  v->sweight = 1.;

  // Data only
  if ( v->itype > 0 ) {

    if ( v->pass_bdt && v->pass_pid ) {
      // have to lumber through sweight tree to find it
      bool found = false;
      for ( Long64_t ev=0; ev<sWeightsTree->GetEntries(); ev++ ) {
        sWeightsTree->GetEntry(ev);
        if ( v->eventNumber == sWeightEvNumber ) {
          v->sweight = sWeight;
          found = true;
          break;
        }
      }
      if ( !found ) error( Form("sWeight not found for event: %llu",v->eventNumber) );
    }
    else {
      v->sweight = 0.;
    }
  }

  return true;
}

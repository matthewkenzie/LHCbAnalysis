#include "TLorentzVector.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

#include "Utils.h"

#include "ApplyBDTAndPIDCuts.h"

using namespace std;
using namespace Utils;

Bs2KstKst::ApplyBDTAndPIDCuts::ApplyBDTAndPIDCuts(TString _name, Variables_Analysis *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::ApplyBDTAndPIDCuts::~ApplyBDTAndPIDCuts(){}

bool sortFunc( pair<TString,double> A, pair<TString,double> B ) {
  if ( A.second > B.second ) return true;
  else return false;
}

bool Bs2KstKst::ApplyBDTAndPIDCuts::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //

  v->pass_bdt = false;

  if ( v->year == "2011" ) {
    if ( v->bdtoutput           > 0.02    ) v->pass_bdt = true;
  }
  else if ( v->year == "2012" ) {
    if ( v->bdtoutput           > 0.03    ) v->pass_bdt = true;
  }
  else {
    error ( "v->year not recognised: "+v->year );
  }

  v->pass_pid = true;

  if ( v->min_kaon_ProbNNk < 0.2 )   v->pass_pid = false;
  if ( v->min_pion_ProbNNpi < 0.2 )  v->pass_pid = false;
  if ( v->min_kaon_ProbNNKpi < 0.3 ) v->pass_pid = false;
  if ( v->min_pion_ProbNNpiK < 0.3 ) v->pass_pid = false;

  // the special case for RhoKst mass veto
  v->pass_rhokst = false;

  vector< pair<TString,double> > ProbNNk;
  if ( v->itype < 0 ) { // MC
    ProbNNk.push_back( make_pair("Kplus", v->Kplus_ProbNNk_corr) );
    ProbNNk.push_back( make_pair("Kminus", v->Kminus_ProbNNk_corr) );
    ProbNNk.push_back( make_pair("Piplus", v->Piplus_ProbNNk_corr) );
    ProbNNk.push_back( make_pair("Piminus", v->Piminus_ProbNNk_corr) );
  }
  else { // Data
    ProbNNk.push_back( make_pair("Kplus", v->Kplus_ProbNNk) );
    ProbNNk.push_back( make_pair("Kminus", v->Kminus_ProbNNk) );
    ProbNNk.push_back( make_pair("Piplus", v->Piplus_ProbNNk) );
    ProbNNk.push_back( make_pair("Piminus", v->Piminus_ProbNNk) );
  }

  map<TString,double> ProbNNpi;
  if ( v->itype < 0 ) { // MC
    ProbNNpi["Kplus"]   = v->Kplus_V3ProbNNpi_corr;
    ProbNNpi["Kminus"]  = v->Kminus_V3ProbNNpi_corr;
    ProbNNpi["Piplus"]  = v->Piplus_V3ProbNNpi_corr;
    ProbNNpi["Piminus"] = v->Piminus_V3ProbNNpi_corr;
  }
  else { // Data
    ProbNNpi["Kplus"]   = v->Kplus_ProbNNpi;
    ProbNNpi["Kminus"]  = v->Kminus_ProbNNpi;
    ProbNNpi["Piplus"]  = v->Piplus_ProbNNpi;
    ProbNNpi["Piminus"] = v->Piminus_ProbNNpi;
  }

  sort( ProbNNk.begin(), ProbNNk.end(), &sortFunc );

  bool passKaon = false;
  // kaon - i.e. one with the highest ProbNNk
  TString kaonName = ProbNNk[0].first;
  double kaonVal = ProbNNk[0].second;
  if ( kaonVal>0.2 && kaonVal*(1-ProbNNpi[kaonName])>0.3 ) {
    passKaon = true;
  }

  // this is the others
  bool passPion=true;
  for (int i=1; i<4; i++ ) { // these are now the "pions"
    TString name = ProbNNk[i].first;
    if ( ProbNNpi[name]< 0.2 || ProbNNpi[name]*(1.-ProbNNk[i].second)<0.3 ) {
      passPion = false;
    }
  }

  // then mass veto
  bool passMassVeto = false;
  if ( TMath::Abs( v->B_s0_M_PipPimKmPip-5279.53 )<30. || TMath::Abs( v->B_s0_M_KpPimPimPip-5279.53 )<30. ) {
    passMassVeto = true;
  }

  if ( passKaon && passPion && passMassVeto ) {
    v->pass_rhokst = true;
  }

  v->pass_massveto = false;
  if ( TMath::Abs( v->M_KpKm - 1850 )<30. || TMath::Abs( v->M_PipPim - 782)<30. ) {
    v->pass_massveto = true;
  }

  passedEvents++;
  return true;
}


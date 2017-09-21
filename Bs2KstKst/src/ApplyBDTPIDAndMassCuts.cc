#include "TLorentzVector.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

#include "Utils.h"

#include "ApplyBDTPIDAndMassCuts.h"

using namespace std;
using namespace Utils;

Bs2KstKst::ApplyBDTPIDAndMassCuts::ApplyBDTPIDAndMassCuts(TString _name, Variables_Analysis *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::ApplyBDTPIDAndMassCuts::~ApplyBDTPIDAndMassCuts(){}

bool sortFunc( pair<TString,double> A, pair<TString,double> B ) {
  if ( A.second > B.second ) return true;
  else return false;
}

bool Bs2KstKst::ApplyBDTPIDAndMassCuts::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //

  // 1.) First apply the BDT cut
  v->pass_bdt = false;
  if ( v->year == "2011" ) {
    if ( v->bdtoutput           > 0.0    ) v->pass_bdt = true;
  }
  else if ( v->year == "2012" ) {
    if ( v->bdtoutput           > 0.0    ) v->pass_bdt = true;
  }
  else {
    error ( "v->year not recognised: "+v->year );
  }
  v->pass_pid = true;

  // 2.) Second apply the PID cuts
  if ( v->min_kaon_ProbNNk < 0.2 )   v->pass_pid = false;
  if ( v->min_pion_ProbNNpi < 0.2 )  v->pass_pid = false;
  if ( v->min_kaon_ProbNNKpi < 0.3 ) v->pass_pid = false;
  if ( v->min_pion_ProbNNpiK < 0.3 ) v->pass_pid = false;

  // 3.) Apply a cut for Lambdas
  double LambdaB_mass = 5620.2;
  v->pass_lambdab = false;
  if ( (TMath::Abs( v->B_s0_M_pPimKmPip - LambdaB_mass) <= 30.) && Kplus_ProbNNp>0.4 ) { // if Kplus switched with proton and near Lambda_b mass apply tight proton PID
    v->pass_lambdab = true;
  }
  if ( (TMath::Abs( v->B_s0_M_KpPimpbPip - LambdaB_mass) <=30.) && Kminus_ProbNNp>0.4 ) { // if Kminus switched with antiproton and near Lambda_b mass apply tight proton PID
    v->pass_lambdab = true;
  }

  // 4.) Apply a cut for Bd(s) -> RhoKst contributions
  double Bd_mass = 5279.53
  v->pass_rhokst = false;
  if ( (TMath::Abs( v->B_s0_M_PipPimKmPip - Bd_mass) <= 30.) && ( Kplus_ProbNNpi>0.2 || (Kplus_ProbNNpi*(1.-Kplus_ProbNNk))>0.3 ) ) { // if Kplus switched with pion and near Bd mass apply pion PID cut on kaon
    v->pass_rhokst = true;
  }
  if ( (TMath::Abs( v->B_s0_M_KpPimPimPip - Bd_mass) <= 30.) && ( Kminus_ProbNNpi>0.2 || (Kminus_ProbNNpi*(1.-Kminus_ProbNNk))>0.3 ) ) { // if Kminus switched with pion and near Bd mass apply pion PID cut on kaon
    v->pass_rhokst = true;
  }

  // 5.) Apply mass vetoes for part_reco stuff

  // 4-body mass vetoes
  v->pass_massveto = false;
  // very-tight vetoes:
  //if ( v->B_s0_M_KpPimPimPip < 5000 || v->B_s0_M_KpPimPimPip > 5400 ) v->pass_massveto = true;
  //if ( v->B_s0_M_PipPimKmPip < 5000 || v->B_s0_M_PipPimKmPip > 5400 ) v->pass_massveto = true;
  //if ( v->B_s0_M_KpPimpbPip  < 5400 || v->B_s0_M_KpPimpbPip  > 6200 ) v->pass_massveto = true;
  //if ( v->B_s0_M_pPimKmPip   < 5400 || v->B_s0_M_pPimKmPip   > 6200 ) v->pass_massveto = true;
  //if ( v->B_s0_M_KpPimKmKp   < 5350 || v->B_s0_M_KpPimKmKp   > 5800 ) v->pass_massveto = true;
  //if ( v->B_s0_M_KpKmKmPip   < 5350 || v->B_s0_M_KpKmKmPip   > 5800 ) v->pass_massveto = true;
  // looser vetoes (help keep some components for fitting)
  if ( v->B_s0_M_KpPimpbPip  < 5350 ) v->pass_massveto = true;
  if ( v->B_s0_M_pPimKmPip   < 5350 ) v->pass_massveto = true;
  if ( v->B_s0_M_KpPimKmKp   < 5270 ) v->pass_massveto = true;
  if ( v->B_s0_M_KpKmKmPip   < 5270 ) v->pass_massveto = true;

  // 2-body mass vetoes
  if ( TMath::Abs( v->M_KpKm - 1850 )<30. || TMath::Abs( v->M_PipPim - 782)<30. ) {
    v->pass_massveto = true;
  }

  // 6.) Finally put everything together into a single veto variable
  v->pass_bdtpidmass = false;

  if ( v->pass_bdt && v->pass_pid && !v->pass_lambdab && !v->pass_rhokst && !v->pass_massveto ) v->pass_bdtpidmass = true;

  passedEvents++;
  return true;
}

// Old RhoKst veto logic
//
//vector< pair<TString,double> > ProbNNk;
//if ( v->itype < 0 ) { // MC
//ProbNNk.push_back( make_pair("Kplus", v->Kplus_ProbNNk_corr) );
//ProbNNk.push_back( make_pair("Kminus", v->Kminus_ProbNNk_corr) );
//ProbNNk.push_back( make_pair("Piplus", v->Piplus_ProbNNk_corr) );
//ProbNNk.push_back( make_pair("Piminus", v->Piminus_ProbNNk_corr) );
//}
//else { // Data
//ProbNNk.push_back( make_pair("Kplus", v->Kplus_ProbNNk) );
//ProbNNk.push_back( make_pair("Kminus", v->Kminus_ProbNNk) );
//ProbNNk.push_back( make_pair("Piplus", v->Piplus_ProbNNk) );
//ProbNNk.push_back( make_pair("Piminus", v->Piminus_ProbNNk) );
//}
//
//map<TString,double> ProbNNpi;
//if ( v->itype < 0 ) { // MC
//ProbNNpi["Kplus"]   = v->Kplus_ProbNNpi_corr;
//ProbNNpi["Kminus"]  = v->Kminus_ProbNNpi_corr;
//ProbNNpi["Piplus"]  = v->Piplus_ProbNNpi_corr;
//ProbNNpi["Piminus"] = v->Piminus_ProbNNpi_corr;
//}
//else { // Data
//ProbNNpi["Kplus"]   = v->Kplus_ProbNNpi;
//ProbNNpi["Kminus"]  = v->Kminus_ProbNNpi;
//ProbNNpi["Piplus"]  = v->Piplus_ProbNNpi;
//ProbNNpi["Piminus"] = v->Piminus_ProbNNpi;
//}
//
//sort( ProbNNk.begin(), ProbNNk.end(), &sortFunc );
//
//bool passKaon = false;
//// kaon - i.e. one with the highest ProbNNk
//TString kaonName = ProbNNk[0].first;
//double kaonVal = ProbNNk[0].second;
//if ( kaonVal>0.2 && kaonVal*(1-ProbNNpi[kaonName])>0.3 ) {
//passKaon = true;
//}
//
//// this is the others
//bool passPion=true;
//for (int i=1; i<4; i++ ) { // these are now the "pions"
//TString name = ProbNNk[i].first;
//if ( ProbNNpi[name]< 0.2 || ProbNNpi[name]*(1.-ProbNNk[i].second)<0.3 ) {
//passPion = false;
//}
//}


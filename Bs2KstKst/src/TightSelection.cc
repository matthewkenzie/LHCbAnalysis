/////////////////////////////////////
//                                 //
// TightSelection.cc              	 //
// Author: Matthew Kenzie          //
// Auto-generated                  //
// Will execute analysis pre-sel   //
//                                 //
/////////////////////////////////////

#include "TMath.h"
#include "TLorentzVector.h"
#include "TightSelection.h"

using namespace std;

Bs2KstKst::TightSelection::TightSelection(TString _name, const Variables_Analysis *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::TightSelection::~TightSelection(){}

void Bs2KstKst::TightSelection::Init(){
  cout << Form("%-30s","Bs2KstKst::TightSelection::Init()") << " " << "Initialising Analyser (" << name << ")." << endl;
}

void Bs2KstKst::TightSelection::Term(){
  cout << Form("%-30s","Bs2KstKst::TightSelection::Term()") << " " << "Terminating Analyser (" << name << ")." << endl;
}

bool Bs2KstKst::TightSelection::AnalyseEvent(){

  // -----------------------------------------------------------------------
  // do physics cuts here:
  //
  // MC only
  //if ( l->itype < 0 ) {
  //}

  // Data only
  if ( v->itype > 0 ) {

    // B_s0 Cuts
    if ( v->B_s0_DIRA_OWNPV < 0.999 ) return false;
    if ( (v->B_s0_ENDVERTEX_CHI2 / v->B_s0_ENDVERTEX_NDOF ) > 9 ) return false;
    if ( v->B_s0_TAU < 0 ) return false;
    ////// mass window
    if ( v->B_s0_DTF_B_s0_M < 5000. || v->B_s0_DTF_B_s0_M > 5800. ) return false;

    //// Kst cuts
    if ( (v->Kst_ENDVERTEX_CHI2  / v->Kst_ENDVERTEX_NDOF  )  > 9 ) return false;
    if ( (v->Kstb_ENDVERTEX_CHI2 / v->Kstb_ENDVERTEX_NDOF ) > 9 )  return false;
    //if ( v->Kst_PT  < 1200 ) return false;
    //if ( v->Kstb_PT < 1200 ) return false;
    if ( v->B_s0_DTF_KST1_M  < 896.-100. || v->B_s0_DTF_KST1_M  > 896.+100. ) return false;
    if ( v->B_s0_DTF_KST2_M < 896.-100. || v->B_s0_DTF_KST2_M > 896.+100. ) return false;

    //// 3-body mass
    //if ( v->M_KKPiPlus < 2200 || v->M_KKPiMinus < 2200 ) return false;

    //// Track cuts
    ////// chi2/dof
    if ( v->Kplus_TRACK_CHI2NDOF   > 3 ) return false;
    if ( v->Kminus_TRACK_CHI2NDOF  > 3 ) return false;
    if ( v->Piplus_TRACK_CHI2NDOF  > 3 ) return false;
    if ( v->Piminus_TRACK_CHI2NDOF > 3 ) return false;
    ////// PT
    if ( v->Kplus_PT             < 500 ) return false;
    if ( v->Kminus_PT            < 500 ) return false;
    if ( v->Piplus_PT            < 500 ) return false;
    if ( v->Piminus_PT           < 500 ) return false;
    ////// PID
    if ( v->Kplus_ProbNNk        < 0.5 ) return false;
    if ( v->Kminus_ProbNNk       < 0.5 ) return false;
    if ( v->Kplus_ProbNNp        > 0.8 ) return false;
    if ( v->Kminus_ProbNNp       > 0.8 ) return false;
    if ( v->Piplus_ProbNNpi      < 0.5 ) return false;
    if ( v->Piminus_ProbNNpi     < 0.5 ) return false;
    if ( v->Kplus_isMuon               ) return false;
    if ( v->Kminus_isMuon              ) return false;
    if ( v->Piplus_isMuon              ) return false;
    if ( v->Piminus_isMuon             ) return false;
    // -----------------------------------------------------------------------

  }

  return true;
}


#include "TMath.h"
#include "TLorentzVector.h"

#include "PreSelection.h"

using namespace std;

Bs2KstKst::PreSelection::PreSelection(TString _name, Variables_PreSel *_v):
	Analyser(_name),
  v(_v)
{}

Bs2KstKst::PreSelection::~PreSelection(){}

bool Bs2KstKst::PreSelection::AnalyseEvent(){

  // -----------------------------------------------------------------------
	// add new variables
  TLorentzVector B_s0_p4;
	TLorentzVector Kst_p4;
	TLorentzVector Kstb_p4;
	TLorentzVector Kplus_p4;
	TLorentzVector Kminus_p4;
	TLorentzVector Piplus_p4;
	TLorentzVector Piminus_p4;

  B_s0_p4   .SetXYZM( v->B_s0_PX,    v->B_s0_PY,    v->B_s0_PZ,    v->B_s0_M ) ;
  Kst_p4    .SetXYZM( v->Kst_PX,     v->Kst_PY,     v->Kst_PZ,     v->Kst_M ) ;
  Kstb_p4   .SetXYZM( v->Kstb_PX,    v->Kstb_PY,    v->Kstb_PZ,    v->Kstb_M ) ;
  Kplus_p4  .SetXYZM( v->Kplus_PX,   v->Kplus_PY,   v->Kplus_PZ,   v->Kplus_M ) ;
  Kminus_p4 .SetXYZM( v->Kminus_PX,  v->Kminus_PY,  v->Kminus_PZ,  v->Kminus_M ) ;
  Piplus_p4 .SetXYZM( v->Piplus_PX,  v->Piplus_PY,  v->Piplus_PZ,  v->Piplus_M ) ;
  Piminus_p4.SetXYZM( v->Piminus_PX, v->Piminus_PY, v->Piminus_PZ, v->Piminus_M ) ;

	v->B_s0_ETA    = B_s0_p4.Eta();
	v->Kst_ETA     = Kst_p4.Eta();
	v->Kstb_ETA    = Kstb_p4.Eta();
	v->Kplus_ETA   = Kplus_p4.Eta();
	v->Kminus_ETA  = Kminus_p4.Eta();
	v->Piplus_ETA  = Piplus_p4.Eta();
	v->Piminus_ETA = Piminus_p4.Eta();

	v->M_KKPiPlus  = (Kplus_p4 + Kminus_p4 + Piplus_p4).M();
	v->M_KKPiMinus = (Kplus_p4 + Kminus_p4 + Piminus_p4).M();

  v->max_track_chi2 = TMath::Max( TMath::Max( v->Kplus_TRACK_CHI2NDOF, v->Kminus_TRACK_CHI2NDOF) , TMath::Max( v->Piplus_TRACK_CHI2NDOF, v->Piminus_TRACK_CHI2NDOF) );

  // add DTF version of new variables

  B_s0_p4   .SetXYZM( v->B_s0_DTF_B_s0_PX,    v->B_s0_DTF_B_s0_PY,    v->B_s0_DTF_B_s0_PZ,    v->B_s0_DTF_B_s0_M  ) ;
  Kst_p4    .SetXYZM( v->B_s0_DTF_KST1_PX,    v->B_s0_DTF_KST1_PY,    v->B_s0_DTF_KST1_PZ,    v->B_s0_DTF_KST1_M  ) ;
  Kstb_p4   .SetXYZM( v->B_s0_DTF_KST2_PX,    v->B_s0_DTF_KST2_PY,    v->B_s0_DTF_KST2_PZ,    v->B_s0_DTF_KST2_M  ) ;

  v->B_s0_DTF_B_s0_ETA    = B_s0_p4.Eta();
	v->B_s0_DTF_Kst_ETA     = Kst_p4.Eta();
	v->B_s0_DTF_Kstb_ETA    = Kstb_p4.Eta();

  // -----------------------------------------------------------------------


  // -----------------------------------------------------------------------
  // do physics cuts here:
  //
  // B_s0 Cuts   ( 97% eff )
	if ( v->B_s0_DIRA_OWNPV < 0.99 ) return false;                                // stripping is 0.99
  if ( (v->B_s0_ENDVERTEX_CHI2 / v->B_s0_ENDVERTEX_NDOF ) > 12 ) return false; // in stripping
	if ( v->B_s0_DTF_CTAU < 0 ) return false;                                          // sanity
	////// mass window
  if ( v->B_s0_DTF_B_s0_M < 5000 || v->B_s0_DTF_B_s0_M > 5800 ) return false;                  // mass window

  //// Kst cuts    ( 99% eff )
  if ( (v->Kst_ENDVERTEX_CHI2  / v->Kst_ENDVERTEX_NDOF  )  > 12 ) return false; // in stripping
  if ( (v->Kstb_ENDVERTEX_CHI2 / v->Kstb_ENDVERTEX_NDOF ) > 12 )  return false; // in stripping
	// not using DTF variables here because want to keep inside stripping
	if ( v->Kst_PT  < 900 ) return false;                                         // in stripping
	if ( v->Kstb_PT < 900 ) return false;                                         // in stripping
  if ( v->Kst_MM  < 750 || v->Kst_MM  > 1700 ) return false;                   // stripping in range (740,2100)
  if ( v->Kstb_MM < 750 || v->Kstb_MM > 1700 ) return false;                   // stripping in range (740,2100)

  //// 3-body mass ( 99% eff )
  if ( v->M_KKPiPlus < 2100 || v->M_KKPiMinus < 2100 ) return false;           // not in stripping

  //// Track cuts
  ////// chi2/dof  ( 100% eff )
  if ( v->Kplus_TRACK_CHI2NDOF   > 4 ) return false;                            // in stripping
  if ( v->Kminus_TRACK_CHI2NDOF  > 4 ) return false;                            // in stripping
  if ( v->Piplus_TRACK_CHI2NDOF  > 4 ) return false;                            // in stripping
  if ( v->Piminus_TRACK_CHI2NDOF > 4 ) return false;                            // in stripping
  ////// PT        ( 100% eff )
	if ( v->Kplus_PT             < 500 ) return false;                            // in stripping
	if ( v->Kminus_PT            < 500 ) return false;                            // in stripping
	if ( v->Piplus_PT            < 500 ) return false;                            // in stripping
	if ( v->Piminus_PT           < 500 ) return false;                            // in stripping
  ////// PID       ( 98% eff )
  //if ( v->Kplus_ProbNNk        < 0.0 ) return false;                            // not in stripping
  //if ( v->Kminus_ProbNNk       < 0.0 ) return false;                            // not in stripping
  //if ( v->Kplus_ProbNNp        < 0.0 ) return false;                            // not in stripping
  //if ( v->Kminus_ProbNNp       < 0.0 ) return false;                            // not in stripping
  //if ( v->Piplus_ProbNNpi      < 0.0 ) return false;                            // not in stripping
  //if ( v->Piminus_ProbNNpi     < 0.0 ) return false;                            // not in stripping
  //if ( v->Kplus_isMuon               ) return false;                            // not in stripping
  //if ( v->Kminus_isMuon              ) return false;                            // not in stripping
  if ( v->Piplus_isMuon              ) return false;                            // not in stripping
  if ( v->Piminus_isMuon             ) return false;                            // not in stripping
  // -----------------------------------------------------------------------

	// MC only
	//if ( l->itype < 0 ) {
	//}
	// Data only
	//if ( l->itype > 0 ) {
	//}

	return true;
}


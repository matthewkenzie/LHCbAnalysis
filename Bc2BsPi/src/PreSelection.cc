#include "TMath.h"
#include "TLorentzVector.h"

#include "PreSelection.h"

using namespace std;

Bc2BsPi::PreSelection::PreSelection(TString _name, Variables_PreSel *_v):
	Analyser(_name),
  v(_v)
{}

Bc2BsPi::PreSelection::~PreSelection(){}

bool Bc2BsPi::PreSelection::AnalyseEvent(){

  // -----------------------------------------------------------------------
  // add some new variables
  v->B_c_B_s0_VTX_DIFF_X  = v->B_c_ENDVERTEX_X - v->B_s0_ENDVERTEX_X ;
  v->B_c_B_s0_VTX_DIFF_Y  = v->B_c_ENDVERTEX_Y - v->B_s0_ENDVERTEX_Y ;
  v->B_c_B_s0_VTX_DIFF_Z  = v->B_c_ENDVERTEX_Z - v->B_s0_ENDVERTEX_Z ;
  v->B_c_B_s0_VTX_DIFF_XY = TMath::Sqrt( (v->B_c_B_s0_VTX_DIFF_X*v->B_c_B_s0_VTX_DIFF_X) + (v->B_c_B_s0_VTX_DIFF_Y*v->B_c_B_s0_VTX_DIFF_Y) );
  v->B_c_B_s0_VTX_DIFF_R  = TMath::Sqrt( (v->B_c_B_s0_VTX_DIFF_X*v->B_c_B_s0_VTX_DIFF_X) + (v->B_c_B_s0_VTX_DIFF_Y*v->B_c_B_s0_VTX_DIFF_Y) + (v->B_c_B_s0_VTX_DIFF_Z*v->B_c_B_s0_VTX_DIFF_Z) );

  v->B_c_B_s0_VTX_DIFF_XERR = TMath::Sqrt( (v->B_c_ENDVERTEX_XERR*v->B_c_ENDVERTEX_XERR) + (v->B_s0_ENDVERTEX_XERR*v->B_s0_ENDVERTEX_XERR) );
  v->B_c_B_s0_VTX_DIFF_YERR = TMath::Sqrt( (v->B_c_ENDVERTEX_YERR*v->B_c_ENDVERTEX_YERR) + (v->B_s0_ENDVERTEX_YERR*v->B_s0_ENDVERTEX_YERR) );
  v->B_c_B_s0_VTX_DIFF_ZERR = TMath::Sqrt( (v->B_c_ENDVERTEX_ZERR*v->B_c_ENDVERTEX_ZERR) + (v->B_s0_ENDVERTEX_ZERR*v->B_s0_ENDVERTEX_ZERR) );

  v->B_c_B_s0_VTX_DIFF_XYERR = TMath::Sqrt( (v->B_c_B_s0_VTX_DIFF_X*v->B_c_B_s0_VTX_DIFF_X)*(v->B_c_B_s0_VTX_DIFF_XERR*v->B_c_B_s0_VTX_DIFF_XERR) + (v->B_c_B_s0_VTX_DIFF_Y*v->B_c_B_s0_VTX_DIFF_Y)*(v->B_c_B_s0_VTX_DIFF_YERR*v->B_c_B_s0_VTX_DIFF_YERR) ) / v->B_c_B_s0_VTX_DIFF_XY;
  v->B_c_B_s0_VTX_DIFF_RERR  = TMath::Sqrt( (v->B_c_B_s0_VTX_DIFF_X*v->B_c_B_s0_VTX_DIFF_X)*(v->B_c_B_s0_VTX_DIFF_XERR*v->B_c_B_s0_VTX_DIFF_XERR) + (v->B_c_B_s0_VTX_DIFF_Y*v->B_c_B_s0_VTX_DIFF_Y)*(v->B_c_B_s0_VTX_DIFF_YERR*v->B_c_B_s0_VTX_DIFF_YERR) + (v->B_c_B_s0_VTX_DIFF_Z*v->B_c_B_s0_VTX_DIFF_Z)*(v->B_c_B_s0_VTX_DIFF_ZERR*v->B_c_B_s0_VTX_DIFF_ZERR) ) / v->B_c_B_s0_VTX_DIFF_R;

  v->B_c_B_s0_VTX_DIFF_XFRACERR  = v->B_c_B_s0_VTX_DIFF_XFRACERR  / v->B_c_B_s0_VTX_DIFF_X ;
  v->B_c_B_s0_VTX_DIFF_YFRACERR  = v->B_c_B_s0_VTX_DIFF_YFRACERR  / v->B_c_B_s0_VTX_DIFF_Y ;
  v->B_c_B_s0_VTX_DIFF_ZFRACERR  = v->B_c_B_s0_VTX_DIFF_ZFRACERR  / v->B_c_B_s0_VTX_DIFF_Z ;
  v->B_c_B_s0_VTX_DIFF_XYFRACERR = v->B_c_B_s0_VTX_DIFF_XYFRACERR / v->B_c_B_s0_VTX_DIFF_X ;
  v->B_c_B_s0_VTX_DIFF_RFRACERR  = v->B_c_B_s0_VTX_DIFF_RFRACERR  / v->B_c_B_s0_VTX_DIFF_R ;

  // B_c Cuts
  //if ( v->B_c_AllDIRA[thePV] < 0.99 ) return false;
  if ( v->B_c_DIRA_OWNPV < 0.99 ) return false;
  if ( (v->B_c_ENDVERTEX_CHI2 / v->B_c_ENDVERTEX_NDOF ) > 5 ) return false;
  //if ( v->B_c_ConsBc_ctau[thePV] < 0 ) return false;
  //if ( v->B_c_ConsBc_M[thePV] < 5800 || v->B_c_ConsBc_M[thePV] > 6600 ) return false;
  if ( v->B_c_M < 5800 || v->B_c_M > 6600 ) return false;
  if ( v->B_c_FD_OWNPV > 15 ) return false;
  //if ( v->B_c_AllIPchi2[thePV] > 20 ) return false;
  //if ( v->B_c_AllIPchi2[thePV] > 20 ) return false;
  if ( v->B_c_P < 30000 ) return false;
  if ( v->B_c_PT < 500 ) return false;
  if ( v->B_c_B_s0_VTX_DIFF_R < -40 || v->B_c_B_s0_VTX_DIFF_R > 50 ) return false;

  // B_s0 Cuts   ( 97% eff )
  //if ( v->B_s0_AllDIRA[thePV] < 0.999 ) return false;
	if ( v->B_s0_DIRA_OWNPV < 0.999 ) return false;
  if ( (v->B_s0_ENDVERTEX_CHI2 / v->B_s0_ENDVERTEX_NDOF ) > 2 ) return false;
  //if ( v->B_s0_ConsBs_M[thePV] < 5200 || v->B_s0_ConsBs_M[thePV] > 5500 ) return false;
  if ( v->B_s0_M < 5200 || v->B_s0_M > 5500 ) return false;
  if ( v->B_s0_P < 20000 ) return false;
  if ( v->B_s0_PT < 250 ) return false;

  // Bachelor cuts
  if ( v->Pibach_isMuon             ) return false;
  if ( v->Pibach_TRACK_CHI2NDOF > 3 ) return false;
  //if ( v->Pibach_PT < 500           ) return false;
  //if ( v->Pibach_P  < 1000          ) return false;

  // J/psi cuts
  if ( ( v->Jpsi_ENDVERTEX_CHI2 / v->Jpsi_ENDVERTEX_NDOF ) > 8 ) return false;
  if ( v->Jpsi_PT < 500 ) return false;
  if ( v->Jpsi_P  < 10000 ) return false;

  // phi cuts
  if ( ( v->phi_ENDVERTEX_CHI2 / v->phi_ENDVERTEX_NDOF ) > 8 ) return false;
  if ( v->phi_PT < 1000 ) return false; // in stripping
  if ( v->phi_P  < 5000 ) return false;

  // Daughter cuts
  if ( v->Kplus_TRACK_CHI2NDOF   > 3 ) return false;
  if ( v->Kminus_TRACK_CHI2NDOF  > 3 ) return false;
  if ( v->muplus_TRACK_CHI2NDOF  > 3 ) return false;
  if ( v->muminus_TRACK_CHI2NDOF > 3 ) return false;
	if ( v->Kplus_PT             < 500 ) return false;
	if ( v->Kminus_PT            < 500 ) return false;
	if ( v->muplus_PT            < 500 ) return false;
	if ( v->muminus_PT           < 500 ) return false;
  if ( v->Kplus_isMuon              ) return false;
  if ( v->Kminus_isMuon             ) return false;
  if ( !v->muplus_isMuon            ) return false;
  if ( !v->muminus_isMuon           ) return false;

	// MC only
	//if ( v->itype < 0 ) {
  //}
	// Data only
	//if ( v->itype > 0 ) {
	//}

	return true;
}


#include "TMath.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include "AddAngles.h"

using namespace std;
using namespace TMath;

Bs2KstKst::AddAngles::AddAngles(TString _name, Variables_Analysis *_v):
	Analyser(_name),
  v(_v)
{}

Bs2KstKst::AddAngles::~AddAngles(){}

bool Bs2KstKst::AddAngles::AnalyseEvent(){

  // Add helicity angles and planar angle for decays of type X -> P1P2 -> (p11p12) (p21p22) a la PDG
  //
  Double_t K_mass  = 493.677;
  Double_t PI_mass = 139.57018;

	TLorentzVector p11;
	TLorentzVector p12;
	TLorentzVector p21;
	TLorentzVector p22;

  p11.SetXYZM(  v->B_s0_DTF_KST1_K_PX  , v->B_s0_DTF_KST1_K_PY  , v->B_s0_DTF_KST1_K_PZ  , K_mass  );
  p12.SetXYZM(  v->B_s0_DTF_KST1_PI_PX , v->B_s0_DTF_KST1_PI_PY , v->B_s0_DTF_KST1_PI_PZ , PI_mass );
  p21.SetXYZM(  v->B_s0_DTF_KST2_K_PX  , v->B_s0_DTF_KST2_K_PY  , v->B_s0_DTF_KST2_K_PZ  , K_mass  );
  p22.SetXYZM(  v->B_s0_DTF_KST2_PI_PX , v->B_s0_DTF_KST2_PI_PY , v->B_s0_DTF_KST2_PI_PZ , PI_mass );

  //

  TLorentzVector P1 = p11 + p12;
  TLorentzVector P2 = p21 + p22;

  TLorentzVector X = p11 + p12 + p21 + p22;

  // boost everything into X frame (usually B frame)

  TVector3 X_boost_vec = X.BoostVector();

  TLorentzVector boostedX_X   ( X );   boostedX_X.Boost( -X_boost_vec );
  TLorentzVector boostedX_P1  ( P1 );  boostedX_P1.Boost( -X_boost_vec );
  TLorentzVector boostedX_P2  ( P2 );  boostedX_P2.Boost( -X_boost_vec );
  TLorentzVector boostedX_p11 ( p11 ); boostedX_p11.Boost( -X_boost_vec );
  TLorentzVector boostedX_p12 ( p12 ); boostedX_p12.Boost( -X_boost_vec );
  TLorentzVector boostedX_p21 ( p21 ); boostedX_p21.Boost( -X_boost_vec );
  TLorentzVector boostedX_p22 ( p22 ); boostedX_p22.Boost( -X_boost_vec );

  // boost p11 into P1 frame and p21 into P2 frame (as we take these as two angles)
  TVector3 P1_boost_vec = boostedX_P1.BoostVector();
  TVector3 P2_boost_vec = boostedX_P2.BoostVector();

  TLorentzVector boostedP1_p11 ( boostedX_p11 ); boostedP1_p11.Boost( -P1_boost_vec );
  TLorentzVector boostedP2_p21 ( boostedX_p21 ); boostedP2_p21.Boost( -P2_boost_vec );

  // get the angles between these
  v->B_s0_DTF_B_s0_CosTheta1 = Cos( boostedX_P1.Angle( boostedP1_p11.Vect() ) );
  v->B_s0_DTF_B_s0_CosTheta2 = Cos( boostedX_P2.Angle( boostedP2_p21.Vect() ) );

  // now define some unit vectors for the phi planar angle
  TVector3 boostedX_p11_unit = boostedX_p11.Vect().Unit();
  TVector3 boostedX_p12_unit = boostedX_p12.Vect().Unit();
  TVector3 boostedX_p21_unit = boostedX_p21.Vect().Unit();
  TVector3 boostedX_p22_unit = boostedX_p22.Vect().Unit();

  // get normals of two decay planes
  TVector3 e1 = ( boostedX_p11_unit.Cross( boostedX_p12_unit ) ).Unit();
  TVector3 e2 = ( boostedX_p21_unit.Cross( boostedX_p22_unit ) ).Unit();

  // dir of P1 in B frame
  TVector3 ez = boostedX_P1.Vect().Unit();

  // planar angle
  double cosPhi = e1.Dot(e2);
  double sinPhi = (e2.Cross(e1)).Dot(ez);
  double phi = ACos(cosPhi) - Pi();

  v->B_s0_DTF_B_s0_Phi1 = sinPhi > 0. ? phi : -phi;

  // add in DTF proper time and error in fs
  v->B_s0_DTF_TAU    = v->B_s0_DTF_CTAU/(TMath::C()/(1.e9)) ;
  v->B_s0_DTF_TAUERR = v->B_s0_DTF_CTAUERR/(TMath::C()/(1.e9)) ;

  return true;
}



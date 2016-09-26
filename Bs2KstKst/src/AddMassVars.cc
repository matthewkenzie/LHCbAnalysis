#include "TLorentzVector.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

#include "AddMassVars.h"

using namespace std;

Bs2KstKst::AddMassVars::AddMassVars(TString _name, Variables_Analysis *_v):
  Analyser(_name),
  v(_v)
{}

Bs2KstKst::AddMassVars::~AddMassVars(){}

bool Bs2KstKst::AddMassVars::AnalyseEvent() {

  processedEvents ++;

  //
  // Exectute actions
  //

  Double_t K_mass  = 493.677;
  Double_t Pi_mass = 139.57018;

	TLorentzVector Kplus_p4;
	TLorentzVector Kminus_p4;
	TLorentzVector Piplus_p4;
	TLorentzVector Piminus_p4;

  // standard 4-vectors
  Kminus_p4 .SetXYZM( v->Kminus_PX  , v->Kminus_PY  , v->Kminus_PZ  , K_mass  );
  Kplus_p4  .SetXYZM( v->Kplus_PX   , v->Kplus_PY   , v->Kplus_PZ   , K_mass  );
  Piminus_p4.SetXYZM( v->Piminus_PX , v->Piminus_PY , v->Piminus_PZ , Pi_mass );
  Piplus_p4 .SetXYZM( v->Piplus_PX  , v->Piplus_PY  , v->Piplus_PZ  , Pi_mass );

  // 2 and 3-body masses
  v->M_KpKmPip = ( Kplus_p4 + Kminus_p4 + Piplus_p4 ).M();
  v->M_KpKmPim = ( Kplus_p4 + Kminus_p4 + Piminus_p4).M();
  v->M_KpPipPim = ( Kplus_p4 + Piplus_p4 + Piminus_p4).M();
  v->M_KmPipPim = ( Kminus_p4 + Piplus_p4 + Piminus_p4).M();
  v->M_KpKm     = ( Kplus_p4 + Kminus_p4 ).M();
  v->M_PipPim   = ( Piplus_p4 + Piminus_p4 ).M();

  // switch Km for Pim
  Kminus_p4.SetVectM( Kminus_p4.Vect(), Pi_mass );
  TLorentzVector B_s0_KpPimPimPip_p4 = Kminus_p4 + Kplus_p4 + Piminus_p4 + Piplus_p4;
  v->B_s0_M_KpPimPimPip = B_s0_KpPimPimPip_p4.M();
  Kminus_p4.SetVectM( Kminus_p4.Vect(), K_mass ); // switch back

  // switch Kp for Pip
  Kplus_p4.SetVectM( Kplus_p4.Vect(), Pi_mass );
  TLorentzVector B_s0_PipPimKmPip_p4 = Kminus_p4 + Kplus_p4 + Piminus_p4 + Piplus_p4;
  v->B_s0_M_PipPimKmPip = B_s0_PipPimKmPip_p4.M();
  Kplus_p4.SetVectM( Kplus_p4.Vect(), K_mass ); // switch back

  // switch Pim for Km
  Piminus_p4.SetVectM( Piminus_p4.Vect(), K_mass );
  TLorentzVector B_s0_KpKmKmPip_p4 = Kminus_p4 + Kplus_p4 + Piminus_p4 + Piplus_p4;
  v->B_s0_M_KpKmKmPip = B_s0_KpKmKmPip_p4.M();
  Piminus_p4.SetVectM( Piminus_p4.Vect(), Pi_mass ); // switch back

  // switch Pip for K
  Piplus_p4.SetVectM( Piplus_p4.Vect(), K_mass );
  TLorentzVector B_s0_KpPimKmKp_p4 = Kminus_p4 + Kplus_p4 + Piminus_p4 + Piplus_p4;
  v->B_s0_M_KpPimKmKp = B_s0_KpPimKmKp_p4.M();
  Piplus_p4.SetVectM( Piplus_p4.Vect(), Pi_mass ); // switch back

  passedEvents++;
  return true;
}

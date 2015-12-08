#include "LifetimeFitter.h"

using namespace std;
using namespace RooFit;

Dst2D0pi::LifetimeFitter::LifetimeFitter(TString wsname, TString name, bool verbose, bool debug):
  FitterBase(wsname,name,verbose,debug)
{}

Dst2D0pi::LifetimeFitter::~LifetimeFitter(){}

void Dst2D0pi::LifetimeFitter::addObsVars(){

  addObsVar("Dst_M",   "m(D*^{+})", "MeV", 1985., 2035.);
  addObsVar("D0_M",    "m(D^{0})",  "MeV", 1830., 1900.);
  addObsVar("D0_LTIME", "t(D^{0})", "ns",  0.00, 0.005);
}

void Dst2D0pi::LifetimeFitter::addCuts(){

  //addCut( "LTIME", double(0.00025), double(0.1) );
}

void Dst2D0pi::LifetimeFitter::addDatasets() {

  addDataset("Data", "Data", 80);

}

void Dst2D0pi::LifetimeFitter::constructPdfs() {

  w->factory( "Gaussian::dst_mass1( Dst_M, dst_mean[2000,2020], dst_sigma1[1,20] )" );
  w->factory( "Gaussian::dst_mass2( Dst_M, dst_mean, dst_sigma2[1,20] )" );
  w->factory( "SUM::dst_mass_sig( dst_f[0.1,1.]*dst_mass1, dst_mass2 )" );
  w->factory( "Bernstein::dst_mass_bkg( Dst_M, {1.,dst_p0[0.,1.]} )" );
  w->factory( "SUM::dst_mass( dst_mass_sy[0,10e5]*dst_mass_sig, dst_mass_by[0,10e5]*dst_mass_bkg )" );

  w->factory( "Gaussian::d0_mass1( D0_M, d0_mean[1850,1880], d0_sigma1[1,20] )" );
  w->factory( "Gaussian::d0_mass2( D0_M, d0_mean, d0_sigma2[1,20] )" );
  w->factory( "SUM::d0_mass_sig( d0_f[0.1,1.]*d0_mass1, d0_mass2 )" );
  w->factory( "Bernstein::d0_mass_bkg( D0_M, {1.,d0_p0[0.,1.]} )" );
  w->factory( "SUM::d0_mass( d0_mass_sy[0,10e5]*d0_mass_sig, d0_mass_by[0,10e5]*d0_mass_bkg )" );

  w->factory( "d0_tau[0,1.]" );
  w->factory( "expr::d0_e( '-1/@0', d0_tau)" );
  w->factory( "Exponential::d0_t( D0_LTIME, d0_e )" );

}

void Dst2D0pi::LifetimeFitter::run() {

  fit("dst_mass", "Data");
  fit("d0_mass", "Data");
  fit("d0_t", "Data", false, 0.00025, 0.005);

}

void Dst2D0pi::LifetimeFitter::makePlots() {

  plot( "Dst_M","Data","dst_mass",2 );
  plot( "D0_M" ,"Data","d0_mass" ,2 );
  plot( "D0_LTIME", "Data", "d0_t", 2);

  PlotComponent pc_data( "Data", "Data" );
  pc_data.setDefaultDataStyle();
  pc_data.noleg = true;

  PlotComponent pc_data_invis("Data", "Data Invis" );
  pc_data_invis.noleg = true;

  PlotComponent pc_pdf_dst_m( "dst_mass", "PDF" );
  pc_pdf_dst_m.setSolidLine(kRed);
  pc_pdf_dst_m.noleg = true;

  PlotComponent pc_pdf_d0_m( "d0_mass", "PDF" );
  pc_pdf_d0_m.setSolidLine(kRed);
  pc_pdf_d0_m.noleg = true;

  PlotComponent pc_pdf_d0_t( "d0_t", "PDF" );
  pc_pdf_d0_t.setSolidLine(kRed);
  pc_pdf_d0_t.noleg = true;

  vector<PlotComponent> plotComps_Dst_M;
  plotComps_Dst_M.push_back(pc_data_invis);
  plotComps_Dst_M.push_back(pc_pdf_dst_m);
  plotComps_Dst_M.push_back(pc_data);

  vector<PlotComponent> plotComps_D0_M;
  plotComps_D0_M.push_back(pc_data_invis);
  plotComps_D0_M.push_back(pc_pdf_d0_m);
  plotComps_D0_M.push_back(pc_data);

  vector<PlotComponent> plotComps_D0_t;
  plotComps_D0_t.push_back(pc_data_invis);
  plotComps_D0_t.push_back(pc_pdf_d0_t);
  plotComps_D0_t.push_back(pc_data);

  setTitle("LHCb");
  setDrawLog(true);
  setResidType(2);
  setRedPull(3);

  plot("Dst_M", plotComps_Dst_M, "Dst_M");
  plot("D0_M",  plotComps_D0_M,  "D0_M" );
  plot("D0_LTIME", plotComps_D0_t, "D0_LTIME");

  setResidType(0);
  setDrawLog(false);
  plot("Dst_M", plotComps_Dst_M, "Dst_M_NoPull");
  plot("D0_M",  plotComps_D0_M,  "D0_M_NoPull" );
  plot("D0_LTIME", plotComps_D0_t, "D0_LTIME_NoPull");

}

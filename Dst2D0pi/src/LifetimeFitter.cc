#include "LifetimeFitter.h"

#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "RooDataHist.h"

using namespace std;
using namespace RooFit;

Dst2D0pi::LifetimeFitter::LifetimeFitter(TString wsname, TString name, bool verbose, bool debug):
  FitterBase(wsname,name,verbose,debug)
{}

Dst2D0pi::LifetimeFitter::~LifetimeFitter(){}

void Dst2D0pi::LifetimeFitter::addObsVars(){

  //addObsVar("Dst_M",       "m(D*^{+})", "MeV", 1950., 2080.);
  addObsVar("D0_M",        "m(D^{0})",  "MeV", 1810., 1910.);
  w->var("D0_M")->setBins(500);
  addObsVar("D0_LTIME_ps", "t(D^{0})",  "ps" , 0.3 , 5.);
  w->var("D0_LTIME_ps")->setBins(470);
}

void Dst2D0pi::LifetimeFitter::addCuts(){

  //addCut( "LTIME", double(0.00025), double(0.1) );
}

void Dst2D0pi::LifetimeFitter::addDatasets() {

  //addDataset("Data", "Data", 80);

}

void Dst2D0pi::LifetimeFitter::constructPdfs() {

  //w->factory( "Gaussian::dst_mass1( Dst_M, dst_mean[2000,2020], dst_sigma1[1,20] )" );
  //w->factory( "Gaussian::dst_mass2( Dst_M, dst_mean, dst_sigma2[1,20] )" );
  //w->factory( "SUM::dst_mass_sig( dst_f[0.1,1.]*dst_mass1, dst_mass2 )" );
  //w->factory( "Bernstein::dst_mass_bkg( Dst_M, {1.,dst_p0[0.,1.]} )" );
  //w->factory( "SUM::dst_mass( dst_mass_sy[0,10e5]*dst_mass_sig, dst_mass_by[0,10e5]*dst_mass_bkg )" );

  //w->factory( "Gaussian::d0_mass1( D0_M, d0_mean[1850,1880], d0_sigma1[1,20] )" );
  //w->factory( "Gaussian::d0_mass2( D0_M, d0_mean, d0_sigma2[1,20] )" );
  //w->factory( "SUM::d0_mass_sig( d0_f[0.1,1.]*d0_mass1, d0_mass2 )" );
  //w->factory( "Bernstein::d0_mass_bkg( D0_M, {1.,d0_p0[0.,1.]} )" );
  //w->factory( "SUM::d0_mass( d0_mass_sy[0,10e5]*d0_mass_sig, d0_mass_by[0,10e5]*d0_mass_bkg )" );

  w->factory( "Bernstein::pdf_mass_bkg( D0_M, { p0[0.,1.],p1[0.,1.], p2[0.,1.] } )" );
  w->factory( "CBShape::pdf_mass_sig1( D0_M, mean[1860,1850,1870], sigma1[2,1,10], alpha1[0.5,0.1,10.], n1[0.1,0.,20.] )" );
  w->factory( "CBShape::pdf_mass_sig2( D0_M, mean[1860,1850,1870], sigma2[2,1,10], alpha2[-0.5,-10.,-0.1], n2[0.1,0.,10.] )" );
  w->factory( "SUM::pdf_mass_sig( f1[0.4,0.01,1.]*pdf_mass_sig1, pdf_mass_sig2 )" );
  w->factory( "SUM::d0_mass( bkg_y[1e3,1e1,1e5]*pdf_mass_bkg, sig_y[1e5,1e3,1e8]*pdf_mass_sig)" );
  w->factory( "expr::d0_e( '-1/@0', d0_tau[0.4,0.,1.] )" );
  w->factory( "Exponential::d0_t( D0_LTIME_ps, d0_e )" );

}

void Dst2D0pi::LifetimeFitter::run() {

  //w->var("Dst_M")->setBins(260);
  //w->var("D0_M")->setBins(220);
  //w->var("D0_LTIME_ps")->setBins(200);
  //makeDataHist( "Data", "DataHist" );

  TFile *tf = TFile::Open("root/AnalysisOutEos.root");
  TTree *tree = (TTree*)tf->Get("AnalysisTree");
  TString cuts("D0_LTIME_ps>0.3 && D0_MINIPCHI2<4");

  TH1F *th1f_mass = new TH1F("hmass","",500,1810,1910);
  TH1F *th1f_time = new TH1F("htime","",470,0.3,5.);

  tree->Draw("D0_M>>hmass",cuts,"goff");
  tree->Draw("D0_LTIME_ps>>htime",cuts,"goff");

  RooDataHist *dhist_mass = new RooDataHist("dmass","",RooArgList(*w->var("D0_M")), th1f_mass);
  RooDataHist *dhist_time = new RooDataHist("dtime","",RooArgList(*w->var("D0_LTIME_ps")), th1f_time);

  w->import(*dhist_mass);
  w->import(*dhist_time);

  delete dhist_mass;
  delete dhist_time;
  delete th1f_mass;
  delete th1f_time;

  //fit("dst_mass", "Data", false, 1970, 2050);
  //fit("d0_mass", "Data" , false, 1820,1910);
  //fit("d0_t", "Data", false, 0.275, 5.);

  fit("d0_mass", "dmass" );
  fit("d0_t", "dtime" );
}

void Dst2D0pi::LifetimeFitter::makePlots() {

  //plot( "Dst_M","Data","dst_mass", 2 , "", false, 1970, 2050 );
  plot( "D0_M" ,"dmass","d0_mass" , 2 , "", false, 1810, 1910 );
  plot( "D0_LTIME_ps", "dtime", "d0_t", 2, "", false, 0.,5.   );

  PlotComponent pc_data_m( "dmass", "Data" );
  pc_data_m.setDefaultDataStyle();
  pc_data_m.noleg = true;

  PlotComponent pc_data_m_invis("dmass", "Data Invis" );
  pc_data_m_invis.noleg = true;

  PlotComponent pc_data_t( "dtime", "Data" );
  pc_data_t.setDefaultDataStyle();
  pc_data_t.noleg = true;

  PlotComponent pc_data_t_invis("dtime", "Data Invis" );
  pc_data_t_invis.noleg = true;

  //PlotComponent pc_pdf_dst_m( "dst_mass", "PDF" );
  //rk/gangadir/workspace/mkenzie/LocalXML/282/0/output/TurboOut.root
  //pc_pdf_dst_m.setSolidLine(kRed);
  //pc_pdf_dst_m.noleg = true;

  PlotComponent pc_pdf_d0_m_bkg( "d0_mass:pdf_mass_bkg", "PDF" );
  pc_pdf_d0_m_bkg.setSolidLine(kBlue);
  pc_pdf_d0_m_bkg.noleg = true;

  PlotComponent pc_pdf_d0_m( "d0_mass", "PDF" );
  pc_pdf_d0_m.setSolidLine(kRed);
  pc_pdf_d0_m.noleg = true;

  PlotComponent pc_pdf_d0_t( "d0_t", "PDF" );
  pc_pdf_d0_t.setSolidLine(kRed);
  pc_pdf_d0_t.noleg = true;

  //vector<PlotComponent> plotComps_Dst_M;
  //plotComps_Dst_M.push_back(pc_data_invis);
  //plotComps_Dst_M.push_back(pc_pdf_dst_m);
  //plotComps_Dst_M.push_back(pc_data);

  vector<PlotComponent> plotComps_D0_M;
  //plotComps_D0_M.push_back(pc_data_m_invis);
  plotComps_D0_M.push_back(pc_data_m);
  plotComps_D0_M.push_back(pc_pdf_d0_m_bkg);
  plotComps_D0_M.push_back(pc_pdf_d0_m);
  //plotComps_D0_M.push_back(pc_data_m);

  vector<PlotComponent> plotComps_D0_t;
  //plotComps_D0_t.push_back(pc_data_t_invis);
  plotComps_D0_t.push_back(pc_data_t);
  plotComps_D0_t.push_back(pc_pdf_d0_t);
  //plotComps_D0_t.push_back(pc_data_t);

  setTitle("#splitline{LHCb}{Trigger}");
  setDrawLog(true);
  setResidType(2);
  setRedPull(3);

  w->var("D0_M")->setBins(100);
  w->var("D0_LTIME_ps")->setBins(94);

  //plot("Dst_M", plotComps_Dst_M, "Dst_M", 0, 1970, 2050 );
  plot("D0_M",  plotComps_D0_M,  "D0_M" , 0, 1810, 1910 );
  plot("D0_LTIME_ps", plotComps_D0_t, "D0_LTIME_ps", 0, 0., 5.);

  setResidType(0);
  //plot("Dst_M", plotComps_Dst_M, "Dst_M_NoPull", 0, 1970, 2050 );
  plot("D0_M",  plotComps_D0_M,  "D0_M_NoPull" , 0, 1810, 1910 );
  plot("D0_LTIME_ps", plotComps_D0_t, "D0_LTIME_ps_NoPull", 0, 0., 5.);

}

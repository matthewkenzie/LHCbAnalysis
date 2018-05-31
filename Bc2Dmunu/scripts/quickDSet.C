using namespace RooFit;
using namespace std;

void quickDSet(){

  RooWorkspace *w  = new RooWorkspace("w","w");

  TChain *c = new TChain("AnalysisTree");
  c->AddFile("anal_jobs/Data2011.root");
  c->AddFile("anal_jobs/Data2012.root");
  c->AddFile("anal_jobs/MCBu2DMuNu.root");
  c->AddFile("anal_jobs/MCBc2DMuNu.root");

  double B_plus_MCORR;
  double D0_M;
  int itype;
  int K_minus_ID;
  int Mu_plus_ID;
  double B_plus_FDCHI2_OWNPV;
  double B_plus_LTIME;

  c->SetBranchAddress("B_plus_MCORR", &B_plus_MCORR);
  c->SetBranchAddress("D0_M", &D0_M);
  c->SetBranchAddress("itype", &itype);
  c->SetBranchAddress("K_minus_ID", &K_minus_ID);
  c->SetBranchAddress("Mu_plus_ID", &Mu_plus_ID);
  c->SetBranchAddress("B_plus_FDCHI2_OWNPV", &B_plus_FDCHI2_OWNPV);
  c->SetBranchAddress("B_plus_LTIME", &B_plus_LTIME);

  RooRealVar *bm = new RooRealVar("B_plus_MCORR","",2000,12000);
  RooRealVar *dm = new RooRealVar("D0_M","",1790,1940);
  RooRealVar *bl = new RooRealVar("B_plus_LTIME","",0,0.01);
  RooRealVar *bf = new RooRealVar("B_plus_FDCHI2_OWNPV","",0,20000);

  RooArgSet *observables = new RooArgSet();
  observables->add(*bm);
  observables->add(*dm);
  observables->add(*bl);
  observables->add(*bf);


  RooDataSet *dset_11_dcs = new RooDataSet("dset_11_dcs","",RooArgSet(*observables));
  RooDataSet *dset_11_cf  = new RooDataSet("dset_11_cf" ,"",RooArgSet(*observables));
  RooDataSet *dset_12_dcs = new RooDataSet("dset_12_dcs","",RooArgSet(*observables));
  RooDataSet *dset_12_cf  = new RooDataSet("dset_12_cf" ,"",RooArgSet(*observables));

  RooDataSet *mcset_bu_dcs = new RooDataSet("mcset_bu_dcs","",RooArgSet(*observables));
  RooDataSet *mcset_bu_cf  = new RooDataSet("mcset_bu_cf" ,"",RooArgSet(*observables));
  RooDataSet *mcset_bc_dcs = new RooDataSet("mcset_bc_dcs","",RooArgSet(*observables));
  RooDataSet *mcset_bc_cf  = new RooDataSet("mcset_bc_cf" ,"",RooArgSet(*observables));

  int nentries = c->GetEntries();
  //nentries = 10000;
  for ( int i=0; i<nentries; i++){
    c->GetEntry(i);
    if (i%10000==0) cout << i << " / " << nentries << endl;
    if ( B_plus_MCORR < bm->getMin() || B_plus_MCORR > bm->getMax() ) continue;
    if ( D0_M < dm->getMin() || D0_M > dm->getMax() ) continue;
    bm->setVal( B_plus_MCORR );
    dm->setVal( D0_M );
    bl->setVal( B_plus_LTIME );
    bf->setVal( B_plus_FDCHI2_OWNPV );
    bool dcs = (K_minus_ID*Mu_plus_ID)>0 ? true: false;

    if      ( (itype==11)  && dcs)  dset_11_dcs->add(*observables);
    else if ( (itype==11)  && !dcs) dset_11_cf->add(*observables);
    else if ( (itype==12)  && dcs)  dset_12_dcs->add(*observables);
    else if ( (itype==12)  && !dcs) dset_12_cf->add(*observables);
    else if ( (itype==-20) && dcs)  mcset_bc_dcs->add(*observables);
    else if ( (itype==-20) && dcs)  mcset_bc_dcs->add(*observables);
    else if ( (itype==-29) && !dcs) mcset_bu_cf->add(*observables);
    else if ( (itype==-29) && !dcs) mcset_bu_cf->add(*observables);
  }

  w->import( *dset_11_dcs );
  w->import( *dset_11_cf );
  w->import( *dset_12_dcs );
  w->import( *dset_12_cf );
  w->import( *mcset_bc_dcs );
  w->import( *mcset_bc_cf );
  w->import( *mcset_bu_dcs );
  w->import( *mcset_bu_cf );

  w->writeToFile("root/Dsets.root");

  RooPlot *bm_plot = bm->frame();
  dset_11_cf->plotOn(bm_plot);
  dset_11_dcs->plotOn(bm_plot);

  RooPlot *dm_plot = dm->frame();
  dset_11_cf->plotOn(dm_plot);
  dset_11_dcs->plotOn(dm_plot);

  TCanvas *canv = new TCanvas("c","c",800,1200);
  canv->Divide(1,2);
  canv->cd(1);
  bm_plot->Draw();
  canv->cd(2);
  dm_plot->Draw();




  //TFile *f = new TFile("root/Dsets.root","recreate");

}

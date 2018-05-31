void fom(){

  TFile *f = TFile::Open("root/Small.root");
  TTree *t =  (TTree*)f->Get("AnalysisTree");

  TGraph *gr = new TGraph();

  int nsteps=10;
  double step = 2./nsteps;
  for ( int i=0; i<nsteps; i++ ) {

    double cutval = -1. + step*nsteps;

    t->GetEntries(

  }

}

void StreamlineTreeForFitting() {

  //TString inf = "root/AnalysisDoubleSelOut.root";
  //TFile *oldfile = TFile::Open(inf);
  //TTree *oldtree = (TTree*)oldfile->Get("AnalysisTree");
  TChain *oldtree = new TChain("AnalysisTree");
  oldtree->Add("anal_jobs/*.root");

  oldtree->SetBranchStatus("*",0);
  oldtree->SetBranchStatus("itype", 1);
  oldtree->SetBranchStatus("D0_M" , 1);
  oldtree->SetBranchStatus("B_plus_M", 1);
  oldtree->SetBranchStatus("B_plus_MCORR", 1);
  oldtree->SetBranchStatus("B_plus_MCORRERR", 1);
  oldtree->SetBranchStatus("K_minus_ProbNNk", 1);
  oldtree->SetBranchStatus("K_minus_ProbNNpi", 1);
  oldtree->SetBranchStatus("K_minus_ProbNNmu", 1);
  oldtree->SetBranchStatus("K_minus_ProbNNghost", 1);
  oldtree->SetBranchStatus("Pi_plus_ProbNNk", 1);
  oldtree->SetBranchStatus("Pi_plus_ProbNNpi", 1);
  oldtree->SetBranchStatus("Pi_plus_ProbNNmu", 1);
  oldtree->SetBranchStatus("Pi_plus_ProbNNghost", 1);
  oldtree->SetBranchStatus("Mu_plus_ProbNNk", 1);
  oldtree->SetBranchStatus("Mu_plus_ProbNNpi", 1);
  oldtree->SetBranchStatus("Mu_plus_ProbNNmu", 1);
  oldtree->SetBranchStatus("Mu_plus_ProbNNghost", 1);
  oldtree->SetBranchStatus("comb_rejection_bdtoutput", 1);
  oldtree->SetBranchStatus("bu_rejection_bdtoutput", 1);

  TFile *newfile = new TFile("root/AnalysisDoubleSelSmall.root","recreate");
  //TTree *newtree = oldtree->CloneTree();
  TTree *newtree = oldtree->CloneTree(0);

  int itype;
  int K_minus_ID;
  int Mu_plus_ID;
  oldtree->SetBranchAddress("K_minus_ID", &K_minus_ID);
  oldtree->SetBranchAddress("Mu_plus_ID", &Mu_plus_ID);
  oldtree->SetBranchAddress("itype", &itype);

  int nentries = oldtree->GetEntries();

  for ( int i=0; i<nentries; i++ ){
    if ( i%10000==0 ) cout << i << " / " << nentries << endl;
    oldtree->GetEntry(i);
    if ( ( (K_minus_ID*Mu_plus_ID>0) && itype>0 ) || itype<0 ) {
      newtree->Fill();
    }
  }
  newtree->AutoSave();
  //delete oldfile;
  newtree->Print();
  delete newfile;
}

using namespace RooFit;
using namespace std;

void wserror( TString name )
{
  cout << "Couldn't find object " << name << " in workspace" << endl;
  exit(1);
}

void createHist( RooWorkspace *w, TString varName, TString dsetName, TString histName ) {

  if ( ! w->data(dsetName) ) wserror(dsetName);
  if ( ! w->var(varName) ) wserror(varName);

  RooDataSet *dset = (RooDataSet*)w->data(dsetName);
  RooRealVar *var = (RooRealVar*)w->var(varName);
  TH1D *hist = (TH1D*)dset->createHistogram("h"+histName, *var, Binning( var->getBins(), var->getMin(), var->getMax() ) );
  RooDataHist *hset = new RooDataHist(histName,"",RooArgList(*var),hist);
  w->import( *hset );
}

void histToPdf( RooWorkspace *w, TString varName, TString histName, TString pdfName) {

  if ( ! w->data(histName) ) wserror(histName);
  if ( ! w->var(varName) ) wserror(varName);

  RooDataHist *hist = (RooDataHist*)w->data(histName);
  RooRealVar *var = (RooRealVar*)w->var(varName);
  RooHistPdf *hpdf = new RooHistPdf(pdfName,"",*var,*hist);
  w->import(*hpdf);
}

void histAndPdf( RooWorkspace *w, TString varName, TString dsetName, TString histName, TString pdfName ) {

  createHist(w,varName,dsetName,histName);
  histToPdf(w,varName,histName,pdfName);
}

void addDSets( RooWorkspace *w, TString dset1Name, TString dset2Name, TString combName ) {

  if ( ! w->data(dset1Name) ) wserror(dset1Name);
  if ( ! w->data(dset2Name) ) wserror(dset2Name);

  RooDataSet *d1 = (RooDataSet*)w->data(dset1Name);
  RooDataSet *d2 = (RooDataSet*)w->data(dset2Name);

  RooDataSet *dset = (RooDataSet*)d1->Clone(combName);
  dset->append( *d2 );
  w->import( *dset );
}

void reduceDSet( RooWorkspace *w, TString oldDset, TString newDset, TString cut ) {

  if ( ! w->data(oldDset) ) wserror(oldDset);
  RooDataSet *oldD = (RooDataSet*)w->data(oldDset);
  RooDataSet *newD = (RooDataSet*)oldD->reduce( Name(newDset), Cut(cut) );
  w->import( *newD );
}

void reduceDSetRange( RooWorkspace *w, TString oldDset, TString newDset, TString cutRange ) {

  if ( ! w->data(oldDset) ) wserror(oldDset);
  RooDataSet *oldD = (RooDataSet*)w->data(oldDset);
  RooDataSet *newD = (RooDataSet*)oldD->reduce( Name(newDset), CutRange(cutRange) );
  w->import( *newD );
}

void plotData(){

  TFile *tf = TFile::Open("root/Dsets.root");
  RooWorkspace *w = (RooWorkspace*)tf->Get("w");
  w->Print();

  RooRealVar *bm = (RooRealVar*)w->var("B_plus_MCORR");
  RooRealVar *dm = (RooRealVar*)w->var("D0_M");

  addDSets( w, "dset_11_dcs", "dset_12_dcs", "dset_dcs" );
  addDSets( w, "dset_11_cf", "dset_12_cf", "dset_cf" );
  addDSets( w, "dset_cf", "dset_dcs", "dset" );
  reduceDSet( w, "dset_cf", "dset_cf_usb", "D0_M>1920" );
  reduceDSet( w, "dset_dcs", "dset_dcs_usb", "D0_M>1920" );
  reduceDSet( w, "dset", "dset_usb", "D0_M>1920" );


  RooCategory *cat = new RooCategory("dec","dec");
  cat->defineType("cf");
  cat->defineType("dcs");
  w->import(*cat);

  w->var("D0_M")->setRange(1790,1940);
  w->var("D0_M")->setBins(300);
  w->var("B_plus_MCORR")->setRange(2000,12000);
  w->var("B_plus_MCORR")->setBins(100);

  createHist( w, "D0_M", "dset_cf" , "hset_dm_cf" );
  createHist( w, "D0_M", "dset_dcs", "hset_dm_dcs" );
  createHist( w, "D0_M", "dset"    , "hset_dm_all" );

  createHist( w, "B_plus_MCORR", "dset_cf" , "hset_bm_cf" );
  createHist( w, "B_plus_MCORR", "dset_dcs", "hset_bm_dcs" );
  createHist( w, "B_plus_MCORR", "dset"    , "hset_bm_all" );

  createHist( w, "B_plus_MCORR", "dset_cf_usb" , "hset_bm_cf_usb" );
  createHist( w, "B_plus_MCORR", "dset_dcs_usb", "hset_bm_dcs_usb" );
  createHist( w, "B_plus_MCORR", "dset_usb"    , "hset_bm_all_usb" );

  histToPdf( w, "B_plus_MCORR", "hset_bm_cf_usb", "hpdf_bm_cf_nond");
  histToPdf( w, "B_plus_MCORR", "hset_bm_dcs_usb", "hpdf_bm_dcs_nond");
  histToPdf( w, "B_plus_MCORR", "hset_bm_all_usb", "hpdf_bm_all_nond");

  // create PDF to fit D mass
  w->factory("D0_mean[1850,1950]");
  w->factory("D0_sigma[10,0,200]");
  w->factory("D0_sigma2[10,0,200]");
  w->factory("D0_alpha[2.,0,20.]");
  w->factory("D0_n[3,0,20]");
  w->factory("CBShape::dm_sig1( D0_M, D0_mean, D0_sigma2, D0_alpha, D0_n )");
  w->factory("Gaussian::dm_sig2( D0_M, D0_mean, D0_sigma )");
  w->factory("SUM::dm_sig( dm_sigf[0.4,0.01,1.]*dm_sig1,dm_sig2 )");

  w->factory("dm_pr_mean[1790,1720,1800]");
  w->factory("dm_pr_sigma[10,1,20]");
  w->factory("Gaussian::dm_pr( D0_M, dm_pr_mean, dm_pr_sigma )");

  w->factory("dm_bkg_p[-0.002,-0.8,0.]");
  w->factory("Exponential::dm_bkg( D0_M, dm_bkg_p )");

  w->factory("SUM::dm_pdf( dm_sy[1e4,1e1,1e8]*dm_sig, dm_by[1e4,1e1,1e8]*dm_bkg, dm_py[1e1,1,1e5]*dm_pr  ) ");
  w->factory("SUM::dm_pdf_sig_bkg( dm_sy*dm_sig, dm_by*dm_bkg )");

  // est bkg
  dm->setRange("sbhigh", 1920, 1940);
  w->pdf("dm_bkg")->fitTo(*w->data("hset_dm_cf"),Range("sbhigh"));
  w->var("dm_bkg_p")->setConstant();

  // est sig
  w->pdf("dm_pdf_sig_bkg")->fitTo(*w->data("hset_dm_cf"),Extended());

  // est pr
  w->var("D0_mean")->setConstant();
  w->var("D0_sigma")->setConstant();
  w->var("D0_sigma2")->setConstant();
  w->var("D0_alpha")->setConstant();
  w->var("D0_n")->setConstant();
  w->var("dm_sigf")->setConstant();
  w->pdf("dm_pdf")->fitTo(*w->data("hset_dm_cf"),Extended());

  // relax to fit cf
  w->var("dm_bkg_p")->setConstant(false);
  w->var("D0_mean")->setConstant(false);
  w->var("D0_sigma")->setConstant(false);
  w->var("D0_sigma2")->setConstant(false);
  w->var("D0_alpha")->setConstant(false);
  w->var("D0_n")->setConstant(false);
  w->var("dm_sigf")->setConstant(false);
  w->pdf("dm_pdf")->fitTo(*w->data("hset_dm_cf"),Extended());

  // now fix ?
  //w->var("dm_bkg_p")->setConstant();
  w->var("D0_mean")->setConstant();
  w->var("D0_sigma")->setConstant();
  w->var("D0_sigma2")->setConstant();
  w->var("D0_alpha")->setConstant();
  w->var("D0_n")->setConstant();
  w->var("dm_pr_mean")->setConstant();
  w->var("dm_pr_sigma")->setConstant();

  // sim fit
  w->factory("SUM::dm_cf_pdf( dm_cf_sy[1.3e7,1e5,1e8]*dm_sig, dm_cf_by[1.3e6,1e5,1e7]*dm_bkg, dm_cf_py[2.3e4,1e3,1e5]*dm_pr  ) ");
  w->factory("prod::dm_dcs_sy( dm_frac_s[0.1,0.,1.0],dm_cf_sy )");
  w->factory("prod::dm_dcs_by( dm_frac_b[0.1,0.,1.0],dm_cf_by )");
  w->factory("prod::dm_dcs_py( dm_frac_p[0.1,0.,1.0],dm_cf_py )");
  w->factory("SUM::dm_dcs_pdf( dm_dcs_sy*dm_sig, dm_dcs_by*dm_bkg, dm_dcs_py*dm_pr  ) ");
  //w->factory("SUM::dm_dcs_pdf( dm_dcs_sy[1.3e7,1e5,1e8]*dm_sig, dm_dcs_by[1.3e7,1e5,1e8]*dm_bkg, dm_dcs_py[1.3e7,1e5,1e8]*dm_pr  ) ");

  RooDataHist *hset_dm_comb = new RooDataHist("hset_dm_comb","",RooArgSet(*w->var("D0_M")),Index(*cat),Import("cf",*(RooDataHist*)w->data("hset_dm_cf")),Import("dcs",*(RooDataHist*)w->data("hset_dm_dcs")));
  RooSimultaneous *sim_pdf = new RooSimultaneous("dm_sim_pdf","",*cat);
  sim_pdf->addPdf(*w->pdf("dm_cf_pdf"),"cf");
  sim_pdf->addPdf(*w->pdf("dm_dcs_pdf"),"dcs");

  sim_pdf->fitTo(*hset_dm_comb,Extended());

  // relax to fit finally
  w->var("dm_bkg_p")->setConstant(false);
  w->var("D0_mean")->setConstant(false);
  w->var("D0_sigma")->setConstant(false);
  w->var("D0_sigma2")->setConstant(false);
  w->var("D0_alpha")->setConstant(false);
  w->var("D0_n")->setConstant(false);
  w->var("dm_sigf")->setConstant(false);
  w->var("dm_pr_mean")->setConstant(false);
  w->var("dm_pr_sigma")->setConstant(false);
  sim_pdf->fitTo(*hset_dm_comb,Extended());

  w->import(*hset_dm_comb);
  w->import(*sim_pdf);

  // plot fit
  RooPlot *dm_pl = dm->frame(Title("D0 line shape"));
  dm_pl->GetXaxis()->SetTitle("m(D0) [MeV]");
  RooPlot *bm_pl = bm->frame(Title("B+ line shape"));
  bm_pl->GetXaxis()->SetTitle("m(B+)_{corr} [MeV]");
  TColor *r1 = gROOT->GetColor(kRed-3);
  TColor *r2 = gROOT->GetColor(kRed-2);
  TColor *b1 = gROOT->GetColor(kBlue-3);
  TColor *b2 = gROOT->GetColor(kBlue-2);
  r1->SetAlpha(0.6);
  r2->SetAlpha(0.6);
  b1->SetAlpha(0.6);
  b2->SetAlpha(0.6);
  TLegend *leg = new TLegend(0.7,0.5,0.9,0.9);
  w->data("hset_dm_cf")->plotOn(dm_pl,MarkerColor(kRed+2),LineColor(kRed+2),Binning(75));
  leg->AddEntry( dm_pl->getObject(dm_pl->numItems()-1), "CF", "LEP" );
  w->pdf("dm_cf_pdf")->plotOn(dm_pl,Components("dm_bkg,dm_pr"),FillColor(r2->GetNumber()),LineColor(r2->GetNumber()),LineWidth(0),DrawOption("F"));
  leg->AddEntry( dm_pl->getObject(dm_pl->numItems()-1), "CF PR", "F" );
  w->pdf("dm_cf_pdf")->plotOn(dm_pl,Components("dm_bkg"),FillColor(r1->GetNumber()),LineColor(r1->GetNumber()),LineWidth(0),DrawOption("F"));
  leg->AddEntry( dm_pl->getObject(dm_pl->numItems()-1), "CF Non-D", "F" );
  w->pdf("dm_cf_pdf")->plotOn(dm_pl,LineColor(kBlack),LineWidth(1));
  w->data("hset_dm_cf")->plotOn(dm_pl,MarkerColor(kRed+1),LineColor(kRed+1),Binning(75));
  w->data("hset_dm_dcs")->plotOn(dm_pl,MarkerColor(kBlue+1),LineColor(kBlue+1),Binning(75));
  leg->AddEntry( dm_pl->getObject(dm_pl->numItems()-1), "DCS", "LEP" );
  w->pdf("dm_dcs_pdf")->plotOn(dm_pl,Components("dm_bkg,dm_pr"),FillColor(b2->GetNumber()),LineColor(b2->GetNumber()),LineWidth(0),DrawOption("F"));
  leg->AddEntry( dm_pl->getObject(dm_pl->numItems()-1), "DCS PR", "F" );
  w->pdf("dm_dcs_pdf")->plotOn(dm_pl,Components("dm_bkg"),FillColor(b1->GetNumber()),LineColor(b1->GetNumber()),LineWidth(0),DrawOption("F"));
  leg->AddEntry( dm_pl->getObject(dm_pl->numItems()-1), "DCS Non-D", "F" );
  w->pdf("dm_dcs_pdf")->plotOn(dm_pl,LineColor(kBlack),LineWidth(1));
  w->data("hset_dm_dcs")->plotOn(dm_pl,MarkerColor(kBlue+1),LineColor(kBlue+1),Binning(75));

  TLegend *leg2 = new TLegend(0.6,0.6,0.9,0.9);
  w->data("hset_bm_all")->plotOn(bm_pl,Binning(100));
  leg2->AddEntry( bm_pl->getObject(bm_pl->numItems()-1), "All Data", "LEP" );
  w->data("hset_bm_cf")->plotOn(bm_pl,MarkerColor(kRed),LineColor(kRed),Binning(100));
  leg2->AddEntry( bm_pl->getObject(bm_pl->numItems()-1), "CF Data", "LEP" );
  w->data("hset_bm_dcs")->plotOn(bm_pl,MarkerColor(kBlue),LineColor(kBlue),Binning(100));
  leg2->AddEntry( bm_pl->getObject(bm_pl->numItems()-1), "DCS Data", "LEP" );

  // define signal region and some new datasets therein
  double sMin = w->var("D0_mean")->getVal()-3.*TMath::Min( w->var("D0_sigma")->getVal(), w->var("D0_sigma2")->getVal()) ;
  double sMax = w->var("D0_mean")->getVal()+3.*TMath::Min( w->var("D0_sigma")->getVal(), w->var("D0_sigma2")->getVal()) ;
  w->var("D0_M")->setRange("signal", sMin , sMax  );
  TBox *sr = new TBox();
  sr->SetFillColorAlpha(kGray+1,0.3);
  sr->SetLineColor(kBlack);
  sr->SetLineWidth(0);
  TBox *usb = new TBox();
  usb->SetFillColorAlpha(kGreen+3,0.3);
  usb->SetLineColor(kGreen+3);
  usb->SetLineWidth(0);
  leg->AddEntry(sr, "Sig Reg", "F");
  leg->AddEntry(usb, "Upper SB", "F");

  reduceDSetRange(w, "dset_cf", "dset_cf_sr", "signal");
  reduceDSetRange(w, "dset_dcs", "dset_dcs_sr", "signal");
  reduceDSetRange(w, "dset", "dset_sr", "signal");
  createHist(w, "B_plus_MCORR", "dset_cf_sr", "hset_bm_cf_sr");
  createHist(w, "B_plus_MCORR", "dset_dcs_sr", "hset_bm_dcs_sr");
  createHist(w, "B_plus_MCORR", "dset_sr", "hset_bm_all_sr");

  TCanvas *c = new TCanvas("c","c",1600,1200);
  c->Divide(2,2);
  c->cd(1);
  bm_pl->Draw();
  leg2->Draw();
  c->cd(2);
  bm_pl->Draw();
  leg2->Draw();
  c->cd(2)->SetLogy();
  c->cd(3);
  dm_pl->Clone()->Draw();
  sr->DrawBox( sMin, 0, sMax, dm_pl->GetMaximum() );
  usb->DrawBox( 1920, 0, 1940, dm_pl->GetMaximum() );
  leg->Draw();
  c->cd(4);
  dm_pl->GetYaxis()->SetRangeUser(8e3,2e6);
  dm_pl->Draw();
  sr->DrawBox( sMin, dm_pl->GetMinimum(), sMax, dm_pl->GetMaximum() );
  usb->DrawBox( 1920, dm_pl->GetMinimum(), 1940, dm_pl->GetMaximum() );
  leg->Draw();
  c->cd(4)->SetLogy();
  c->Update();
  c->Modified();
  c->Print("plots/CompShapes/pdf/dset.pdf");

  // estimate yields in sig window
  RooAbsReal *bint = (RooAbsReal*)w->pdf("dm_bkg")->createIntegral(*w->var("D0_M"), NormSet(*w->var("D0_M")), Range("signal"));
  RooAbsReal *sint = (RooAbsReal*)w->pdf("dm_sig")->createIntegral(*w->var("D0_M"), NormSet(*w->var("D0_M")), Range("signal"));
  double sY_dcs = w->function("dm_dcs_sy")->getVal()*sint->getVal();
  double bY_dcs = w->function("dm_dcs_by")->getVal()*bint->getVal();
  double sY_cf = w->var("dm_cf_sy")->getVal()*sint->getVal();
  double bY_cf = w->var("dm_cf_by")->getVal()*bint->getVal();

  cout << "Sig Reg: [ " << sMin << " , " << sMax << " ] " << endl;
  cout << "sIntegral : " << sint->getVal() << endl;
  cout << "bIntegral : " << bint->getVal() << endl;
  cout << "SY DCS in SigReg: " << sY_dcs << endl;
  cout << "BY DCS in SigReg: " << bY_dcs << endl;
  cout << "DCS Events in SR: " << w->data("dset_dcs_sr")->sumEntries() << endl;
  cout << "SY CF in SigReg: " << sY_cf << endl;
  cout << "BY CF in SigReg: " << bY_cf << endl;
  cout << "CF Events in SR: " << w->data("dset_cf_sr")->sumEntries() << endl;


  RooPlot *bm_pl2 = w->var("B_plus_MCORR")->frame(Title("B+ line shape in SB"));
  w->data("hset_bm_all_usb")->plotOn(bm_pl2, Binning(100));
  w->data("hset_bm_cf_usb")->plotOn(bm_pl2,MarkerColor(kRed),LineColor(kRed),Binning(100));
  w->pdf("hpdf_bm_cf_nond")->plotOn(bm_pl2,LineColor(kBlack),LineWidth(1));
  w->data("hset_bm_dcs_usb")->plotOn(bm_pl2,MarkerColor(kBlue),LineColor(kBlue),Binning(100));
  w->pdf("hpdf_bm_dcs_nond")->plotOn(bm_pl2,LineColor(kBlack),LineWidth(1));
  bm_pl2->Draw();

  TCanvas *c2 = new TCanvas("c2","c2",1600,800);
  c2->Divide(2,1);
  c2->cd(1);
  bm_pl2->Draw();
  c2->cd(2);
  bm_pl2->Draw();
  c2->cd(2)->SetLogy();
  c2->Update();
  c2->Modified();
  c2->Print("plots/CompShapes/pdf/dset_sb.pdf");

  RooPlot *bm_pl3 = w->var("B_plus_MCORR")->frame(Title("B+ line shape DCS"));
  //hpdf_bm_dcs_nond->setUnitNorm(w->function("dm_dcs_by")->getVal());
  w->data("hset_bm_dcs")->plotOn(bm_pl3, Binning(100));
  w->pdf("hpdf_bm_dcs_nond")->plotOn(bm_pl3,Normalization(w->function("dm_dcs_by")->getVal(),RooAbsReal::NumEvent));

  RooPlot *bm_pl4 = w->var("B_plus_MCORR")->frame(Title("B+ line shape CF"));
  //hpdf_bm_cf_nond->setUnitNorm(w->var("dm_cf_by")->getVal());
  w->data("hset_bm_cf")->plotOn(bm_pl4, Binning(100));
  w->pdf("hpdf_bm_cf_nond")->plotOn(bm_pl4,Normalization(w->var("dm_cf_by")->getVal(),RooAbsReal::NumEvent));

  TCanvas *c3 = new TCanvas("c3","c3",800,1200);
  c3->Divide(1,2);
  c3->cd(1);
  bm_pl3->Draw();
  c3->cd(2);
  bm_pl4->Draw();
  c3->Print("plots/CompShapes/pdf/dset_fit_all.pdf");

  cout << "Number of NonD DCS Events: " << w->function("dm_dcs_by")->getVal() << endl;
  cout << "Total Number of DCS Events: " << w->data("hset_bm_dcs")->sumEntries() << endl;
  cout << "Number of NonD CF Events: " << w->var("dm_cf_by")->getVal() << endl;
  cout << "Total Number of CF Events: " << w->data("hset_bm_cf")->sumEntries() << endl;
  cout << w->var("dm_frac_b")->getVal() << endl;

  // also want integral of NonD B+ shape in narrower window
  w->var("B_plus_MCORR")->setRange("fit",4000,7000);
  RooAbsReal *nond_int = w->pdf("hpdf_bm_dcs_nond")->createIntegral( *w->var("B_plus_MCORR"), NormSet(*w->var("B_plus_MCORR")), Range("fit") );
  double nond_cands_coarse = nond_int->getVal()*bY_dcs;
  cout << "Number of NonD DCS Events in Fit Window: " << nond_cands_coarse << endl;

  // think about the Bu shape from MC and Data (CF only) only and shapes in SR
  histToPdf( w, "B_plus_MCORR", "hset_bm_cf_sr", "hpdf_bm_cf_sr" );

  RooPlot *bm_pl5 = w->var("B_plus_MCORR")->frame(Title("B+ line shape DCS SigReg"),Range(4000,7000));
  TLegend *leg3 = new TLegend(0.6,0.6,0.9,0.9);
  w->data("hset_bm_dcs_sr")->plotOn(bm_pl5,Binning(30,4000,7000));
  leg3->AddEntry( bm_pl5->getObject( bm_pl5->numItems()-1 ), "Data", "LEP");
  w->pdf("hpdf_bm_dcs_nond")->plotOn(bm_pl5,Normalization(bY_dcs,RooAbsReal::NumEvent));
  leg3->AddEntry( bm_pl5->getObject(bm_pl5->numItems()-1), "NonD Contribution", "LEP" );
  //w->pdf("hpdf_bm_cf_sr")->plotOn(bm_pl5,Normalization(sY_dcs,RooAbsReal::NumEvent), LineColor(kRed));

  w->var("B_plus_MCORR")->setBins(30);
  w->var("B_plus_MCORR")->setRange(4000,7000);
  createHist( w, "B_plus_MCORR", "dset_dcs_sr", "hset_bm_dcs_sr_coarse");
  histAndPdf( w, "B_plus_MCORR", "dset_dcs_usb", "hset_bm_dcs_usb_coarse", "hpdf_bm_dcs_nond_coarse");
  histAndPdf( w, "B_plus_MCORR", "mcset_bu_cf", "hset_bu_cf", "hpdf_bu_cf");
  histAndPdf( w, "B_plus_MCORR", "mcset_bc_dcs", "hset_bc_dcs", "hpdf_bc_dcs");

  w->factory("nond_y[20e3,10e3,40e6]");
  w->factory("bu_y[20e3,5e3,40e6]");
  w->factory("bc_y[1000,10,10000]");

  RooAddPdf *totPdf = new RooAddPdf("totPdf","",RooArgList( *w->pdf("hpdf_bm_dcs_nond_coarse"), *w->pdf("hpdf_bu_cf"), *w->pdf("hpdf_bc_dcs") ), RooArgList( *w->var("nond_y"), *w->var("bu_y"), *w->var("bc_y")) );
  w->import(*totPdf);

  w->var("nond_y")->setVal(nond_cands_coarse);
  w->var("nond_y")->setConstant();
  totPdf->fitTo( *w->data("hset_bm_dcs_sr_coarse"), Extended() );

  TLegend *leg4 = new TLegend(0.6,0.6,0.9,0.9);
  RooPlot *bm_pl6 = w->var("B_plus_MCORR")->frame(Title("B+ line shape CF SigReg floating comps"));
  w->data("hset_bm_dcs_sr_coarse")->plotOn(bm_pl6);
  leg4->AddEntry( bm_pl6->getObject( bm_pl6->numItems()-1 ), "Data", "LEP");
  w->pdf("totPdf")->plotOn(bm_pl6);
  leg4->AddEntry( bm_pl6->getObject( bm_pl6->numItems()-1 ), "Total", "L");
  w->pdf("totPdf")->plotOn(bm_pl6, LineColor(kBlack),Components("hpdf_bm_dcs_nond_coarse"));
  leg4->AddEntry( bm_pl6->getObject( bm_pl6->numItems()-1 ), "NonD", "L");
  w->pdf("totPdf")->plotOn(bm_pl6, LineColor(kRed),Components("hpdf_bu_cf"));
  leg4->AddEntry( bm_pl6->getObject( bm_pl6->numItems()-1 ), "B+", "L");
  w->pdf("totPdf")->plotOn(bm_pl6, LineColor(kGreen),Components("hpdf_bc_dcs"));
  leg4->AddEntry( bm_pl6->getObject( bm_pl6->numItems()-1 ), "Bc+", "L");
  w->data("hset_bm_dcs_sr_coarse")->plotOn(bm_pl6);

  TCanvas *c4 = new TCanvas("c4","c4",800,1200);
  c4->Divide(1,2);
  c4->cd(1);
  bm_pl5->Draw();
  leg3->Draw();
  c4->cd(2);
  bm_pl6->Draw();
  leg4->Draw();
  c4->Print("plots/CompShapes/pdf/dset_fit_sr.pdf");

}

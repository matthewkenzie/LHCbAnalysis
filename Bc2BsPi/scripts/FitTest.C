using namespace RooFit;

void FitTest(){

  // Bs Mass Fit

  // declare obs
  RooRealVar *mass = new RooRealVar("mass","mass",5200,5500);

  // bkg pdf
  RooRealVar *exp_p0 = new RooRealVar("exp_p0","",-0.002,-0.1,0.);
  RooExponential *bkg_pdf = new RooExponential("bkg","bkg", *mass, *exp_p0);

  // sig pdf
  RooRealVar *mean = new RooRealVar("mean","",5350, 5300,5400);
  RooRealVar *sigma = new RooRealVar("sigma","",20,0,50);
  RooGaussian *sig_pdf = new RooGaussian("sig","sig",*mass,*mean,*sigma);

  // relative yields
  RooRealVar *bkg_y = new RooRealVar("bkg_y","bkg_y",1000,0,10000);
  RooRealVar *sig_y = new RooRealVar("sig_y","sig_y",200,0,5000);

  // combined pdf
  RooAddPdf *pdf = new RooAddPdf("pdf","pdf", RooArgList(*sig_pdf,*bkg_pdf), RooArgList(*sig_y,*bkg_y) );

  // generate a toy dataset
  RooDataSet *data = pdf->generate( RooArgSet(*mass), 1200, Extended() );

  // plot
  RooPlot *plot = mass->frame();
  data->plotOn(plot);
  pdf->plotOn(plot, Components("bkg"), LineColor(kRed) );
  pdf->plotOn(plot, Components("sig"), LineColor(kGreen+2) );
  pdf->plotOn(plot);

  // Draw
  TCanvas *canv = new TCanvas();
  plot->Draw();
  canv->Update();
  canv->Modified();

}

import ROOT as r

r.gROOT.ProcessLine(".L ../build/libLHCbAnalysisComponents.dylib")

rand = r.RooRandom.randomGenerator().SetSeed(17)

w = r.RooWorkspace("w","w")
#tf = r.TFile("root/MassFit/MassFitResultWSWeights.root")
#w = tf.Get("w") 
#w.factory( 'SUM::mypdf( bkg_y[1000,0,4000]*bkg_pdf, sig_y[400,0,4000]*bs2kstkst_mc_pdf )' )
#w.factory( 'Gaussian::gaus( B_s0_DTF_B_s0_M, mean[5200,5000,5800], sigma[10,0,40] )' )
#w.factory( 'SUM::mypdf( bkg_y[1000,0,4000]*bkg_pdf, sig_y[400,0,4000]*gaus )' )

mass = r.RooRealVar("mass","mass",5000,5800)
l = r.RooRealVar("l","l",-5.3715e+00)
z = r.RooRealVar("z","z",0)
f = r.RooRealVar("f","f",0)
s = r.RooRealVar("s","s",1.7584e+01)
m = r.RooRealVar("m","m",5.3715e+03)
a  =r.RooRealVar("a","a",2.5031e+00)
n  =r.RooRealVar("n","n",2.5041e+00)
a2 =r.RooRealVar("a2","a2",2.5749e+00)
n2 =r.RooRealVar("n2","n2",2.5263e+00)
b  =r.RooRealVar("b","b",-2.8767e-03)
sig_y = r.RooRealVar("sig_y","sig_y",400,0,4000)
bkg_y = r.RooRealVar("bkg_y","bkg_y",1000,0,4000)

sig = r.RooIpatia2("sig","sig",mass, l,z,f,s,m,a,n,a2,n2)
bkg = r.RooExponential("bkg","bkg",mass, b)
yields = r.RooArgList()
yields.add( sig_y )
yields.add( bkg_y )
pdfs = r.RooArgList()
pdfs.add( sig )
pdfs.add( bkg )
mypdf = r.RooAddPdf( 'mypdf','',pdfs,yields)

data = mypdf.generate(r.RooArgSet(mass),100,r.RooFit.Extended())

params = mypdf.getParameters(data)
params.Print("v")
sig_y.setVal(0.01)
bkg_y.setVal(1000)
params.setAttribAll("Constant")

plot = mass.frame()
data.plotOn(plot)
mypdf.plotOn(plot)

sData = r.RooStats.SPlot( 'sData','sData', data, mypdf, yields )
data.Print("v")
getattr(w,'import')(data)
dataSW = r.RooDataSet( 'sw','sw', data, r.RooArgSet( mass, w.var('sig_y_sw') ), '','sig_y_sw' )

for i in range(0,dataSW.numEntries()):
    vals = dataSW.get(i)
    print i, vals.getRealValue('mass'), dataSW.weight()

plot2 = mass.frame()
dataSW.plotOn(plot2)

c = r.TCanvas('c','c',800,1200)
c.Divide(1,2)
c.cd(1)
plot.Draw()
c.cd(2)
plot2.Draw()
l = r.TLine()
l.SetLineWidth(2)
l.SetLineColor(r.kRed)
l.DrawLine(5000,0,5800,0)
c.Update()

raw_input()

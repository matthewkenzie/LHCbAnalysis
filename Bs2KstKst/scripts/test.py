import ROOT as r

w = r.RooWorkspace("w","w")
w.factory( 'mass[5000,5800]' )
w.factory( 'Exponential::bkg( mass, exp_p0[-0.002,-2.,0.] )' )
w.factory( 'Gaussian::gaus( mass, mean[5400,5000,5800], sigma[12,0,40] )' )
w.factory( 'SUM::pdf( bkg_y[1000,0,2000]*bkg, sig_y[400,0,1000]*gaus )' )

pdf = w.pdf('pdf')
data = pdf.generate(r.RooArgSet(w.var('mass')),4000)

plot = w.var('mass').frame()
data.plotOn(plot)
pdf.plotOn(plot)

pdf.getParameters(data).setAttribAll('Constant')
pdf.getParameters(data).Print('v')

sData = r.RooStats.SPlot( 'sData','sData', data, pdf, r.RooArgList(w.var('sig_y'),w.var('bkg_y')) )
data.Print("v")
getattr(w,'import')(data)
dataSW = r.RooDataSet( 'sw','sw', data, r.RooArgSet( w.var('mass'), w.var('sig_y_sw') ), '','sig_y_sw' )

plot2 = w.var('mass').frame()
dataSW.plotOn(plot2)

c = r.TCanvas('c','c',800,1200)
c.Divide(1,2)
c.cd(1)
plot.Draw()
c.cd(2)
plot2.Draw()
c.Update()

raw_input()

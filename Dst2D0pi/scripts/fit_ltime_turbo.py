#!/usr/bin/env python

import ROOT as r
r.gROOT.ProcessLine('.x ~/bin/lhcbStyle.C')

import os
os.system('mkdir -p plots/LFit/pdf')
os.system('mkdir -p plots/LFit/png')
os.system('mkdir -p plots/LFit/C')

tf = r.TFile('root/AnalysisOutWithCuts.root')
tree = tf.Get('AnalysisTree')

w = r.RooWorkspace('w','w')
w.factory( 'Dst_M[1970,2050]' )
w.factory( 'D0_M[1820,1910]' )
w.factory( 'D0_LTIME[0.,5.]' )

obs = r.RooArgSet( w.var('Dst_M'), w.var('D0_M'), w.var('D0_LTIME') )
w.defineSet('observables',obs)

data = r.RooDataSet('Data','Data',obs)

for ev in range(tree.GetEntries()):
  tree.GetEntry(ev)
  if ev%10000==0:
   print ev, '/', tree.GetEntries()
  if tree.D0_LTIME<0.00025: continue

  if tree.Dst_M < w.var('Dst_M').getMin() or tree.Dst_M > w.var('Dst_M').getMax(): continue
  if tree.D0_M < w.var('D0_M').getMin() or tree.D0_M > w.var('D0_M').getMax(): continue
  if 1000.*tree.D0_LTIME < w.var('D0_LTIME').getMin() or 1000.*tree.D0_LTIME > w.var('D0_LTIME').getMax(): continue
  w.var('Dst_M').setVal( tree.Dst_M )
  w.var('D0_M').setVal( tree.D0_M )
  w.var('D0_LTIME').setVal( tree.D0_LTIME*1000. )
  data.add( obs )

getattr(w,'import')(data)
w.writeToFile('root/DataCache.root')

#tf = r.TFile('root/DataCache.root')
#w = tf.Get('w')
#data = w.data('Data')

w.factory( "Gaussian::dst_mass1( Dst_M, dst_mean[2000,2020], dst_sigma1[1,20] )" );
w.factory( "Gaussian::dst_mass2( Dst_M, dst_mean, dst_sigma2[1,20] )" );
w.factory( "SUM::dst_mass_sig( dst_f[0.1,1.]*dst_mass1, dst_mass2 )" );
w.factory( "Bernstein::dst_mass_bkg( Dst_M, {1.,dst_p0[0.,1.]} )" );
w.factory( "SUM::dst_mass( dst_mass_sy[0,10e5]*dst_mass_sig, dst_mass_by[0,10e5]*dst_mass_bkg )" );

w.factory( "Gaussian::d0_mass1( D0_M, d0_mean[1850,1870], d0_sigma1[1,20] )" );
w.factory( "Gaussian::d0_mass2( D0_M, d0_mean, d0_sigma2[1,20] )" );
w.factory( "SUM::d0_mass_sig( d0_f[0.1,1.]*d0_mass1, d0_mass2 )" );
w.factory( "Bernstein::d0_mass_bkg( D0_M, {1.,d0_p0[0.,1.]} )" );
w.factory( "SUM::d0_mass( d0_mass_sy[0,10e5]*d0_mass_sig, d0_mass_by[0,10e5]*d0_mass_bkg )" );

w.factory( "d0_tau[0,400]" );
w.factory( "expr::d0_e( '-1/@0', d0_tau)" );
w.factory( "Exponential::d0_t( D0_LTIME, d0_e )" );


w.pdf('dst_mass').fitTo(data)
w.pdf('d0_mass').fitTo(data)
w.pdf('d0_t').fitTo(data,r.RooFit.Range(0.25,5.))

text = r.TPaveText(0.74,0.8,0.87,0.9,"ndc");
text.SetFillColor(0);
text.SetShadowColor(0);
text.SetLineColor(0);
text.AddText('LHCb');

c1 = r.TCanvas('c1','c1',800,600)
c1.SetLeftMargin(0.18)

plot_dst_m = w.var('Dst_M').frame()
data.plotOn(plot_dst_m)
w.pdf('dst_mass').plotOn(plot_dst_m,r.RooFit.LineColor(r.kRed))
plot_dst_m.GetXaxis().SetTitle('m(D*^{+}) [MeV]')
plot_dst_m.GetYaxis().SetTitle('')
plot_dst_m.GetXaxis().SetTitleOffset(0.8)
plot_dst_m.GetYaxis().SetTitleOffset(0.8)
plot_dst_m.Draw()
text.Draw("same");

c2 = r.TCanvas('c2','c2',800,600)
c2.SetLeftMargin(0.18)

plot_d0_m = w.var('D0_M').frame()
data.plotOn(plot_d0_m)
w.pdf('d0_mass').plotOn(plot_d0_m,r.RooFit.LineColor(r.kRed))
plot_d0_m.GetXaxis().SetTitle('m(D^{0}) [MeV]')
plot_d0_m.GetYaxis().SetTitle('')
plot_d0_m.GetXaxis().SetTitleOffset(0.8)
plot_d0_m.GetYaxis().SetTitleOffset(0.8)
plot_d0_m.Draw()
text.Draw("same");

c3 = r.TCanvas('c3','c3',800,600)
c3.SetLeftMargin(0.18)

plot_d0_t = w.var('D0_LTIME').frame()
data.plotOn(plot_d0_t)
w.pdf('d0_t').plotOn(plot_d0_t,r.RooFit.LineColor(r.kRed))
plot_d0_t.GetXaxis().SetTitle('t(D^{0}) [ps]')
plot_d0_t.GetYaxis().SetTitle('')
plot_d0_t.GetXaxis().SetTitleOffset(0.8)
plot_d0_t.GetYaxis().SetTitleOffset(0.8)
plot_d0_t.Draw()
text.Draw("same");

c4 = r.TCanvas('c4','c4',800,600)
c4.SetLeftMargin(0.18)
c3.DrawClonePad()
c4.SetLogy()

c1.Update()
c1.Modified()
c1.Print('plots/LFit/pdf/Dst_M.pdf')
c1.Print('plots/LFit/png/Dst_M.png')
c1.Print('plots/LFit/C/Dst_M.C')

c2.Update()
c2.Modified()
c2.Print('plots/LFit/pdf/D0_M.pdf')
c2.Print('plots/LFit/png/D0_M.png')
c2.Print('plots/LFit/C/D0_M.C')

c3.Update()
c3.Modified()
c3.Print('plots/LFit/pdf/D0_t.pdf')
c3.Print('plots/LFit/png/D0_t.png')
c3.Print('plots/LFit/C/D0_t.C')

c4.Update()
c4.Modified()
c4.Print('plots/LFit/pdf/D0_t_log.pdf')
c4.Print('plots/LFit/png/D0_t_log.png')
c4.Print('plots/LFit/C/D0_t_log.C')


print '-------- RESULT ----------'
print 't = (%5.3f +/- %5.3f)ps'%(w.var('d0_tau').getVal(), w.var('d0_tau').getError())
print '--------------------------'

raw_input()

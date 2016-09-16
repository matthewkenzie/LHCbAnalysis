#!/usr/bin/env python

import ROOT as r
r.gROOT.ProcessLine('.x ~/bin/lhcbStyle.C')

import os
os.system('mkdir -p plots/LFit/pdf')
os.system('mkdir -p plots/LFit/png')
os.system('mkdir -p plots/LFit/C')

global canvs
global pads

canvs = []
pads = []

def getNewCanv():
  canvs.append( r.TCanvas('c%d'%len(canvs),'c%d'%len(canvs),800,600) )
  return canvs[-1]

def getNewPad(x1,y1,x2,y2):
  pads.append( r.TPad('p%d'%len(pads),'p%d'%len(pads), x1,y1,x2,y2) )
  return pads[-1]

def drawPlot(plot,fname,xtitle):

  c = getNewCanv()
  #pUp = getNewPad(0.,0.35,1.,1.)
  #pUp.SetBottomMargin(0.02)
  #pDn = getNewPad(0.,0.,1.,0.35)
  #pDn.SetTopMargin(0.02)
  #pDn.SetBottomMargin(0.3)

  c.cd()
  #pUp.Draw()
  #pDn.Draw()

  #pUp.cd()
  plot.GetXaxis().SetTitle(xtitle)
  plot.GetYaxis().SetTitle('')
  plot.GetXaxis().SetTitleSize(0.07)
  plot.GetXaxis().SetTitleOffset(0.9)
  plot.GetYaxis().SetTitleOffset(0.9)
  plot.GetXaxis().SetNdivisions(505)
  plot.Draw()

  lat = r.TLatex()
  lat.SetNDC()
  lat.SetTextSize(0.1)
  lat2 = r.TLatex()
  lat2.SetNDC()
  lat2.SetTextSize(0.082)
  lat.DrawLatex(0.75,0.83,'LHCb')
  lat2.DrawLatex(0.75,0.76,'Trigger')
  #text = r.TPaveText(0.74,0.8,0.87,0.9,"ndc");
  #text.SetFillColor(0);
  #text.SetShadowColor(0);
  #text.SetLineColor(0);
  #text.AddText('LHCb');
  #text.AddText('Trigger')
  #text.Draw("same");

  #resDum = r.TH1F('%s_resDum'%plot.GetName(),'',1,plot.GetXaxis().GetXmin(),plot.GetXaxis().GetXmax())
  #resid = plot.pullHist()

  #pDn.cd()
  #resid.Draw("AEP")
  #resDum.GetYaxis().SetRangeUser(resid.GetYaxis().GetXmin(),resid.GetYaxis().GetXmax())

  #resDum.GetXaxis().SetTitle(xtitle)
  #resDum.GetYaxis().SetTitle('')
  #resDum.GetXaxis().SetTitleOffset(0.9)
  #resDum.GetYaxis().SetTitleOffset(0.8)
  #resDum.GetXaxis().SetLabelSize(0.12)
  #resDum.GetYaxis().SetLabelSize(0.12)
  #resDum.GetXaxis().SetTitleSize(0.15)
  #resDum.GetXaxis().SetNdivisions(505)

  #pDn.cd()
  #resDum.Draw("AXIS")
  #l = r.TLine()
  #l.SetLineWidth(3)
  #l.SetLineColor(r.kRed)
  #l.DrawLine(resDum.GetXaxis().GetXmin(),0.,resDum.GetXaxis().GetXmax(),0.)
  #resid.Draw("EPsame")

  c.Update()
  c.Modified()

  c.Print('plots/LFit/png/%s.png'%fname)
  c.Print('plots/LFit/pdf/%s.pdf'%fname)
  c.Print('plots/LFit/C/%s.C'%fname)

  clog = getNewCanv()
  clog.cd()
  c.DrawClonePad()
  clog.SetLogy()
  #pUp.SetLogy()
  #pUp.Draw()
  #pDn.Draw()
  clog.Update()
  clog.Modified()

  clog.Print('plots/LFit/png/%s_log.png'%fname)
  clog.Print('plots/LFit/pdf/%s_log.pdf'%fname)
  clog.Print('plots/LFit/C/%s_log.C'%fname)

# __main__
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

# make plots
# 1.) Dst_M
plot_dst_m = w.var('Dst_M').frame()
data.plotOn(plot_dst_m)
w.pdf('dst_mass').plotOn(plot_dst_m,r.RooFit.LineColor(r.kRed))
drawPlot(plot_dst_m,'Dst_M','m(D*^{+}) [MeV/c^{2}]')

# 2.) D0_M
plot_d0_m = w.var('D0_M').frame()
data.plotOn(plot_d0_m)
w.pdf('d0_mass').plotOn(plot_d0_m,r.RooFit.LineColor(r.kRed))
drawPlot(plot_d0_m,'D0_M','m(D^{0}) [MeV/c^{2}]')

# 3.) D0_LTIME
plot_d0_t = w.var('D0_LTIME').frame()
data.plotOn(plot_d0_t)
w.pdf('d0_t').plotOn(plot_d0_t,r.RooFit.LineColor(r.kRed))
drawPlot(plot_d0_t,'D0_t','t(D^{0}) [ps]')

# 4.) D0_LTIME log

#c3 = r.TCanvas('c3','c3',800,600)
#c3.SetLeftMargin(0.18)

#plot_d0_t = w.var('D0_LTIME').frame()
#data.plotOn(plot_d0_t)
#w.pdf('d0_t').plotOn(plot_d0_t,r.RooFit.LineColor(r.kRed))
#plot_d0_t.GetXaxis().SetTitle('t(D^{0}) [ps]')
#plot_d0_t.GetYaxis().SetTitle('')
#plot_d0_t.GetXaxis().SetTitleOffset(0.8)
#plot_d0_t.GetYaxis().SetTitleOffset(0.8)
#plot_d0_t.Draw()
#text.Draw("same");

#c4 = r.TCanvas('c4','c4',800,600)
#c4.SetLeftMargin(0.18)
#c3.DrawClonePad()
#c4.SetLogy()

#c1.Update()
#c1.Modified()
#c1.Print('plots/LFit/pdf/Dst_M.pdf')
#c1.Print('plots/LFit/png/Dst_M.png')
#c1.Print('plots/LFit/C/Dst_M.C')

#c2.Update()
#c2.Modified()
#c2.Print('plots/LFit/pdf/D0_M.pdf')
#c2.Print('plots/LFit/png/D0_M.png')
#c2.Print('plots/LFit/C/D0_M.C')

#c3.Update()
#c3.Modified()
#c3.Print('plots/LFit/pdf/D0_t.pdf')
#c3.Print('plots/LFit/png/D0_t.png')
#c3.Print('plots/LFit/C/D0_t.C')

#c4.Update()
#c4.Modified()
#c4.Print('plots/LFit/pdf/D0_t_log.pdf')
#c4.Print('plots/LFit/png/D0_t_log.png')
#c4.Print('plots/LFit/C/D0_t_log.C')


print '-------- RESULT ----------'
print 't = (%5.3f +/- %5.3f)ps'%(w.var('d0_tau').getVal(), w.var('d0_tau').getError())
print '--------------------------'

raw_input()

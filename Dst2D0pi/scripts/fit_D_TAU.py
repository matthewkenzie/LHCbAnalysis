#!/usr/bin/env python

import ROOT as r

r.gROOT.ProcessLine(".x ~/bin/lhcbStyle.C")

tf = r.TFile("root/AnalysisOut.root")
tree = tf.Get('AnalysisTree')

xmin = 0.0005
xmax = 0.01

th1f = r.TH1F("t","",50,xmin,xmax)
tree.Draw('D0_TAU>>t','','goff')

tf1 = r.TF1("f","[0]*exp(-x/[1])",xmin,xmax)
th1f.Fit(tf1,"","",xmin,xmax)

canv = r.TCanvas()
th1f.Draw("HIST")
tf1.Draw("Lsame")
canv.Update()
canv.Modified()

print tf1.GetParameter(1), tf1.GetParError(1)

raw_input()



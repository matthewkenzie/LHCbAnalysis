import ROOT as r
r.TH1.SetDefaultSumw2()
r.gROOT.ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C")

import os
os.system('mkdir -p plots/PhiStudy/pdf')

tf = r.TFile("root/AnalysisOutWSWeights.root")

tree = tf.Get("AnalysisTree")

c = r.TCanvas("c","c",800,1800)
c.SetLeftMargin(0.2)

h1 = r.TH1F("h1","; #phi; Arbitary Units",50,-3.14,3.14)
h2 = r.TH1F("h2","",50,-3.14,3.14)
h3 = r.TH1F("h3","",50,-3.14,3.14)
h4 = r.TH1F("h4","",50,-3.14,3.14)
h5 = r.TH1F("h5","",50,-3.14,3.14)
h6 = r.TH1F("h6","",50,-3.14,3.14)

tree.Draw("B_s0_DTF_B_s0_Phi1>>h1", "itype>0", "goff")
tree.Draw("B_s0_DTF_B_s0_Phi1>>h2", "itype>0 && pass_bdt", "goff")
tree.Draw("B_s0_DTF_B_s0_Phi1>>h3", "itype>0 && pass_pid", "goff")
tree.Draw("B_s0_DTF_B_s0_Phi1>>h4", "itype>0 && pass_bdt && pass_pid", "goff")
tree.Draw("B_s0_DTF_B_s0_Phi1>>h5", "itype>0 && pass_all", "goff")
tree.Draw("B_s0_DTF_B_s0_Phi1>>h6", "sweight*(itype>0 && pass_all)", "goff")

h2.Scale( 0.8*h1.Integral() / h2.Integral() )
h3.Scale( 0.6*h1.Integral() / h3.Integral() )
h4.Scale( 0.42*h1.Integral() / h4.Integral() )
h5.Scale( 0.28*h1.Integral() / h5.Integral() )
h6.Scale( 0.1*h1.Integral() / h6.Integral() )

h1.GetYaxis().SetRangeUser(0,h1.GetMaximum()*1.1)
h1.GetYaxis().SetTitleSize(0.08)
h1.GetXaxis().SetTitleSize(0.08)
h1.GetYaxis().SetTitleOffset(1.2)
h1.GetXaxis().SetTitleOffset(0.7)
h1.SetLineColor(r.kBlack)
h2.SetLineColor(r.kRed)
h3.SetLineColor(r.kBlue)
h4.SetLineColor(r.kGreen+2)
h5.SetLineColor(r.kMagenta+1)
h6.SetLineColor(r.kBlack)

h1.Draw("HIST")
h2.Draw("HISTsame")
h3.Draw("HISTsame")
h4.Draw("HISTsame")
h5.Draw("HISTsame")
h6.Draw("HISTsame")

t1 = r.TLine()
t1.SetLineColor(r.kBlack)
t1.DrawLine(-3.14, h1.Integral()/h1.GetNbinsX(), 3.14, h1.Integral()/h1.GetNbinsX() )
t1.SetLineColor(r.kRed)
t1.DrawLine(-3.14, h2.Integral()/h1.GetNbinsX(), 3.14, h2.Integral()/h1.GetNbinsX() )
t1.SetLineColor(r.kBlue)
t1.DrawLine(-3.14, h3.Integral()/h1.GetNbinsX(), 3.14, h3.Integral()/h1.GetNbinsX() )
t1.SetLineColor(r.kGreen+2)
t1.DrawLine(-3.14, h4.Integral()/h1.GetNbinsX(), 3.14, h4.Integral()/h1.GetNbinsX() )
t1.SetLineColor(r.kMagenta+1)
t1.DrawLine(-3.14, h5.Integral()/h1.GetNbinsX(), 3.14, h5.Integral()/h1.GetNbinsX() )
t1.SetLineColor(r.kBlack)
t1.DrawLine(-3.14, h6.Integral()/h1.GetNbinsX(), 3.14, h6.Integral()/h1.GetNbinsX() )

pt = r.TLatex()
pt.SetTextAlign(22)
pt.SetTextSize(0.7*pt.GetTextSize())
pt.SetTextColor(r.kBlack)
pt.DrawLatex(0.,h1.Integral()/h1.GetNbinsX()-200,"Presel")
pt.SetTextColor(r.kRed)
pt.DrawLatex(0.,h2.Integral()/h1.GetNbinsX()-200,"Presel+BDT")
pt.SetTextColor(r.kBlue)
pt.DrawLatex(0.,h3.Integral()/h1.GetNbinsX()-200,"Presel+PID")
pt.SetTextColor(r.kGreen+2)
pt.DrawLatex(0.,h4.Integral()/h1.GetNbinsX()-200,"Presel+BDT+PID")
pt.SetTextColor(r.kMagenta+1)
pt.DrawLatex(0.,h5.Integral()/h1.GetNbinsX()-200,"Presel+BDT+PID+MassVeto")
pt.SetTextColor(r.kBlack)
pt.DrawLatex(0.,h6.Integral()/h1.GetNbinsX()-200,"Pass All w/ SWeight")

c.Update()
c.Modified()
c.Print("plots/PhiStudy/Phi.pdf")

c2 = r.TCanvas("c2","c2",800,1600)
c2.Divide(1,3)
hb1 = r.TH1F("hb1","",100,-0.5,0.5)
hb2 = r.TH1F("hb2","",100,-0.5,0.5)

tree.Draw("bdtoutput>>hb1","itype>0 && B_s0_DTF_B_s0_Phi1>0","goff")
tree.Draw("bdtoutput>>hb2","itype>0 && B_s0_DTF_B_s0_Phi1<0","goff")

hb1.SetLineColor(r.kRed)
hb2.SetLineColor(r.kBlue)

gr = r.TGraph()
for b in range(1,hb1.GetNbinsX()+1):
  evs_pos = hb1.Integral(b,hb1.GetNbinsX()+1)
  evs_neg = hb2.Integral(b,hb2.GetNbinsX()+1)
  gr.SetPoint(b-1, hb1.GetBinCenter(b), evs_pos-evs_neg)

c2.cd(1)
hb1.Draw("HIST")
hb2.Draw("HISTsame")

hbdiff = hb1.Clone("hbdiff")
hbdiff.Add(hb2,-1.)

c2.cd(2)
hbdiff.SetLineColor(r.kBlack)
hbdiff.Draw("HIST")

c2.cd(3)
gr.Draw("ALP")

c2.Update()
c2.Modified()

c2.Print("plots/bdtout_Phi.pdf")


raw_input()

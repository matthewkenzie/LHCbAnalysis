import ROOT as r

r.gROOT.ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C")
r.TH1.SetDefaultSumw2()

tf = r.TFile("root/AnalysisOutWSWeights.root")
tree = tf.Get("AnalysisTree")

tree.Draw( "B_s0_DTF_B_s0_M>>h1(100,5000,5800)", "itype>0 && pass_all && category==0", "goff")
tree.Draw( "B_s0_DTF_B_s0_M>>h2(100,5000,5800)", "itype>0 && pass_all && category==1", "goff")
tree.Draw( "B_s0_DTF_B_s0_M>>h3(100,5000,5800)", "itype>0 && pass_all && category==2", "goff")
tree.Draw( "B_s0_DTF_B_s0_M>>h4(100,5000,5800)", "itype>0 && pass_all && category==3", "goff")

h1 = r.gROOT.FindObject('h1')
h2 = r.gROOT.FindObject('h2')
h3 = r.gROOT.FindObject('h3')
h4 = r.gROOT.FindObject('h4')

c = r.TCanvas()
h1.SetLineColor(r.kRed)
h2.SetLineColor(r.kBlue)
h3.SetLineColor(r.kGreen+2)
h4.SetLineColor(r.kBlack)
h1.SetMarkerColor(r.kRed)
h2.SetMarkerColor(r.kBlue)
h3.SetMarkerColor(r.kGreen+2)
h4.SetMarkerColor(r.kBlack)

h1.Scale(1./h1.Integral())
h2.Scale(1./h2.Integral())
h3.Scale(1./h3.Integral())
h4.Scale(1./h4.Integral())

h1.GetXaxis().SetTitle("m(K^{+}#pi^{-}K^{-}#pi^{+}) [MeV]")
h1.GetXaxis().SetTitleSize(0.08)
h1.GetXaxis().SetTitleOffset(0.9)
h1.GetYaxis().SetTitle("Arbitary Units")
h1.GetYaxis().SetTitleSize(0.08)
h1.GetYaxis().SetTitleOffset(0.9)

leg = r.TLegend(0.6,0.6,0.9,0.9)
leg.SetFillColor(0)
leg.SetBorderSize(0)
leg.AddEntry(h1, "2011 Hadron TOS", "LEP")
leg.AddEntry(h2, "2011 Global TIS", "LEP")
leg.AddEntry(h3, "2012 Hadron TOS", "LEP")
leg.AddEntry(h4, "2012 Global TIS", "LEP")

h1.Draw("LEP")
h2.Draw("LEPsame")
h3.Draw("LEPsame")
h4.Draw("LEPsame")
leg.Draw()

c.Update()
c.Print("plots/DataSetsComp.pdf")
raw_input()


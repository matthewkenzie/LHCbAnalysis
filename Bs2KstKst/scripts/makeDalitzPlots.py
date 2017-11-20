import ROOT as r
import sys

r.gROOT.ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C")

if len(sys.argv)>1: r.gStyle.SetPalette(int(sys.argv[1]))
else: r.gStyle.SetPalette(55)

tf = r.TFile("root/AnalysisOutWSWeights.root")
t = tf.Get("AnalysisTree")

t.Draw("B_s0_DTF_KST1_M:B_s0_DTF_KST2_M>>h(85,750,1600,85,750,1600)","itype>0 && pass_all", "goff")
t.Draw("B_s0_DTF_KST1_M:B_s0_DTF_KST2_M>>hsw(85,750,1600,85,750,1600)","sweight*(itype>0 && pass_all)", "goff")

#t.Draw("B_s0_DTF_KST1_M*B_s0_DTF_KST1_M:B_s0_DTF_KST2_M*B_s0_DTF_KST2_M>>h","itype>0 && pass_all", "goff")
#t.Draw("B_s0_DTF_KST1_M*B_s0_DTF_KST1_M:B_s0_DTF_KST2_M*B_s0_DTF_KST2_M>>hsw","sweight*(itype>0 && pass_all)", "goff")

th2f = r.gROOT.FindObject("h")
th2fsw = r.gROOT.FindObject("hsw")

#th2f.GetZaxis().SetRangeUser(-0.01,22)
th2fsw.GetZaxis().SetRangeUser(0,20)

th2f.GetXaxis().SetTitle("#it{m}(#it{K}^{#plus}#it{#pi}^{#minus}) [MeV/#it{c}^{2}]")
th2f.GetYaxis().SetTitle("#it{m}(#it{K}^{#minus}#it{#pi}^{#plus}) [MeV/#it{c}^{2}]")
th2f.GetZaxis().SetTitle("Candidates")
th2f.GetXaxis().SetTitleSize(0.08)
th2f.GetYaxis().SetTitleSize(0.08)
th2f.GetZaxis().SetTitleSize(0.08)
th2f.GetXaxis().SetTitleOffset(0.9)
th2f.GetYaxis().SetTitleOffset(0.9)
th2f.GetZaxis().SetTitleOffset(0.6)

th2fsw.GetXaxis().SetTitle("#it{m}(#it{K}^{#plus}#it{#pi}^{#minus}) [MeV/#it{c}^{2}]")
th2fsw.GetYaxis().SetTitle("#it{m}(#it{K}^{#minus}#it{#pi}^{#plus}) [MeV/#it{c}^{2}]")
th2fsw.GetZaxis().SetTitle("Weighted candidates")
th2fsw.GetXaxis().SetTitleSize(0.08)
th2fsw.GetYaxis().SetTitleSize(0.08)
th2fsw.GetZaxis().SetTitleSize(0.08)
th2fsw.GetXaxis().SetTitleOffset(0.9)
th2fsw.GetYaxis().SetTitleOffset(0.9)
th2fsw.GetZaxis().SetTitleOffset(0.6)

box = r.TBox(1300,1450,1550,1580)
box.SetFillColor(r.kWhite)
box.SetLineColor(r.kBlack)
box.SetLineWidth(6)
text = r.TPaveText(0.6,0.88,0.8,0.85,"ndc")
text.SetFillColor(r.kWhite)
text.SetFillStyle(0)
text.SetLineColor(0)
text.SetShadowColor(0)
text.SetBorderSize(0)
text.SetTextSize(0.1)
text.AddText("LHCb")

c = r.TCanvas()
c.SetRightMargin(0.15)
c.SetLeftMargin(0.15)
#th2f.Draw("box")
th2f.Draw("colz")
box.Draw()
text.Draw()
c.Update()

c2 = r.TCanvas()
c2.SetRightMargin(0.15)
c2.SetLeftMargin(0.15)
#th2fsw.Draw("box")
th2fsw.Draw("colz")
box.Draw()
text.Draw()
c2.Update()

c.Print("plots/Paper/pdf/DataDalitz.pdf")
c2.Print("plots/Paper/pdf/DataDalitz_wsweights.pdf")
c.Print("plots/Paper/png/DataDalitz.png")
c2.Print("plots/Paper/png/DataDalitz_wsweights.png")
c.Print("plots/Paper/C/DataDalitz.C")
c2.Print("plots/Paper/C/DataDalitz_wsweights.C")

#c.Print("plots/Paper/pdf/DataDalitz_box.pdf")
#c2.Print("plots/Paper/pdf/DataDalitz_wsweights_box.pdf")
#c.Print("plots/Paper/png/DataDalitz_box.png")
#c2.Print("plots/Paper/png/DataDalitz_wsweights_box.png")
#c.Print("plots/Paper/C/DataDalitz_box.C")
#c2.Print("plots/Paper/C/DataDalitz_wsweights_box.C")


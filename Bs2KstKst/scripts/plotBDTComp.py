import ROOT as r

r.gROOT.ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C")

tf = r.TFile("root/AnalysisOutWSWeights.root")
tree = tf.Get("AnalysisTree")

tree.Draw( "bdtoutput>>h1(100,-1,1)", "itype==-70", "goff")
tree.Draw( "bdtoutput>>h2(100,-1,1)", "itype==71 && B_s0_DTF_B_s0_M>5600", "goff")
tree.Draw( "bdtoutput>>h3(100,-1,1)", "itype==71","goff")

h1 = r.gROOT.FindObject('h1')
h2 = r.gROOT.FindObject('h2')
h3 = r.gROOT.FindObject('h3')

h1.SetFillColorAlpha(r.kBlue-4, 0.4)
h2.SetFillColorAlpha(r.kRed-4, 0.4)
h3.SetMarkerStyle(r.kFullCircle)

c = r.TCanvas()
h1.GetXaxis().SetTitle("BDT Output")
h1.GetXaxis().SetTitleSize(0.08)
h1.GetXaxis().SetTitleOffset(0.9)

h3.Draw("LEP")
h1.Draw("HISTFsame")
h2.Draw("HISTFsame")
h3.Draw("LEPsame")

c.Update()
raw_input()

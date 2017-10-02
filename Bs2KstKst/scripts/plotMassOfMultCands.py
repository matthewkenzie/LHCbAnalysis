import ROOT as r

import os
os.system('mkdir -p plots/MultCands/pdf')
os.system('mkdir -p plots/MultCands/png')
os.system('mkdir -p plots/MultCands/eps')
os.system('mkdir -p plots/MultCands/C')

def plot( tree, var, bins, xmin, xmax, xlabel ):

    h = r.TH1F( var, var, bins, xmin, xmax)
    h.GetXaxis().SetTitle(xlabel)
    h.GetXaxis().SetTitleOffset(0.8)

    tree.Draw( "%s >> %s"%(var,var), "itype>0 && pass_bdt && pass_pid && !pass_multcand && !pass_rhokst && !pass_massveto", "goff" )

    c = r.TCanvas()
    h.Draw("HIST")
    c.Update()
    c.Modified()
    c.Print("plots/MultCands/pdf/%s.pdf"%var)
    c.Print("plots/MultCands/png/%s.png"%var)
    c.Print("plots/MultCands/eps/%s.eps"%var)
    c.Print("plots/MultCands/C/%s.C"%var)


r.gROOT.ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C")
tf = r.TFile("root/AnalysisOut.root")
tree = tf.Get("AnalysisTree")
plot( tree, "B_s0_DTF_B_s0_M", 100, 5200,5800, "m(K^{+}#pi^{-}K^{-}#pi^{+}) [MeV]" )
plot( tree, "B_s0_DTF_KST1_M", 100, 700, 1700, "m(K^{-}#pi^{+}) [MeV]")
plot( tree, "B_s0_DTF_KST2_M", 100, 700, 1700, "m(K^{+}#pi^{-}) [MeV]")



import ROOT as r

def drawAsymm(c, var, itypestr, bins=50, xmin=-1, xmax=1):

    hp = r.TH1F("hp","hp",bins,xmin,xmax)
    hm = r.TH1F("hm","hm",bins,xmin,xmax)

    tree.Draw("%s>>hp"%var, itypestr + " && B_s0_DTF_B_s0_Phi1>0", "goff")
    tree.Draw("%s>>hm"%var, itypestr + " && B_s0_DTF_B_s0_Phi1<0", "goff")

    hs = hp.Clone("hs")
    hs.Add(hm,-1.)

    hs.Draw()
    c.Update()
    c.Modified()
    return (hp,hm,hs)



tf = r.TFile("~/cernbox/Bs2KstKst/AnalysisFiles/AnalysisOutWSWeights.root")
tree = tf.Get("AnalysisTree")

c = r.TCanvas()

(hp,hm,hs) = drawAsymm(c, "B_s0_DTF_B_s0_CosTheta2", "(itype==-70 || itype==-80)")
#(hp,hm,hs) = drawAsymm(c, "B_s0_M_PipPimKmPip", "(itype==-70 || itype==-80)")
#(hp,hm,hs) = drawAsymm(c, "B_s0_DTF_B_s0_CosTheta1", "(itype==-70 || itype==-80)")

raw_input()

import ROOT as r

r.gROOT.ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C")
r.TH1.SetDefaultSumw2()
tf = r.TFile("root/AnalysisOutWSWeights.root")
tree = tf.Get("AnalysisTree")

def draw(tree, name, scale, var, hists=None):

  h1 = None
  h2 = None

  if not hists:
    tree.Draw(var+'>>'+name+'_h1', "(itype==-70 || itype==-80)", "goff")
    tree.Draw(var+'>>'+name+'_h2', "sweight*(itype>0 && pass_all)","goff")

    h1 = r.gROOT.FindObject(name+'_h1')
    h2 = r.gROOT.FindObject(name+'_h2')
  else:
    h1 = hists[0]
    h2 = hists[1]

  h1.SetLineColor(r.kBlue)
  h1.SetStats(0)
  h1.GetXaxis().SetTitle(name)
  h1.GetXaxis().SetTitleSize(0.08)
  h1.GetXaxis().SetTitleOffset(0.9)
  h2.SetMarkerStyle(r.kFullCircle)
  h2.SetMarkerColor(r.kBlack)
  h2.SetLineColor(r.kBlack)

  h2.Scale( scale*h1.Integral()/h2.Integral() )

  c = r.TCanvas()
  h1.Draw("HIST")
  h2.Draw("LEPsame")
  c.Update()
  c.Print("plots/BDTNoPID/pdf/sw_comp_%s.pdf"%name)
  return c

def getEta(tree):

  minMC   = r.TH1F('min_track_eta_h1','h1',100,0,6)
  minData = r.TH1F('min_track_eta_h2','h2',100,0,6)
  maxMC   = r.TH1F('max_track_eta_h1','h1',100,0,6)
  maxData = r.TH1F('max_track_eta_h2','h2',100,0,6)

  tree.SetBranchStatus("*",0)
  tree.SetBranchStatus("Kplus_PT", 1)
  tree.SetBranchStatus("Kminus_PT", 1)
  tree.SetBranchStatus("Piplus_PT", 1)
  tree.SetBranchStatus("Piminus_PT", 1)
  tree.SetBranchStatus("Kplus_ETA", 1)
  tree.SetBranchStatus("Kminus_ETA", 1)
  tree.SetBranchStatus("Piplus_ETA", 1)
  tree.SetBranchStatus("Piminus_ETA", 1)
  tree.SetBranchStatus("itype",1)
  tree.SetBranchStatus("pass_all",1)
  tree.SetBranchStatus("sweight", 1)

  nentries = tree.GetEntries()
  for i in range(nentries):
    if (i%1000==0): print i, '/', nentries
    tree.GetEntry(i)
    if tree.itype!=-70 and tree.itype!=-80 and tree.itype!=71 and tree.itype!=81: continue

    pts  = [ tree.Kplus_PT , tree.Kminus_PT , tree.Piplus_PT , tree.Piminus_PT ]
    etas = [ r.TMath.Abs(tree.Kplus_ETA), r.TMath.Abs(tree.Kminus_ETA), r.TMath.Abs(tree.Piplus_ETA), r.TMath.Abs(tree.Piminus_ETA) ]
    imax = -1
    imin = -1
    maxpt = -1.e10
    minpt = 1.e10
    for i in range(4):
      if pts[i] > maxpt:
        maxpt = pts[i]
        imax = i
      if pts[i] < minpt:
        minpt = pts[i]
        imin = i

    min_eta = etas[imin]
    max_eta = etas[imax]

    if (tree.itype==71 or tree.itype==81) and tree.pass_all:
      minData.Fill( min_eta, tree.sweight )
      maxData.Fill( max_eta, tree.sweight )
    if (tree.itype==-70 or tree.itype==-80):
      minMC.Fill( min_eta )
      maxMC.Fill( max_eta )

  return [minData, maxData, minMC, maxMC]

canvs = []
#canvs.append ( draw(tree, 'B_s0_DTF_B_s0_PT'   , 1.2, 'B_s0_DTF_B_s0_PT') )
#canvs.append ( draw(tree, 'B_s0_DTF_KST1_PT'   , 1.4, 'B_s0_DTF_KST1_PT') )
#canvs.append ( draw(tree, 'B_s0_DTF_KST2_PT'   , 1.2, 'B_s0_DTF_KST2_PT') )
#canvs.append ( draw(tree, 'min_track_PT'       , 1.8, 'TMath::Min(TMath::Min(Kplus_PT, Kminus_PT), TMath::Min(Piplus_PT, Piminus_PT))') )
#canvs.append ( draw(tree, 'max_track_PT'       , 1.1, 'TMath::Max(TMath::Max(Kplus_PT, Kminus_PT), TMath::Max(Piplus_PT, Piminus_PT))') )
#canvs.append ( draw(tree, 'B_s0_ETA'           , 0.7, 'TMath::Abs(B_s0_ETA)') )
#canvs.append ( draw(tree, 'Kst_ETA'            , 0.95, 'TMath::Abs(Kst_ETA)') )
#canvs.append ( draw(tree, 'Kstb_ETA'           , 1., 'TMath::Abs(Kstb_ETA)') )
#canvs.append ( draw(tree, 'min_track_ETA'      , 0.8, 'TMath::Min(TMath::Min(TMath::Abs(Kplus_ETA), TMath::Abs(Kminus_ETA)), TMath::Min(TMath::Abs(Piplus_ETA), TMath::Abs(Piminus_ETA)))') )
#canvs.append ( draw(tree, 'max_track_ETA'      , 1., 'TMath::Max(TMath::Max(TMath::Abs(Kplus_ETA), TMath::Abs(Kminus_ETA)), TMath::Max(TMath::Abs(Piplus_ETA), TMath::Abs(Piminus_ETA)))') )
#canvs.append ( draw(tree, 'B_s0_DIRA_OWNPV'    , 1., 'B_s0_DIRA_OWNPV') )
#canvs.append ( draw(tree, 'B_s0_ENDVERTEX_CHI2', 1.1, 'B_s0_ENDVERTEX_CHI2') )
canvs.append ( draw(tree, 'bdtoutput'          , 1.8 , 'bdtoutput') )

#hists = getEta(tree)
#canvs.append( draw(tree, 'min_track_ETA', 1., '', [hists[2],hists[0]]) )
#canvs.append( draw(tree, 'max_track_ETA', 0.9, '', [hists[3],hists[1]]) )

raw_input()

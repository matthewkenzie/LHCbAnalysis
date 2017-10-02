import ROOT as r

def splitByYearAndPol(tree,var,xmin,xmax,sweight=False,nopass=False):

  c1 = r.TCanvas("c1","c1",1600,1200)
  c1.Divide(2,2)

  #h1 = r.TH1F("h1","2011 MagUp",50,xmin,xmax)
  #h2 = r.TH1F("h2","2011 MagDown",50,xmin,xmax)
  #h3 = r.TH1F("h3","2012 MagUp",50,xmin,xmax)
  #h4 = r.TH1F("h4","2012 MagDown",50,xmin,xmax)
  h1 = r.TH1F("h1","2011 Hadron TOS",50,xmin,xmax)
  h2 = r.TH1F("h2","2011 Glabal TIS",50,xmin,xmax)
  h3 = r.TH1F("h3","2012 Hadron TOS",50,xmin,xmax)
  h4 = r.TH1F("h4","2012 Global TIS",50,xmin,xmax)

  if nopass:
    if sweight:
      tree.Draw("%s>>h1"%var,"sweight*(itype==71 && Polarity==1)","goff")
      tree.Draw("%s>>h2"%var,"sweight*(itype==71 && Polarity==-1)","goff")
      tree.Draw("%s>>h3"%var,"sweight*(itype==81 && Polarity==1)","goff")
      tree.Draw("%s>>h4"%var,"sweight*(itype==81 && Polarity==-1)","goff")
    else:
      tree.Draw("%s>>h1"%var,"itype==71 && Polarity==1","goff")
      tree.Draw("%s>>h2"%var,"itype==71 && Polarity==-1","goff")
      tree.Draw("%s>>h3"%var,"itype==81 && Polarity==1","goff")
      tree.Draw("%s>>h4"%var,"itype==81 && Polarity==-1","goff")
  else:
    if sweight:
      #tree.Draw("%s>>h1"%var,"sweight*(itype==71 && Polarity==1 && pass_all)","goff")
      #tree.Draw("%s>>h2"%var,"sweight*(itype==71 && Polarity==-1 && pass_all)","goff")
      #tree.Draw("%s>>h3"%var,"sweight*(itype==81 && Polarity==1 && pass_all)","goff")
      #tree.Draw("%s>>h4"%var,"sweight*(itype==81 && Polarity==-1 && pass_all)","goff")
      tree.Draw("%s>>h1"%var,"sweight*(itype>0 && category==0 && pass_all)","goff")
      tree.Draw("%s>>h2"%var,"sweight*(itype>0 && category==1 && pass_all)","goff")
      tree.Draw("%s>>h3"%var,"sweight*(itype>0 && category==2 && pass_all)","goff")
      tree.Draw("%s>>h4"%var,"sweight*(itype>0 && category==3 && pass_all)","goff")
    else:
      #tree.Draw("%s>>h1"%var,"itype==71 && Polarity==1 && pass_all","goff")
      #tree.Draw("%s>>h2"%var,"itype==71 && Polarity==-1 && pass_all","goff")
      #tree.Draw("%s>>h3"%var,"itype==81 && Polarity==1 && pass_all","goff")
      #tree.Draw("%s>>h4"%var,"itype==81 && Polarity==-1 && pass_all","goff")
      tree.Draw("%s>>h1"%var,"itype>0 && category==0 && pass_all","goff")
      tree.Draw("%s>>h2"%var,"itype>0 && category==1 && pass_all","goff")
      tree.Draw("%s>>h3"%var,"itype>0 && category==2 && pass_all","goff")
      tree.Draw("%s>>h4"%var,"itype>0 && category==3 && pass_all","goff")

  if nopass:
    h1.GetYaxis().SetRangeUser(h1.GetMaximum()-400,h1.GetMaximum()+50)
    h2.GetYaxis().SetRangeUser(h2.GetMaximum()-400,h2.GetMaximum()+50)
    h3.GetYaxis().SetRangeUser(h3.GetMaximum()-400,h3.GetMaximum()+50)
    h4.GetYaxis().SetRangeUser(h4.GetMaximum()-400,h4.GetMaximum()+50)
  c1.cd(1)
  h1.Draw()
  c1.cd(2)
  h2.Draw()
  c1.cd(3)
  h3.Draw()
  c1.cd(4)
  h4.Draw()

  c1.Update()
  c1.Modified()

  #c2 = r.TCanvas()
  #c2.cd()

  #h1.SetLineColor(r.kBlack)
  #h2.SetLineColor(r.kBlue)
  #h3.SetLineColor(r.kRed)
  #h4.SetLineColor(r.kGreen+2)

  #h1.Scale(1./h1.Integral())
  #h2.Scale(1./h2.Integral())
  #h3.Scale(1./h3.Integral())
  #h4.Scale(1./h4.Integral())

  #h1.Draw()
  #h2.Draw("same")
  #h3.Draw("same")
  #h4.Draw("same")

  #c2.Update()
  #c2.Modified()

  if nopass:
    if sweight:
      c1.Print("plots/%s_np_sw.pdf"%var)
    else:
      c1.Print("plots/%s_np.pdf"%var)
  else:
    if sweight:
      c1.Print("plots/%s_sw.pdf"%var)
    else:
      c1.Print("plots/%s.pdf"%var)

f = r.TFile('root/AnalysisOutWSWeights.root')
t = f.Get("AnalysisTree")
#splitByYearAndPol(t,"B_s0_DTF_B_s0_Phi1",-3.7,3.7,False,True)
splitByYearAndPol(t,"B_s0_DTF_B_s0_Phi1",-3.7,3.7)
splitByYearAndPol(t,"B_s0_DTF_B_s0_Phi1",-3.7,3.7,True)
#splitByYearAndPol(t,"B_s0_DTF_B_s0_CosTheta1",-1,1)
#splitByYearAndPol(t,"B_s0_DTF_B_s0_CosTheta1",-1,1,True)
#splitByYearAndPol(t,"B_s0_DTF_B_s0_CosTheta2",-1,1)
#splitByYearAndPol(t,"B_s0_DTF_B_s0_CosTheta2",-1,1,True)

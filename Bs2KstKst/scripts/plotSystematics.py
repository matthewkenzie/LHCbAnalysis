import ROOT as r

def drawWeight(tree):
  tree.Draw('sweight>>h0(50,-0.6,1.2)','','goff')
  tree.Draw('shift_yield_up1_sw>>h1(50,-0.6,1.2)','','goff')
  tree.Draw('shift_yield_dn1_sw>>h2(50,-0.6,1.2)','','goff')
  tree.Draw('altsigmodel_sw>>h3(50,-0.6,1.2)','','goff')
  tree.Draw('altbkgmodel_sw>>h4(50,-0.6,1.2)','','goff')
  tree.Draw('magup_sw>>h5(50,-0.6,1.2)','','goff')
  tree.Draw('magdn_sw>>h6(50,-0.6,1.2)','','goff')
  tree.Draw('phi0_sw>>h7(50,-0.6,1.2)','','goff')
  tree.Draw('phi1_sw>>h8(50,-0.6,1.2)','','goff')
  tree.Draw('phi2_sw>>h9(50,-0.6,1.2)','','goff')
  tree.Draw('phi3_sw>>h10(50,-0.6,1.2)','','goff')

  canv.cd()

  r.gROOT.FindObject('h1').SetLineColor(r.kRed)
  r.gROOT.FindObject('h2').SetLineColor(r.kRed)
  r.gROOT.FindObject('h3').SetLineColor(r.kBlue)
  r.gROOT.FindObject('h4').SetLineColor(r.kBlue)
  r.gROOT.FindObject('h5').SetLineColor(r.kGreen+2)
  r.gROOT.FindObject('h6').SetLineColor(r.kGreen+2)
  r.gROOT.FindObject('h7').SetLineColor(r.kMagenta)
  r.gROOT.FindObject('h8').SetLineColor(r.kMagenta)
  r.gROOT.FindObject('h9').SetLineColor(r.kMagenta)
  r.gROOT.FindObject('h10').SetLineColor(r.kMagenta)

  r.gROOT.FindObject('h0').Draw("HIST")
  r.gROOT.FindObject('h1').Draw("HISTsame")
  r.gROOT.FindObject('h2').Draw("HISTsame")
  r.gROOT.FindObject('h3').Draw("HISTsame")
  r.gROOT.FindObject('h4').Draw("HISTsame")
  r.gROOT.FindObject('h5').Draw("HISTsame")
  r.gROOT.FindObject('h6').Draw("HISTsame")
  r.gROOT.FindObject('h7').Draw("HISTsame")
  r.gROOT.FindObject('h8').Draw("HISTsame")
  r.gROOT.FindObject('h9').Draw("HISTsame")
  r.gROOT.FindObject('h10').Draw("HISTsame")

  canv.Update()

def drawSyst(tree, var):

  tree.Draw(var+'>>'+var+'h0','sweight'           ,'goff')
  tree.Draw(var+'>>'+var+'h1','shift_yield_up1_sw','goff')
  tree.Draw(var+'>>'+var+'h2','shift_yield_dn1_sw','goff')
  tree.Draw(var+'>>'+var+'h3','altsigmodel_sw'    ,'goff')
  tree.Draw(var+'>>'+var+'h4','altbkgmodel_sw'    ,'goff')
  tree.Draw(var+'>>'+var+'h5','magup_sw'          ,'goff')
  tree.Draw(var+'>>'+var+'h6','magdn_sw'          ,'goff')
  tree.Draw(var+'>>'+var+'h7','phi0_sw'           ,'goff')
  tree.Draw(var+'>>'+var+'h8','phi1_sw'           ,'goff')
  tree.Draw(var+'>>'+var+'h9','phi2_sw'           ,'goff')
  tree.Draw(var+'>>'+var+'h10','phi3_sw'          ,'goff')

  canv.cd()

  r.gROOT.FindObject(var+'h1').SetLineColor(r.kRed)
  r.gROOT.FindObject(var+'h2').SetLineColor(r.kRed)
  r.gROOT.FindObject(var+'h3').SetLineColor(r.kBlue)
  r.gROOT.FindObject(var+'h4').SetLineColor(r.kBlue)
  r.gROOT.FindObject(var+'h5').SetLineColor(r.kGreen+2)
  r.gROOT.FindObject(var+'h6').SetLineColor(r.kGreen+2)
  r.gROOT.FindObject(var+'h7').SetLineColor(r.kMagenta)
  r.gROOT.FindObject(var+'h8').SetLineColor(r.kMagenta)
  r.gROOT.FindObject(var+'h9').SetLineColor(r.kMagenta)
  r.gROOT.FindObject(var+'h10').SetLineColor(r.kMagenta)

  r.gROOT.FindObject(var+'h1').SetMarkerColor(r.kRed)
  r.gROOT.FindObject(var+'h2').SetMarkerColor(r.kRed)
  r.gROOT.FindObject(var+'h3').SetMarkerColor(r.kBlue)
  r.gROOT.FindObject(var+'h4').SetMarkerColor(r.kBlue)
  r.gROOT.FindObject(var+'h5').SetMarkerColor(r.kGreen+2)
  r.gROOT.FindObject(var+'h6').SetMarkerColor(r.kGreen+2)
  r.gROOT.FindObject(var+'h7').SetMarkerColor(r.kMagenta)
  r.gROOT.FindObject(var+'h8').SetMarkerColor(r.kMagenta)
  r.gROOT.FindObject(var+'h9').SetMarkerColor(r.kMagenta)
  r.gROOT.FindObject(var+'h10').SetMarkerColor(r.kMagenta)

  r.gROOT.FindObject(var+'h0').Draw("LEP")
  r.gROOT.FindObject(var+'h1').Draw("LEPsame")
  r.gROOT.FindObject(var+'h2').Draw("LEPsame")
  r.gROOT.FindObject(var+'h3').Draw("LEPsame")
  r.gROOT.FindObject(var+'h4').Draw("LEPsame")
  r.gROOT.FindObject(var+'h5').Draw("LEPsame")
  r.gROOT.FindObject(var+'h6').Draw("LEPsame")
  r.gROOT.FindObject(var+'h7').Draw("LEPsame")
  r.gROOT.FindObject(var+'h8').Draw("LEPsame")
  r.gROOT.FindObject(var+'h9').Draw("LEPsame")
  r.gROOT.FindObject(var+'h10').Draw("LEPsame")

  canv.Update()

canv = r.TCanvas()
tf = r.TFile('root/AnalysisOutTrimmedSmall.root')
tree = tf.Get('AnalysisTree')

drawWeight(tree)

drawSyst(tree, "B_s0_DTF_KST1_M")
raw_input()
#while 1:
  #var = raw_input('Which var you wanna look at?\n')
  #print var
  #drawSyst(tree, var)


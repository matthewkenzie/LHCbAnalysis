
import ROOT as r
r.TH1.SetDefaultSumw2()
r.gROOT.ProcessLine(".x ~/Scratch/lhcb/lhcbStyle.C")
r.gROOT.SetBatch()

import os
os.system('mkdir -p plots/MassVetoes/pdf')
os.system('mkdir -p plots/MassVetoes/png')
os.system('mkdir -p plots/MassVetoes/eps')
os.system('mkdir -p plots/MassVetoes/C')
os.system('mkdir -p plots/MassVetoes/root')

tf = r.TFile("root/AnalysisOutWSWeightsOldMassVeto.root")
tree = tf.Get("AnalysisTree")

c = r.TCanvas("c","c",1600,1800)
c.Divide(2,3)

m_B_s0 = 5.3663*1000.
m_K    = 0.493677*1000.
m_Pi   = 0.13957018*1000.
m_P    = 0.93827203*1000.

vars = [ 'KpPimPimPip', 'KpPimpbPip', 'KpKmKmPip', 'PipPimKmPip', 'pPimKmPip', 'KpPimKmKp' ]
xtitles = ['m(K^{+}#pi^{-}#pi^{-}#pi^{+})', 'm(K^{+}#pi^{-}#bar{p}#pi^{+})', 'm(K^{+}K^{-}K^{-}#pi^{+})', 'm(#pi^{+}#pi^{-}K^{-}#pi^{+})', 'm(p#pi^{-}K^{-}#pi^{+})', 'm(K^{+}#pi^{-}K^{-}K^{+})' ]
vetoes = [ [5000,5400], [5400,6200], [5350,5800], [5000,5400], [5400,6200], [5350,5800] ]
#vetoes = [ [4000,8000], [5350,8000], [5270,8000], [4000,8000], [5350,8000], [5270,8000] ]
limits = [ [4800,5600], [5200,6400], [5200,6000], [4800,5600], [5200,6400], [5200,6000] ]
hists = []

leg = r.TLegend(0.17,0.48,0.48,0.92)
leg.SetLineColor(1)
leg.SetLineWidth(1)

for i, var in enumerate(vars):
  c.cd(i*2+1-(i>=3)*5)
  h_sig1 = r.TH1F("h_sig1%s"%var,"M_%s; %s"%(var,xtitles[i]),100,limits[i][0],limits[i][1])
  h_sig2 = r.TH1F("h_sig2%s"%var,"M_%s; %s"%(var,xtitles[i]),100,limits[i][0],limits[i][1])
  h_sig3 = r.TH1F("h_sig3%s"%var,"M_%s; %s"%(var,xtitles[i]),100,limits[i][0],limits[i][1])
  h_sig4 = r.TH1F("h_sig4%s"%var,"M_%s; %s"%(var,xtitles[i]),100,limits[i][0],limits[i][1])
  tree.Draw( 'B_s0_M_%s>>h_sig1%s'%(var,var), '(itype==-70 || itype==-80) && pass_all' )
  tree.Draw( 'B_s0_M_%s>>h_sig2%s'%(var,var), '(itype==-71 || itype==-81) && pass_all' )
  tree.Draw( 'B_s0_M_%s>>h_sig3%s'%(var,var), '(itype==-72 || itype==-82) && pass_all' )
  tree.Draw( 'B_s0_M_%s>>h_sig4%s'%(var,var), '(itype==-73 || itype==-83) && pass_all' )

  h_sig1.SetFillColor(r.kRed)
  h_sig2.SetFillColor(r.kBlue)
  h_sig3.SetFillColor(r.kGreen+1)
  h_sig4.SetFillColor(r.kMagenta-1)

  h_sig = r.THStack("hs%s"%var,"M_%s; %s"%(var,xtitles[i]))
  h_sig.Add(h_sig1)
  h_sig.Add(h_sig2)
  h_sig.Add(h_sig3)
  h_sig.Add(h_sig4)

  h_data = r.TH1F("h_data%s"%var,"; %s"%xtitles[i],100,limits[i][0],limits[i][1])
  tree.Draw( 'B_s0_M_%s>>h_data%s'%(var,var), '(itype>0) && pass_all' )

  h_data.Scale( 2.*(h_sig1.Integral() + h_sig2.Integral() + h_sig3.Integral() + h_sig4.Integral() ) / h_data.Integral() )

  h_data.SetLineColor(r.kBlack)
  h_data.SetMarkerColor(r.kBlack)
  h_data.SetMarkerStyle(r.kFullCircle)
  h_data.SetLineWidth(1)
  h_data.SetMarkerSize(0.3)

  h_data.GetYaxis().SetRangeUser(0, h_data.GetMaximum()*1.1)
  h_data.GetXaxis().SetTitleSize(0.07)
  h_data.GetXaxis().SetTitleOffset(0.8)
  h_data.Draw("LEP")
  h_sig.Draw("HISTFsame")
  h_data.Draw("LEPsame")
  h_sig.GetXaxis().SetTitleSize(0.07)
  h_sig.GetXaxis().SetTitleOffset(0.8)

  box = r.TBox()
  box.SetFillColorAlpha(r.kGray,0.6)
  box.SetFillStyle(1001)
  if (vetoes[i][0]>limits[i][0]):
    box.DrawBox( limits[i][0], 0, vetoes[i][0], h_data.GetMaximum() )
  if (vetoes[i][1]<limits[i][1]):
    box.DrawBox( vetoes[i][1], 0., limits[i][1], h_data.GetMaximum() )

  m_B_s0_line = r.TLine()
  m_B_s0_line.SetLineWidth(3)
  m_B_s0_line.SetLineColor(r.kBlue+2)
  #m_B_s0_line.DrawLine( m_B_s0, 0, m_B_s0, h_data.GetMaximum() )

  print m_B_s0, h_sig1.GetBinCenter(h_sig1.GetMaximumBin())

  if i==0:
    leg.AddEntry( h_sig1, "B_{s}#rightarrow K^{*0}(892)#bar{K}^{*0}(892)", "F");
    leg.AddEntry( h_sig2, "B_{s}#rightarrow K^{*0}(892)#bar{K}^{*0}(1430)", "F");
    leg.AddEntry( h_sig3, "B_{s}#rightarrow K^{*0}(1430)#bar{K}^{*0}(1430)", "F");
    leg.AddEntry( h_sig4, "B_{s}#rightarrow (K^{+}#pi^{-})(K^{-}#pi^{+}) PS", "F");
    leg.AddEntry( h_data, "Data", "LEP");
    leg.AddEntry( box,    "Mass Veto", "F");

  c.cd(i*2+1-(i>=3)*5).RedrawAxis()
  hists.append( [h_sig1, h_sig2, h_sig3, h_sig4, h_data, h_sig, box] )

c.cd(1)
leg.Draw()
c.cd()
c.Update()
c.Modified()
c.Print("plots/MassVetoes/pdf/InvMasses.pdf")
c.Print("plots/MassVetoes/png/InvMasses.png")
c.Print("plots/MassVetoes/eps/InvMasses.eps")
c.Print("plots/MassVetoes/C/InvMasses.C")
c.SaveAs("plots/MassVetoes/root/InvMasses.root")

# now focus on the Lb invariant mass peak
c2 = r.TCanvas("c2","c2",1600,1200)
c2.Divide(2,2)
cuts = [0.0,0.2,0.4,0.6]
cut_hists=[]

leg1 = r.TLegend(0.6,0.6,0.9,0.9)
leg2 = r.TLegend(0.6,0.6,0.9,0.9)

for i, probNNp_cut in enumerate(cuts):
  h_data_at_cut_pb = r.TH1F( "h_data_pb_c%d"%i, "Data; Data %s"%xtitles[1], 100, limits[1][0], limits[1][1] )
  h_data_at_cut_p  = r.TH1F( "h_data_p_c%d"%i, "Data; Data %s"%xtitles[4], 100, limits[1][0], limits[1][1] )
  h_sig_at_cut_pb = r.TH1F( "h_sig_pb_c%d"%i, "Signal MC; Signal %s"%xtitles[1], 100, limits[1][0], limits[1][1] )
  h_sig_at_cut_p  = r.TH1F( "h_sig_p_c%d"%i, "Signal MC; Signal %s"%xtitles[4], 100, limits[1][0], limits[1][1] )
  tree.Draw( 'B_s0_M_KpPimpbPip>>h_data_pb_c%d'%i, 'itype>0 && pass_all && Kminus_ProbNNp>%4.2f'%probNNp_cut, "goff" )
  tree.Draw( 'B_s0_M_pPimKmPip>>h_data_p_c%d'%i, 'itype>0 && pass_all && Kplus_ProbNNp>%4.2f'%probNNp_cut, "goff" )
  tree.Draw( 'B_s0_M_KpPimpbPip>>h_sig_pb_c%d'%i, '(itype==-70 || itype==-80) && pass_all && Kminus_ProbNNp>%4.2f'%probNNp_cut, "goff" )
  tree.Draw( 'B_s0_M_pPimKmPip>>h_sig_p_c%d'%i, '(itype==-70 || itype==-80) && pass_all && Kplus_ProbNNp>%4.2f'%probNNp_cut, "goff" )

  h_data_at_cut_pb.GetXaxis().SetTitleSize(0.07)
  h_data_at_cut_pb.GetXaxis().SetTitleOffset(0.8)
  h_data_at_cut_p.GetXaxis().SetTitleSize(0.07)
  h_data_at_cut_p.GetXaxis().SetTitleOffset(0.8)
  h_sig_at_cut_pb.GetXaxis().SetTitleSize(0.07)
  h_sig_at_cut_pb.GetXaxis().SetTitleOffset(0.8)
  h_sig_at_cut_p.GetXaxis().SetTitleSize(0.07)
  h_sig_at_cut_p.GetXaxis().SetTitleOffset(0.8)

  leg1.AddEntry( h_sig_at_cut_pb, 'K^{-} ProbNNp>%4.2f'%probNNp_cut, 'L')
  leg2.AddEntry( h_sig_at_cut_pb, 'K^{+} ProbNNp>%4.2f'%probNNp_cut, 'L')

  h_data_at_cut_pb.SetLineColor( i+1 )
  c2.cd(1)
  if i==0: h_data_at_cut_pb.Draw("HIST")
  else: h_data_at_cut_pb.Draw("HISTsame")

  h_sig_at_cut_pb.SetLineColor( i+1 )
  c2.cd(2)
  if i==0: h_sig_at_cut_pb.Draw("HIST")
  else: h_sig_at_cut_pb.Draw("HISTsame")

  h_data_at_cut_p.SetLineColor( i+1 )
  c2.cd(3)
  if i==0: h_data_at_cut_p.Draw("HIST")
  else: h_data_at_cut_p.Draw("HISTsame")

  h_sig_at_cut_p.SetLineColor( i+1 )
  c2.cd(4)
  if i==0: h_sig_at_cut_p.Draw("HIST")
  else: h_sig_at_cut_p.Draw("HISTsame")

  cut_hists.append( h_data_at_cut_pb )
  cut_hists.append( h_data_at_cut_p )
  cut_hists.append( h_sig_at_cut_pb )
  cut_hists.append( h_sig_at_cut_p )

c2.cd(1)
leg1.Draw()
c2.cd(2)
leg1.Draw()
c2.cd(3)
leg2.Draw()
c2.cd(4)
leg2.Draw()

c2.Update()
c2.Modified()
c2.Print("plots/MassVetoes/pdf/MassAfterVetoes.pdf")
c2.Print("plots/MassVetoes/png/MassAfterVetoes.png")
c2.Print("plots/MassVetoes/eps/MassAfterVetoes.eps")
c2.Print("plots/MassVetoes/C/MassAfterVetoes.C")
c2.SaveAs("plots/MassVetoes/root/MassAfterVetoes.root")

c3 = r.TCanvas("c3","c3")

h_data_bef = r.TH1F("hbef","",100,5000,5800)
h_data_aft = r.TH1F("haft","",100,5000,5800)
h_data_rem = r.TH1F("hrem","",100,5000,5800)

mass_veto_string = '('
mass_veto_string +=   ' (B_s0_M_KpPimPimPip>=%d && B_s0_M_KpPimPimPip<=%d)'%(vetoes[0][0],vetoes[0][1])
mass_veto_string += '&& (B_s0_M_PipPimKmPip>=%d && B_s0_M_PipPimKmPip<=%d)'%(vetoes[3][0],vetoes[3][1])
mass_veto_string += '&& (B_s0_M_KpPimpbPip>=%d && B_s0_M_KpPimpbPip<=%d)'%(vetoes[1][0],vetoes[1][1])
mass_veto_string += '&& (B_s0_M_pPimKmPip>=%d && B_s0_M_pPimKmPip<=%d)'%(vetoes[4][0],vetoes[4][1])
mass_veto_string += '&& (B_s0_M_KpKmKmPip>=%d && B_s0_M_KpKmKmPip<=%d)'%(vetoes[2][0],vetoes[2][1])
mass_veto_string += '&& (B_s0_M_KpPimKmKp>=%d && B_s0_M_KpPimKmKp<=%d)'%(vetoes[5][0],vetoes[5][1])
mass_veto_string += ' )'

tree.Draw( "B_s0_DTF_B_s0_M>>hbef", "(itype>0) && pass_all", "goff")
tree.Draw(" B_s0_DTF_B_s0_M>>haft", "(itype>0) && pass_all && (Kplus_ProbNNp<0.4) && (Kminus_ProbNNp<0.4) && %s"%mass_veto_string,"goff");
tree.Draw(" B_s0_DTF_B_s0_M>>hrem", "(itype>0) && pass_all && !%s"%mass_veto_string,"goff");

h_data_bef.GetXaxis().SetTitle("m(K^{+}#pi^{-}K^{-}#pi^{+})")
h_data_bef.GetXaxis().SetTitleSize(0.08)
h_data_bef.GetXaxis().SetTitleOffset(0.8)
h_data_bef.SetLineWidth(2)
h_data_bef.Draw("HIST")
h_data_aft.SetLineColor(2)
h_data_aft.SetFillColor(2)
h_data_aft.SetFillStyle(3004)
h_data_aft.Draw("HISTFsame")
h_data_rem.SetLineColor(r.kBlue)
h_data_rem.SetFillColor(r.kBlue)
h_data_rem.SetFillStyle(3003)
h_data_rem.Draw("HISTFsame")
h_data_bef.Draw("HISTsame")

leg3 = r.TLegend(0.55,0.7,0.93,0.9)
leg3.AddEntry(h_data_bef, "Data Before Mass Vetoes", "L")
leg3.AddEntry(h_data_aft, "Data After Mass Vetoes", "F")
leg3.AddEntry(h_data_rem, "Data Removed By Mass Vetoes", "F")
leg3.Draw()

c3.Update()
c3.Modified()
c3.Print("plots/MassVetoes/pdf/MassAfterVetoes.pdf")
c3.Print("plots/MassVetoes/png/MassAfterVetoes.png")
c3.Print("plots/MassVetoes/eps/MassAfterVetoes.eps")
c3.Print("plots/MassVetoes/C/MassAfterVetoes.C")
c3.SaveAs("plots/MassVetoes/root/MassAfterVetoes.root")


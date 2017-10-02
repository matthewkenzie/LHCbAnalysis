import ROOT as r
import sys

all_four = True
if len(sys.argv)>1:
  all_four = False


f = r.TFile("root/AnalysisOutWSWeights.root")
t = f.Get("AnalysisTree")

c = r.TCanvas("c","c",1600,1200)
c.Divide(2,2)

h11 = r.TH1F("h11","h11; m(p#piK#pi)",100,5000,6500)
h12 = r.TH1F("h12","h12; m(p#piK#pi)",100,5000,6500)
h13 = r.TH1F("h13","h13; m(p#piK#pi)",100,5000,6500)
h14 = r.TH1F("h14","h14; m(p#piK#pi)",100,5000,6500)
h21 = r.TH1F("h21","h21; m(K#pip#pi)",100,5000,6500)
h22 = r.TH1F("h22","h22; m(K#pip#pi)",100,5000,6500)
h23 = r.TH1F("h23","h23; m(K#pip#pi)",100,5000,6500)
h24 = r.TH1F("h24","h24; m(K#pip#pi)",100,5000,6500)

h31 = r.TH1F("h31","h31; #Phi",50,-3.14,3.14)
h32 = r.TH1F("h32","h32; #Phi",50,-3.14,3.14)
h33 = r.TH1F("h33","h33; #Phi",50,-3.14,3.14)
h34 = r.TH1F("h34","h34; #Phi",50,-3.14,3.14)

t.Draw("B_s0_M_pPimKmPip>>h11","(itype>0) && pass_bdt && pass_pid","goff")
t.Draw("B_s0_M_pPimKmPip>>h12","(itype>0) && pass_all","goff")
t.Draw("B_s0_M_pPimKmPip>>h13","(itype>0) && (B_s0_DTF_B_s0_M>5450 && B_s0_DTF_B_s0_M<5550) && pass_bdt && pass_pid","goff")
t.Draw("B_s0_M_pPimKmPip>>h14","(itype>0) && (B_s0_DTF_B_s0_M>5450 && B_s0_DTF_B_s0_M<5550) && pass_all","goff")
t.Draw("B_s0_M_KpPimpbPip>>h21","(itype>0) && pass_bdt && pass_pid","goff")
t.Draw("B_s0_M_KpPimpbPip>>h22","(itype>0) && pass_all","goff")
t.Draw("B_s0_M_KpPimpbPip>>h23","(itype>0) && (B_s0_DTF_B_s0_M>5450 && B_s0_DTF_B_s0_M<5550) && pass_bdt && pass_pid","goff")
t.Draw("B_s0_M_KpPimpbPip>>h24","(itype>0) && (B_s0_DTF_B_s0_M>5450 && B_s0_DTF_B_s0_M<5550) && pass_all","goff")

t.Draw("B_s0_DTF_B_s0_Phi1>>h31","(itype>0) && pass_bdt && pass_pid","goff")
t.Draw("B_s0_DTF_B_s0_Phi1>>h32","(itype>0) && pass_all", "goff")
t.Draw("B_s0_DTF_B_s0_Phi1>>h33","(itype>0) && (B_s0_DTF_B_s0_M>5450 && B_s0_DTF_B_s0_M<5550) && pass_bdt && pass_pid","goff")
t.Draw("B_s0_DTF_B_s0_Phi1>>h34","(itype>0) && (B_s0_DTF_B_s0_M>5450 && B_s0_DTF_B_s0_M<5550) && pass_all","goff")

c.cd(1)
h11.SetLineColor(r.kBlue)
h12.SetLineColor(r.kRed)
h13.SetLineColor(r.kGreen+2)
h14.SetLineColor(r.kBlack)
if all_four:
  h11.Draw("HIST")
  h12.Draw("HISTsame")
  h13.Draw("HISTsame")
  h14.Draw("HISTsame")
else:
  h13.Draw("HIST")
  h14.Draw("HISTsame")

c.cd(3)
h21.SetLineColor(r.kBlue)
h22.SetLineColor(r.kRed)
h23.SetLineColor(r.kGreen+2)
h24.SetLineColor(r.kBlack)
if all_four:
  h21.Draw("HIST")
  h22.Draw("HISTsame")
  h23.Draw("HISTsame")
  h24.Draw("HISTsame")
else:
  h23.Draw("HIST")
  h24.Draw("HISTsame")

c.cd(2)
h31.SetLineColor(r.kBlue)
h32.SetLineColor(r.kRed)
h33.SetLineColor(r.kGreen+2)
h34.SetLineColor(r.kBlack)
if all_four:
  h31.GetYaxis().SetRangeUser(0,h31.GetMaximum()*1.1)
  h31.Draw("HIST")
  h32.Draw("HISTsame")
  h33.Draw("HISTsame")
  h34.Draw("HISTsame")
else:
  h33.GetYaxis().SetRangeUser(0,h33.GetMaximum()*1.1)
  h33.Draw("HIST")
  h34.Draw("HISTsame")

leg = r.TLegend(0.6,0.3,0.88,0.7)
leg.AddEntry(h21,"Pass BDT+PID")
leg.AddEntry(h22,"Pass All")
leg.AddEntry(h23,"Pass BDT+PID (sideband)")
leg.AddEntry(h24,"Pass All (sideband)")
c.cd(1)
leg.Draw()

c.Update()
c.Modified()

if all_four: c.Print("plots/greig_mass.pdf")
else: c.Print("plots/greig_mass2.pdf")

raw_input()

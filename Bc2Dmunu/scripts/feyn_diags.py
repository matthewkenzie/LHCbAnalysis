#!/usr/bin/env python

import os
os.system('mkdir -p plots/FeynmanDiags/pdf')
os.system('mkdir -p plots/FeynmanDiags/png')
os.system('mkdir -p plots/FeynmanDiags/C')


import ROOT as r

def Bc2D0munu():

  canv = r.TCanvas("c","",10,10,600,400)
  canv.Range(0, 0, 140, 70);

  solid_line = r.TLine()
  solid_line.SetLineWidth(2)

  wavy_line = r.TCurlyLine( 70, 30, 95, 50)
  wavy_line.SetLineWidth(2)
  wavy_line.SetWavy()

  spiral_line = r.TCurlyLine()
  spiral_line.SetLineWidth(2)

  solid_line.DrawLine( 20, 30, 120, 30 );
  solid_line.DrawLine( 20, 10, 120, 10 );
  solid_line.DrawLine( 95, 50, 120, 40 );
  solid_line.DrawLine( 95, 50, 120, 60 );

  wavy_line.Draw();

  lat = r.TLatex()
  lat.SetTextFont(43)
  lat.SetTextSize(25.)
  lat.SetTextAlign(22)
  lat.DrawLatex( 16, 30, "#bar{#it{b}}")
  lat.DrawLatex( 16, 10, "#it{c}" )
  lat.DrawLatex( 124, 60, "#mu^{+}" )
  lat.DrawLatex( 124, 40, "#nu" )
  lat.DrawLatex( 124, 30, "#bar{#it{u}}")
  lat.DrawLatex( 124, 10, "#it{c}" )

  ckmV = r.TLatex()
  ckmV.SetTextFont(63)
  ckmV.SetTextSize(25.)
  ckmV.SetTextAlign(22)
  ckmV.DrawLatex( 66, 34, "V_{ub}" )

  biglat = r.TLatex()
  biglat.SetTextFont(43)
  biglat.SetTextSize(30.)
  biglat.SetTextAlign(22)
  biglat.DrawLatex(10,20, "#it{B}_{#it{c}}^{+}")
  biglat.DrawLatex(130,20, "#it{D}^{0}")

  canv.Update()
  canv.Modified()
  canv.Print('plots/FeynmanDiags/pdf/bc2dmunu.pdf')
  canv.Print('plots/FeynmanDiags/png/bc2dmunu.png')
  canv.Print('plots/FeynmanDiags/C/bc2dmunu.C')

  return canv

def Bc2D0pizmunu():

  canv = r.TCanvas("c","",10,10,600,400)
  canv.Range(0, 0, 140, 90);

  solid_line = r.TLine()
  solid_line.SetLineWidth(2)

  wavy_line = r.TCurlyLine( 70, 50, 95, 70)
  wavy_line.SetLineWidth(2)
  wavy_line.SetWavy()

  spiral_line = r.TCurlyLine( 70, 10, 95, 30)
  spiral_line.SetLineWidth(2)

  solid_line.DrawLine( 20, 50, 120, 50 );
  solid_line.DrawLine( 20, 10, 120, 10 );
  solid_line.DrawLine( 95, 70, 120, 60 );
  solid_line.DrawLine( 95, 70, 120, 80 );
  solid_line.DrawLine( 95, 30, 120, 20 );
  solid_line.DrawLine( 95, 30, 120, 40 );

  wavy_line.Draw();
  spiral_line.Draw();


  lat = r.TLatex()
  lat.SetTextFont(43)
  lat.SetTextSize(25.)
  lat.SetTextAlign(22)
  lat.DrawLatex( 16, 50, "#bar{#it{b}}")
  lat.DrawLatex( 16, 10, "#it{c}" )
  lat.DrawLatex( 124, 80, "#mu^{+}" )
  lat.DrawLatex( 124, 60, "#nu" )
  lat.DrawLatex( 124, 50, "#bar{#it{u}}")
  lat.DrawLatex( 124, 40, "#it{u}")
  lat.DrawLatex( 124, 20, "#bar{#it{u}}")
  lat.DrawLatex( 124, 10, "#it{c}" )

  ckmV = r.TLatex()
  ckmV.SetTextFont(63)
  ckmV.SetTextSize(25.)
  ckmV.SetTextAlign(22)
  ckmV.DrawLatex( 66, 54, "V_{ub}" )

  biglat = r.TLatex()
  biglat.SetTextFont(43)
  biglat.SetTextSize(30.)
  biglat.SetTextAlign(22)
  biglat.DrawLatex(10,30, "#it{B}_{#it{c}}^{+}")
  biglat.DrawLatex(130,15, "#it{D}^{0}")
  biglat.DrawLatex(130,45, "#pi^{0}")

  canv.Update()
  canv.Modified()
  canv.Print('plots/FeynmanDiags/pdf/bc2dpizmunu.pdf')
  canv.Print('plots/FeynmanDiags/png/bc2dpizmunu.png')
  canv.Print('plots/FeynmanDiags/C/bc2dpizmunu.C')

  return canv

def Bc2Dppimmunu():

  canv = r.TCanvas("c","",10,10,600,400)
  canv.Range(0, 0, 140, 90);

  solid_line = r.TLine()
  solid_line.SetLineWidth(2)

  wavy_line = r.TCurlyLine( 70, 50, 95, 70)
  wavy_line.SetLineWidth(2)
  wavy_line.SetWavy()

  spiral_line = r.TCurlyLine( 70, 10, 95, 30)
  spiral_line.SetLineWidth(2)

  solid_line.DrawLine( 20, 50, 120, 50 );
  solid_line.DrawLine( 20, 10, 120, 10 );
  solid_line.DrawLine( 95, 70, 120, 60 );
  solid_line.DrawLine( 95, 70, 120, 80 );
  solid_line.DrawLine( 95, 30, 120, 20 );
  solid_line.DrawLine( 95, 30, 120, 40 );

  wavy_line.Draw();
  spiral_line.Draw();


  lat = r.TLatex()
  lat.SetTextFont(43)
  lat.SetTextSize(25.)
  lat.SetTextAlign(22)
  lat.DrawLatex( 16, 50, "#bar{#it{b}}")
  lat.DrawLatex( 16, 10, "#it{c}" )
  lat.DrawLatex( 124, 80, "#mu^{+}" )
  lat.DrawLatex( 124, 60, "#nu" )
  lat.DrawLatex( 124, 50, "#bar{#it{u}}")
  lat.DrawLatex( 124, 40, "#it{d}")
  lat.DrawLatex( 124, 20, "#bar{#it{d}}")
  lat.DrawLatex( 124, 10, "#it{c}" )

  ckmV = r.TLatex()
  ckmV.SetTextFont(63)
  ckmV.SetTextSize(25.)
  ckmV.SetTextAlign(22)
  ckmV.DrawLatex( 66, 54, "V_{ub}" )

  biglat = r.TLatex()
  biglat.SetTextFont(43)
  biglat.SetTextSize(30.)
  biglat.SetTextAlign(22)
  biglat.DrawLatex(10,30, "#it{B}_{#it{c}}^{+}")
  biglat.DrawLatex(130,15, "#it{D}^{+}")
  biglat.DrawLatex(130,45, "#pi^{-}")

  canv.Update()
  canv.Modified()
  canv.Print('plots/FeynmanDiags/pdf/bc2dppimmunu.pdf')
  canv.Print('plots/FeynmanDiags/png/bc2dppimmunu.png')
  canv.Print('plots/FeynmanDiags/C/bc2dppimmunu.C')

  return canv

def Bc2Dsppimmunu():

  canv = r.TCanvas("c","",10,10,600,400)
  canv.Range(0, 0, 140, 90);

  solid_line = r.TLine()
  solid_line.SetLineWidth(2)

  wavy_line = r.TCurlyLine( 70, 50, 95, 70)
  wavy_line.SetLineWidth(2)
  wavy_line.SetWavy()

  spiral_line = r.TCurlyLine( 70, 10, 95, 30)
  spiral_line.SetLineWidth(2)

  solid_line.DrawLine( 20, 50, 120, 50 );
  solid_line.DrawLine( 20, 10, 120, 10 );
  solid_line.DrawLine( 95, 70, 120, 60 );
  solid_line.DrawLine( 95, 70, 120, 80 );
  solid_line.DrawLine( 95, 30, 120, 20 );
  solid_line.DrawLine( 95, 30, 120, 40 );

  wavy_line.Draw();
  spiral_line.Draw();


  lat = r.TLatex()
  lat.SetTextFont(43)
  lat.SetTextSize(25.)
  lat.SetTextAlign(22)
  lat.DrawLatex( 16, 50, "#bar{#it{b}}")
  lat.DrawLatex( 16, 10, "#it{c}" )
  lat.DrawLatex( 124, 80, "#mu^{+}" )
  lat.DrawLatex( 124, 60, "#nu" )
  lat.DrawLatex( 124, 50, "#bar{#it{u}}")
  lat.DrawLatex( 124, 40, "#it{s}")
  lat.DrawLatex( 124, 20, "#bar{#it{s}}")
  lat.DrawLatex( 124, 10, "#it{c}" )

  ckmV = r.TLatex()
  ckmV.SetTextFont(63)
  ckmV.SetTextSize(25.)
  ckmV.SetTextAlign(22)
  ckmV.DrawLatex( 66, 54, "V_{ub}" )

  biglat = r.TLatex()
  biglat.SetTextFont(43)
  biglat.SetTextSize(30.)
  biglat.SetTextAlign(22)
  biglat.DrawLatex(10,30, "#it{B}_{#it{c}}^{+}")
  biglat.DrawLatex(130,15, "#it{D}_{#it{s}}^{+}")
  biglat.DrawLatex(130,45, "K^{-}")

  canv.Update()
  canv.Modified()
  canv.Print('plots/FeynmanDiags/pdf/bc2dspimmunu.pdf')
  canv.Print('plots/FeynmanDiags/png/bc2dspimmunu.png')
  canv.Print('plots/FeynmanDiags/C/bc2dspimmunu.C')

  return canv

def Bu2D0bmunu():

  canv = r.TCanvas("c","",10,10,600,400)
  canv.Range(0, 0, 140, 70);

  solid_line = r.TLine()
  solid_line.SetLineWidth(2)

  wavy_line = r.TCurlyLine( 70, 30, 95, 50)
  wavy_line.SetLineWidth(2)
  wavy_line.SetWavy()

  spiral_line = r.TCurlyLine()
  spiral_line.SetLineWidth(2)

  solid_line.DrawLine( 20, 30, 120, 30 );
  solid_line.DrawLine( 20, 10, 120, 10 );
  solid_line.DrawLine( 95, 50, 120, 40 );
  solid_line.DrawLine( 95, 50, 120, 60 );

  wavy_line.Draw();

  lat = r.TLatex()
  lat.SetTextFont(43)
  lat.SetTextSize(25.)
  lat.SetTextAlign(22)
  lat.DrawLatex( 16, 30, "#bar{#it{b}}")
  lat.DrawLatex( 16, 10, "#it{u}" )
  lat.DrawLatex( 124, 60, "#mu^{+}" )
  lat.DrawLatex( 124, 40, "#nu" )
  lat.DrawLatex( 124, 30, "#bar{#it{c}}")
  lat.DrawLatex( 124, 10, "#it{u}" )

  ckmV = r.TLatex()
  ckmV.SetTextFont(63)
  ckmV.SetTextSize(25.)
  ckmV.SetTextAlign(22)
  ckmV.DrawLatex( 66, 34, "V_{cb}" )

  biglat = r.TLatex()
  biglat.SetTextFont(43)
  biglat.SetTextSize(30.)
  biglat.SetTextAlign(22)
  biglat.DrawLatex(10,20, "#it{B}^{+}")
  biglat.DrawLatex(130,20, "#bar{#it{D}}^{0}")

  canv.Update()
  canv.Modified()
  canv.Print('plots/FeynmanDiags/pdf/bu2dbmunu.pdf')
  canv.Print('plots/FeynmanDiags/png/bu2dbmunu.png')
  canv.Print('plots/FeynmanDiags/C/bu2dbmunu.C')

  return canv

def Bd2Dmmunu():

  canv = r.TCanvas("c","",10,10,600,400)
  canv.Range(0, 0, 140, 70);

  solid_line = r.TLine()
  solid_line.SetLineWidth(2)

  wavy_line = r.TCurlyLine( 70, 30, 95, 50)
  wavy_line.SetLineWidth(2)
  wavy_line.SetWavy()

  spiral_line = r.TCurlyLine()
  spiral_line.SetLineWidth(2)

  solid_line.DrawLine( 20, 30, 120, 30 );
  solid_line.DrawLine( 20, 10, 120, 10 );
  solid_line.DrawLine( 95, 50, 120, 40 );
  solid_line.DrawLine( 95, 50, 120, 60 );

  wavy_line.Draw();

  lat = r.TLatex()
  lat.SetTextFont(43)
  lat.SetTextSize(25.)
  lat.SetTextAlign(22)
  lat.DrawLatex( 16, 30, "#bar{#it{b}}")
  lat.DrawLatex( 16, 10, "#it{d}" )
  lat.DrawLatex( 124, 60, "#mu^{+}" )
  lat.DrawLatex( 124, 40, "#nu" )
  lat.DrawLatex( 124, 30, "#bar{#it{c}}")
  lat.DrawLatex( 124, 10, "#it{d}" )

  ckmV = r.TLatex()
  ckmV.SetTextFont(63)
  ckmV.SetTextSize(25.)
  ckmV.SetTextAlign(22)
  ckmV.DrawLatex( 66, 34, "V_{cb}" )

  biglat = r.TLatex()
  biglat.SetTextFont(43)
  biglat.SetTextSize(30.)
  biglat.SetTextAlign(22)
  biglat.DrawLatex(10,20, "#it{B}^{0}")
  biglat.DrawLatex(130,20, "#it{D}^{-}")

  canv.Update()
  canv.Modified()
  canv.Print('plots/FeynmanDiags/pdf/bd2dmmunu.pdf')
  canv.Print('plots/FeynmanDiags/png/bd2dmmunu.png')
  canv.Print('plots/FeynmanDiags/C/bd2dmmunu.C')

  return canv

def Bdb2Dmmunu():

  canv = r.TCanvas("c","",10,10,600,400)
  canv.Range(0, 0, 140, 70);

  solid_line = r.TLine()
  solid_line.SetLineWidth(2)

  wavy_line = r.TCurlyLine( 70, 30, 95, 50)
  wavy_line.SetLineWidth(2)
  wavy_line.SetWavy()

  spiral_line = r.TCurlyLine()
  spiral_line.SetLineWidth(2)

  solid_line.DrawLine( 20, 30, 120, 30 );
  solid_line.DrawLine( 20, 10, 120, 10 );
  solid_line.DrawLine( 95, 50, 120, 40 );
  solid_line.DrawLine( 95, 50, 120, 60 );

  wavy_line.Draw();

  lat = r.TLatex()
  lat.SetTextFont(43)
  lat.SetTextSize(25.)
  lat.SetTextAlign(22)
  lat.DrawLatex( 16, 30, "#it{b}")
  lat.DrawLatex( 16, 10, "#bar{#it{d}}" )
  lat.DrawLatex( 124, 60, "#mu^{-}" )
  lat.DrawLatex( 124, 40, "#nu" )
  lat.DrawLatex( 124, 30, "#it{c}")
  lat.DrawLatex( 124, 10, "#bar{#it{d}}" )

  ckmV = r.TLatex()
  ckmV.SetTextFont(63)
  ckmV.SetTextSize(25.)
  ckmV.SetTextAlign(22)
  ckmV.DrawLatex( 66, 34, "V_{cb}" )

  biglat = r.TLatex()
  biglat.SetTextFont(43)
  biglat.SetTextSize(30.)
  biglat.SetTextAlign(22)
  biglat.DrawLatex(10,20, "#bar{#it{B}}^{0}")
  biglat.DrawLatex(130,20, "#it{D}^{+}")

  canv.Update()
  canv.Modified()
  canv.Print('plots/FeynmanDiags/pdf/bdb2dmmunu.pdf')
  canv.Print('plots/FeynmanDiags/png/bdb2dmmunu.png')
  canv.Print('plots/FeynmanDiags/C/bdb2dmmunu.C')

  return canv

def Bc2Jpsimunu():

  canv = r.TCanvas("c","",10,10,600,400)
  canv.Range(0, 0, 140, 70);

  solid_line = r.TLine()
  solid_line.SetLineWidth(2)

  wavy_line = r.TCurlyLine( 70, 30, 95, 50)
  wavy_line.SetLineWidth(2)
  wavy_line.SetWavy()

  spiral_line = r.TCurlyLine()
  spiral_line.SetLineWidth(2)

  solid_line.DrawLine( 20, 30, 120, 30 );
  solid_line.DrawLine( 20, 10, 120, 10 );
  solid_line.DrawLine( 95, 50, 120, 40 );
  solid_line.DrawLine( 95, 50, 120, 60 );

  wavy_line.Draw();

  lat = r.TLatex()
  lat.SetTextFont(43)
  lat.SetTextSize(25.)
  lat.SetTextAlign(22)
  lat.DrawLatex( 16, 30, "#bar{#it{b}}")
  lat.DrawLatex( 16, 10, "#it{c}" )
  lat.DrawLatex( 124, 60, "#mu^{+}" )
  lat.DrawLatex( 124, 40, "#nu" )
  lat.DrawLatex( 124, 30, "#bar{#it{c}}")
  lat.DrawLatex( 124, 10, "#it{c}" )

  ckmV = r.TLatex()
  ckmV.SetTextFont(63)
  ckmV.SetTextSize(25.)
  ckmV.SetTextAlign(22)
  ckmV.DrawLatex( 66, 34, "V_{cb}" )

  biglat = r.TLatex()
  biglat.SetTextFont(43)
  biglat.SetTextSize(30.)
  biglat.SetTextAlign(22)
  biglat.DrawLatex(10,20, "#it{B}_{#it{c}}^{+}")
  biglat.DrawLatex(130,20, "J/#psi")

  canv.Update()
  canv.Modified()
  canv.Print('plots/FeynmanDiags/pdf/bc2jpsimunu.pdf')
  canv.Print('plots/FeynmanDiags/png/bc2jpsimunu.png')
  canv.Print('plots/FeynmanDiags/C/bc2jpsimunu.C')

  return canv

Bc2D0munu()
Bc2D0pizmunu()
Bc2Dppimmunu()
Bc2Dsppimmunu()
Bu2D0bmunu()
Bd2Dmmunu()
Bdb2Dmmunu()
Bc2Jpsimunu()

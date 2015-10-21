#!/usr/bin/env python

import os
import sys
import ROOT as r

os.system('mkdir -p plots/Efficiencies/pdf')
os.system('mkdir -p plots/Efficiencies/png')
os.system('mkdir -p plots/Efficiencies/C')
r.gROOT.ProcessLine(".x ~/bin/lhcbStyle.C")
r.gStyle.SetGridWidth(3)
r.gStyle.SetGridStyle(1)
r.gStyle.SetPalette(1)

infiles = sys.argv[1:]
canvs=[]

xlabels = []
ylabels = []

def fillDict(hName, isEff=False, skipStart=True):
  theDict = {}
  for f in infiles:
    tf = r.TFile.Open(f)
    hist = tf.Get(hName)
    for xbin in range(1,hist.GetNbinsX()+1):
      analyser_label = hist.GetXaxis().GetBinLabel(xbin)
      if skipStart and analyser_label=='Start': continue
      if 'BDTReader' in analyser_label: continue
      if analyser_label not in theDict.keys():
        theDict[analyser_label] = {}
        if analyser_label not in xlabels: xlabels.append(analyser_label)
      for ybin in range(1,hist.GetNbinsY()+1):
        dataset_label = hist.GetYaxis().GetBinLabel(ybin)
        if dataset_label not in theDict[analyser_label].keys():
          bin2d = hist.FindBin(xbin-1,ybin-1)
          theDict[analyser_label][dataset_label] = hist.GetBinContent(bin2d)
          if dataset_label not in ylabels: ylabels.append(dataset_label)
        elif analyser_label == 'Start':
          bin2d = hist.FindBin(xbin-1,ybin-1)
          new_val = hist.GetBinContent(bin2d)
          old_val = theDict[analyser_label][dataset_label]
          theDict[analyser_label][dataset_label] = max(new_val,old_val)
        else:
          print 'WARNING - clash - entry already exists for [', analyser_label, ',', dataset_label, ']'
    tf.Close()

  #theDict['Total'] = {}
  #if 'Total' in xlabels: xlabels.remove('Total')
  #for ylabel in ylabels:
    #totalval = 0
    #if isEff: totalval = 1.
    #for xlabel in xlabels:
      #val = theDict[xlabel][ylabel]
      #if isEff:
        #totalval *= val
      #else:
        #totalval = val
    #theDict['Total'][ylabel] = totalval
  #xlabels.append('Total')
  return theDict

def convertDictToTH2(theDict, name, title=""):

  global ylabels
  xbins = len(theDict.keys())
  ybins = len(theDict[theDict.keys()[0]])

  for xlabel, yDict in theDict.items():
    assert( len(yDict.keys()) == ybins )

  assert( len(xlabels)==xbins )
  assert( len(ylabels)==ybins )

  ylabels.sort()
  ylabels_mc   = [ x for x in ylabels if 'Data' not in x ]
  ylabels_data = [ x for x in ylabels if 'Data' in x ]
  ylabels = sorted(ylabels_mc) + sorted(ylabels_data)
  ylabels.reverse()

  th2f = r.TH2F(name,name,xbins,0,xbins,ybins,0,ybins)
  th2f.SetTitle( title )

  for xbin, xlabel in enumerate(xlabels):
    yDict = theDict[xlabel]
    th2f.GetXaxis().SetBinLabel(xbin+1,tidyLabelX(xlabel))
    for ybin, ylabel in enumerate(ylabels):
      th2f.GetYaxis().SetBinLabel(ybin+1,tidyLabel(ylabel))
      bin2d = th2f.FindBin(xbin,ybin)
      th2f.SetBinContent(bin2d, theDict[xlabel][ylabel])

  return th2f

def printDict(theDict):
  for xlabel, yDict in theDict.items():
    for ylabel, value in yDict.items():
      print '%20s  %20s'%(xlabel,ylabel), ' = ', value

def tidyLabel(label):
  new_label = label.replace('_',' ')
  new_label = new_label.replace('MagDown','MD')
  new_label = new_label.replace('MagUp','MU')
  new_label = new_label.replace('LowMass','(LM)')
  new_label = new_label.replace('HighMass','(HM)')
  return new_label

def tidyLabelX(label):
  new_label = label
  if 'ApplyBDTAndPIDCuts' in label:  new_label = 'BDT + PID Cut'
  if 'BDTAndPIDCut' in label:        new_label = 'BDT + PID Cut'
  if 'BDTReader' in label:           new_label = 'BDT Cut'
  if 'PreSelection' in label:        new_label = 'Pre-selection'
  if 'TruthMatching' in label:       new_label = 'Truth Matching'
  if 'PIDSelection' in label:        new_label = 'PID Cut'
  return new_label

def draw(th2f, name, textformat='', col=False):

  canvs.append(r.TCanvas(name,name,1200,700))
  canvs[-1].SetLeftMargin(0.3)
  if th2f.GetTitle() != "":
    canvs[-1].SetTopMargin(0.1)
  if col: canvs[-1].SetRightMargin(0.15)
  else: canvs[-1].SetRightMargin(0.05)
  canvs[-1].SetGridx()
  canvs[-1].SetGridy()
  canvs[-1].cd()
  r.gStyle.SetPaintTextFormat(textformat)
  th2f.SetMarkerSize(2.0)
  #if col: th2f.SetMarkerColor(r.kWhite)
  text = r.TPaveText(0.4,0.94,0.6,0.99,"ndc")
  text.SetFillColor(0)
  text.SetShadowColor(0)
  text.SetLineColor(0)
  text.AddText(th2f.GetTitle())
  text.SetTextAlign(22)
  text.SetTextSize(0.06)

  th2f.SetStats(0)
  th2f.GetZaxis().SetTitle("Efficiency (%)")
  th2f.GetZaxis().SetTitleOffset(0.7)
  th2f.GetZaxis().SetTitleSize(0.06)
  if col: th2f.Draw("TEXTcolz")
  else: th2f.Draw("TEXT")

  if th2f.GetTitle() != "": text.Draw("same")
  ## highlight total
  #box = r.TBox()
  #box.SetLineWidth(6)
  #box.SetLineColor(r.kRed)
  #box.SetFillColor(0)
  #box.SetFillStyle(0)
  #box.DrawBox(th2f.GetNbinsX()-1,0.,th2f.GetNbinsX(),th2f.GetNbinsY())
  #line = r.TLine()
  #line.SetLineWidth(6)
  #line.SetLineColor(r.kWhite)
  #line.DrawLine(th2f.GetNbinsX()-1,0.,th2f.GetNbinsX()-1,th2f.GetNbinsY())
  canvs[-1].Update()
  canvs[-1].Modified()
  canvs[-1].Print("plots/Efficiencies/pdf/eff_%s.pdf"%name)
  canvs[-1].Print("plots/Efficiencies/png/eff_%s.png"%name)
  canvs[-1].Print("plots/Efficiencies/C/eff_%s.C"%name)

def makeEffDict( passDict, failDict ):
  effDict = {}
  for analyser, sampleDict in passDict.iteritems():
    print analyser
    effDict[analyser] = {}
    for sample, evs in sampleDict.iteritems():
      effDict[analyser][sample] = float( passDict[analyser][sample] ) / float( passDict[analyser][sample] + failDict[analyser][sample] )

    effDict['Total'] = {}
    for ylabel in ylabels:
      effDict['Total'][ylabel] = float( passDict[xlabels[-1]][ylabel] ) / float( passDict[xlabels[0]][ylabel] + failDict[xlabels[0]][ylabel] )

  return effDict

# __main__
passDict = fillDict('hPass')
failDict = fillDict('hFail')
procDict = fillDict('hProc')
effDict  = makeEffDict( passDict, failDict )
#effDict  = fillDict('hEff',True)

passHist = convertDictToTH2(passDict,'hPass', 'Passing events')
failHist = convertDictToTH2(failDict,'hFail', 'Failed events')
procDict = convertDictToTH2(procDict,'hProc', 'Processed events')
xlabels.append('Total')
effHist  = convertDictToTH2(effDict,'hEff', 'Selection efficiency')
effHist.Scale(100.)

draw(passHist,'pass','g')
draw(failHist,'fail','g')
draw(procDict,'proc','g')
draw(effHist, 'eff','5.1f%%',True)

raw_input('Done\n')

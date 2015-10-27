#!/usr/bin/env python

import ROOT as r
import sys

files = sys.argv[1:]

for f in files:

  tf = r.TFile(f)
  lumi = tf.Get('GetIntegratedLuminosity/LumiTuple')
  intL = 0.
  errL = 0.
  for ev in range(lumi.GetEntries()):
    lumi.GetEntry(ev)
    intL += lumi.IntegratedLuminosity
    errL += lumi.IntegratedLuminosityErr

  print '%40s  :   %6.2f  +/- %6.2f pb' %(f,intL,errL)

  tf.Close()


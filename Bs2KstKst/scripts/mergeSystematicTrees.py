import ROOT as r

fileNames = [ ]
njobs = 10
ntoys = 200

for i in range(njobs):
  fileNames.append( 'root/AnalysisOutTrimmedJob%d.root'%(i+1) )

files = [ r.TFile(name) for name in fileNames ]

trees = [ tf.Get('AnalysisTree') for tf in files ]

import os
os.system('cp root/AnalysisOutTrimmedSmall.root root/AnalysisOutTrimmed.root')
import array

tf = r.TFile( 'root/AnalysisOutTrimmed.root', 'UPDATE' )
tree = tf.Get('AnalysisTree')

branchHolder = {}
varHolder = {}

for i in range(njobs):
  for j in range(ntoys):
    newtoyn = (i*ntoys)+j
    branchName = 'random_yield_toy%d_sw'%newtoyn
    varHolder[branchName] = array.array('f',[0])
    branchHolder[ branchName ] = tree.Branch( branchName, varHolder[branchName], branchName+'/F' )

nentries = tree.GetEntries()
for i in range(nentries):
  tree.GetEntry(i)
  if (i%1000==0): print i, '/', nentries
  for j in range(njobs):
    trees[j].GetEntry(i)
    assert( tree.eventNumber == trees[j].eventNumber )
    for k in range(ntoys):
      oldBranchName = 'random_yield_toy%d_sw'%k
      newBranchName = 'random_yield_toy%d_sw'%((j*ntoys)+k)
      val = getattr(trees[j],'random_yield_toy%d_sw'%k)
      #print oldBranchName, newBranchName, val
      varHolder[newBranchName][0] = val
      branchHolder[newBranchName].Fill()

tree.Write()

tree.Scan('random_yield_toy0_sw','','',10,17)
trees[0].Scan('random_yield_toy0_sw','','',10,17)
tree.Scan('random_yield_toy876_sw','','',10,17)
trees[4].Scan('random_yield_toy76_sw','','',10,17)
tree.Scan('random_yield_toy1875_sw','','',10,17)
trees[9].Scan('random_yield_toy75_sw','','',10,17)
tf.Close()

#!/usr/bin/env python

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-f","--filename")
parser.add_option("-t","--treename")
parser.add_option("-o","--outputloc",default="tmp")
(opts,args) = parser.parse_args()

import ROOT as r
import os
import sys

tf = r.TFile(opts.filename)
if not tf:
  sys.exit('Not a valid filename: '+opts.filename)

tree = tf.Get(opts.treename)
if not tree:
  sys.exit('No such tree '+opts.tree+' in file')

os.system('mkdir -p %s'%opts.outputloc)

headerLines       = []
sourceInputLines  = []
sourceOutputLines = []

for branch in tree.GetListOfBranches():

  branch_name = branch.GetName()
  name = branch.GetName().replace('.','_')
  dec_name = name
  title = branch.GetTitle().replace('.','_')
  type_name = branch.GetLeaf(branch.GetName()).GetTypeName()

  if '[' in title and ']' in title: # must be array
    for el in title.split('[')[1:]:
      arrcount = el.split(']')[0]
      try:
        arr_int_val = int(arrcount)
        dec_name += '[%d]'%arr_int_val
      except:
        dec_name += '[100]'

  headerLines.append('      %-20s %-40s;\n'%(type_name,dec_name))
  if '[' in dec_name and ']' in dec_name:
    sourceInputLines.append('  tree->SetBranchAddress(%-40s,  %-40s);\n'%('\"'+branch_name+'\"',name))
  else:
    sourceInputLines.append('  tree->SetBranchAddress(%-40s, &%-40s);\n'%('\"'+branch_name+'\"',name))

  sourceOutputLines.append('  tree->Branch(%-40s, &%-40s, %-40s);\n'%('\"'+name+'\"', name, '\"'+title+'\"'))  

f = open('%s/branchdump.h'%opts.outputloc,'w')
f.write('// ---- HEADER ---- //\n')
for line in headerLines:
  f.write(line)
f.write('// ---- SRC input --- //\n')
for line in sourceInputLines:
  f.write(line)
f.write('// ---- SRC output --- //\n')
for line in sourceOutputLines:
  f.write(line)

f.close()

print 'Written output to: \n\t', f.name

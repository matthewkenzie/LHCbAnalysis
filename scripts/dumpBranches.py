#!/usr/bin/env python

import ROOT as r

tf = r.TFile('nTuples/DataTuple.root')
tree = tf.Get('HltTuple/DecayTree')

f1 = open('tmp/headerstuff.h','w')
f2 = open('tmp/srcstuff.cc','w')
f3 = open('tmp/srcstuff2.cc','w')

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

  f1.write('%-20s %-40s;\n'%(type_name,dec_name))
  if '[' in dec_name and ']' in dec_name:
    f2.write('tree->SetBranchAddress(%-40s,  %-40s);\n'%('\"'+branch_name+'\"',name))
  else:
    f2.write('tree->SetBranchAddress(%-40s, &%-40s);\n'%('\"'+branch_name+'\"',name))

  f3.write('tree->Branch(%-40s, &%-40s, %-40s);\n'%('\"'+name+'\"', name, '\"'+title+'\"'))

f1.close()
f2.close()
f3.close()

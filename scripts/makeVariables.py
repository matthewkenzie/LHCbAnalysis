#!/usr/bin/env python

def printUsage():
	print '  -  This will make a new class which can be used to set which branches are read and written'
	print '     for your analysis.'
	print '  -  It will inherit from the BranchDef class'
	print '  -  All this will do is define the branches which should be read in and written out to file'
	print '  -  This saves time as you don\'t have to read every branch'

def readBranchesIn():

	f = open(opts.inputbranches)
	for line in f.readlines():
		if line.startswith('#'): continue
		if line=='' or line=='\n': continue
		els = line.split()
		if len(els)!=3: continue
		vtype = els[0]
		vname = els[1]
		itype = int(els[2])
		variableDict[vname] = [vtype, itype, -1] # -1 means read in

	f.close()

def readBranchesOut():

	f = open(opts.outputbranches)
	for line in f.readlines():
		if line.startswith('#'): continue
		if line=='' or line=='\n': continue
		els = line.split()
		if len(els)!=3: continue
		vtype = els[0]
		vname = els[1]
		itype = int(els[2])
		if vname in variableDict.keys():
			if vtype!=variableDict[vname][0]:
				sys.exit('ERROR -- Same branch ('+vname+') requested for in and out but with different types!')
			if itype!=variableDict[vname][1]:
				sys.exit('ERROR -- Same branch ('+vname+') requested for in and out but with different types!')
			variableDict[vname] = [vtype, itype, 0] # means read in AND write out
		else:
			variableDict[vname] = [vtype, itype, 1] # 1 means write out

	f.close()

def printHeaderStuff():

	varKeys = variableDict.keys()
	varKeys.sort()

	f = open('tmp/headerstuff.h','w')
	for key in varKeys:
		f.write('      %-20s   %-40s ;\n'%(variableDict[key][0],key))
	f.close()

def printSourceStuff():

	varKeys = variableDict.keys()
	varKeys.sort()

	f = open('tmp/srcstuff.cc','w')

	f.write('void %s::%s::setInputBranches(TTree *tree) {\n'%(opts.namespace,opts.classname))
	for key in varKeys:
		if variableDict[key][2]>0: continue # read only
		f.write('  tree->SetBranchAddress(%-40s, &%-40s);\n'%('\"'+key+'\"',key))
	f.write('}\n\n')

	f.write('void %s::%s::setOutputBranches(TTree *tree) {\n'%(opts.namespace,opts.classname))
	f.write('  tree->Branch(%-40s, &%-40s);\n'%('\"itype\"','itype'))
	f.write('  tree->Branch(%-40s, &%-40s);\n'%('\"sqrts\"','sqrts'))
	f.write('  tree->Branch(%-40s, &%-40s);\n'%('\"evname\"','evname'))
	f.write('  tree->Branch(%-40s, &%-40s);\n'%('\"year\"','year'))
	for key in varKeys:
		#if variableDict[key][2]<=0: continue # write only
		f.write('  tree->Branch(%-40s, &%-40s);\n'%('\"'+key+'\"',key))
	f.write('}\n\n')

	f.close()

def main():
	readBranchesIn()
	readBranchesOut()

	printHeaderStuff()
	printSourceStuff()

if __name__ == "__main__":

	from optparse import OptionParser

	parser = OptionParser(usage=printUsage())
	parser.add_option("-i","--inputbranches")
	parser.add_option("-o","--outputbranches")
	parser.add_option("-c","--classname")
	parser.add_option("-n","--namespace")
	(opts,args) = parser.parse_args()

	import os
	import sys

	os.system('mkdir -p tmp')

	variableDict = {}

	main()

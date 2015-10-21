import os
import fnmatch
import sys

from optparse import OptionParser
parser = OptionParser()
parser.add_option("-d","--dirname")
parser.add_option("--resubmit", help = "one of fail or nothing")
parser.add_option("--backend", default="Local")
(opts,args) = parser.parse_args()

path = opts.dirname 

done = []
fail = []
run = []
nothing = []

for root, dirs, files in os.walk(path):

  if root != path:
    continue

  for f in fnmatch.filter(files,'*.sh'):
    
    basename = os.path.join(path,f.split('.sh')[0])

    if os.path.exists( basename+".done" ):
      done.append( basename+".done" )
    elif os.path.exists( basename+".fail" ):
      fail.append( basename+".fail" )
    elif os.path.exists( basename+".run" ):
      run.append( basename+".run" )
    else:
      nothing.append( basename+".sh" )


print 'Finished jobs:'
for j in done:
  print '\t', j
print 'Failed jobs:'
for j in fail:
  print '\t', j
print 'Running jobs:'
for j in run:
  print '\t', j
print 'Nothing jobs:'
for j in nothing:
  print '\t' ,j

print '------------------------------'
print 'Finished : ', len(done)
print 'Failed   : ', len(fail)
print 'Running  : ', len(run)
print 'Nothing  : ', len(nothing)
print 'TOTAL    : ', len(done)+len(fail)+len(run)+len(nothing)
print '------------------------------'

#!/usr/bin/env python

import string, os, getopt, time, sys, datetime

def ts():
	rt = time.strftime("%Y/%m/%d %H:%M:%S", time.localtime( time.time() ))
	rt = rt + " = "

	return rt

try:
	optlist, args = getopt.getopt(sys.argv[1:], '', ['file=', ])
except getopt.GetoptError, e:
	print ts() + 'GetOutputFromLog: argument error'
	sys.stdout.flush()
	sys.exit(1)

file = ''
for key, val in optlist:
	if key == '--file':
		file = val

if file == '' or os.path.exists(file) == 0:
	print ts() + 'no file assigned.'
	sys.stdout.flush()
	sys.exit(1) 


lines = open(file).readlines()

output = []
for line in lines:
        if string.find(line, 'RUN_START_TIME:') != -1:
                tstart=line.split()[1]+' '+line.split()[2].split('.')[0];
        elif (string.find(line, 'RUN_END_TIME:') != -1) :
                tend=line.split()[1]+' '+line.split()[2].split('.')[0];

t1 = time.mktime(datetime.datetime.strptime(tstart, "%Y-%m-%d %H:%M:%S").timetuple())
t2 = time.mktime(datetime.datetime.strptime(tend  , "%Y-%m-%d %H:%M:%S").timetuple())

print tstart, tend
print t1, t2, t2-t1

sys.exit(0)


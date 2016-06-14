#!/usr/bin/env python3
import math
import random
import getopt, sys
import numpy as np

def dis( rp1, rp2 ):
	return math.fabs(rp1-rp2)

def findClas(rps, clases, rpCenters):
	#maxrp = max(rps)			#acquire max and min value of rayparameter
	#minrp = min(rps)
	for irp,rp in enumerate(rps):
		dises = [ dis(rp, center) for center in rpCenters ]
		clases[irp] = np.argmin(dises)
#		print(irp, rp, clases[irp] ,dises)
#		l = maxrp - minrp
#		for icenter,center in enumerate(rpCenters):
#			dl = dis(rp, center)
#			if dl < l:
#				l = dl
#				clases[irp] = icenter
#				print(icenter)
#		print(irp, rp, clases[irp] ,dises)
		#print(clases[irp])


def newCenter(rps, clases, rpCenters, countRpCenters):
	rpCentersNew = [ 0.0 for center in rpCenters]	#intialize rp Center to be ZERO before calculate new center
	#countRpCenters = [ 0 for count in countRpCenters]		#Number of members for each classes
	for icount,count in enumerate( countRpCenters):
		countRpCenters[icount] = 0

	for iclas, clas in enumerate(clases):
		rpCentersNew[clas] += rps[iclas]
		countRpCenters[clas] += 1
	#rpCenters = [center / float( countRpCenters[icenter] ) for icenter, center in enumerate(rpCenters) ]
	for icenter, center in enumerate(rpCentersNew):
		if countRpCenters[icenter] > 0:
			rpCenters[icenter] = center / float( countRpCenters[icenter] )

def calVar(rps, clases, rpCenters):
	var = 0.0
	for irp, rp in enumerate(rps):
		dl = (rp - rpCenters[ clases[irp] ] )
		var += dl * dl
	var = math.sqrt( var / len(rps) )
	return var

def kMean(rps, clases, rpCenters, countRpCenters, numIter, minVar):
	vars = []
	for iterNum in range(numIter):
		findClas(rps, clases, rpCenters)
		newCenter(rps, clases, rpCenters, countRpCenters)
		var = calVar(rps, clases, rpCenters)
		vars.append( var )
		print("Iteration: %4d, Var: %12.6f" %(iterNum,var), file=sys.stderr )
		if var <= minVar:										#VAR cross minVar
			print("Iteration Ends:\n\treach min VAR setting. var: %12.6f" %(var), file=sys.stderr )
			return vars
	print("Iteration Ends:\n\treach max iteratio. var: %12.6f" %(var), file=sys.stderr )
	return vars

def usage():
	HELP = "%s -M numIteration -N numCenter -I inputfile -O outputfile -C centerOutputFile -T minVar -H"
	print( HELP %(sys.argv[0]) )


##########
##########
try:
	opts,args = getopt.getopt( sys.argv[1:], "M:N:I:O:C:T:H", ["iter=","numCenter=","input=","output=","centerfile=","minVar=","help"]  )
except getopt.GetoptError as err:
	print(str(err))
	usage()
	sys.exit(1)

for o,a in opts:
	if o in ("-M", "--iter"):
		numIter = int(a)
	if o in ("-N", "--numCenter"):
		numCenter = int(a)
	if o in ("-I", "--input"):
		fileIn = a
	if o in ("-O", "--output"):
		fileOut = a
	if o in ("-C", "--centerfile"):
		centerFileOut = a
	if o in ("-T", "--minVar"):
		minVar = float(a)
	if o in ("-H", "--help"):
		usage()
		sys.exit(0)


fileNames = [        line.split()[0]   for line in open(fileIn) ]		#Read input file. 1st column is filename, 2nd column is rayparameter
rps   = [ float( line.split()[1] ) for line in open(fileIn) ]
clases  = [0] * len(rps)												#Initialize the class
#maxrp = max(rps)
#minrp = min(rps)
rpCenters = sorted([ random.choice(rps)  for i in range(numCenter) ])	#Initialize the class center, and sort
countRpCenters = [0] * numCenter
#drp = (maxrp - minrp) / numCenter
#rpCenters = [ minrp + drp*(i+0.5)  for i in range(numCenter) ]

vars = kMean(rps, clases, rpCenters, countRpCenters, numIter, minVar)	#K-MEAN

#.append(rpCenters[line[2]])
lst = [ list(line) for line in zip(fileNames, rps, clases) ]			#Sort according to class order
arr = [ [] for i in range(numCenter)]
for line in lst:
	line.append(rpCenters[ line[2] ])
	line.append( countRpCenters[ line[2] ] )
	arr[line[2]].append(line)

fpOut = open(fileOut,"w")												#Output the result
for clas in arr:
	for fileName,rp,clas,center,count in clas:
		print("%-50s %12.5f %3d %12.5f %4d" %(fileName,rp,clas,center,count), file = fpOut)

#fpCenterOut = open(centerFileOut, "w")									#Output the center
#for 

#for fileName,rp,clas in zip(fileNames, rps, clases):
#	print("%-50s %12.5f %3d" %(fileName,rp,clas), file = fpOut )
#


#dtype = [ ('filename', 'S1024'), ('rayparameter', float), ('class', int) ]	#sort using np.sort
#arr = np.array(lst, dtype = dtype)
#arrSorted = np.sort(arr, order = 'class')
#print( type(arrSorted[0][0]) )
#
#print(s)
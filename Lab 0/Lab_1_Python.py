from Photon import *
from pylab import *

ac = "ACCESS_TOKEN_HERE"
name = "SPARK_CORE_NAME_OR_COREID_HERE"

g = Photon(name,ac)
print g.getDevices()
print g.getFunctions()
print g.getVariables()

''' YOUR CODE HERE
Loop and gather data

Save to csv
'''

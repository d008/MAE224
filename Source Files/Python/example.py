from Photon import *

atoken = "abc123"                   #Change this to your access
name = "class1"                      #Change this to your photons name
g = Photon(name,atoken)
g.getDevices()
time.sleep(10)
g.getFunctions()
t = g.getVariables()
print g.setFreq(500)
print g.setInput('A0')
print g.analogRead('A0')

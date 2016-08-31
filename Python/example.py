from Photon import *

atoken = "bd6ce2e37c8f82ea597c418a87e8d4fd480d01be"                   #Change this to your access
name = "class1"                      #Change this to your photons name
g = Photon(name,atoken)
g.getDevices()
time.sleep(10)
g.getFunctions()
t = g.getVariables()
print g.setFreq(500)
print g.setInput('A0')
print g.analogRead('A0')

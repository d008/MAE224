import json
#from matplotlib import pylab
#from pylab import *
from urllib2 import urlopen, Request
from urllib import urlencode
import time
import os.path
import requests

class Photon:
    """A particle object belongs to a class developed to help you communicate to your photon in the wild. Each photon has two attributes that it needs to work.

    Attributes:
        name: A string representing the name of the Photon you wish to communicate with. Your photon ID would also suffice. Note that it is case sensitive.
        access: This is your Particle account access token passed as a string. This is necessary to access your functions and prevents people from abusing your Photon.

    Functions:
        getDevices(): Returns the devices associated with the access token
        getConnection(): Returns whether the device of the same name is connected to the internet
        getFunctions():Returns the functions known by the Photon. Functions can be accessed with push()
        getVariables(): Returns the variables known to the Photon. Variables can be accessed
        fetch(String variable): Returns the value associated with a given variable name
        push(String variable,String argument):Executes the particular funciton with a given String argument
        flash(String file): Takes String argument that is a given path to code that you want to flash to the Photon.
        move(int angle):
        attachServo(int pin)



    Your Particle Photon code will look something like this:

        double var1;
        int var2;
        void setup()
        {
        ...
            Particle.function("PithyName1",function2Execute);
            Particle.function("PithyName2",function3Execute);
            Particle.variable("PithyName3",var1);
            Particle.variable("PithyName4",var2);
        ...
        }

        void loop()
        {
        ...
        }

        int funcion2Execute(String args)
        {
        ...
        }
        int funcion3Execute(String args)
        {
        ...
        }

    """
    def __init__(self, name, access):
        """Return a particle object whose name is *name* and access token is *access*"""
        self.access = access
        self.name = name

    def getDevices(self):
        """Returns the status of any and every device attached to your Particle account. This s handy for finding out if your Photon/Cores are connected to the internet"""
        self.devices = self.cmd("")
        for i in range(len(self.devices)):
            if(self.devices[i]['connected']==True):
                print "%s is connected" %self.devices[i]['name']
            else:
                print "%s is not connected" %self.devices[i]['name']
        return json.dumps(self.devices, sort_keys=True, indent=4, separators=(',', ': '))

    def getConnection(self):
        """Returns the status of any and every device attached to your Particle account. This s handy for finding out if your Photon/Cores are connected to the internet"""
        self.devices = self.cmd("")
        for i in range(size(self.devices)):
            if(self.devices[i]['name']==self.name):
                if(self.devices[i]['connected']==True):
                    return True
                else:
                    return False
        return False

        #return self.devices

    def getFunctions(self):
        """Returns the functions of the device correspoding to the Photon of the same name. This is handy for knowing what functions the Photon is able to execute.
        E.G.

        If you Photon code contains the following lines in setup():

        Particle.function("PithyName1",function2Execute);
        Particle.function("PithyName2",function3Execute);

        Then calling getFunctions() in pithy will return

        PithyName1
        PithyName2
        """
        self.functions = self.cmd("/%s/"%self.name)
        print json.dumps(self.functions['functions'], sort_keys=True, indent=4, separators=(',', ': '))
        funcs = self.functions['functions']
        for (i,j) in enumerate(funcs):
            funcs[i] = str(j)
        return funcs


    def getVariables(self):
        """Returns the name and types of variables for the device correspoding to the Photon of the same name. This is handy for knowing what variables the Photon is able to return.
        E.G.

        If you Photon code contains the following lines:
        double var1;
        int var2;

        Particle.variable("PithyName3",var1);
        Particle.variable("PithyName4",var2);

        Then calling getVariables() in pithy will return

        PithyName3: double
        PithyName4: int
        """
        self.functions = self.cmd("/%s/" %self.name)
        print json.dumps(self.functions['variables'], sort_keys=True, indent=4, separators=(',', ': '))
        funcs = self.functions['variables']
        return funcs



    def cmd(self,cmd,params=None):

        """Simple helper function to access the internet. You should not need to call this function ever, ever ,ever. When you instantiate a particle object, all of the particle functions and variables you wish to access are pushed to specific websites on the Particle servers. This class accesses that data by creating a website and passing your id and access token """
        url = "https://api.particle.io/v1/devices"
        request = Request(url+cmd)
        request.add_header('Authorization', 'Bearer %s' % (self.access))
        if params != None:
            post_params = {'args' : params,}
            post_params = urlencode(post_params)
            response = urlopen(request,post_params)
        else:
            response = urlopen(request)

        data = response.read()
        interp = json.loads(data)
        return interp

    def fetch(self,var):
        """Returns the value associated with the given Particle variable.
        E.G.

        If you Photon code contains the following lines:
        double var1;
        int var2;

        Particle.variable("PithyName3",var1);
        Particle.variable("PithyName4",var2);


        If you call fetch("PithyName3") then the function will return the value associated with var1
        """
        return self.cmd("/%s/%s/" % (self.name,var))['result']

    def push(self,var,val):
        """Executes the function associated with the given Particle variable.
        E.G.

        If you Photon code contains the following lines:
        Particle.function("PithyName1",function2Execute);
        Particle.function("PithyName2",function3Execute);

        int funcion2Execute(String args)
        {
        ...
        }
        int funcion3Execute(String args)
        {
        ...
        }


        If you call push("PithyName1","strArg") then the code will send the String argument *strArg* to the particle server and tell the Photon to execute the function *function2Execute* using the argument *strArgs*.
        """
        return self.cmd("/%s/%s/" % (self.name,var),params = val)['return_value']

    def flash(self,file=None):
        """Flashes the source code located at the filepath given by the String file.

        """
        if file == None:
            return "Empty File"
        elif os.path.isfile(file) == 0:
            return "No Such File"
        elif file.endswith('.ino') or file.endswith('.cpp'):
            base = "https://api.particle.io/v1/devices/%s" %(self.name)
            headers = {'Authorization':"Bearer %s " %self.access}
            files = {'file': open(file, 'rb')}
            r = requests.put(base,headers=headers,files=files)
            r =  r.json()
            print "Message: " + str(r['message'])
            print "Flash OK:" + str(r['ok'])

    def move(self,angle):
        """Requires that a servo be attached to the Photon. Make sure to call attachServo(String pin) prior to trying to call move(int angle). Takes a single int argument, angle, and moves the servo attached to the Photon to that angle.
        """
        return self.push('move',angle)

    def attachServo(self,pin):
        """Takes a single String argument, pin, and attaches a servo to that pin.
        """
        return self.push('attachServo',pin)

    def getPin(self,pin):
        """Takes a single String argument pin and returns the integer equivalent of that pin.
        """
        return self.push('getPin',pin)

    def detachServo(self):
        """Detach the servo from the Photon
        """
        return self.push('detachServo','')

    def setInput(self,pin):
        """Set one of the io pins to input type. Takes a single String argument, pin, and sets it to an input.
        """
        return self.push('setInput',pin)

    def setOutput(self,pin):
        """Set one of the io pins to input type. Takes a single String argument, pin, and sets it to an output.
            """
        return self.push('setOutput',pin)

    def getPinMode(self,pin):
        """Takes a single String argument, pin, and returns whether that pin is an input or and output"""
        t = self.push('setOutput',pin)
        if t == 1:
            print pin +' is an INPUT pin'
        elif t==0:
            print pin + ' is an OUTPUT pin'

    def analogRead(self,pin):
        """Takes a single String argument, pin, and returns the analog bit value read from that pin = [0,4096)"""
        self.setInput(pin)
        return self.push('analogRead',pin)

    def digitalRead(self,pin):
        
        """Takes a single String argument, pin, and returns the digital bit value read from that pin = 0,1"""
        self.setInput(pin)
        return self.push('digitalRead',pin)

    def analogWrite(self,pin,value):
        """Takes a single String argument, pin, and writes the analog bit value read to that pin=[0,256)"""
        self.setOutput(pin)
        return self.push('analogWrite',pin+','+str(value))

    def digitalWrite(self,pin,value):
        """Takes a single String argument, pin, and writes the digital bit value read to that pin=0,1"""
        self.setOutput(pin)
        return self.push('digitalWrite',pin+','+str(value))

    def setFreq(self,f):
        """Set the frequency at which the analog pins are writing data."""
        t= self.push('setFreq',f)
        print "Analog write Frequency is now %d Hz" %t
        return t

    def getTone(self,pin):
        """Returns the frequency of a square wave from a given pin, passed by String argument pin"""
        self.push('getPulse',pin)
        return self.fetch('pulse')

if __name__ == "__main__":
    ac = "abc123"                   #Change this to your access token
    g = Photon("class1",ac)         #Change this to your photons name
    g.getDevices()
    g.flash('PhotonCode.ino')
    time.sleep(10)
    g.getFunctions()
    t = g.getVariables()
    print g.setFreq(500)
    print g.setInput('A0')
    print g.setInput('A0')
    print g.analogRead('A0')
    #print g.flash("temp.ino")

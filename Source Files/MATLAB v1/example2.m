%% Read Data Example
%Enter access token below. This can be found in the settings of your Particle Account
atoken = 'abc123'; %YOUR ACCESS TOKEN HERE

%Enter the core ID
core =  'class1'; %YOUR PHOTON ID OR NAME HERE

%Instantiates a new Photon object
g = Photon(core,atoken);
g.getConnectedDevices()'


%% Move a servo

%Attach a servo to D0
g.attachServo('D0');

%Check if the device is connected
if g.getConnection
%Rotate a half roration clockwise
    for i = 10:10:180
        g.move(i);
        pause(1)
    end
%Rotate a half roration counter-clockwise    
    for i = 180:-10:10
        g.move(i);
        pause(1)
    end
end
%Detach the servo
g.detachServo()
%% Read PWM Frequency
%Set the write frequency of the analog pins
g.setFreq(3400)
%Write to A5
g.analogWrite('A5',100)
g.analogRead('A1')
g.getTone('A1')

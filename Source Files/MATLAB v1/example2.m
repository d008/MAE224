%% Read Data Example
%Enter access token below. This can be found in the settings of your Particle Account
atoken = 'bd6ce2e37c8f82ea597c418a87e8d4fd480d01be'; %YOUR ACCESS TOKEN HERE

%Enter the core ID
core =  'class1'; %YOUR PHOTON ID OR NAME HERE

%Instantiates a new Photon object
g = Photon(core,atoken);
g.getConnectedDevices()'


%% Move a servo
N = 20;

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
%%
g.setFreq(100)
g.analogWrite('A5',100)
g.analogRead('A0')
g.analogRead('A0')

g.getTone('A0')
%%  Plot that data
%{
figure(1)
clf
plot(time,data,'b-o')
hold on
plot(time,data2,'r-s');
hold off
xlabel('Time(s)')
ylabel('Voltage')
legend('Analog','Digital','location','southeast'
%}
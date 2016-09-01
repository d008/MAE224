%% Read Data Example
%Enter access token below. This can be found in the settings of your Particle Account
atoken = 'abc123'; %YOUR ACCESS TOKEN HERE

%Enter the core ID
core =  'class1'; %YOUR PHOTON ID OR NAME HERE

%Instantiates a new Photon object
g = Photon(core,atoken);
g.getConnectedDevices()'

%% Create Empty arrays
N = 20;
data = zeros(1,N);
data2 = zeros(1,N);
time = zeros(1,N);


%Check if the device is connected
if g.getConnection
    %Iterate from 1 to 10. Increment the analog write from 0 -255. Alternate
    %the digital write between 1 and 0.  Connect A3 to A4 and D3 to D7.
    tic
    for i = 1:N
        i
        g.analogWrite('A4',floor(255/N)*i);
        g.digitalWrite('D7',mod(i,2));
        data(i)  = g.analogRead('A3')/4095*3.3;
        data2(i)  = g.digitalRead('D3')*3.3;
        time(i) = toc;
    end
end
%%  Plot that data
figure(1)
clf
plot(time,data,'b-o')
hold on
plot(time,data2,'r-s');
hold off
xlabel('Time(s)')
ylabel('Voltage')
legend('Analog','Digital','location','southeast')

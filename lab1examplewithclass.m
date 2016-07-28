%% Read Data Example

%Enter access token here. This can be found from the settings of your Spark
%Core account
atoken = 'bd6ce2e37c8f82ea597c418a87e8d4fd480d01be';

%Enter the core ID
core =  'class1';
g = Photon(core,atoken);
g.getConnectedDevices()

%%
N = 25;
data = zeros(1,N);
data2 = zeros(1,N);
time = zeros(1,N);

g.setFreq(500);
tic

g.analogWrite('A4',0);
    pause(1)
for i = 1:N
    i;
    g.analogWrite('A4',10*i);
    g.digitalWrite('D4',mod(i,2));
    pause(1)
    temp  = g.analogRead('A3')/4095*3.3
    temp2  = g.digitalRead('D3')
    data(i)  = temp;
    data2(i)  = temp2;
    time(i) = toc;
    pause(1)
end

plot(time,data)

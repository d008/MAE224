%% Read Data Example

%Enter access token here. This can be found from the settings of your Spark
%Core account
atoken = '';

%Enter the core ID
core =  'class1';
g = Photon(core,atoken);
g.getConnectedDevices()
N = 40;
data = zeros(1,N);
time = zeros(1,N);
g.setInput('A4');
tic
for i = 1:N
    i
    data(i) = g.analogRead('A4');
    time(i) = toc;
    pause(0.5)
end
plot(time,data)

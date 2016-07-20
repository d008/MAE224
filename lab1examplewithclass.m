%% Read Data Example
%Number of Data Points
npoints = 100;
data = zeros(1,npoints);
time = data;
%Enter access token here. This can be found from the settings of your Spark
%Core account
atoken = 'bd6ce2e37c8f82ea597c418a87e8d4fd480d01be';

%Enter the core ID
core =  'steve';

g = Photon(core,atoken);
g.getFunctions()
g.getVariables()
%g.fetch('temp')
%g.push('f','100')
%g.flash('untitled.ino')
%Enter the variable name 
%varname = 'Flow';
%varname = 'getPower';
%This for loop fetches the data and plots it
tic 
%% 
%g.push('setPower','0')
for n = 1:1:npoints    
    temp = g.fetch('pressure')
    %temp = g.push('functionname', 'functionargs')
    data(n) = temp;
    time(n) = toc;
    if n > 10001
    plot(time(n-100:n),data(n-100:n),'k-s','MarkerSize',20,'MarkerFaceColor','k');
    else
    plot(time(1:n),data(1:n),'k-s','MarkerSize',20,'MarkerFaceColor','k');
    end
    pause(1)
    %g.push('turn',int2str(50));
end
toc

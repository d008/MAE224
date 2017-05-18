% Initail guess w/ shooting method
clear all
deta = 0.01;
eta = 20;

etas = 0:deta:eta;
f1 = zeros(length(etas),3);
f1(1,:) = [1,0,0];
f1 = rk4(f1,etas,deta);

f2 = zeros(length(etas),3);
f2(1,:) = [0.5,0,0];
f2 = rk4(f2,etas,deta);

%Secant Iteration
for j = 1:6
    f3 = zeros(length(etas),3);
    sec = f2(1,1) + (f2(1,1)-f1(1,1))/(f2(end,2)-f1(end,2))*(1-f2(end,2));
    f3(1,:) = [sec,0,0];
    f3 = rk4(f3,etas,deta);
    f1=f2;
    f2=f3;
end


%% Physical Parameters
clc
Uinf = 17.0;
nu =  1.8e-5/1.2;
x = 595e-3;

re = Uinf * x /nu
y = etas/sqrt(Uinf/(2*nu*x));
u = Uinf*f3(:,2);
deltaL = 4.9*x/sqrt(re);
d99 = y(min(find(u>0.99*Uinf)))
deltaT = 0.38*x/re.^(0.2)
plot(u,y,'b')
hold on
plot(Uinf*(y/deltaT).^(1.0/7.0),y,'g');
hold off
title('Boundary Layer Profiles')
xlabel('U (m/s)')
ylabel('y (m)')
legend('Blasius/Laminar','Turbulent"')
axis([0, Uinf*1.05,0,deltaT])

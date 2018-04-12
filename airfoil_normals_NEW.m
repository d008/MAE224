function [ xn, yn, u, v ] = airfoil_normals( aoa )
%Code for outputting NACA 0018 Surface Normals
%   aoa = current angle of attack (degrees)
% OUTPUTS: xn,yn pointer vectors from leading edge of airfoil
% u,v are the x and y components of the surface normals
aoa = deg2rad(aoa); %convert to radians
% Chord (m) %
c = 0.3 ;
%eqn to enerate NACA 0018 Points%
naca = @(x,t,c) 5.*t./100.*c.*( 0.2969.*sqrt(x./c) ...
                -0.1260.*(x./c) - 0.3516.*(x./c).^2 ...
                +0.2843.*(x./c).^3 - 0.1015.*(x./c).^4 );
%% draw NACA 0018 airfoil outline
xs=c.*[0:0.01:1];  
ys=naca(xs,18,c); 
xs=[xs fliplr(xs)]; % add bottom of airfoil
ys=[ys -1.*fliplr(ys)];
% Angela Merkel--% Rotate Airfoil
x_a = xs.*cos(aoa) + ys.*sin(aoa);
y_a = ys.*cos(aoa) - xs.*sin(aoa);
% plot airfoil
figure(1); hold on;
plot(x_a,y_a,'b-'); 
daspect([1 1 1]);

%% calculate and plot the pressure tap locations%
X_top = 0:1/15.5:1; X_bot = (1-1.5/15.5):-1/15.5:0;
X=c.*[X_top X_bot(1:end-1)];
Y=naca(X,18,c);
Y = [ Y(1:16) -1.*Y(17:end)];
% Angela Merkel--% Rotate Airfoil 
xn = X.*cos(aoa) + Y.*sin(aoa);
yn = Y.*cos(aoa) - X.*sin(aoa);
% plot pressure taps 
plot(xn,yn,'bo')
    text(xn(1),yn(1),'Tap 1 (top)')
    text(xn(end),yn(end),'Tap 30 (bottom)')
    

%% Find x and y surface normals%
xL = circshift(xn,[0,1]); xR = circshift(xn,[0,-1]); %Generate x and y area
yL = circshift(yn,[0,1]); yR = circshift(yn,[0,-1]); %elements that P acts on
    dx = (xR - xL);
    dy = (yL - yR);
% modify the 2 taps closest to trailing edge 
x_TE= c.*cos(aoa);% coordinates of trailing edge
y_TE=  -c.*sin(aoa);
% tap 16 
dx(16)=x_TE-xL(16); 
dy(16)=yL(16)-y_TE;
% tap 17 
dx(17)=xR(17)-x_TE; 
dy(17)=y_TE-yR(17);

%Normalize Vectors%
m = sqrt(dx.^2 + dy.^2);
u = dy ./ m ;
v = dx ./ m ;

%Plot surface Normals%
quiver(xn,yn,u,v,'b');

end

function [ xn, yn, u, v ] = airfoil_normals( aoa )
%Code for outputting NACA 0018 Surface Normals
%   aoa = current angle of attack (degrees)
% OUTPUTS: xn,yn pointer vectors from leading edge of airfoil
% u,v are the x and y components of the surface normals

% Chord (m) %
c = 0.3 ;

%Generate NACA 0018 Points%
naca = @(x,t,c) 5.*t./100.*c.*( 0.2969.*sqrt(x./c) ...
                -0.1260.*(x./c) - 0.3516.*(x./c).^2 ...
                +0.2843.*(x./c).^3 - 0.1015.*(x./c).^4 );
    
%Set the x spacing%
X = linspace(0,1,16);
X = X'.*c; Y = naca(X,18,c);

xpts = [ X; flipud(X(2:end))];% - 0.25.*c;
ypts = [ Y; -1.*flipud(Y(2:end))];

%Angela Merkel--%
%Rotate Airfoil to given AoA about 1/4 chord%
aoa = aoa .* pi ./ 180; %convert to radians
xn = xpts.*cos(aoa) + ypts.*sin(aoa);
yn = ypts.*cos(aoa) - xpts.*sin(aoa);

%Find x and y surface normals%
xL = circshift(xn,[0,1]); xR = circshift(xn,[0,-1]); %Generate x and y area
yL = circshift(yn,[0,1]); yR = circshift(yn,[0,-1]); %elements that P acts on
    m = sqrt((xL-xR).^2 + (yL-yR).^2);
    dx = (xR - xL);
    dy = (yL - yR);
    
%Normalize Vectors%
    u = dy ./ m ;
    v = dx ./ m ;
%Get rid of taps not connected%    
    ind = find(1:1:numel(u) ~= 17);
    xn = xn(ind); yn = yn(ind); u = u(ind); v = v(ind);
%Plot surface Normals%
figure(1)
quiver(xn,yn,u,v,'r');
    hold on
plot([xn; 0],[yn; 0],'bo-')
    text(xn(1),yn(1),'Tap 1 (top)')
    text(xn(end),yn(end),'Tap 30 (bottom)')
    hold off

end


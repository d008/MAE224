function [ xn, yn, u, v ] = cylinder_normals()
%Code for outputting the Cylinder Surface Normals
%   aoa = current angle of attack (degrees)
% OUTPUTS: xn,yn pointer vectors from leading edge of cylinder
% u,v are the x and y components of the surface normals

r = 0.1/2;      %Cyl. radius (m)
dtheta = 15;    %Distance between each tap
%Generate angular locations%
theta = 0:dtheta:(360-dtheta);
    xn = r.*cos(theta.*pi./180) + r;%switch to cartesian
    yn = r.*sin(theta.*pi./180) ;

%Find Surface Normals%
xL = circshift(xn,1); xR = circshift(xn,-1); %Generate x and y area
yL = circshift(yn,1); yR = circshift(yn,-1); %elements that P acts on
    v = (xL - xR);
    u = (yR - yL);

%Get rid of taps not connected%
    ind = 3:1:round(numel(theta)-1);
    xn = xn(ind); yn = yn(ind); u = u(ind); v = v(ind);
% Plot actual cylinder % 
figure(2)
    plot(xn,yn,'bo-')
    hold on
%Plot surface  normals%
 quiver(xn,yn,u,v,'r');
    %Add some useful text%
    text(xn(1),yn(1),'Tap 11 (top)')
    text(xn(end),yn(end),'Tap 12 (bottom)')
    stag = find(xn == 0);
    text(xn(stag),yn(stag),'Tap 1 (top)')
    text(xn(stag+1),yn(stag+1),'Tap 21 (bottom)')
  hold off 
    
    
end


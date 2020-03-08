function [ dy ] = blasius_diffeq( f )
%blasius_diffeq - a function that takes a single parameter and returns the
% derivative according to the blasius ODE
%   Detailed explanation goes here

dy = zeros(1,3);
dy(1) = -0.5*(f(1)*f(3));
dy(2) = f(1);
dy(3) = f(2);

end


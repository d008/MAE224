function [ f ] = rk4( f, etas,deta)
%rk4 - iterates in time according to fourth order rungekutta
for i = 1:(length(etas)-1)
    k1 = deta*blasius_diffeq(f(i,:));
    k2 = deta*blasius_diffeq(f(i,:)+k1/2);
    k3 = deta*blasius_diffeq(f(i,:)+k2/2);
    k4 = deta*blasius_diffeq(f(i,:)+k3);
    f(i+1,:) = f(i,:) + k1/6 + (k2 +k3)/3 + k4/6;
    
end

end
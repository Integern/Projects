function [f] = myexpt(a,n)
%computes the exponential a^n recursively

if n==0
    f=eye(size(a));
elseif n==2*floor(n/2)
    f=myexpt(a*a,floor(n/2));
else
    f=a*myexpt(a*a,floor((n-1)/2));
end
end


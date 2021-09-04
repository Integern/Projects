function [f] = Simple(a,x)
%Returns the polynomial p(x) according to Horner's Rule (1)
%Note: a is a 3D array where a_i=a(:,:,i+1)

k=size(a,3);

if size(x,1)~= size(x,2)
    
    f='Error: x is not compatible (ie. not a square matrix)';

elseif k>1
    
    f = a(:,:,(k-1)) + x*(a(:,:,k)); %starts output at a_n-1+xa_n
    
    if k>2
        for i=2:(k-1) %repetition of Horner's rule to find p(x)
            f = a(:,:,(k-i)) + x*f;
        end
    end

elseif k==1
    f=a(:,:,1);

end
end


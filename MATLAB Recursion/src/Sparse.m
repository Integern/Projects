function [f] = Sparse(index,b,x)
%Returns the polynomial p(x) according to Horner's Rule (2)
%index is a row vector corresponding to the powers of x with non-zero
%coefficients in ascending order
%Note: b is a 3D array with the corresponding coefficients

k=length(index);

if k==0 && isempty(b)==1 %accounts for when p(x) has no non-zero coefficients
    f=0; 
elseif k~= size(b,3)
    f='Error: Incorrect number of coefficients to exponents';
elseif k>1    
    %start output at b_k-1 +b_k*x^(i_k-i_k-1)
    f = b(:,:,(k-1)) + myexpt(x,(index(1,k)-index(1,k-1)))*b(:,:,k);

    if k>2
        for i=1:(k-2) %repetition in Horner's Rule (2) to find p(x)
            n = index(1,k-i) - index(1,k-(i+1));
            f = myexpt(x,n)*f +b(:,:,k-(i+1));
        end
    else
        f=myexpt(x,index(1,k-1))*f;
    end
elseif k>0
    f=myexpt(x,index(1,1))*b(:,:,1);
end

end


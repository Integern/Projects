function [marks, error] = test_spar3()
%Test 3 of Sparse function for correctness
%tests with integer coefficients and a matrix x
%tests '12x^7 -4x' with x=[2,-3;7,0]

error=[];
marks=0;

try
   b(:,:,1)=-4;
   b(:,:,2)=12;
   
   index=[1,7];
   
   x=[2,-3;7,0];
   correct=12*x^7-4*x;
   fprintf('Test 3 Running: [checking Sparse]')
   output=Sparse(index,b,x);
   
   if size(output,1)~=2 && size(output,2)~=2 %checks for size error
       error='Output matrix not of size 2x2';
       fprintf('...Test 3 Failed; \n')
       return
   end
   
   errorM=abs(correct-output);
   if errorM > eps(42) %error margin
       error='Output matrix is outside error margin';
       fprintf('...Test 3 Failed; \n')
       return
   end
   
   marks=1;
   fprintf('...Test 3 Passed; \n')
   
catch error
    error=error.message;
    fprintf('%s\n',error);
end

end
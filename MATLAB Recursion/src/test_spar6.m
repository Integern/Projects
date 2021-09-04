function [marks, error] = test_spar6()
%Test 6 of Sparse function for correctness
%tests a constant function p(x)=0
%(from Coursework Sheet) index=[], b is an array of size NxNx0

error=[];
marks=0;

try
   b=[];
   
   index=[];
   
   x=[2,3;1,0];
   correct=0;
   fprintf('Test 6 Running: [checking Sparse]')
   output=Sparse(index,b,x);
   
   errorM=abs(correct-output);
   if errorM > eps(42) %error margin
       error='Output matrix is outside error margin';
       fprintf('...Test 6 Failed; \n')
       return
   end
   
   marks=1;
   fprintf('...Test 6 Passed; \n')
   
catch error
    error=error.message;
    fprintf('%s\n',error);
end

end
function [marks, error] = test_spar5()
%Test 5 of Sparse function for correctness
%tests with matrix coefficients and a matrix for x (where matrix
%multiplication is possible)
%tests 'mx^2 -nx' with x=[2,3;1,0], m=[6,7;1,3], n=[4,9;4,1]

error=[];
marks=0;

try
   b(1,1,1)=4;
   b(1,2,1)=9;
   b(2,2,1)=1;
   b(2,1,1)=4;
   b(1,1,2)=6;
   b(1,2,2)=7;
   b(2,2,2)=3;
   b(2,1,2)=1;
   
   index=[1,2];
   
   x=[2,3;1,0];
   correct=[73,69;22,27];
   fprintf('Test 5 Running: [checking Sparse]')
   output=Sparse(index,b,x);
   
   if size(output,1)~=2 && size(output,2)~=2 %checks for size error
       error='Output matrix not size 2x2';
       fprintf('...Test 5 Failed; \n')
       return
   end
   
   errorM=abs(correct-output);
   if errorM > eps(42) %error margin
       error='Output matrix is outside error margin';
       fprintf('...Test 5 Failed; \n')
       return
   end
   
   marks=1;
   fprintf('...Test 5 Passed; \n')
   
catch error
    error=error.message;
    fprintf('%s\n',error);
end

end
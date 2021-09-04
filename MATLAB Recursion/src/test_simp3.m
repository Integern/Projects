function [marks, error] = test_simp3()
%Test 3 of Simple function for correctness
%tests with integer coefficients and a matrix for x
%tests '5x^2 -2x' with x=[2,3;1,0]

error=[];
marks=0;

try
   a(:,:,1)=0;
   a(:,:,2)=-2;
   a(:,:,3)=5;
   
   x=[2,3;1,0];
   correct=[31,24;8,15];
   fprintf('Test 3 Running: [checking Simple]')
   output=Simple(a,x);
   
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
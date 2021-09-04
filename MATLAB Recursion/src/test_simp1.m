function [marks, error] = test_simp1()
%Test 1 of Simple function for correctness
%tests 'x^2 - 1' with x=5

error=[];
marks=0;

try
   a(:,:,1)=-1;
   a(:,:,2)=0;
   a(:,:,3)=1;
   
   x=5;
   correct=24;
   fprintf('Test 1 Running: [checking Simple]')
   output=Simple(a,x);
   
   if size(output,1)~=1 && size(output,2)~=1 %checks for size error
       error='Output matrix not of size 1x1';
       fprintf('...Test 1 Failed; \n')
       return
   end
   
   errorM=abs(correct-output);
   if errorM > eps(42) %error margin
       error='Output matrix is outside error margin';
       fprintf('...Test 1 Failed; \n')
       return
   end
   
   marks=1;
   fprintf('...Test 1 Passed; \n')
   
catch error
    error=error.message;
    fprintf('%s\n',error);
end

end


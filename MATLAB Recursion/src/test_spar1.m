function [marks, error] = test_spar1()
%Test 1 of Sparse function for correctness
%tests 'x^2 - 1' with x=5

error=[];
marks=0;

try
   b(:,:,1)=-1;
   b(:,:,2)=1;
   
   index=[0,2];
   
   x=5;
   correct=24;
   fprintf('Test 1 Running: [checking Sparse]')
   output=Sparse(index,b,x);
   
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


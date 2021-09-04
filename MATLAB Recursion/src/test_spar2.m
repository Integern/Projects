function [marks, error] = test_spar2()
%Test 2 of Sparse function for correctness
%tests '3x^4 +12x^3 -17x +24' with x=143

error=[];
marks=0;

try
   b(:,:,1)=24;
   b(:,:,2)=-17;
   b(:,:,3)=12;
   b(:,:,4)=3;
   
   index=[0,1,3,4];
   
   x=143;
   correct=1289572880;
   fprintf('Test 2 Running: [checking Sparse]')
   output=Sparse(index,b,x);
   
   if size(output,1)~=1 && size(output,2)~=1 %checks for size error
       error='Output matrix not of size 1x1';
       fprintf('...Test 2 Failed; \n')
       return
   end
   
   errorM=abs(correct-output);
   if errorM > eps(42) %error margin
       error='Output matrix is outside error margin';
       fprintf('...Test 2 Failed; \n')
       return
   end
   
   marks=1;
   fprintf('...Test 2 Passed; \n')
   
catch error
    error=error.message;
    fprintf('%s\n',error);
end

end
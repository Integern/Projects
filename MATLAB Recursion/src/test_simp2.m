function [marks, error] = test_simp2()
%Test 2 of Simple function for correctness
%tests '3x^4 +12x^3 -17x +24' with x=143

error=[];
marks=0;

try
   a(:,:,1)=24;
   a(:,:,2)=-17;
   a(:,:,3)=0;
   a(:,:,4)=12;
   a(:,:,5)=3;
   
   x=143;
   correct=1289572880;
   fprintf('Test 2 Running: [checking Simple]')
   output=Simple(a,x);
   
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


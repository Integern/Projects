function [marks, error] = test_simp6()
%Test 6 of Simple function for correctness
%tests the case when p(x)=0

error=[];
marks=0;

try
   a(:,:,1)=0;
     
   x=[2,3;1,0];
   correct=0;
   fprintf('Test 6 Running: [checking Simple]')
   output=Simple(a,x);
   
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
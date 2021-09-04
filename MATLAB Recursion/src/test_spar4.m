function [marks, error] = test_spar4()
%Test 4 of Sparse function for detection of non-compatible matrices
%tests index=[0,3,4] with b an NxNx5 matrix
%x can be any value; in this case x=5

error=[];
marks=0;

try
   b(:,:,1)=1;
   b(:,:,2)=2;
   b(:,:,3)=4;
   b(:,:,4)=5;
   b(:,:,5)=3;
   
   index=[0,3,4];
   
   x=5;
   
   fprintf('Test 4 Running: [checking Sparse]')
   output=Sparse(index,b,x);
   
   if 1== strcmp('Error: Incorrect number of coefficients to exponents',output)
       %checks function outputs the correct error message
       fprintf('...Test 4 Passed; \n')
       marks=1;
       return
   else 
       fprintf('...Test Failed; \n')
   end
   
catch error
    error=error.message;
    fprintf('%s\n',error);
end

end
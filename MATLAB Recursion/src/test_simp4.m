function [marks, error] = test_simp4()
%Test 4 of Simple function for detection
%tests when x isn't a compatible matrix
%i.e. x isn't a square matrix

error=[];
marks=0;

try
   
   a(:,:,1)=-1;
   a(:,:,2)=0;
   a(:,:,3)=1;
   
   x=[1,1,1;2,3,7];
   
   fprintf('Test 4 Running: [checking Simple]')
   output=Simple(a,x);
   
   if 1== strcmp('Error: x is not compatible (ie. not a square matrix)',output)
       %checks function outputs correct error message
       fprintf('...Test 4 Passed; \n')
       marks=1;
       return
   else 
       fprintf('...Test 4 Failed; \n')
   end
   
catch error
    error=error.message;
    fprintf('%s\n',error);
end

end
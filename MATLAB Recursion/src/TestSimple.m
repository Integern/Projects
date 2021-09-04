function [marks,total] = TestSimple()
%Tests that the function Simple computes the polynomial p(x) 
%according Horner's Rule (1)
%based on JHD's TestMult

marks = 0;
total=0;
for i=1:6
    total=total+1;
    fhandle = str2func(strcat('test_simp',num2str(total))); % Make the function name test_simpX
    [m,e]=fhandle();  % These functions return the marks (0/1) and any error
    marks = marks + m;
    if (m==0)
        if isempty(e)== 0
            fprintf('test_simp%d gave error: %s\n',total,e)
        else
            fprintf('test_simp%d is wrong but no error',e)
        end
    end
end

end


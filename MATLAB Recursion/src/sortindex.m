function [fv, ind] = sortindex(v)
%Sorts a row vector into increasing order using JHD's myIsort function
%Also outputs a row vector for the index to the original vector

fv=myMsort(v); %sorts row vector v into ascending order
k=length(v);
ind=v; %to prevent variable changing length as function progresses

for i=1:k
    for j=1:k 
        if v(1,j)== fv(1,i)
            ind(1,i)=j;
        end
    end
end

end


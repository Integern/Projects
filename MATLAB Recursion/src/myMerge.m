function [ out ] = myMerge( a,b )
% MYMERGE: takes two sorted row vectors and produces the (sorted) merge
% running time linear in output size J.H.Davenport 4 November 2009 
out=zeros(1,length(a)+length(b)); % Avoid growing array!
j=1;k=1;l=1;
while (j<=length(a))&&(k<=length(b))
    if (a(1,j)<b(1,k))
        out(1,l)=a(1,j); j=j+1;l=l+1;
    else
        out(1,l)=b(1,k); k=k+1;l=l+1;
    end
end    % That's the merging done, now for the residual bits
% Note that only one of these loops will be used in any case,
% We just don't know which!
while (j<=length(a))
    out(1,l)=a(1,j); j=j+1;l=l+1;
end
while (k<=length(b))
    out(1,l)=b(1,k); k=k+1;l=l+1;
end

end


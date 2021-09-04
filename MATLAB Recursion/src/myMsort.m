function [ out ] = myMsort( in )
%MYMSORT sorts a row vector into increasing order with <
% J.H.Davenport 27/10/2015; Merge sort
n=length(in);
if (n==1)
    out=in;
else
    out=myMerge(myMsort(in(1:floor(n/2))),...
                myMsort(in(floor(n/2)+1:n)));
end

end


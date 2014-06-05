B = importdata('../A.txt');
nvars = 297;
[m,n] = size(B);
H = zeros(nvars*2,nvars*2);
f = zeros(nvars*2,1);
c = 0;
xorCount = 0;
nxorCount = 0;

for i = 1:m,
    if B(i,3) == 1,
        %xor gate
        H(B(i,1), B(i,2))=H(B(i,1), B(i,2))+2; 
        H(B(i,2), B(i,1))=H(B(i,2), B(i,1))+2; 
        f(B(i,1),1) = f(B(i,1),1)-1;
        f(B(i,2),1) = f(B(i,2),1)-1;
        xorCount = xorCount+1;
    elseif B(i,3) == 2,
        %xnor gate
        H(B(i,1), B(i,2))=H(B(i,1), B(i,2))-2; 
        H(B(i,2), B(i,1))=H(B(i,2), B(i,1))-2; 
        f(B(i,1),1) = f(B(i,1),1)+1;
        f(B(i,2),1) = f(B(i,2),1)+1;
        c = c-1;
        nxorCount = nxorCount+1;
    else
        disp 'incorrect input file';
        break;
    end
end
ctype = '';
%char(zeros(nvars*2,1));
for i = 1:nvars*2,
    ctype = strcat(ctype,'B');
end
x = cplexmiqp(H,f,[],[],[],[],[],[],[],[],[],ctype);
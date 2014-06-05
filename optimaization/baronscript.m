nvars = 713;
f  = @objfun;
ctype = '';
for i = i:nvars*2,
    ctype = strcat(ctype, 'B');
end
x0 = zeros(713*2,1);
opts = baronset('MaxTime', 120);
x = baron(f,[],[],[],[],[],[],[],[],ctype,x0,opts);
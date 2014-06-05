cd('F:/study/btp/btp');
A_ = importdata('A.txt');
b = importdata('b.txt');
A = zeros(10391, 21493);
[m,n] = size(A_);
[am,an]= size(A);
for j=1:m,
    if A_(j,1)+1>am | A_(j,2)+1 >an
        disp(A_(j,1)+1);
        disp(A_(j,2)+1);
        break;
    end
    A( A_(j,1)+1, A_(j,2)+1 ) = A_(j,3);
end
f = [zeros(5551*2,1 );ones(10391,1)];
f= transpose(f);
x = bintprog(f,A,b);
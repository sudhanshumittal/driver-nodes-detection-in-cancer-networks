function[]= GSA(F, Ef ,... %=tolerance for function evaluation,
                Eu,... %=tolerance for barrier value,
                M,... %=maximum penalty value,
                N,... %=iteration limit for applying Newton’s method,
                Tu,... %=ratio for barrier parameter decrement,
                Tg,... %=ratio for penalty parameter increment,
                U0,... %=initial barrier parameter,
                G0,... %=initial penalty parameter,
                r... %= any feasible starting point
)
G = G0;
U= U0;
grad = gradient(F);
gradSq =   gradient(grad);      
while G < M || (U > Eu)
    x0 = r;
    xn= x0;
    for i = 0:N,
        if norm(transpose(Z)*grad(x0))<Ef*U,
            xn= xl; l=n;
            %check if xn isa direction of negative gradient
        else
           A = transpose(Z)*gradSq(xl);
           CONJGRAD( A*Z,-transpose(Z)*grad(xl));
           
        end
    
    end
    r = xn; U = Tu*U; G = Tg*G;
end
end
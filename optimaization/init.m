nvars = 2*1259;
LB = zeros(1, nvars);
UB = ones(1, nvars);
ObjectiveFunction = @objfun;
ConstraintFunction = @confuneq;
options = gaoptimset('MutationFcn',@mutationadaptfeasible);
options=gaoptimset(options,'PlotFcns',{@gaplotbestf,@gaplotmaxconstr},...
    'Display','iter');
[x,fval] = ga(ObjectiveFunction,nvars,[],[],[],[],LB,UB, ...
    ConstraintFunction,options);
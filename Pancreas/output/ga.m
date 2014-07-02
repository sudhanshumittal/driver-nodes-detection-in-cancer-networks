function [x,fval,exitflag,output,population,score] = ga(nvars,lb,ub,intcon,PopulationSize_Data)
%% This is an auto generated MATLAB file from Optimization Tool.

%% Start with the default options
options = gaoptimset;
%% Modify options setting
options = gaoptimset(options,'PopulationSize', PopulationSize_Data);
options = gaoptimset(options,'CreationFcn', @gacreationlinearfeasible);
options = gaoptimset(options,'SelectionFcn', {  @selectiontournament 2  });
options = gaoptimset(options,'CrossoverFcn', @crossoversinglepoint);
options = gaoptimset(options,'MutationFcn', {  @mutationuniform [] });
options = gaoptimset(options,'Display', 'off');
options = gaoptimset(options,'PlotFcns', { @gaplotbestf });
[x,fval,exitflag,output,population,score] = ...
ga(@cost,nvars,[],[],[],[],lb,ub,[],intcon,options);

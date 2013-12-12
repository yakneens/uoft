% This is code for backpropagation learning that is designed to be 
% easy to understand and to use. It is not at all elegant.
% It uses global variables instead of functions.
% This makes it easier to look at all the internal variables.
% It assumes that there is only one hidden layer.
% It assumes the error function is squared error.


% You must define the following global variables: 
% data     (must be a matrix with one row per training case).
% targets  (must be a matrix with one row per training case).
% restart  (set it to 1 to initialize the weights & epoch number).
% maxepoch (set it to be the number of training epochs.
%   Increase maxepoch and call basicbp again to continue training).

numcases = size(data,1);
numin    = size(data,2); 
numout   = size(targets,2);

%%%%%%%  GLOBAL VARIABLES: SET BY EDITING THIS FILE %%%
numhid   = 4; %number of hidden units
initialweightsize = 0.1;
weightcost = .001; %determines cost of squared weights. Try .01

initialepsilon = .01; %initial learning rate
epsiloninc = 1.1; %for increasing learning rate when its working
epsilondec = .5;  %for decreasing the rate when its not.

errorprintfreq  = 10; %how often to show the error.
weightprintfreq = 50; %how often to show the weights.

%%%%%%%  END OF GLOBAL VARIABLES FOR YOU TO SET %%%%%


%%%%% BEGIN INITIALIZATION %%%%%%%
if restart==1
  restart=0;
  epoch=1; 
  epsilon=initialepsilon;
  previousE = inf; %we need to know if the error is decreasing.
     %Now initialize the weights and biases from a gaussian 
     %distribution with small standard deviation.
  inhid = initialweightsize*randn(numin, numhid); 
     %inhid is the matrix of input to hidden weights.
  hidout = initialweightsize*randn(numhid, numout); 
  hidbiases = initialweightsize*randn(1, numhid); 
  outbiases = initialweightsize*randn(1, numout); 
end;
%%%%% END INITIALIZATION %%%%%%%%%



for epoch=epoch:maxepoch

  %%%%%% BEGIN FORWARD PASS %%%%%%%%%
  % First we do forward passes for all of the cases in
  % parallel by using one case per row in various matrices.
  hidsum = data*inhid + repmat(hidbiases, numcases, 1);
    % repmat(M,V,H) replicates M V times Vertically 
    % and H times Horizontally
  hidacts = 1./(1+exp(-hidsum));
    % the ./ makes matlab replicate across all elements of hidsum,
    % so each row of hidacts will be the activity of
    % all hidden units for a particular training case.
  outsum = hidacts*hidout + repmat(outbiases, numcases, 1);
  outputs = 1./(1+exp(-outsum));
  %%%%%% END FORWARD PASS  %%%%%%%%%


  residuals = outputs-targets;
  dEbydoutputs = residuals;
    %the error derivatives with respect to the output activities.
  E = 0.5 * sum(sum(residuals.*residuals));
    %% the .* makes matlab multiply together corresponding elements
  if rem(epoch,errorprintfreq)==0
     fprintf(1, 'epoch= %5i , epsilon= %3.5f, E=%6.5f \n',...
                 epoch,       epsilon,        E);
     %% three dots are used to continue the line of matlab code.
  end;

  if E > previousE 
     %% the learning has gone unstable. Restore previous weights.
     epsilon = epsilon*epsilondec;
     inhid     = oldinhid; 
       %pray you dont get here til oldinhid is set (see below)
     hidout    = oldhidout;
     hidbiases = oldhidbiases;
     outbiases = oldoutbiases;
  else epsilon = epsilon*epsiloninc;
     % this gently increases epsilon until the next disaster.
     previousE = E;

     %%%%%% BEGIN BACKWARD PASS %%%%%%
     dEbydoutsum = dEbydoutputs.*outputs.*(1-outputs);
     dEbydhidacts = dEbydoutsum*(hidout');
     dEbydhidsum = dEbydhidacts.*hidacts.*(1-hidacts);
     dEbydoutbiases = sum(dEbydoutsum); 
       % sums the bias derivatives over all cases
     dEbydhidout = hidacts'*dEbydoutsum; 
       %derivatives for hidden-to-output weights
     dEbydhidbiases = sum(dEbydhidsum); 
     dEbydinhid  = data'*dEbydhidsum; 
     %%%%%% END BACKWARD PASS  %%%%%%%
   
     oldinhid     = inhid;
     oldhidout    = hidout;
     oldhidbiases = hidbiases;
     oldoutbiases = outbiases;
   
     %%%%%% UPDATE WEIGHTS AND BIASES %%%%%%
     inhid     = inhid - epsilon*(dEbydinhid + weightcost*inhid);
     hidout    = hidout - epsilon*(dEbydhidout + weightcost*hidout);
     hidbiases = hidbiases - epsilon*dEbydhidbiases;
     outbiases = outbiases - epsilon*dEbydoutbiases;
  end;
  if rem(epoch,weightprintfreq)==0
     figure(1); clf; hinton(hidout,10);
     %clears the figure and displays weights with whole cell area=10.
     figure(2); clf; hinton(inhid',10);
  end;
end;   

 hidsum = data2*inhid + repmat(hidbiases, 1, 1);
    % repmat(M,V,H) replicates M V times Vertically 
    % and H times Horizontally
  hidacts = 1./(1+exp(-hidsum));
    % the ./ makes matlab replicate across all elements of hidsum,
    % so each row of hidacts will be the activity of
    % all hidden units for a particular training case.
  outsum = hidacts*hidout + repmat(outbiases, 1, 1);
  outputs2 = 1./(1+exp(-outsum));
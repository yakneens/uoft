% FOR LEARNING THE WEIGHTS IN A RESTRICTED BOLTZMANN MACHINE.

% ASSUMES DATA, MAXEPOCH, AND NUMHID ARE SET EXTERNALLY.
% THIS PROGRAM USES LOTS OF UNDECLARED GLOBAL VARIABLES.

epsilon      = .2;
weightcost  = 0.0001;
initialmomentum  = 0;
finalmomentum    = 0.9;

[numcases numdims]=size(data);

if restart ==1,
  restart=0;
  epoch=1;
  vishid     = 0.1*randn(numdims, numhid);
  hidbiases  = 0*ones(1,numhid);
  visbiases  = zeros(1,numdims);
  vishidinc  = zeros(numdims,numhid);
  hidbiasinc = zeros(1,numhid);
  visbiasinc = zeros(1,numdims);
end

for epoch = epoch:maxepoch,

   %%%%%%%%% START OF POSITIVE PHASE  %%%%%%%
   poshidprobs = 1./(1 + exp(-data*vishid ...
                             -repmat(hidbiases,numcases,1)));    
   posprods    = data' * poshidprobs;
   poshidact   = sum(poshidprobs);
   posvisact = sum(data);
   %%%%%%%%% END OF POSITIVE PHASE  %%%%%%%

   poshidstates = poshidprobs > rand(numcases,numhid);
   negdata = 1./(1 + exp(-poshidstates*vishid' ...
                         -repmat(visbiases,numcases,1)));

   %%%%%%%%  START NEGATIVE PHASE  %%%%%%%%%
   neghidprobs = 1./(1 + exp(-negdata*vishid ...
                             -repmat(hidbiases,numcases,1)));    
   negprods  = negdata'*neghidprobs;
   neghidact = sum(neghidprobs);
   negvisact = sum(negdata); 
   %%%%%%%%% END OF NEGATIVE PHASE %%%%%%%%

   %%%%%%%%  START WEIGHT AND BIAS UPDATES  %%%%%%%%
   if epoch>5 momentum=finalmomentum; else momentum=initialmomentum; end;
   vishidinc = momentum*vishidinc + ...
               epsilon*( (posprods-negprods)/numcases - weightcost*vishid);
   visbiasinc = momentum*visbiasinc + (epsilon/numcases)*(posvisact-negvisact);
   hidbiasinc = momentum*hidbiasinc + (epsilon/numcases)*(poshidact-neghidact);
   vishid = vishid + vishidinc;
   visbiases = visbiases + visbiasinc;
   hidbiases = hidbiases + hidbiasinc;
   %%%%%%%%  END WEIGHT AND BIAS UPDATES  %%%%%%%%


   diff = sum(sum( (data-negdata).^2 ));
   fprintf(1, 'epoch %5i  recon_diff  %6.2f  \n', epoch, diff); 

   if rem(epoch,100) ==0,
      showrbmweights;  %% this is slow so we do not do it every epoch.
   end;

end;













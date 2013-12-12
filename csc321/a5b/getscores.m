%% ASSUMES TESTDATA IS EXTERNALLY SET.

tiny=exp(-30);

[numcases, numdims]   = size(testdata);

testhidtotalinput = testdata*vishid + repmat(hidbiases,numcases,1);    
testposhidprobs   = 1./(1 + exp(-testhidtotalinput));
testscores = sum(testdata.*repmat(visbiases,numcases,1), 2)  ...
             + sum(testposhidprobs.*testhidtotalinput, 2) ...
             - sum((1-testposhidprobs).*log(tiny+1-testposhidprobs)  ,2) ...
             - sum(testposhidprobs.*log(tiny+testposhidprobs)  ,2);



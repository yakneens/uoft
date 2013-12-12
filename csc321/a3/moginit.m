global train_data;
global valid_data;
global truenumgaussians;

truenumgaussians = 10; 
casespergaussian = 20;
sd		 = .05;


numcases = casespergaussian*truenumgaussians;
train_data = zeros(numcases,2);
valid_data  = zeros(numcases,2);

rand('seed',2);
randn('seed',2);
truecenters = rand(truenumgaussians,2);

for i = 1:truenumgaussians, 
  center =  truecenters(i,:);
  noise = sd *  randn(casespergaussian,2);
  gdata = noise + repmat(center,casespergaussian,1);
  train_data(1+(i-1)*casespergaussian : i*casespergaussian ,:) = gdata(:,:);

  noise = sd *  randn(casespergaussian,2);
  gdata = noise + repmat(center,casespergaussian,1);
  valid_data(1+(i-1)*casespergaussian : i*casespergaussian ,:) = gdata(:,:);
end



global truenumgaussians;
clf;
str=['gx'; 'rx';  'cx'; 'bx'; 'mx'; 'kx'; 'go'; 'ro';  'co'; 'bo'; 'mo'; 'ko'];
hold on
axis([-0.3 1.3 -0.3 1.3]); %sets display size
axis square;
cpg=size(train_data(:,1))/truenumgaussians;

for i=1:truenumgaussians,
  thisdata = train_data(1+(i-1)*cpg:i*cpg,:);
  plot(thisdata(:,1), thisdata(:,2), str(1+rem(i,7),:));
end

for i=1:numgaussians,
  plotellipse(centers(i,1),centers(i,2),sqrt(xvars(i)),sqrt(yvars(i)));
end
hold off
drawnow;











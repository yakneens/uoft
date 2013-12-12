figure(2);
clf;
maxval=max([scoresmodel2data2;scoresmodel2data3;scoresmodel3data2;scoresmodel3data3]);
% if maxval>3000 maxval=3000; end;
minval=min([scoresmodel2data2;scoresmodel2data3;scoresmodel3data2;scoresmodel3data3]);

axis([minval maxval minval maxval]);
axis square;
hold on;
plot(scoresmodel2data3, scoresmodel3data3, 'ro');
hold on;
plot(scoresmodel2data2, scoresmodel3data2, 'bx');
drawnow;


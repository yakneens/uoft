numhid = 5;
maxepoch =  400;
restart = 1;
doitall;

sc221 = scoresmodel2data2;
sc231 = scoresmodel2data3;
sc321 = scoresmodel3data2;
sc331 = scoresmodel3data3;

mySum221 = 0;
mySum231 = 0;
mySum321 = 0;
mySum331 = 0;
for i = 1:200,
    
  mySum221 = mySum221 + sc221(i);
  mySum231 = mySum231 + sc231(i);
  mySum321 = mySum321 + sc321(i);
  mySum331 = mySum331 + sc331(i);
end

avg221 = mySum221 / 200;
avg231 = mySum231 / 200;
avg321 = mySum321 / 200;
avg331 = mySum331 / 200;
%sd32 = std(scoresmodel3data2);

avg1 = (avg231 - avg221)*(avg231 - avg221) + (avg331 - avg321)*(avg331 - avg321);

figure(2);
clf;
maxval=max([sc221;sc231;sc321;sc331]);
% if maxval>3000 maxval=3000; end;
minval=min([sc221;sc231;sc321;sc331]);

axis([minval maxval minval maxval]);
axis square;
hold on;
plot(sc231, sc331, 'ro');
hold on;
plot(sc221, sc321, 'bx');
drawnow;

numhid = 105;
maxepoch =  400;
restart = 1;
doitall;

sc222 = scoresmodel2data2;
sc232 = scoresmodel2data3;
sc322 = scoresmodel3data2;
sc332 = scoresmodel3data3;
mySum222 = 0;
mySum232 = 0;
mySum322 = 0;
mySum332 = 0;
for i = 1:200,
    
  mySum222 = mySum222 + sc222(i);
  mySum232 = mySum232 + sc232(i);
  mySum322 = mySum322 + sc322(i);
  mySum332 = mySum332 + sc332(i);
end

avg222 = mySum222 / 200;
avg232 = mySum232 / 200;
avg322 = mySum322 / 200;
avg332 = mySum332 / 200;
%sd32 = std(scoresmodel3data2);

avg2 = (avg232 - avg222)*(avg232 - avg222) + (avg332 - avg322)*(avg332 - avg322);

figure(3);
clf;
maxval=max([sc222;sc232;sc322;sc332]);
% if maxval>3000 maxval=3000; end;
minval=min([sc222;sc232;sc322;sc332]);

axis([minval maxval minval maxval]);
axis square;
hold on;
plot(sc232, sc332, 'ro');
hold on;
plot(sc222, sc322, 'bx');
drawnow;

sc223 = sc221 + sc222;
sc233 = sc231 + sc232;
sc323 = sc321 + sc322;
sc333 = sc331 + sc332;

sc223 = sc223 ./ 2;
sc233 = sc233 ./2;
sc323 = sc323 ./2;
sc333 = sc333 ./2;
mySum223 = 0;
mySum233 = 0;
mySum323 = 0;
mySum333 = 0;
for i = 1:200,
    
  mySum223 = mySum223 + sc223(i);
  mySum233 = mySum233 + sc233(i);
  mySum323 = mySum323 + sc323(i);
  mySum333 = mySum333 + sc333(i);
end

avg223 = mySum223 / 200;
avg233 = mySum233 / 200;
avg323 = mySum323 / 200;
avg333 = mySum333 / 200;
%sd32 = std(scoresmodel3data2);

avg3 = (avg233 - avg223)*(avg233 - avg223) + (avg333 - avg323)*(avg333 - avg323);

fprintf(1, '%6.2f %6.2f %6.2f \n', avg1, avg2, avg3);

figure(4);
clf;
maxval=max([sc223;sc233;sc323;sc333]);
% if maxval>3000 maxval=3000; end;
minval=min([sc223;sc233;sc323;sc333]);

axis([minval maxval minval maxval]);
axis square;
hold on;
plot(sc233, sc333, 'ro');
hold on;
plot(sc223, sc323, 'bx');
drawnow;
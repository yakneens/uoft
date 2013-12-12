restart=1;
data=train2;
rbmlearn;

testdata=test2;
getscores;
scoresmodel2data2=testscores;


avg22 = mean(scoresmodel2data2);
sd22 = std(scoresmodel2data2);

testdata=test3;
getscores;
scoresmodel2data3=testscores;

avg23 = mean(scoresmodel2data3);
sd23 = std(scoresmodel2data3);

restart=1;
data=train3;
rbmlearn;

testdata=test2;
getscores;
scoresmodel3data2=testscores;
mySum = 0;

avg32 = mean(scoresmodel3data2);
sd32 = std(scoresmodel3data2);

testdata=test3;
getscores;
scoresmodel3data3=testscores;

avg33 = mean(scoresmodel3data3);
sd33 = std(scoresmodel3data3);

%plot23test;




restart=1;
data=train2;
rbmlearn;

testdata=test2;
getscores;
scoresmodel2data2=testscores;

mySum = 0;
for i = 1:200,
    
  mySum = mySum + scoresmodel2data2(i);  
end

avg22 = mySum / 200;
sd22 = std(scoresmodel2data2);

testdata=test3;
getscores;
scoresmodel2data3=testscores;
mySum = 0;
for i = 1:200,
    
  mySum = mySum + scoresmodel2data3(i);  
end

avg23 = mySum / 200;
sd23 = std(scoresmodel2data3);

restart=1;
data=train3;
rbmlearn;

testdata=test2;
getscores;
scoresmodel3data2=testscores;
mySum = 0;
for i = 1:200,
    
  mySum = mySum + scoresmodel3data2(i);  
end

avg32 = mySum / 200;
sd32 = std(scoresmodel3data2);

testdata=test3;
getscores;
scoresmodel3data3=testscores;
mySum = 0;
for i = 1:200,
    
  mySum = mySum + scoresmodel3data3(i);  
end

avg33 = mySum / 200;
sd33 = std(scoresmodel3data3);

plot23test;




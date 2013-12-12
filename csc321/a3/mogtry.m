x=[];
y=[];
z=[];
Num=[];

for numgaussians=1:20,
  moginit;
  result=mogem(150,numgaussians,1,0);
  x(numgaussians)=result(1);
  y(numgaussians)=result(2);
  z(numgaussians)=result(3);
Num=[Num,numgaussians];
end

plot(Num,x,'b-o',Num,y,'r-*');
legend('Validation','Training');
%plot(Num,z,'o');
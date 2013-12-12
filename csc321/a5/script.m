load assignment5data;

[x,y] = meshgrid([10:50:410],[1:10:201]);
res = zeros(9,20);
iind = 1;
jind = 1;
for maxepoch = 10:50:410,
jind = 1;
fprintf(1,'Maxepoch %i \n', maxepoch);
    for numhid = 1:10:201,
	fprintf(1, 'Numhid %i \n', numhid);
        restart = 1;
        doitall;
        res(iind,jind) = (avg23 - avg22)*(avg23 - avg22) + (avg33 - avg32)*(avg33 - avg32);
        jind = jind+1;
    end
    
    iind = iind + 1;
end
save('data.mat','res','x','y');

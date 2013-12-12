

[x,y] = meshgrid([10:50:410],[1:10:201]);
result = zeros(9,20);
i = 1;
j = 1;
for maxepoch = 10:50:410,
j = 1;

    for numhid = 1:10:201,
        restart = 1;
        %numhid = 32;
        %maxepoch = 60;

        doitall;
    result(i,j) = (avg23 - avg22)*(avg23 - avg22) + (avg33 - avg32)*(avg33 - avg32);
    j = j+1;
    end
    
    i = i + 1;
end

%fprintf(1, '%6.2f %6.2f \n', avg23, sd23);
%fprintf(1, '%6.2f %6.2f \n', avg33, sd33);
%fprintf(1, '%6.2f %6.2f \n', avg22, sd22);
%fprintf(1, '%6.2f %6.2f \n', avg32, sd32);
%num = (avg23 - avg22)*(avg23 - avg22) + (avg33 - avg32)*(avg33 - avg32);
%fprintf(1, '%6.2f \n', num);
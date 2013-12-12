%for showing weights of hidden units.
%assume each row of data is a SQUARE image.

cols   = 10;
rows   = ceil(numhid/cols);


width = sqrt(size(data,2));

figure(1);
clf;

for h = 1:numhid;
  col = 1+ rem(h-1,cols);
  row = ceil( (numhid+1-h)/cols);
  rect = [(col-.95)/cols, (row-.95)/rows, 1/(cols*1.1), 1/(rows*1.1)];
  axes('position', rect); 
  show(reshape(vishid(:,h)', width, width)', -3, 3);
end;

drawnow;

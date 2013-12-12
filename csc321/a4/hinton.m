function maxWeight = hinton(W, maxWeight, linesPerWeight);

[height,width] = size(W);

if nargin<3,
  linesPerWeight=ceil(max(min(10,200/height),2));
end;
if nargin<2,
  maxWeight=2^ceil(log(max(abs(W(:))))/log(2));
end;
 
% fprintf('Lines: %g \n',linesPerWeight);
% fprintf('Weights ranging from %g to %g\n',-maxWeight,maxWeight);

nhor  = 1 + linesPerWeight*height;
nvert = 1 + linesPerWeight*width;
spacing = 1/linesPerWeight;

% column vectors of first  matrix are initial and final x coords of line.
% column vectors of second matrix are initial and final y coords of line.

fill([0 width width 0],[0 0 height height],0.5*[1 1 1]); %0.65
axis('equal');
axis('off');
hold on
% plot([0;width]*ones(1,nhor), [1;1]*(0:spacing:height),'-','Color',0.5*[1 1 1],'LineWidth',0.1); 

% plot([1;1]*(0:spacing:width), [0;height]*ones(1,nvert), 'w:');

for x = 1:width,
   for y = 1:height,
     w=W(y,x);
     if w> 0,
       blob(x-0.5, height - y + 0.5, min(1,w/maxWeight),0.999*[1 1 1]);
     elseif w < 0,
       blob(x-0.5, height - y + 0.5, min(1,-w/maxWeight),0.01*[1 1 1]);
     end;
   end;
end;

axis('equal');
axis('off');

hold off




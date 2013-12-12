% function show(X,cmin,cmax);
% Shows a matrix as grayscale values, scaled by cmin and cmax (optional)
% 
% show(X)            - autoscales
% show(X,cmin)       - scales (-cmin,cmin)
% show(X,cmin,cmax)  - scales (cmin,cmax)
%

function show(X,cmin, cmax);
  
[n,m]=size(X);

Y=[rot90(X') zeros(n,1);zeros(1,m+1)];

% pcolor(Y);

cax = newplot;
next = lower(get(cax,'NextPlot'));
hold_state = ishold;
hh = surface(zeros(size(Y)),Y);
lims = [ 1 m+1 1 n+1];
set(cax,'View',[0 90]);
set(cax,'Box','on');
axis(lims);
colormap('gray');
shading flat
axis('off');
axis('image');
hold off;

if nargin==3,
  caxis([cmin cmax]);
elseif nargin==2,
  caxis([-cmin cmin]);
end;

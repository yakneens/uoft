function [] = plotellipse(x,y,rx,ry);

hold on;

theta = 0 :.05: 2*pi;
plot(x+rx*sin(theta),y+ry*cos(theta),'k');






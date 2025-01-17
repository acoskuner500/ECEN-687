clear; clc;
c=0.2;
r=0.1;
C = [1 2 1 2 2 1 2 1];

x1 = 0.608;
D1 = 8*x1*r*(C(8) + 8*x1*c/2);

u1 = [5, 17-8*x1];
u2 = [10, 2+8*(1-x1)];
u3 = [17, 2+8*(1-x1)];
u4 = [20-8*x1, 14];

C = zeros(1,4) + 0.8;
em12 = sum(abs(u1-u2))/2;
u5 = [u2(1), u2(2)+em12];
em34 = sum(abs(u3-u4))/2;
u6 = [u3(1), u3(2)+em34];

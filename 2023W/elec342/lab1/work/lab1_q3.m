% Andre Hei Wang Law
% 4017 5600
% Lab 1 Question 3

clear
% i) X[n] is periodic if X[n+T]=x[n]
n1 = 1
xi1 = cos((2.*pi)/n1);                 % n = 1

n2 = 1+1024
xi2 = cos((2.*pi)/n2);               % n = 1 + 1024

format long
y1 = xi1-xi2;
display(y1);

% ii) Same as (i), bu we use 3.14 rather than pi
n1 = 1
xii1 = cos((2.*3.14)/n1);                 % n = 1

n2 = 1+1024
xii2 = cos((2.*3.14)/n2);               % n = 1 + 1024

format long
y2 = xii1-xii2;
display(y2);

% iii) Determine if two signals are Equal
niii = 1
xiii1 = cos(pi./4.*niii+pi./3);                

xiii2 = cos(9.*pi./4.*niii+pi./3);               

format long
y3 = xiii1-xiii2;
display(y3);






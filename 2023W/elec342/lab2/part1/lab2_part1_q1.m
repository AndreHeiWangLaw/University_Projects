% Andre Hei Wang Law
% 4017 5600
% lab 2, Part 1, Question 1

% Part a)
clear
clc

% define n
n = [0 : 9];
% define the input x1[n] 
x = n;
% define the response y1[n]
y = x.^2;

for i = 0 : 9
   x = x + x.^2;
   y = y + y.^2;
end

disp(x)
disp(y)



% Part b)
clear
clc

% define n
n = [0 : 9];
% define the input x1[n] = n
x = sin((2*pi)./(10*n));
% define the response y1[n] = 2* x1[n]
y = x.^2;

for i = 0 : 9
   x = x + x.^2;
   y = y + y.^2;
end

disp(x)
disp(y)
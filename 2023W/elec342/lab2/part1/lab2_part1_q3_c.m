% Andre Hei Wang Law
% 4017 5600
% lab 2, Part 1, Question 3

% part c)
clc
clear
n = [1 : 20 ]
x1 = sin((2*pi/40) * n) .* cos((2*pi/40) * n)
for index = 1 : 20
% Note: In MATLAB, no need to pre-allocate the array,
% unlike C++ and other high-level programming languages.
 x2(index) = sin((2*pi/40) * index) * cos((2*pi/40) * index)
end
subplot(2,1,1)
stem(n, x1)
title("Elegant method making full use of MATLABs array capabilities")
xlabel("n")
ylabel("x[n]")
subplot(2,1,2)
stem(n, x2)
title("Gets the job done, but it is a lot of work and we are not in the MATLAB mindset")
xlabel("n")
ylabel("x[n]")

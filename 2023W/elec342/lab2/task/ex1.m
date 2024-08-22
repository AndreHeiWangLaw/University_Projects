% Andre Hei Wang Law
% 4017 5600
% Example 1
% for loop

% finds the sum of the integers from 1 to 10

clc;
clear;

sum = 0; % initialize running sum

for i = 1 : 10
    sum = sum + i;
end

disp("The sum is = ") % output a descriptive string
disp(sum) % followed by the value of sum
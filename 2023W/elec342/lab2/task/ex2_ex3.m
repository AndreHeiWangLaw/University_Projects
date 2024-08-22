% Andre Hei Wang Law
% 4017 5600
% Example 2 and 3
% for loop

clc;
clear;
sum = 0; % initialize sum to 0
for index = 1 : 5 % set up the loop index
    square = index ^ 2; % compute square of current loop index and
    sum = sum + square; % add it to the running sum
end
disp("The sum of the squares of 1 to 5 is")
disp(sum)

clear;
x = [ 1 : 5 ]; % intialize the x array
y = x .^2 ; % compute the squares of the elements in the
 % x array
sum = 0;
for index = y % loop through every element in the y array
    sum = sum + index; % and add each element to sum
end
disp("The sum of the squares of 1 to 5 rewritten more concisely using array is")
disp(sum)
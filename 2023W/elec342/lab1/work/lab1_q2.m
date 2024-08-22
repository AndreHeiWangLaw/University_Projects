% Andre Hei Wang Law
% 4017 5600
% Lab 1 Question 2

clear
subplot(2, 2, 1)
x = -5 : 4
vec1 = zeros(1,10)      %size of array
vec1(2) = 1             % 2nd element will be 1
vec1(3) = 1             % 3rd element will be 1
stem(x, vec1)
title("Stem Plots of First Vector")
xlabel("X-axis from -5 to 4")
ylabel("Zeros Except Two Chosen Elements")

subplot(2, 2, 2)
x = -5 : 4
vec1 = ones(1,10)      %size of array
n = 4                  % arbitrary number
vec1(1:n) = 0
stem(x, vec1)
title("Stem Plots of Second Vector")
xlabel("X-axis from -5 to 4")
ylabel("First 'n' elements being 0, remainder all ones")

subplot(2, 2, 3)
x = 0 : 10
vec1 = zeros(1,11)      % size of array
vec1(4) = 1             % 4nd element will be 1
vec1(5) = 1             % 5rd element will be 1
stem(x, vec1)
title("Stem Plots of First Vector")
xlabel("X-axis from 0 to 10")
ylabel("Zeros Except Two Chosen Elements")

subplot(2, 2, 4)
x = 0 : 10
vec1 = ones(1,11)      % size of array
n = 7                  % arbitrary number
vec1(1:n) = 0
stem(x, vec1)
title("Stem Plots of Second Vector")
xlabel("X-axis from 0 to 10")
ylabel("First 'n' elements equal to 0, remainder all ones")

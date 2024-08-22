% Andre Hei Wang Law
% 4017 5600
% Example 2
% Properties of Signals and Systems
% Odd or Even of Unit Step Function (Negative array Indices)

% determines if a signal x[n] = function(n) is even or odd
% x[n] is EVEN is x[-n] = x[n]
% x[n] is ODD if x[-n] = -x[n]
% define n over the interval -4 to +4
n = [-4 : 4 ]
% define x1[n]
x1 = ones(1,9)
x1(1:4) = 0

% define x2[n] = x1[-n]
x2 = zeros(1,9)
% manually assign the values since
% MATLAB does not allow for negative array indices
% what we want to do is:
% x2[n] = x1[-n] for n in -4, -3, -2, -1, 0, 1 ,2 3, 4
% x2[-4] = x2[4]
% x2[-3] = x1[3]
% etc
% x2[3] = x1[-3]
% x2[4] = x1[-4]
% but MATLAB does not allow for negative array indices so we
% do the following instead:
 for index = 1 : 9
    x2(index) = x1(10 -index);
 end
 
% plot the two signals
subplot(4,1,1)
stem(n,x1)
ylabel(" x[n] " )
xlabel(" n " )
subplot(4,1,2)
stem(n,x2)
ylabel(" x[-n] ")
xlabel(" n " )

% check for even/odd
% since working with integers values, we can compare for direct equality
if ( x2 == x1 )
    disp("EVEN")
elseif ( x2 == (-x1) )
    disp("ODD")
else
    disp("NEITHER EVEN NOR ODD")
end

% decompose the signal into its
% even and odd components
% using the definitions
% even component = (1/2) * ( x[n] + x[-n])
% odd component = (1/2) * ( x[n] - x[-n])
even_comp = (1/2) * ( x1 + x2 )
odd_comp = (1/2) * ( x1 - x2 )

% plot the even and odd components as a
% visual verification that the even component is indeed even
% and the odd component is indeed odd
subplot(4,1,3)
stem(n, even_comp)
xlabel(" n ")
ylabel(" Even component of x[n] ")
subplot(4,1,4)
stem(n, odd_comp)
xlabel(" n ")
ylabel("Odd component of x[n] ")

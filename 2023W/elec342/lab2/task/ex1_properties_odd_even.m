% Andre Hei Wang Law
% 4017 5600
% Example 1
% Properties of Signals and Systems
% Odd or Even

clc;
clear;

% determines if a signal x[n] = function(n)
% is even or odd
% x[n] is EVEN is x[n] = x[-n]
% x[n] is ODD if x[n] = -x[-n]
% define n
n = 0 : 9
% define x1[n]
x1 = sin( 2*pi/10 * n )
subplot(1,2,1)
stem(n, x1)
% define x2 = x1[-n]
x2 = sin( 2*pi/10 * (-n) )
subplot(1,2,2)
stem(-n, x2)
if ( x2 == x1 )
    disp("EVEN")
elseif ( x2 == (-x1) )
    disp("ODD")
else
    disp("NEITHER EVEN NOR ODD")
end
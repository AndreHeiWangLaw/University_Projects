% Andre Hei Wang Law
% 4017 5600
% lab 2, Part 1, Question 3

% part a)
clc
clear
% define n
n = [0:10]
% define the input x1[n] 
format long
x1 = exp(-2*abs(n)).*sin((2.*pi./36).*n);
% define the input x2[n] 
x2 = x1;

% x[-n] input
for index = 1 : 11
    x2(index) = x1(12 -index);
end

subplot(4,1,1)
stem(n,x1)      % first plot
ylabel(" x[n] " )
xlabel(" n ")

subplot(4,1,2)
stem(n,x2)      % second plot
ylabel(" x[-n]")
xlabel(" n ")

if ( x2 == x1 )
    disp("EVEN")
elseif ( x2 == (-x1) )
    disp("ODD")
else
    disp("NEITHER EVEN NOR ODD")
end

% decompose the signal into its even and odd components
% using the definitions even component = (1/2) * ( x[n] + x[-n])
% odd component = (1/2) * ( x[n] - x[-n])
even_comp = (1/2) * ( x1 + x2 );
odd_comp = (1/2) * ( x1 - x2 );
% plot the even and odd components as a visual verification that the 
% even component is indeed even and the odd component is indeed odd
subplot(4,1,3)
stem(n, even_comp)      % third plot
xlabel(" n " )
ylabel(" Even component of x[n] ")

subplot(4,1,4)
stem(n, odd_comp)       % fourth plot
xlabel(" n " )
ylabel(" Odd component of x[n] ")


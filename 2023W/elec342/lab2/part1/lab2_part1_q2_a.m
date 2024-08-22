% Andre Hei Wang Law
% 4017 5600
% lab 2, Part 1, Question 2

% part a)
clc
clear

% define n
n = [ 0 : 10 ]
% define the input x1[n] 
x1 = sin(((2*pi)./10)*n);
% define input x2[n] 
x2 = sin(((2*pi)./10)*n);
% define the response y1[n] 
y1 = 2 * x1;
% define the response y2[n] 
y2 = 2 * x2;

% define x3[n] = A*x1[n] + B*x2[n] where A = B = 1 
x3 = x1 + x2;
% define the response y3[n] 
y3 = 2 * x3
% define the response y4[n] 
y4 = y1 + y2

% Check if y3[n] = y1[n] + y2[n]
if ( abs(y4 - y3) <= 0.0001 )
    disp( "Outputs are consistent with a linear system")
else
    disp( "System is not linear")
end
        
hold on     % command so plot remains on same graph
stem(n,y3)  % output signal plot
stem(n,x3)  % input signal plot

title("Stem Plots of all input and output signals")
xlabel("Interval from 0 to 10")
ylabel("Input and Output Signals")
hold off


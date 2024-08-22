% Andre Hei Wang Law
% 4017 5600
% Example 1
% Properties of Signals and Systems
% Linearity

% determines if the given system y[n] = 2 * x[n] 
% produces outputs consistent with a linear system

clc
clear
% define n
n = [ 0 : 4 ]
% define the input x1[n] = n
x1 = n
% define input x2[n] = 2* n
x2 = 2 * n
% define the response y1[n] = 2* x1[n]
y1 = 2 * x1
% define the response y2[n] = 2 * x2[n]
y2 = 2 * x2
% define x3[n] = A*x1[n] + B*x2[n]
% for simplicity we make A = B = 1 in this example
x3 = x1 + x2 ;
% define the response y3[n] = 2 * x3[n]
y3 = 2 * x3
% Check if y3[n] = y1[n] + y2[n]
if ( y3 == ( y1 + y2 ) )
    disp( "Outputs are consistent with a linear system")
else
    disp( "System is not linear")
end
% stem plots of the responses may also be obtained.
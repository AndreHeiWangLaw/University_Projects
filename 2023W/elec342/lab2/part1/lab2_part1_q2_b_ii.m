% Andre Hei Wang Law
% 4017 5600
% lab 2, Part 1, Question 1

% Part b) 
% ii) y[n] = 2x[n] + 5δ[n]
% small set of values

clc
clear

% define the input x1[n] 
x1 = [0,1];
% define input x2[n] 
x2 = [0,1];
% define the response y1[n] 
y1 = 2*x1 + 5*x1;
% define the response y2[n] 
y2 = 2*x2 + 5*x2;

% define x3[n] = A*x1[n] + B*x2[n] where A = B = 1 
x3 = x1 + x2;
% define the response y3[n] 
y3 = x3.^2
% define the response y4[n] 
y4 = y1 + y2

% Check if y3[n] = y1[n] + y2[n]

if ( abs(y4 - y3) <= 0.0001 )
    disp( "Outputs are consistent with a linear system")
else
    disp( "System is not linear")
end
        

% large set of values

% define the input x1[n] 
x1 = [0:10];
% define input x2[n] 
x2 = [0:10];
% define the response y1[n] 
y1 = 2*x1 + 5*x1;
% define the response y2[n] 
y2 = 2*x2 + 5*x2;

% define x3[n] = A*x1[n] + B*x2[n] where A = B = 1 
x3 = x1 + x2;
% define the response y3[n] 
y3 = x3.^2
% define the response y4[n] 
y4 = y1 + y2

% Check if y3[n] = y1[n] + y2[n]
if ( abs(y4 - y3) <= 0.0001 )
    disp( "Outputs are consistent with a linear system")
else
    disp( "System is not linear")
end
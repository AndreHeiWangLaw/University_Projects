% Andre Hei Wang Law
% 4017 5600
% Example 4
% if statement

clc;
clear
x = 0.1 + 0.1 + 0.1
if ( x == 0.3)
    disp( " x is equal to 0.3")
else
    disp( " x is not equal to 0.3")
end

clear
x = 0.1 + 0.1 + 0.1
if ( abs( x - 0.3 ) <= 0.0001)
    disp( " x is equal to 0.3")
else
    disp( " x is not equal to 0.3")
end
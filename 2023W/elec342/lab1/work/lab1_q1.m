% Andre Hei Wang Law
% 4017 5600
% Lab 1 Question 1

clear
subplot(1, 2, 1)

r = 1 : 10                  % radius from 1 to 10
vol = (4/3).*pi.*(r.^3)     % volume of sphere
plot(r, vol)                % plot of volume vs radius

hold on
sur = 4.*pi.*(r.^3)         % surface area of a sphere
plot(r, sur)                % plot of surface vs radius
hold off
title("Volume (blue) and Surface (red) of a Sphere as a Function of it's Radius")
xlabel("Radius")
ylabel("Volume and Surface Area of a Sphere")

subplot(1, 2, 2)
r = 1 : 10                  % radius from 1 to 10
vol2 = r.^3                 % volume of square
plot(r, vol2)               % plot of volume vs radius

hold on
sur2 = 6.*(r.^2)            % surface area of a sphere
plot(r, sur2)               % plot of surface vs radius
hold off
title("Volume (blue) and Surface (red) of a Square as a Function of it's Radius")
xlabel("Radius")
ylabel("Volume and Surface Area of a Square")



function simulations
[t, x] = ode45(@deq, [0:0.1:100], [0 0]);
plot(t,x(:,1),'.')

M = zeros(size(t,1), size(t,2)+size(x,2));
M(:,1) = t;
M(:,2:end) = x;

dlmwrite('VarsForIdentToolbox.txt', M, 'delimiter', '\t');
end

function dx = deq(t,x)
k = 1.;
m = 1.;
F = 1.5;
dx = zeros(size(x));
% xdot
dx(1) = x(2);
% vdot
dx(2) = - k/m * x(1) + F/m;

end
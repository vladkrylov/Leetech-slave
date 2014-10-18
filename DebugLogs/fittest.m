function fittest()
t = [1985 1991 1994 1997 2002];
y = [12 91 145 192 224];

F = @(x,xdata) x(1) * log(x(2) * (xdata - x(3)));
x0 = [1 1 1];
[x,resnorm,~,exitflag,output] = lsqcurvefit(F,x0,t,y)

figure(1)
hold on
    plot(t, y, 'ro')
%     plot(t, F(x,t))
hold off

    
end
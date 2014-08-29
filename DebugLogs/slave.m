function slave()
in = load('TimeCoords.log');
params = ReadYaml('Params.yaml');

dest = params.dest * 0.488;

times = in(:,1);
coords = in(:,2);

times = [times(1); times(times ~= 0)];
coords = coords(coords ~= 0) * 0.488;
len = length(times);

figure(1)
title('Time-Coordinate plot')
xlabel('t, \mus')
ylabel('Coordinate, \mum')
hold on
    plot(times, coords, 'b*-');
    plot( linspace(min(times), max(times), len), dest*ones(len), 'r--');
hold off

end

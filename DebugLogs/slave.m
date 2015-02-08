function slave(yaml)
%% Function for debugging LEETECH motors moving

%% Load the data
if nargin > 0
    yamlFile = yaml;
else
    yamlFile = 'Data.yaml';
end

[times, u, coords, dest, stopInd] = getData(yamlFile);
len = length(coords);

coords_pulses = coords(1:stopInd);
times_pulses = times(1:stopInd);

coords_inertion = coords(stopInd+1:end);
times_inertion = times(stopInd+1:end);

%% Plot the data
% figure(1)
figure('units','normalized','outerposition',[0 0 1 1])
subplot(2,1,1)
title('\bf Coordinate of motor')
xlabel('t, ms')
ylabel('Coordinate, \mum')
hold on
    plot(times_pulses, coords_pulses, 'bo-');
    plot( linspace(min(times), max(times), len), dest*ones(len), 'r--');
    plot(times_inertion, coords_inertion, 'g*');

hold off

subplot(2,1,2)

plot(times, u, '.-');
title('{\bfDriven signal - width of pulse} (1 unit = 0.025\mus)')
xlabel('t, ms')
ylabel('Driven signal, units')

end

function res = to_um (arg1)
    res = arg1 * 0.488;
end

function pulseWidthus = pulse2us(pulseWidth)
    pulseWidthus = 0.025 * pulseWidth;
end

function [t, u, x, destination, stopIndex] = getData(yaml)
    params = ReadYaml(yaml);

    destination = to_um(params.dest);
    stopIndex = params.pulsesStopIndex;

    t = cell2mat(params.t) / 100;
    u = cell2mat(params.u);
    x = to_um( cell2mat(params.x) );
end





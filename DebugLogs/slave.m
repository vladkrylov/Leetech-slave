function slave()
in = load('TimeCoords.log');
params = ReadYaml('Params.yaml');

dest = to_um(params.dest);
final = to_um(params.final);
stopInd = params.pulsesStopIndex;

times = in(:,1) / 100;
disp( diff(times) );
coords = to_um( in(:,2) );

coords_pulses = coords(1:stopInd);
times_pulses = times(1:stopInd);

coords_inertion = coords(stopInd+1:end);
times_inertion = times(stopInd+1:end);

len = length(times);

figure(1)
title('Time-Coordinate plot')
xlabel('t, ms')
ylabel('Coordinate, \mum')
hold on
    plot(times_pulses, coords_pulses, 'bo-');
    plot( linspace(min(times), max(times), len), dest*ones(len), 'r--');
    plot(times_inertion, coords_inertion, 'g*');
hold off

% PrintInertionData( (coords_pulses(end) - coords_pulses(end-1)) / (times(end) - times(end-1)) , ...
%                             final - coords_pulses(end) );

end

function res = to_um (arg1)
    res = arg1 * 0.488;
end

function PrintInertionData(speed, deviation)
    fileID = fopen('InertionData.txt', 'a');
    fprintf ( fileID, '%f\t%f\n', speed, deviation );
    fclose(fileID);
end



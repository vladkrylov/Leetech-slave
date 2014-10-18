function slave()
in = load('TimeCoords.log');
params = ReadYaml('Params.yaml');

dest = to_um(params.dest);
final = to_um(params.final);
stopInd = params.pulsesStopIndex;

coords = to_um( in(:,2) );
len = length(coords);
% times = [0 : 0.653 : 0.653*(len-1)];
times = in(:,1) / 100;
% disp( diff(times) );

coords_pulses = coords(1:stopInd);
times_pulses = times(1:stopInd);

coords_inertion = coords(stopInd+1:end);
times_inertion = times(stopInd+1:end);

figure(1)
title('Time-Coordinate plot')
xlabel('t, ms')
ylabel('Coordinate, \mum')
hold on
    plot(times_pulses, coords_pulses, 'bo-');
    plot( linspace(min(times), max(times), len), dest*ones(len), 'r--');
    plot(times_inertion, coords_inertion, 'g*');
    
    % fitted plot
    plot(times_pulses, fitmodel(times_pulses, coords_pulses, coords_pulses(1)), 'r.')
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

function res = fitmodel(t, x, x0)
% x(1) = beta
% x(2) = gamma
F = @(pars,xdata) x0 + pars(1) * (xdata - 1/pars(2) * (1 - exp( -pars(2) * xdata) ));
x_init = [1 1];
[xfitted,resnorm,~,exitflag,output] = lsqcurvefit(F,x_init,t,x)
res = F(xfitted, t);

% FittedCoeficientsFile = fopen('FittedCoeficients.txt','a');
% fprintf(FittedCoeficientsFile,'%6.2f\t%6.4f\n', xfitted(1), xfitted(2));
% fclose(FittedCoeficientsFile);

end

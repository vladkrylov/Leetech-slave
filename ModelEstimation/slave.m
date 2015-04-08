function [coords, u, times] = slave(yamlFile, plotOpt)
%% Function for debugging LEETECH motors moving

%% Load the data
if nargin < 2
    plotOpt =0;
    if nargin < 1
        yamlFile = 'Data.yaml';
    end
end

[times, u, coords, dest, stopInd] = getData(yamlFile);
times = [times(1) times(times ~= 0)];
len = length(times);
u = u(1:len);
coords = coords(1:len);

coords_pulses = coords(1:stopInd);
times_pulses = times(1:stopInd);

coords_inertion = coords(stopInd+1:end);
times_inertion = times(stopInd+1:end);

%% Plot the data
if (plotOpt ~= 0)
%     figure(1)
    close all
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

end

function res = to_um (arg1)
    res = arg1 * 0.488;
end

function pulseWidthus = pulse2us(pulseWidth)
    pulseWidthus = 0.025 * pulseWidth;
end

function [t, u, x, destination, stopIndex] = getData(yaml)
    params = ReadYaml(yaml);

%     destination = to_um(params.dest);
%     stopIndex = params.pulsesStopIndex;
    destination = 0;

    t = cell2mat(params.t) / 100;
    u = cell2mat(params.u);
    x = to_um( cell2mat(params.x) );
    
    stopIndex = length(u(u ~= 0));
    destination = x(end);
end

function savePreviousData(yaml)
    params = ReadYaml(yaml);
    u = cell2mat(params.u);
    
    motorFolder = [params.Set_id, 'Motor', num2str(params.Motor_id)];
    dataFileName = ['U(0)=', num2str(u(1)),'_Dest=', num2str(params.dest),...
                             '_StopAt=', num2str(params.pulsesStopIndex),...
                             '.yaml'];
    mkdir(motorFolder);
    copyfile(yaml, [motorFolder, '/', dataFileName]);
end



function newYaml = saveData(yaml)
try
    params = ReadYaml(yaml);
    u = cell2mat(params.u);
    
    motorFolder = [params.Set_id, 'Motor', num2str(params.Motor_id)];
    dataFileName = ['U(0)=', num2str(u(1)),'_Dest=', num2str(params.dest),...
                             '_StopAt=', num2str(params.pulsesStopIndex),...
                             '.yaml'];
    newYaml = [motorFolder, '/', dataFileName];
    mkdir(motorFolder);
    copyfile(yaml, newYaml);
catch exception
    disp('Error...');
end

end
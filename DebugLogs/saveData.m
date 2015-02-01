function saveData(yaml)
    params = ReadYaml(yaml);
    u = cell2mat(params.u);
    
    motorFolder = [params.Set_id, 'Motor', num2str(params.Motor_id)];
    dataFileName = ['U(0)=', num2str(u(1)),'_Dest=', num2str(params.dest),...
                             '_StopAt=', num2str(params.pulsesStopIndex),...
                             '.yaml'];
    mkdir(motorFolder);
    copyfile(yaml, [motorFolder, '/', dataFileName]);
end
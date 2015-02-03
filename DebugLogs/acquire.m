function acquire
yaml = 'Data.yaml';
log = 'MatlabSignalFile.log';
last_modification_time = getModificationTime(log);

while 1
    pause(0.1)
    modification_time = getModificationTime(log);
    if modification_time > last_modification_time
        new_yaml = saveData(yaml);
        close all;
        slave(new_yaml);
        last_modification_time = modification_time;
    end
end

end

% last modification time in seconds
function time = getModificationTime(filename)
    f = dir(filename);
    last_timestamp = regexp(f.date,'\s','split');
    t = regexp(last_timestamp(2),':','split');
    t = t{1,1};
    time = str2num(cell2mat(t(1)))*60*60 + str2num(cell2mat(t(2)))*60 + str2num(cell2mat(t(3)));
end
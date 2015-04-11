folder = 'Keil_Debug_data/';
listing = dir(fullfile(folder,'*.yaml'));
L = length(listing);

data_file = matfile('trajectories_ex1.mat','Writable',true);
good_counter = 0;

for i = 1:L
   [t, u, x] = slave([folder '/' listing(i).name], 1);
   fprintf('plot of file %s\n', listing(i).name)
%    input('Press any key');
   good = 'y';
   if strcmp(good, 'y')
       good_counter = good_counter + 1;
       s(good_counter) = pack_trajectory(t, u, x);
   end
%     params = ReadYaml([folder '/' listing(i).name]);
%     t = cell2mat(params.t) / 100;
%     u = cell2mat(params.u);
%     x = cell2mat(params.x);
%     plot(t,x)
end
% data_file.trajectories = s;
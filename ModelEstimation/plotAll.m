folder = 'Keil_Debug_data/';
listing = dir(fullfile(folder,'*.yaml'));
L = length(listing);

% good_list = zeros(1,L);
% 
%data_file = matfile('trajectories_new.mat','Writable',true);
%good_counter = 0;

for i = 1:L
    [t, u, x] = slave([folder '/' listing(i).name], 1);
%    good = input('Is this a good data?\n', 's');
%     good = 'y';
%    if strcmp(good, 'y')
%         good_list(i) = 1;
%        good_counter = good_counter + 1;
%        s(good_counter) = pack_trajectory(t, u, x);
%    end
end

%data_file.trajectories = s;




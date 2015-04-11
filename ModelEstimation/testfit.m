function testfit

folder = 'Keil_Debug_data/';
listing = dir(fullfile(folder,'*.yaml'));
L = length(listing);

us = zeros(1,L);
gs = zeros(1,L);
fs = zeros(1,L);

k = 0.5;
N_iter = 10;
for iter=1:N_iter
    if iter == 1
        lower = [0, 0, 0];
        upper = [4096, Inf, Inf];
        start = [2075, 1.69e-3, 1e-3];
    else
        lower = [0, 0, mean(gs) - k/iter^2*rms(gs)];
        upper = [4096, Inf, mean(gs) + k/iter^2*rms(gs)];
        start = [2075, mean(fs), mean(gs)];
    end
    
    
    for i=1:L
        [tb, ub, xb] = slave([folder '/' listing(i).name]);
        motion_inds = find(ub ~= 0);
        tm = tb(motion_inds);
        xm = xb(motion_inds);


        model = 'a-b*(1-exp(-2*x*c)-2*x*c)/(4*c^2)';
        f1 = fit(tm', xm', model, 'Start', start,...
            'Lower', lower, 'Upper', upper);
        us(i) = ub(1);
        fs(i) = f1.b;
        gs(i) = f1.c;

%         figure(10)
%         disp(us(i))
%         plot(f1, tm, xm)
    %     input('Press any key');
    end
%     figure(1)
%     plot(us, fs, 'o')

%     figure(2)
%     plot(us, gs, 'o')
%     input('Press any key');
end

us = [0, us];
fs = [0, fs];


figure(1)
plot(us, fs, 'o')

data_file = matfile('force.mat','Writable',true);
data_file.force = fs;
data_file.usignal = us;

end
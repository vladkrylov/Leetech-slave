folder = '/home/vlad/Leetech/build-one_motor_raw_ss-Desktop_Qt_5_4_0_GCC_64bit-Debug/';
listing = dir(fullfile(folder,'*.yaml'));
L = length(listing);
ord = 2;
% as = zeros(ord, ord, L);
% bs = zeros(ord, 1, L);
% cs = zeros(1, ord, L);

Ts = 1;

xc = cell(1,L);
uc = cell(1,L);
tc = cell(1,L);

sys0 = idtf([NaN], [1 -2.885 NaN], Ts,'ioDelay',1);

sys0.Structure.ioDelay.Free = true;
% sys0.Structure(1).ioDelay.Maximum = 7;

sys0.Structure.num.Minimum = [-Inf];
sys0.Structure.num.Maximum = [Inf];
sys0.Structure.num.Free = [1];

sys0.Structure.den.Minimum = [1, -Inf, -Inf];
sys0.Structure.den.Maximum = [1, Inf, Inf];
sys0.Structure.den.Free = [0, 1, 1];

for i = 1:L
    [x, u, t] = slave([folder '/' listing(i).name], 0);
    [xmax, max_ind] = max(x);
    xc{i} = [x(1)*ones(1,100) x(1:max_ind) x(max_ind)*ones(1,200)];
    uc{i} = [0*ones(1,100) u zeros(1,length(xc{i}) - length(u) - 100)];
    tc{i} = Ts*[1:length(xc{i})];
    
%     xc{i} = [x(1:max_ind) x(max_ind)*ones(1,200)];
%     uc{i} = [u zeros(1,length(xc{i}) - length(u))];
%     tc{i} = Ts*[1:length(xc{i})];

%     
%     
%     as(:,:,i) = sys.a;
%     bs(:,:,i) = sys.b;
%     cs(:,:,i) = sys.c;
end

x = xc{1};
u = uc{1};
data = iddata((x - x(1))', u', Ts);
for i=2:L
    x = xc{i};
    u = uc{i};
    data = merge(data, iddata((x - x(1))', u', Ts));
end

opt = tfestOptions('InitialCondition', 'zero','Focus','prediction');
sys = tfest(data,sys0,opt);
mypid = pidtune(sys, 'pi')

% figure(1)
% subplot(2,1,1)
% plot(tc{1}, xc{1})
% hold on
% 
% subplot(2,1,2)
% plot(tc{1}, uc{1})
% hold on
% 
% for i=2:L
%     subplot(2,1,1)
%     plot(tc{i}, xc{i})
%     
%     subplot(2,1,2)
%     plot(tc{i}, uc{i})
% end

n = 5;
x = xc{n};
u = uc{n};
test_data = iddata((x - x(1))', u', Ts);

compare_opt = compareOptions('InitialCondition','z');
figure(3)
compare(test_data, sys, compare_opt)




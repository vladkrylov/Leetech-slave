folder = 'D:\TBONE\Arm\Qt_interfase\build-one_motor_raw_ss-Desktop_Qt_5_2_1_MinGW_32bit-Debug\';
listing = dir(fullfile(folder,'*.yaml'));
L = length(listing);
ord = 2;
as = zeros(ord, ord, L);
bs = zeros(ord, 1, L);
cs = zeros(1, ord, L);

Ts = 0.65;

% A=-1*ones(ord,ord);
% A(1,1) = 1;
% A(1,2) = Ts;
% B=zeros(ord,1);
% B(1,1) = 0;
% C=zeros(1,ord);
% C(1,1) = 1;
% C(1,2) = 0;
% D=0;
% sys0=idss(A,B,C,D);
% sys0.Structure.a.Free = logical([0 0; 1 1]);
% sys0.Structure.b.Free = logical([0; 1]);
% sys0.Structure.c.Free = logical([0 0]);
% sys0.Structure.d.Free = logical(0);

% Options = n4sidOptions;
% Options.Display = 'on';
% Options.Focus = 'stability';
% Options.N4Weight = 'CVA';
% Options.OutputWeight='noise';
% Options.N4Horizon=[9,25,25];
xc = cell(L);
uc = cell(L);
tc = cell(L);

for i = 1:L
    [x, u, t] = slave([folder '/' listing(i).name], 1);
    xc{i} = [x(1)*ones(1,100) x x(end)*ones(1,100)];
    uc{i} = [0*ones(1,100) u u(end)*ones(1,100)];
    tc{i} = Ts*[1:length(x)];
%     data = iddata((x - x(1))',u',Ts);
%     
%     sys = ssest(data,sys0);
%     as(:,:,i) = sys.a;
%     bs(:,:,i) = sys.b;
%     cs(:,:,i) = sys.c;
end

% figure(1)
% subplot(2,2,1)
% hist(as(1,1,:));
% title('A(1,1)')
% 
% subplot(2,2,2)
% hist(as(1,2,:));
% title('A(1,2)')
% 
% subplot(2,2,3)
% hist(as(2,1,:));
% title('A(2,1)')
% 
% subplot(2,2,4)
% hist(as(2,2,:));
% title('A(2,2)')

% figure(2)
% compare(data,sys)



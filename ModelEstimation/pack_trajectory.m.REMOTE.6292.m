function s = pack_trajectory(t, u, x)
motion_inds = find(u ~= 0);
s = struct('t', t,...
           'x', x,...
           'u', u,...
           't_m', t(motion_inds),...
           'u_m', u(motion_inds),...
           'x_m', x(motion_inds));

end
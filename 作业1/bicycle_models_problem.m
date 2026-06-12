clear; clc; close all
%% 第一问：运动学自行车模型 (开环测试 - 必做)
% 车辆基础参数
lf = 2.168; % 前轮到质心距离
lr = 1.907; % 后轮到质心距离
lfr = lf + lr; % 轴距 L
sigma = 5 / 180 * pi; % 恒定前轮转角 5度
dt = 0.01; % 仿真步长
v = 30; % 恒定纵向车速 30 m/s （沿车头方向）

X = 1; Y = 10; phi = 0; % 初始状态，phi是车身与x轴的夹角，X,Y是后轮坐标
phi_vec = []; X_vec = []; Y_vec = [];

for ii = 1:5000 % 仿真步长为5000，总计50秒
    phi_vec = [phi_vec, phi]; 
    % 每次转角为phi，这里的作用是把原来的 phi_vec 和当前的 phi 值水平拼接成一个新的航向角
    % 这个的目的是记录每个时刻（每隔0.01s）航向角的值（车头朝向与x轴的夹角），从而用于最后的仿真
    %也因此，phi需要是一个累加值
    X_vec = [X_vec, X + lr*cos(phi)]; % 记录质心位置
    Y_vec = [Y_vec, Y + lr*sin(phi)];

    % ================= TODO 1.1: 运动学模型状态更新 =================
    % 提示: 根据车辆运动学方程，计算横摆角速度 phi_dot，
    % 然后利用 dt 更新航向角 phi 和全局坐标 X, Y。

    R = lfr/tan(sigma);
    phi_dot = v/R;                        % [在此填空]
    phi = phi + phi_dot*dt;               % [在此填空]
    delta_L = R* sin(phi_dot*dt);
    X = X + delta_L*cos(phi);             % [在此填空]
    Y = Y + delta_L*sin(phi);             % [在此填空]
    % ===============================================================
end

figure(1); hold on; plot(X_vec, Y_vec, 'b.');
axis equal; title("Kinematic Bicycle Model");
xlabel("X [m]"); ylabel("Y [m]");


% %% =================================================================
% %% 拓展问题：动力学模型 (Dynamic Model - 选做)
% %% =================================================================
% % 如果你对车辆底盘动力学感兴趣，请全选以下行，ctrl+T取消注释, 激活这段代码，并完成 TODO 拓展 部分。
% 
% 
% Iz = 5633.44; % 横摆转动惯量
% Cf = 100000;  % 前轮侧偏刚度
% Cr = 100000;  % 后轮侧偏刚度
% m = 1500;     % 车辆质量
% 
% X = 1; Y = 10; phi = 0; % 重置初始状态
% x_dot = v; y_dot = 0; phi_dot = 0; % 初始速度状态
% phi_vec = []; X_vec_dyn = []; Y_vec_dyn = [];
% 
% for ii = 1:5000
%     phi_vec = [phi_vec, phi];
%     X_vec_dyn = [X_vec_dyn, X];
%     Y_vec_dyn = [Y_vec_dyn, Y];
% 
%     % 计算前后轮侧偏角
%     alpha_f = sigma - (y_dot + lf * phi_dot) / x_dot;
%     alpha_r = - (y_dot - lr * phi_dot) / x_dot;
% 
%     % ================= TODO 拓展: 动力学模型状态更新 =================
%     % 提示: 
%     % 1. 根据侧偏刚度计算横向轮胎力 Fyf, Fyr
%     % 2. 根据牛顿第二定律计算横向加速度 y_ddot 和横摆角加速度 phi_ddot
%     % 3. 积分更新速度 y_dot, phi_dot
%     % 4. 将车体坐标系下的速度转换到全局坐标系下，更新 X, Y, phi
% 
%     % Fyf = % [在此填空]
%     % Fyr = % [在此填空]
%     % 
%     % y_ddot = % [在此填空]
%     % phi_ddot = % [在此填空]
%     % 
%     % y_dot = % [在此填空]
%     % phi_dot = % [在此填空]
%     % 
%     % phi = % [在此填空]
%     % X = % [在此填空]
%     % Y = % [在此填空]
% 
%     % ===============================================================
% end
% figure(1); hold on;
% plot(X_vec_dyn, Y_vec_dyn, 'r.');
% legend('Kinematic (运动学)', 'Dynamic (动力学)');
% title("Kinematic vs Dynamic Bicycle Model (v = 30m/s)");

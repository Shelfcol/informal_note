clear all
[x,y] = meshgrid(0.2:0.01:2);
theta = 0.5*(x + y);
alpha = x ./ y;
right = (-1./alpha).* (log((exp(alpha.*(theta - y)))./(exp(alpha.*(theta - y)) + 1)));

neg = (-1./alpha).* (log((exp(alpha.*(x - theta )))./(exp(alpha.*(x - theta)) + 1)));
 
z = right + neg;



[x1,y1,z1]=meshgrid(linspace(0,2,100) , linspace(0,2,100) ,  linspace(0,6,100) );
%
 %z1 = meshgrid(1.*ones(1,100));



%[x, y, z] = meshgrid(-5:0.1:5); % 坐标范围与粒度 
%f = x.^2 + 4 * y.^2 + z.^2; % 定义三元标量函数
%[u v] = meshgrid((-1:0.1:1) * pi, -5:0.1:5); % 曲面参数范围与粒度 
%xi = 1.5 * cos(u); yi = 1.5 * sin(u); zi = v; % 定义曲面网格
h = contourslice(x1, y1, z1,z1 , x, y, z,[1 1]); % 绘制函数等高面在曲面上的截线 
set(h, 'EdgeColor', 'g', 'LineWidth', 5);
%axis equal tight; grid on; box on; % 一些美化操作

hold on
mesh(x,y,z);
hold on
[x2,y2] = meshgrid(0:0.001:2);
z2 = meshgrid(1.*ones(1,2001));
mesh(x2,y2,z2);

xlabel('\delta-','Fontname', 'Bell MT','FontSize',20);
ylabel('\delta+','Fontname', 'Bell MT','FontSize',20);
%h = contourslice(x, y, z, f, xi, yi, zi, [4 4]); % 绘制函数值为4的等高面在圆柱面上的截线
%set(h, 'EdgeColor', 'b', 'LineWidth', 2); % 看得清楚一些



hold on






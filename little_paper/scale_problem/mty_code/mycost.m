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



%[x, y, z] = meshgrid(-5:0.1:5); % ���귶Χ������ 
%f = x.^2 + 4 * y.^2 + z.^2; % ������Ԫ��������
%[u v] = meshgrid((-1:0.1:1) * pi, -5:0.1:5); % ���������Χ������ 
%xi = 1.5 * cos(u); yi = 1.5 * sin(u); zi = v; % ������������
h = contourslice(x1, y1, z1,z1 , x, y, z,[1 1]); % ���ƺ����ȸ����������ϵĽ��� 
set(h, 'EdgeColor', 'g', 'LineWidth', 5);
%axis equal tight; grid on; box on; % һЩ��������

hold on
mesh(x,y,z);
hold on
[x2,y2] = meshgrid(0:0.001:2);
z2 = meshgrid(1.*ones(1,2001));
mesh(x2,y2,z2);

xlabel('\delta-','Fontname', 'Bell MT','FontSize',20);
ylabel('\delta+','Fontname', 'Bell MT','FontSize',20);
%h = contourslice(x, y, z, f, xi, yi, zi, [4 4]); % ���ƺ���ֵΪ4�ĵȸ�����Բ�����ϵĽ���
%set(h, 'EdgeColor', 'b', 'LineWidth', 2); % �������һЩ



hold on






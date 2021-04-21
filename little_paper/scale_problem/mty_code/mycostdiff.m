syms x;
syms y;
f = (-1./(x ./ y)).* (log((exp((x ./ y).*((0.5*(x + y)) - y)))./(exp((x ./ y).*((0.5*(x + y)) - y)) + 1))) + (-1./(x ./ y)).* (log((exp((x ./ y).*(x - (0.5*(x + y)) )))./(exp((x ./ y).*(x - (0.5*(x + y)))) + 1)));
n = diff(f , x);%%%%%x是负样本

%%%%   n = -(2*y*log(exp((x*(x/2 - y/2))/y)/(exp((x*(x/2 - y/2))/y) + 1)))/x
p = diff(f , y);%%%%%x是正样本
%%%%% p = (2*y*exp(-(x*(x/2 - y/2))/y)*(exp((x*(x/2 - y/2))/y) + 1)*((exp((x*(x/2 - y/2))/y)*(x/(2*y) + (x*(x/2 - y/2))/y^2))/(exp((x*(x/2 - y/2))/y) + 1) - (exp((2*x*(x/2 - y/2))/y)*(x/(2*y) + (x*(x/2 - y/2))/y^2))/(exp((x*(x/2 - y/2))/y) + 1)^2))/x - (2*log(exp((x*(x/2 - y/2))/y)/(exp((x*(x/2 - y/2))/y) + 1)))/x
[x,y] = meshgrid(0.2:0.001:2);
ns = -(2*y.*log(exp((x.*(x/2 - y/2))./y)./(exp((x.*(x/2 - y/2))./y) + 1)))./x;
ps = (2*y.*exp(-(x.*(x/2 - y/2))./y).*(exp((x.*(x/2 - y/2))./y) + 1).*((exp((x.*(x/2 - y/2))./y).*(x./(2*y) + (x.*(x/2 - y/2))./y.^2))./(exp((x.*(x/2 - y/2))./y) + 1) - (exp((2*x.*(x/2 - y/2))./y).*(x./(2*y) + (x.*(x/2 - y/2))./y.^2))./(exp((x.*(x/2 - y/2))./y) + 1).^2))./x - (2*log(exp((x.*(x/2 - y/2))./y)./(exp((x.*(x/2 - y/2))./y) + 1)))./x;
mesh(x,y,sqrt(ps.^2+ ns.^2)),xlabel('\delta-','Fontname', 'Times New Roman','FontSize',20),ylabel('\delta+', 'Fontname', 'Bell MT','FontSize',20),zlabel('\itdiff' ,'Fontname', 'Bell MT','FontSize',20 )
hold on
%%%z2 =  y - x + 8;
%%mesh(x , y , z2);
%x为δ+，y为δ-
f=@(x,y)(-x./y*log((exp(y./x.*(y-x)))./(exp(y./x.*(y-x))+1)));%函数简化结果
gF=@(f,x,y)gradient(f(x,y),x(1,:),y(:,1));
x=linspace(0.1,1,200);
y=linspace(0.1,1,200);

X=zeros(length(x)*length(y),1);
Y=zeros(length(x)*length(y),1);
T=zeros(length(x)*length(y),1);
gT=ones(length(x)*length(y),1);
num=0;

for i=1:length(x)
    for j=1:length(y)
        T_=f(x(i),y(j));
        gT_=gF(f,x(i),y(j));
        num=num+1;
        Y(num)=y(j);
        X(num)=x(i);
        T(num)=T_;
        gT(num)=gT_;
    end
end
figure(1)
scatter3(X,Y,T);
grid on 
xlabel('delta+')
ylabel('delta-')
zlabel('T')

figure(2)
scatter3(X,Y,gT);
grid on 
xlabel('delta+')
ylabel('delta-')
zlabel('gT')
        
        
        

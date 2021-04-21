x=linspace(0.05,2,1000);
y=linspace(0.05,2,1000);

X=zeros(length(x)*length(y),1);
Y=zeros(length(x)*length(y),1);
T_scale=zeros(length(x)*length(y),1);
T_no_scale=zeros(length(x)*length(y),1);

gT=ones(length(x)*length(y),1);

num=0;

for i=1:length(x)
    for j=1:length(y)
        T_=funcT_no_scale(x(i),y(j));
        gT_=diffF(x(i),y(j));
        num=num+1;
        Y(num)=y(j);
        X(num)=x(i);
        T_no_scale(num)=T_;
        gT(num)=gT_;
    end
end

figure(1)
scatter3(X,Y,T_no_scale,'.');
grid on 
xlabel('delta+')
ylabel('delta-')
zlabel('T_no_scale')
title('no scale aware')


figure(2)
scatter3(X,Y,gT,'.');
grid on 
xlabel('delta+')
ylabel('delta-')
zlabel('gT')
title('no scale aware')
function T=funcT_no_scale(x,y)
    z=exp((y-x));
    T=-log(z/(z+1));
end

%partialT_partialx
function pT_pX=partial_funcTx(x,y)
    z=exp(y-x);
    pT_pX=1/(z+1);
end

function pT_pY=partial_funcTy(x,y)
    z=exp(y-x);
    pT_pY=-1/(z+1);
end

function pT_diff=diffF(x,y)
    pT_pX=partial_funcTx(x,y);
    pT_pY=partial_funcTy(x,y);
    pT_diff=sqrt(pT_pX*pT_pX+pT_pY*pT_pY);
end
    
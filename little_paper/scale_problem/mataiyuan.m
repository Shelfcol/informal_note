delta_p=linspace(0.2,1,200);
delta_n=linspace(0.2,1,200);

x=zeros(size(delta_p,1)*size(delta_n,1),1);
y=zeros(size(delta_p,1)*size(delta_n,1),1);
T=zeros(size(delta_p,1)*size(delta_n,1),1);
num=1;
for i=1:length(delta_p)
    for j=1:length(delta_n)
        %kesi=delta_n(j)/delta_p(i);
        kesi=1.0;
        theta=(delta_p(i)+delta_n(j))/2;
        tmp_p=2*kesi*(theta-delta_p(i));
        tmp_n=2*kesi*(delta_n(j)-theta);
        T_p=-1/2/kesi*log(exp(tmp_p)/(exp(tmp_p)+1));
        T_n=-1/2/kesi*log(exp(tmp_n)/(exp(tmp_n)+1));
        num=(i-1)*length(delta_p)+j;
        x(num)=delta_p(i);
        y(num)=delta_n(j);
        T(num)=T_p+T_n;
        num=num+1;
    end
end
figure(2)
scatter3(x,y,T);
% patch(x,y,T,T,'edgecolor','flat','facecolor','none');
% view(3);
grid on 
% colorbar;
xlabel('delta+')
ylabel('delta-')
zlabel('T')
        
    
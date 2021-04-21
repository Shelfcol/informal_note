#include <iostream>
#include<vector>
using namespace std;
//https://wiki.jikexueyuan.com/project/easy-learn-algorithm/dijkstra.html
//O(n^2)ʱ�临�Ӷ�
int main()
{
    int n=6;//�������
    vector<vector<int>> e(6,vector<int>(6,9999));
    e[0][0]=0;
    e[0][1]=1;
    e[0][2]=12;
    e[1][1]=0;
    e[1][2]=9;
    e[1][3]=3;
    e[2][2]=0;
    e[2][4]=5;
    e[3][2]=4;
    e[3][3]=0;
    e[3][4]=13;
    e[3][5]=15;
    e[4][4]=0;
    e[4][5]=4;
    e[5][5]=0;

    vector<int> book(6,0);//��ʾ��û����P
    vector<int> dist=e[0];//��ʼʱ�ܹ������[0,0]ֱ�ӵ�������5����ľ��룬������Ҫ����Щ������и���
    book[0]=1;//��ʾ���뵽P�У��Ѿ����ɳ���

    while(1)
    {
        //�Ӳ���P�еĵ��ҳ�һ�������������ĵ㣬�����뵽P��
        int dis=9999;
        int index=-1;
        for(int i=0;i<n;++i)
        {
            if(book[i]==0)
            {
                if(dist[i]<dis)
                {
                    dis=dist[i];
                    index=i;
                }
            }
        }
        if(index==-1)
            break;

        //���˵���뵽P��
        book[index]=1;
        //��index���еĳ��߽����ɳ�
        for(int i=0;i<n;++i)
        {
            if(dist[index]+e[index][i]<dist[i])//�����ǰ�㾭��index�㵽��������̣������dist
            {
                dist[i]=dist[index]+e[index][i];
            }
        }
    }
    //��ӡ���·��
    for(int i=0;i<n;++i)
        cout<<dist[i]<<" ";
    return 0;
}

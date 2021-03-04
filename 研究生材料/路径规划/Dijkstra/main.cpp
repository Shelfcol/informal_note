#include <iostream>
#include<vector>
using namespace std;
//https://wiki.jikexueyuan.com/project/easy-learn-algorithm/dijkstra.html
//O(n^2)时间复杂度
int main()
{
    int n=6;//顶点个数
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

    vector<int> book(6,0);//表示都没加入P
    vector<int> dist=e[0];//初始时能够有起点[0,0]直接到达另外5个点的距离，后面需要对这些距离进行更新
    book[0]=1;//表示加入到P中，已经被松弛了

    while(1)
    {
        //从不在P中的点找出一个距离起点最近的点，并加入到P中
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

        //将此点加入到P中
        book[index]=1;
        //对index所有的出边进行松弛
        for(int i=0;i<n;++i)
        {
            if(dist[index]+e[index][i]<dist[i])//如果当前点经过index点到起点距离更短，则更新dist
            {
                dist[i]=dist[index]+e[index][i];
            }
        }
    }
    //打印最短路径
    for(int i=0;i<n;++i)
        cout<<dist[i]<<" ";
    return 0;
}

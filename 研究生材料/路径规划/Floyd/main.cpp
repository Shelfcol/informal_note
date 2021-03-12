#include <iostream>
#include <vector>
//https://wiki.jikexueyuan.com/project/easy-learn-algorithm/floyd.html
using namespace std;

int main()
{
    int n=4;//¶¥µã¸öÊý
    vector<vector<int>> e(4,vector<int>(4,9999));
    e[0][0]=0;
    e[0][1]=2;
    e[0][2]=6;
    e[0][3]=4;

    e[1][1]=0;
    e[1][2]=3;

    e[2][0]=7;
    e[2][2]=0;
    e[2][3]=1;

    e[3][0]=5;
    e[3][2]=12;
    e[3][3]=0;

    for(int i=0;i<n;++i)
    {
        for(int j=0;j<n;++j)
        {
            for(int k=0;k<n;++k)
            {
                if(e[i][k]<9999&& e[k][j]<9999&& e[i][j]>e[i][k]+e[k][j])
                    e[i][j]=e[i][k]+e[k][j];
            }
        }
    }
    for(int i=0;i<n;++i)
    {
        for(int j=0;j<n;++j)
            cout<<e[i][j]<<" ";
        cout<<endl;
    }

    return 0;
}

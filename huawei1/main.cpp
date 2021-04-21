//dp[i]表示第i个数据前面有多少个比第i个数据小的个数
#include <iostream>
#include <vector>
using namespace std;


void readLine(vector<int>& line_vec,int num)
{
    int n;
    line_vec.clear();
    while((num--)>0)
    {
        cin>>n;
        line_vec.push_back(n);
    }
}
int min_inc_vec(vector<int>& line_vec)
{
    if(line_vec.size()<=1)   return line_vec.size();
    vector<int> dp(line_vec.size(),1);
    for(int i=1;i<line_vec.size();++i)
    {
        for(int j=0;j<i;++j)
        {
            if(line_vec[i]>line_vec[j])
                dp[i]=max(dp[i],dp[j]+1);
        }
    }
    int ans=dp[0];
    for(int i=0;i<dp.size();++i)
        ans=max(ans,dp[i]);
    return ans;
}

int main()
{
    int T;
    while(1)
    {
        cin>>T;//数字个数
        vector<int> line_vec;
        readLine(line_vec,T);
        cout<<min_inc_vec(line_vec)<<endl;
    }
    return 0;
}

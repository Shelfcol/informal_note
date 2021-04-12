#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <algorithm>
using namespace std;

void printOutput(map<char,int>& team_score)
{
    vector<pair<char,int>> s_t;
    for(auto it=team_score.begin();it!=team_score.end();++it)
    {
        s_t.push_back(pair<char,int>{it->first,it->second});
    }
    sort(s_t.begin(),s_t.end(),[&](const pair<char,int>& a,const pair<char,int>& b)
         {
               if(a.second==b.second)    return a.first<b.first;
               return a.second>b.second;
         });
    string ans;
    for(int i=0;i<s_t.size();++i )
    {
        pair<char,int> st=s_t[i];
        ans.push_back(st.first);
        ans.push_back(' ');
        ans.push_back(st.second);
        ans.push_back(',');
    }
    ans.pop_back();
    cout<<ans<<endl;
}

int main()
{
        while(1)
        {
            if(line_str==" ")//表示输入结束，计算结果
            {
                printOutput(team_score);
                break;
            }
            map<char,int> team_score;//保存每个队的得分
            string line_str;
            getline(cin,line_str);

            char t1=line_str[0];
            char t2=line_str[2];
            int score1=line_str[4];
            int score2=line_str[6];
            if(score1>score2)
            {
                team_score[t1]+=3;
                team_score[t2]+=0;
            }
            else if(score1==score2)
            {
                team_score[t1]+=1;
                team_score[t2]+=1;
            }
            else
            {
                team_score[t1]+=0;
                team_score[t2]+=3;
            }
        }

    return 0;
}


/*
* @Author: wilson_t
* @Date:   2020-09-10 09:53:11
* @Last Modified by:   wilson_t
* @Last Modified time: 2021-03-31 10:30:26
*/

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;
using pii = pair<int, int>;
using pil = pair<int, long long>;
using pll = pair<long long, long long>;
using pli = pair<long long, int>;
//用了speedio之后，cin和scanf，cout和printf不能同时用
inline void speedio() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
}

const int d4x[] = {1, 0, -1, 0};
const int d4y[] = {0, 1, 0, -1};
const int d8x[] = {1, 0, -1, 0, 1, 1, -1, -1};
const int d8y[] = {0, 1, 0, -1, 1, -1, 1, -1};

struct Edge {
    int from, to;
    double dist;
    Edge(int f, int t, double d) : from(f), to(t), dist(d) {}

    bool operator<(const Edge& r)const {
        return dist < r.dist;
    }
    bool operator>(const Edge& r)const {
        return dist > r.dist;
    }
};

const int maxn = 200005;
int a[maxn];
int f[maxn];
int T, N;

inline void readline(string& s) {
    getline(cin >> ws, s);
}

//将s用a分开，a可能是逗号或者空格
inline vector<string> splitStr(string& s,string a)
{
	if(s.size()==0)	return vector<string>{};
	while(s.size()!=0)
	{
		
	}
	
}

inline void read(vector<int>& v) {
    int number;
    while (cin >> number) {
        v.push_back(number);
        if (cin.get() == '\n')
            break;
    }
}



void solve(int c) {
    int res = 0;

    cout << "Case #" << c << ": " << res << "\n";
}

int main() {
    speedio();
    freopen("in.txt", "r", stdin);
    // freopen("out.txt", "w", stdout);
    
    // cout.setf(ios::fixed);
    // cout.precision(10);

    cin >> T;
    for(int c = 1; c <= T; ++c) {

        solve(c);
    }
    return 0;
}
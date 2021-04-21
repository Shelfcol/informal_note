/*
* @Author: wilson_t
* @Date:   2021-03-20 21:40:46
* @Last Modified by:   wilson_t
* @Last Modified time: 2021-03-22 22:12:48
*/
#include <bits/stdc++.h>
#include "./SDK_C++/CodeCraft-2021/baseStruct.h"
using namespace std;
const int n = 4;
const int mxCore = 20;
const int mxMem = 20;
int core[n] = {3, 11, 5, 7};
int mem[n] = {10, 1, 6, 6};
int value[n] = {13, 12, 11, 13};

// int f[n+1][mxCore+1][mxMem+1];
// int path[n+1][mxCore+1][mxMem+1];
// int solve() {
//     for(int i = 1; i <= n; ++i) {
//         for(int j = 1; j <= mxCore; ++j) {
//             if(j >= core[i-1]) {
//                 for(int k = 1; k <= mxMem; ++k) {
//                     if(k >= mem[i-1]) {
//                         // f[i][j][k] = max(f[i][j][k], max(f[i - 1][j][k], f[i - 1][j - core[i-1]][k - mem[i-1]] + value[i-1]));
//                         if(f[i - 1][j][k] > f[i][j][k]) {
//                             f[i][j][k] = f[i-1][j][k];
//                             path[i][j][k] = path[i-1][j][k];
//                         }
//                         if(f[i - 1][j - core[i-1]][k - mem[i-1]] + value[i-1] > f[i][j][k]) {
//                             f[i][j][k] = f[i - 1][j - core[i-1]][k - mem[i-1]] + value[i-1];
//                             path[i][j][k] = i-1;
//                         }
//                         // path[j][k] = i-1;
//                     }
//                 }
//             }
//         }
//     }
//     return f[n][mxCore][mxMem];
// }

int f[mxCore+1][mxMem+1];
int path[n+1][mxCore+1][mxMem+1];
int solve() {
    for(int i = 0; i < n; ++i) {
        for(int j = mxCore; j >= core[i]; --j) {
            for(int k = mxMem; k >= mem[i]; --k) {
                path[i+1][j][k] = path[i][j][k];
                if(f[j - core[i]][k - mem[i]] + value[i] > f[j][k]) {
                    f[j][k] = f[j - core[i]][k - mem[i]] + value[i];
                    path[i+1][j][k] = i;
                }
            }
        }
    }
    return f[mxCore][mxMem];
}

int main() {
    memset(f, 0, sizeof(f));
    memset(path, -1, sizeof(path));
    int mxVal = solve();

    // for(int i=0; i<= mxCore; i++) {
    //     for(int j=0; j<= mxMem; j++) {
    //         cout << path[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    cout << "maxVal: " << mxVal << "\n";
    int nowVal = 0, nowCore = mxCore, nowMem = mxMem, nowIdx=n;
    vector<int> choose;
    while(nowVal < mxVal && nowCore > 0 && nowMem > 0) {
        int idx = path[nowIdx][nowCore][nowMem];
        nowIdx = idx;
        if(idx == -1) break;
        choose.emplace_back(idx);
        nowVal += value[idx];
        nowCore -= core[idx];
        nowMem -= mem[idx];
    }
    for(int x: choose) {
        cout << x << " ";
    }
    cout << "\n";


    // cout << "sizeof(serverInfo): " << sizeof(serverInfo) << "\n";
    // cout << "sizeof(vmInfo): " << sizeof(vmInfo) << "\n";
    // cout << "sizeof(requestInfo): " << sizeof(requestInfo) << "\n";
    // cout << "sizeof(vmManager): " << sizeof(vmManager) << "\n";
    return 0;
}
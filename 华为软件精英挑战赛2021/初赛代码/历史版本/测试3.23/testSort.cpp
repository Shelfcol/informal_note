/*
* @Author: wilson_t
* @Date:   2021-03-19 19:21:54
* @Last Modified by:   wilson_t
* @Last Modified time: 2021-03-20 00:00:19
*/
#include <bits/stdc++.h>
using namespace std;

#define TEST

enum reqType {ADD, DEL};
ostream& operator<<(ostream& os, const reqType& t) {
    if(t == ADD) os << "ADD";
    else if(t == DEL) os << "DEL";
    return os;
}
struct requestInfo {
    string vmName; //ADD时需要用  改一下
    reqType type;
    int vmId; //虚拟机id
    requestInfo(string name, reqType t, int id): vmName(name), type(t), vmId(id) {}
#ifdef TEST
    friend ostream& operator<<(ostream& os, const requestInfo& info) {
        os << "(vmName: " << info.vmName << ", "
           << "type: " << info.type << ", "
           << "vmId: " << info.vmId << ")";
        return os;
    }
#endif
};

typedef pair<int, requestInfo*> piReq;


int main() {
    vector<requestInfo> reqs = {{"0", ADD, 10}, {"1", ADD, 20}, {"2", DEL, 5},
        {"3", ADD, 5}, {"4", DEL, 6}, {"5", ADD, 7}, {"6", ADD, 30}, {"7", DEL, 40}, {"8", ADD, 50}
    };

    for(auto& rq : reqs) cout << rq << "\n";
    cout << "----------------------------\n";
    vector<piReq> addIdx;
    vector<piReq> delIdx;
    vector<piReq> reqIdx;
    for(int i = 0; i < (int)reqs.size(); ++i) {
        // if(reqs[i].type == ADD) addIdx.emplace_back(i, &reqs[i]);
        // else if(reqs[i].type == DEL) delIdx.emplace_back(i, &reqs[i]);
        reqIdx.emplace_back(i, &reqs[i]);
    }

    // stable_sort(reqIdx.begin(), reqIdx.end(), [&](const piReq & a, const piReq & b) {
    //     // return a.second->vmId > b.second->vmId;
    //     if(a.second->type==DEL || b.second->type==DEL) return false;
    //     return a.second->vmId > b.second->vmId;
    // });
    // 
    int last = 0, i = 0;
    for( ; i < (int)reqIdx.size(); ++i) {
        if(reqIdx[i].second->type == DEL) {
            if(i > last) {
                sort(reqIdx.begin() + last, reqIdx.begin() + i, [&](const piReq & a, const piReq & b) {
                    return a.second->vmId > b.second->vmId;
                });
            }
            last = i + 1;
        }
    }
    if(i > last) {
        sort(reqIdx.begin() + last, reqIdx.begin() + i, [&](const piReq & a, const piReq & b) {
            return a.second->vmId > b.second->vmId;
        });
    }

    // for(auto [idx, rq] : addIdx) {
    //     cout << "[" << idx << ", " << rq->type << ", " << rq->vmId << "]\n";
    // }
    
    cout << "-----------------------\n";

    // vector<piReq> reqIdx;
    // int it = 0, i = 0, j = 0;
    // while(i < (int)addIdx.size() || j < (int)delIdx.size()) {
    //     if(i >= (int)addIdx.size()) {
    //         reqIdx.emplace_back(delIdx[j++]);
    //         it++;
    //     } else if(j >= (int)delIdx.size()) {
    //         reqIdx.emplace_back(addIdx[i++]);
    //         it++;
    //     } else {
    //         if(it == delIdx[j].first) {
    //             reqIdx.emplace_back(delIdx[j++]);
    //         } else {
    //             reqIdx.emplace_back(addIdx[i++]);
    //         }
    //         it++;
    //     }
    // }

    for(auto [idx, rq] : reqIdx) {
        cout << "[" << idx << ", " << rq->type << ", " << rq->vmId << "]\n";
    }

    return 0;
}
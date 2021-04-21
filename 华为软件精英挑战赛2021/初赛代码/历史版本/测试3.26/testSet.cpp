/*
* @Author: wilson_t
* @Date:   2021-03-26 09:30:47
* @Last Modified by:   wilson_t
* @Last Modified time: 2021-03-26 09:46:47
*/
#include <bits/stdc++.h>
using namespace std;

#define TEST
enum nodeType {type_A = 0, type_B = 1, type_D = 2};
struct timeStamp {
    int day, idx;
    timeStamp(int d = 0, int i = 0): day(d), idx(i) {}
    bool operator<(const timeStamp& rhs) const {
        if(day != rhs.day) return day < rhs.day;
        return idx < rhs.idx;
    }
    bool operator>(const timeStamp& rhs) const {
        if(day != rhs.day) return day > rhs.day;
        return idx > rhs.idx;
    }
};

struct serverInfo {
    string serverName;
    struct Node {
        int mxCore, mxMemory;
        int core, memory;
        timeStamp recoveryTime;
    } nodes[2];
#define A nodes[0]
#define B nodes[1]
    int serverId;
    int serverCost, powerCost;


    void alloc(int A_core, int A_mem, int B_core, int B_mem) {
        this->A.core -= A_core;
        this->A.memory -= A_mem;
        this->B.core -= B_core;
        this->B.memory -= B_mem;
        assert(this->A.core >= 0 & this->A.memory >= 0 && this->B.core >= 0 && this->B.memory >= 0);
    }

    void deAlloc(int A_core, int A_mem, int B_core, int B_mem) {
        this->A.core += A_core;
        this->A.memory += A_mem;
        this->B.core += B_core;
        this->B.memory += B_mem;
    }

    //判断服务器是否有AB节点是否有足够资源
    bool hasEnoughSpace(int A_core, int A_mem, int B_core, int B_mem) {
        if(this->A.core >= A_core && this->A.memory >= A_mem && this->B.core >= B_core && this->B.memory >= B_mem)
            return true;
        return false;
    }


    serverInfo* clone(int _serverId) const {
        serverInfo* newSer = new serverInfo();
        newSer->serverName = serverName;
        newSer->A = A;
        newSer->B = B;
        newSer->serverCost = serverCost;
        newSer->powerCost = powerCost;
        newSer->serverId = _serverId;
        newSer->A.recoveryTime.day = newSer->B.recoveryTime.day = 0;
        newSer->A.recoveryTime.idx = newSer->B.recoveryTime.idx = 0;
        return newSer;
    }


    void setRecoveryTime(timeStamp time, nodeType type) {
        this->setRecoveryTime(time.day, time.idx, type);
    }

    void setRecoveryTime(int day, int idx, nodeType type) {
        if(type == type_A) {
            A.recoveryTime.day = day;
            A.recoveryTime.idx = idx;
        } else if(type == type_B) {
            B.recoveryTime.day = day;
            B.recoveryTime.idx = idx;
        } else {
            A.recoveryTime.day = B.recoveryTime.day = day;
            A.recoveryTime.idx = B.recoveryTime.idx = idx;
        }
    }

    bool canPlaceVmAfterRecovery(timeStamp time, nodeType type) {
        if(type == type_A) {
            return A.recoveryTime < time;
        } else if(type == type_B) {
            return B.recoveryTime < time;
        } else {
            return A.recoveryTime < time && B.recoveryTime < time;
        }
    }

    bool isEmpty() const {    //服务器上当前是否没有虚拟机
        return A.core == A.mxCore && B.core == B.mxCore && A.memory == A.mxMemory && B.memory == B.mxMemory;
    }
#ifdef TEST
    friend ostream& operator<<(ostream& os, const serverInfo& info) {
        os << "(serverName: " << info.serverName << ", "
           << "core: " << info.A.core + info.B.core << ", "
           << "memory: " << info.A.memory + info.B.memory << ", "
           << "serverCost: " << info.serverCost << ", "
           << "powerCost: " << info.powerCost << ")";
        return os;
    }
#endif
};

typedef serverInfo serverEntity;

struct serverCmpByCoreMem {
    bool operator()(serverEntity* a, serverEntity* b) const {
        return a->A.core + a->A.memory > b->A.core + b->A.memory;
    }
};

int main() {

    serverEntity servers[5];
    for(int i=0; i<5; ++i) {
        servers[i].serverId = 0;
        servers[i].serverName = string(1, 'A'+i);
        servers[i].A.core = servers[i].B.core = 4*i;
        servers[i].A.memory = servers[i].B.memory = 4*i;
    }
    set<serverEntity*, serverCmpByCoreMem> st;
    for(int i=0; i<5; ++i) st.insert(&servers[i]);

    for(auto& it: st) {
        cout << *it << "\n";
    }

    serverEntity* ser=*st.begin();
    st.erase(st.begin());
    ser->A.core-=5;
    ser->B.core-=5;
    ser->A.memory-=5;
    ser->A.memory-=5;
    st.insert(ser);

    cout << "------------------------\n";
    for(auto& it: st) {
        cout << *it << "\n";
    }

    return 0;
}
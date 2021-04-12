#ifndef __BASESTRUCT_H
#define __BASESTRUCT_H

#include <algorithm>
#include <assert.h>
#include <climits>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <unordered_set>
using namespace std;

#define TEST

//需要调试的参数
struct Param {
    static constexpr int serverLoadBalanceRatio = 8;
    static  double Q1;
       // static constexpr double splitRatioGain = 0.15;
    static double splitRatioGain;
};

double Param::Q1 = 1;
double Param::splitRatioGain = 0.15;//没啥用

enum nodeType {type_A = 0, type_B = 1, type_D = 2,type_E=-1};
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
        // timeStamp recoveryTime;
        short mxCore, mxMemory;
        short core, memory;
    } nodes[2];
    double leftResRatio;     //资源剩余率
    int serverId;
    int serverCost;
    int vmCnt;
#define A nodes[0]
#define B nodes[1]
    short powerCost;


    void alloc(int A_core, int A_mem, int B_core, int B_mem) {
        this->A.core -= A_core;
        this->A.memory -= A_mem;
        this->B.core -= B_core;
        this->B.memory -= B_mem;
        ++vmCnt;
        assert(this->A.core >= 0 && this->A.memory >= 0 && this->B.core >= 0 && this->B.memory >= 0);
        leftResRatio = (double)(A.core * Param::Q1 + B.core * Param::Q1 + A.memory + B.memory)
                       / (A.mxCore * Param::Q1 + B.mxCore * Param::Q1 + A.mxMemory + B.mxMemory);
    }

    void deAlloc(int A_core, int A_mem, int B_core, int B_mem) {
        this->A.core += A_core;
        this->A.memory += A_mem;
        this->B.core += B_core;
        this->B.memory += B_mem;
        --vmCnt;
        leftResRatio = (double)(A.core * Param::Q1 + B.core * Param::Q1 + A.memory + B.memory)
                       / (A.mxCore * Param::Q1 + B.mxCore * Param::Q1 + A.mxMemory + B.mxMemory);
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
        // newSer->A.recoveryTime.day = newSer->B.recoveryTime.day = 0;
        // newSer->A.recoveryTime.idx = newSer->B.recoveryTime.idx = 0;
        newSer->vmCnt = 0;
        newSer->leftResRatio = 1.0;
        return newSer;
    }

    // void setRecoveryTime(timeStamp time, nodeType type) {
    //     this->setRecoveryTime(time.day, time.idx, type);
    // }

    // void setRecoveryTime(int day, int idx, nodeType type) {
    //     if(type == type_A) {
    //         A.recoveryTime.day = day;
    //         A.recoveryTime.idx = idx;
    //     } else if(type == type_B) {
    //         B.recoveryTime.day = day;
    //         B.recoveryTime.idx = idx;
    //     } else {
    //         A.recoveryTime.day = B.recoveryTime.day = day;
    //         A.recoveryTime.idx = B.recoveryTime.idx = idx;
    //     }
    // }

    // bool canPlaceVmAfterRecovery(timeStamp time, nodeType type) {
    //     if(type == type_A) {
    //         return A.recoveryTime < time;
    //     } else if(type == type_B) {
    //         return B.recoveryTime < time;
    //     } else {
    //         return A.recoveryTime < time && B.recoveryTime < time;
    //     }
    // }

    bool isEmpty() const {    //服务器上当前是否没有虚拟机
        return A.core == A.mxCore && B.core == B.mxCore && A.memory == A.mxMemory && B.memory == B.mxMemory;
    }
#ifdef TEST
    friend ostream& operator<<(ostream& os, const serverInfo& info) {
        os << "(addr: " << &info << ", "
           << "serverName: " << info.serverName << ", "
           << "core: " << info.A.core + info.B.core << ", "
           << "memory: " << info.A.memory + info.B.memory << ", "
           << "serverId: " << info.serverId << ", "
           << "serverCost: " << info.serverCost << ", "
           << "powerCost: " << info.powerCost << ", "
           << "leftResRatio: " << info.leftResRatio << ")";
        return os;
    }
#endif
};

struct vmInfo {
    string vmName;
    timeStamp time;
    serverInfo* server; //虚拟机所在的服务器
    nodeType serNode;   //虚拟机所在的server的节点: 0代表A节点，1代表B节点
    int id;             //虚拟机的id
    short core, memory;
    bool isTwoNode;
    vmInfo* clone(int _id, int _day, int _idx) const {
        vmInfo* vm = new vmInfo();
        vm->vmName = vmName;
        vm->core = core;
        vm->memory = memory;
        vm->isTwoNode = isTwoNode;
        vm->server = nullptr;//初始时虚拟机没有server
        vm->serNode = serNode;
        vm->id = _id;
        vm->time.day = _day;
        vm->time.idx = _idx;
        return vm;
    }
    bool operator<(const vmInfo& rhs) const {
        return time < rhs.time;
    }
    bool operator>(const vmInfo& rhs) const {
        return time > rhs.time;
    }
#ifdef TEST
    friend ostream& operator<<(ostream& os, const vmInfo& info) {
        os << "(vmName: " << info.vmName << ", "
           << "core: " << info.core << ", "
           << "memory: " << info.memory << ", "
           << "server: " << info.server << ", "
           << "serNode: " << info.serNode << ", "
           << "id: " << info.id << ")";
        return os;
    }

#endif
};

typedef vmInfo vmEntity;
typedef serverInfo serverEntity;

struct serCmpByLeftResInc {    //将服务器按照当前资源剩余率升序排序
    bool operator()(serverEntity* a, serverEntity* b) const {
        // if(a->vmCnt != b->vmCnt) return a->vmCnt > b->vmCnt;
        // if(a->leftResRatio != b->leftResRatio) return a->leftResRatio < b->leftResRatio;
        // return a < b;
        double la = max(a->A.core * Param::Q1 + a->A.memory, a->B.core * Param::Q1 + a->B.memory);
        double lb = max(b->A.core * Param::Q1 + b->A.memory, b->B.core * Param::Q1 + b->B.memory);
        if(la != lb) return la < lb;
        return a < b;
    }
};

bool serCmpLeftResIncFunc(serverEntity* a, serverEntity* b) {
    if(a->leftResRatio != b->leftResRatio) return a->leftResRatio < b->leftResRatio;
    return a->A.core + a->B.core < b->A.core + b->B.core;
}

bool serCmpVmCntFunc(serverEntity* a, serverEntity* b) {
    if(a->vmCnt != b->vmCnt) return a->vmCnt < b->vmCnt;
    return a->A.core + a->B.core < b->A.core + b->B.core;
}

bool serCmpPowerCostFunc(serverEntity* a, serverEntity* b) {
    if(a->powerCost != b->powerCost) return a->powerCost < b->powerCost;
    return a->A.core + a->B.core < b->A.core + b->B.core;
}

//资源剩余多的排在前面，便于做firstFit
bool serCmpLeftResDescFunc(serverEntity* a, serverEntity* b) {
    if(a->leftResRatio != b->leftResRatio) return a->leftResRatio > b->leftResRatio;
    return a->A.core + a->B.core > b->A.core + b->B.core;
}

//满的排在前面,剩得少的在前
bool serCmpLeftAbsolueResIncFunc(serverEntity* a, serverEntity* b) {
    int aLeft=a->A.core+a->A.memory+a->B.core+a->B.memory;
    int bLeft=b->A.core+b->A.memory+b->B.core+b->B.memory;
    if(aLeft!=bLeft) return aLeft<bLeft;
    return a<b;
}

enum reqType {ADD, DEL};
struct requestInfo {
    string vmName; //ADD时需要用  改一下
    timeStamp time;
    reqType type;
    int vmId; //虚拟机id
#ifdef TEST
    friend ostream& operator<<(ostream& os, const requestInfo& info) {
        os << "(vmName: " << info.vmName << ", "
           << "type: " << info.type << ", "
           << "vmId: " << info.vmId << ")";
        return os;
    }
#endif
};

struct intTriple {
    int x, y, z;
    intTriple(int a = 0, int b = 0, int c = 0) : x(a), y(b), z(c) {}
};

struct outputManager {
    enum outputType {Purchase = 0, Migration = 1, Match = 2};
    struct outputItem {
        vector<pair<string, int>> purchaseInfo;
        vector<intTriple> migrationInfo;
        timeStamp time;
        outputType outType;
        nodeType type;
        int serverId;

        outputItem(int serId, nodeType t, int d, int index):
            serverId(serId), type(t), time(d, index), outType(Match) {}
        outputItem(vector<pair<string, int>>& _info, int d, int index): purchaseInfo(_info), time(d, index), outType(Purchase) {}
        outputItem(vector<intTriple>& _info, int d, int index): migrationInfo(_info), time(d, index), outType(Migration) {}

    };
    vector<string> output;
    vector<outputItem> items;
    //添加一条输出信息
    void addOutputInfo(int serverId, nodeType type, int day, int idx) {
        items.emplace_back(serverId, type, day, idx);
    }
    void addPurchaseInfo(vector<pair<string, int>>& purchase, int day, int idx) {
        items.emplace_back(purchase, day, idx);
    }

    void addMigrationInfo(vector<intTriple>& migration, int day, int idx) {
        items.emplace_back(migration, day, idx);
    }

    void adjustserverIdmap(map<int, int>& serIdMap) {
        for(auto& item : items) {
            if(item.outType == Match) {
                if(serIdMap.count(item.serverId)) item.serverId = serIdMap[item.serverId];
            } else if(item.outType == Migration) {
                for(auto& mig : item.migrationInfo) {
                    if(serIdMap.count(mig.y)) mig.y = serIdMap[mig.y];
                }
            }
        }
    }
    //清空当前的缓冲区，将输出信息存到输出数组中
    void flushAndStore() {
        this->inlineSort();
        for(auto& item : items) {
            if(item.outType == Match) {
                if(item.type == type_A) {
                    output.emplace_back("(" + to_string(item.serverId) + ", " + "A" + ")\n");
                    // output.emplace_back("(" + to_string(item.serverId) + ", " + "A" + ") ("
                    //     + to_string(item.time.day) + ", " + to_string(item.time.idx) + ")\n");
                } else if(item.type == type_B) {
                    output.emplace_back("(" + to_string(item.serverId) + ", " + "B" + ")\n");
                    // output.emplace_back("(" + to_string(item.serverId) + ", " + "B" + ") ("
                    //     + to_string(item.time.day) + ", " + to_string(item.time.idx) + ")\n");
                } else {
                    output.emplace_back("(" + to_string(item.serverId) + ")\n");
                    // output.emplace_back("(" + to_string(item.serverId) + ") ("
                    //     + to_string(item.time.day) + ", " + to_string(item.time.idx) + ")\n");
                }
            } else if(item.outType == Purchase) {
                output.emplace_back("(purchase, " + to_string(item.purchaseInfo.size()) + ")\n");
                // output.emplace_back("(purchase, " + to_string(item.purchaseInfo.size()) + ") ("
                //         + to_string(item.time.day) + ", " + to_string(item.time.idx) + ")\n");
                for(int i = 0; i < item.purchaseInfo.size(); ++i) {
                    output.emplace_back("(" + item.purchaseInfo[i].first + ", " + to_string(item.purchaseInfo[i].second) + ")\n");
                }
            } else if(item.outType == Migration) {
                output.emplace_back("(migration, " + to_string(item.migrationInfo.size()) + ")\n");
                // output.emplace_back("(migration, " + to_string(item.purchaseInfo.size()) + ") ("
                //         + to_string(item.time.day) + ", " + to_string(item.time.idx) + ")\n");
                for(int i = 0; i < (int)item.migrationInfo.size(); ++i) {
                    if(item.migrationInfo[i].z == 2) {
                        output.emplace_back("(" + to_string(item.migrationInfo[i].x) + ", " + to_string(item.migrationInfo[i].y) + ")\n");
                    } else if(item.migrationInfo[i].z == 0) {
                        output.emplace_back("(" + to_string(item.migrationInfo[i].x) + ", " +
                                            to_string(item.migrationInfo[i].y) + ", A)\n");
                    } else if(item.migrationInfo[i].z == 1) {
                        output.emplace_back("(" + to_string(item.migrationInfo[i].x) + ", " +
                                            to_string(item.migrationInfo[i].y) + ", B)\n");
                    }
                }
            }
        }
        items.clear();
    }

    void inlineSort() {
        ::sort(items.begin(), items.end(), [&](const outputItem & a, const outputItem & b) {
            if(a.time.day != b.time.day) return a.time.day < b.time.day;
            return a.time.idx < b.time.idx;
        });
    }

    void printAll() {
        for(auto& s : output) {
            cout << s;
        }
    }

    void clear() {
        items.clear();
        output.clear();
    }
};

struct vmManager {
    map<int, vmEntity*> vmPool;
    vmManager() {
        vmPool.clear();
    }
    ~vmManager() {
        for (auto& it : vmPool) {
            if(it.second) delete it.second;
        }
    }
    void insert(int vmId, vmEntity* vm) {
        vmPool[vmId] = vm;
    }
    void remove(int vmId) {
        if(vmPool[vmId]) {
            delete vmPool[vmId];
        }
        vmPool.erase(vmId);
    }
    bool contains(int vmId) const {
        return vmPool.find(vmId) != vmPool.end();
    }
    vmEntity* find(int vmId) {
        return vmPool[vmId];
    }
};

typedef bool(*ServerCmp)(serverEntity*, serverEntity*);

//用于存leastPriorityServer
struct serVectorSet {
    vector<serverEntity*> vec;
    unordered_set<serverEntity*> st;

    bool contains(serverEntity* ser) const {
        return st.find(ser) != st.end();
    }

    void push(serverEntity* ser) {
        if(!contains(ser)) {
            vec.push_back(ser);
            st.insert(ser);
        }
    }

    void sort(ServerCmp cmp) {
        ::sort(vec.begin(), vec.end(), cmp);
    }
    void clear(){
        vec.clear();
        st.clear();
    }
};

#endif

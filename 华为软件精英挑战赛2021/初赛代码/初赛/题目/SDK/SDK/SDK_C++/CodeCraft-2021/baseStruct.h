#include <algorithm>
#include <assert.h>
#include <climits>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

#define TEST

enum nodeType {type_A = 0, type_B = 1, type_D = 2};
struct timeStamp {
    int day, idx;
    timeStamp(int d=0, int i=0): day(d), idx(i) {}
};
struct serverInfo {
    string serverName;
    struct Node {
        int mxCore, mxMemory;
        int core, memory;
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
    }

    void deAlloc(int A_core, int A_mem, int B_core, int B_mem) {
        this->A.core += A_core;
        this->A.memory += A_mem;
        this->B.core += B_core;
        this->B.memory += B_mem;
    }

    serverInfo* clone(int _serverId) const {
        serverInfo* newSer = new serverInfo();
        newSer->serverName = serverName;
        newSer->A = A;
        newSer->B = B;
        newSer->serverCost = serverCost;
        newSer->powerCost = powerCost;
        newSer->serverId = _serverId;
        return newSer;
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

struct vmInfo {
    string vmName;
    timeStamp time;
    int core, memory, isTwoNode;
    int server;  //虚拟机所在的server的
    nodeType serNode; //虚拟机所在的server的节点: 0代表A节点，1代表B节点
    int id;      //虚拟机的id
    vmInfo* clone(int _id, int _day, int _idx) const {
        vmInfo* vm = new vmInfo();
        vm->vmName = vmName;
        vm->core = core;
        vm->memory = memory;
        vm->isTwoNode = isTwoNode;
        vm->server = server;
        vm->serNode = serNode;
        vm->id = _id;
        vm->time.day = _day;
        vm->time.idx = _idx;
        return vm;
    }
    bool operator<(const vmInfo& rhs) const {
        return this->id < rhs.id;
    }
    bool operator>(const vmInfo& rhs) const {
        return this->id > rhs.id;
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

enum reqType {ADD, DEL};
struct requestInfo {
    string vmName; //ADD时需要用  改一下
    reqType type;
    timeStamp time;
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

struct outputManager {
    struct outputItem {
        int serverId;
        nodeType type;
        timeStamp time;
        outputItem(int serId, nodeType t, int d, int index):
            serverId(serId), type(t), time(d, index) {}
    };
    vector<string> output;
    vector<outputItem> items;
    //添加一条输出信息
    void addOutputInfo(int serverId, nodeType type, int day, int idx) {
        items.emplace_back(serverId, type, day, idx);
    }
    //清空当前的缓冲区，将输出信息存到输出数组中
    void flushAndStore() {
        this->inlineSort();
        for(auto& item : items) {
            if(item.type == type_A) {
                output.emplace_back("(" + to_string(item.serverId) + ", " + "A" + ")\n");
            } else if(item.type == type_B) {
                output.emplace_back("(" + to_string(item.serverId) + ", " + "B" + ")\n");
            } else {
                output.emplace_back("(" + to_string(item.serverId) + ")\n");
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
        if(vmPool[vmId]) delete vmPool[vmId];
        vmPool.erase(vmId);
    }
    bool contains(int vmId) const {
        return vmPool.find(vmId) != vmPool.end();
    }
    vmEntity* find(int vmId) {
        return vmPool[vmId];
    }
};




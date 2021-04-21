// #include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <assert.h>
using namespace std;

#define TEST

typedef long long ll;

void speedio() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
}
struct serverInfo {
    string serverName;
    struct Node {
        int core, memory;
    } A, B;
    int serverCost, powerCost;
    serverInfo* clone() const {
        serverInfo* newSer = new serverInfo();
        newSer->serverName = serverName;
        newSer->A = A;
        newSer->B = B;
        newSer->serverCost = serverCost;
        newSer->powerCost = powerCost;
        return newSer;
    }
#ifdef TEST
    friend ostream& operator<<(ostream& os, const serverInfo& info) {
        os  << "(serverName: " << info.serverName << ", "
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
    int core, memory, isTwoNode;
    int server;  //虚拟机所在的server
    int serNode; //虚拟机所在的server的节点: 0代表A节点，1代表B节点
    int id;      //虚拟机的id
    vmInfo* clone() const {
        vmInfo* vm = new vmInfo();
        vm->vmName = vmName;
        vm->core = core;
        vm->memory = memory;
        vm->isTwoNode = isTwoNode;
        vm->server = server;
        vm->serNode = serNode;
        vm->id = id;
        return vm;
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
enum reqType {ADD, DEL};
struct requestInfo {
    string vmName; //ADD时需要用  改一下
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

typedef vmInfo vmEntity;
typedef serverInfo serverEntity;

// struct vmEntity {
//     int core, memory, isTwoNode;
//     int server;
//     int id;
// }

//供选的服务器
vector<serverInfo> serverInfos; //这里string是唯一的
//1、按照自定义的标准（性价比： 单位核的成本（硬件+100天能耗）和单位内存的成本）排序  这部分gxf
//---2、直接按照规格大小排序，然后每隔多少步取一种。假如取四种---
//排序之后映射string->index;
unordered_map<string, int> serverType2Info; //string to index
//虚拟机类型
unordered_map<string, vmInfo*> vmInfos;
//当前存在的服务器（已经购买的）
// vector<serverInfo> serverResource;
int serverNumber = 0; //服务器数量（编号从0开始）
unordered_map<int, serverEntity*> serverFor1Node;   //服务器id->服务器实体
unordered_map<int, serverEntity*> serverFor2Node;
//当前存在的虚拟机 虚拟机有ID
unordered_map<int, vmEntity*> vmPool;        //虚拟机ID->虚拟机实体
vector<string> output;
vector<requestInfo> dailyRequests;        //每天的请求
// vector<vector<requestInfo>> allDaysRequests; //所有天的请求
const int MaxChooseSerType = 4;              //选择的服务器的最大种类数

void readline(string& s) {
    getline(cin >> ws, s);
}
bool isNumLetter(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
serverInfo readServer() {
    //to do
    serverInfo serInfo;
    string s;
    readline(s);
    // cout << "serverInfo " << s << endl;
    unsigned int index = 0;
    while(index < s.size() && !isNumLetter(s[index]))
        index++;
    while(index < s.size() && isNumLetter(s[index])) {
        serInfo.serverName.push_back(s[index]);
        index++;
    }


    string cpuCoreStr;
    while (index < s.size() && !isNumLetter(s[index]))
        index++;
    while (index < s.size() && isNumLetter(s[index])) {
        cpuCoreStr.push_back(s[index]);
        index++;
    }
    int cpuCore = stoi(cpuCoreStr);
    serInfo.A.core = cpuCore / 2;
    serInfo.B.core = cpuCore / 2;

    string memorySizeStr;
    while (index < s.size() && !isNumLetter(s[index]))
        index++;
    while (index < s.size() && isNumLetter(s[index])) {
        memorySizeStr.push_back(s[index]);
        index++;
    }
    int memorySize = stoi(memorySizeStr);
    serInfo.A.memory = memorySize / 2;
    serInfo.B.memory = memorySize / 2;

    string serverCostStr;
    while (index < s.size() && !isNumLetter(s[index]))
        index++;
    while (index < s.size() && isNumLetter(s[index])) {
        serverCostStr.push_back(s[index]);
        index++;
    }
    serInfo.serverCost = stoi(serverCostStr);

    string powerCostStr;
    while (index < s.size() && !isNumLetter(s[index]))
        index++;
    while (index < s.size() && isNumLetter(s[index])) {
        powerCostStr.push_back(s[index]);
        index++;
    }
    serInfo.powerCost = stoi(powerCostStr);

    //cout << serInfo.serverName << " " << serInfo.A.core * 2 << " " << serInfo.A.memory * 2 << " " << serInfo.serverCost << " " << serInfo.powerCost << endl;

    return serInfo;
}

vmInfo* readVm() {
    //to do
    // vmInfo vm;
    vmInfo* vm = new vmInfo();

    string s;
    readline(s);

    unsigned int index = 0;
    while (index < s.size() && !isNumLetter(s[index]))
        index++;
    while (index < s.size() && isNumLetter(s[index])) {
        vm->vmName.push_back(s[index]);
        index++;
    }

    string cpuCoreStr;
    while (index < s.size() && !isNumLetter(s[index]))
        index++;
    while (index < s.size() && isNumLetter(s[index])) {
        cpuCoreStr.push_back(s[index]);
        index++;
    }
    int cpuCore = stoi(cpuCoreStr);
    vm->core = cpuCore;

    string memorySizeStr;
    while (index < s.size() && !isNumLetter(s[index]))
        index++;
    while (index < s.size() && isNumLetter(s[index])) {
        memorySizeStr.push_back(s[index]);
        index++;
    }
    int memorySize = stoi(memorySizeStr);
    vm->memory = memorySize;


    string isTwoNodeStr;
    while (index < s.size() && !isNumLetter(s[index]))
        index++;
    while (index < s.size() && isNumLetter(s[index])) {
        isTwoNodeStr.push_back(s[index]);
        index++;
    }
    vm->isTwoNode = stoi(isTwoNodeStr);

    //cout << vm->vmName << " " << vm->core  << " " << vm->memory  << " " << vm->isTwoNode << endl;

    return vm;
}

requestInfo readRequest() {
    //to do
    requestInfo reqInfo;

    string s;
    readline(s);
    // cout << "request " << s << endl;
    string reqVmName, reqId;
    if (s[1] == 'a') { //ADD

        unsigned int index = 0;
        //跳过ADD
        while (index < s.size() && !isNumLetter(s[index]))
            index++;
        while (index < s.size() && isNumLetter(s[index])) {
            index++;
        }
        /**********************/

        while (index < s.size() && !isNumLetter(s[index]))
            index++;

        string vmNameStr;
        while (index < s.size() && isNumLetter(s[index])) {
            vmNameStr.push_back(s[index]);
            index++;
        }
        reqInfo.vmName = vmNameStr;


        while (index < s.size() && !isNumLetter(s[index]))
            index++;
        string vmIDStr;
        while (index < s.size() && isNumLetter(s[index])) {
            vmIDStr.push_back(s[index]);
            index++;
        }
        reqInfo.vmId = stoi(vmIDStr);

        reqInfo.type = ADD;
    } else { //DEL
        unsigned int index = 0;
        //跳过DEL和挨着的，空格
        while (index < s.size() && !isNumLetter(s[index]))
            index++;
        while (index < s.size() && isNumLetter(s[index])) {
            index++;
        }
        /**********************/

        while (index < s.size() && !isNumLetter(s[index]))
            index++;

        string vmIdStr;
        while (index < s.size() && isNumLetter(s[index])) {
            vmIdStr.push_back(s[index]);
            index++;
        }
        reqInfo.vmId = stoi(vmIdStr);

        reqInfo.vmName = vmPool[reqInfo.vmId]->vmName;
        reqInfo.type = DEL;
    }
    // cout << reqInfo.type << " " << reqInfo.vmName << " " << reqInfo.vmId << endl;
    return reqInfo;
}


void initServer() {
    //初始化服务器的数量和种类
    //to do
    string serverType = "hostUY41I";
    int n = 2800;
    string initBuy = "(purchase, ";
    initBuy += to_string(2) + ")\n";

    output.emplace_back(initBuy);

    string pauseInfo = "(" + serverType + ", ";
    pauseInfo += std::to_string(n / 2) + ")\n";

    output.emplace_back(pauseInfo);

    for(int i = 0; i < n / 2; ++i) {
        if(i % 2 == 0) {
            serverFor2Node[serverNumber++] = serverInfos[serverType2Info[serverType]].clone();
        } else {
            serverFor1Node[serverNumber++] = serverInfos[serverType2Info[serverType]].clone();
        }
    }
    serverType = "host78BMY";
    pauseInfo = "(" + serverType + ", ";
    pauseInfo += std::to_string(serverNumber) + ")\n";

    output.emplace_back(pauseInfo);

    for(int i = 0; i < n / 2; i++) {
        if(i % 2 == 0) {
            serverFor2Node[serverNumber++] = serverInfos[serverType2Info[serverType]].clone();
        } else {
            serverFor1Node[serverNumber++] = serverInfos[serverType2Info[serverType]].clone();
        }
    }
}

double ratio_core_mem = 1; //core和memory之间的重要性关系
void match(vmEntity* vm) {
    //分配
    //to do
    //分配该虚拟机给一个服务器，服务器资源的更新
    //分配的信息包括：虚拟机所在的服务器索引以及所在的节点（需要更新server和serNode两个属性）
    int core = vm->core, memory = vm->memory;
    if (vm->isTwoNode) {
        //这里采用最优适配：即分配剩余最小的可用资源(先考虑核数)（跟操作系统分配内存类似）
        //暂时采用遍历的方式：如果后续需要提高效率，可设计LinkedList的方式
        int best = -1;
        core /= 2;
        memory /= 2;
        for (const auto& it : serverFor2Node) {
            int id = it.first;
            serverEntity* server = it.second;
            if (server->A.core >= core && server->A.memory >= memory) {
                if (best == -1) {
                    best = id;
                } else {
                    //这里可根据输入的虚拟机的类型进行分析core和memory的分布，得到core和memory的重要性
                    if (ratio_core_mem * server->A.core + server->A.memory <
                            ratio_core_mem * serverFor2Node[best]->A.core + serverFor2Node[best]->A.memory) {
                        best = id;
                    }
                }
            }
        }
        assert(best != -1);
        vm->server = best;
        vm->serNode = 2;
        //分配资源
        serverFor2Node[best]->A.core -= core;
        serverFor2Node[best]->B.core -= core;
        serverFor2Node[best]->A.memory -= memory;
        serverFor2Node[best]->B.memory -= memory;
        output.emplace_back("(" + to_string(best) + ")\n");
        // cout << "core: " << core << ", " << "mem: " << memory << ", "
        //     << serverFor2Node[best] << "\n";
    } else {
        int bestSer = -1, bestNode = 0;
        for (const auto& it : serverFor1Node) {
            int id = it.first;
            serverEntity* server = it.second;
            //节点 A
            if (server->A.core >= core && server->A.memory >= memory) {
                if (bestSer == -1) {
                    bestSer = id;
                    bestNode = 0;
                } else {
                    if (ratio_core_mem * server->A.core + server->A.memory <
                            ratio_core_mem * serverFor1Node[bestSer]->A.core + serverFor1Node[bestSer]->A.memory) {
                        bestSer = id;
                        bestNode = 0;
                    }
                }
            }
            //节点 B
            if (server->B.core >= core && server->B.memory >= memory) {
                if (bestSer == -1) {
                    bestSer = id;
                    bestNode = 1;
                } else {
                    if (ratio_core_mem * server->B.core + server->B.memory <
                            ratio_core_mem * serverFor1Node[bestSer]->B.core + serverFor1Node[bestSer]->B.memory) {
                        bestSer = id;
                        bestNode = 1;
                    }
                }
            }
        }
        assert(bestSer != -1);
        vm->server = bestSer;
        vm->serNode = bestNode;
        //分配资源
        if (bestNode == 0) {
            serverFor1Node[bestSer]->A.core -= core;
            serverFor1Node[bestSer]->A.memory -= memory;
            output.emplace_back("(" + to_string(bestSer) + ", A)\n");
        } else {
            serverFor1Node[bestSer]->B.core -= core;
            serverFor1Node[bestSer]->B.memory -= memory;
            output.emplace_back("(" + to_string(bestSer) + ", B)\n");
        }
        // cout << "core: " << core << ", " << "mem: " << memory << ", "
        //     << serverFor1Node[bestSer] << "\n";
    }
}

// void match(vmEntity* vm) {
    
// }

void expansion() {
    //扩容
    //to do
    string s = "(purchase, 0)\n";
    output.emplace_back(s);
}

void migrate() {
    //迁移
    //to do
    string s = "(migration, 0)\n";
    output.emplace_back(s);
}

void calcPowerCost() {
    //to do
}

void addVm(vmEntity* vm) {
    //添加虚拟机
    match(vm);          //分配给服务器
    vmPool[vm->id] = vm; //放入虚拟机池
}

void delVm(int id) {
    vmEntity* vm = vmPool[id];
    if (vm->isTwoNode) {
        serverEntity* server = serverFor2Node[vm->server];
        //释放资源
        server->A.core += vm->core / 2;
        server->B.core += vm->core / 2;
        server->A.memory += vm->memory / 2;
        server->B.memory += vm->memory / 2;
    } else {
        serverEntity* server = serverFor1Node[vm->server];
        if (vm->serNode == 0) {
            //A节点
            server->A.core += vm->core;
            server->A.memory += vm->memory;
        } else {
            //B节点
            server->B.core += vm->core;
            server->B.memory += vm->memory;
        }
    }
#ifdef TEST

#endif
    delete vmPool[id];
    vmPool.erase(id);
}

void freeSysMemory() {
    for(auto& it : vmInfos) {
        delete it.second;
    }
    for(auto& it : serverFor1Node) {
        delete it.second;
    }
    for(auto& it : serverFor2Node) {
        delete it.second;
    }
    for(auto& it : vmPool) {
        delete it.second;
    }
}

void allRequestsAnalysis() {    //读入所有的requests, 进行分析

}

void dailyProcess(int day) {
    if (day == 0) initServer();
    else expansion();
    migrate();
    for (const requestInfo& req : dailyRequests) {
        if (req.type == ADD) {
            vmEntity* vm = (vmInfos[req.vmName])->clone();
            vm->id = req.vmId;
            addVm(vm);
        } else {
            delVm(req.vmId);
        }
    }
    calcPowerCost();
}

int N_allServerType, N_allVmType, T_days, R_requests;
int main() {
    // speedio();
#ifdef TEST
    freopen("../training-data/training-2.txt", "rb", stdin);
    freopen("../training-data/training-2-output.txt", "w", stdout);
#endif
    cin >> N_allServerType;
#ifdef TEST
    // cout << "N_allServerType: " << N_allServerType << "\n";
#endif
    for (int i = 0; i < N_allServerType; ++i) {
        serverInfos.emplace_back(readServer());
    }
    //将供选的服务器信息进行排序（先按核数，再按内存）
    sort(serverInfos.begin(), serverInfos.end(), [&](serverInfo & a, serverInfo & b) {
        if (a.A.core != b.A.core)
            return a.A.core < b.A.core;
        return a.A.memory < b.A.memory;
    });
    for (int i = 0; i < (int)serverInfos.size(); ++i) {
        serverType2Info[serverInfos[i].serverName] = i;
    }

    cin >> N_allVmType;
#ifdef TEST
    // cout << "N_allVmType: " << N_allVmType << "\n";
#endif
    for (int i = 0; i < N_allVmType; ++i) {
        vmInfo* vm = readVm();
        vmInfos[vm->vmName] = vm;
    }

    // for(auto& it: vmInfos) {
    //     cout << it.first << ": " << *it.second << "\n";
    // }

    cin >> T_days;
#ifdef TEST
    // cout << "T_days: " << T_days << "\n";
#endif
    for (int day = 0; day < T_days; ++day) {
        cin >> R_requests;
        dailyRequests.clear();
        for (int j = 0; j < R_requests; ++j) {
            dailyRequests.emplace_back(readRequest());
        }
        dailyProcess(day);
    }

    for (string& s : output) {
        cout << s;
    }
#ifdef TEST
    // cout << "finish!\n";
#endif
    freeSysMemory();
    return 0;
}
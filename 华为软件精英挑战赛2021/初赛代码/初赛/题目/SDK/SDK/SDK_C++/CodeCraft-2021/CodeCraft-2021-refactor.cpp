// #include <bits/stdc++.h>
#include <algorithm>
#include <assert.h>
#include <climits>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <set>
#include <vector>
#include <string.h>
#include "baseStruct.h"
using namespace std;

#define TEST

typedef long long ll;

void speedio() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
}

//供选的服务器
vector<serverInfo> serverInfos; //这里string是唯一的
//1、按照自定义的标准（性价比： 单位核的成本（硬件+100天能耗）和单位内存的成本）排序  这部分gxf
//---2、直接按照规格大小排序，然后每隔多少步取一种。假如取四种---
//排序之后映射string->index;
map<string, int> serverType2Info; //string to index
//虚拟机类型
map<string, vmInfo*> vmInfos;

int serverNumber = 0; //服务器数量（编号从0开始）单双节点一起编号
map<int, serverEntity*> serverFor1Node;
map<int, serverEntity*> serverFor2Node;

vmManager vmPool;
vector<requestInfo> dailyRequests; //每天的请求
vector<vector<requestInfo>> allDaysRequests;
outputManager outManager;

void readline(string& s) {
    getline(cin >> ws, s);
}
bool isNumLetter(char c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '.');
}
serverInfo readServer() {
    //to do
    serverInfo serInfo;
    string s;
    readline(s);

    unsigned int index = 0;
    while (index < s.size() && !isNumLetter(s[index]))
        index++;
    while (index < s.size() && isNumLetter(s[index])) {
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
    serInfo.A.mxCore = serInfo.A.core = cpuCore / 2;
    serInfo.B.mxCore = serInfo.B.core = cpuCore / 2;


    string memorySizeStr;
    while (index < s.size() && !isNumLetter(s[index]))
        index++;
    while (index < s.size() && isNumLetter(s[index])) {
        memorySizeStr.push_back(s[index]);
        index++;
    }
    int memorySize = stoi(memorySizeStr);
    serInfo.A.mxMemory = serInfo.A.memory = memorySize / 2;
    serInfo.B.mxMemory = serInfo.B.memory = memorySize / 2;

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

    return vm;
}

requestInfo readRequest() {
    //to do
    requestInfo reqInfo;

    string s;
    readline(s);
    string reqVmName, reqId;
    if (s[1] == 'a') {
        //ADD

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
    } else {
        //DEL
        unsigned int index = 0;
        //跳过DEL和挨着的，空格
        while (index < s.size() && !isNumLetter(s[index]))
            index++;
        while (index < s.size() && isNumLetter(s[index])) {
            index++;
        }

        while (index < s.size() && !isNumLetter(s[index]))
            index++;

        string vmIdStr;
        while (index < s.size() && isNumLetter(s[index])) {
            vmIdStr.push_back(s[index]);
            index++;
        }
        reqInfo.vmId = stoi(vmIdStr);

        // reqInfo.vmName = (vmPool.find(reqInfo.vmId))->vmName;
        reqInfo.type = DEL;
    }
    return reqInfo;
}

void freeSysMemory() {
    for (auto& it : vmInfos) {
        delete it.second;
    }
    for (auto& it : serverFor1Node) {
        delete it.second;
    }
    for (auto& it : serverFor2Node) {
        delete it.second;
    }
}

void bindVm2Server(vmEntity* vm, serverEntity* server, nodeType type) {
    vm->server = server->serverId;
    vm->serNode = type;
    int core = vm->core, memory = vm->memory;
    int A_core = 0, A_mem = 0, B_core = 0, B_mem = 0;
    if(type == type_A) {
        A_core = core;
        A_mem = memory;
    } else if(type == type_B) {
        B_core = core;
        B_mem = memory;
    } else {
        core /= 2;
        memory /= 2;
        A_core = B_core = core;
        A_mem = B_mem = memory;
    }
    server->alloc(A_core, A_mem, B_core, B_mem);
}

void unbindVm2Server(vmEntity* vm, serverEntity* server, nodeType type) {
    int core = vm->core, memory = vm->memory;
    int A_core = 0, A_mem = 0, B_core = 0, B_mem = 0;
    if(type == type_A) {
        A_core = core;
        A_mem = memory;
    } else if(type == type_B) {
        B_core = core;
        B_mem = memory;
    } else {
        core /= 2;
        memory /= 2;
        A_core = B_core = core;
        A_mem = B_mem = memory;
    }
    server->deAlloc(A_core, A_mem, B_core, B_mem);
}

const int MAXSIZE = 200;
const int MAXCORE = 800;
const int MAXMEM = 800;
int dpBag[MAXCORE][MAXMEM];
int pathBag[MAXSIZE][MAXCORE][MAXMEM];
void solveBagProblem(vector<vmEntity*>& vms, serverEntity* server, nodeType type) {
    //对于单节点虚拟机，单个服务器需要调用两次
    //对于双节点虚拟机，匹配的时候core, mem需要除以2
    int vmSz = vms.size(), mxCore, mxMem;
    if(vmSz == 0) return;
    if(type == type_A) {
        mxCore = server->A.core;
        mxMem = server->A.memory;
    } else if(type == type_B) {
        mxCore = server->B.core;
        mxMem = server->B.memory;
    } else {
        mxCore = server->A.core;
        mxMem = server->A.memory;
    }
    vector<int> value;
    for(vmEntity* vm : vms) {
        value.emplace_back(vm->core + vm->memory);
    }
    memset(dpBag, 0, sizeof(dpBag));
    memset(pathBag, -1, sizeof(pathBag));
    for(int i = 0; i < vmSz; ++i) {
        for(int j = mxCore; j >= vms[i]->core; --j) {
            for(int k = mxMem; k >= vms[i]->memory; --k) {
                pathBag[i + 1][j][k] = pathBag[i][j][k];
                if(dpBag[j - vms[i]->core][k - vms[i]->memory] + value[i] > dpBag[j][k]) {
                    dpBag[j][k] = dpBag[j - vms[i]->core][k - vms[i]->memory] + value[i];
                    pathBag[i + 1][j][k] = i;
                }
            }
        }
    }
    int mxVal = dpBag[mxCore][mxMem];
    int nowVal = 0, nowCore = mxCore, nowMem = mxMem, nowIdx = vmSz;
    set<int> choose;
    while(nowVal < mxVal && nowCore > 0 && nowMem > 0) {
        nowIdx = pathBag[nowIdx][nowCore][nowMem];
        if(nowIdx < 0) break;
        choose.insert(nowIdx);
        nowVal += value[nowIdx];
        nowCore -= vms[nowIdx]->core;
        nowMem -= vms[nowIdx]->memory;
    }
    //分配vm给对应的服务器
    vector<vmEntity*> leftVm;
    for(int i = 0; i < vmSz; ++i) {
        if(choose.count(i)) {   //分配
            bindVm2Server(vms[i], server, type);
            outManager.addOutputInfo(server->serverId, type, vms[i]->time.day, vms[i]->time.idx);
        } else {
            leftVm.emplace_back(vms[i]);
        }
    }
    //返回新的剩余的
    vms = leftVm;
    // return leftVm;
}


int N_allServerType, N_allVmType, T_days, R_requests;
int main() {
    speedio();
#ifdef TEST
    freopen("../training-data/training-2.txt", "rb", stdin);
    // freopen("../training-data/training-2-output.txt", "w", stdout);
#endif
    cin >> N_allServerType;
#ifdef TEST
    cout << N_allServerType << "\n";
#endif
    for (int i = 0; i < N_allServerType; ++i) {
        serverInfos.emplace_back(readServer());
    }

    cin >> N_allVmType;
#ifdef TEST
    cout << N_allVmType << "\n";
#endif
    for (int i = 0; i < N_allVmType; ++i) {
        vmInfo* vm = readVm();
        vmInfos[vm->vmName] = vm;
    }

    cin >> T_days;
#ifdef TEST
    cout << T_days << "\n";
#endif
    for (int day = 0; day < T_days; ++day) {
        cin >> R_requests;
        dailyRequests.clear();
        for (int j = 0; j < R_requests; ++j) {
            dailyRequests.push_back(readRequest());
        }
        allDaysRequests.push_back(dailyRequests);
    }
    outManager.flushAndStore();
    outManager.printAll();
#ifdef TEST
#endif
    freeSysMemory();
    return 0;
}
// #include <bits/stdc++.h>
#include <algorithm>
#include <assert.h>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <string.h>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>
#include <climits>
#include <ctime>
#include <math.h>
#include <thread>
#include <mutex>
// #include <unistd.h>
#include "baseStruct.h"

using namespace std;

typedef long long ll;

void speedio() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
}


//供选的服务器
vector<serverInfo> serverInfos; //这里string是唯一的

//拟合平面参数
struct FaceFit {
    double a, b;
    double getFitVal(double x, double y) const {
        return a * x + b * y;
    }
} serCoreMemCostFit, serCoreMemPowerFit;

//1、按照自定义的标准（性价比： 单位核的成本（硬件+100天能耗）和单位内存的成本）排序  这部分gxf
//排序之后映射string->index;
map<string, int> serverType2Info; //string to index
//虚拟机类型
map<string, vmInfo*> vmInfos;

int serverNumber = 0; //服务器数量（编号从0开始）单双节点一起编号
string serverNameToBeBought;//买一种服务器
set<serverEntity*, serCmpByLeftResInc> serverForNode;//保存购买的服务器，下标就是中间的购买顺序，最终重新编号的时候进行映射即可

map<int, serverEntity*> serverForNodeToday;    //

//parms to adjust
const int typesOfServer2Choose = 2; //选择服务器的种类
vector<serverInfo> chosenServers[2];
// double ratioCoreMemSplit[typesOfServer2Choose + 1] = {0.15, 0.75, 1.25, 6};
double ratioCoreMemSplit[typesOfServer2Choose + 1] = {0.15, 1.0,  6};
vector<serverInfo*> serverCandidates[typesOfServer2Choose];
//保存对应三种服务器的等待放到新的对应的服务器的虚拟机。
//每次添加的时候，判断如果某一行超过400个虚拟机，则做一次bagProblem，直到剩余200个。然后继续添加
vector<vector<vmEntity*>> vmsPlacedNewForNodeServers;//待放到新服务器的单节点虚拟机
map<serverEntity*, set<vmEntity*>> vmsInServers; //保存每个server对应的所有虚拟机，为了方便后续的迁移

vmManager vmPool;
vector<requestInfo> dailyRequests; //每天的请求
deque<vector<requestInfo>> windowRequests;
vector<vector<requestInfo>> toBeProcessedRequests;//选出某一批次处理的request


//每天迁移的时候分del和非del
serVectorSet leastPriorityAddVmsServerForNode;//!!每天操作完成之后需要清空
set<int> vmIdDeletedToday;//读取当天的请求del的虚拟机ID，这里面有今天之前已经生成好的vm实体，另外一部分是今天add然后今天del的虚拟机。每天读取当天的请求进行维护


int day_time;//今天是第几天
int N_allServerType, N_allVmType, T_days, R_requests, K_windowSize;
int dayAfterChooseServerCost;//这天之后选择服务器时用serverCost代替powerCost
mutex mutWindowReadWrite;



outputManager outManager;
const int MAXSIZE = 400;
const int MAXCORE = 800;
const int MAXMEM = 800;
struct MaxCoreMem {
    int core, mem;
} MaxcoreMaxmemory[2]; //{单节点core，单节点memory，双节点core，双节点memory}
map<int, int> vmAliveTime;  //虚拟机id->存活时间
int migrationTimeToday;//记录某天迁移的次数
//迁移相关的变量
const float serverSplitLeftResourceRatio = 0.24; //=0.17;// 0.423; //利用资源的剩余率将服务器分为两部分，前面一部分剩余少，后面一部分剩余多.这个数字越小，则被迁移放置虚拟机的服务器越满
const int serverMigrateStopSize = 6; //!!当递归排序之后最终的服务器vector里面只有10个服务器的时候终止递归.这个数字<8就会断错误


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

    int index = 0;
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

        int index = 0;
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
        // try {
        reqInfo.vmId = stoi(vmIDStr);
        // } catch (std::invalid_argument){
        // cerr << "add input error" << "\n";
        // }

        reqInfo.type = ADD;
    } else {
        //DEL
        int index = 0;
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
        // try{
        reqInfo.vmId = stoi(vmIdStr);
        // } catch (std::invalid_argument){
        // cerr << "del input error" << "\n";
        // }

        // reqInfo.vmName = (vmPool.find(reqInfo.vmId))->vmName;
        reqInfo.type = DEL;
    }
    return reqInfo;
}

void freeSysMemory() {
    for (auto& it : vmInfos) {
        delete it.second;
    }
    for (auto& it : serverForNode) {
        delete it;
    }
}

//通过虚拟机选择对应的服务器数组的编号，三种服务器就是0 1 2
int getRowOfvm2server(vmEntity* vm) {
    //to do
    double ratio_core_mem = (double)vm->core / vm->memory;
    for(int i = 1; i < typesOfServer2Choose; ++i) {
        if(ratio_core_mem < ratioCoreMemSplit[i]) return i - 1;
    }
    return typesOfServer2Choose - 1;
}

int getRowOfServer(serverEntity* server) {
    double ratio_core_mem = (double)(server->A.mxCore) / server->A.mxMemory;
    for(int i = 1; i < typesOfServer2Choose; ++i) {
        if(ratio_core_mem < ratioCoreMemSplit[i]) return i - 1;
    }
    return typesOfServer2Choose - 1;
}


void findAllrequestMaxcoreMaxmemory() {
    vector<pair<double, double>> coreMemRatio;
    double coreMemTot = 0;
    for(int i = 0; i < windowRequests.size(); ++i) {
        vector<requestInfo>& oneDayRequests = windowRequests[i];
        for(int j = 0; j < oneDayRequests.size(); ++j) {
            if(oneDayRequests[j].type == ADD) {

                //得到这个虚拟机的实体，自己可以判断是单双节点服务器
                vmEntity* vmInfo = vmInfos[oneDayRequests[j].vmName];
                if(!vmInfo->isTwoNode) {
                    if(vmInfo->core > MaxcoreMaxmemory[0].core) MaxcoreMaxmemory[0].core = vmInfo->core;
                    if(vmInfo->memory > MaxcoreMaxmemory[0].mem) MaxcoreMaxmemory[0].mem = vmInfo->memory;
                } else if(vmInfo->isTwoNode) {
                    if(vmInfo->core / 2 > MaxcoreMaxmemory[1].core) MaxcoreMaxmemory[1].core = vmInfo->core / 2;
                    if(vmInfo->memory / 2 > MaxcoreMaxmemory[1].core) MaxcoreMaxmemory[1].core = vmInfo->memory / 2;
                }
                double coreMem = vmInfo->core * Param::Q1 + vmInfo->memory;
                coreMemRatio.emplace_back((double)vmInfo->core / vmInfo->memory, coreMem);
                coreMemTot += coreMem;
            }
        }
    }

    sort(coreMemRatio.begin(), coreMemRatio.end());
    int len = coreMemRatio.size();
    double step = coreMemTot / typesOfServer2Choose;
    ratioCoreMemSplit[0] = max(0.15, coreMemRatio[0].first);
    double nowCoreMem = coreMemRatio[0].second;
    int type = 1;
    for(int i = 1; i < len; ++i) {
        nowCoreMem += coreMemRatio[i].second;
        if(nowCoreMem > step * type - step*0.16) {//0.16最好，0.14,0.15,0.17,0.18,0.2都比较拉胯
            ratioCoreMemSplit[type++] = coreMemRatio[i].first;
            if(type == typesOfServer2Choose) break;
        }
    }
    ratioCoreMemSplit[typesOfServer2Choose] = min(6.0, coreMemRatio[len - 1].first);
// #ifdef TEST
//     cerr << "ratioCoreMemSplit: step: " << step << ", ";
//     for(int i = 0; i <= typesOfServer2Choose; ++i) {
//         cerr << ratioCoreMemSplit[i] << ", ";
//     }
//     cerr << "\n";
// #endif
}

double distance2(double x1, double y1, double x2, double y2) {
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

//选择serverCost或者powerCost来进行服务器的选择
int daySplit2ChooseServerCostOrPowerCost()
{
    return max(0,T_days-int((serCoreMemCostFit.a+serCoreMemCostFit.b)/(serCoreMemPowerFit.a+serCoreMemPowerFit.b)));
}


//MaxcoreMaxmemory
//根据core/memory的比例确定三种服务器的选择(可以是多种服务器)
void chooseServers(int leftDays) {
    //to do，下面是示意，具体怎么选可以按照对此次数据的虚拟机的core/memory分布的分析来选择服务器 lzy
    chosenServers[0].resize(typesOfServer2Choose);
    chosenServers[1].resize(typesOfServer2Choose);
    int i = 0, start;
    for(int type = 0; type < 2; ++type) {
        i = 0;
        for(int rt = 1; rt <= typesOfServer2Choose; ++rt) {
            serverInfo* nowSer = nullptr;
            serverInfo* candiate = &serverInfos[i];
            while(i < (int)serverInfos.size()) {
                if(serverInfos[i].A.core > MaxcoreMaxmemory[type].core && serverInfos[i].A.memory > MaxcoreMaxmemory[type].mem
                    //  && distance2(serverInfos[i].A.core, serverInfos[i].A.memory, MaxcoreMaxmemory[type].core, MaxcoreMaxmemory[type].mem) 
                    //  < distance2(candiate->A.core, candiate->A.memory, MaxcoreMaxmemory[type].core, MaxcoreMaxmemory[type].mem)
                     ) {
                    //&& candiate->A.core > serverInfos[i].A.core && candiate->A.memory > serverInfos[i].A.memory
                    candiate = &serverInfos[i];
                }
                double serRatio = (double)serverInfos[i].A.core / serverInfos[i].A.memory;
                if(nowSer == nullptr) {
                    // if(serverInfos[i].A.core < MaxcoreMaxmemory[type][0] + (2 - type) * 50 && serverInfos[i].A.memory < MaxcoreMaxmemory[type][1] + (2 - type) * 80 &&
                    //         serverInfos[i].A.core > MaxcoreMaxmemory[type][0] && serverInfos[i].A.memory > MaxcoreMaxmemory[type][1]) {
                    //     nowSer = &serverInfos[i];
                    // }
                    if(serverInfos[i].A.core > MaxcoreMaxmemory[type].core && serverInfos[i].A.memory > MaxcoreMaxmemory[type].mem)
                        nowSer = &serverInfos[i];
                } else {
                    // double nowCostEffect = (double)(nowSer->A.core * Param::Q1 + nowSer->A.memory) / (nowSer->serverCost + (double)leftDays * nowSer->powerCost);
                    // double costEffect = (double)(serverInfos[i].A.core * Param::Q1 + serverInfos[i].A.memory) / (serverInfos[i].serverCost + (double)leftDays * serverInfos[i].powerCost);

                    // if(costEffect > nowCostEffect && serverInfos[i].powerCost < nowSer->powerCost && serverInfos[i].A.core > MaxcoreMaxmemory[type].core
                    //         && serverInfos[i].A.memory > MaxcoreMaxmemory[type].mem) {
                    //     nowSer = &serverInfos[i];
                    // }

                    if(day_time<dayAfterChooseServerCost)// 300 400 500试过，300最好, 250跟300一样
                    {
                        if(serverInfos[i].A.core > MaxcoreMaxmemory[type].core && serverInfos[i].A.memory > MaxcoreMaxmemory[type].mem
                        //    && distance2(serverInfos[i].A.core, serverInfos[i].A.memory, MaxcoreMaxmemory[type].core, MaxcoreMaxmemory[type].mem) 
                        //         < distance2(nowSer->A.core, nowSer->A.memory, MaxcoreMaxmemory[type].core, MaxcoreMaxmemory[type].mem) 
                                && serverInfos[i].powerCost < nowSer->powerCost)
                        
                        //&& serverInfos[i].powerCost < nowSer->powerCost
                        nowSer = &serverInfos[i];
                    }
                    else 
                    {
                        if(serverInfos[i].A.core > MaxcoreMaxmemory[type].core && serverInfos[i].A.memory > MaxcoreMaxmemory[type].mem
                        //    && distance2(serverInfos[i].A.core, serverInfos[i].A.memory, MaxcoreMaxmemory[type].core, MaxcoreMaxmemory[type].mem) 
                        //         < distance2(nowSer->A.core, nowSer->A.memory, MaxcoreMaxmemory[type].core, MaxcoreMaxmemory[type].mem) 
                                && serverInfos[i].serverCost < nowSer->serverCost)
                        
                        //&& serverInfos[i].powerCost < nowSer->powerCost
                        nowSer = &serverInfos[i];                    
                    }

                    
                }
                if(serRatio > ratioCoreMemSplit[rt]) start = i;
                if(serRatio > ratioCoreMemSplit[rt] + 0.1) break;
                ++i;
            }
            i = start;
            if(nowSer == nullptr) {
                nowSer = candiate;
                cerr << "no find proper server....\n";
            }
            // assert(nowSer != nullptr);
            // cerr << *nowSer << endl;
            chosenServers[type][rt - 1] = *nowSer;
        }
    }
    vmsPlacedNewForNodeServers.resize(typesOfServer2Choose);
}

serverInfo* chooseServers(vmEntity* vm) {
    int core, mem;
    if(vm->isTwoNode) {
        core = vm->core / 2;
        mem = vm->memory / 2;
    } else {
        core = vm->core;
        mem = vm->memory;
    }
    int row = getRowOfvm2server(vm);
    for(serverInfo* ser : serverCandidates[row]) {
        if(ser->A.core >= core && ser->A.memory >= mem) {
            return ser;
        }
    }
    if(row > 0) {
        for(serverInfo* ser : serverCandidates[row - 1]) {
            if(ser->A.core >= core && ser->A.memory >= mem) {
                return ser;
            }
        }
    }
    if(row < typesOfServer2Choose - 1) {
        for(serverInfo* ser : serverCandidates[row + 1]) {
            if(ser->A.core >= core && ser->A.memory >= mem) {
                return ser;
            }
        }
    }
    // return &serverInfos[serverInfos.size() / 2];
    return &serverInfos[serverType2Info["host86WZS"]];
    //
    // return &serverInfos[serverType2Info["host9YJB1"]];
}

void bindVm2Server(vmEntity* vm, serverEntity* server, nodeType type) {
    auto it = serverForNode.find(server);
    if(it == serverForNode.end()) {
        cerr << "no found server in serverFor2Node\n";
        exit(-1);
    } else {
        serverForNode.erase(it);
    }
    vm->server = server;
    vm->serNode = type;//放置的新服务器的类型

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
    serverForNode.insert(server);
}

void unbindVm2Server(vmEntity* vm, serverEntity* server, nodeType type) {
    serverForNode.erase(server);
    if(vm->serNode == type_D) { //双节点部署
        server->deAlloc(vm->core / 2, vm->memory / 2, vm->core / 2, vm->memory / 2); //回收资源
    } else { //单节点服务器的部署
        if(vm->serNode == type_A) {
            server->deAlloc(vm->core, vm->memory, 0, 0);
        } else if(vm->serNode == type_B) {
            server->deAlloc( 0, 0, vm->core, vm->memory);
        }
    }
    serverForNode.insert(server);
}


int dpBag[MAXCORE][MAXMEM];
vector<vector<vector<int>>> pathBag;
bool solveBagProblem(vector<vmEntity*>& vms, serverEntity* server, nodeType type, bool canRefuse) {
    //对于单节点虚拟机，单个服务器需要调用两次
    //对于双节点虚拟机，匹配的时候core, mem需要除以2
    int vmSz = vms.size(), mxCore, mxMem;
    // cout << "vmSz: " << vmSz << "\n";
    if(vmSz == 0) return false;
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
    pathBag.assign(vmSz + 1, vector<vector<int>> (mxCore + 1, vector<int>(mxMem + 1, -1)));
    vector<int> value;
    vector<int> core;
    vector<int> mem;
    for(vmEntity* vm : vms) {
        value.emplace_back(vm->core + vm->memory);
        if(type == type_A || type == type_B) {
            core.emplace_back(vm->core);
            mem.emplace_back(vm->memory);
        } else {
            core.emplace_back(vm->core / 2);
            mem.emplace_back(vm->memory / 2);
            // core.emplace_back(vm->core);
            // mem.emplace_back(vm->memory);
        }
    }
    memset(dpBag, 0, sizeof(dpBag));
    // memset(pathBag, -1, sizeof(pathBag));
    for(int i = 0; i < vmSz; ++i) {
        for(int j = mxCore; j >= core[i]; --j) {
            assert(vms[i] != nullptr);
            for(int k = mxMem; k >= mem[i]; --k) {
                pathBag[i + 1][j][k] = pathBag[i][j][k];
                if(dpBag[j - core[i]][k - mem[i]] + value[i] > dpBag[j][k]) {
                    dpBag[j][k] = dpBag[j - core[i]][k - mem[i]] + value[i];
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
        if(nowIdx == -1) break;
        choose.insert(nowIdx);
        nowVal += value[nowIdx];
        nowCore -= core[nowIdx];
        nowMem -= mem[nowIdx];
    }
#ifdef TEST
    // cerr << "choose: ";
#endif
    int _core = 0, _mem = 0;
    for(int x : choose) {
#ifdef TEST
        // cerr << x << ", ";
#endif
        _core += core[x];
        _mem += mem[x];
    }
#ifdef TEST
    // cerr << "\n";
#endif
    double coreUseRatio = (double)_core / mxCore, memUseRatio = (double)_mem / mxMem;
    if(canRefuse && (coreUseRatio < 0.75 || memUseRatio < 0.75) && vms.size() < 80) {
        return false;
    }

    // cout << type <<  " core: " << _core << ", mem: " << _mem << ", server: " << mxCore << ", " << mxMem <<  "\n";
    //分配vm给对应的服务器
    vector<vmEntity*> leftVm;
    for(int i = 0; i < vmSz; ++i) {
        if(choose.count(i)) {   //分配
            bindVm2Server(vms[i], server, type);
            vmPool.insert(vms[i]->id, vms[i]);
            vmsInServers[server].insert(vms[i]);
            outManager.addOutputInfo(server->serverId, type, vms[i]->time.day, vms[i]->time.idx);
        } else {
            leftVm.emplace_back(vms[i]);
        }
    }
    //返回新的剩余的
    vms = leftVm;
    return true;
    //return leftVm;
}

vmManager vmPoolThisBatchNotProccessed;//保存这个批次里面每两个DEL之间ADD的虚拟机没有被加入到以及购买的服务器中的虚拟机，这些虚拟机后面用新买的服务器来存储
vmManager vmPoolBetweenTwoDels;//保存当前到下一个DEL之前的ADD命令

//根据虚拟机的单双节点判断服务器某个节点能否部署此虚拟机
//如果不能部署，则返回-1
//如果是单节点部署，能部署在A节点，则返回0,能部署在B节点，则返回1
//如果是双节点部署，能够部署则返回2
nodeType canLayThisVm(serverEntity*& server, vmEntity*& vm) {

    bool chooseBalance=false;
    //不加负载均衡
    //!!不加效果更好
    if(!chooseBalance)
    {
        nodeType type=type_E;
        if(!vm->isTwoNode) { //单节点服务器
            if(server->hasEnoughSpace(vm->core, vm->memory, 0, 0))
                type = type_A;
            else if(server->hasEnoughSpace( 0, 0, vm->core, vm->memory))
                type =  type_B;
        } else { //双节点服务器
            if(server->hasEnoughSpace( vm->core / 2, vm->memory / 2, vm->core / 2, vm->memory / 2)) //剩余资源大于vm需要的资源
                type = type_D;
            }
        return type;
    }

    else
    {
        nodeType type=type_E;
        if(!vm->isTwoNode) 
        { //单节点服务器
            float balanceRatio;
            float Q1=1.0;
            float Q2=1.0;
            if(server->hasEnoughSpace(vm->core, vm->memory, 0, 0))
            {
                balanceRatio=server->A.core*Q1+server->A.memory*Q2;//计算A结点的core和memory的加权和
                type=type_A;
            }

            if(server->hasEnoughSpace( 0, 0, vm->core, vm->memory))
            {
                int balanceRatioB=server->B.core*Q1+server->B.memory*Q2;
                if(type==type_A)//AB同时可选，选更均衡的
                {
                    
                    if(balanceRatioB<balanceRatio)//!! 选加权和更多的,这里反着效果还要好一些
                        type=type_B;

                    // //随即选择
                    // if(rand()%2==1)
                    // {
                    //     type=type_B;
                    // }

                }  
                else 
                {
                    type= type_B;
                }  
            }
        } 
        else 
        { //双节点服务器
            if(server->hasEnoughSpace( vm->core / 2, vm->memory / 2, vm->core / 2, vm->memory / 2)) //剩余资源大于vm需要的资源
                type= type_D;
        }
        // if(type==type_A)    cerr<<" A"<<endl;
        // if(type==type_B)    cerr<<" B"<<endl;
        return type;
    }
}


bool firstFit(vmEntity* vm) {
    bool delVmThinking=true;

    if(!delVmThinking)
    {
        //分配该虚拟机给一个服务器，服务器资源的更新
        //分配的信息包括：虚拟机所在的服务器索引以及所在的节点（需要更新server和serNode两个属性）
        if (vm->isTwoNode) {//双结点虚拟机
            //这里采用最优适配：即分配剩余最小的可用资源(先考虑核数)（跟操作系统分配内存类似）
            //暂时采用遍历的方式：如果后续需要提高效率，可设计LinkedList的方式
            // int best = -1;
            nodeType type;
            serverEntity* best = nullptr;
            for (const auto& it : serverForNode) {
                serverEntity* server = it;
                type = canLayThisVm(server, vm);
                if(type==type_D)
                {
                    if (best == nullptr) {
                        best = server;
                        break;
                    }
                }
            }
            if(best != nullptr) {
                vm->server = best;
                vm->serNode = type;
                bindVm2Server(vm, best, type);
                vmPool.insert(vm->id, vm);
                vmsInServers[best].insert(vm);
                outManager.addOutputInfo(best->serverId, type, vm->time.day, vm->time.idx);
                return true;
            }
        } else {//单节点虚拟机

            serverEntity* bestSer = nullptr;
            nodeType bestNode;
            for (const auto& it : serverForNode) {
                serverEntity* server = it;
                bestNode = canLayThisVm(server, vm);
                if(bestNode==type_A||bestNode==type_B)
                {
                    bestSer = server;
                    break;               
                }
            }
            if(bestSer != nullptr) {
                vm->server = bestSer;
                vm->serNode = bestNode;
                bindVm2Server(vm, bestSer, bestNode);
                vmPool.insert(vm->id, vm);
                vmsInServers[bestSer].insert(vm);
                outManager.addOutputInfo(bestSer->serverId, bestNode, vm->time.day, vm->time.idx);
                return true;
            }
        }
        return false;
    }
    else //!!考虑del的vm的时候
    {
        //分配该虚拟机给一个服务器，服务器资源的更新
        //分配的信息包括：虚拟机所在的服务器索引以及所在的节点（需要更新server和serNode两个属性）
        if (vm->isTwoNode) {
            //这里采用最优适配：即分配剩余最小的可用资源(先考虑核数)（跟操作系统分配内存类似）
            //暂时采用遍历的方式：如果后续需要提高效率，可设计LinkedList的方式
            // int best = -1;
            serverEntity* best = nullptr;
            //找到一个能装下的服务器
            nodeType type;
            for (const auto& it : serverForNode) {
                serverEntity* server = it;
                if(leastPriorityAddVmsServerForNode.contains(server)) continue;
                type = canLayThisVm(server, vm);
                if(type==type_D)
                {
                    best = server;
                    break;   
                }
            }
            if(best != nullptr) {
                vm->server = best;
                vm->serNode = type;
                bindVm2Server(vm, best, type);
                vmPool.insert(vm->id, vm);
                vmsInServers[best].insert(vm);
                outManager.addOutputInfo(best->serverId, type, vm->time.day, vm->time.idx);
                return true;
            } else {//非leastPriority不够
                //leastPriorityAddVmsServer
                vector<serverEntity*>& serVec = leastPriorityAddVmsServerForNode.vec;
                for(serverEntity* server: serVec) {
                    type = canLayThisVm(server, vm);
                    if (type==type_D) 
                    {
                        best = server;
                        break;
                        
                    }
                }
                if(best != nullptr) {
                    vm->server = best;
                    vm->serNode = type;
                    bindVm2Server(vm, best, type);
                    vmPool.insert(vm->id, vm);
                    vmsInServers[best].insert(vm);
                    outManager.addOutputInfo(best->serverId, type, vm->time.day, vm->time.idx);
                    return true;
                }
            }
        } 
        else {//单节点
            serverEntity* bestSer = nullptr;
            nodeType bestNode ;
            for (const auto& it : serverForNode) {
                serverEntity* server = it;
                if(leastPriorityAddVmsServerForNode.contains(server)) continue;

                //可以放置的节点
                bestNode=canLayThisVm(server, vm);
                if(bestNode==type_A||bestNode==type_B)
                {
                    bestSer = server;
                    break;   
                }
            }
            if(bestSer != nullptr) {
                vm->server = bestSer;
                vm->serNode = bestNode;
                bindVm2Server(vm, bestSer, bestNode);
                vmPool.insert(vm->id, vm);
                vmsInServers[bestSer].insert(vm);
                outManager.addOutputInfo(bestSer->serverId, bestNode, vm->time.day, vm->time.idx);
                return true;
            } else {
                //leastPriorityAddVmsServer
                vector<serverEntity*>& serVec = leastPriorityAddVmsServerForNode.vec;
                for(serverEntity* server: serVec) {
                    //节点 A
                    bestNode=canLayThisVm(server, vm);
                    if(bestNode==type_A||bestNode==type_B)
                    {
                        bestSer = server;
                        break;
                    }
                }
                if(bestSer != nullptr) {
                    vm->server = bestSer;
                    vm->serNode = bestNode;
                    bindVm2Server(vm, bestSer, bestNode);
                    vmPool.insert(vm->id, vm);
                    vmsInServers[bestSer].insert(vm);
                    outManager.addOutputInfo(bestSer->serverId, bestNode, vm->time.day, vm->time.idx);
                    return true;
                }
            }
        }
        return false;
    }
}

set<int> vmIdsInThisBatchDeleteFailed;//有可能此时的DEL的虚拟机还未加入vmPool，所以需要保存在一个vector里面，在每个batch处理完之后再进行资源回收


//根据数据结构需要修改一下
bool serverResourceRecycleOfThisVm(int vmID) { //后面可以根据单双节点分开
    //有可能此时的DEL的虚拟机还未加入vmPool，所以需要保存在一个vector里面，在每个batch处理完之后再进行资源回收
    if(!vmPool.contains(vmID)) {
        return false;
    }
    vmEntity* thisVm = vmPool.vmPool[vmID];
    serverEntity* serverOfThisVm = thisVm->server;

    if(serverForNode.find(serverOfThisVm) != serverForNode.end()) {
        if(thisVm->serNode == type_A) {
            unbindVm2Server(thisVm, serverOfThisVm, type_A);
            // serverOfThisVm->deAlloc(thisVm->core, thisVm->memory, 0, 0);
        } else if(thisVm->serNode == type_B) {
            unbindVm2Server(thisVm, serverOfThisVm, type_B);
            // serverOfThisVm->deAlloc(0, 0, thisVm->core, thisVm->memory);
        } else if(thisVm->serNode == type_D) {
            unbindVm2Server(thisVm, serverOfThisVm, type_D);
        } else {
            cerr << thisVm->vmName << ": " << thisVm->isTwoNode << ", " << thisVm->serNode << "\n";
            cerr << " not find vm " << endl;
            exit(-1);
        }
    } else {
        cerr << " not find vm int serverForNode" << endl;
        exit(-1);
    }
    return true;
}

//在做了一次bagProblem之后把前面因为没有ADD的虚拟机的资源能释放掉的都释放掉
void freeSpaceOfvmIdsInThisBatchDeleteFailed() {
    set<int> notDeleted;
    for(int vmNotDeleted : vmIdsInThisBatchDeleteFailed) {
        if(serverResourceRecycleOfThisVm(vmNotDeleted)) {
            vmEntity* vm = vmPool.find(vmNotDeleted);
            serverEntity* ser = vm->server;
            vmsInServers[ser].erase(vm);

            vmPool.remove(vmNotDeleted);
        } else {
            notDeleted.insert(vmNotDeleted);
        }
    }
    vmIdsInThisBatchDeleteFailed = notDeleted;
}


void firstFitSuccessful(vmEntity* thisVm) {
    if(!firstFit(thisVm)) {
        int row = getRowOfvm2server(thisVm);
        // serverEntity* newServer = chosenServers[thisVm->isTwoNode ? 1:0][row].clone(serverNumber);
        serverEntity* newServer = chosenServers[0][row].clone(serverNumber);
        // serverEntity* newServer = chooseServers(thisVm)->clone(serverNumber);

        if(thisVm->isTwoNode) {
            serverForNodeToday[serverNumber] = newServer;
            serverForNode.insert(newServer);
            ++serverNumber;
            bindVm2Server(thisVm, newServer, type_D);
            vmPool.insert(thisVm->id, thisVm);
            vmsInServers[newServer].insert(thisVm);
            outManager.addOutputInfo(newServer->serverId, type_D, thisVm->time.day, thisVm->time.idx);
        } else {
            serverForNodeToday[serverNumber] = newServer;
            serverForNode.insert(newServer);
            ++serverNumber;
            bindVm2Server(thisVm, newServer, type_A);
            vmPool.insert(thisVm->id, thisVm);
            vmsInServers[newServer].insert(thisVm);
            outManager.addOutputInfo(newServer->serverId, type_A, thisVm->time.day, thisVm->time.idx);
        }
    }
}


// best fit算法进行迁移，将服务器容量占用少的迁移到占用多的
void bestFit(vmEntity* vm) {
    //to do
}

//判断这个server是不是空的。
//如果全空，返回0
//如果A节点空，返回1
//如果B节点空，返回2
//如果全不空，返回3
int isEmptyOfServer( serverEntity*& server) {
    if(server->A.core == server->A.mxCore && server->A.memory == server->A.mxMemory && server->B.core == server->B.mxCore && server->B.memory == server->B.mxMemory) //剩余量=最大量
        return 0;
    else if(server->A.core == server->A.mxCore && server->A.memory == server->A.mxMemory)
        return 1;
    else if(server->B.core == server->B.mxCore && server->B.memory == server->B.mxMemory)
        return 2;
    else
        return 3;
}



//将原来位于originServer的vm服务器转移到newServer上
//1.originServer的资源回收，2.newServer资源分配，3.vm重新指向newServer
//type返回的是部署的对应服务器的节点类型，type_A,type_B,type_D
//to modify
//void bindVm2Server(vmEntity* vm, serverEntity* server, nodeType type)
void vmRePlaced(vmEntity* vm, serverEntity* originServer, serverEntity* newServer, nodeType type) {
    if(type == type_D) { //双节点部署
        serverForNode.erase(originServer);
        serverForNode.erase(newServer);
        originServer->deAlloc(vm->core / 2, vm->memory / 2, vm->core / 2, vm->memory / 2); //回收资源
        newServer->alloc(vm->core / 2, vm->memory / 2, vm->core / 2, vm->memory / 2);
        vm->server = newServer;
        serverForNode.insert(originServer);
        serverForNode.insert(newServer);
    } else { //单节点服务器的部署
        //旧服务器回收资源
        serverForNode.erase(originServer);
        serverForNode.erase(newServer);
        if(vm->serNode == type_A) {
            originServer->deAlloc(vm->core, vm->memory, 0, 0);
        } else if(vm->serNode == type_B) {
            originServer->deAlloc( 0, 0, vm->core, vm->memory);
        }


        if(type == type_A) {
            newServer->alloc(vm->core, vm->memory, 0, 0);
            vm->serNode = type_A; //虚拟机部署在新服务器的A节点上
        } else if(type == type_B) {
            newServer->alloc( 0, 0, vm->core, vm->memory);
            vm->serNode = type_B;
        }
        vm->server = newServer;
        serverForNode.insert(originServer);
        serverForNode.insert(newServer);
    }
    // unbindVm2Server(vm, originServer, type);
    // bindVm2Server(vm, newServer, type);
}


struct serCmpByCapDesc {    //将服务器按照当前core容量降序排序
    bool operator()(serverEntity* a, serverEntity* b) const {
        return (int)a->A.core * Param::Q1 + a->A.memory > (int)b->A.core * Param::Q1 + b->A.memory;
        // return a->A.core + a->A.memory > b->A.core + b->A.memory;
    }
};

typedef bool(*ServerCmp)(serverEntity*, serverEntity*);



void migrateNodeServersFirstFit(int& maxNodeServerMigrateTime, int& realMigrateTime,
                                vector<intTriple>& migration, ServerCmp cmp, double loadRatio) {
    realMigrateTime = 0;

    //2.vector从前往后遍历，首先判断服务器上是否有虚拟机，如果没有虚拟机，就直接跳过如果有虚拟机，则返回所有的虚拟机实体。
    //3.从后往前遍历，根据虚拟机的core/memory选出对应的服务器，进行资源的配置，然后返回被分配的服务器编号，并且将原服务器的资源释放
    //4.判断返回的服务器编号，如果编号相同，则表示迁移失败，没有服务器可以进行迁移，或者迁移次数达到上限，则这组服务器的迁移完成，然后进行下一组服务器的迁移

    //2.然后从后往前遍历，对于虚拟机数量不为0的，从前往后放置
    vector<serverEntity*> vecServer;
    vecServer.reserve(serverForNode.size());
    for(auto ser : serverForNode) {
        //负载较满的服务器不用进行迁移
        // if(((int)ser->A.core * Param::serverLoadBalanceRatio) < ser->A.mxCore) continue;
        if(ser->leftResRatio < loadRatio) continue;
        // if(ser->A.core==ser->A.mxCore && ser->A.memory==ser->A.mxMemory) continue;
        if(isEmptyOfServer(ser) == 0)
            continue;
        vecServer.emplace_back(ser);
    }
    // sort(vecServer.begin(), vecServer.end(), [&](serverEntity * a, serverEntity * b) {
    //     // return a->vmCnt > b->vmCnt;
    //     return a->powerCost < b->powerCost;
    // });
    sort(vecServer.begin(), vecServer.end(), cmp);
    int len = vecServer.size();
    for(int right = vecServer.size() - 1; right >= 0 && maxNodeServerMigrateTime > 0; --right) {
        //前面的虚拟机数量少的，往虚拟机数量多的服务器上放.每种服务器都要遍历一次
        //判断此服务器上是否有虚拟机
        serverEntity* ser = vecServer[right];
        //服务器为空，则跳过
        // if(isEmptyOfServer(ser) == 0)
        //     continue;
        //服务器不为空，找出此服务器上的所有虚拟机，进行资源的释放

        //3.从后往前遍历，根据虚拟机的core/memory选出对应的服务器，进行资源的配置，然后返回被分配的服务器编号，并且将原服务器的资源释放

        //map<serverEntity*,set<vmEntity*>> vmsInServers;//保存每个server对应的所有虚拟机，为了方便后续的迁移
        assert(vmsInServers.find(ser) != vmsInServers.end());

        set<vmEntity*>& vmSetOfServer = vmsInServers.find(ser)->second; //找到这个服务器上的所有虚拟机???????????????????????????????引用
        vector<vmEntity*> vmsToBeDeleted;//保存待删除的迭代器
        for(vmEntity* vm : vmSetOfServer) {
            if(maxNodeServerMigrateTime <= 0) break; //表示迁移次数不够

            int vmServerTypeIndex = getRowOfvm2server(vm); //虚拟机对应的服务器种类的下标

            int left = 0;
            for(left = 0; left < right; ++left) {
                // 找到一个对应类型的服务器,根据虚拟机判断当前服务器是否同一类型
                //返回虚拟机对应的服务器的下标
                serverEntity* newServer = vecServer[left];
                // if(newServer->serverName == chosenServers[vmServerTypeIndex].serverName)
                {
                    //虚拟机和服务器种类对应
                    //判断是否装得下，如果装得下，且不是与原服务器同样的服务器，则进行原服务器资源的释放，新部署服务器资源的占用，将需要的输出信息保存到一个位置
                    int layPlace = canLayThisVm(newServer, vm);

                    nodeType freeType;//空闲的节点
                    if(layPlace == 2) { //双节点部署，则进行服务器的迁移
                        freeType = type_D;
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        maxNodeServerMigrateTime--;
                        realMigrateTime++;

                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(vm);//保存待转移的虚拟机迭代器

                        vmRePlaced(vm, ser, newServer, freeType);
                        //migrate信息记录
                        migration.emplace_back(vm->id, newServer->serverId, 2); //双节点

                        break;//此虚拟机迁移成功，退出
                    } else if(layPlace == type_A) { //部署在A节点
                        freeType = type_A;
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        maxNodeServerMigrateTime--;
                        ++realMigrateTime;


                        vmRePlaced(vm, ser, newServer, freeType);

                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(vm);//保存待转移的虚拟机迭代器

                        migration.emplace_back(vm->id, newServer->serverId, freeType); //A节点
                        break;//此虚拟机迁移成功，退出
                    } else if (layPlace == type_B) { //部署在B节点
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        freeType = type_B;
                        maxNodeServerMigrateTime--;
                        realMigrateTime++;
                        vmRePlaced(vm, ser, newServer, freeType);
                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(vm);//保存待转移的虚拟机迭代器

                        migration.emplace_back(vm->id, newServer->serverId, freeType); //B节点
                        break;//此虚拟机迁移成功，退出
                    }
                }
            }
            // if (left == right) { //表示没有服务器可以进行迁移。
            //     break;
            // }
        }

        //删除
        for(auto& vmIt : vmsToBeDeleted)
            vmSetOfServer.erase(vmIt);
    }
}





/*begin***************************单节点服务器迁移*****************************/

void migrateLessLeftResourceToMoreForNode(vector<serverEntity*>& serverLessLeftResource, vector<serverEntity*>& serverMoreLeftResource, int& maxNodeServerMigrateTime,
        vector<intTriple>& migration, ServerCmp cmp, double loadRatio) {

    sort(serverLessLeftResource.begin(), serverLessLeftResource.end(), cmp);
    sort(serverMoreLeftResource.begin(), serverMoreLeftResource.end(), cmp);

    for(int i = serverMoreLeftResource.size() - 1; i >= 0; --i)
        // for(auto& ser: serverMoreLeftResource)//将剩余多的服务器放到剩余少的
    {
        auto ser = serverMoreLeftResource[i];
        //服务器为空，则跳过
        if(isEmptyOfServer(ser) == 0)
            continue;
        //服务器不为空，找出此服务器上的所有虚拟机，进行资源的释放
        //3.从后往前遍历，根据虚拟机的core/memory选出对应的服务器，进行资源的配置，然后返回被分配的服务器编号，并且将原服务器的资源释放

        //map<serverEntity*,set<vmEntity*>> vmsInServers;//保存每个server对应的所有虚拟机，为了方便后续的迁移
        assert(vmsInServers.find(ser) != vmsInServers.end());

        //3.从前往后遍历，根据虚拟机的core/memory选出对应的服务器，进行资源的配置，然后返回被分配的服务器编号，并且将原服务器的资源释放

        //map<serverEntity*,set<vmEntity*>> vmsInServers;//保存每个server对应的所有虚拟机，为了方便后续的迁移
        set<vmEntity*>& vmSetOfServer = vmsInServers.find(ser)->second; //找到这个服务器上的所有虚拟机
        vector<vmEntity*> vmsToBeDeleted;//保存待删除的迭代器
        for(vmEntity* vm : vmSetOfServer) {
            if(maxNodeServerMigrateTime <= 0) break; //表示迁移次数不够

            int vmServerTypeIndex = getRowOfvm2server(vm); //虚拟机对应的服务器种类的下标

            for(auto& newServer : serverLessLeftResource) {
                // 找到一个对应类型的服务器,根据虚拟机判断当前服务器是否同一类型
                //返回虚拟机对应的服务器的下标
                // if(newServer->serverName == chosenServers[0][vmServerTypeIndex].serverName)
                {
                    //虚拟机和服务器种类对应
                    //判断是否装得下，如果装得下，且不是与原服务器同样的服务器，则进行原服务器资源的释放，新部署服务器资源的占用，将需要的输出信息保存到一个位置
                    int layPlace = canLayThisVm(newServer, vm);
                    nodeType freeType;//空闲的节点
                    if(layPlace == 2) { //双节点部署，则进行服务器的迁移
                        freeType = type_D;
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        maxNodeServerMigrateTime--;

                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(vm);//保存待转移的虚拟机迭代器

                        vmRePlaced(vm, ser, newServer, freeType);
                        //migrate信息记录
                        migration.emplace_back(vm->id, newServer->serverId, 2); //双节点

                        break;//此虚拟机迁移成功，退出
                    } else if(layPlace == type_A) { //部署在A节点
                        freeType = type_A;
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        maxNodeServerMigrateTime--;

                        vmRePlaced(vm, ser, newServer, freeType);

                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(vm);//保存待转移的虚拟机迭代器

                        migration.emplace_back(vm->id, newServer->serverId, freeType); //A节点
                        break;//此虚拟机迁移成功，退出
                    } else if (layPlace == type_B) { //部署在B节点
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        freeType = type_B;
                        maxNodeServerMigrateTime--;
                        vmRePlaced(vm, ser, newServer, freeType);
                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(vm);//保存待转移的虚拟机迭代器

                        migration.emplace_back(vm->id, newServer->serverId, freeType); //B节点
                        break;//此虚拟机迁移成功，退出
                    }
                }
            }
            // if (it_forward->first == it_backward->first) { //表示没有服务器可以进行迁移。
            //     break;
            // }
        }
        //删除
        for(auto& vmIt : vmsToBeDeleted)
            vmSetOfServer.erase(vmIt);
    }
}

void migrateRecursiveForNode(vector<serverEntity*>& serverLeftResourceVec, int& maxSingleNodeServerMigrateTime, vector<intTriple>& migration, ServerCmp cmp, double loadRatio) {
    //递归地进行migrate
    //终止条件：迁移次数<=0, serverLeftResourceVec.size()<10,剩余最多的也为接近1.0
    // if(serverLeftResourceVec.size() < serverMigrateStopSize)  return;
    if(maxSingleNodeServerMigrateTime <= 0)  return;
    //!!如果剩余最多的接近1.0,也可以退出
    sort(serverLeftResourceVec.begin(), serverLeftResourceVec.end(), serCmpLeftResIncFunc); //跳过服务器都用服务器的资源剩余率

    //将vec分为两部分,跳过剩余率<0.07的服务器
    int left = 0, right = serverLeftResourceVec.size() - 1;
    for(; left <= right; ++left) {
        if(serverLeftResourceVec[left]->leftResRatio > loadRatio)
            break;
    }

    //跳过全空的服务器
    for(; left <= right; --right) {
        if(isEmptyOfServer(serverLeftResourceVec[right]) != 0)
            break;
    }

    if((right - left) < serverMigrateStopSize)  return;

    int mid = (right - left) * serverSplitLeftResourceRatio + left;
    vector<serverEntity*> serverLessLeftResource(serverLeftResourceVec.begin() + left, serverLeftResourceVec.begin() + mid);
    vector<serverEntity*> serverMoreLeftResource(serverLeftResourceVec.begin() + mid, serverLeftResourceVec.end());

    //将剩得多的往剩得少的服务器上迁移
    if(serverLessLeftResource.size() > 0)
        migrateLessLeftResourceToMoreForNode(serverLessLeftResource, serverMoreLeftResource, maxSingleNodeServerMigrateTime, migration, cmp, loadRatio);
    else {
        cerr << "serverLessLeftResource is too small!!" << endl;
        return;
    }

    migrateRecursiveForNode(serverMoreLeftResource, maxSingleNodeServerMigrateTime, migration, cmp,  loadRatio);
}


//用新方法迁移单节点服务器
void migrateFirstFitForSplitServerForNode(int& maxSingleNodeServerMigrateTime, vector<intTriple>& migration, ServerCmp cmp, double loadRatio) {
    vector<serverEntity*> serverLeftResourceVec;
    for(auto& ser : serverForNode) //直接set排好序的放进去，不用排序，前面资源剩余得少
        serverLeftResourceVec.push_back(ser);
    migrateRecursiveForNode(serverLeftResourceVec, maxSingleNodeServerMigrateTime, migration,  cmp,  loadRatio);
}

/*end*********************************单节点服务器迁移******************************/









/**begin*****************************************************************************/
//传入所有服务器vector，此函数最大迁移次数，迁移输出变量，服务器资源剩余率判断（迁移的时候用来跳过）
void migrateServersFirstFitForNode(vector<serverEntity*>& vecServerForNode,int& maxNodeServerMigrateTime,vector<intTriple>& migration,double loadRatio)
{
    vector<serverEntity*> vecServerForNodeLessFull;//保存不是很满的服务器
    for(auto& ser:vecServerForNode)
    {
        // if(isEmptyOfServer(ser)==0) continue;//跳过空的服务器
        if(ser->leftResRatio > loadRatio) 
            vecServerForNodeLessFull.push_back(ser);
        
    }

    //从右往左，将非当天del的vm迁移到前面去
    for(int right = vecServerForNodeLessFull.size() - 1; right >= 0 && maxNodeServerMigrateTime > 0; --right) {
        serverEntity* ser = vecServerForNodeLessFull[right];

        //map<serverEntity*,set<vmEntity*>> vmsInServers;//保存每个server对应的所有虚拟机，为了方便后续的迁移
        set<vmEntity*>& vmSetOfServer = vmsInServers.find(ser)->second; //找到这个服务器上的所有虚拟机
        vector<vmEntity*> vmsToBeDeleted;//保存待删除的迭代器
        for(vmEntity* vm : vmSetOfServer) {
            if(maxNodeServerMigrateTime <= 0) break; //表示迁移次数不够
            if(vmIdDeletedToday.find(vm->id)!=vmIdDeletedToday.end())   continue;//跳过需要当天需要del的虚拟机

            int vmServerTypeIndex = getRowOfvm2server(vm); //虚拟机对应的服务器种类的下标

            for(int left = 0; left < right; ++left) {//不能迁移到右边服务器
                serverEntity* newServer = vecServerForNodeLessFull[left];

                //!!跳过leastPriorityAddVmsServerForNode，非del的vm不能迁移到里面去
                if(leastPriorityAddVmsServerForNode.contains(newServer))
                    continue;

                // 找到一个对应类型的服务器,根据虚拟机判断当前服务器是否同一类型
                //返回虚拟机对应的服务器的下标
                // if(newServer->serverName == chosenServers[0][vmServerTypeIndex].serverName)
                {
                    //虚拟机和服务器种类对应
                    //判断是否装得下，如果装得下，且不是与原服务器同样的服务器，则进行原服务器资源的释放，新部署服务器资源的占用，将需要的输出信息保存到一个位置
                    nodeType layPlace = canLayThisVm(newServer, vm);

                    if(layPlace == type_D) { //双节点部署，则进行服务器的迁移
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        maxNodeServerMigrateTime--;

                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(vm);//保存待转移的虚拟机迭代器

                        vmRePlaced(vm, ser, newServer, layPlace);
                        //migrate信息记录
                        migration.emplace_back(vm->id, newServer->serverId, layPlace); //双节点

                        break;//此虚拟机迁移成功，退出
                    }else if(layPlace == type_A) { //部署在A节点
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        maxNodeServerMigrateTime--;

                        vmRePlaced(vm, ser, newServer, layPlace);

                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(vm);//保存待转移的虚拟机迭代器

                        migration.emplace_back(vm->id, newServer->serverId, layPlace); //A节点
                        break;//此虚拟机迁移成功，退出
                    } else if (layPlace == type_B) { //部署在B节点
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        maxNodeServerMigrateTime--;
                        vmRePlaced(vm, ser, newServer, layPlace);
                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(vm);//保存待转移的虚拟机迭代器

                        migration.emplace_back(vm->id, newServer->serverId, layPlace); //B节点
                        break;//此虚拟机迁移成功，退出
                    }

                }
            }
        }
        //删除
        for(auto& vmIt : vmsToBeDeleted)
            vmSetOfServer.erase(vmIt);
    }
  
}

//将vecServerForNode的今天要del的vm放到后面去
//如果放入的服务器是空的，则记为leastPriorityAddVmsServerForNode
void moveVmDelTodayToBackForNode(vector<serverEntity*>& vecServerForNode,int& maxNodeServerMigrateTime,vector<intTriple>& migration,double loadRatio)
{
    for(int left=0;left<vecServerForNode.size();++left)
    {
        serverEntity* ser = vecServerForNode[left];
        //!!跳过leastPriorityAddVmsServerFor2Node里面的vm的迁移
        if(leastPriorityAddVmsServerForNode.contains(ser))
            continue;

        set<vmEntity*>& vmSetOfServer = vmsInServers.find(ser)->second; //找到这个服务器上的所有虚拟机
        vector<vmEntity*> vmsToBeDeleted;//!!保存待从此服务器中迁移出去的虚拟机
        for(vmEntity* vm : vmSetOfServer) 
        {
            if(maxNodeServerMigrateTime <= 0) break; //表示迁移次数不够
            if(vmIdDeletedToday.find(vm->id)==vmIdDeletedToday.end())   continue;//跳过当天不需要del的虚拟机
            //将其从后往前找一个合适服务器迁移

            //把当天需要del的vm迁移到后面去
            for(int right=vecServerForNode.size()-1; left < right; --right) 
            {
                //不能迁移到右边服务器
                serverEntity* newServer = vecServerForNode[right];
              
                // 找到一个对应类型的服务器,根据虚拟机判断当前服务器是否同一类型
                //返回虚拟机对应的服务器的下标
                // if(newServer->serverName == chosenServers[1][vmServerTypeIndex].serverName)
                {
                    //虚拟机和服务器种类对应
                    //判断是否装得下，如果装得下，且不是与原服务器同样的服务器，则进行原服务器资源的释放，新部署服务器资源的占用，将需要的输出信息保存到一个位置
                    nodeType layPlace = canLayThisVm(newServer, vm);
                    if(layPlace!=type_E && isEmptyOfServer(newServer)==0)//能够放当前vm，且为全空，则在firstFit的时候先跳过
                    {
                        leastPriorityAddVmsServerForNode.push(newServer);
                    }
                    if(layPlace == type_D) { //双节点部署，则进行服务器的迁移
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(vm);//保存待转移的虚拟机迭代器
                        maxNodeServerMigrateTime--;
                        vmRePlaced(vm, ser, newServer, layPlace);
                        //migrate信息记录
                        migration.emplace_back(vm->id, newServer->serverId, layPlace); //双节点
                        break;//此虚拟机迁移成功，退出
                    }

                    else if(layPlace == type_A) { //部署在A节点
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        maxNodeServerMigrateTime--;

                        vmRePlaced(vm, ser, newServer, layPlace);

                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(vm);//保存待转移的虚拟机迭代器

                        migration.emplace_back(vm->id, newServer->serverId, layPlace); //A节点
                        break;//此虚拟机迁移成功，退出
                    } else if (layPlace == type_B) { //部署在B节点
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        maxNodeServerMigrateTime--;
                        vmRePlaced(vm, ser, newServer, layPlace);
                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(vm);//保存待转移的虚拟机迭代器

                        migration.emplace_back(vm->id, newServer->serverId, layPlace); //B节点
                        break;//此虚拟机迁移成功，退出
                    }
                }
            }
        }
        //删除
        for(auto& vmIt : vmsToBeDeleted)
            vmSetOfServer.erase(vmIt);
    }
}

/**************************************************************************************/











//第day天的迁移情况
void migrate(vector<intTriple>& migration) {
    int migrationTime = floor(vmPool.vmPool.size() * 3.0 / 100.0);
    int migrationTimeTmp = migrationTime;
    int singleRealMigrateTime = 0;

    // 创建两个线程对象,将要运行的函数作为参数

    // migrateNodeServersFirstFit(migrationTimeTmp, doubleRealMigrateTime, migration, serCmpPowerCostFunc, 0.07);

    // migrateDoubleNodeServersFirstFit(migrationTimeTmp, doubleRealMigrateTime, migrationDoubleNode, serCmpLeftResIncFunc, 0.05);
    // migrateSingleNodeServersFirstFit(migrationTimeTmp, singleRealMigrateTime, migrationSingleNode, serCmpLeftResIncFunc, 0.05);

    //迁移方式1
    // migrateNodeServersFirstFit(migrationTimeTmp, singleRealMigrateTime, migration, serCmpLeftResIncFunc, 0.02);

    //迁移方式2
    // migrateFirstFitForSplitServerForNode(migrationTimeTmp, migration, serCmpLeftResIncFunc, 0.025);






    //迁移方式3
    //将单双节点分开保存到vector里面，进行一次排序，后续的多次迁移就用这个vector，且不再重新排序
    double loadRatio=0.07;
    vector<serverEntity*> vecServerForNode;
    vecServerForNode.reserve(serverForNode.size());
    for(auto ser : serverForNode) {
        vecServerForNode.emplace_back(ser);
    }

    sort(vecServerForNode.begin(), vecServerForNode.end(), serCmpLeftResIncFunc);
    // {
    //     migrateServersFirstFitForNode( vecServerForNode, migrationTimeTmp,migration, loadRatio);
    //     moveVmDelTodayToBackForNode(vecServerForNode, migrationTimeTmp,migration,loadRatio);
    // }    
    // {
    //     migrateServersFirstFitForNode( vecServerForNode, migrationTimeTmp,migration, loadRatio);
    //     moveVmDelTodayToBackForNode(vecServerForNode, migrationTimeTmp,migration,loadRatio);
    // } 

    // {
    //     migrateServersFirstFitForNode( vecServerForNode, migrationTimeTmp,migration, loadRatio);
    //     moveVmDelTodayToBackForNode(vecServerForNode, migrationTimeTmp,migration,loadRatio);
    // } 
    // {
    //     migrateServersFirstFitForNode( vecServerForNode, migrationTimeTmp,migration, 0.06);
    //     moveVmDelTodayToBackForNode(vecServerForNode, migrationTimeTmp,migration,0.06);
    // } 


     {
        int migrateNotDelTime=migrationTimeTmp*0.9;
        migrationTimeTmp-=migrateNotDelTime;
        migrateServersFirstFitForNode( vecServerForNode, migrateNotDelTime,migration, loadRatio);
        migrationTimeTmp+=migrateNotDelTime;
        moveVmDelTodayToBackForNode(vecServerForNode, migrationTimeTmp,migration,loadRatio);
    }    
    {
        int migrateNotDelTime=migrationTimeTmp*0.9;
        migrationTimeTmp-=migrateNotDelTime;
        migrateServersFirstFitForNode( vecServerForNode, migrateNotDelTime,migration, loadRatio);
        migrationTimeTmp+=migrateNotDelTime;
        moveVmDelTodayToBackForNode(vecServerForNode, migrationTimeTmp,migration,loadRatio);
    }  
    {
        int migrateNotDelTime=migrationTimeTmp*0.9;
        migrationTimeTmp-=migrateNotDelTime;
        migrateServersFirstFitForNode( vecServerForNode, migrateNotDelTime,migration, loadRatio);
        migrationTimeTmp+=migrateNotDelTime;
        moveVmDelTodayToBackForNode(vecServerForNode, migrationTimeTmp,migration,loadRatio);
    }  

    {
        migrateServersFirstFitForNode( vecServerForNode, migrationTimeTmp,migration, 0.06);
        moveVmDelTodayToBackForNode(vecServerForNode, migrationTimeTmp,migration,0.06);
    } 

    
    leastPriorityAddVmsServerForNode.sort(serCmpLeftResDescFunc);//将剩余率多的放在前面


   //将两者加入
    migrationTimeToday = migrationTime - migrationTimeTmp;
}

//将ADD的虚拟机利用best fit策略加入到旧服务器中，如果加入成功，则返回true，否则返回false
bool vmMoveToOldServers(vmEntity* vm) {
    //to do
    return firstFit(vm);
}


const int threashForMinNumBound = 10;
const int threashForMinCoreBound = 50;
void dealAllAddBeforeDel() {
    for(int row = 0; row < (int)chosenServers[0].size(); ++row) {
        auto& vmList = vmsPlacedNewForNodeServers[row];
        sort(vmList.begin(), vmList.end(), [&](vmEntity * a, vmEntity * b) {
            return (int)a->core * Param::Q1 + a->memory > (int)b->core * Param::Q1 + b->memory;
        });
        for(vmEntity* vm : vmList) {
            firstFitSuccessful(vm);
        }
        vmsPlacedNewForNodeServers[row].clear();
    }
}

long long migrationTimeTotal = 0;

void dealOneDayOutput(int day) {
#ifdef TEST
    // cerr << "day " << day << " dealOneDayOutput\n";
#endif
    //处理purchase信息
    vector<pair<string, int>> purchase;
    //to do
    vector<serverEntity*> serverSortedToday;
    map<int, int> serverIdmapToday;
    for(auto [id, ser] : serverForNodeToday) {
        serverSortedToday.emplace_back(ser);
    }
    sort(serverSortedToday.begin(), serverSortedToday.end(), [&](serverEntity * a, serverEntity * b) {
        return a->serverName < b->serverName;
    });

    int offset = serverNumber - serverSortedToday.size();
#ifdef TEST
    // cerr << "offset: " << offset << " ,severNum: " << serverNumber << "\n";
#endif
    for(int i = 0; i < (int)serverSortedToday.size(); ++i) {
        serverIdmapToday[serverSortedToday[i]->serverId] = i + offset;
    }
    for(int i = 0; i < (int)serverSortedToday.size(); ++i) {
        serverSortedToday[i]->serverId = i + offset;
    }

    int i = 0, n = serverSortedToday.size();
    while(i < n) {
        int j = i + 1;
        while(j < n && serverSortedToday[j]->serverName == serverSortedToday[i]->serverName) {
            ++j;
        }
        purchase.emplace_back(serverSortedToday[i]->serverName, j - i);
        i = j;
    }

    serverForNodeToday.clear();
    outManager.addPurchaseInfo(purchase, day, -2);
    outManager.adjustserverIdmap(serverIdmapToday);
    outManager.flushAndStore();
    outManager.printAll();
    outManager.clear();
    fflush(stdout);
}

//parms to adjust
void processOneDay(vector<requestInfo>& oneDayRequests, int day) {
    //1. best fit算法进行迁移，将服务器容量占用少的迁移到占用多的
    vector<intTriple> migration;
    clock_t time_pre = clock();
    migrate(migration);//单线程迁移
    migrationTimeTotal += clock() - time_pre;
    outManager.addMigrationInfo(migration, day, -1);
    
    //2.对于ADD操作，每次先将服务器试着往就服务器里面放。放不下的存到vmsPlacedNewFor1NodeServers或者vmsPlacedNewFor2NodeServers,然后进行新服务器放置操作
    for (int i = 0; i < oneDayRequests.size(); ++i) {
        if(oneDayRequests[i].type == DEL) { //操作DEL
            int vmID = oneDayRequests[i].vmId;
            dealAllAddBeforeDel();
            serverResourceRecycleOfThisVm(vmID);
            vmEntity* vm = vmPool.find(vmID);
            serverEntity* ser = vm->server;
            vmsInServers[ser].erase(vm);
            vmPool.remove(vmID);
            // if(serverResourceRecycleOfThisVm(vmID)) { //回收vmID部署的服务器的资源，如果此时的虚拟机还没有放置到服务器上，则将其放到vmIdsInThisBatchDeleteFailed，然后做了一个BagProblem之后就对资源进行一次回收
            //     vmEntity* vm = vmPool.find(vmID);
            // serverEntity* ser=vm->server;
            // vmsInServers[ser].erase(vm);
            //     vmPool.remove(oneDayRequests[i].vmId);//回收当前请求的虚拟机对应的服务器的资源,
            // } else { //表示虚拟机还没有找到服务器ADD
            //     vmIdsInThisBatchDeleteFailed.insert(vmID);
            // }
        } else if(oneDayRequests[i].type == ADD) {

            //得到这个虚拟机的实体，自己可以判断是单双节点服务器
            vmEntity* vm = vmInfos[oneDayRequests[i].vmName]->clone(oneDayRequests[i].vmId,
                           oneDayRequests[i].time.day, oneDayRequests[i].time.idx);

            bool hasPlacedToOldServer = vmMoveToOldServers(vm);
            if(!hasPlacedToOldServer) { //没有成功加到旧服务器里面
                int vmIdxBelongingToservers = getRowOfvm2server(vm); //返回的是对应的服务器种类
                vmsPlacedNewForNodeServers[vmIdxBelongingToservers].push_back(vm);//将vm放置到对应的里面去
            }
        }
    }
    dealAllAddBeforeDel();
    dealOneDayOutput(day);
    leastPriorityAddVmsServerForNode.clear();
}


void refreshVmIdDeletedToday(const vector<requestInfo>& oneDayRequests)
{
    vmIdDeletedToday.clear();//清除前一天
    for (int i = 0; i < oneDayRequests.size(); ++i) {
        if(oneDayRequests[i].type == DEL) { 
            int vmID = oneDayRequests[i].vmId;
            vmIdDeletedToday.insert(vmID);
        } 
    }
}



//0:core, 1:mem, 2:cost
FaceFit faceFit2D(vector<array<double, 3>>& points) {
    double p1 = 0;//sum(memory^2)
    double p2 = 0;//sum(memory*core)
    double p3 = 0;//sum(core^2)
    double p4 = 0;//sum(core*cost)
    double p5 = 0;//sum(memory*cost)
    for(int i = 0; i < (int)points.size(); ++i) {
        p1 += (double) points[i][1] * points[i][1];
        p2 += (double) points[i][1] * points[i][0];
        p3 += (double) points[i][0] * points[i][0];
        p4 += (double) points[i][0] * points[i][2];
        p5 += (double) points[i][1] * points[i][2];
    }

    double numerator = (double) p1 * p4 - p2 * p5;
    double denominator = (double) p3 * p5 - p2 * p4;
    double det = p1 * p3 - p2 * p2;
    // return numerator / denominator;
    FaceFit res;
    res.a = numerator / det;
    res.b = denominator / det;
    return res;
    // return {numerator, denominator};
}


void getPlaneParam()
{
    //0:core, 1:mem, 2:cost
    //FaceFit faceFit2D(vector<array<int, 3>>& points) {
    //serCoreMemCostFit, serCoreMemPowerFit
    vector<array<double, 3>> _serCost, _powerCost;//分别保存core，memory和购买价格，能耗
    _serCost.resize(serverInfos.size());
    _powerCost.resize(serverInfos.size());
    for(int i = 0; i < (int)serverInfos.size(); ++i) {
        _serCost[i][0] = _powerCost[i][0] = serverInfos[i].A.mxCore + serverInfos[i].B.mxCore;
        _serCost[i][1] = _powerCost[i][1] = serverInfos[i].A.mxMemory + serverInfos[i].B.mxMemory;
        _serCost[i][2] = serverInfos[i].serverCost;
        _powerCost[i][2] = serverInfos[i].powerCost;
    }

    serCoreMemCostFit = faceFit2D(_serCost);
    serCoreMemPowerFit = faceFit2D(_powerCost);
    // Param::Q1 = serCoreMemPowerFit.a / serCoreMemPowerFit.b;
    // Param::Q1=1.0/Param::Q1;
    // Param::Q1 = serCoreMemCostFit.a / serCoreMemCostFit.b;
    // Param::splitRatioGain = Param::Q1 * 0.128;
#ifdef TEST
    cerr << "Param::Q1: " << Param::Q1 << "\n";
    cerr << "Param::splitRatioGain: " << Param::splitRatioGain << "\n";
    cerr << "serCoreMemCostFit: (" << serCoreMemCostFit.a << ", " << serCoreMemCostFit.b << ")\n";
    cerr << "serCoreMemPowerFit: (" << serCoreMemPowerFit.a << ", " << serCoreMemPowerFit.b << ")\n";
#endif
}



void readRequestThreadfunc(int totDays) {
    for(int i = 0; i < totDays; ++i) {
        cin >> R_requests;
        cin.ignore(1024, '\n');
        dailyRequests.clear();
        for (int j = 0; j < R_requests; ++j) {
            requestInfo request = readRequest();
            request.time.day = i + K_windowSize;
            request.time.idx = j;
            dailyRequests.emplace_back(request);
        }
        {
            lock_guard<mutex> guard(mutWindowReadWrite);
            windowRequests.push_back(dailyRequests);
#ifdef TEST
            if((i + K_windowSize) % 100 == 0) cerr << i + K_windowSize << " day read in\n";
#endif
        }
    }
}

int main(int argc, char** argv) {
    // speedio();   //交互式输入不能用speedio
    srand(time(NULL));
#ifdef TEST
    string filename = argv[1];
    cerr << filename << "\n";
    string input = "../training-data/training-" + filename + ".txt";
    string output = "../training-data/training-" + filename + "-output.txt";

    freopen(input.c_str(), "rb", stdin);
    freopen(output.c_str(), "w", stdout);
#endif
    cin >> N_allServerType;
#ifdef TEST
    cerr << "N_allServerType: " << N_allServerType << "\n";
#endif
    for (int i = 0; i < N_allServerType; ++i) {
        serverInfos.emplace_back(readServer());
    }

    sort(serverInfos.begin(), serverInfos.end(), [&](const serverInfo & a, const serverInfo & b) {
        return (double)a.A.core / a.A.memory < (double)b.A.core / b.A.memory;
        // return a.powerCost < b.powerCost;
    });

    for (int i = 0; i < (int)serverInfos.size(); ++i) {
        serverType2Info[serverInfos[i].serverName] = i;
    }

    getPlaneParam();//计算平面参数

    /****************************************************************/
    //将所有服务器分类
    for(int i = 0; i < (int)serverInfos.size(); ++i) {
        serverCandidates[getRowOfServer(&serverInfos[i])].emplace_back(&serverInfos[i]);
    }
    for(int i = 0; i < typesOfServer2Choose; ++i) {
        sort(serverCandidates[i].begin(), serverCandidates[i].end(), [&](serverEntity * a, serverEntity * b) {
            if(a->powerCost != b->powerCost) return a->powerCost < b->powerCost;
            return a->A.core < b->A.core;
        });
    }
    /****************************************************************/

    // serverNameToBeBought = "hostJJA26";

    cin >> N_allVmType;
#ifdef TEST
    cerr << "N_allVmType: " << N_allVmType << "\n";
#endif
    for (int i = 0; i < N_allVmType; ++i) {
        vmInfo* vm = readVm();
        vmInfos[vm->vmName] = vm;
    }

    cin >> T_days >> K_windowSize;
#ifdef TEST
    cerr << "T_days: " << T_days << ", " << "K_windowSize: " << K_windowSize << "\n";
#endif
    for(int day = 0; day < K_windowSize; ++day) {
        cin >> R_requests;
        dailyRequests.clear();
        for (int j = 0; j < R_requests; ++j) {
            requestInfo request = readRequest();
            request.time.day = day;
            request.time.idx = j;
            dailyRequests.emplace_back(request);
        }
        windowRequests.emplace_back(dailyRequests);
    }

    // thread readRequestThread(&readRequestThreadfunc, T_days - K_windowSize);
    // readRequestThread.detach();


    dayAfterChooseServerCost=daySplit2ChooseServerCostOrPowerCost();
#ifdef TEST
    cerr<<" dayAfterChooseServerCost = "<<dayAfterChooseServerCost<<endl;
#endif
    for (int day = 0; day < T_days; ++day) {
#ifdef TEST
        if(day % 100 == 0)
            cerr << "day: " << day << "\n";
#endif
        // int chooseServerStep=min(K_windowSize,1);
        int chooseServerStep=max(K_windowSize,1);//!!取windowsize为最大的时候最好，然后就是windowSize=1差不多
        if(day % chooseServerStep == 0) {
            // sort(serverInfos.begin(), serverInfos.end(), [&](const serverInfo & a, const serverInfo & b) {
            //     // return (double)a.A.core / a.A.memory < (double)b.A.core / b.A.memory;
            //     return a.powerCost < b.powerCost;
            // });
            findAllrequestMaxcoreMaxmemory();
            chooseServers(T_days - day);//选出多种类型的待选服务器
        }
        // while(windowRequests.size() == 0) {}
        // lock_guard<mutex> guard(mutWindowReadWrite);
        refreshVmIdDeletedToday(windowRequests.front());
        day_time=day;
        processOneDay(windowRequests.front(), day);//传入每天的所有请求和天数

        windowRequests.pop_front();
        if(day < T_days - K_windowSize) {
            cin >> R_requests;
            cin.ignore(1024, '\n');
            dailyRequests.clear();
            for (int j = 0; j < R_requests; ++j) {
                requestInfo request = readRequest();
                request.time.day = day + K_windowSize;
                request.time.idx = j;
                dailyRequests.emplace_back(request);
            }
            windowRequests.emplace_back(dailyRequests);
        }
    }
#ifdef TEST
    // cerr << "all request vm max core and max memory: \n";
    // for(int i = 0; i < 2; ++i) {
    //     cerr << i << ": ";
    //     for(int j = 0; j < 2; ++j)
    //         cerr << MaxcoreMaxmemory[i][j] << " ";
    //     cerr << "\n";
    // }
    // cerr << "\n";
#endif

#ifdef TEST
    cerr << (double)migrationTimeTotal / CLOCKS_PER_SEC;
#endif
    freeSysMemory();
    return 0;
}
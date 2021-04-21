// #include <bits/stdc++.h>
#include <algorithm>
#include <assert.h>
#include <climits>
#include <iostream>
#include <map>
#include <string>
#include <string.h>
#include <unordered_map>
#include <vector>
#include <set>
#include "baseStruct.h"
using namespace std;

typedef long long ll;

void speedio() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
}

//供选的服务器
vector<serverInfo> serverInfos; //这里string是唯一的

//1、按照自定义的标准（性价比： 单位核的成本（硬件+100天能耗）和单位内存的成本）排序  这部分gxf
//排序之后映射string->index;
// map<string, int> serverType2Info; //string to index
//虚拟机类型
map<string, vmInfo*> vmInfos;

int serverNumber = 0; //服务器数量（编号从0开始）单双节点一起编号
string serverNameToBeBought;//买一种服务器
map<int, serverEntity*> serverFor1Node;//保存购买的服务器，下标就是中间的购买顺序，最终重新编号的时候进行映射即可
set<int> emptyServer_A_For1Node;
set<int> emptyServer_B_For1Node;
map<int, serverEntity*> serverFor2Node;
set<int> emptyServerFor2Node;
vector<serverEntity*> serverSortByName; //用于最后编号重排和输出
map<int, int> serverIdMp;

const int typesOfServer2Choose = 3; //选择服务器的种类
vector<serverInfo> chosenServers;//保存待选的三种服务器
double ratioCoreMemSplit[typesOfServer2Choose] = {0.75, 1.25, 6};
//保存对应三种服务器的等待放到新的对应的服务器的虚拟机。
//每次添加的时候，判断如果某一行超过400个虚拟机，则做一次bagProblem，直到剩余200个。然后继续添加
vector<map<int, vmEntity*>> vmsPlacedNewFor1NodeServers;//待放到新服务器的单节点虚拟机
vector<map<int, vmEntity*>> vmsPlacedNewFor2NodeServers;//待放到新服务器的单节点虚拟机

vmManager vmPool;
vmManager vmPoolThisBatchNotProccessed;//保存这个批次里面每两个DEL之间ADD的虚拟机没有被加入到以及购买的服务器中的虚拟机，这些虚拟机后面用新买的服务器来存储
vmManager vmPoolBetweenTwoDels;//保存当前到下一个DEL之前的ADD命令
vector<requestInfo> dailyRequests; //每天的请求
vector<vector<requestInfo>> allDaysRequests;
vector<vector<requestInfo>> toBeProcessedRequests;//选出某一批次处理的request
set<int> vmIdsInThisBatchDeleteFailed;//有可能此时的DEL的虚拟机还未加入vmPool，所以需要保存在一个vector里面，在每个batch处理完之后再进行资源回收

outputManager outManager;
int N_allServerType, N_allVmType, T_days, R_requests;
const int MAXSIZE = 400;
const int MAXCORE = 800;
const int MAXMEM = 800;

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

//将serverInfos按照性价比排序
void sortServerByCostEffect(int day) {
    // serverInfos
    sort(serverInfos.begin(), serverInfos.end(), [&](const serverInfo & a, const serverInfo & b) {
        return (double)(a.A.mxCore + a.A.mxMemory) / (a.serverCost + a.powerCost * (T_days - day))
               <  (double)(b.A.mxCore + b.A.mxMemory) / (b.serverCost + b.powerCost * (T_days - day));
    });
}

//通过虚拟机选择对应的服务器数组的编号，三种服务器就是0 1 2
int getRowOfvm2server(vmEntity* vm) {
    //to do
    double ratio_core_mem = (double)vm->core / vm->memory;
    for(int i = 0; i < typesOfServer2Choose - 1; ++i) {
        if(ratio_core_mem < ratioCoreMemSplit[i]) return i;
    }
    return typesOfServer2Choose - 1;
}

//根据core/memory的比例确定三种服务器的选择(可以是多种服务器)
void chooseServers() {
    //to do，下面是示意，具体怎么选可以按照对此次数据的虚拟机的core/memory分布的分析来选择服务器 lzy
    chosenServers.resize(typesOfServer2Choose);
    int i = 0;
    for(int rt = 0; rt < typesOfServer2Choose; ++rt) {
        serverInfo* nowSer = &serverInfos[i];
        while(i < (int)serverInfos.size()) {
            double serRatio = (double)serverInfos[i].A.core / serverInfos[i].A.memory;
            if(serRatio > ratioCoreMemSplit[rt]) break;
            if(serverInfos[i].A.core > nowSer->A.core &&
                    serverInfos[i].A.core < 400 && serverInfos[i].A.memory < 400 &&
                    serverInfos[i].A.core > 100 && serverInfos[i].A.memory > 100) {
                nowSer = &serverInfos[i];
            }
            ++i;
        }
        chosenServers[rt] = *nowSer;
    }
    vmsPlacedNewFor1NodeServers.resize(typesOfServer2Choose);
    vmsPlacedNewFor2NodeServers.resize(typesOfServer2Choose);
}

void bindVm2Server(vmEntity* vm, serverEntity* server, nodeType type) {
    vm->server = server;
    vm->serNode = type;
    vmPool.insert(vm->id, vm);
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
    vmPool.remove(vm->id);
}


int dpBag[MAXCORE][MAXMEM];
vector<vector<vector<int>>> pathBag;
bool solveBagProblem(map<int, vmEntity*>& st_vms, serverEntity* server, nodeType type, bool canRefuse) {
    //对于单节点虚拟机，单个服务器需要调用两次
    //对于双节点虚拟机，匹配的时候core, mem需要除以2
    vector<vmEntity*> vms;
    for(auto [vmID, vm] : st_vms) {
        if(server->canPlaceVmAfterRecovery(vm->time, type)) {
            vms.emplace_back(vm);
        }
    }
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
        double ratio = (double)vm->core / vm->memory;
        if(ratio < 0.75) {
            value.emplace_back(vm->memory);
        } else if(ratio > 1.25) {
            value.emplace_back(vm->core);
        } else {
            value.emplace_back((vm->core + vm->memory) / 2);
        }
        // value.emplace_back(vm->core + vm->memory);
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
    // cout << "choose: ";
    int _core = 0, _mem = 0;
    for(int x : choose) {
        // cout << x << ", ";
        _core += core[x];
        _mem += mem[x];
    }
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
            outManager.addOutputInfo(server->serverId, type, vms[i]->time.day, vms[i]->time.idx);
            st_vms.erase(vms[i]->id);
        }
        // else {
        //     leftVm.emplace_back(vms[i]);
        // }
    }
    //返回新的剩余的
    // vms = leftVm;
    return true;
    //return leftVm;
}

bool firstFit(vmEntity* vm) {
    //分配
    //to do
    //分配该虚拟机给一个服务器，服务器资源的更新
    //分配的信息包括：虚拟机所在的服务器索引以及所在的节点（需要更新server和serNode两个属性）
    int core = vm->core, memory = vm->memory;
    if (vm->isTwoNode) {
        //这里采用最优适配：即分配剩余最小的可用资源(先考虑核数)（跟操作系统分配内存类似）
        //暂时采用遍历的方式：如果后续需要提高效率，可设计LinkedList的方式
        // int best = -1;
        serverEntity* best = nullptr;
        core /= 2;
        memory /= 2;
        for (const auto& it : serverFor2Node) {
            int id = it.first;
            serverEntity* server = it.second;
            if(emptyServerFor2Node.count(id)) continue;
            if(!server->canPlaceVmAfterRecovery(vm->time, type_D)) continue;
            if (server->A.core >= core && server->A.memory >= memory) {
                if (best == nullptr) {
                    best = server;
                    break;
                }
            }
        }
        if(best != nullptr) {
            vm->server = best;
            vm->serNode = type_D;
            bindVm2Server(vm, best, type_D);
            outManager.addOutputInfo(best->serverId, type_D, vm->time.day, vm->time.idx);
            return true;
        }
    } else {
        // int bestSer = -1;
        serverEntity* bestSer = nullptr;
        nodeType bestNode = type_A;
        for (const auto& it : serverFor1Node) {
            int id = it.first;
            serverEntity* server = it.second;
            if(emptyServer_A_For1Node.count(id) || emptyServer_B_For1Node.count(id)) continue;
            //节点 A
            if (server->canPlaceVmAfterRecovery(vm->time, type_A) && server->A.core >= core && server->A.memory >= memory) {
                if (bestSer == nullptr) {
                    bestSer = server;
                    bestNode = type_A;
                    break;
                }
            }
            //节点 B
            if(bestSer == nullptr) {
                if (server->canPlaceVmAfterRecovery(vm->time, type_B) && server->B.core >= core && server->B.memory >= memory) {
                    if (bestSer == nullptr) {
                        bestSer = server;
                        bestNode = type_B;
                        break;
                    }
                }
            }
        }
        if(bestSer != nullptr) {
            vm->server = bestSer;
            vm->serNode = bestNode;
            bindVm2Server(vm, bestSer, bestNode);
            outManager.addOutputInfo(bestSer->serverId, bestNode, vm->time.day, vm->time.idx);
            return true;
        }
    }
    return false;
}

//根据数据结构需要修改一下
bool serverResourceRecycleOfThisVm(int vmID) { //后面可以根据单双节点分开
    //有可能此时的DEL的虚拟机还未加入vmPool，所以需要保存在一个vector里面，在每个batch处理完之后再进行资源回收
    if(!vmPool.contains(vmID)) {
        return false;
    }
    vmEntity* thisVm = vmPool.vmPool[vmID];
    int serverIdOfThisVm = vmPool.vmPool[vmID]->server->serverId;

    //回收这个虚拟机对应的服务器的资源
    serverEntity* serverOfThisVm = NULL;

    if(thisVm->isTwoNode == 0) { //单节点
        if(serverFor1Node.find(serverIdOfThisVm) != serverFor1Node.end()) {
            serverOfThisVm = serverFor1Node[serverIdOfThisVm];
            if(thisVm->serNode == type_A) {
                serverOfThisVm->deAlloc(thisVm->core, thisVm->memory, 0, 0);
                if(serverOfThisVm->A.core == serverOfThisVm->A.mxCore
                        && serverOfThisVm->A.memory == serverOfThisVm->A.mxMemory) {
                    emptyServer_A_For1Node.insert(serverOfThisVm->serverId);
                    serverOfThisVm->setRecoveryTime(thisVm->time, type_A);
                }
            } else if(thisVm->serNode == type_B) {
                serverOfThisVm->deAlloc(0, 0, thisVm->core, thisVm->memory);
                if(serverOfThisVm->B.core == serverOfThisVm->B.mxCore
                        && serverOfThisVm->B.memory == serverOfThisVm->B.mxMemory) {
                    emptyServer_B_For1Node.insert(serverOfThisVm->serverId);
                    serverOfThisVm->setRecoveryTime(thisVm->time, type_B);
                }
            } else {
                cout << thisVm->vmName << ": " << thisVm->isTwoNode << ", " << thisVm->serNode << "\n";
                cout << " not find vm " << endl;
                exit(-1);
            }
        } else {
            cout << " not find vm int serverFor1Node" << endl;
            exit(-1);
        }
    } else if(thisVm->isTwoNode) { //双节点
        if(serverFor2Node.find(serverIdOfThisVm) != serverFor2Node.end()) {
            serverOfThisVm = serverFor2Node[serverIdOfThisVm];
            if(thisVm->serNode != type_A && thisVm->serNode != type_B) {
                serverOfThisVm->deAlloc(thisVm->core / 2, thisVm->memory / 2, thisVm->core / 2, thisVm->memory / 2);
                if(serverOfThisVm->A.core == serverOfThisVm->A.mxCore
                        && serverOfThisVm->A.memory == serverOfThisVm->A.mxMemory) {
                    emptyServerFor2Node.insert(serverOfThisVm->serverId);
                    serverOfThisVm->setRecoveryTime(thisVm->time, type_D);
                }
            } else {
                cout << thisVm->vmName << ": " << thisVm->isTwoNode << ", " << thisVm->serNode << "\n";
                cout << " not find vm " << endl;
                exit(-1);
            }
        } else {
            cout << " not find vm int serverFor2Node" << endl;
            exit(-1);
        }
    } else {
        cout << " not find corresponding vm" << endl;
        exit(-1);
    }
    return true;
}

//在做了一次bagProblem之后把前面因为没有ADD的虚拟机的资源能释放掉的都释放掉
void freeSpaceOfvmIdsInThisBatchDeleteFailed() {
    set<int> notDeleted;
    for(int vmNotDeleted : vmIdsInThisBatchDeleteFailed) {
        if(serverResourceRecycleOfThisVm(vmNotDeleted)) {
            vmPool.remove(vmNotDeleted);
        } else {
            notDeleted.insert(vmNotDeleted);
        }
    }
    vmIdsInThisBatchDeleteFailed = notDeleted;
}


//虚拟机单双节点，属于第几行，到maxVmSize数量的时候才进行BagProblem，到minVmSize的时候停止BagProblem。最终所有天数处理完了之后，需要minVmSize=0再处理一次
void proceessVmsPlacedNewServers(int isTwoNodeOfVm, int row, int minVmSize, int maxVmSize) {
    // cout << "in proceessVmsPlacedNewServers: " << isTwoNodeOfVm <<  "\n";

    if(isTwoNodeOfVm == 0) { //单节点服务器的处理
        if((int)vmsPlacedNewFor1NodeServers[row].size() > maxVmSize) { //这一行的虚拟机数量大于一定量
            while((int)vmsPlacedNewFor1NodeServers[row].size() > minVmSize) {
                if((int)emptyServer_A_For1Node.size() > 0) {
                    serverEntity* newServer = serverFor1Node[*emptyServer_A_For1Node.begin()];
                    emptyServer_A_For1Node.erase(emptyServer_A_For1Node.begin());
                    solveBagProblem(vmsPlacedNewFor1NodeServers[row], newServer, type_A, false);
                } else if((int)emptyServer_B_For1Node.size() > 0) {
                    serverEntity* newServer = serverFor1Node[*emptyServer_B_For1Node.begin()];
                    emptyServer_B_For1Node.erase(emptyServer_B_For1Node.begin());
                    solveBagProblem(vmsPlacedNewFor1NodeServers[row], newServer, type_B, false);
                } else {
                    serverEntity* newServer = chosenServers[row].clone(serverNumber);//购买一个新的服务器，clone里面的编号最终给也可以
                    // serverEntity* newServer = serverInfos.back().clone(serverNumber);
                    serverFor1Node[serverNumber] = newServer;
                    ++serverNumber;
                    if(!solveBagProblem(vmsPlacedNewFor1NodeServers[row], newServer, type_A, false)) {
                        delete newServer;
                        --serverNumber;
                        serverFor1Node.erase(serverNumber);
                        break;
                    }
                    if(vmsPlacedNewFor1NodeServers[row].size() > minVmSize) {
                        solveBagProblem(vmsPlacedNewFor1NodeServers[row], newServer, type_B, false);
                    } else {
                        emptyServer_B_For1Node.insert(serverNumber - 1);
                    }
                }
            }
            //将前面因为虚拟机还没有加而没有释放的资源释放掉
            // freeSpaceOfvmIdsInThisBatchDeleteFailed();//这个放到这里比较合理，因为反正每次bagproblem释放的资源都是旧服务器的资源，只是用来进行迁移和ADD的时候加入
        }
    } else if(isTwoNodeOfVm == 1) { //双节点服务器的处理
        if((int)vmsPlacedNewFor2NodeServers[row].size() > maxVmSize) { //这一行的虚拟机数量大于一定量
            while((int)vmsPlacedNewFor2NodeServers[row].size() > minVmSize) {
                if((int)emptyServerFor2Node.size() > 0) {
                    serverEntity* newServer = serverFor2Node[*emptyServerFor2Node.begin()];
                    emptyServerFor2Node.erase(emptyServerFor2Node.begin());
                    solveBagProblem(vmsPlacedNewFor2NodeServers[row], newServer, type_D, false);
                } else {
                    serverEntity* newServer = chosenServers[row].clone(serverNumber);//购买一个新的服务器
                    // serverEntity* newServer = serverInfos.back().clone(serverNumber);
                    serverFor2Node[serverNumber] = newServer;//新买的服务器加入
                    ++serverNumber;
                    if(!solveBagProblem(vmsPlacedNewFor2NodeServers[row], newServer, type_D, false)) {
                        delete newServer;
                        --serverNumber;
                        serverFor2Node.erase(serverNumber);
                        break;
                    }
                }
            }
            //将前面因为虚拟机还没有加而没有释放的资源释放掉
            // freeSpaceOfvmIdsInThisBatchDeleteFailed();
        }
    }
    // cout << "out proceessVmsPlacedNewServers" << "\n";
}
// best fit算法进行迁移，将服务器容量占用少的迁移到占用多的
void bestFit(vmEntity* vm) {
    //to do
}

void migrate() {
    //to do
}

//将ADD的虚拟机利用best fit策略加入到旧服务器中，如果加入成功，则返回true，否则返回false
bool vmMoveToOldServers(vmEntity* vm) {
    //to do
    return firstFit(vm);
}

void processOneDay(vector<requestInfo>& oneDayRequests, int day) {
    //1. best fit算法进行迁移，将服务器容量占用少的迁移到占用多的
    migrate();
    // sortServerByCostEffect(day);

    if(day > 0) { //处理输出信息
        vector<pair<string, int>> purchase;
        vector<intTriple> migration;
        // cout << "(purchase, 0)\n";
        // cout << "(migration, 0)\n";
        // 这里加入为空，为了匹配格式问题
        outManager.addPurchaseInfo(purchase, day, -2);
        outManager.addMigrationInfo(migration, day, -1);
    }
    //2.对于ADD操作，每次先将服务器试着往就服务器里面放。放不下的存到vmsPlacedNewFor1NodeServers或者vmsPlacedNewFor2NodeServers,然后进行新服务器放置操作
    for (int i = 0; i < oneDayRequests.size(); ++i) {
        if(oneDayRequests[i].type == DEL) { //操作DEL
            int vmID = oneDayRequests[i].vmId;
            if(serverResourceRecycleOfThisVm(vmID)) { //回收vmID部署的服务器的资源，如果此时的虚拟机还没有放置到服务器上，则将其放到vmIdsInThisBatchDeleteFailed，然后做了一个BagProblem之后就对资源进行一次回收
                vmPool.remove(oneDayRequests[i].vmId);//回收当前请求的虚拟机对应的服务器的资源,
            } else { //表示虚拟机还没有找到服务器ADD
                // vmIdsInThisBatchDeleteFailed.insert(vmID);
                // 强制回收
                vmEntity* thisVm = nullptr;
                for(int i = 0; i < (int)vmsPlacedNewFor1NodeServers.size(); ++i) {
                    if(vmsPlacedNewFor1NodeServers[i].count(vmID)) {
                        thisVm = vmsPlacedNewFor1NodeServers[i][vmID];
                        vmsPlacedNewFor1NodeServers[i].erase(vmID);
                        break;
                    }
                }
                if(thisVm == nullptr) {
                    for(int i = 0; i < (int)vmsPlacedNewFor2NodeServers.size(); ++i) {
                        if(vmsPlacedNewFor2NodeServers[i].count(vmID)) {
                            thisVm = vmsPlacedNewFor2NodeServers[i][vmID];
                            vmsPlacedNewFor2NodeServers[i].erase(vmID);
                            break;
                        }
                    }
                }
                if(thisVm != nullptr) {
                    if(!firstFit(thisVm)) {
                        int row = getRowOfvm2server(thisVm);
                        serverEntity* newServer = chosenServers[row].clone(serverNumber);
                        if(thisVm->isTwoNode) {
                            serverFor2Node[serverNumber] = newServer;
                            ++serverNumber;
                            outManager.addOutputInfo(newServer->serverId, type_D, thisVm->time.day, thisVm->time.idx);
                            newServer->setRecoveryTime(day, i, type_D);
                        } else {
                            serverFor1Node[serverNumber] = newServer;
                            ++serverNumber;
                            outManager.addOutputInfo(newServer->serverId, type_A, thisVm->time.day, thisVm->time.idx);
                            newServer->setRecoveryTime(day, i, type_A);
                        }
                    } else {
                        serverEntity* server = thisVm->server;
                        server->setRecoveryTime(day, i, thisVm->serNode);
                        unbindVm2Server(thisVm, server, thisVm->serNode);
                    }
                } else {
                    cerr << "not find the vm in the vmsPlace" << endl;
                }
            }
        } else if(oneDayRequests[i].type == ADD) {

            //得到这个虚拟机的实体，自己可以判断是单双节点服务器
            vmEntity* vm = vmInfos[oneDayRequests[i].vmName]->clone(oneDayRequests[i].vmId,
                           oneDayRequests[i].time.day, oneDayRequests[i].time.idx);

            bool hasPlacedToOldServer = vmMoveToOldServers(vm);
            if(!hasPlacedToOldServer) { //没有成功加到旧服务器里面
                int vmIdxBelongingToservers = getRowOfvm2server(vm); //返回的是对应的服务器种类
                if(vm->isTwoNode == 0) { //单节点虚拟机？？？？？这个tw判断一下
                    vmsPlacedNewFor1NodeServers[vmIdxBelongingToservers][vm->id] = vm;//将vm放置到对应的里面去
                    proceessVmsPlacedNewServers(0, vmIdxBelongingToservers, MAXSIZE / 5, MAXSIZE / 2); //-2只是为了防止越界，这里可以减去其他数都行

                } else if(vm->isTwoNode == 1) {
                    vmsPlacedNewFor2NodeServers[vmIdxBelongingToservers][vm->id] = vm;//将vm放置到对应的里面去
                    proceessVmsPlacedNewServers(1, vmIdxBelongingToservers, MAXSIZE / 5, MAXSIZE / 2);
                }
            }
        }
    }
}

//返回一个映射
void reSortServersIndex() {
    //to do  根据serverFor1Node和serverFor2Node里面每个服务器的原始购买序号映射一个新的购买序号。然后根据最终确定的顺序修改server实体的编号，则最终虚拟机指向的服务器编号也同时改变了
    serverSortByName.clear();
    //服务器旧编号-->新编号
    serverIdMp.clear();
    for(auto [id, ser] : serverFor1Node) {
        serverSortByName.emplace_back(ser);
    }
    for(auto [id, ser] : serverFor2Node) {
        serverSortByName.emplace_back(ser);
    }
    sort(serverSortByName.begin(), serverSortByName.end(), [&](serverEntity * a, serverEntity * b) {
        return a->serverName < b->serverName;
    });

    for(int i = 0; i < (int)serverSortByName.size(); ++i) {
        serverIdMp[serverSortByName[i]->serverId] = i;
    }
}

//处理请求
void process() {
    for(int i = 0; i < allDaysRequests.size(); ++i) {
        // cout << "day: " << i << "\n";
        processOneDay(allDaysRequests[i], i);//传入每天的所有请求和天数
    }
    //将剩余小于MAXSIZE/2的虚拟机都处理掉
    // cout << "before proceessVmsPlacedNewServers\n";
    for(int row = 0; row < chosenServers.size(); ++row) {
        proceessVmsPlacedNewServers(0, row, 0, 0); //单节点服务器处理
        proceessVmsPlacedNewServers(1, row, 0, 0); //双节点服务器处理
    }
    //将前面因为虚拟机还没有加而没有释放的资源释放掉
    // freeSpaceOfvmIdsInThisBatchDeleteFailed();
    reSortServersIndex();
}

//加入第一天的购买信息
void addFirstDayPurchaseInfo() {
    int i = 0, n = serverSortByName.size();
    vector<pair<string, int>> purchase;
    vector<intTriple> migration;
    while(i < n) {
        int j = i + 1;
        while(j < n && serverSortByName[j]->serverName == serverSortByName[i]->serverName) {
            ++j;
        }
        purchase.emplace_back(serverSortByName[i]->serverName, j - i);
        i = j;
    }
    outManager.addPurchaseInfo(purchase, 0, -2);
    outManager.addMigrationInfo(migration, 0, -1);
}

void input_test() {
    //serverInfo
    // cout << "server info :" << endl;
    // for(const auto& si : serverInfos) {
    //     cout << si << endl;
    // }

    // //vmInfo
    // cout << "vm info :" << endl;
    // for (auto it = vmInfos.begin(); it != vmInfos.end(); ++it) {
    //     cout << *(it->second) << endl;
    // }

    // //requestInfo
    // cout << " request info : " << endl;
    // for(const auto& reqs : allDaysRequests) {
    //     for(const auto& req : reqs)
    //         cout << req << endl;

    // }
}


int main() {
    // speedio();
#ifdef TEST
    freopen("../training-data/training-2.txt", "rb", stdin);
    freopen("../training-data/training-2-output.txt", "w", stdout);
#endif
    cin >> N_allServerType;
#ifdef TEST
    // cout << N_allServerType << "\n";
#endif
    for (int i = 0; i < N_allServerType; ++i) {
        serverInfos.emplace_back(readServer());
    }

    sort(serverInfos.begin(), serverInfos.end(), [&](const serverInfo & a, const serverInfo & b) {
        return (double)a.A.core / a.A.memory < (double)b.A.core / b.A.memory;
    });

    // for (int i = 0; i < (int)serverInfos.size(); ++i) {
    //     serverType2Info[serverInfos[i].serverName] = i;
    // }

    serverNameToBeBought = "hostJJA26";

    cin >> N_allVmType;
#ifdef TEST
    // cout << N_allVmType << "\n";
#endif
    for (int i = 0; i < N_allVmType; ++i) {
        vmInfo* vm = readVm();
        vmInfos[vm->vmName] = vm;
    }

    cin >> T_days;
#ifdef TEST
    // cout << T_days << "\n";
#endif
    for (int day = 0; day < T_days; ++day) {
        cin >> R_requests;
        dailyRequests.clear();
        for (int j = 0; j < R_requests; ++j) {
            requestInfo request = readRequest();
            request.time.day = day;
            request.time.idx = j;
            dailyRequests.emplace_back(request);
        }
        allDaysRequests.emplace_back(dailyRequests);
    }
    //input_test();
    chooseServers();//选出多种类型的待选服务器
    // for(serverInfo& info: chosenServers) {
    //     cout << info << "\n";
    // }
    process();
    // cout << "(purchase, 1)\n";
    // cout << "(" << serverNameToBeBought << ", " << to_string(serverNumber) << ")\n";
    // cout << "(migration, 0)\n";
    // for(serverSortByName)
    addFirstDayPurchaseInfo();
    outManager.adjustserverIdmap(serverIdMp);
    //因为在outManager里面要处理重编号的问题，所以flush函数只能最后调用一次
    outManager.flushAndStore();
    outManager.printAll();
    // cout << "serverNumber: " << serverNumber << "\n";
#ifdef TEST
#endif
    freeSysMemory();
    return 0;
}
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
#include <ctime>

//#include <thread> //thread 头文件,实现了有关线程的类
using namespace std;

typedef long long ll;

#define TEST


clock_t start_time,end_time;

void speedio() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
}

//供选的服务器
vector<serverInfo> serverInfos; //这里string是唯一的

//1、按照自定义的标准（性价比： 单位核的成本（硬件+100天能耗）和单位内存的成本）排序  这部分gxf
//排序之后映射string->index;
map<string, int> serverType2Info; //string to index
//虚拟机类型
map<string, vmInfo*> vmInfos;

int serverNumber = 0; //服务器数量（编号从0开始）单双节点一起编号
string serverNameToBeBought;//买一种服务器
map<int, serverEntity*> serverFor1Node;//保存购买的服务器，下标就是中间的购买顺序，最终重新编号的时候进行映射即可
map<int, serverEntity*> serverFor2Node;
vector<serverEntity*> serverSortByName; //用于最后编号重排和输出
map<int, int> serverIdMp;

const int typesOfServer2Choose = 3; //选择服务器的种类
vector<serverInfo> chosenServers;//保存待选的三种服务器
double ratioCoreMemSplit[typesOfServer2Choose] = {0.75, 1.25, 6};
//保存对应三种服务器的等待放到新的对应的服务器的虚拟机。
//每次添加的时候，判断如果某一行超过400个虚拟机，则做一次bagProblem，直到剩余200个。然后继续添加
vector<vector<vmEntity*>> vmsPlacedNewFor1NodeServers;//待放到新服务器的单节点虚拟机
vector<vector<vmEntity*>> vmsPlacedNewFor2NodeServers;//待放到新服务器的单节点虚拟机
map<serverEntity*,set<vmEntity*>> vmsInServers;//保存每个server对应的所有虚拟机，为了方便后续的迁移

vmManager vmPool;
vector<requestInfo> dailyRequests; //每天的请求
vector<vector<requestInfo>> allDaysRequests;
vector<vector<requestInfo>> toBeProcessedRequests;//选出某一批次处理的request

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
const int MAXSIZE = 400;
const int MAXCORE = 800;
const int MAXMEM = 800;

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

    vmsInServers[server].insert(vm);

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

//每次寻找一个容量剩余最少，但又能放进去的服务器部署
bool bestFit(vmEntity* vm) {
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
            if (server->A.core >= core && server->A.memory >= memory) {
                if (best == nullptr) {
                    best = server;
                    //break;
                }
                else {
                //找个一个容量剩余少的
                    if( (server->A.mxCore-server->A.core)<(best->A.mxCore-best->A.core)  )
                    {
                        best = server;
                    }
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
            //节点 A
            if (server->A.core >= core && server->A.memory >= memory) {
                if (bestSer == nullptr) {
                    bestSer = server;
                    bestNode = type_A;
                    //break;
                }
                else {
                    if( (server->A.mxCore-server->A.core)<(bestSer->A.mxCore-bestSer->A.core)  )
                    {
                        bestSer = server;
                        bestNode = type_A;
                    }
                }
            }
            //节点 B
            if(bestSer == nullptr) {
                if (server->B.core >= core && server->B.memory >= memory) {
                    if (bestSer == nullptr) {
                        bestSer = server;
                        bestNode = type_B;
                        //break;
                    }
                    else {
                        if( (server->B.mxCore-server->B.core)<(bestSer->B.mxCore-bestSer->B.core)  )
                        {
                            bestSer = server;
                            bestNode = type_B;
                        } 
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
            //节点 A
            if (server->A.core >= core && server->A.memory >= memory) {
                if (bestSer == nullptr) {
                    bestSer = server;
                    bestNode = type_A;
                    break;
                }
            }
            //节点 B
            if(bestSer == nullptr) {
                if (server->B.core >= core && server->B.memory >= memory) {
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



set<int> vmIdsInThisBatchDeleteFailed;//有可能此时的DEL的虚拟机还未加入vmPool，所以需要保存在一个vector里面，在每个batch处理完之后再进行资源回收


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
            } else if(thisVm->serNode == type_B) {
                serverOfThisVm->deAlloc(0, 0, thisVm->core, thisVm->memory);
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
            vmEntity* vm = vmPool.find(vmNotDeleted);
            serverEntity* ser=vm->server;
            vmsInServers[ser].erase(vm);

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
                serverEntity* newServer = chosenServers[row].clone(serverNumber);//购买一个新的服务器，clone里面的编号最终给也可以
                serverFor1Node[serverNumber] = newServer;
                ++serverNumber;
                if(!solveBagProblem(vmsPlacedNewFor1NodeServers[row], newServer, type_A, false)) {
                    delete newServer;
                    --serverNumber;
                    serverFor1Node.erase(serverNumber);
                    break;
                }
                if(vmsPlacedNewFor1NodeServers[row].size() > 0) {
                    if(!solveBagProblem(vmsPlacedNewFor1NodeServers[row], newServer, type_B, false)) break;
                }
            }
            //将前面因为虚拟机还没有加而没有释放的资源释放掉
            freeSpaceOfvmIdsInThisBatchDeleteFailed();//这个放到这里比较合理，因为反正每次bagproblem释放的资源都是旧服务器的资源，只是用来进行迁移和ADD的时候加入
        }
    } else if(isTwoNodeOfVm == 1) { //双节点服务器的处理
        if((int)vmsPlacedNewFor2NodeServers[row].size() > maxVmSize) { //这一行的虚拟机数量大于一定量
            while((int)vmsPlacedNewFor2NodeServers[row].size() > minVmSize) {
                serverEntity* newServer = chosenServers[row].clone(serverNumber);//购买一个新的服务器
                serverFor2Node[serverNumber] = newServer;//新买的服务器加入
                ++serverNumber;
                if(!solveBagProblem(vmsPlacedNewFor2NodeServers[row], newServer, type_D, false)) {
                    delete newServer;
                    --serverNumber;
                    serverFor2Node.erase(serverNumber);
                    break;
                }
            }
            //将前面因为虚拟机还没有加而没有释放的资源释放掉
            freeSpaceOfvmIdsInThisBatchDeleteFailed();
        }
    }
    // cout << "out proceessVmsPlacedNewServers" << "\n";
}


//判断这个server是不是空的。
//如果全空，返回0
//如果A节点空，返回1
//如果B节点空，返回2
//如果全不空，返回3
int isEmptyOfServer( serverEntity* &server)
{
    if(server->A.core==server->A.mxCore&&server->A.memory==server->A.mxMemory&&server->B.core==server->B.mxCore&&server->B.memory==server->B.mxMemory)//剩余量=最大量
        return 0;
    else if(server->A.core==server->A.mxCore&&server->A.memory==server->A.mxMemory)
        return 1;
    else if(server->B.core==server->B.mxCore&&server->B.memory==server->B.mxMemory)
        return 2;
    else 
        return 3;
}


//迁移，将服务器上虚拟机少的迁移到虚拟机多的.
//1.对单双节点的服务器按部署的虚拟机数量从少到多排序。这里不分服务器的种类
//2.遍历三种服务器，每次从前往后选出相同种类的服务器，将其虚拟机从后往前部署到同样种类的服务器上。如果某次部署之后被迁移的服务器的虚拟机数量没有减少，则表示无法再迁移，则迁移下一种服务器

void sortServerByLeftResource(map<int,serverEntity*>& serverForNode,vector<serverEntity*>& purchasedServersSortedByResourceLeft)
{
    for (auto it = serverForNode.begin(); it != serverForNode.end(); ++it) 
    {
        serverEntity *ser = it->second;
        purchasedServersSortedByResourceLeft.emplace_back(ser); //虚拟机数量和服务器编号
    }
    //将占用容量少,剩余量多的放在前面
    sort(purchasedServersSortedByResourceLeft.begin(), purchasedServersSortedByResourceLeft.end(), [&]( serverEntity*  &a, serverEntity*  &b) {
        return (a->A.core+a->A.memory +a->B.core+a->B.memory) > (b->A.core+b->A.memory +b->B.core+b->B.memory) ;
        //return (a->A.core) > (b->A.core) ;
    });
}

//为当前这个服务器在vmPool里面找到服务器上对应的虚拟机，赋值给vmInThisServer  这里必须能找到，
void findCorrespondingVmsForServer(serverEntity* server,vector<vmEntity*>& vmVec)
{
    vmVec.clear();
    for (auto &v : vmPool.vmPool) //vmId->*vmEntity
    {
        if (v.second->server == server) //从虚拟机池里找到一个对应服务器的一个虚拟机
        {
            vmVec.push_back(v.second);
        }
    }
}

//根据虚拟机的单双节点判断服务器某个节点能否部署此虚拟机
//如果不能部署，则返回-1
//如果是单节点部署，能部署在A节点，则返回0,能部署在B节点，则返回1
//如果是双节点部署，能够部署则返回2
int canLayThisVm(serverEntity* &server,vmEntity* &vm)
{
    //to  do
    if(vm->isTwoNode==0)//单节点服务器
    {
        if(server->hasEnoughSpace(vm->core, vm->memory, 0, 0))
            return 0;
        else if(server->hasEnoughSpace( 0, 0,vm->core, vm->memory))
            return 1;
        else 
        {
            return -1;
        }
    }
    else//双节点服务器 
    {
        if(server->hasEnoughSpace( vm->core/2, vm->memory/2,vm->core/2, vm->memory/2))//剩余资源大于vm需要的资源
            return 2;
        else 
        {
            return -1;
        }
    }
    cout<<" wrong judge"<<endl;
    exit(-1);
    return -1;
}

//根据当前寻找到的vm，从后往前搜索对应种类的服务器，找到一个能接收的服务器，将vm指向他，并且将这个服务器的资源减去。返回这个服务器在这个vector中的编号
int firstFitFromServervec(vmEntity* &vm,vector<serverEntity*> purchasedServersSortedByResourceLeftForNode)
{
    //to do
    return 0;
}

//将原来位于originServer的vm服务器转移到newServer上
//1.originServer的资源回收，2.newServer资源分配，3.vm重新指向newServer
//type返回的是部署的对应服务器的节点类型，type_A,type_B,type_D
void vmRePlaced(vmEntity* &vm,serverEntity* &originServer,serverEntity* &newServer,nodeType& type)
{
    if(type==type_D)//双节点部署
    {
        originServer->deAlloc(vm->core/2, vm->memory/2, vm->core/2, vm->memory/2);//回收资源
        newServer->alloc(vm->core/2, vm->memory/2, vm->core/2, vm->memory/2);
        vm->server=newServer;
    }
    else //单节点服务器的部署
    {
        //旧服务器回收资源
        if(vm->serNode==type_A)
        {
            originServer->deAlloc(vm->core, vm->memory, 0, 0);
        }
        else if(vm->serNode==type_B)
        {
            originServer->deAlloc( 0, 0,vm->core, vm->memory);
        }


        if(type==type_A)
        {
            newServer->alloc(vm->core, vm->memory, 0, 0);
            vm->serNode=type_A;//虚拟机部署在新服务器的A节点上
        }
        else if(type==type_B)
        {
            newServer->alloc( 0, 0,vm->core, vm->memory);
            vm->serNode=type_B;
        }
        vm->server=newServer;
    }
}

//单双节点分开，进行多线程

//迁移单节点服务器
void migrateSingleNodeServersBestFit(int& maxSingleNodeServerMigrateTime,int& realMigrateTime, vector<intTriple>& migration)//两个同时push可能有问题，到时可以分开，然后在合并两种服务器的节点
{
    realMigrateTime=0;
    //单节点服务器
    vector<serverEntity*> purchasedServersSortedByResourceLeftFor1Node;//根据server的剩余容量排序
    //根据虚拟机数量排好续的(由小到大放入)，我们将其保存到一个vector里面,元素是虚拟机数量和所在的server的编号
    sortServerByLeftResource(serverFor1Node, purchasedServersSortedByResourceLeftFor1Node);

    //2.然后从前往后遍历，对于虚拟机数量不为0的，从后往前放置
    for (unsigned int i = 0; i < purchasedServersSortedByResourceLeftFor1Node.size()&&maxSingleNodeServerMigrateTime>0; ++i) //前面的虚拟机数量少的，往虚拟机数量多的服务器上放.每种服务器都要遍历一次
    {
        //判断此服务器上是否有虚拟机
        serverEntity* ser=purchasedServersSortedByResourceLeftFor1Node[i];
        //服务器为空，则跳过
        if(isEmptyOfServer(ser)==0)  
            continue;
        //服务器不为空，找出此服务器上的所有虚拟机，进行资源的释放.这些虚拟机有可能是服务器A节点，有可能是B节点的
        vector<vmEntity*> vmVec;
        //返回所有的虚拟机实体
        findCorrespondingVmsForServer(ser,vmVec);//为当前服务器找到虚拟机
        
        //3.从后往前遍历，根据虚拟机的core/memory选出对应的服务器，进行资源的配置，然后返回被分配的服务器编号，并且将原服务器的资源释放
        for(auto& vm:vmVec)//迁移每个虚拟机
        {
            if(maxSingleNodeServerMigrateTime<=0) break;//表示迁移次数不够

            int vmServerTypeIndex=getRowOfvm2server(vm);//虚拟机对应的服务器种类的下标
            int j= purchasedServersSortedByResourceLeftFor1Node.size()-1;//从后往前遍历每个对应种类服务器的AB节点，找到合适的就放进去
            for(;j>i;--j)
            {
                // 找到一个对应类型的服务器,根据虚拟机判断当前服务器是否同一类型
                //返回虚拟机对应的服务器的下标
                serverEntity* newServer=purchasedServersSortedByResourceLeftFor1Node[j];
                if(newServer->serverName==chosenServers[vmServerTypeIndex].serverName)//虚拟机和服务器种类对应
                {
                    //判断是否装得下，如果装得下，且不是与原服务器同样的服务器，则进行原服务器资源的释放，新部署服务器资源的占用，将需要的输出信息保存到一个位置
                    int layPlace=canLayThisVm(newServer,vm);
                    nodeType freeType;//空闲的节点
                    if(layPlace==type_A)//部署在A节点
                    {
                        freeType=type_A;
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        maxSingleNodeServerMigrateTime--;
                        ++realMigrateTime;
                        vmRePlaced(vm,ser,newServer,freeType); 
                        migration.emplace_back(vm->id, newServer->serverId, freeType); //A节点
                        break;//此虚拟机迁移成功，退出
                    }
                    else if (layPlace==type_B) //部署在B节点
                    {
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        freeType=type_B;
                        maxSingleNodeServerMigrateTime--;
                        realMigrateTime++;
                        vmRePlaced(vm,ser,newServer,freeType);              
                        migration.emplace_back(vm->id, newServer->serverId, freeType); //B节点    
                        break;//此虚拟机迁移成功，退出      
                    }
                }
            }
            if (j == i) //表示没有服务器可以进行迁移。
            {
                break;
            }
        }
    }

}

//迁移双节点服务器
void migrateDoubleNodeServersBestFit(int& maxDoubleNodeServerMigrateTime,int& realMigrateTime,vector<intTriple>& migration)
{
    realMigrateTime=0;

    //双节点服务器
    //1.将双节点服务器按照被占用的容量从小到达排序，前面的被占用得更少
    vector<serverEntity*> purchasedServersSortedByResourceLeftFor2Node;//根据server的剩余容量排序
    //根据虚拟机数量排好续的(由小到大放入)，我们将其保存到一个vector里面,元素是虚拟机数量和所在的server的编号
    sortServerByLeftResource(serverFor2Node, purchasedServersSortedByResourceLeftFor2Node);
    
    //2.vector从前往后遍历，首先判断服务器上是否有虚拟机，如果没有虚拟机，就直接跳过如果有虚拟机，则返回所有的虚拟机实体。
    //3.从后往前遍历，根据虚拟机的core/memory选出对应的服务器，进行资源的配置，然后返回被分配的服务器编号，并且将原服务器的资源释放
    //4.判断返回的服务器编号，如果编号相同，则表示迁移失败，没有服务器可以进行迁移，或者迁移次数达到上限，则这组服务器的迁移完成，然后进行下一组服务器的迁移
    

    //2.然后从前往后遍历，对于虚拟机数量不为0的，从后往前放置
    for (unsigned int i = 0; i < purchasedServersSortedByResourceLeftFor2Node.size()&&maxDoubleNodeServerMigrateTime>0; ++i) //前面的虚拟机数量少的，往虚拟机数量多的服务器上放.每种服务器都要遍历一次
    {
        //判断此服务器上是否有虚拟机
        serverEntity* ser=purchasedServersSortedByResourceLeftFor2Node[i];
        //服务器为空，则跳过
        if(isEmptyOfServer(ser)==0)  
            continue;
        //服务器不为空，找出此服务器上的所有虚拟机，进行资源的释放
        vector<vmEntity*> vmVec;
        //返回所有的虚拟机实体
        findCorrespondingVmsForServer(ser,vmVec);//为当前服务器找到虚拟机
        
        //3.从后往前遍历，根据虚拟机的core/memory选出对应的服务器，进行资源的配置，然后返回被分配的服务器编号，并且将原服务器的资源释放
        for(auto& vm:vmVec)//迁移每个虚拟机
        {
            if(maxDoubleNodeServerMigrateTime<=0) break;//表示迁移次数不够

            int vmServerTypeIndex=getRowOfvm2server(vm);//虚拟机对应的服务器种类的下标
            int j= purchasedServersSortedByResourceLeftFor2Node.size()-1;//从后往前遍历
            for(;j>i;--j)
            {
                // 找到一个对应类型的服务器,根据虚拟机判断当前服务器是否同一类型
                //返回虚拟机对应的服务器的下标
                serverEntity* newServer=purchasedServersSortedByResourceLeftFor2Node[j];
                if(newServer->serverName==chosenServers[vmServerTypeIndex].serverName)//虚拟机和服务器种类对应
                {
                    //判断是否装得下，如果装得下，且不是与原服务器同样的服务器，则进行原服务器资源的释放，新部署服务器资源的占用，将需要的输出信息保存到一个位置
                    int layPlace=canLayThisVm(newServer,vm);
                    nodeType freeType;//空闲的节点
                    if(layPlace==2)//双节点部署，则进行服务器的迁移
                    {
                        freeType=type_D;
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        maxDoubleNodeServerMigrateTime--;
                        realMigrateTime++;
                        vmRePlaced(vm,ser,newServer,freeType);
                        //migrate信息记录
                        migration.emplace_back(vm->id, newServer->serverId, 2); //双节点
                        break;//此虚拟机迁移成功，退出
                    }
                }
            }
            if (j == i) //表示没有服务器可以进行迁移。
            {
                break;
            }
        }
    }

}


//每次都将末尾服务器的虚拟机往前面迁移，这样就有可能将末尾的服务器空出来，省一点能耗
//迁移单节点服务器
void migrateSingleNodeServersFirstFit(int& maxSingleNodeServerMigrateTime,int& realMigrateTime, vector<intTriple>& migration)//两个同时push可能有问题，到时可以分开，然后在合并两种服务器的节点
{
    realMigrateTime=0;
    //2.然后从前往后遍历，对于虚拟机数量不为0的，从后往前放置
    for (auto it_backward = serverFor1Node.rbegin(); it_backward != serverFor1Node.rend()&&maxSingleNodeServerMigrateTime>0; ++it_backward) //前面的虚拟机数量少的，往虚拟机数量多的服务器上放.每种服务器都要遍历一次
    {
        //判断此服务器上是否有虚拟机
        serverEntity* ser=it_backward->second;
        //服务器为空，则跳过
        if(isEmptyOfServer(ser)==0)  
            continue;
      
        //3.从前往后遍历，根据虚拟机的core/memory选出对应的服务器，进行资源的配置，然后返回被分配的服务器编号，并且将原服务器的资源释放

        //map<serverEntity*,set<vmEntity*>> vmsInServers;//保存每个server对应的所有虚拟机，为了方便后续的迁移
        set<vmEntity*>& vmSetOfServer=vmsInServers.find(ser)->second;//找到这个服务器上的所有虚拟机
        vector<set<vmEntity*>::iterator> vmsToBeDeleted;//保存待删除的迭代器
        for (auto it_set=vmSetOfServer.begin(); it_set!=vmSetOfServer.end(); ++it_set)//转移每个虚拟机
        {
            vmEntity* vm=*it_set;
            if(maxSingleNodeServerMigrateTime<=0) break;//表示迁移次数不够

            int vmServerTypeIndex=getRowOfvm2server(vm);//虚拟机对应的服务器种类的下标

            auto it_forward = serverFor1Node.begin();
            for(;it_forward->first!=it_backward->first;++it_forward)
            {
                // 找到一个对应类型的服务器,根据虚拟机判断当前服务器是否同一类型
                //返回虚拟机对应的服务器的下标
                serverEntity* newServer=it_forward->second;
                if(newServer->serverName==chosenServers[vmServerTypeIndex].serverName)//虚拟机和服务器种类对应
                {
                    //判断是否装得下，如果装得下，且不是与原服务器同样的服务器，则进行原服务器资源的释放，新部署服务器资源的占用，将需要的输出信息保存到一个位置
                    int layPlace=canLayThisVm(newServer,vm);
                    nodeType freeType;//空闲的节点
                    if(layPlace==type_A)//部署在A节点
                    {
                        freeType=type_A;
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        maxSingleNodeServerMigrateTime--;
                        ++realMigrateTime;


                        vmRePlaced(vm,ser,newServer,freeType); 

                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(it_set);//保存待转移的虚拟机迭代器

                        migration.emplace_back(vm->id, newServer->serverId, freeType); //A节点
                        break;//此虚拟机迁移成功，退出
                    }
                    else if (layPlace==type_B) //部署在B节点
                    {
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        freeType=type_B;
                        maxSingleNodeServerMigrateTime--;
                        realMigrateTime++;
                        vmRePlaced(vm,ser,newServer,freeType);   
                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(it_set);//保存待转移的虚拟机迭代器                        

                        migration.emplace_back(vm->id, newServer->serverId, freeType); //B节点    
                        break;//此虚拟机迁移成功，退出      
                    }
                }
            }
            if (it_forward->first==it_backward->first) //表示没有服务器可以进行迁移。
            {
                break;
            }
        }
        //删除
        for(auto& vmIt:vmsToBeDeleted)
            vmSetOfServer.erase(vmIt);
    }

}

 double totalFindCorrespondingVmsForServerTime{0};      

//迁移双节点服务器
void migrateDoubleNodeServersFirstFit(int& maxDoubleNodeServerMigrateTime,int& realMigrateTime,vector<intTriple>& migration)
{
    realMigrateTime=0;

    //2.vector从前往后遍历，首先判断服务器上是否有虚拟机，如果没有虚拟机，就直接跳过如果有虚拟机，则返回所有的虚拟机实体。
    //3.从后往前遍历，根据虚拟机的core/memory选出对应的服务器，进行资源的配置，然后返回被分配的服务器编号，并且将原服务器的资源释放
    //4.判断返回的服务器编号，如果编号相同，则表示迁移失败，没有服务器可以进行迁移，或者迁移次数达到上限，则这组服务器的迁移完成，然后进行下一组服务器的迁移
    
    //2.然后从后往前遍历，对于虚拟机数量不为0的，从前往后放置
    for (auto it_backward = serverFor2Node.rbegin(); it_backward != serverFor2Node.rend()&&maxDoubleNodeServerMigrateTime>0; ++it_backward) //前面的虚拟机数量少的，往虚拟机数量多的服务器上放.每种服务器都要遍历一次
    {
        //判断此服务器上是否有虚拟机
        serverEntity* ser=it_backward->second;
        //服务器为空，则跳过
        if(isEmptyOfServer(ser)==0)  
            continue;
        //服务器不为空，找出此服务器上的所有虚拟机，进行资源的释放
        
        clock_t find_start=clock();
        clock_t	find_end=clock();		//程序结束用时
        totalFindCorrespondingVmsForServerTime+=(double)(find_end-find_start)/CLOCKS_PER_SEC;


        //3.从后往前遍历，根据虚拟机的core/memory选出对应的服务器，进行资源的配置，然后返回被分配的服务器编号，并且将原服务器的资源释放

        //map<serverEntity*,set<vmEntity*>> vmsInServers;//保存每个server对应的所有虚拟机，为了方便后续的迁移
        assert(vmsInServers.find(ser)!=vmsInServers.end());
        

        set<vmEntity*>& vmSetOfServer=vmsInServers.find(ser)->second;//找到这个服务器上的所有虚拟机???????????????????????????????引用
        vector<set<vmEntity*>::iterator> vmsToBeDeleted;//保存待删除的迭代器
        for (auto it_set=vmSetOfServer.begin(); it_set!=vmSetOfServer.end(); ++it_set)//转移每个虚拟机
        {
            vmEntity* vm=*it_set;
            if(maxDoubleNodeServerMigrateTime<=0) break;//表示迁移次数不够

            int vmServerTypeIndex=getRowOfvm2server(vm);//虚拟机对应的服务器种类的下标
            
            auto it_forward = serverFor2Node.begin();
            for(;it_forward->first!=it_backward->first;++it_forward)//服务器从前往后遍历
            {
                // 找到一个对应类型的服务器,根据虚拟机判断当前服务器是否同一类型
                //返回虚拟机对应的服务器的下标
                serverEntity* newServer=it_forward->second;
                //cout<<"38 "<<endl;
                if(newServer->serverName==chosenServers[vmServerTypeIndex].serverName)//虚拟机和服务器种类对应
                {
                    //判断是否装得下，如果装得下，且不是与原服务器同样的服务器，则进行原服务器资源的释放，新部署服务器资源的占用，将需要的输出信息保存到一个位置
                    int layPlace=canLayThisVm(newServer,vm);

                    nodeType freeType;//空闲的节点
                    if(layPlace==2)//双节点部署，则进行服务器的迁移
                    {
                        freeType=type_D;
                        //新服务器的资源的消耗，旧服务器资源的回收
                        //迁移次数的增加
                        maxDoubleNodeServerMigrateTime--;
                        realMigrateTime++;



                        vmsInServers[newServer].insert(vm);
                        vmsToBeDeleted.push_back(it_set);//保存待转移的虚拟机迭代器



                        vmRePlaced(vm,ser,newServer,freeType);
                        //migrate信息记录
                        migration.emplace_back(vm->id, newServer->serverId, 2); //双节点
                      
                        break;//此虚拟机迁移成功，退出
                    }
                }
            }
            if (it_forward->first==it_backward->first) //表示没有服务器可以进行迁移。
            {
                break;
            }
        }

        //删除
        for(auto& vmIt:vmsToBeDeleted)
            vmSetOfServer.erase(vmIt);
    }

}


int migrationTimeToday;//记录某天迁移的次数

/*
#ifdef MULTI_THREAD
//双线程迁移
void migrateDoubleThread(vector<intTriple>& migration)
{
    int migrationTime = floor(vmPool.vmPool.size() * 5.0 / 1000.0);
    vector<intTriple> migrationSingleNode;
    vector<intTriple> migrationDoubleNode;

    // 创建两个线程对象,将要运行的函数作为参数
    int singleRealMigrateTime=0;
    int doubleRealMigrateTime=0;

    std::thread t1(migrateSingleNodeServersBestFit,migrationTime/2,ref(singleRealMigrateTime) ,ref(migrationSingleNode));
    std::thread t2(migrateDoubleNodeServersBestFit,migrationTime/2,ref(doubleRealMigrateTime),ref(migrationDoubleNode));
    // join()函数,运行线程
    t1.join();
    t2.join();

    migration.clear();
    migration.reserve(migrationSingleNode.size()+migrationDoubleNode.size()+10);
    for(auto& m:migrationSingleNode)
        migration.push_back(m);
    for(auto& m:migrationDoubleNode)
        migration.push_back(m);    

    //将两者加入
    migrationTimeToday =  singleRealMigrateTime+doubleRealMigrateTime;
}
#endif
*/
//第day天的迁移情况
void migrateSingleThread(vector<intTriple>& migration) {
    int migrationTime = floor(vmPool.vmPool.size() * 5.0 / 1000.0);
    int migrationTimeTmp=migrationTime;
    vector<intTriple> migrationSingleNode;
    vector<intTriple> migrationDoubleNode;
    
    // 创建两个线程对象,将要运行的函数作为参数
    int singleRealMigrateTime=0;
    int doubleRealMigrateTime=0;
    /*
    //BestFit
    migrateDoubleNodeServersBestFit(migrationTimeTmp,doubleRealMigrateTime,migrationDoubleNode);
    migrateSingleNodeServersBestFit(migrationTimeTmp,singleRealMigrateTime ,migrationSingleNode);
    */
    //FirstFit
    migrateDoubleNodeServersFirstFit(migrationTimeTmp,doubleRealMigrateTime,migrationDoubleNode);
    //migrateSingleNodeServersFirstFit(migrationTimeTmp,singleRealMigrateTime ,migrationSingleNode);

    migration.clear();
    migration.reserve(migrationSingleNode.size()+migrationDoubleNode.size()+10);
    for(auto& m:migrationSingleNode)
        migration.push_back(m);
    for(auto& m:migrationDoubleNode)
        migration.push_back(m);    
    //将两者加入
    migrationTimeToday = singleRealMigrateTime + doubleRealMigrateTime;
}


//结论：bestFit效果更差，可能需要搭配meigrate效果会好一点

//将ADD的虚拟机利用best fit策略加入到旧服务器中，如果加入成功，则返回true，否则返回false
bool vmMoveToOldServers(vmEntity* vm) {
    //return bestFit(vm);
    return firstFit(vm);
}

float totalMigrationTime{0};

void processOneDay(vector<requestInfo>& oneDayRequests, int day) {
    //1. best fit算法进行迁移，将服务器容量占用少的迁移到占用多的
    vector<pair<string, int>> purchase;//保存购买信息
    vector<intTriple> migration;//保存迁移信息
    
    if(day > 0) { //处理输出信息
 
        // cout << "(purchase, 0)\n";
        // cout << "(migration, 0)\n";
        // 这里加入为空，为了匹配格式问题

        clock_t   mig_start=clock();


        migrateSingleThread(migration);//单线程迁移

        //migrationTimeToday=0;

        clock_t	mig_end=clock();		//程序结束用时
        totalMigrationTime+=(double)(mig_end-mig_start)/CLOCKS_PER_SEC;

        //migrate(migration);//单线程迁移
        //migrateDoubleThread(migration);//多线程迁移
        outManager.addPurchaseInfo(purchase, day, -2);
        outManager.addMigrationInfo(migration, day, -1);
    }
    //2.对于ADD操作，每次先将服务器试着往就服务器里面放。放不下的存到vmsPlacedNewFor1NodeServers或者vmsPlacedNewFor2NodeServers,然后进行新服务器放置操作
    for (unsigned int i = 0; i < oneDayRequests.size(); ++i) {
        if(oneDayRequests[i].type == DEL) { //操作DEL
            int vmID = oneDayRequests[i].vmId;
            if(serverResourceRecycleOfThisVm(vmID)) 
            { //回收vmID部署的服务器的资源，如果此时的虚拟机还没有放置到服务器上，则将其放到vmIdsInThisBatchDeleteFailed，然后做了一个BagProblem之后就对资源进行一次回收
                vmEntity* vm = vmPool.find(vmID);
                serverEntity* ser=vm->server;
                vmsInServers[ser].erase(vm);
                vmPool.remove(oneDayRequests[i].vmId);//回收当前请求的虚拟机对应的服务器的资源
            } else { //表示虚拟机还没有找到服务器ADD
                vmIdsInThisBatchDeleteFailed.insert(vmID);
            }
        } else if(oneDayRequests[i].type == ADD) {

            //得到这个虚拟机的实体，自己可以判断是单双节点服务器
            vmEntity* vm = vmInfos[oneDayRequests[i].vmName]->clone(oneDayRequests[i].vmId,
                           oneDayRequests[i].time.day, oneDayRequests[i].time.idx);

            bool hasPlacedToOldServer = vmMoveToOldServers(vm);
            if(!hasPlacedToOldServer) { //没有成功加到旧服务器里面
                int vmIdxBelongingToservers = getRowOfvm2server(vm); //返回的是对应的服务器种类
                if(vm->isTwoNode == 0) { 
                    vmsPlacedNewFor1NodeServers[vmIdxBelongingToservers].push_back(vm);//将vm放置到对应的里面去
                    proceessVmsPlacedNewServers(0, vmIdxBelongingToservers, MAXSIZE / 10, MAXSIZE / 4); //-2只是为了防止越界，这里可以减去其他数都行

                } else if(vm->isTwoNode == 1) {
                    vmsPlacedNewFor2NodeServers[vmIdxBelongingToservers].push_back(vm);//将vm放置到对应的里面去
                    proceessVmsPlacedNewServers(1, vmIdxBelongingToservers, MAXSIZE / 10, MAXSIZE / 4);
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
    /*
    for(auto [id, ser] : serverFor1Node) {
        serverSortByName.emplace_back(ser);
    }
    for(auto [id, ser] : serverFor2Node) {
        serverSortByName.emplace_back(ser);
    }
    */
  	for(auto it : serverFor1Node) {
		auto ser=it.second;
        serverSortByName.emplace_back(ser);
    }

    for(auto it: serverFor2Node) {
		auto ser=it.second;
        serverSortByName.emplace_back(ser);

    }    
    
    sort(serverSortByName.begin(), serverSortByName.end(), [&](serverEntity * a, serverEntity * b) {
        return a->serverName < b->serverName;
    });

    for(unsigned int i = 0; i < (int)serverSortByName.size(); ++i) {
        serverIdMp[serverSortByName[i]->serverId] = i;
    }
}

//处理请求
void process() {
    for(unsigned int i = 0; i < allDaysRequests.size(); ++i) {
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
    freeSpaceOfvmIdsInThisBatchDeleteFailed();
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


int N_allServerType, N_allVmType, T_days, R_requests;
int main() {
    start_time=clock();		//程序开始计时
    // speedio();
#ifdef TEST
    freopen("../training-data/training-2.txt", "rb", stdin);
    freopen("../training-data/training-2-output.txt", "w", stdout);
#endif
    cin >> N_allServerType;
#ifdef TEST
    // cout << N_allServerType << "\n";
#endif
    for (unsigned int i = 0; i < N_allServerType; ++i) {
        serverInfos.emplace_back(readServer());
    }

    sort(serverInfos.begin(), serverInfos.end(), [&](const serverInfo & a, const serverInfo & b) {
        return (double)a.A.core / a.A.memory < (double)b.A.core / b.A.memory;
    });

    for (unsigned int i = 0; i < (int)serverInfos.size(); ++i) {
        serverType2Info[serverInfos[i].serverName] = i;
    }

    serverNameToBeBought = "hostJJA26";

    cin >> N_allVmType;
#ifdef TEST
    // cout << N_allVmType << "\n";
#endif
    for (unsigned int i = 0; i < N_allVmType; ++i) {
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
	end_time=clock();		//程序结束用时
    double endtime=(double)(end_time-start_time)/CLOCKS_PER_SEC;
    //cout<<"Total time:"<<endtime<<endl;		//s为单位
    //cout<<"migration time:"<<totalMigrationTime<<endl;		//s为单位
    //cout<<"totalFindCorrespondingVmsForServer time:"<<totalFindCorrespondingVmsForServerTime<<endl;		//s为单位
    
    return 0;
}

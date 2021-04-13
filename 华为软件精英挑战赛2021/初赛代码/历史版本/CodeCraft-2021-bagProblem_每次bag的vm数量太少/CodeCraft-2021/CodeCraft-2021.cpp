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
//---2、直接按照规格大小排序，然后每隔多少步取一种。假如取四种---
//排序之后映射string->index;
map<string, int> serverType2Info; //string to index
//虚拟机类型
map<string, vmInfo*> vmInfos;

int serverNumber = 0; //服务器数量（编号从0开始）单双节点一起编号
string serverNameToBeBought;//买一种服务器
map<int, serverEntity*> serverFor1Node;
map<int, serverEntity*> serverFor2Node;

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



void bindVm2Server(vmEntity* vm, serverEntity* server, nodeType type) {
    vm->server = server->serverId;
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
}

const int MAXSIZE = 200;//每次背包算法能处理的最大虚拟机数量
const int MAXCORE = 500;
const int MAXMEM = 600;
int dpBag[MAXCORE][MAXMEM];
int pathBag[MAXSIZE][MAXCORE][MAXMEM];
void solveBagProblem(vector<vmEntity*>& vms, serverEntity* server, nodeType type) {
    // for(vmEntity* vm: vms) {
    //     vmPool.insert(vm->id, vm);
    // }
    // vms.clear();
    //对于单节点虚拟机，单个服务器需要调用两次
    //对于双节点虚拟机，匹配的时候core, mem需要除以2
    int vmSz = vms.size(), mxCore, mxMem;
    if(vmSz == 0) return;
    cout << " vmSz size = " << vmSz << endl;
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
            assert(vms[i] != nullptr);
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
        if(nowIdx == -1) break;
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
}

vmManager vmPoolThisBatchNotProccessed;//保存这个批次里面每两个DEL之间ADD的虚拟机没有被加入到以及购买的服务器中的虚拟机，这些虚拟机后面用新买的服务器来存储
vmManager vmPoolBetweenTwoDels;//保存当前到下一个DEL之前的ADD命令

void migrate() { //每天之前执行migrate的操作

}

//处理两个DEL之间的虚拟机vmPoolBetweenTwoDels，没处理完的加入到vmPoolThisBatchNotProccessed里面，等着最后加入到新服务器里面
void proceessVmPoolBetweenTwoDels() {
    if(vmPoolBetweenTwoDels.vmPool.size() == 0)
        return;

    vector<vmEntity*> vmsOfSingleNode; //单节点虚拟机
    vmsOfSingleNode.resize(0);
    vector<vmEntity*> vmsOfDoubleNode; //双节点虚拟机
    vmsOfDoubleNode.resize(0);

    for (map<int, vmEntity*>::iterator it = vmPoolBetweenTwoDels.vmPool.begin(); it != vmPoolBetweenTwoDels.vmPool.end(); ++it) {
        vmEntity* vm = it->second;
        if(vm->isTwoNode) //双节点部署
            vmsOfDoubleNode.push_back(vm);
        else {
            vmsOfSingleNode.push_back(vm);
        }
    }

    //单节点部署，每台已购买的server执行两次
    for (map<int, serverEntity*>::iterator it = serverFor1Node.begin(); it != serverFor1Node.end(); ++it) {
        serverEntity* server = it->second;
        if(vmsOfSingleNode.size() == 0)
            break;
        if(server->A.core > 50 && server->A.memory > 50) solveBagProblem(vmsOfSingleNode, it->second, type_A);
        if(vmsOfSingleNode.size() == 0)
            break;
        if(server->B.core > 50 && server->B.memory > 50) solveBagProblem(vmsOfSingleNode, it->second, type_B);
    }
    //把剩余的加入到vmPoolThisBatchNotProccessed
    for(auto& vm : vmsOfSingleNode) {
        vmPoolThisBatchNotProccessed.insert(vm->id, vm);
    }

    //处理双节点部署的
    for (map<int, serverEntity*>::iterator it = serverFor2Node.begin(); it != serverFor2Node.end(); ++it) {
        serverEntity* server = it->second;
        if(vmsOfDoubleNode.size() == 0)
            break;
        if(server->A.core > 50 && server->A.memory > 50) solveBagProblem(vmsOfDoubleNode, it->second, type_D);
    }
    //把剩余的加入到vmPoolThisBatchNotProccessed
    for(auto& vm : vmsOfDoubleNode) {
        vmPoolThisBatchNotProccessed.insert(vm->id, vm);
    }
    vmPoolBetweenTwoDels.vmPool.clear();//清除这个变量
}

//将这里的所有虚拟机塞到新买的服务器,每次调用背包函数的虚拟机数量最多为MAXSIZE
void proceessVmPoolThisBatchNotProccessed() {
    if(vmPoolThisBatchNotProccessed.vmPool.size() == 0)
        return;
    // cout << "here 0..\n";
    cout<<" vmPoolThisBatchNotProccessed.vmPool.size() ="<<vmPoolThisBatchNotProccessed.vmPool.size() <<endl;
    vector<vmEntity*> vmsOfSingleNode; //单节点虚拟机
    vector<vmEntity*> vmsOfDoubleNode; //双节点虚拟机
    for (map<int, vmEntity*>::iterator it = vmPoolThisBatchNotProccessed.vmPool.begin(); it != vmPoolThisBatchNotProccessed.vmPool.end(); ++it) {
        vmEntity* vm = it->second;
        if(vm->isTwoNode==0) //单节点部署
        {
            vmsOfSingleNode.push_back(vm);
            if(vmsOfSingleNode.size()>MAXSIZE-5)
            {
                //处理单节点服务器
                while((int)vmsOfSingleNode.size() > 30)//??这里对于每个批次可能不需要到0，可能最后购买的一个服务器浪费较多
                {
                    serverEntity* newServer = serverInfos[serverType2Info[serverNameToBeBought]].clone(serverNumber);//购买一个新的服务器
                    serverFor1Node[serverNumber] = newServer;
                    solveBagProblem(vmsOfSingleNode, newServer, type_A);
                    if(vmsOfSingleNode.size() > 0) {
                        solveBagProblem(vmsOfSingleNode, newServer, type_B);}
                    ++serverNumber;
                }
            }
        }
        else // 双节点
        {
            vmsOfDoubleNode.push_back(vm);
            if(vmsOfDoubleNode.size()>MAXSIZE-5)
            {
                //处理双节点服务器
                while(vmsOfDoubleNode.size() > 30) 
                {
                    serverEntity* newServer = serverInfos[serverType2Info[serverNameToBeBought]].clone(serverNumber);//购买一个新的服务器
                    serverFor2Node[serverNumber] = newServer;
                    solveBagProblem(vmsOfDoubleNode, newServer, type_D);
                    ++serverNumber;
                }
            }
        }
    }

    //处理单节点服务器
    while((int)vmsOfSingleNode.size() > 0)//??这里对于每个批次可能不需要到0，可能最后购买的一个服务器浪费较多
    {
        serverEntity* newServer = serverInfos[serverType2Info[serverNameToBeBought]].clone(serverNumber);//购买一个新的服务器
        serverFor1Node[serverNumber] = newServer;
        solveBagProblem(vmsOfSingleNode, newServer, type_A);
        if(vmsOfSingleNode.size() > 0) {
            solveBagProblem(vmsOfSingleNode, newServer, type_B);
        ++serverNumber;
        }
    }
    //处理双节点服务器
    while(vmsOfDoubleNode.size() > 0) 
    {
        serverEntity* newServer = serverInfos[serverType2Info[serverNameToBeBought]].clone(serverNumber);//购买一个新的服务器
        serverFor2Node[serverNumber] = newServer;
        solveBagProblem(vmsOfDoubleNode, newServer, type_D);
        ++serverNumber;
    }
    vmPoolThisBatchNotProccessed.vmPool.clear();
}

vector<int> vmIdsInThisBatchDeleteFailed;//有可能此时的DEL的虚拟机还未加入vmPool，所以需要保存在一个vector里面，在每个batch处理完之后再进行资源回收


bool serverResourceRecycleOfThisVm(int vmID)
{
    //有可能此时的DEL的虚拟机还未加入vmPool，所以需要保存在一个vector里面，在每个batch处理完之后再进行资源回收
    if(!vmPool.contains(vmID))
    {
        vmIdsInThisBatchDeleteFailed.push_back(vmID);
        return false;
    }
    vmEntity* thisVm = vmPool.vmPool[vmID];
    int serverIdOfThisVm = vmPool.vmPool[vmID]->server;

    //回收这个虚拟机对应的服务器的资源
    serverEntity* serverOfThisVm = NULL;

    if(!thisVm->isTwoNode)//单节点 
    { 
        if(serverFor1Node.find(serverIdOfThisVm) != serverFor1Node.end()) 
        {
            serverOfThisVm = serverFor1Node[serverIdOfThisVm];
            if(thisVm->serNode == type_A) 
            {
                serverOfThisVm->deAlloc(thisVm->core, thisVm->memory, 0, 0);
            } 
            else if(thisVm->serNode == type_B) 
            {
                serverOfThisVm->deAlloc(0, 0, thisVm->core, thisVm->memory);
            } 
            else 
            {
                cout << thisVm->vmName << ": " << thisVm->isTwoNode << ", " << thisVm->serNode << "\n";
                cout << " not find vm " << endl;
                exit(-1);
            }
        } 
        else 
        {
            cout << " not find vm int serverFor1Node" << endl;
            exit(-1);
        }
    } 
    else if(thisVm->isTwoNode) 
    { //双节点
        if(serverFor2Node.find(serverIdOfThisVm) != serverFor2Node.end()) 
        {
            serverOfThisVm = serverFor2Node[serverIdOfThisVm];
            if(thisVm->serNode != type_A && thisVm->serNode != type_B) 
            {
                serverOfThisVm->deAlloc(thisVm->core / 2, thisVm->memory / 2, thisVm->core / 2, thisVm->memory / 2);
            } else 
            {
                cout << thisVm->vmName << ": " << thisVm->isTwoNode << ", " << thisVm->serNode << "\n";
                cout << " not find vm " << endl;
                exit(-1);
            }
        } 
        else 
        {
            cout << " not find vm int serverFor2Node" << endl;
            exit(-1);
        }
    } 
    else 
    {
        cout << " not find corresponding vm" << endl;
        exit(-1);
    }
    return true;
}

void processOneDay(vector<requestInfo>& oneDayRequests) 
{
    migrate();

    for (int i = 0; i < oneDayRequests.size(); ++i) 
    {
        while(i < oneDayRequests.size() && oneDayRequests[i].type == DEL) 
        {
            int vmID = oneDayRequests[i].vmId;
            if(serverResourceRecycleOfThisVm(vmID)) 
            {//回收vmID部署的服务器的资源
                vmPool.remove(oneDayRequests[i].vmId);//回收当前请求的虚拟机对应的服务器的资源,
            }
            ++i;
        }
        //处理这个Batch之前，将上一个查询上一个Batch的虚拟机是否用完

        while(i < oneDayRequests.size() && oneDayRequests[i].type == ADD) 
        {
            //将其加入到这一批次的虚拟机池里
            vmPoolBetweenTwoDels.insert(oneDayRequests[i].vmId, vmInfos[oneDayRequests[i].vmName]->clone(
                                            oneDayRequests[i].vmId, oneDayRequests[i].time.day, oneDayRequests[i].time.idx)); //根据请求生成虚拟机，并加入
            if(vmPoolBetweenTwoDels.vmPool.size()>MAXSIZE-5)//ADD数量接近最大MAXSIZE就进行一次加入到旧服务器上
            {
                proceessVmPoolBetweenTwoDels();
            }
            ++i;
        }
    }
    //可能每天最终是ADD，所以需要再处理
    cout << "this day last" << endl;
    proceessVmPoolBetweenTwoDels();
    cout << "this day end" << endl;
}

void processOneBatch() 
{ //处理一个批次的请求
    if(vmPoolThisBatchNotProccessed.vmPool.size() != 0) 
    {
        cout << "this batch wrong " << endl;
        exit(-1);
    }
    vmIdsInThisBatchDeleteFailed.clear();
    //cout << " proccess today " << endl;
    //static int count = 0;

    for(auto& reqs : toBeProcessedRequests) 
    {
        //count++;
        processOneDay(reqs);//每次ADD接近MAXSIZE时就进行一次处理
    }
    //一个batch完了之后处理vmPoolThisBatchNotProccessed的虚拟机
    // cout << "count: " << count << "\n";
    proceessVmPoolThisBatchNotProccessed();//处理没能加入就服务器中的虚拟机
    // cout << "count: " << count << "\n";
    for (auto vmIdx : vmIdsInThisBatchDeleteFailed) 
    {
        if(!vmPool.contains(vmIdx)) 
        {
            cout << " delete error" << endl;
            exit(-1);
        }
        serverResourceRecycleOfThisVm(vmIdx);
        vmPool.remove(vmIdx);
    }
    vmIdsInThisBatchDeleteFailed.clear();
}

//选出allDaysRequests里面第dayBegin天开始，数量为days的请求
void chooseThisBatchRequests(int dayBegin, int days) {
    toBeProcessedRequests.clear();
    for (int i = dayBegin; i < min(dayBegin + days, (int)allDaysRequests.size()); ++i) {
        toBeProcessedRequests.emplace_back(allDaysRequests[i]);
    }
}

void process(int dayNumOneBatch) {
    int dayBegin = 0;
    while(dayBegin < (int)allDaysRequests.size()) {
        cout << "                       dayBegin: " << dayBegin << "\n";
        chooseThisBatchRequests(dayBegin, dayNumOneBatch);
        processOneBatch();
        outManager.flushAndStore();
        dayBegin += dayNumOneBatch;
    }
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
    // speedio();
#ifdef TEST
    freopen("../training-data/training-2.txt", "rb", stdin);
    //freopen("../training-data/training-2-output.txt", "w", stdout);
#endif
    cin >> N_allServerType;
#ifdef TEST
    // cout << N_allServerType << "\n";
#endif
    for (int i = 0; i < N_allServerType; ++i) {
        serverInfos.emplace_back(readServer());
    }

    for (int i = 0; i < (int)serverInfos.size(); ++i) {
        serverType2Info[serverInfos[i].serverName] = i;
    }

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
            dailyRequests.push_back(readRequest());
        }
        allDaysRequests.push_back(dailyRequests);
    }

    //input_test();
    int dayNumOfOneBatch = 1;//每次处理20天的
    process(dayNumOfOneBatch);
    outManager.flushAndStore();
    outManager.printAll();
#ifdef TEST
#endif
    freeSysMemory();
    return 0;
}

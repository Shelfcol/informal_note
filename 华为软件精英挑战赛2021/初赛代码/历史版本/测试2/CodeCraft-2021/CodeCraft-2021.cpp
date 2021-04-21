// #include <bits/stdc++.h>
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
    int core, memory, isTwoNode;
    int server;  //虚拟机所在的server的
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
enum reqType {
    ADD,
    DEL
};
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

struct outputVmInfo { //保存虚拟即对应的结果，以便最终输出的时候服务器编号改变之后进行修改
    int vmServerIdBeforeRearrange;
    int vmServerIdAfterArrange;
    int vmID;      //请求的虚拟机的ID
    string ABType; //单节点保存"A","B"，双节点保存"D"
    outputVmInfo(int _vmID, int _vmServerIdBeforeRearrange, int _vmServerIdAfterArrange, string _ABType) {
        vmID = _vmID;
        vmServerIdBeforeRearrange = _vmServerIdBeforeRearrange;
        vmServerIdAfterArrange = _vmServerIdAfterArrange;
        ABType = _ABType;
    };
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
map<string, int> serverType2Info; //string to index
//虚拟机类型
map<string, vmInfo*> vmInfos;
//当前存在的服务器（已经购买的）
//vector<serverInfo> serverResource;

//选出三种待选服务器，按照core/memory。选出大的，小的，以及两者差不多的
serverInfo serCoreMinMemory; //core小的
serverInfo serCoreMaxMemory; //core>memory
serverInfo serCoreEqualMemory;

//用来保存待选的服务器以及当天购买的数量
struct serverTypeChosenNum {
    string serverName;
    int serverN;
    serverTypeChosenNum(string _serverName, int _serverN) {
        serverName = _serverName;
        serverN = _serverN;
    }
};

vector<serverTypeChosenNum> serverTypeChosed; //保存待选的几种服务器和对应的服务器的数量，后面对服务器的进行重新编号的时候就按照这个顺序
vector<int> serverTypeChosedBoughtToday;      //保存购买当天待选的服务器中各个服务器的数量

multimap<string, int> serverBoughtName2index; //保存今天购买的服务器的名称和对应的编号。某种服务器可能会买了多个，有多个编号 名称->编号
int migrationTimeToday = 0;                   //今天迁移的此数

vector<outputVmInfo> outputVmInfosToday; //保存今天的虚拟机部署情况，方便后面服务器序号更改后进行修改

int serverNumber = 0; //服务器数量（编号从0开始）单双节点一起编号
map<int, serverEntity*> serverFor1Node;
map<int, serverEntity*> serverFor2Node;

map<int, serverEntity*> serverFor1NodeBoughtToday;  //今天新买的服务器，这是为了每天之后给新买的服务器赋上对应的编号，然后重新给虚拟机编号
map<int, serverEntity*> serverFor2NodeBoughtToday;

//当前存在的虚拟机 虚拟机有ID
map<int, vmEntity*> vmPool;  //虚拟机ID->虚拟机实体
vector<string> output;
vector<requestInfo> dailyRequests; //每天的请求
const int MaxChooseSerType = 4;    //选择的服务器的最大种类数

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
    // cout << "serverInfo " << s << endl;
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

//从服务器中选出三种服务器，按照core/memory的比值大于4（这个比值后面可以首先对服务器数据进行读取获得）的，选一个最大的，最小的，和两者差不多的.都选择core最大的那个
//按照服务器的core/memory选择三种服务器。serverResource
//将供选的服务器信息进行排序（先按核数，再按内存）
const float serverCoreMemoryRatio = 2.2;
const float vmCoreMemoryRatio = 2.2;
void serverToBeBought() {
    //  core/memory 小 的放前面。
    sort(serverInfos.begin(), serverInfos.end(), [&](serverInfo & a, serverInfo & b) {
        return a.A.core / a.A.memory < b.A.core / b.A.memory;
    });
    serCoreMinMemory = serverInfos.front(); //core小的
    serCoreMaxMemory = serverInfos.back();  //core>memory
    serCoreEqualMemory = serverInfos[serverInfos.size() / 2];

    for (unsigned int i = 0; i < serverInfos.size(); ++i) {
        if (float(serverInfos[i].A.core) / float(serverInfos[i].A.memory) > serverCoreMemoryRatio && serverInfos[i].A.core > serCoreMaxMemory.A.core)
            serCoreMaxMemory = serverInfos[i];
        if (float(serverInfos[i].A.core) / float(serverInfos[i].A.memory) < 1.0 / serverCoreMemoryRatio && serverInfos[i].A.core > serCoreMinMemory.A.core)
            serCoreMinMemory = serverInfos[i];
        if (abs(float(serverInfos[i].A.core) / float(serverInfos[i].A.memory) - 1.0) < 0.2 && serverInfos[i].A.core > serCoreEqualMemory.A.core)
            serCoreEqualMemory = serverInfos[i];
    }

    serverTypeChosed.emplace_back(serCoreMinMemory.serverName, 0);
    serverTypeChosed.emplace_back(serCoreMaxMemory.serverName, 0);
    serverTypeChosed.emplace_back(serCoreEqualMemory.serverName, 0);

    //cout<< "serCoreMinMemory:  " << serCoreMinMemory.A.core * 2 << " " << serCoreMinMemory.A.memory * 2 << endl;
    //cout << "serCoreMaxMemory:  " << serCoreMaxMemory.A.core * 2 << " " << serCoreMaxMemory.A.memory * 2 << endl;
    //cout << "serCoreEqualMemory:  " << serCoreEqualMemory.A.core * 2 << " " << serCoreEqualMemory.A.memory * 2 << endl;

    for (int i = 0; i < (int)serverInfos.size(); ++i) {
        serverType2Info[serverInfos[i].serverName] = i;
    }
}

//优化方向：用一个数组存起来，单节点从前往后找，双节点从后往前找。有冲突之后再考虑购买新的服务器
void initServer() {
    //初始化服务器的数量和种类
    serverToBeBought(); //确定三种购买的服务器
}

void buyServer(vmEntity* vm) { //根据虚拟机购买对应的服务器，需要考虑被core/memory和是否是双节点
    //cout << " buy server" << endl;
    float core = vm->core, memory = vm->memory;
    string serverType;

    if (float(core) / float(memory) > vmCoreMemoryRatio) {

        serverType = serCoreMaxMemory.serverName;
    } else if (float(core) / float(memory) < 1.0 / vmCoreMemoryRatio) {
        serverType = serCoreMinMemory.serverName;
    } else {
        serverType = serCoreEqualMemory.serverName;
    }

    //分为单节点和双节点
    if (vm->isTwoNode) {
        serverFor2NodeBoughtToday[serverNumber++] = serverInfos[serverType2Info[serverType]].clone(); //将今天新买的服务器放入此变量中
    } else {
        serverFor1NodeBoughtToday[serverNumber++] = serverInfos[serverType2Info[serverType]].clone();
    }

    serverBoughtName2index.emplace(serverType, (serverNumber - 1)); //记录购买的此台服务器的名称和对应的编号 名称->编号
}

double ratio_core_mem = 0; //core和memory之间的重要性关系

//虚拟机按照core/memory >4  <0.25   (0.25,4)分成三类，分别放入服务器中core/memory >

/*********跳过非对应的服务器*********/
bool isCorrespondingServer(int core, int memory, serverEntity* server) {
    if (float(core) / float(memory) > vmCoreMemoryRatio) {
        if (float(server->A.core) / float(server->A.memory) <= serverCoreMemoryRatio)
            return false;
    }
    if (float(core) / float(memory) < 1.0 / vmCoreMemoryRatio) {
        if ((float(server->A.core) / float(server->A.memory) >= 1.0 / serverCoreMemoryRatio))
            return false;
    }

    if (float(core) / float(memory) <= vmCoreMemoryRatio && float(core) / float(memory) >= 1.0 / vmCoreMemoryRatio) {
        if (!(float(server->A.core) / float(server->A.memory) <= serverCoreMemoryRatio && float(server->A.core) / float(server->A.memory) >= 1.0 / serverCoreMemoryRatio))
            return false;
    }
    return true;
}

void match(vmEntity* vm) {
    //分配
    //to do
    //分配该虚拟机给一个服务器，服务器资源的更新
    //分配的信息包括：虚拟机所在的服务器索引以及所在的节点（需要更新server和serNode两个属性）
    int core = vm->core, memory = vm->memory;
    //cout << "start matching" << endl;
    if (vm->isTwoNode) { //双节点
        //cout << "部署在双节点上" << endl;
        //这里采用最优适配：即分配剩余最小的可用资源(先考虑核数)（跟操作系统分配内存类似）
        //暂时采用遍历的方式：如果后续需要提高效率，可设计LinkedList的方式
        int best = -1;
        core /= 2;
        memory /= 2;

        for (const auto& it : serverFor2Node) { //遍历所有旧的对应的服务器
            int id = it.first;
            serverEntity* server = it.second;

            //跳过非对应的服务器
            if (!isCorrespondingServer(core, memory, server))
                continue;

            if (server->A.core >= core && server->A.memory >= memory) { //因为我我们单双节点分开，所以只需要看A就可以了
                if (best == -1) {
                    best = id;
                    break;
                }
            }
        }
        if (best != -1) { //在旧的服务器上部署虚拟机
            vm->server = best;
            vm->serNode = 2; //表示双节点
            //分配资源
            serverFor2Node[best]->A.core -= core;
            serverFor2Node[best]->B.core -= core;
            serverFor2Node[best]->A.memory -= memory;
            serverFor2Node[best]->B.memory -= memory;
            outputVmInfosToday.emplace_back(vm->id, best, best, "D"); //-1表示重新编号后的结果，因为还没编号，所以用-1当默认值
            // cout << "outputVmInfosToday.emplace_back1" << endl;
            return;                                                   //退出此次部署
        }

        //旧的服务器无法部署，在今天买的服务器上部署，如果还是不够，则再重新买服务器
        /**********************在新买的服务器中遍历*******************************************/

        for (const auto& it : serverFor2NodeBoughtToday) { //遍历今天新买的服务器

            int id = it.first;
            serverEntity* server = it.second;

            /*********跳过非对应的服务器*********/
            if (!isCorrespondingServer(core, memory, server))
                continue;

            /****************************************/
            if (server->A.core >= core && server->A.memory >= memory) { // && server->B.core >= core && server->B.memory >= memory)//双节点部署，需要看AB！！！！！错了，因为我我们单双节点分开，所以只需要看A就可以了
                if (best == -1) {
                    best = id;
                    break;
                }
            }
        }

        /*****************************************************************/

        /***************************************************/
        if (best == -1) { //表示没找到合适的服务器，则需要根据虚拟机的core/memory来购买对应的服务器

            buyServer(vm);
            best = serverNumber - 1;
        }
        /******************************************************/
        vm->server = best;
        vm->serNode = 2; //表示双节点部署
        //分配资源
        serverFor2NodeBoughtToday[best]->A.core -= core;
        serverFor2NodeBoughtToday[best]->B.core -= core;
        serverFor2NodeBoughtToday[best]->A.memory -= memory;
        serverFor2NodeBoughtToday[best]->B.memory -= memory;
        outputVmInfosToday.emplace_back(vm->id, best, best, "D");
    } else { //单节点部署，可以部署在A或者B上
        //cout << "单节点部署，可以部署在A或者B上" << endl;
        int bestSer = -1, bestNode = 0; //默认优先部署在A节点上

        //首先在旧的服务器上搜部署
        for (const auto& it : serverFor1Node) {
            int id = it.first;
            serverEntity* server = it.second;
            /************************跳过不对应的服务器**********************************/
            if (!isCorrespondingServer(core, memory, server))
                continue;

            /**************************************************************************/

            //节点 A
            if (server->A.core >= core && server->A.memory >= memory) {
                if (bestSer == -1) {
                    bestSer = id;
                    bestNode = 0;
                    break;
                }
            }
            //节点 B，如果A点不满足条件，就看B节点
            if (server->B.core >= core && server->B.memory >= memory) {
                if (bestSer == -1) {
                    bestSer = id;
                    bestNode = 1;
                    break;
                }
            }
        }

        if (bestSer != -1) { //旧服务器上找到可以部署的位置
            vm->server = bestSer;
            vm->serNode = bestNode;
            //分配资源
            if (bestNode == 0) {
                serverFor1Node[bestSer]->A.core -= core;
                serverFor1Node[bestSer]->A.memory -= memory;
                outputVmInfosToday.emplace_back(vm->id, bestSer, bestSer, "A");
            } else {

                serverFor1Node[bestSer]->B.core -= core;
                serverFor1Node[bestSer]->B.memory -= memory;
                outputVmInfosToday.emplace_back(vm->id, bestSer, bestSer, "B");
            }
            return;
        }

        //旧的服务器上没找到，需要在新的服务器上部署
        for (const auto& it : serverFor1NodeBoughtToday) {
            int id = it.first;
            serverEntity* server = it.second;
            //跳过不对应的服务器
            if (!isCorrespondingServer(core, memory, server))
                continue;

            //节点 A
            if (server->A.core >= core && server->A.memory >= memory) {
                if (bestSer == -1) {
                    bestSer = id;
                    bestNode = 0;
                    break;
                }
            }
            //节点 B，如果A点不满足条件，就看B节点
            if (server->B.core >= core && server->B.memory >= memory) {
                if (bestSer == -1) {
                    bestSer = id;
                    bestNode = 1;
                    break;
                }
            }
        }

        if (bestSer == -1) { //表示没找到合适的服务器，则需要根据虚拟机的core/memory来购买对应的服务器
            buyServer(vm);
            bestSer = serverNumber - 1;
        }

        vm->server = bestSer;
        vm->serNode = bestNode;
        //分配资源
        if (bestNode == 0) {
            serverFor1NodeBoughtToday[bestSer]->A.core -= core;
            serverFor1NodeBoughtToday[bestSer]->A.memory -= memory;
            outputVmInfosToday.emplace_back(vm->id, bestSer, bestSer, "A");
        } else {
            serverFor1NodeBoughtToday[bestSer]->B.core -= core;
            serverFor1NodeBoughtToday[bestSer]->B.memory -= memory;
            outputVmInfosToday.emplace_back(vm->id, bestSer, bestSer, "B");
        }
    }
    //cout << " match finished " << endl;
}

void expansion() {
    //扩容
    //to do
    //string s = "(purchase, 0)\n";
    //output.emplace_back(s);
}

void migrate() {
    //迁移
    //to do

    migrationTimeToday = 0;
}

void calcPowerCost() {
    //to do
}

void addVm(vmEntity* vm) {
    //添加虚拟机
    match(vm); //分配给服务器，如果对应的服务器的资源不够，则购买对应的服务器资源

    vmPool[vm->id] = vm; //放入虚拟机池
}

//需要判断需要删除的虚拟机是在新的服务器还是旧的服务器上
void delVm(int id) {
    vmEntity* vm = vmPool[id]; //从虚拟机池里面找出请求的虚拟机ID号
    if (vm->isTwoNode) {
        //首先判断需要删除的虚拟机是在今天购买的新服务器上还是在旧服务器上
        auto iter = serverFor2Node.find(vm->server);
        auto iterToday = serverFor2NodeBoughtToday.find(vm->server);
        if (iter != serverFor2Node.end()) {
            serverEntity* server = serverFor2Node[vm->server]; //找出虚拟机对应的服务器

            //释放资源
            server->A.core += vm->core / 2;
            server->B.core += vm->core / 2;
            server->A.memory += vm->memory / 2;
            server->B.memory += vm->memory / 2;
            //cout << " server->A.core =" << server->A.core << endl;
        } else if (iterToday != serverFor2NodeBoughtToday.end()) { //在今天买的服务器里面删除
            serverEntity* server = serverFor2NodeBoughtToday[vm->server]; //找出虚拟机对应的服务器

            //释放资源
            server->A.core += vm->core / 2;
            server->B.core += vm->core / 2;
            server->A.memory += vm->memory / 2;
            server->B.memory += vm->memory / 2;
        } else {
            cout << " deleted server not exist " << endl;
            exit(-1);
        }
    } else { //单节点
        //首先判断需要删除的虚拟机是在今天购买的新服务器上还是在旧服务器上
        auto iter = serverFor1Node.find(vm->server);
        auto iterToday = serverFor1NodeBoughtToday.find(vm->server);
        if (iter != serverFor1Node.end()) {
            serverEntity* server = serverFor1Node[vm->server];
            if (vm->serNode == 0) {
                //A节点
                server->A.core += vm->core;
                server->A.memory += vm->memory;
            } else { //if(vm->serNode==1)
                //B节点
                server->B.core += vm->core;
                server->B.memory += vm->memory;
            }
        } else if (iterToday != serverFor1NodeBoughtToday.end()) { //在今天买的服务器里面删除
            serverEntity* server = serverFor1NodeBoughtToday[vm->server];
            if (vm->serNode == 0) {
                //A节点
                server->A.core += vm->core;
                server->A.memory += vm->memory;
            } else { //if(vm->serNode==1)
                //B节点
                server->B.core += vm->core;
                server->B.memory += vm->memory;
            }
        } else {
            cout << " deleted server not exist " << endl;
        }
    }
#ifdef TEST

#endif
    delete vmPool[id]; //删除虚拟机池里面对应的虚拟机
    vmPool.erase(id);
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
    for (auto& it : vmPool) {
        delete it.second;
    }
}

//计算需要交换的服务器,生成服务器  旧->新  序号的map
map<int, int> calcServerBoughtTodayOld2NewIndex(multimap<string, int>& serverBoughtName2index, int startIndexOfBoughtServer) {
    map<int, int> serverBoughtTodayOld2NewIndex;
    //serverBoughtName2index保存的是每种服务器的名称和对应的旧的编号
    if (serverBoughtName2index.size() == 0)
        return serverBoughtTodayOld2NewIndex;

    for (auto& st : serverTypeChosed) {
        for (multimap<string, int>::iterator it = serverBoughtName2index.begin(); it != serverBoughtName2index.end(); ++it) {

            if (it->first == st.serverName) {
                st.serverN++;
                serverBoughtTodayOld2NewIndex[it->second] = startIndexOfBoughtServer++;
            }
        }
    }
    return serverBoughtTodayOld2NewIndex;
}

//将新买的服务器加到以前的服务器组合里面
void addServerBoughtTodayToOldServers(map<int, int> serverBoughtTodayOld2NewIndex) {
    if (serverBoughtTodayOld2NewIndex.size() == 0)
        return;
    for (const auto& so : serverBoughtTodayOld2NewIndex) {
        auto iter1 = serverFor1NodeBoughtToday.find(so.first);
        auto iter2 = serverFor2NodeBoughtToday.find(so.first);

        if (iter1 != serverFor1NodeBoughtToday.end()) { //在1里面
            serverFor1Node[so.second] = serverFor1NodeBoughtToday[so.first]; //新的服务器编号指向今天买的服务器的旧编号对应的内存
        } else if (iter2 != serverFor2NodeBoughtToday.end()) { //在2里面
            serverFor2Node[so.second] = serverFor2NodeBoughtToday[so.first];
        } else {
            cout << so.first << " " << so.second << endl;
            cout << "WRONG INDEX.............................." << endl;
            exit(-1);
        }
    }
}

void preHandle() {
    if (serverBoughtName2index.size() != 0) { //当今天购买新服务器之后
        //1.找出新购买服务器的起始编号
        int startIndexOfBoughtServer = INT_MAX; //找出服务器的起始编号
        for (multimap<string, int>::iterator it = serverBoughtName2index.begin(); it != serverBoughtName2index.end(); ++it) {
            if (it->second < startIndexOfBoughtServer) //这个不能删除
                startIndexOfBoughtServer = it->second;
        }

        //2.根据题目要求更新今天购买的服务器的节点的序号  旧->新
        map<int, int> serverBoughtTodayOld2NewIndex = calcServerBoughtTodayOld2NewIndex(serverBoughtName2index, startIndexOfBoughtServer);

        //3.更改outputVmInfosToday里面的虚拟机部署的server的新编号，方便后面打印结果
        if (serverBoughtName2index.size() != 0) {
            for (auto& out : outputVmInfosToday) {
                if (out.vmServerIdBeforeRearrange >= startIndexOfBoughtServer) ////////?????????/不加就会出错！！！！！！！！！！！！！！！！！！
                    out.vmServerIdAfterArrange = serverBoughtTodayOld2NewIndex[out.vmServerIdBeforeRearrange];
            }
        }

        //4.更新虚拟机池里面虚拟机所部署的服务器的编号
        for (auto& vmP : vmPool) {
            //遍历serverBoughtTodayOld2NewIndex,如果当前server节点的值与serverBoughtTodayOld2NewIndex.second相关，则更新
            for (const auto& so : serverBoughtTodayOld2NewIndex) {
                if (vmP.second->server == so.first) {
                    vmP.second->server = so.second;
                    break;
                }
            }
        }
        //5.将新买的服务器加到以前的服务器的组合里面
        addServerBoughtTodayToOldServers(serverBoughtTodayOld2NewIndex);
    }
}

void logTodayOutput() { //记录当天的操作
    preHandle();
    //打印结果
    int serverTypeBounghtToday = 0;
    for (auto& st : serverTypeChosed) {
        if (st.serverN > 0)
            ++serverTypeBounghtToday;
    }
    output.emplace_back("(purchase, " + to_string(serverTypeBounghtToday) + ")\n");

    if (serverBoughtName2index.size() != 0) {
        for (auto& st : serverTypeChosed) {
            if (st.serverN > 0)
                output.emplace_back("(" + st.serverName + ", " + to_string(st.serverN) + ")\n");
        }
    }

    output.emplace_back("(migration, " + to_string(migrationTimeToday) + ")\n");

    //输出虚拟机和对应的编号
    for (const auto& out : outputVmInfosToday) {
        if (out.ABType == "D") { //表示为单节点的虚拟机
            output.emplace_back("(" + to_string(out.vmServerIdAfterArrange) + ")\n");
        }

        else if (out.ABType == "A") {
            output.emplace_back("(" + to_string(out.vmServerIdAfterArrange) + ", " + out.ABType + ")\n");
        } else if (out.ABType == "B") {
            output.emplace_back("(" + to_string(out.vmServerIdAfterArrange) + ", " + out.ABType + ")\n");
        } else {
            cout << " wrong ABType" << endl;
            exit(-1);
        }
    }
}

void dailyProcess(int day) {
    if (day == 0)
        initServer();
    migrate();

    //一个一个处理请求
    for (const requestInfo& req : dailyRequests) {

        if (req.type == DEL) {
            // cout << " DELETE VM" << endl;
            delVm(req.vmId);
        }

        if (req.type == ADD) {
            //cout << "ADD VM" << endl;
            vmEntity* vm = (vmInfos[req.vmName])->clone(); //根据请求里面虚拟机的名称新建一个虚拟机
            vm->id = req.vmId;                             //请求里面给虚拟机赋一个ID
            //cout << "  vm  id = " << vm->id << endl;
            addVm(vm); //给定虚拟机的名称和ID，找出合适的服务器放进去
        }
    }

    logTodayOutput();
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

        outputVmInfosToday.clear();        //清除前一天的虚拟机的信息
        serverFor1NodeBoughtToday.clear(); //清除前一天购买的单节点的服务器
        serverFor2NodeBoughtToday.clear(); //清除前一天购买的双节点的服务器
        serverBoughtName2index.clear();
        for (auto& st : serverTypeChosed) { //对前一天的清零
            st.serverN = 0;
        }
        migrationTimeToday = 0; //清除前一天迁移的此数
        dailyProcess(day);
    }

    for (string& s : output) {
        cout << s;
    }
    //cout << "server bought num= " << serverNumber << endl;
#ifdef TEST
    // cout << "finish!\n";
#endif
    freeSysMemory();
    return 0;
}

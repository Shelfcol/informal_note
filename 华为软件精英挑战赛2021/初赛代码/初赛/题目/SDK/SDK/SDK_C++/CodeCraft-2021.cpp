// #include <bits/stdc++.h>
#include <algorithm>
#include <assert.h>
#include <iostream>
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
//vector<serverInfo> serverResource;

//选出三种待选服务器，按照core/memory。选出大的，小的，以及两者差不多的
serverInfo serCoreMinMemory; //core小的
serverInfo serCoreMaxMemory; //core>memory
serverInfo serCoreEqualMemory;

unordered_map<string, int> serverBoughtToday; //记录当天购买的三种服务器的数量
int migrationTimeToday = 0;//今天迁移的此数

vector<string> vmArrangeToday; //记录今天的虚拟机部署情况

int serverNumber = 0; //服务器数量（编号从0开始）
unordered_map<int, serverEntity*> serverFor1Node;
unordered_map<int, serverEntity*> serverFor2Node;
//当前存在的虚拟机 虚拟机有ID
unordered_map<int, vmEntity*> vmPool;  //虚拟机ID->虚拟机实体
vector<string> output;
vector<requestInfo> dailyRequests; //每天的请求
const int MaxChooseSerType = 4;    //选择的服务器的最大种类数

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
const float serverCoreMemoryRatio = 4.0;
const float vmCoreMemoryRatio = 4.0;
void serverToBeBought() {
    //  core/memory 小 的放前面。
    sort(serverInfos.begin(), serverInfos.end(), [&](serverInfo & a, serverInfo & b) {
        return a.A.core / a.A.memory < b.A.core / b.A.memory;
    });
    serCoreMinMemory = serverInfos.front(); //core小的
    serCoreMaxMemory = serverInfos.back();  //core>memory
    serCoreEqualMemory = serverInfos[serverInfos.size() / 2];

    //找出几乎相等的服务器
    for (int i = 0; i < serverInfos.size(); ++i) {
        if (float(serverInfos[i].A.core) / float(serverInfos[i].A.memory) > serverCoreMemoryRatio && serverInfos[i].A.core > serCoreMaxMemory.A.core)
            serCoreMaxMemory = serverInfos[i];
        if (float(serverInfos[i].A.core) / float(serverInfos[i].A.memory) < 1.0 / serverCoreMemoryRatio && serverInfos[i].A.core > serCoreMinMemory.A.core)
            serCoreMinMemory = serverInfos[i];
        if (abs(float(serverInfos[i].A.core) / float(serverInfos[i].A.memory) - 1.0) < 1.0 && serverInfos[i].A.core > serCoreEqualMemory.A.core)
            serCoreEqualMemory = serverInfos[i];
    }

    //记录每天购买的服务器数量
    serverBoughtToday[serCoreMinMemory.serverName] = 0;
    serverBoughtToday[serCoreMaxMemory.serverName] = 0;
    serverBoughtToday[serCoreEqualMemory.serverName] = 0;

    //cout<< "serCoreMinMemory:  " << serCoreMinMemory.A.core * 2 << " " << serCoreMinMemory.A.memory * 2 << endl;
    //cout << "serCoreMaxMemory:  " << serCoreMaxMemory.A.core * 2 << " " << serCoreMaxMemory.A.memory * 2 << endl;
    //cout << "serCoreEqualMemory:  " << serCoreEqualMemory.A.core * 2 << " " << serCoreEqualMemory.A.memory * 2 << endl;

    for (int i = 0; i < (int)serverInfos.size(); ++i) {
        serverType2Info[serverInfos[i].serverName] = i;
    }
}

//当前直接买的3000台，平均分成两组。
//优化方向：用一个数组存起来，单节点从前往后找，双节点从后往前找。有冲突之后再考虑购买新的服务器
void initServer() {
    //初始化服务器的数量和种类
    //to do
    serverToBeBought(); //确定三种购买的服务器
    /*
    string serverType = "hostUY41I";
    int n = 3000;
    string initBuy = "(purchase, ";
    initBuy += to_string(2) + ")\n";

    output.emplace_back(initBuy);

    string pauseInfo = "(" + serverType + ", ";
    pauseInfo += std::to_string(n / 2) + ")\n";

    output.emplace_back(pauseInfo);

    for (int i = 0; i < n / 2; ++i)
    {
        if (i & 1)
        {
            serverFor1Node[serverNumber++] = serverInfos[serverType2Info[serverType]].clone();
        }
        else
        {
            serverFor2Node[serverNumber++] = serverInfos[serverType2Info[serverType]].clone();
        }
    }
    serverType = "host78BMY";
    pauseInfo = "(" + serverType + ", ";
    pauseInfo += std::to_string(serverNumber) + ")\n";

    output.emplace_back(pauseInfo);

    for (int i = 0; i < n / 2; i++)
    {
        if (i & 1)
        {
            serverFor1Node[serverNumber++] = serverInfos[serverType2Info[serverType]].clone();
        }
        else
        {
            serverFor2Node[serverNumber++] = serverInfos[serverType2Info[serverType]].clone();
        }
    }
    */
}

void buyServer(vmEntity* vm) { //根据虚拟机购买对应的服务器，需要考虑被core/memory和是否是双节点
    int core = vm->core, memory = vm->memory;
    string serverType;

    if (core / memory > vmCoreMemoryRatio) {
        serverType = serCoreMaxMemory.serverName;
    } else if (core / memory < 1.0 / vmCoreMemoryRatio) {
        serverType = serCoreMinMemory.serverName;
    } else {
        serverType = serCoreEqualMemory.serverName;
    }

    if (vm->isTwoNode) {
        serverFor2Node[serverNumber++] = serverInfos[serverType2Info[serverType]].clone();
    } else {
        serverFor1Node[serverNumber++] = serverInfos[serverType2Info[serverType]].clone();
    }

    serverBoughtToday[serverType]++;//记录今天购买的服务器类型和数量
}

double ratio_core_mem = 1; //core和memory之间的重要性关系

//虚拟机按照core/memory >4  <0.25   (0.25,4)分成三类，分别放入服务器中core/memory >

void match(vmEntity* vm) {
    //static int count = 0;
    //+count;
    //cout << count << " ";
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
        for (const auto& it : serverFor2Node) { //遍历所有对应的服务器

            int id = it.first;
            serverEntity* server = it.second;

            /*********跳过非对应的服务器*********/
            if (!((double)core / memory > vmCoreMemoryRatio && (double)server->A.core / server->A.memory > serverCoreMemoryRatio)) {
                continue;
            }

            if (!((double)core / memory < 1.0 / vmCoreMemoryRatio && (double)server->A.core / server->A.memory < 1.0 / serverCoreMemoryRatio)) {
                continue;
            }

            if (!((double)core / memory <= vmCoreMemoryRatio && (double)core / memory >= 1.0 / vmCoreMemoryRatio && (double)server->A.core / server->A.memory <= serverCoreMemoryRatio && (double)server->A.core / server->A.memory >= 1.0 / serverCoreMemoryRatio)) {
                continue;
            }
            /****************************************/
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

        //assert(best != -1);
        /***************************************************/
        if (best == -1) { //表示没找到合适的服务器，则需要根据虚拟机的core/memory来购买对应的服务器
            buyServer(vm);
            best = serverNumber - 1; //当前的序号？？？？？看看序号是否正确
        }
        /******************************************************/

        vm->server = best;
        vm->serNode = 2;
        //分配资源
        serverFor2Node[best]->A.core -= core;
        serverFor2Node[best]->B.core -= core;
        serverFor2Node[best]->A.memory -= memory;
        serverFor2Node[best]->B.memory -= memory;
        vmArrangeToday.emplace_back("(" + to_string(best) + ")\n");
        // cout << "core: " << core << ", " << "mem: " << memory << ", "
        //     << serverFor2Node[best] << "\n";
    } else {
        int bestSer = -1, bestNode = 0;
        for (const auto& it : serverFor1Node) {
            int id = it.first;
            serverEntity* server = it.second;

            /*********跳过非对应的服务器*********/
            if (!(core / memory > vmCoreMemoryRatio && server->A.core / server->A.memory > serverCoreMemoryRatio)) {
                continue;
            }

            if (!(core / memory < 1.0 / vmCoreMemoryRatio && server->A.core / server->A.memory < 1.0 / serverCoreMemoryRatio)) {
                continue;
            }

            if (!(core / memory <= vmCoreMemoryRatio && core / memory >= 1.0 / vmCoreMemoryRatio && server->A.core / server->A.memory <= serverCoreMemoryRatio && server->A.core / server->A.memory >= 1.0 / serverCoreMemoryRatio)) {
                continue;
            }
            /****************************************/

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
        //assert(bestSer != -1);

        /*****************************************************************************/
        if (bestSer == -1) { //表示没找到合适的服务器，则需要根据虚拟机的core/memory来购买对应的服务器
            buyServer(vm);
            bestSer = serverNumber - 1; //当前的序号？？？？？看看序号是否正确
        }
        /*****************************************************************************/

        vm->server = bestSer;
        vm->serNode = bestNode;
        //分配资源
        if (bestNode == 0) {
            serverFor1Node[bestSer]->A.core -= core;
            serverFor1Node[bestSer]->A.memory -= memory;
            vmArrangeToday.emplace_back("(" + to_string(bestSer) + ", A)\n");
        } else {
            serverFor1Node[bestSer]->B.core -= core;
            serverFor1Node[bestSer]->B.memory -= memory;
            vmArrangeToday.emplace_back("(" + to_string(bestSer) + ", B)\n");
        }
        // cout << "core: " << core << ", " << "mem: " << memory << ", "
        //     << serverFor1Node[bestSer] << "\n";
    }
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

void logTodayOutput() { //记录当天的操作
    //记录当天的操作的结果
    int serverTypeBounghtToday = 0; //当天购买的服务器类型
    auto iter = serverBoughtToday.begin();
    while (iter != serverBoughtToday.end()) {
        if (iter->second > 0)
            ++serverTypeBounghtToday;
        ++iter;
    }
    output.emplace_back("(purchase, " + to_string(serverTypeBounghtToday) + ")\n");

    for (iter = serverBoughtToday.begin(); iter != serverBoughtToday.end(); ++iter) {
        if(iter->second > 0) {
            output.emplace_back("(" + iter->first + ", " + to_string(iter->second) + ")\n");
        }
    }

    output.emplace_back("(migration, " + to_string(migrationTimeToday) + ")\n");

    //将今天虚拟机的部署情况加进去
    for(auto& v : vmArrangeToday)
        output.push_back(v);
}

void dailyProcess(int day) {
    if (day == 0)
        initServer();
    //else
    // expansion();
    migrate();

    //回收资源
    for (const requestInfo& req : dailyRequests) {
        if (req.type == DEL) {
            delVm(req.vmId);
        }
    }

    //分配资源，如果虚拟机对应的服务器内存不够，则购买新的
    for (const requestInfo& req : dailyRequests) {
        if (req.type == ADD) {
            vmEntity* vm = (vmInfos[req.vmName])->clone();
            vm->id = req.vmId;
            addVm(vm);
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
        //清除前一天购买的服务器数量
        serverBoughtToday[serCoreMinMemory.serverName] = 0;
        serverBoughtToday[serCoreMaxMemory.serverName] = 0;
        serverBoughtToday[serCoreEqualMemory.serverName] = 0;
        vmArrangeToday.clear(); //清除今天的虚拟机部署情况，准备记录几天的
        migrationTimeToday = 0; //清除前一天迁移的此数
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
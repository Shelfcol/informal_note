#include <bits/stdc++.h>
using namespace std;

#define TEST

typedef long long ll;

void speedio() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
}

struct serverInfo {
    string typeId;
    struct Node {
        int core, memory;
    } A, B;
    int serverCost, powerCost;
};
struct vmInfo {
    string typeId;
    int core, memory, isTwoNode;
    int server;     //虚拟机所在的server
    int serNode; //虚拟机所在的server的节点: 0代表A节点，1代表B节点
    int id;         //虚拟机的id
};
enum reqType {ADD, DEL};
struct requestInfo {
    string vmType;  //ADD时需要用
    reqType type;
    int vmId; //虚拟机id
};


//供选的服务器
vector<serverInfo> serverInfos; //这里string是唯一的
//1、按照自定义的标准（性价比： 单位核的成本（硬件+100天能耗）和单位内存的成本）排序  这部分gxf
//---2、直接按照规格大小排序，然后每隔多少步取一种。假如取四种---
//排序之后映射string->index;
unordered_map<string, int> serverType2Info; //string to index
//虚拟机类型
unordered_map<string, vmInfo> vmInfos;

typedef vmInfo vmEntity;
typedef serverInfo serverEntity;
// struct vmItem {
//     int core, memory, isTwoNode;
//     int server;
//     int id;
// }

//当前存在的服务器（已经购买的）
// vector<serverInfo> serverResource;
vector<serverEntity> server4OneNode;
vector<serverEntity> server4TwoNode;

//当前存在的虚拟机 虚拟机有ID
unordered_map<int, vmEntity> vmPool; //虚拟机ID->虚拟机实体

vector<string> output;


serverInfo readServer() {
    //to do
    serverInfo serInfo;

    return serInfo;
}

vmInfo readVm() {
    //to do
    vmInfo vm;

    return vm;
}

requestInfo readRequest() {
    //to do
    requestInfo reqInfo;

    return reqInfo;
}

void initServer() {//初始化服务器的数量和种类
    //to do
}
void match(vmEntity& vm) {//分配
    //to do
    //分配该虚拟机给一个服务器，服务器资源的更新
    //分配的信息包括：虚拟机所在的服务器索引以及所在的节点（需要更新server和serNode两个属性）

}
void expansion() {//扩容
    //to do
    string s = "(purchase, 0)\n";
    output.emplace_back(s);
}

void migrate() {//迁移
    //to do
    string s = "(migration, 0)\n";
    output.emplace_back(s);
}

void calcPowerCost() {
    //to do
}

void addVm(vmEntity& vm) {//添加虚拟机
    match(vm);      //分配给服务器
    vmPool[vm.id] = vm; //放入虚拟机池
}

void delVm(int id) {
    vmEntity& vm = vmPool[id];
    if(vm.isTwoNode) {
        serverEntity& server = server4TwoNode[vm.server];
        server.A.core += vm.core / 2;
        server.B.core += vm.core / 2;
        server.A.memory += vm.memory / 2;
        server.B.memory += vm.memory / 2;
    } else {
        serverEntity& server = server4TwoNode[vm.server];
        if(vm.serNode == 0) { //A节点
            server.A.core += vm.core;
            server.A.memory += vm.memory;
        } else { //B节点
            server.B.core += vm.core;
            server.B.memory += vm.memory;
        }
    }
    vmPool.erase(id);
}

void dailyProcess(vector<requestInfo>& requests, int day) {
    if(day == 0) initServer();
    else expansion();
    migrate();
    for(requestInfo& req : requests) {
        if(req.type == ADD) {
            vmInfo& vminfo = vmInfos[req.vmType];
            vmEntity vm;
            vm.core = vminfo.core;
            vm.memory = vminfo.memory;
            vm.isTwoNode = vminfo.isTwoNode;
            vm.id = req.vmId;
            addVm(vm);
        } else {
            delVm(req.vmId);
        }
    }
    calcPowerCost();
}

int N_allServerType, N_allVmType, T_days, R_requests;
int main() {
    speedio();
#ifdef TEST
    freopen("../training-data/training-2.txt", "r", stdin);
    freopen("../training-data/training-2-output.txt", "w", stdout);
#endif
    cin >> N_allServerType;
    for(int i = 0; i < N_allServerType; ++i) {
        serverInfos.emplace_back(readServer());
    }
    //将供选的服务器信息进行排序（先按核数，再按内存）
    sort(serverInfos.begin(), serverInfos.end(), [&](serverInfo& a, serverInfo& b) {
        if(a.A.core != b.A.core) return a.A.core < b.A.core;
        return a.A.memory < b.A.memory;
    });
    cin >> N_allVmType;
    for(int i = 0; i < N_allVmType; ++i) {
        vmInfo vm = readVm();
        vmInfos[vm.typeId] = vm;
    }
    cin >> T_days;
    for(int day = 0; day < T_days; ++day) {
        cin >> R_requests;
        vector<requestInfo> requests;
        for(int j = 0; j < R_requests; ++j) {
            requests.emplace_back(readRequest());
        }
        dailyProcess(requests, day);
    }

    for(string& s : output) {
        cout << s << "\n";
    }

    return 0;
}
#include <bits/stdc++.h>

#include <utility>
using std::cerr;
struct Ser_t {
    int core, cpu, tot, day;
};

struct Ser {
    std::string name;
    std::pair<int, int> ANode, BNode;
    Ser() = default;
    Ser(std::string _name, const Ser_t& _s) {
        name = std::move(_name);
        ANode = {_s.core / 2, _s.cpu / 2};
        BNode = {_s.core / 2, _s.cpu / 2};
    }
};

struct Vir_t {
    int core, cpu, type;
};

struct Vir {
    std::string name;
    int core{}, cpu{}, type{};
    int pos{}, numa{};
    Vir() = default;
    Vir(std::string _name, const Vir_t& _v, int _pos, int _numa) {
        name = std::move(_name);
        core = _v.core;
        cpu = _v.cpu;
        type = _v.type;
        pos = _pos;
        numa = _numa;
    }
};

void runJudger(std::string inputFile, std::string outputFile) {
    std::ifstream infile(inputFile);
    std::ifstream outfile(outputFile);
    std::string tmp_str;
    int server_type_num;
    infile >> server_type_num;
    infile.ignore();
    // read server type
    std::map<std::string, Ser_t> sts;
    for (int i = 0; i < server_type_num; ++i) {
        infile.ignore();
        getline(infile, tmp_str);
        tmp_str.pop_back();
        std::string name = tmp_str.substr(0, tmp_str.find(','));
        tmp_str = tmp_str.substr(tmp_str.find(',') + 2);
        int core = std::stoi(tmp_str.substr(0, tmp_str.find(',')));
        tmp_str = tmp_str.substr(tmp_str.find(',') + 2);
        int cpu = std::stoi(tmp_str.substr(0, tmp_str.find(',')));
        tmp_str = tmp_str.substr(tmp_str.find(',') + 2);
        int tot = std::stoi(tmp_str.substr(0, tmp_str.find(',')));
        tmp_str = tmp_str.substr(tmp_str.find(',') + 2);
        int day = std::stoi(tmp_str);
        sts[name] = {core, cpu, tot, day};
    }
    int vir_type_num;
    infile >> vir_type_num;
    infile.ignore();
    // read virtual type
    std::map<std::string, Vir_t> vts;
    for (int i = 0; i < vir_type_num; ++i) {
        infile.ignore();
        getline(infile, tmp_str);
        tmp_str.pop_back();
        std::string name = tmp_str.substr(0, tmp_str.find(','));
        tmp_str = tmp_str.substr(tmp_str.find(',') + 2);
        int core = std::stoi(tmp_str.substr(0, tmp_str.find(',')));
        tmp_str = tmp_str.substr(tmp_str.find(',') + 2);
        int cpu = std::stoi(tmp_str.substr(0, tmp_str.find(',')));
        tmp_str = tmp_str.substr(tmp_str.find(',') + 2);
        int type = std::stoi(tmp_str);
        vts[name] = {core, cpu, type};
    }
    long long cost = 0, mi = 0;
    std::vector<Ser> sers;
    std::map<int, Vir> virs; // id -> vir
    int T, K;
    infile >> T >> K;
    for (int date = 0; date < T; ++date) {
        if(date % 100 == 0) cerr << "day: " << date << std::endl;
        auto add_vir_2_ser = [&](const std::string & name, int add_id, int pos, int numa = -1) {
            virs[add_id] = Vir(name, vts[name], pos, numa);
            if (numa == 0) {
                sers[pos].ANode.first -= virs[add_id].core;
                sers[pos].ANode.second -= virs[add_id].cpu;
            } else if (numa == 1) {
                sers[pos].BNode.first -= virs[add_id].core;
                sers[pos].BNode.second -= virs[add_id].cpu;
            } else {
                sers[pos].ANode.first -= virs[add_id].core / 2;
                sers[pos].ANode.second -= virs[add_id].cpu / 2;
                sers[pos].BNode.first -= virs[add_id].core / 2;
                sers[pos].BNode.second -= virs[add_id].cpu / 2;
            }
            if(date == -1)
                cerr << "ser " << pos << ": (" << sers[pos].ANode.first << ", " << sers[pos].ANode.second << "), ("
                     << sers[pos].BNode.first << ", " << sers[pos].BNode.second << ")\n";
            if (sers[pos].ANode.first < 0 || sers[pos].ANode.second < 0 ||
                    sers[pos].BNode.first < 0 || sers[pos].BNode.second < 0) {
                if(date == -1) {
                    if(sers[pos].ANode.first < 0) std::cerr << "ANode core not enough\n";
                    if(sers[pos].ANode.second < 0) std::cerr << "ANode mem not enough\n";
                    if(sers[pos].BNode.first < 0) std::cerr << "BNode core not enough\n";
                    if(sers[pos].BNode.second < 0) std::cerr << "BNode mem not enough\n";
                }
                std::cerr << "date " << date << ": add vir " << add_id << " to ser " << pos << " filed\n";
                exit(0);
            }
        };
        auto del_vir_from_ser = [&](int del_id) {
            if (!virs.count(del_id)) {
                std::cerr << "date " << date << ": delete vir " << del_id << " filed\n";
                exit(0);
            }
            if (virs[del_id].type == 0) {
                if (virs[del_id].numa == 0) {
                    sers[virs[del_id].pos].ANode.first += virs[del_id].core;
                    sers[virs[del_id].pos].ANode.second += virs[del_id].cpu;
                } else {
                    sers[virs[del_id].pos].BNode.first += virs[del_id].core;
                    sers[virs[del_id].pos].BNode.second += virs[del_id].cpu;
                }
            } else {
                sers[virs[del_id].pos].ANode.first += virs[del_id].core / 2;
                sers[virs[del_id].pos].ANode.second += virs[del_id].cpu / 2;
                sers[virs[del_id].pos].BNode.first += virs[del_id].core / 2;
                sers[virs[del_id].pos].BNode.second += virs[del_id].cpu / 2;
            }
            virs.erase(del_id);
        };
        // read purchase
        getline(outfile, tmp_str);
        tmp_str.pop_back();
        int buy_num = std::stoi(tmp_str.substr(11));
        for (int i = 0; i < buy_num; ++i) {
            outfile.ignore();
            getline(outfile, tmp_str);
            tmp_str.pop_back();
            std::string name = tmp_str.substr(0, tmp_str.find(','));
            int t_num = std::stoi(tmp_str.substr(tmp_str.find(',') + 2));
            while (t_num--) {
                if (!sts.count(name)) {
                    std::cerr << "date " << date << ": buy server " << name << " filed\n";
                    exit(0);
                }
                sers.emplace_back(name, sts[name]);
                cost += sts[name].tot;
            }
        }
        // read migration
        getline(outfile, tmp_str);
        tmp_str.pop_back();
        int mi_num = std::stoi(tmp_str.substr(12));
        mi += mi_num;
        if (mi_num > 3 * virs.size() / 100) {
            std::cerr << mi_num << " / " << virs.size() << std::endl;
            std::cerr << "date " << date << ": migration exceed 3*n/100\n";
            exit(0);
        }
        for (int i = 0; i < mi_num; ++i) {
            outfile.ignore();
            getline(outfile, tmp_str);
            tmp_str.pop_back();
            int vir_id = std::stoi(tmp_str.substr(0, tmp_str.find(',')));
            tmp_str = tmp_str.substr(tmp_str.find(',') + 2);
            int pos = std::stoi(tmp_str.substr(0, tmp_str.find(',')));
            tmp_str = tmp_str.substr(tmp_str.find(',') + 2);
            std::string name = virs[vir_id].name;
            int numa = -1;
            if (virs[vir_id].type == 0) {
                std::string ANode_or_BNode = tmp_str.substr(0, tmp_str.find(','));
                // numa = std::stoi(tmp_str.substr(0, tmp_str.find(',')));
                numa = ANode_or_BNode == "ANode" ? 0 : 1;
                tmp_str = tmp_str.substr(tmp_str.find(',') + 2);
            }
            // migration
            del_vir_from_ser(vir_id);
            add_vir_2_ser(name, vir_id, pos, numa);
        }
        // read operator
        int n;
        infile >> n;
        infile.ignore();
        while (n--) {
            infile.ignore();
            getline(infile, tmp_str);
            tmp_str.pop_back();
            if (tmp_str[0] == 'a') {
                tmp_str = tmp_str.substr(5);
                std::string name = tmp_str.substr(0, tmp_str.find(','));
                tmp_str = tmp_str.substr(tmp_str.find(',') + 2);
                int add_id = std::stoi(tmp_str);

                outfile.ignore();
                getline(outfile, tmp_str);
                tmp_str.pop_back();
                if (vts[name].type == 0) {
                    if (tmp_str.back() == 'ANode') {
                        for (int _ = 0; _ < 3; ++_) tmp_str.pop_back();
                        int pos = std::stoi(tmp_str);
                        add_vir_2_ser(name, add_id, pos, 0);
                    } else {
                        for (int _ = 0; _ < 3; ++_) tmp_str.pop_back();
                        int pos = std::stoi(tmp_str);
                        add_vir_2_ser(name, add_id, pos, 1);
                    }
                } else {
                    int pos = std::stoi(tmp_str);
                    add_vir_2_ser(name, add_id, pos, -1);
                }
            } else {
                tmp_str = tmp_str.substr(5);
                int del_id = std::stoi(tmp_str);
                del_vir_from_ser(del_id);
            }
        }
        // calculate daily-cost
        for (const auto& se : sers) {
            if (se.ANode.first + se.BNode.first != sts[se.name].core || se.ANode.second + se.BNode.second != sts[se.name].cpu) {
                cost += sts[se.name].day;
            }
        }
    }
    std::cerr << "total cost: " << cost << '\n';
    std::cerr << "migration num: " << mi << '\n';
}
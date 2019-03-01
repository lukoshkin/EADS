#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <string>



class Box {
    std::pair<int,int> base;
    public:
    int small, nb;
    Box(std::vector<int> triple,int i) : small(triple[0]), nb(i) 
    {
        base = std::make_pair(triple[1],triple[2]);
    }
    std::pair<int,int> Get_base() {return base;}
    friend bool operator== (const Box&,const Box&);
    friend bool Compare(const Box&,const Box&);
};

bool operator== (const Box& b1, const Box& b2) {
    return ((b1.base.first == b2.base.first) && (b1.base.second == b2.base.second));
}

bool Compare(const Box& b1, const Box& b2) {
    return b1.small < b2.small;
}



class Hash {
    int m, seed;
    public:
    Hash() {}
    Hash(int mm,int ss) : m(mm), seed(ss) {}
    int Fun(std::pair<int,int> base) {
        int a,b;
        std::srand(seed);
        a = std::rand() % m;
        b = std::rand() % m;
        return (a * base.first + b * base.second) % m;
    }
};



class HashTable {
    int tabsize;
    Hash hmain;
    std::vector<Box> tmp_store;
    std::vector<std::vector<Box>> layer;
    std::vector<std::vector<Box>> perfect;
    public:
    HashTable(std::string);
    std::vector<std::vector<Box>> Distribute();
    void MakePerfect();
    std::tuple<int,int,int> MaxDim();

};

HashTable::HashTable(std::string filename) {
    std::ifstream infile(filename);
    std::string line;
    std::vector<int> triple(3);
    std::getline(infile,line);
    tabsize = std::stoi(line);
    std::vector<Box> empty_vector;
    for (int i=0;i<tabsize;++i) {
        layer.push_back(empty_vector);
        std::getline(infile,line);
        std::istringstream iss(line);
        iss >> triple[0] >> triple[1] >> triple[2];
        std::sort(triple.begin(),triple.begin()+3);
        Box box(triple,i+1);
        tmp_store.push_back(box);
    }
    infile.close();

    int seed = time(NULL);
    Hash h(tabsize,seed);
    for (auto&& it: tmp_store) 
        layer[h.Fun(it.Get_base())].push_back(it);  
}

void HashTable::MakePerfect() {
    int k;
    std::vector<Box> filter;
    for (int i=0;i<tabsize;++i) {
        if (layer[i].empty())
            continue;
        if (layer[i].size() == 1) {
            perfect.push_back(layer[i]);
            continue;
        }
        k = 0;
        while (!layer[i].empty()) {
            filter.clear();
            auto elem = layer[i][0];
            k = 0;
            while  (k + layer[i].begin() != layer[i].end()) {
                if (layer[i][k] == elem) {
                    filter.push_back(layer[i][k]);
                    layer[i].erase(layer[i].begin() + k);
                    continue;
                }
                k++;
            }
            perfect.push_back(filter);
        }
    }
}

std::tuple<int,int,int> HashTable::MaxDim() {
    int res(0), pos1(0), pos2(0); 
    int a,b,_pos1,_pos2,_res0,_res1,_res2,_res;
    for (auto&& it: perfect){
        if (it.empty())
            continue;
        if (it.size() > 1) {
            a = it[0].Get_base().first;
            b = it[0].Get_base().second;
            _res0 = std::min(a,b);

            auto _emax = std::max_element(it.begin(),it.end(),Compare);
            _pos1 = _emax->nb;
            _res1 = _emax->small;
            it.erase(_emax);

            _emax = std::max_element(it.begin(),it.end(),Compare);
            _pos2 = _emax->nb;
            _res2 = _emax->small;
            _res = _res1 + _res2;
            _res = std::min(_res0,_res);

            if (res < _res) {
                res = _res;
                pos1 = _pos1;
                pos2 = _pos2;
            }

        } else {
            _res = it[0].small;
            _pos1 = it[0].nb;
            if (res < _res) {
                res = _res;
                pos1 = _pos1;
                pos2 = 0;
            }
        }
    }
    return std::make_tuple(res,pos1,pos2);
}


int main() {
    HashTable htab("input.txt");
    htab.MakePerfect();
    auto t = htab.MaxDim();
    int res = std::get<0>(t);
    int pos1 = std::get<1>(t);
    int pos2 = std::get<2>(t);
    std::ofstream outfile("output.txt");
    if (pos2) {
        outfile << "2\n";
        if (pos1 < pos2)
            outfile << pos1 << ' ' << pos2 << '\n';
        else
            outfile << pos2 << ' ' << pos1 << '\n';
        outfile << res;
    } else {
        outfile << "1\n";
        outfile << pos1 << '\n';
        outfile << res;
    }
    outfile.close();
}

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
      friend bool CompareBase(const Box&,const Box&);
      friend bool SmallCompare(const Box&,const Box&);
};
   
bool operator== (const Box& b1, const Box& b2) {
  return ((b1.base.first == b2.base.first) && (b1.base.second == b2.base.second));
}

bool CompareBase(const Box& b1, const Box& b2) {
    if (b1.base.first < b2.base.first)
        return true;
    if (b1.base.first == b2.base.first)
        return (b1.base.second <= b2.base.second);
    return false;
}

bool SmallCompare(const Box& b1, const Box& b2) {
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
      void operator= (const Hash& some_h) {
          m = some_h.m; seed = some_h.seed;
      }
      int Size() {return m;}
};



class HashTable {
    int n;
    Hash hmain;
    std::vector<Hash> hh;
    std::vector<Box> tmp_store;
    std::vector<std::vector<Box>> layer;
    std::vector<std::vector<std::vector<Box>>> table;
    public:
      HashTable(std::string);
      std::tuple<int,int,int> MaxDim();
};

int unique(std::vector<Box> array) {
    std::sort(array.begin(),array.end(),CompareBase);
    return std::unique(array.begin(),array.end()) - array.begin();
}

HashTable::HashTable(std::string filename) {
    std::ifstream infile(filename);
    std::string line;
    std::vector<int> triple(3);
    std::getline(infile,line);
    int p = std::stoi(line);
    std::vector<Box> empty_vector;
    
    for (int i=0;i<p;++i) 
        layer.push_back(empty_vector);
    for (int i=0;i<p;++i) 
	table.push_back(layer);
    for (int i=0;i<p;++i) {
	hh.push_back(hmain);
        std::getline(infile,line);
        std::istringstream iss(line);
        iss >> triple[0] >> triple[1] >> triple[2];
        std::sort(triple.begin(),triple.begin()+3);
        Box box(triple,i+1);
        tmp_store.push_back(box);
    }
    infile.close();

    n = unique(tmp_store);
    int seed,squares_sum;
    do {
        for (int i=0;i<n;++i)
            layer[i].clear();   
        seed = time(NULL);
        Hash h(n,seed);
        for (auto&& it: tmp_store) 
            layer[h.Fun(it.Get_base())].push_back(it);  
        squares_sum = 0;
        for (int i=0;i<n;++i)
            squares_sum += std::pow(unique(layer[i]),2);
        hmain = h;
    } while (squares_sum >= 4 * n);
    
    int _ni,_m, _sum;
    for (int i=0;i<n;++i) {
        if (layer[i].empty())
            continue;
        _ni = unique(layer[i]);
        _m = _ni * _ni;
	for (int j=0;j<_m;++j)
	    table[i].push_back(empty_vector);    
        do {
            for (int j=0;j<_m;++j)
                table[i][j].clear();
            seed = time(NULL);
            Hash h(_m,seed);
            for (auto&& it: layer[i]) 
                table[i][h.Fun(it.Get_base())].push_back(it);
            _sum = 0;
            for (int j=0;j<_m;++j) {
                if (!table[i][j].empty())
                    _sum += (unique(table[i][j]) == 1);
            }
            hh[i] = h;
        } while (_sum != _ni);
    }
}

std::tuple<int,int,int> HashTable::MaxDim() {
    int res(0), pos1(0), pos2(0); 
    int a,b,_pos1,_pos2,_res0,_res1,_res2,_res;
    for (int i=0;i<n;++i){
        if (layer[i].empty())
            continue;
        for (int j=0;j<hh[i].Size();++j) {
            if (table[i][j].empty())
                continue;
            if (table[i][j].size() > 1) {
                a = table[i][j][0].Get_base().first;
                b = table[i][j][0].Get_base().second;
                _res0 = std::min(a,b);
                    
                auto _emax = std::max_element(table[i][j].begin(),table[i][j].end(),SmallCompare);
                _pos1 = _emax->nb;
                _res1 = _emax->small;
                table[i][j].erase(_emax);

                _emax = std::max_element(table[i][j].begin(),table[i][j].end(),SmallCompare);
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
                _res = table[i][j][0].small;
                _pos1 = table[i][j][0].nb;
                if (res < _res) {
                   res = _res;
                   pos1 = _pos1;
                   pos2 = 0;
                }
            }
        }
    }
    return std::make_tuple(res,pos1,pos2);
}


int main() {
    HashTable htab("input.txt");
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

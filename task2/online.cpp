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
    int Basemin() {return std::min(base.first,base.second);}
    friend bool operator== (const Box&,const Box&);
    friend bool Compare(const Box&,const Box&);
};

bool operator== (const Box& b1, const Box& b2) {
    return ((b1.base.first == b2.base.first) && (b1.base.second == b2.base.second));
}

bool Compare(const Box& b1, const Box& b2) {
    return b1.small < b2.small;
}



class UniqueDepot {
    int maxdim;
    int pos1,pos2,pair;
    std::vector<std::vector<Box>> depo;
    public:
    UniqueDepot(int);
    void Submit(Box,int);
    std::tuple<int,int,int> Show() {return std::make_tuple(maxdim,pos1,pos2);}
};

UniqueDepot::UniqueDepot(int n) {
    maxdim = 0;
    pos1 = 0;
    pos2 = 0;
    std::vector<Box> empty_vector;
    for (int i=0;i<n;++i)
        depo.push_back(empty_vector);
}

void UniqueDepot::Submit(Box box, int hash) {
    int xflag = 0;
    auto it = depo[hash].begin();
    if (box.small > maxdim) {
        maxdim = box.small;
        pos1 = box.nb;
        pos2 = 0;
    }
    while (it != depo[hash].end()) {
        if (xflag) {
            if (*it == box) {
                depo[hash].erase(it);
            } else {
                it++;
            }
        } else {
            if (maxdim >= box.Basemin()) {
                break;
            }
            if (*it == box) {
                if (std::min(it->small + box.small,box.Basemin()) < maxdim) {
                    if (it->small < box.small) {
                        it = depo[hash].erase(it);
                        it = depo[hash].insert(it,box);
                    }
                } else {
                    maxdim = std::min(it->small + box.small,box.Basemin());
                    pos1 = box.nb;
                    pos2 = it->nb;
                    if (pos1 > pos2) 
                        std::swap(pos1,pos2);
                    if (it->small < box.small) {
                        it = depo[hash].insert(it,box);
                        it++;
                    } else { 
                        it = depo[hash].insert(it+1,box);
                    }
                }
                xflag = 1;
            }
            it++;
        }
    }
    if (xflag == 0 && it == depo[hash].end()) 
        depo[hash].push_back(box);
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

int main() {
    std::ifstream infile("input.txt");
    std::string line;
    std::vector<int> triple(3);
    std::getline(infile,line);
    int hash;
    int n = std::stoi(line);
    Hash h(n,0);
    UniqueDepot depot(n);
    for (int i=0;i<n;++i) {
        std::getline(infile,line);
        std::istringstream iss(line);
        iss >> triple[0] >> triple[1] >> triple[2];
        std::sort(triple.begin(),triple.begin()+3);
        Box box(triple,i+1);
        hash = h.Fun(box.Get_base());
        depot.Submit(box,hash);
    }
    infile.close();
    auto[maxdim,pos1,pos2] = depot.Show();
    std::ofstream outfile("output.txt");
    if (pos2) {
        outfile << "2\n";
        if (pos1 < pos2)
            outfile << pos1 << ' ' << pos2 << '\n';
        else
            outfile << pos2 << ' ' << pos1 << '\n';
        outfile << maxdim;
    } else {
        outfile << "1\n";
        outfile << pos1 << '\n';
        outfile << maxdim;
    }
    outfile.close();
}


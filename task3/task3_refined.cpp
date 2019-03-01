#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <assert.h>

class Region;

//--------------- class Point ---------------------------
class Point {
    public:
    int x,y;
    int used;
    Point() : x(-1), y(-1), used(0) {}
    Point (int x,int y) : x(x), y(y), used(0) {}
    Point (const Point& P) { x = P.x; y = P.y; used = P.used; }
    bool is_in (const Region&);
    int operator[] (int) const;
    bool operator!= (const Point& P) {
        return ((x != P.x) || (y != P.y));
    }
};

int Point::operator[] (int id) const {
    assert (id < 2);
    if (id) return y;
    return x;
}

//--------------- struct Mine ----------------------------
struct Mine: public Point {
    int W;
    Mine() : W(0) {};
    Mine (std::vector<int> arg) : Point(arg[0],arg[1]), W(arg[2]) {}
    void Print() {
        std::cout << '('<< x << ',' << y << ',' 
        << W << ',' << used << ")\n";
    }
};

struct Comparator {
    Comparator(int which) : which(which) {}
    bool operator()(const Mine& s1, const Mine& s2) {
        return s1[which] < s2[which];
    }
    private:
      int which;
};

struct UnaryComparator {
    UnaryComparator(Mine mm, std::string order, int dim) : 
        dim(dim), order(order), mm(mm) {}
    bool operator()(const Mine& s) {
        if (order == "greater")
            return s[dim] >= mm[dim];
        return s[dim] < mm[dim];
    }
    private:
      int dim;
      std::string order;
      Mine mm;
};



//---------------- class Region ----------------------------
// My rectangle is traditionally set by its lower left(LL) and 
// upper right(UR) points
class Region {
    public:
      Point LL, UR;
      Region() {}
      Region (std::vector<int> arg) :
            LL(arg[0],arg[1]), UR(arg[2],arg[3]) {}
//      Region (Point P1, Point P2) : LL(P1), UR(P2) {} 
//      Region (const Region& reg) : LL(reg.LL), UR(reg.UR) {}
//      // these two constructors seem not be used here
};

bool Point::is_in (const Region& reg) {
    if ((reg.LL[0] <= x) && 
        (x <= reg.UR[0]) &&
        (reg.LL[1] <= y) &&
        (y <= reg.UR[1]))
        return true;
    return false;
}

// This function is only needed for converting input data
// into the working format
std::vector<int> transform (std::vector<int> quadruple) {
    std::vector<int> transformed(4);
    transformed[0] = quadruple[0];
    transformed[1] = quadruple[3];
    transformed[2] = quadruple[2];
    transformed[3] = quadruple[1];
    return transformed;
}



//----------------- struct Node -----------------------------------
struct Node {
      Node * left;
      Node * right;
      Mine pit;
      Node() : left(nullptr), right(nullptr) {}
};



//---------------- builder-functions of  Kd-Tree ----------------------------------
int get_median(std::vector<Mine> &Pset,int lvl) {
    Comparator compare_by(lvl % 2);
    std::sort(Pset.begin(),Pset.end(),compare_by);
    return Pset.size() / 2;  // Strictly speaking, this part should be rewritten  
                             // into a faster algorithm which is O(n). 
                             // Then the kd-tree building will be O(n log n)
                             // So far, it is O(n log^2 n)
}

std::vector<Mine> 
minorsSet(std::vector<Mine> Pset, Mine pit, int lvl) {
    std::vector<Mine> Subset(Pset.size());
    UnaryComparator compare_by(pit,"lesser",lvl % 2);
    auto it = std::copy_if(Pset.begin(),Pset.end(),Subset.begin(),compare_by);
    Subset.resize(it - Subset.begin());
    return Subset;    
}

std::vector<Mine> 
majorsSet(std::vector<Mine> Pset, Mine pit, int lvl) {
    std::vector<Mine> Subset(Pset.size());
    UnaryComparator compare_by(pit,"greater",lvl % 2);
    auto it = std::copy_if(Pset.begin(),Pset.end(),Subset.begin(),compare_by);
    Subset.resize(it - Subset.begin());
    return Subset;    
}

void build(Node * t,std::vector<Mine> Pset,int lvl) {
    if (Pset.empty()) return;
    if (Pset.size() == 1) {
        Pset[0].used += 1;
        t->pit = Pset[0];
       return;
    }
    int median = get_median(Pset,lvl);
    Pset[median].used += 1;
    t->pit = Pset[median];
    auto Pset1 = minorsSet(Pset,t->pit,lvl);
    auto Pset2 = majorsSet(Pset,t->pit,lvl);
    
    if (Pset1.size() != 1) {
        t->left = new Node;
        build(t->left,Pset1,lvl+1);
    } else { 
        if (!Pset1[0].used) {
            Pset1[0].used += 1;
            t->left = new Node;
            t->left->pit = Pset1[0];
        }
    }
    if (Pset2.size() != 1) {
        t->right = new Node;
        build(t->right,Pset2,lvl+1);
    } else {
        if (!Pset2[0].used) {
            Pset2[0].used += 1;
            t->right = new Node;
            t->right->pit = Pset2[0];
        }
    }
}



//---------------------- search procedure ----------------------------------------
void search (Node * t,const Region& reg, std::vector<Mine>& discovered, int lvl) {
    if (t == nullptr) return;
    if (reg.LL[lvl % 2] <= t->pit[lvl % 2]) {
        search(t->left,reg, discovered,lvl+1);
        if (t->pit.is_in(reg)) {
            if (!t->pit.used) { 
                discovered.push_back(t->pit);
                t->pit.used += 1;
            }
        }
    }
    if (reg.UR[lvl % 2] > t->pit[lvl % 2]) {
        search(t->right,reg, discovered,lvl+1);
        if (t->pit.is_in(reg)) {
            if (!t->pit.used) { 
                discovered.push_back(t->pit);
                t->pit.used += 1;
            }
        }
    }
}



//--------------------- little helpers ------------------------
int collect (std::vector<Mine> discovered) {
    int sum = 0;
    for (auto& it: discovered)
        sum += it.W;
    return sum;
}

void nullify (Node * t) {
    if (t == nullptr) return;
    t->pit.used = 0;
    nullify(t->left);
    nullify(t->right);
}

void inorder(Node * t) {
    if (t == nullptr) return;
    t->pit.Print();
    inorder(t->left);
    inorder(t->right);
}

void clear(Node * t) {
    if (t == nullptr) return;
    clear(t->left);
    clear(t->right);
    delete t;
}



//----------------------- MAIN --------------------------------
int main() {
    // read input
    std::ifstream infile("input.txt");
    std::string line;
    std::vector<int> triple(3);
    std::vector<Mine> feed;
    std::getline(infile,line);
    int num; // number of mines 
    num = std::stoi(line);
    for (int i=0;i<num;++i) {
        std::getline(infile,line);
        std::istringstream iss(line);
        iss >> triple[0] >> triple[1] >> triple[2];
        Mine pit(triple);
        feed.push_back(pit);
    }
    int k;
    std::vector<int> quadruple(4);
    std::vector<Region> query;
    std::vector<std::vector<Region>> queries;
    std::getline(infile,line);
    num = std::stoi(line); // now it is redefined as the number of queries
    for (int i=0;i<num;++i) {
        std::getline(infile,line);
        k = std::stoi(line); 
        for (int j=0;j<k;++j) {
            std::getline(infile,line);
            std::istringstream iss(line);
            for (int m=0;m<4;++m)
                iss >> quadruple[m];
            Region reg(transform(quadruple));
            query.push_back(reg);
        }
        queries.push_back(query);
    }
    infile.close();

    // kd-tree
    Node * root = new Node;
    std::vector<Mine> discovered;
    std::vector<int> results(num);
    build(root,feed,0);

    // bulk search 
    int power, cnt(0);
    for (auto& query: queries) {
        power = 0;
        nullify(root);
        for (auto& reg: query) {
            search(root,reg,discovered,0);
        }
        results[cnt] = collect(discovered);
        cnt++;
        discovered.clear();
    }
    // release the allocated memory
    clear(root);
    
    // output the results 
    std::ofstream outfile ("output.txt");
    for (int i=0;i<num-1;++i)
        outfile << results[i] << '\n';
    outfile << results[num-1];
    outfile.close();
}

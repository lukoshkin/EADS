#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#define inf 1000

//========================= Priority Queue =============================

class PriorityQueue
{
    std::vector<int> A;
    std::map<int,int> dist;

    int Heapify(int);
    int FindParent(int i) {return std::floor((i + 1)/ 2) - 1;}
    int height(int size) {return std::floor(std::log2(size));}
    int full_capacity(int size) {int h = height(size);
        return std::pow(2,h+1);}
    public:
      void BuildHeap(const std::map<int,int>&);
      int Max() {return A[0];};
      void ExtractMax();
      void IncreaseKey(int,int);
      void Insert(int,int);
      int GetDistAt(int i) {return dist[i];}
      bool Empty() {return A.empty();}
      int Print();
      std::map<int,int> Copy() {return dist;}
};



int PriorityQueue::Heapify(int i) {
    if (i > std::floor(A.size() / 2) - 1)  
        return 0;
    if ((dist[A[2*i+1]] < dist[A[i]]) or 
        (dist[A[2*i+2]] < dist[A[i]])) {
        if (dist[A[2*i+2]] < dist[A[2*i+1]]) {
            std::swap(A[i],A[2*i+2]);
            Heapify(2*i+2);
        } else {
            std::swap(A[i],A[2*i+1]);
            Heapify(2*i+1);
        }
    }
    return 0;
}

void PriorityQueue::BuildHeap(const std::map<int,int>& B) {
    if (!A.empty())
        A.clear();
    for (auto&& it: B)
        A.push_back(it.first);
    dist = B;
    for (int i=std::floor(A.size()/2)-1;i>=0;--i) 
        Heapify(i);
}

void PriorityQueue::ExtractMax() {
    std::swap(A[0],A.back());
    A.pop_back();
    Heapify(0);
}

inline void PriorityQueue::IncreaseKey(int i,int k) {
    dist[i] = k;
    i = std::find(A.begin(),A.end(),i) - A.begin(); 
    while (dist[A[FindParent(i)]] > dist[A[i]]) {
        std::swap(A[FindParent(i)],A[i]);
        i = FindParent(i);
        if (i == 0)
            break;
    }
}

void PriorityQueue::Insert(int i,int k) {
    A.push_back(i);
    dist.emplace(i,inf);
    IncreaseKey(i,k);
}

int PriorityQueue::Print() {
    int step, p, k(0), gapsize(3);
    for (int i=1;i<=height(A.size())+1;++i) {
        step = full_capacity(A.size()) / std::pow(2,i);
        p = 0;
        for (int j=step;j<full_capacity(A.size());j+=step) {
            std::cout << std::string(gapsize * (step-1),' ');
            if (p % 2) {
                std::cout << std::string(gapsize,' ');
            } else {
                std::cout <<'(' << A[k] << ',' << dist[A[k]] << ')';
                k++;
            }
            if (k == (int)A.size()) {
                std::cout << '\n';
                return 0;
            }
            p++;
        }
        std::cout << '\n';
    }
    return 0;
}


//================ Dijkstra's Algorithm =======================================

class StrangeDijkstra {
    int source,destination;
    bool early_exit;
    std::vector<std::pair<int,int>> edges;
    std::vector<int> _outsets;
    std::vector<bool> visited;
    std::map<int,int> path;
    PriorityQueue Q;
    public:
      StrangeDijkstra(std::string,bool);
      void BFS();
      void WriteTheResult();
};

StrangeDijkstra::StrangeDijkstra(std::string filename, bool ee=true) : early_exit(ee) {
    std::ifstream infile(filename);
    std::string line, two;
    std::vector<int> tmp_store;
    int one;
    while (std::getline(infile,line)) {
        std::istringstream iss(line);
        iss >> one >> two;
        tmp_store.push_back(one);
        if (!two.empty())
            tmp_store.push_back(std::stoi(two));
    }
    infile.close();

    source = tmp_store[2];
    destination = tmp_store[3];

    for (auto it=tmp_store.begin()+4;it!=tmp_store.end();it+=2)
        edges.push_back(std::make_pair(*it,*(it + 1)));
    std::sort(edges.begin(),edges.end());
    for (auto& it: edges)
        _outsets.push_back(it.first);
    for (int i=0;i<tmp_store[0];++i) 
        path[i] = inf;
    path[source] = 0;
    visited.insert(visited.end(),tmp_store[0],false);
    visited[source] = true;
}

void StrangeDijkstra::BFS() {
    int current, successor;
    std::vector<int>::iterator from, to;
    std::map<int,int> primer; 
    primer.insert(std::make_pair(source,0));
    Q.BuildHeap(primer);
    while (!Q.Empty()) {
        current = Q.Max();
        Q.ExtractMax();
        from = std::lower_bound(_outsets.begin(),_outsets.end(),current);
        to = std::upper_bound(_outsets.begin(),_outsets.end(),current);
        for (auto it=from;it!=to;++it) {
            successor = edges[it - _outsets.begin()].second;
            if (!visited[successor]) {
                visited[successor] = true;
                path[successor] = path[current] + 1;
                Q.Insert(successor,path[successor]);
            }
        }
        if ((early_exit) && (current == destination))
            break;
    }
}

void StrangeDijkstra::WriteTheResult() {
    int res(0);
    for (int i=0;i<path[destination];++i)
        res += (i % 2) ? 2 : 1; 
    std::ofstream outfile("output.txt");
        outfile << res;
    outfile.close();
}



//====================== MAIN ====================================
int main() {
    StrangeDijkstra SD("input.txt");
    SD.BFS();
    SD.WriteTheResult();
}

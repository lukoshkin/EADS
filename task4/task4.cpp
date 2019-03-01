#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

std::vector<int> weight, n_lines, loss;
void tabulate (int);
int L;

int main() {
    std::ifstream infile ("input.txt");
    std::string str;
    std::getline(infile,str);
    L = std::stoi(str);
    std::getline(infile,str);
    std::istringstream iss(str);
    while (iss >> str) {
        weight.push_back(str.size());
    }
    infile.close();


    int num_words = weight.size();
    loss.resize(num_words); 
    n_lines.resize(num_words); 

    
    tabulate(num_words);
    std::ofstream outfile ("output.txt");
      outfile << loss.back() << '\n';
      outfile << n_lines.back();
    outfile.close();
}

int square (int x) { return x * x; }

void tabulate (int n) {
    int k,a, tail(weight[0]);
    loss[0] = square(L - tail);
    for (int i=1;i<n;++i) {
        k = 1;
        tail = weight[i];
        loss[i] = loss[i-1] + square(L - tail); 
        while (tail + weight[i-k] + 1 <= L) {
            tail += weight[i-k] + 1;
            if (i == k) {
                loss[i] = square(L - tail);
                n_lines[i] = 1;
                break;
            } else {
                a = loss[i-k-1] + square(L - tail);
                if (a < loss[i]) {
                    loss[i] = a;
                    n_lines[i] = n_lines[i-k-1] + 1;
                }
            }
            k++;
        }
    }
}

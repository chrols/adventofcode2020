#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <map>

std::map<int, std::vector<int>> mem;
std::map<int, long> mem2;

std::vector<int> build_chain(int current_adapter, const std::set<int> &adapters) {

    auto it = mem.find(current_adapter);
    if (it != mem.end()) {
        return it->second;
    }

    // std::cout << "Looking at: " << current_adapter << std::endl;
    std::vector<int> r { current_adapter };

    std::vector<int> t;

    for (int i = 0; i < 3; i++) {
        std::vector<int> temp;
        auto p_adapter = current_adapter - (i + 1);
        if (adapters.count(p_adapter)) {
            temp = build_chain(p_adapter, adapters);
            if (temp.size() > t.size()) {
              t = temp;
            }
        }
    }

    r.insert(r.end(), t.begin(), t.end());

    mem[current_adapter] = r;

    return r;
}

long find_ways(int current_adapter, const std::set<int> &adapters) {
    if (current_adapter == 0) return 1;
    if (!adapters.count(current_adapter)) return 0;

    auto it = mem2.find(current_adapter);
    if (it != mem2.end()) {
        return it->second;
    }

    long ways = 0;

    for (int i = 1; i <= 3; i++) {
        ways += find_ways(current_adapter - i, adapters);
    }

    mem2[current_adapter] = ways;
    return ways;
}

int main(int argc, char **argv) {
    std::ifstream ifs("input10");

    std::set<int> adapters;

    std::copy(std::istream_iterator<int>(ifs),
              std::istream_iterator<int>(),
              std::inserter(adapters, adapters.end()));

    auto largest = *adapters.rbegin();

    auto list = build_chain(largest+3, adapters);
    list.push_back(0);

    int num_1 = 0, num_3 = 0;

    for (int i = 0; i < list.size()-1; i++) {
        auto diff = list[i] - list[i+1];
        switch (diff) {
        case 1:
            num_1++;
            break;
        case 3:
            num_3++;
            break;
        }
    }

    std::cout << num_1 * num_3 << std::endl;
    std::cout << find_ways(largest, adapters) << std::endl;
}

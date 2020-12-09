#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

using Bag = std::string;
using Rules = std::map<Bag, std::map<Bag, int>>;


bool bag_contains_bag(const Bag &source, const Bag &target, Rules &rules) {
    for (const auto sub_bag : rules[source]) {
        if (sub_bag.first == target) {
            return true;
        }
    }

    return false;
}

std::vector<Bag> bags_containing_bag(const Bag &bag, Rules &rules) {
    std::vector<Bag> containing;

    for (auto inspected_bag : rules) {
        if (bag_contains_bag(inspected_bag.first, bag, rules)) {
            containing.push_back(inspected_bag.first);
            std::cout << inspected_bag.first << " contains " << bag << std::endl;
        }
    }

    return containing;
}

long count_sub_bags(const Bag &bag, Rules &rules) {
    int num_total = 1;
    for (const auto &e : rules[bag]) {
        num_total += e.second * count_sub_bags(e.first, rules);
    }
    return num_total;
}

int main(int argc, char **argv) {
    std::ifstream ifs("input7");

    Rules rules;

    std::string adj, color, temp;

    while (ifs >> adj >> color) {
        ifs >> temp >> temp; // "bags contain"

        ifs >> temp;

        if (temp == "no") {
            ifs >> temp >> temp; // "other bags."
        } else {
          for (auto it = temp.rbegin(); it != temp.rend(); it++) {
              ifs.putback(*it);
          }
            auto &cmap = rules[adj + color];

            // std::cout << adj + color << std::endl;

            do {
                int amount = 0;
                ifs >> amount >> adj >> color >> temp; // [amount] [adj] [color] bag(s),/.
                cmap[adj + color] = amount;
            } while (ifs.peek() != '\n' );

            // for (auto &e : cmap) {
            //     std::cout << e.first << "=>" << e.second << std::endl;
            // }
        }
    }

    for (auto &e : rules) {
        std::cout << e.first << std::endl;
    }

    std::vector<Bag> eventually;
    std::vector<Bag> stack;
    stack.push_back("shinygold");

    do {
        Bag target = stack.back();
        stack.pop_back();

        for (auto e : bags_containing_bag(target, rules)) {
            eventually.push_back(e);
            stack.push_back(e);
        }
    } while (!stack.empty());

    std::sort(eventually.begin(), eventually.end());

    auto last = std::unique(eventually.begin(), eventually.end());
    eventually.erase(last, eventually.end());

    for (auto e : eventually) {
        std::cout << e << std::endl;
    }



    std::cout << eventually.size() << std::endl;
    std::cout << count_sub_bags("shinygold", rules) - 1 << std::endl;
}

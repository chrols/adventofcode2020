#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <deque>

const long PREAMBLE_LEN = 25;

std::deque<std::pair<long, std::set<long>>> collate_preamble(std::istream &istream) {
    std::deque<std::pair<long, std::set<long>>> r;

    for (long i = 0; i < PREAMBLE_LEN; i++) {
        long temp;
        istream >> temp;
        r.push_back(std::make_pair(temp, std::set<long>()));
    }

    for (long i = 0; i < PREAMBLE_LEN-1; i++) {
        auto &e = r[i];
        for (long j = i + 1; j < PREAMBLE_LEN; j++) {
            e.second.insert(e.first + r[j].first);
        }
    }

    return r;
}

bool valid_number(long number, std::deque<std::pair<long, std::set<long>>> &q) {
  for (const auto &e : q) {
    if (e.second.count(number) != 0) {
        return true;
    }
  }

  return false;
}

void process_number(long number, std::deque<std::pair<long, std::set<long>>> &q) {
    q.pop_front();
    for (auto &e : q) {
        e.second.insert(e.first + number);
    }
    q.push_back(std::make_pair(number, std::set<long>()));
}

long find_invalid() {
    std::ifstream ifs("input9");

    auto collation = collate_preamble(ifs);

    long temp;
    while (ifs >> temp) {
        if (!valid_number(temp, collation)) {
            return temp;
        }

        process_number(temp, collation);
    }

    return -1;
}

void find_seq(long target) {
    std::ifstream ifs("input9");

    std::vector<long> l;
    long t;
    while (ifs >> t) {
        l.push_back(t);
    }

    std::vector<long> sums = l;

    size_t width = 2;

    while (width <= l.size()) {
        sums.pop_back();
        for (int i = 0; i < sums.size(); i++) {
            auto new_e = l[i + width - 1];
            sums[i] += new_e;
            if (sums[i] == target) {
                long min = l[i];
                long max = l[i];
                for (int j = i+1; j < i + width - 1; j++) {
                    min = std::min(min, l[j]);
                    max = std::max(max, l[j]);
                }

              std::cout << min << " & " << max << " = " << min + max
                        << std::endl;
              return;
            }
        }
        width++;
    }

    std::cerr << "Impossible" << std::endl;
}

int main(int argc, char **argv) {
    auto invalid = find_invalid();
    std:: cout << invalid << std::endl;
    find_seq(invalid);
}

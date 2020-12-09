#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <tuple>
#include <map>
#include <sstream>
#include <set>

int solve1(const std::vector<int> &entries) {
    for (int i = 0; i < entries.size() - 1; i++) {
        for (int j = i + 1; j < entries.size(); j++) {
            if (entries[i] + entries[j] == 2020) {
                return entries[i] * entries[j];
            }
        }
    }
}

int solve2(const std::vector<int> &entries) {
    for (int i = 0; i < entries.size() - 2; i++) {
        for (int j = i + 1; j < entries.size() - 1; j++) {
            for (int k = j + 1; k < entries.size(); k++) {
                if (entries[i] + entries[j] + entries[k] == 2020) {
                    return entries[i] * entries[j] * entries[k];
                }
            }
        }
    }
}

void solve_day_1() {
    std::ifstream ifs("/home/chrols/projects/advent2020/input");

    std::vector<int> entries;
    int i;

    while (ifs >> i) {
        entries.push_back(i);
    }


    std::cout << solve1(entries) << std::endl;
    std::cout << solve2(entries) << std::endl;

}

void solve_day_2() {
    std::ifstream ifs("/home/chrols/projects/advent2020/input2");
    int min, max;
    char letter, discard;
    std::string password;

    int valid = 0;
    int valid_2 = 0;

    while (ifs >> min >> discard >> max >> letter >> discard >> password) {
        std::cout << min << "\n" << max << "\n" << letter << "\n" << password << std::endl;
        int occur = std::count(password.begin(), password.end(), letter);
        if (occur >= min && occur <= max) {
            valid++;
        }

        bool pos_1 = (password[min-1] == letter);
        bool pos_2 = (password[max-1] == letter);

        if (!pos_1 != !pos_2) {
            valid_2++;
        }
    }

    std::cout << valid << std::endl;
    std::cout << valid_2 << std::endl;

}

int trees_in_path(const std::vector<std::string> &map, std::tuple<int, int> delta) {
    auto [d_right, d_down] = delta;
    int width = map[0].size();
    int height = map.size();

    int x = 0;
    int y = 0;
    int trees = 0;

    while (y < height) {
        if (map[y][x%width] == '#') {
            trees++;
        }
        x += d_right;
        y += d_down;
    }

    return trees;
}

void solve_day_3() {
    std::ifstream ifs("/home/chrols/projects/advent2020/input3");
    std::vector<std::string> map;
    std::string s;

    while (ifs >> s) {
        map.emplace_back(s);
    }

    std::cout << trees_in_path(map, std::make_tuple(3, 1)) << std::endl;

    long product = 1;
    std::vector<std::tuple<int,int>> strats  { {1,1}, {3,1}, {5,1}, {7,1}, {1,2}};
    for (auto &e : strats) {
        int mul = trees_in_path(map, e);
        std::cout << mul << std::endl;
        product *= mul;
    }

    std::cout << product << std::endl;
}

using Passport = std::map<std::string, std::string>;

bool valid_passport(const Passport &passport) {
    const std::vector<std::string> required_fields {
        "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"
    };

    for (auto &e : required_fields) {
        if (passport.find(e) == passport.end()) {
            std::cout << "Missing: " << e << std::endl;
            return false;
        }
    }

    return true;
}


bool valid_hair(const std::string &hair) {
    if (hair.size() > 7) return false;
    if (hair[0] != '#') return false;

    for (int i = 1; i < hair.size(); i++) {
        char c = hair[i];
        if (std::isdigit(c)) continue;
        if (c >= 'a' && c <= 'f') continue;
        return false;
    }

    return true;
}

bool valid_height(const std::string &height) {
    std::istringstream iss(height);
    int num_height;
    std::string unit;
    iss >> num_height >> unit;
    if (unit == "cm") {
        return (num_height >= 150 && num_height <= 193);
    } else if (unit == "in") {
        return (num_height >= 59 && num_height <= 76);
    } else {
        return false;
    }
}

bool valid_eye_color(const std::string &color) {
    const std::vector<std::string> colors {
        "amb", "blu", "brn", "gry", "grn", "hzl", "oth"
    };

    for (auto &e : colors) {
        if (color == e) return true;
    }

    return false;
}

bool extended_valid_passport(const Passport &passport) {
    if (!valid_passport(passport)) return false;

    auto it = passport.find("byr");
    int byr = std::stoi(it->second);
    if (byr < 1920 || byr > 2002) return false;

    it = passport.find("iyr");
    int iyr = std::stoi(it->second);
    if (iyr < 2010 || iyr > 2020) return false;

    it = passport.find("eyr");
    int eyr = std::stoi(it->second);
    if (eyr < 2020 || eyr > 2030) return false;

    it = passport.find("hgt");
    if (!valid_height(it->second)) return false;

    it = passport.find("hcl");
    if (!valid_hair(it->second)) return false;

    it = passport.find("ecl");
    if (!valid_eye_color(it->second)) return false;

    it = passport.find("pid");
    std::string v = it->second;
    if (v.length() != 9 || std::any_of(v.begin(), v.end(), [](char c) { return !std::isdigit(c); })) return false;

    return true;
}

void solve_day_4() {
    std::ifstream ifs("/home/chrols/projects/advent2020/input4");
    std::string s;
    Passport passport;
    int num_valid_passports = 0;
    int num_genuine_passports = 0;

    while (ifs >> s) {
        std::string key = s.substr(0, s.find(':'));
        std::string value = s.substr(s.find(':') + 1, s.length());
        std::cout << key << "=>" << value << std::endl;
        passport[key] = value;
        char c;
        if (ifs.peek() == '\n') {
            ifs.get(c);
            if (ifs.peek() == '\n') {
                if (valid_passport(passport)) {
                    num_valid_passports++;
                }

                if (extended_valid_passport(passport)) {
                    num_genuine_passports++;
                }
                std::cout << "New passport" << std::endl;
                passport = Passport();
            }
        }
    }

    if (valid_passport(passport)) {
        num_valid_passports++;
    }

    if (extended_valid_passport(passport)) {
        num_genuine_passports++;
    }

    std::cout << num_valid_passports << std::endl;
    std::cout << num_genuine_passports << std::endl;
}

int handle_pass(const std::string &pass) {
    int row_min = 0;
    int row_max = 127;
    int col_min = 0;
    int col_max = 7;

    for (char c : pass) {
        int row_range = row_max - row_min + 1;
        int col_range = col_max - col_min + 1;
        switch (c) {
            case 'F':
                row_max -= row_range / 2;
                break;
            case 'B':
                row_min += row_range / 2;
                break;
            case 'L':
                col_max -= col_range / 2;
                break;
            case 'R':
                col_min += col_range / 2;
                break;
        }
    }

    std::cout << "Row: " << row_max << std::endl;
    std::cout << "Col: " << col_max << std::endl;
    std::cout << "ID: " << row_max * 8 + col_max << std::endl;
    return row_max * 8 + col_max;

}

void solve_day_5() {
    std::ifstream ifs("/home/chrols/projects/advent2020/input5");
    std::string s;
    int max = 0;

    std::vector<int> seats;

    while (ifs >> s) {
        int id = handle_pass(s);
        max = std::max(max, id);
        seats.push_back(id);
   }

    std::sort(seats.begin(), seats.end());

    auto it = seats.begin();
    auto st = seats.begin();
    st++;

    while (*st++ == *it++ + 1);

    std::cout << max << std::endl;
    std::cout << *(--it) + 1 << std::endl;
}

void solve_day_6() {
    std::ifstream ifs("/home/chrols/projects/advent2020/input6");
    std::string s;
    std::map<char, int> answers;
    int num_answers = 0;
    int num_anyone = 0;
    int num_everyone = 0;

    while (ifs >> s) {
        for (auto &e : s) {
            answers[e]++;
        }

        num_answers++;

        if (ifs.peek() == '\n') {
            ifs.get();
            if (ifs.peek() == '\n') {
                std::cout << answers.size() << std::endl;
                num_anyone += answers.size();

                for (auto &e : answers) {
                    if (e.second == num_answers) {
                        num_everyone++;
                    }
                }

                answers.clear();
                num_answers = 0;
            } else {
                ifs.unget();
            }
        }
    }

    num_anyone += answers.size();

    for (auto &e : answers) {
        if (e.second == num_answers) {
            num_everyone++;
        }
    }

    std::cout << num_anyone << std::endl;
    std::cout << num_everyone << std::endl;
}

int main() {
    solve_day_6();
    return 0;

}

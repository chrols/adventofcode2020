#include <iostream>
#include <fstream>
#include <vector>

enum struct Op {
    Acc,
    Nop,
    Jmp
};

struct Ins {
    Op op;
    int arg;
};

std::pair<int, bool> execute(const std::vector<Ins> &mem) {
    int pc = 0;
    int acc = 0;
    std::vector<bool> executed_once ( mem.size(), false );

    while (!executed_once[pc] && pc < mem.size() ) {
        executed_once[pc] = true;
        auto i = mem[pc];

        switch (i.op) {
        case Op::Acc:
          acc += i.arg;
          break;
        case Op::Jmp:
          pc += (i.arg - 1);
          break;
        }

        pc++;
    }

    return std::make_pair(acc, pc == mem.size());
}

int main(int argc, char **argv) {
    std::ifstream ifs("input8");
    std::vector<Ins> mem;

    Ins ins;
    std::string temp;

    while (ifs >> temp >> ins.arg) {
        if (temp == "acc") {
            ins.op = Op::Acc;
        } else if (temp == "jmp") {
            ins.op = Op::Jmp;
        } else if (temp == "nop") {
            ins.op = Op::Nop;
        }

        mem.push_back(ins);
    }

    auto res = execute(mem);

    std::cout << res.first << std::endl;

    for (auto &e : mem) {
        auto old = e;

      if (e.op == Op::Jmp) {
          e.op = Op::Nop;
      } else if (e.op == Op::Nop) {
          e.op = Op::Jmp;
      }

      auto result = execute(mem);

      if (result.second) {
          std::cout << result.first << std::endl;
          return 0;
      } else {
          e = old;
      }
    }

    std::cout << "OH NOEZ!" << std::endl;
}

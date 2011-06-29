#include <iostream>
#include <string>

#include "cbt/btree.h"

int main() {
    cbt::btree<int, std::string> b;

    b.insert(1, "B");
    b.insert(2, "A");
    const std::string& ret = b.find(1);

    std::cout << "1: " << ret << std::endl;
    std::cout << "2: " << b.find(2) << std::endl;

    return 0;
}

#include <iostream>
#include <string>

#include "cbt/btree.h"

int main() {
    cbt::btree<int, std::string> b;

    b.insert(1, "E");
    b.insert(2, "D");
    b.insert(3, "C");
    b.insert(4, "B");
    b.insert(5, "A");

    cbt::btree<int, std::string>::iterator it = b.find(3);

    std::cout << "key: " << it->first << ", value: " << it->second << std::endl;

    return 0;
}

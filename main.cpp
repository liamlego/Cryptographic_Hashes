#include <iostream>
#include "HashTable.h"
#include "sha_hash.h"

using namespace std;


int main() {


    SHA_Hash hasher;

    std::cout << hasher.sha1_hash("orange juice") << std::endl;

    return 0;
}
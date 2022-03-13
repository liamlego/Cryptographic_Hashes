#include <iostream>
#include "sha_hash.h"

using namespace std;


int main() {


    SHA_Hash hasher;

    std::cout << "Performs same hash for same input: " << std::endl;
    std::cout << hasher.sha1_hash("The quick brown fox jumps over the lazy dog") << std::endl;

    std::cout << hasher.sha1_hash("The quick brown fox jumps over the lazy dog") << std::endl;

    std::cout << "Performs entirely different hash for slightly different input: " << std::endl;
    std::cout << hasher.sha1_hash("The quick brown fox jumps over the lazy dog") << std::endl;
    std::cout << hasher.sha1_hash("The quick brown fox jumps over the lazy cog") << std::endl;

    return 0;
}
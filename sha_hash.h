#ifndef SHA_HASH_H
#define SHA_HASH_H

#include <memory>
#include <string>

class SHA_Hash {

public:

    SHA_Hash();
    ~SHA_Hash();

    std::string sha1_hash(std::string s);
    unsigned int sha256_hash(std::string s);

private:



};





#endif
#include "sha_hash.h"
#include <bitset>
#include <vector>
#include <sstream>
#include <iostream>

/*

References for SHA-1:
    Pseudocode use from: https://en.wikipedia.org/wiki/SHA-1 

    Example Javascript code and explanation from: FullStack Academy's https://www.youtube.com/watch?v=kmHojGMUn0Q 
    
References for SHA-256:


*/

SHA_Hash::SHA_Hash() {

}

SHA_Hash::~SHA_Hash() {


}

int findMSB(unsigned int num) {

    int size = 32;

    int spot = 31;

    while (spot >= 0) {

        if (((num >> spot) & 0x01) == 1) {
            return spot;
        }

        spot--;
    }
    return 0;

}

// returns a truncated version of str
unsigned int strToBinary(std::string str) {

    unsigned int num = 0;

    int index = 31;
    for (char c : str) {

        if (c == '1') {

            int bit = 0x01;
            bit = bit << index;

            num = num | bit;
        }

        index--;
    }

    return num;
}

unsigned int leftRotate(unsigned int n, int times) {

    unsigned int num = 0;

    unsigned int temp = (n >> 31) & 0x01;

    int left = 31;

    for (int i = 31; i-times >= 0; i--) {
        unsigned int bit = (n >> (i-times)) & 0x01;
        bit = bit << i;
        

        num = num | bit;

        left--;
    }

    int index = 31;
    for (int i = left; i >= 0; i--) {
        unsigned int bit = (n >> (index)) & 0x01;
        bit = bit << i;
        num = num | bit;
        index--;
    }

    return num;
}

// returns truncated addition
unsigned int binaryAddition(std::string num1, std::string num2) {


    int smallerLength = num1.length() < num2.length() ? num1.length() : num2.length();



    char carry = '0';
    int count = 0;

    std::string newNum = "";

    while (count < smallerLength) {

        if (num1[count] == '1' && num2[count] == '1' && carry == '1') {
            newNum.insert(0, "1");
            carry = '1';
        } else if ((num1[count] == '1' && num2[count] == '1' && carry == '0') ||
                   (num1[count] == '1' && carry == '1') ||
                    (num2[count] == '1' && carry == '1')) {
            newNum.insert(0, "0");
            carry = '1';
        } else if (num1[count] == '0' && num2[count] == '0' && carry == '0'){

            newNum.insert(0, "0");
        } else {
            newNum.insert(0, "1");
            carry = '0';
        }

        count++;
    }

    return strToBinary(newNum);
}

// returns the hexadecimal hash of the message
std::string SHA_Hash::sha1_hash(std::string str) {


    unsigned int h0 = 0x67452301;
    unsigned int h1 = 0xEFCDAB89;
    unsigned int h2 = 0x98BADCFE;
    unsigned int h3 = 0x10325476;
    unsigned int h4 = 0xC3D2E1F0;

    std::string strInBinary;

    for (char c : str) {
        for (int i = 7; i >= 0; i--) {

            char bit = 0x01;
            bit = bit << i;

            if (((c & bit) >> i) == 0x01) {
                strInBinary.append("1");
            } else {
                strInBinary.append("0");
            }

        }
    }

    strInBinary.append("1");

    unsigned int numBits = (str.length()*(sizeof(char)*8));
    
    while ((strInBinary.length() % 512) != 448) {
        strInBinary.append("0");
    }

    unsigned int length = str.length();

    std::string lengthBinary;
    for (int i = 31; i >= 0; i--) {

        unsigned int bit = 0x01;
        bit = bit << i;

        if (((length & bit) >> i) == 0x01) {
            lengthBinary.append("1");
        } else {
            lengthBinary.append("0");
        }

    }


    while (lengthBinary.length() != 64) {
        lengthBinary.insert(0, "0");
    }


    strInBinary.append(lengthBinary);

    int count = 0;

    std::vector<std::string> arr;

    while (count < strInBinary.length()) {
        arr.push_back(strInBinary.substr(count, 512));
        count += 512;
    }

    std::vector<std::vector<unsigned int>> splitted;

    for (int i = 0; i < arr.size(); i++) {

        std::vector<unsigned int> subVect;
        std::string s = arr[i];

        int c1 = 0;

        while (c1 < s.length()) {
            
            unsigned int num = strToBinary(s.substr(c1, 32));
            //std::cout << std::bitset<32> {num}.to_string() << std::endl;
            subVect.push_back(num);

            c1 += 32;
        }

        splitted.push_back(subVect);
    }

    for (int i = 0; i < splitted.size(); i++) {

        

        for (int j = 16; j <= 79; j++) {
            

            unsigned int numA = splitted[i][j - 3];
            unsigned int numB = splitted[i][j - 8];
            unsigned int numC = splitted[i][j - 14];
            unsigned int numD = splitted[i][j - 16];

            unsigned int xorA = numA ^ numB;
            unsigned int xorB = xorA ^ numC;
            unsigned int xorC = xorB ^ numD;

            unsigned int newNum = leftRotate(xorC, 1);

            splitted[i].push_back(newNum);
        }
    }

    unsigned int a = h0;
    unsigned int b = h1;
    unsigned int c = h2;
    unsigned int d = h3;
    unsigned int e = h4;

    for (int i = 0; i < splitted.size(); i++) {

        for (int j = 0; j < 80; j++) {

            unsigned int f = 0;
            unsigned int k = 0;

            if (j >= 0 && j <= 19) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            } else if (i >= 20 && i <= 39) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            } else if (i >= 40 && i <= 59) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else if (i >= 60 && i <= 79) {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            unsigned int word = splitted[i][j];

            //std::cout << word << std::endl;
            
            unsigned int temp = ((((((leftRotate(a, 5) + f)%UINT32_MAX) + e)%UINT32_MAX + k)%UINT32_MAX) + word)%UINT32_MAX;

            e = d;
            d = c;
            c = leftRotate(b, 30);
            b = a;
            a = temp;
        }

        h0 = (h0 + a)%UINT32_MAX;
        h1 = (h1 + b)%UINT32_MAX;
        h2 = (h2 + c)%UINT32_MAX;
        h3 = (h3 + d)%UINT32_MAX;
        h4 = (h4 + e)%UINT32_MAX;

    }

    std::stringstream hh;

    hh << std::hex << std::bitset<32> {h0}.to_ulong();
    hh << std::hex << std::bitset<32> {h1}.to_ulong();
    hh << std::hex << std::bitset<32> {h2}.to_ulong();
    hh << std::hex << std::bitset<32> {h3}.to_ulong();
    hh << std::hex << std::bitset<32> {h4}.to_ulong();

    hh << std::hex;

    return hh.str();
}


unsigned int SHA_Hash::sha256_hash(std::string str) {

    return 0;
}


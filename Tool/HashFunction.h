//
// Created by JaneZ on 2025/5/6.
//

#ifndef HASHFUNCTION_H
#define HASHFUNCTION_H
#include <string>

using ull = unsigned long long int;
ull HashFunction(const std::string &s) {
    unsigned long long hash = 0;
    unsigned long long prime = 31;
    for (size_t i = 0; i < s.size(); ++i) {
        hash = hash * prime + s[i];
    }
    return hash;
}


#endif //HASHFUNCTION_H

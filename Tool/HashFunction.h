//
// Created by JaneZ on 2025/5/6.
//

#ifndef HASHFUNCTION_H
#define HASHFUNCTION_H
#include "../Data Structure/String.h"

using ull = unsigned long long int;

namespace JaneZ {
    template<size_t length>
    class Hash {
    public:
        static ull HashFunction(const JaneZ::String<length> &s) {
            unsigned long long hash = 0;
            unsigned long long prime = 31;
            for (size_t i = 0; i < length; ++i) {
                hash = hash * prime + s[i];
            }
            return hash;
        }
    };
}

#endif //HASHFUNCTION_H

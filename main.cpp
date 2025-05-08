#include <iostream>
#include <string>
#include "Tool/tokenSlicer.h"

int main(){
    std::string op;
    while (true) {
        std::getline(std::cin,op);
        TokenSlicer knife(op);
        JaneZ::TotalInfo tmp;
        knife.SliceTokens(op,tmp);

    }
    return 0;
}

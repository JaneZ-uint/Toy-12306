//
// Created by JaneZ on 2025/5/5.
//

#include "tokenscanner.h"
bool TokenScanner::checkLetter(char a) {
    if('A' <= a && a <= 'Z') {
        return true;
    }else if('a' <= a && a <= 'z') {
        return true;
    }
    return false;
}

bool TokenScanner::checkNumber(char a) {
    if('0' <= a && a <= '9') {
        return true;
    }
    return false;
}

bool TokenScanner::checkDownLine(char a) {
    if(a == '_') {
        return true;
    }
    return false;
}

bool TokenScanner::checkSee(char a) {
    if(0 <= a && a <= 31) {
        return false;
    }
    if(a == 127) {
        return false;
    }
    return true;
}


bool TokenScanner::checkUsername(const std::string &s) {
    if(s.size() > 20) {
        return false;
    }
    if(checkLetter(s[0])) {
        for(int i = 1;i < s.size();i ++) {
            if(checkLetter(s[i]) || checkNumber(s[i]) || checkDownLine(s[i])) {
                continue;
            }
            return false;
        }
    }else {
        return false;
    }
    return true;
}

bool TokenScanner::checkPassword(const std::string &s) {
    if(s.size() == 0 || s.size() > 30) {
        return false;
    }
    for(int i = 0;i < s.size();i ++) {
        if(checkSee(s[i])) {
            continue;
        }
        return false;
    }
    return true;
}

bool TokenScanner::checkName(const std::string &s) {
    if(s.size() < 6 || s.size() > 20) {
        return false;
    }
    return true;
}

bool TokenScanner::checkPrivilege(int pr) {
    if(pr >= 0 && pr <= 10) {
        return true;
    }
    return false;
}

bool TokenScanner::checkTrainID(const std::string &s) {
    if(s.size() > 20) {
        return false;
    }
    if(checkLetter(s[0])) {
        for(int i = 1;i < s.size();i ++) {
            if(checkLetter(s[i]) || checkNumber(s[i]) || checkDownLine(s[i])) {
                continue;
            }
            return false;
        }
    }else {
        return false;
    }
    return true;
}

bool TokenScanner::checkStationNum(int stn) {
    if(stn < 2 || stn > 100) {
        return false;
    }
    return true;
}

bool TokenScanner::checkStationsName(const std::string &s) {
    if(s.size() > 40) {
        return false;
    }
    return true;
}


bool TokenScanner::checkStations(sjtu::vector<std::string> &stations) {
    for(int i = 0;i < stations.size();i ++) {
        if(checkStationsName(stations[i])) {
            continue;
        }
        return false;
    }
    return true;
}

bool TokenScanner::checkSeatNum(int sn) {
    if(sn > 100000 || sn < 0) {
        return false;
    }
    return true;
}

bool TokenScanner::checkPrices(sjtu::vector<int> &prices) {
    for(int i = 0;i < prices.size();i ++) {
        if(prices[i] <= 100000 && prices[i] >= 0) {
            continue;
        }
        return false;
    }
    return true;
}

bool TokenScanner::checkStartTime(const std::string &s) {
    if(s.size() != 5) {
        return false;
    }
    if(s[2] != ':') {
        return false;
    }
    if(!checkNumber(s[0])) {
        return false;
    }
    if(!checkNumber(s[1])) {
        return false;
    }
    if(!checkNumber(s[3])) {
        return false;
    }
    if(!checkNumber(s[4])) {
        return false;
    }
    return true;
}

bool TokenScanner::checkTravelTimes(sjtu::vector<int> &travelTimes) {
    for(int i = 0;i < travelTimes.size();i ++) {
        if(travelTimes[i] >= 0 && travelTimes[i] <= 10000) {
            continue;
        }
        return false;
    }
    return true;
}

bool TokenScanner::checkStopoverTime(sjtu::vector<int> &stopoverTimes) {
    for(int i = 0;i < stopoverTimes.size();i ++) {
        if(stopoverTimes[i] >= 0 && stopoverTimes[i] <= 10000) {
            continue;
        }
        return false;
    }
    return true;
}

bool TokenScanner::checkSaleDate(const std::string &s) {
    if(s.size() != 5) {
        return false;
    }
    if(s[2] != '-') {
        return false;
    }
    if(!checkNumber(s[0])) {
        return false;
    }
    if(!checkNumber(s[1])) {
        return false;
    }
    if(!checkNumber(s[3])) {
        return false;
    }
    if(!checkNumber(s[4])) {
        return false;
    }
    return true;
}

bool TokenScanner::checkType(char tp) {
    if(tp >= 'A' && tp <= 'Z') {
        return true;
    }
    return false;
}

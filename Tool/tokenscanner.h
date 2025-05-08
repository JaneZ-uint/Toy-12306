//
// Created by JaneZ on 2025/5/5.
//

#ifndef TOKENSCANNER_H
#define TOKENSCANNER_H
#include "../Data Structure/String.h"
#include "../Data Structure/vector.h"

/*
 * This is a class that will never be used.
 * Silly JaneZ found out that "保证所有指令输入格式均合法" after she completely finished it.
 * But apparently she doesn't want to delete it.
 * So here is the graveyard of class TokenScanner.
 * Let's say :
 * Great TokenScanner!
 */

class TokenScanner {
public:
    static bool checkLetter(char a);

    static bool checkNumber(char a);

    static bool checkDownLine(char a);

    static bool checkSee(char a);

    static bool checkUsername(const std::string &s);

    static bool checkPassword(const std::string &s);

    static bool checkName(const std::string &s);

    static bool checkPrivilege(int pr);

    static bool checkTrainID(const std::string &s);

    static bool checkStationNum(int stn);

    static bool checkStationsName(const std::string &s);

    static bool checkStations(sjtu::vector<std::string> &stations);

    static bool checkSeatNum(int sn);

    static bool checkPrices(sjtu::vector<int> &prices);

    static bool checkStartTime(const std::string &s);

    static bool checkTravelTimes(sjtu::vector<int> &travelTimes);

    static bool checkStopoverTime(sjtu::vector<int> &stopoverTimes);

    static bool checkSaleDate(const std::string &s);

    static bool checkType(char tp);
};

#endif //TOKENSCANNER_H

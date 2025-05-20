//
// Created by JaneZ on 2025/5/8.
//

#ifndef TOKENSLICER_H
#define TOKENSLICER_H
#include <string>
#include "HashFunction.h"
#include "../Data Structure/String.h"
#include "../Data Structure/vector.h"
#include "../Management/time.h"

namespace JaneZ {
    enum opType {
        add_user,
        login,
        logout,
        query_profile,
        modify_profile,
        add_train,
        delete_train,
        release_train,
        query_train,
        query_ticket,
        query_transfer,
        buy_ticket,
        query_order,
        refund_ticket,
        clean,
        exit
    };

    enum SortType {
        time,
        cost
    };

    struct TotalInfo {
        std::string op;
        JaneZ::opType a;
        std::string cur_username;
        std::string username;
        std::string password;
        std::string name;
        std::string mail;
        int privilege;
        std::string ID;
        int StationNum;
        int SeatNum;
        sjtu::vector<std::string> Stations;
        sjtu::vector<int> prices;
        JaneZ::Clock startTime;
        sjtu::vector<int> travelTimes;
        sjtu::vector<int> stopoverTimes;
        Date saleBeginDate;
        Date saleEndDate;
        Date BeginDate;
        Date SpecificStationDate;
        char Type;
        int buyNumber;
        int nOrder;
        std::string fromStation;
        std::string toStation;
        bool waitList;
        SortType SortWay;
        int timeStamp;
    };
}

class TokenSlicer {
private:
    std::string op;
public:
    TokenSlicer(std::string &s);

    int StringToInteger(std::string &s);

    JaneZ::Date StringToDate(std::string &s);

    sjtu::vector<std::string> StringToVectorString(std::string &s);

    sjtu::vector<int> StringToVectorInt(std::string &s);

    JaneZ::Clock StringToClock(std::string &s);

    void SliceTokens(std::string &s,JaneZ::TotalInfo &info);
};

#endif //TOKENSLICER_H

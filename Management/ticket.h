//
// Created by JaneZ on 2025/5/5.
//

#ifndef TICKET_H
#define TICKET_H
#include "train.h"
#include "user.h"
#include "../Data Structure/BPT.h"
#include "../Management/time.h"
#include "../Data Structure/String.h"
#include "../Tool/HashFunction.h"

using ull = unsigned long long int;

enum TicketPurchase {
    Succeed, Pending, Refunded
};

struct TicketInfo {
    JaneZ::String<22> username;
    JaneZ::String<22> trainID;
    JaneZ::TrainTime leaveTime;
    JaneZ::String<42> startStation;
    JaneZ::String<42> endStation;
    bool waitList = false;
    int num = 0;
    int timeCost = 0;
    int ticketPrice;
    int timeStamp;//维护时间戳
    TicketPurchase status;

    TicketInfo():num(0),timeCost(0){}

    TicketInfo(const TicketInfo &other) = default;

    bool operator==(const TicketInfo &other) const {
        return timeStamp == other.timeStamp;
    }

    bool operator<(const TicketInfo &other) const {
        return timeStamp < other.timeStamp;
    }

    bool operator<=(const TicketInfo &other) const {
        return timeStamp <= other.timeStamp;
    }

    bool operator>(const TicketInfo &other) const {
        return timeStamp > other.timeStamp;
    }

    bool operator>=(const TicketInfo &other) const {
        return timeCost >= other.timeCost;
    }
};

struct WaitTicket {
    JaneZ::String<22> trainID;
    JaneZ::Date StartDay;
};

class TicketSystem {
    friend class TrainSystem;
    friend class UserSystem;
private:
    BPT<ull, TicketInfo> FormalList;//所有订单（包括候补在内）
    BPT<WaitTicket, TicketInfo> WaitList;//候补订单

public:
    TicketSystem():FormalList("FormalListIndex","FormalListLeaf"),WaitList("WaitListIndex","WaitListLeaf"){}

    ~TicketSystem() = default;

    void buy_ticket(JaneZ::String<22> &username,
                        JaneZ::String<22> &trainID,
                        JaneZ::Date &day,
                        int num,
                        JaneZ::String<42> &startStation,
                        JaneZ::String<42> &endStation,
                        bool waitList = false,
                        TrainSystem &train_system,
                        UserSystem &user_system,
                        int timeStamp);

    void query_order(JaneZ::String<22> &username,UserSystem &user_system,TrainSystem &train_system);

    bool refund_ticket(JaneZ::String<22> &username,int n = 1);

    static void clean() {
        // 删除正式列表的索引和叶子文件
        std::remove("FormalListIndex");
        std::remove("FormalListLeaf");

        // 删除等待列表的索引和叶子文件
        std::remove("WaitListIndex");
        std::remove("WaitListLeaf");
    }
};

#endif //TICKET_H

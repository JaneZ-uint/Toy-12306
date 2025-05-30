//
// Created by JaneZ on 2025/5/5.
//

#ifndef TICKET_H
#define TICKET_H
#include "train.h"
#include "user.h"
#include "../include/BPT.h"
#include "../include/time.h"
#include "String.h"
#include "../include/HashFunction.h"

using ull = unsigned long long int;

enum TicketPurchase {
    Succeed, Pending, Refunded
};

struct TicketInfo {
    JaneZ::String<22> username;
    JaneZ::String<22> trainID;
    JaneZ::TrainTime leaveTime; //从当前站出发的时间
    JaneZ::String<42> startStation;
    int stIndex;
    JaneZ::String<42> endStation;
    int enIndex;
    bool waitList = false;
    int num = 0;
    int timeCost = 0;
    int ticketPrice;
    int timeStamp; //维护时间戳
    TicketPurchase status;

    TicketInfo(): num(0), timeCost(0) {
    }

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

    bool operator==(const WaitTicket &other) const {
        return trainID == other.trainID && StartDay == other.StartDay;
    }

    bool operator<(const WaitTicket &other) const {
        if (trainID < other.trainID) {
            return true;
        }
        if (trainID == other.trainID) {
            return StartDay < other.StartDay;
        }
        return false;
    }

    bool operator>(const WaitTicket &other) const {
        if (trainID > other.trainID) {
            return true;
        }
        if (trainID == other.trainID) {
            return StartDay > other.StartDay;
        }
        return false;
    }
};

class TicketSystem {
    friend class TrainSystem;
    friend class UserSystem;

private:
    BPT<ull, TicketInfo> FormalList; //所有订单（包括候补在内）
    BPT<WaitTicket, TicketInfo> WaitList; //候补订单

public:
    TicketSystem(): FormalList("FormalListIndex", "FormalListLeaf"), WaitList("WaitListIndex", "WaitListLeaf") {
    }

    ~TicketSystem() = default;

    void buy_ticket(JaneZ::String<22> &username,
                    JaneZ::String<22> &trainID,
                    JaneZ::Date &day,
                    int num,
                    JaneZ::String<42> &startStation,
                    JaneZ::String<42> &endStation,
                    bool waitList,
                    TrainSystem &train_system,
                    UserSystem &user_system,
                    int timeStamp);

    void query_order(JaneZ::String<22> &username, UserSystem &user_system, TrainSystem &train_system);

    bool refund_ticket(JaneZ::String<22> &username, int n, UserSystem &user_system, TrainSystem &train_system);

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

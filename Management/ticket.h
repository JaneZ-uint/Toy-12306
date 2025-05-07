//
// Created by JaneZ on 2025/5/5.
//

#ifndef TICKET_H
#define TICKET_H
#include "../Data Structure/BPT.h"
#include "../Management/time.h"
#include "../Data Structure/String.h"
#include "../Tool/HashFunction.h"

using ull = unsigned long long int;

enum TicketPurchase {
    Succeed, Queue, Fail
};

struct TicketInfo {
    JaneZ::String<22> username;
    JaneZ::String<22> trainID;
    JaneZ::Date day;
    JaneZ::String<42> startStation;
    JaneZ::String<42> endStation;
    bool waitList = false;
    int num = 0;
    int timeCost = 0;

    TicketInfo(const TicketInfo &other) = default;

    bool operator==(const TicketInfo &other) const {
        return timeCost == other.timeCost;
    }

    bool operator<(const TicketInfo &other) const {
        return timeCost < other.timeCost;
    }

    bool operator<=(const TicketInfo &other) const {
        return timeCost <= other.timeCost;
    }

    bool operator>(const TicketInfo &other) const {
        return timeCost > other.timeCost;
    }

    bool operator>=(const TicketInfo &other) const {
        return timeCost >= other.timeCost;
    }
};

struct BuyState {
    TicketPurchase a;
    int totalPrice;
};

class TicketSystem {
private:
    BPT<ull, TicketInfo> FormalList;
    BPT<ull, TicketInfo> WaitList;

public:
    BuyState buy_ticket(JaneZ::String<22> &username,
                        JaneZ::String<22> &trainID,
                        JaneZ::Date &day,
                        int num = 0,
                        JaneZ::String<42> &startStation,
                        JaneZ::String<42> &endStation,
                        bool waitList = false);

    void query_order(JaneZ::String<22> &username);

    bool refund_ticket(JaneZ::String<22> &username,int n = 1);
};

#endif //TICKET_H

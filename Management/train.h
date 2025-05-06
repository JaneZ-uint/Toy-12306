//
// Created by JaneZ on 2025/5/5.
//

#ifndef TRAIN_H
#define TRAIN_H
#include "../Data Structure/BPT.h"
#include "../Data Structure/String.h"
#include "../Data Structure/vector.h"
#include "../Management/time.h"

using ull = unsigned long long int;

struct TrainInfo {
    JaneZ::String<22> trainID;
    int stationNum;
    sjtu::vector<JaneZ::String<42>> stations;
    int seatNum;
    sjtu::vector<int> prices;
    JaneZ::Clock startTime;
    sjtu::vector<int> travelTimes;
    sjtu::vector<int> stopoverTimes;
    JaneZ::Date saleStartDate;
    JaneZ::Date saleEndDate;
    char type;
    bool is_released = false;

    TrainInfo(const TrainInfo& other) = default;

    bool operator==(const TrainInfo &other) const {
        return trainID == other.trainID;
    }

    bool operator<(const TrainInfo &other) const {
        return trainID < other.trainID;
    }

    bool operator<=(const TrainInfo &other) const {
        return trainID <= other.trainID;
    }

    bool operator>(const TrainInfo &other) const {
        return trainID > other.trainID;
    }

    bool operator>=(const TrainInfo &other) const {
        return trainID >= other.trainID;
    }
};

class TrainSystem {
private:
    BPT<ull,TrainInfo> TrainBase;

public:
    bool add_train(JaneZ::String<22> &trainID,
    int stationNum,
    sjtu::vector<JaneZ::String<42>> &stations,
    int seatNum,
    sjtu::vector<int> &prices,
    JaneZ::Clock &startTime,
    sjtu::vector<int> &travelTimes,
    sjtu::vector<int> &stopoverTimes,
    JaneZ::Date &saleStartDate,
    JaneZ::Date &saleEndDate,
    char type);

    bool delete_train(JaneZ::String<22> &trainID);

    bool release_train(JaneZ::String<22> &trainID);

    //TODO
};

#endif //TRAIN_H

//
// Created by JaneZ on 2025/5/5.
//

#include <iostream>
#include "train.h"
#include "../Tool/HashFunction.h"
using ull = unsigned long long int;

bool TrainSystem::add_train(JaneZ::String<22> &trainID, int stationNum, int seatNum, sjtu::vector<JaneZ::String<42> > &stations, sjtu::vector<int> &prices, JaneZ::Clock &startTime, sjtu::vector<int> &travelTimes, sjtu::vector<int> &stopoverTimes, JaneZ::Date &saleStartDate, JaneZ::Date &saleEndDate, char type) {
    ull HashID = JaneZ::Hash<22>::HashFunction(trainID);
    sjtu::vector<int> posTmp = TrainBase.find(HashID);
    if(!posTmp.empty()) {
        return false;
    }
    TrainInfo current;
    current.trainID = trainID;
    current.stationNum = stationNum;
    current.seatNum = seatNum;
    for(int i = 0;i < stationNum;i ++) {
        current.stations[i].stationName = stations[i];
        current.stations[i].stationIndex = i;
        if(i == 0) {
            current.stations[i].price = 0;
            current.stations[i].travelTime = 0;
            current.stations[i].stopoverTime = 0;
        }else if(i == stationNum - 1) {
            current.stations[i].price = current.stations[i - 1].price + prices[i - 1];
            current.stations[i].travelTime = current.stations[i - 1].travelTime + current.stations[i - 1].stopoverTime +travelTimes[i - 1];
            current.stations[i].stopoverTime = 0;
        }else {
            current.stations[i].price = current.stations[i - 1].price + prices[i - 1];
            current.stations[i].travelTime = current.stations[i - 1].travelTime + current.stations[i - 1].stopoverTime +travelTimes[i - 1];
            current.stations[i].stopoverTime = stopoverTimes[i - 1];
        }
    }
    current.startTime = startTime;
    current.saleStartDate = saleStartDate;
    current.saleEndDate = saleEndDate;
    current.type = type;
    current.is_released = false;
    TrainFile.write(current,total);
    TrainBase.insert(HashID,total);
    ++ total;
    return true;
}

bool TrainSystem::delete_train(JaneZ::String<22> &trainID) {
    ull HashID = JaneZ::Hash<22>::HashFunction(trainID);
    sjtu::vector<int> posTmp = TrainBase.find(HashID);
    if(posTmp.empty()) {
        return false;
    }
    TrainInfo current;
    TrainFile.read(current,posTmp[0]);
    if(current.is_released) {
        return false;
    }
    TrainBase.erase(HashID,posTmp[0]);
    return true;
}

bool TrainSystem::release_train(JaneZ::String<22> &trainID) {
    ull HashID = JaneZ::Hash<22>::HashFunction(trainID);
    sjtu::vector<int> posTmp = TrainBase.find(HashID);
    if(posTmp.empty()) {
        return false;
    }
    TrainInfo current;
    TrainFile.read(current,posTmp[0]);
    if(current.is_released) {
        return false;  //Wait for TA
    }
    current.is_released = true;
    TrainFile.write(current,posTmp[0]);
    for(int i = 0;i < current.stationNum;i ++) {
        ull HashName = JaneZ::Hash<42>::HashFunction(current.stations[i].stationName);
        StationValue now;
        now.stationName = current.stations[i].stationName;
        now.trainID = trainID;
        now.fileIndex = posTmp[0];
        now.nStation = i;
        //TODO 根据需要可能还要增加一些信息
        ReleasedTrainBase.insert(HashName,now);
    }
    return true;
}

bool TrainSystem::query_train(JaneZ::String<22> &trainID, JaneZ::Date &date) {
    ull HashID = JaneZ::Hash<22>::HashFunction(trainID);
    sjtu::vector<int> posTmp = TrainBase.find(HashID);
    if(posTmp.empty()) {
        return false;
    }
    TrainInfo current;
    TrainFile.read(current,posTmp[0]);
    current.trainID = trainID;
    JaneZ::TrainTime BeginTIme;
    BeginTIme.date = date;
    BeginTIme.clock = current.startTime;
    if(date > current.saleEndDate) {
        return false;
    }
    int nDay = date - current.saleStartDate;//0 base
    Seats currentSeat;
    SeatFile.read(currentSeat,MaxDays * posTmp[0] + nDay);
    std::cout << current.trainID << " " << current.type << '\n';
    int MaxSeatNum = current.seatNum;
    for(int i = 0;i < current.stationNum;i ++) {
        std::cout << current.stations[i].stationName << " ";
        if(i == 0) {
            std::cout << "xx-xx xx:xx -> ";
            std::cout << BeginTIme << " ";
        }else if(i == current.stationNum - 1) {
            JaneZ::TrainTime currentTime = BeginTIme + current.stations[i].travelTime;
            std::cout << currentTime << " ";
            std::cout << "->" << " ";
            std::cout << "xx-xx xx:xx ";
        }else {
            JaneZ::TrainTime currentTime = BeginTIme + current.stations[i].travelTime;
            std::cout << currentTime << " ";
            std::cout << "->" << " ";
            JaneZ::TrainTime leavingTime = currentTime + current.stations[i].stopoverTime;
            std::cout << leavingTime << " ";
        }
        std::cout << current.stations[i].price << " ";
        if(current.is_released) {
            if(i != current.stationNum - 1) {
                MaxSeatNum += currentSeat.DeltaSeatNum[i];
                std::cout << MaxSeatNum;
            }else {
                std::cout << "x";
            }
        }else {
            if(i != current.stationNum - 1) {
                std::cout << current.seatNum;
            }else {
                std::cout << "x";
            }
        }
        std::cout << '\n';
    }
    return true;
}

void TrainSystem::query_ticket(JaneZ::String<42> &s, JaneZ::String<42> &t, JaneZ::Date &d, JaneZ::SortType SortWay) {
    ull HashStartStation = JaneZ::Hash<42>::HashFunction(s);
    ull HashToStation = JaneZ::Hash<42>::HashFunction(t);
    sjtu::vector<StationValue> Start = ReleasedTrainBase.find(HashStartStation);
    sjtu::vector<StationValue> To = ReleasedTrainBase.find(HashToStation);
    size_t startTotalNum = Start.size();
    size_t toTotalNum = To.size();
    size_t st = 0;
    size_t to = 0;
    //TODO
    while(st < startTotalNum && to < toTotalNum) {
        if(Start[st] < To[st]) {
            st ++;
            continue;
        }
        if(Start[st] > To[st]) {
            to ++;
            continue;
        }
        if(Start[st].nStation >= To[st].nStation) {
            st ++;
            to ++;
            continue;
        }
        //TODO
    }
}

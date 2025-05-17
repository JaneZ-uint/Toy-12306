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
    ReleasedTrainBase.insert(HashID,posTmp[0]);
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
        //TODO Wait to deal with the seats.
    }
}

//
// Created by JaneZ on 2025/5/5.
//

#include "../include/ticket.h"

#include "../include/train.h"
#include "../include/user.h"
#include "../include/tokenSlicer.h"

using ull = unsigned long long int;

void TicketSystem::buy_ticket(JaneZ::String<22> &username, JaneZ::String<22> &trainID, JaneZ::Date &day, int num,
                              JaneZ::String<42> &startStation, JaneZ::String<42> &endStation, bool waitList,
                              TrainSystem &train_system, UserSystem &user_system, int timeStamp) {
    ull HashUsername = JaneZ::Hash<22>::HashFunction(username);
    if (user_system.LoginStack.find(HashUsername) == user_system.LoginStack.end()) {
        std::cout << -1 << '\n';
        return;
    }
    ull HashTrainID = JaneZ::Hash<22>::HashFunction(trainID);
    TrainInfo currentTrain;
    sjtu::vector<int> posTmp = train_system.TrainBase.find(HashTrainID);
    if (posTmp.empty()) {
        std::cout << -1 << '\n';
        return;
    }
    train_system.TrainFile.read(currentTrain, posTmp[0]);
    if (!currentTrain.is_released) {
        std::cout << -1 << '\n';
        return;
    }
    int i; //起始站序号
    for (i = 0; i < currentTrain.stationNum; i++) {
        if (currentTrain.stations[i].stationName == startStation) {
            break;
        }
    }
    if (i == currentTrain.stationNum) {
        std::cout << -1 << '\n';
        return;
    }
    int j; //终点站序号
    for (j = i + 1; j < currentTrain.stationNum; j++) {
        if (currentTrain.stations[j].stationName == endStation) {
            break;
        }
    }
    if (j == currentTrain.stationNum) {
        std::cout << -1 << '\n';
        return;
    }
    JaneZ::Date earliestLeave = JaneZ::Date::addMinutesToDateTime(currentTrain.saleStartDate, currentTrain.startTime,
                                                                  currentTrain.stations[i].travelTime + currentTrain.
                                                                  stations[i].stopoverTime); //出发日期必须算上停留时间
    JaneZ::Date latestLeave = JaneZ::Date::addMinutesToDateTime(currentTrain.saleEndDate, currentTrain.startTime,
                                                                currentTrain.stations[i].travelTime + currentTrain.
                                                                stations[i].stopoverTime);
    if (day < earliestLeave || day > latestLeave) {
        std::cout << -1 << '\n';
        return;
    }
    int nDay = day - earliestLeave;
    if (num > currentTrain.seatNum) {
        std::cout << -1 << '\n';
        return;
    }
    JaneZ::TrainTime startTime;
    startTime.date = currentTrain.saleStartDate + nDay;
    startTime.clock = currentTrain.startTime;
    Seats seat;
    train_system.SeatFile.read(seat, currentTrain.fileIndex * train_system.MaxDays + nDay);
    int emptySeat = train_system.getSeats(seat, i, j, currentTrain.seatNum);
    if (emptySeat >= num) {
        seat.DeltaSeatNum[i] -= num;
        seat.DeltaSeatNum[j] += num;
        int totalPrice = num * (currentTrain.stations[j].price - currentTrain.stations[i].price);
        TicketInfo currentOrder;
        currentOrder.trainID = trainID;
        currentOrder.leaveTime = startTime + currentTrain.stations[i].travelTime + currentTrain.stations[i].
                                 stopoverTime;
        currentOrder.num = num;
        currentOrder.username = username;
        currentOrder.startStation = startStation;
        currentOrder.stIndex = i;
        currentOrder.endStation = endStation;
        currentOrder.enIndex = j;
        currentOrder.ticketPrice = currentTrain.stations[j].price - currentTrain.stations[i].price;
        currentOrder.timeCost = currentTrain.stations[j].travelTime - currentTrain.stations[i].travelTime - currentTrain
                                .stations[i].stopoverTime;
        currentOrder.waitList = waitList;
        currentOrder.timeStamp = timeStamp;
        currentOrder.status = Succeed;
        FormalList.insert(HashUsername, currentOrder);
        train_system.SeatFile.write(seat, currentTrain.fileIndex * train_system.MaxDays + nDay);
        std::cout << totalPrice << '\n';
        return;
    } else {
        if (!waitList) {
            std::cout << -1 << '\n';
            return;
        } else {
            std::cout << "queue" << '\n';
            //int totalPrice = num * (currentTrain.stations[j].price - currentTrain.stations[i].price);
            TicketInfo currentOrder;
            currentOrder.trainID = trainID;
            currentOrder.leaveTime = startTime + currentTrain.stations[i].travelTime + currentTrain.stations[i].
                                     stopoverTime;
            currentOrder.num = num;
            currentOrder.username = username;
            currentOrder.startStation = startStation;
            currentOrder.stIndex = i;
            currentOrder.endStation = endStation;
            currentOrder.enIndex = j;
            currentOrder.ticketPrice = currentTrain.stations[j].price - currentTrain.stations[i].price;
            currentOrder.timeCost = currentTrain.stations[j].travelTime - currentTrain.stations[i].travelTime -
                                    currentTrain.stations[i].stopoverTime;
            currentOrder.waitList = waitList;
            currentOrder.timeStamp = timeStamp;
            currentOrder.status = Pending;
            FormalList.insert(HashUsername, currentOrder);
            WaitTicket currentWait;
            currentWait.trainID = trainID;
            currentWait.StartDay = currentTrain.saleStartDate + nDay;
            WaitList.insert(currentWait, currentOrder);
            return;
        }
    }
}

void TicketSystem::query_order(JaneZ::String<22> &username, UserSystem &user_system, TrainSystem &train_system) {
    ull HashUsername = JaneZ::Hash<22>::HashFunction(username);
    if (user_system.LoginStack.find(HashUsername) == user_system.LoginStack.end()) {
        std::cout << -1 << '\n';
        return;
    }
    sjtu::vector<TicketInfo> result = FormalList.find(HashUsername);
    std::cout << result.size() << '\n';
    for (int i = result.size() - 1; i >= 0; i--) {
        if (result[i].status == Succeed) {
            std::cout << "[success] ";
        } else if (result[i].status == Pending) {
            std::cout << "[pending] ";
        } else if (result[i].status == Refunded) {
            std::cout << "[refunded] ";
        }
        std::cout << result[i].trainID << " ";
        std::cout << result[i].startStation << " ";
        std::cout << result[i].leaveTime << " ";
        std::cout << "-> ";
        std::cout << result[i].endStation << " ";
        std::cout << result[i].leaveTime + result[i].timeCost << " ";
        std::cout << result[i].ticketPrice << " ";
        std::cout << result[i].num << '\n';
    }
}

bool TicketSystem::refund_ticket(JaneZ::String<22> &username, int n, UserSystem &user_system,
                                 TrainSystem &train_system) {
    ull HashUsername = JaneZ::Hash<22>::HashFunction(username);
    if (user_system.LoginStack.find(HashUsername) == user_system.LoginStack.end()) {
        return false;
    }
    sjtu::vector<TicketInfo> result = FormalList.find(HashUsername);
    if (result.size() < n) {
        return false;
    }
    TicketInfo refundOrder = result[result.size() - n];
    if (refundOrder.status == Refunded) {
        return false;
    }
    JaneZ::String<22> currentTrainID = refundOrder.trainID;
    ull HashTrainID = JaneZ::Hash<22>::HashFunction(currentTrainID);
    TrainInfo currentTrain;
    sjtu::vector<int> posTmp = train_system.TrainBase.find(HashTrainID);
    train_system.TrainFile.read(currentTrain, posTmp[0]);
    int time = currentTrain.stations[refundOrder.stIndex].travelTime + currentTrain.stations[refundOrder.stIndex].
               stopoverTime;
    JaneZ::TrainTime earliestStart(currentTrain.saleStartDate, currentTrain.startTime);
    JaneZ::TrainTime earliestLeave = earliestStart + time;
    int nDay = refundOrder.leaveTime.date - earliestLeave.date;
    WaitTicket currentPending;
    currentPending.trainID = refundOrder.trainID;
    currentPending.StartDay = currentTrain.saleStartDate + nDay;
    if (refundOrder.status == Pending) {
        WaitList.erase(currentPending, refundOrder); //现在还不能改refundOrder
    } else if (refundOrder.status == Succeed) {
        Seats seat;
        train_system.SeatFile.read(seat, currentTrain.fileIndex * train_system.MaxDays + nDay);
        seat.DeltaSeatNum[refundOrder.stIndex] += refundOrder.num;
        seat.DeltaSeatNum[refundOrder.enIndex] -= refundOrder.num;
        sjtu::vector<TicketInfo> waitingOrder = WaitList.find(currentPending);
        for (int i = 0; i < waitingOrder.size(); i++) {
            int stIndex = waitingOrder[i].stIndex;
            int enIndex = waitingOrder[i].enIndex;
            int emptySeats = train_system.getSeats(seat, stIndex, enIndex, currentTrain.seatNum);
            if (emptySeats >= waitingOrder[i].num) {
                seat.DeltaSeatNum[stIndex] -= waitingOrder[i].num;
                seat.DeltaSeatNum[enIndex] += waitingOrder[i].num;
                waitingOrder[i].status = Succeed;
                WaitList.erase(currentPending, waitingOrder[i]);
                ull HashWaitUser = JaneZ::Hash<22>::HashFunction(waitingOrder[i].username);
                FormalList.erase(HashWaitUser, waitingOrder[i]);
                FormalList.insert(HashWaitUser, waitingOrder[i]);
            }
        }
        train_system.SeatFile.write(seat, currentTrain.fileIndex * train_system.MaxDays + nDay);
    }
    refundOrder.status = Refunded;
    FormalList.erase(HashUsername, refundOrder);
    FormalList.insert(HashUsername, refundOrder);
    return true;
}

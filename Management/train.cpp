//
// Created by JaneZ on 2025/5/5.
//

#include <iostream>
#include "train.h"

#include "../Data Structure/priority_queue.h"
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
    current.fileIndex = total;
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
        now.arrivingTimeCost = current.stations[i].travelTime;
        now.leavingTimeCost = current.stations[i].stopoverTime + now.arrivingTimeCost;
        now.price = current.stations[i].price;
        now.saleBeginDate = current.saleStartDate;
        now.saleEndDate = current.saleEndDate;
        now.StartClock = current.startTime;
        now.MaxSeatsNum = current.seatNum;
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
    JaneZ::TrainTime BeginTime(date,current.startTime);
    if(date > current.saleEndDate || date < current.saleStartDate) {
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
            std::cout << BeginTime << " ";
        }else if(i == current.stationNum - 1) {
            JaneZ::TrainTime currentTime = BeginTime + current.stations[i].travelTime;
            std::cout << currentTime << " ";
            std::cout << "->" << " ";
            std::cout << "xx-xx xx:xx ";
        }else {
            JaneZ::TrainTime currentTime = BeginTime + current.stations[i].travelTime;
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

int TrainSystem::getSeats(Seats seat, int stIndex, int toIndex, int maxSeatsNUm) {
    int delta = 0;
    int min = 100005;
    for(int i = stIndex ;i < toIndex;i ++) {
        delta += seat.DeltaSeatNum[i];
        if(delta < min) {
            min = delta;
        }
    }
    int stSeat = maxSeatsNUm;
    for(int i = 0;i < stIndex;i ++) {
        stSeat += seat.DeltaSeatNum[i];
    }
    return stSeat + min;
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
    sjtu::vector<StationValuePair> possibleSolution;
    while(st < startTotalNum && to < toTotalNum) {
        if(Start[st].fileIndex < To[to].fileIndex) {
            st ++;
            continue;
        }
        if(Start[st].fileIndex > To[to].fileIndex) {
            to ++;
            continue;
        }
        if(Start[st].nStation >= To[to].nStation) {
            st ++;
            to ++;
            continue;
        }
        StationValuePair now;
        now.st = Start[st];
        now.to = To[to];
        possibleSolution.push_back(now);
        st ++;
        to ++;
    }
    sjtu::priority_queue<TimeCostInfo,CompTime> timePQ;
    sjtu::priority_queue<TimeCostInfo,CompCost> costPQ;
    for(int i = 0;i < possibleSolution.size();i ++) {
        JaneZ::Date earliestDate = JaneZ::Date::addMinutesToDateTime(possibleSolution[i].st.saleBeginDate,possibleSolution[i].st.StartClock,possibleSolution[i].st.leavingTimeCost);
        JaneZ::Date latestDate = JaneZ::Date::addMinutesToDateTime(possibleSolution[i].st.saleEndDate,possibleSolution[i].st.StartClock,possibleSolution[i].st.leavingTimeCost);
        if(d < earliestDate || d > latestDate) {
            continue;
        }
        if(SortWay == JaneZ::SortType::time) {
            TimeCostInfo current;
            current.index = i;
            current.totalTime = possibleSolution[i].to.arrivingTimeCost - possibleSolution[i].st.leavingTimeCost;
            current.totalCost = possibleSolution[i].to.price - possibleSolution[i].st.price;
            current.trainID = possibleSolution[i].st.trainID;
            timePQ.push(current);
        }else {
            TimeCostInfo current;
            current.index = i;
            current.totalTime = possibleSolution[i].to.arrivingTimeCost - possibleSolution[i].st.leavingTimeCost;
            current.totalCost = possibleSolution[i].to.price - possibleSolution[i].st.price;
            current.trainID = possibleSolution[i].st.trainID;
            costPQ.push(current);
        }
    }
    if(SortWay == JaneZ::SortType::time) {
        std::cout << timePQ.size() << '\n';
        while(!timePQ.empty()) {
            TimeCostInfo top = timePQ.top();
            timePQ.pop();
            std::cout << top.trainID << " ";
            StationValue topValue1 = possibleSolution[top.index].st;
            JaneZ::Date earliestDate = JaneZ::Date::addMinutesToDateTime(topValue1.saleBeginDate,topValue1.StartClock,topValue1.leavingTimeCost);
            StationValue topValue2 = possibleSolution[top.index].to;
            std::cout << topValue1.stationName << " ";
            std::cout << d << " ";
            JaneZ::Clock startTime = topValue1.StartClock;
            JaneZ::Clock leavingClock = startTime + topValue1.leavingTimeCost;
            std::cout << leavingClock << " ";
            std::cout << "-> ";
            std::cout << topValue2.stationName << " ";
            JaneZ::Date arrivingDate = JaneZ::Date::addMinutesToDateTime(d,leavingClock,top.totalTime);
            std::cout << arrivingDate << " ";
            JaneZ::Clock arrivingClock = startTime + topValue2.arrivingTimeCost;
            std::cout << arrivingClock << " ";
            std::cout << top.totalCost << " ";
            Seats seat;
            int nDay = d - earliestDate;
            SeatFile.read(seat,MaxDays * topValue1.fileIndex + nDay);
            std::cout << getSeats(seat,topValue1.nStation,topValue2.nStation,topValue1.MaxSeatsNum) << '\n';
        }
    }else if(SortWay == JaneZ::SortType::cost) {
        std::cout << costPQ.size() << '\n';
        while(!costPQ.empty()) {
            TimeCostInfo top = costPQ.top();
            costPQ.pop();
            std::cout << top.trainID << " ";
            StationValue topValue1 = possibleSolution[top.index].st;
            JaneZ::Date earliestDate = JaneZ::Date::addMinutesToDateTime(topValue1.saleBeginDate,topValue1.StartClock,topValue1.leavingTimeCost);
            StationValue topValue2 = possibleSolution[top.index].to;
            std::cout << topValue1.stationName << " ";
            std::cout << d << " ";
            JaneZ::Clock startTime = topValue1.StartClock;
            JaneZ::Clock leavingClock = startTime + topValue1.leavingTimeCost;
            std::cout << leavingClock << " ";
            std::cout << "-> ";
            std::cout << topValue2.stationName << " ";
            JaneZ::Date arrivingDate = JaneZ::Date::addMinutesToDateTime(d,leavingClock,top.totalTime);
            std::cout << arrivingDate << " ";
            JaneZ::Clock arrivingClock = startTime + topValue2.arrivingTimeCost;
            std::cout << arrivingClock << " ";
            std::cout << top.totalCost << " ";
            Seats seat;
            int nDay = d - earliestDate;
            SeatFile.read(seat,MaxDays * topValue1.fileIndex + nDay);
            std::cout << getSeats(seat,topValue1.nStation,topValue2.nStation,topValue1.MaxSeatsNum) << '\n';
        }
    }
}

void TrainSystem::printTransfer(JaneZ::String<42> &s, JaneZ::String<42> &t,TransferInfo &current) {
    std::cout << current.firstTrain << " ";
    std::cout << s << " ";
    std::cout << current.initialTime << " ";
    std::cout << "-> ";
    std::cout << current.midStationName << " ";
    std::cout << current.midArriveTime << " ";
    std::cout << current.firstCost << " ";
    std::cout << current.firstEmptySeat << '\n';
    std::cout << current.secondTrain << " ";
    std::cout << current.midStationName << " ";
    std::cout << current.midLeaveTime << " ";
    std::cout << "-> ";
    std::cout << t << " ";
    std::cout << current.finalTime << " ";
    std::cout << current.secondCost << " ";
    std::cout << current.secondEmptySeat << '\n';
}

void TrainSystem::query_transfer(JaneZ::String<42> &s, JaneZ::String<42> &t, JaneZ::Date &d, JaneZ::SortType SortWay) {
    ull HashStartStation = JaneZ::Hash<42>::HashFunction(s);
    ull HashToStation = JaneZ::Hash<42>::HashFunction(t);
    sjtu::vector<StationValue> Start = ReleasedTrainBase.find(HashStartStation);
    sjtu::vector<StationValue> To = ReleasedTrainBase.find(HashToStation);

    TransferInfo minTime;
    TransferInfo minCost;

    bool TimeFlag = false;
    bool CostFlag = false;

    for(int i = 0;i < Start.size();i ++) {
        int departNum = Start[i].nStation;
        TrainInfo firstTrain;
        TrainFile.read(firstTrain,Start[i].fileIndex);
        StationInfo Departure = firstTrain.stations[departNum];
        JaneZ::Date earliestDate = JaneZ::Date::addMinutesToDateTime(firstTrain.saleStartDate,firstTrain.startTime,Departure.travelTime + Departure.stopoverTime);
        JaneZ::Date latestDate = JaneZ::Date::addMinutesToDateTime(firstTrain.saleEndDate,firstTrain.startTime,Departure.travelTime + Departure.stopoverTime);
        if(d < earliestDate || d > latestDate) {
            continue;
        }
        JaneZ::Date day = firstTrain.saleStartDate + (d - earliestDate);

        JaneZ::TrainTime firstTrainStart(day,firstTrain.startTime);//具体哪一天发车
        for(int j = 0;j < To.size();j ++) {
            if(Start[i].fileIndex == To[j].fileIndex) {
                continue;
            }
            TrainInfo secondTrain;
            TrainFile.read(secondTrain,To[i].fileIndex);
            //检查secondTrain是否在可行时间范围内

            int terminalNum = To[j].nStation;
            StationInfo Terminal = secondTrain.stations[terminalNum];
            for(int k = 0;k < terminalNum;k ++) { //遍历第二辆车第0站到终点站前一站
                for(int l = departNum + 1;l < firstTrain.stationNum;l ++) {  //遍历第一辆车从起点站到最后一战
                    if(firstTrain.stations[l].stationName != secondTrain.stations[k].stationName) {
                        continue;
                    }
                    StationInfo midStation1 = firstTrain.stations[l];
                    JaneZ::TrainTime midArrive = firstTrainStart + midStation1.travelTime;//到中转站时间

                    StationInfo midStation2 = secondTrain.stations[k];
                    JaneZ::TrainTime secondStart(secondTrain.saleStartDate,secondTrain.startTime);
                    //第二辆车最早离开中转站时刻
                    JaneZ::TrainTime secondEarliest = secondStart + midStation2.travelTime + midStation2.stopoverTime;
                    JaneZ::TrainTime secondEnd(secondTrain.saleEndDate,secondTrain.startTime);
                    //第二辆车最晚离开中转站时刻
                    JaneZ::TrainTime secondLatest = secondEnd + midStation2.travelTime + midStation2.stopoverTime;

                    if(midArrive > secondLatest) {
                        continue; //绝对没有可能
                    }else if(midArrive < secondEarliest) {
                        //有可能，等最早的第二班车到
                        int waitTime = secondEarliest - midArrive;
                        int firstRunningTime = midStation1.travelTime - Departure.travelTime - Departure.stopoverTime;
                        int secondRunningTime = Terminal.travelTime - midStation2.travelTime - midStation2.stopoverTime;
                        int totalTime = firstRunningTime + secondRunningTime + waitTime;
                        int firstCost = midStation1.price - Departure.price;
                        int secondCost = Terminal.price - midStation2.price;
                        int totalCost = firstCost + secondCost;

                        TransferInfo current;
                        current.totalTime = totalTime;
                        current.totalCost = totalCost;
                        current.firstTrain = firstTrain.trainID;
                        current.secondTrain = secondTrain.trainID;

                        current.initialTime = firstTrainStart + Departure.travelTime + Departure.stopoverTime;
                        current.midArriveTime = midArrive;
                        current.midLeaveTime = secondEarliest;
                        current.finalTime = secondEarliest + secondRunningTime;

                        current.midStationName = midStation1.stationName;

                        current.firstCost = firstCost;
                        current.secondCost = secondCost;

                        Seats seat1;
                        int nDay1 = firstTrainStart.date - firstTrain.saleStartDate;
                        SeatFile.read(seat1,Start[i].fileIndex * MaxDays + nDay1);
                        int emptySeat1 = getSeats(seat1,departNum,l,firstTrain.seatNum);
                        current.firstEmptySeat = emptySeat1;

                        Seats seat2;
                        int nDay2 = 0;
                        SeatFile.read(seat2,To[j].fileIndex * MaxDays + nDay2);
                        int emptySeat2 = getSeats(seat2,k,terminalNum,secondTrain.seatNum);
                        current.secondEmptySeat = emptySeat2;

                        if(SortWay == JaneZ::SortType::time) {
                            if(TransferInfo::timeCmp(current,minTime)) {
                                minTime = current;
                                TimeFlag = true;
                            }
                        }else if(SortWay == JaneZ::SortType::cost) {
                            if(TransferInfo::costCmp(current,minCost)) {
                                minCost = current;
                                CostFlag = true;
                            }
                        }
                    }else {
                        JaneZ::Clock firstClock = midArrive.clock;
                        JaneZ::Clock secondClock = secondEarliest.clock;
                        TransferInfo current;
                        int firstRunningTime = midStation1.travelTime - Departure.travelTime - Departure.stopoverTime;
                        int secondRunningTime = Terminal.travelTime - midStation2.travelTime - midStation2.stopoverTime;
                        int firstCost = midStation1.price - Departure.price;
                        int secondCost = Terminal.price - midStation2.price;
                        int totalCost = firstCost + secondCost;

                        current.firstTrain = firstTrain.trainID;
                        current.secondTrain = secondTrain.trainID;

                        current.initialTime = firstTrainStart + Departure.travelTime + Departure.stopoverTime;
                        current.midArriveTime = midArrive;

                        current.midStationName = midStation1.stationName;

                        current.firstCost = firstCost;
                        current.secondCost = secondCost;
                        current.totalCost = totalCost;

                        Seats seat1;
                        int nDay1 = firstTrainStart.date - firstTrain.saleStartDate;
                        SeatFile.read(seat1,Start[i].fileIndex * MaxDays + nDay1);
                        int emptySeat1 = getSeats(seat1,departNum,l,firstTrain.seatNum);
                        current.firstEmptySeat = emptySeat1;

                        if(firstClock <= secondClock) {
                            //当天即可完成换乘
                            int waitTime = secondClock - firstClock;
                            int totalTime = firstRunningTime + secondRunningTime + waitTime;
                            current.totalTime = totalTime;

                            current.midLeaveTime.date = midArrive.date;
                            current.midLeaveTime.clock = secondClock;
                            current.finalTime = current.midLeaveTime + secondRunningTime;

                            Seats seat2;
                            int nDay2 = current.midLeaveTime.date - secondEarliest.date;
                            SeatFile.read(seat2,To[j].fileIndex * MaxDays + nDay2);
                            int emptySeat2 = getSeats(seat2,k,terminalNum,secondTrain.seatNum);
                            current.secondEmptySeat = emptySeat2;
                        }else {
                            //隔夜换乘
                            int waitTime = secondClock.hour * 60 + secondClock.minute + 24 * 60 - firstClock.hour * 60 - firstClock.minute;
                            int totalTime = firstRunningTime + secondRunningTime + waitTime;
                            current.totalTime = totalTime;

                            current.midLeaveTime.date = midArrive.date + 1;
                            current.midLeaveTime.clock = secondClock;
                            current.finalTime = current.midLeaveTime + secondRunningTime;

                            Seats seat2;
                            int nDay2 = current.midLeaveTime.date - secondEarliest.date;
                            SeatFile.read(seat2,To[j].fileIndex * MaxDays + nDay2);
                            int emptySeat2 = getSeats(seat2,k,terminalNum,secondTrain.seatNum);
                            current.secondEmptySeat = emptySeat2;
                        }
                        if(SortWay == JaneZ::SortType::time) {
                            if(TransferInfo::timeCmp(current,minTime)) {
                                minTime = current;
                                TimeFlag = true;
                            }
                        }else if(SortWay == JaneZ::SortType::cost) {
                            if(TransferInfo::costCmp(current,minCost)) {
                                minCost = current;
                                CostFlag = true;
                            }
                        }
                    }
                }
            }
        }
    }
    if(SortWay == JaneZ::SortType::time) {
        if(!TimeFlag) {
            std::cout << 0 << '\n';
        }else {
            printTransfer(s,t,minTime);
        }
    }else if(SortWay == JaneZ::SortType::cost) {
        if(!CostFlag) {
            std::cout << 0 << '\n';
        }else {
            printTransfer(s,t,minCost);
        }
    }
}

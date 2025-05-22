//
// Created by JaneZ on 2025/5/5.
//

#ifndef TRAIN_H
#define TRAIN_H
#include "../Data Structure/BPT.h"
#include "../Data Structure/String.h"
#include "../Data Structure/vector.h"
#include "../Management/time.h"
#include "../Tool/HashFunction.h"
#include "../Tool/tokenSlicer.h"
#include "../Tool/MemoryRiver.h"

using ull = unsigned long long int;

struct StationInfo {
    JaneZ::String<42> stationName;//NJN
    /*以 SH-NJ-BJ为例
     * stations[0].prices = 0
     * stations[1].prices = SH-NJ 票价
     * stations[2].prices = SH-BJ 票价
     * 所以这意味着我维护的是前缀和。所以我需要对传入的数据做一定处理
     */
    int price;
    /*依然以 SH-NJ-BJ为例
     * stations[0].travelTime = 0
     * stations[1].travelTime = SH-NJ时间（59min bushi）
     * stations[2].travelTime = SH-BJ时间
     * 所以这意味着我维护的是前缀和。所以我需要对传入的数据做一定处理
     */
    int travelTime;
    /*依然以 SH-NJ-BJ为例
     * stations[0].stopoverTime = 0
     * stations[1].stopoverTime = NJN 停留时间（2min bushi）
     * stations[2].stopoverTime = 0
     */
    int stopoverTime;
    //int freeSeatNum;  在这里存freeSeat不太合适
    /*依然以 SH-NJ-BJ为例
     * stations[i].stationIndex = i;
     */
    int stationIndex;//沿线的第几站
};

struct Seats {
    /*依然以 SH-NJ-BJ为例
     * freeSeatNum[0] = seatNum;
     * freeSeatNum[1] = seatNum - 100 （打个比方
     * freeSeatNum[2] = seatNum - 200 （再打个比方
     * 所以求某个区间[a,b]上的余票数等价于求min(freeSeatNum[a+1],...,freeSeatNum[b])
     */
    int DeltaSeatNum[102] = {};
};

struct TrainInfo {
    JaneZ::String<22> trainID;//列车车次
    int stationNum;//停站数 （SH-NJ-BJ 为3站）
    StationInfo stations[102];
    int seatNum;//总座位数
    JaneZ::Clock startTime;//每日发车时间
    JaneZ::Date saleStartDate;//开始售卖日期
    JaneZ::Date saleEndDate;//停止售卖日期
    char type;//列车类型
    bool is_released = false;
    int fileIndex;

    TrainInfo():is_released(false){}

    TrainInfo(const TrainInfo &other) = default;

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

/* 维护每个车次经过的站信息
 * 用在对已发布列车的车票查询中
 * 需要将站名作为KEY；将这一信息作为Other
 */
struct StationValue {
    JaneZ::String<42> stationName;
    JaneZ::String<22> trainID;
    int fileIndex;//源信息在TrainFile中的位置
    int nStation;//第几站
    int arrivingTimeCost;
    int leavingTimeCost;
    int price;
    int MaxSeatsNum;
    JaneZ::Date saleBeginDate;
    JaneZ::Date saleEndDate;
    JaneZ::Clock StartClock;

    bool operator<(const StationValue &other) const{
        return fileIndex < other.fileIndex;
    }

    bool operator>(const StationValue &other) const{
        return fileIndex > other.fileIndex;
    }

    bool operator==(const StationValue &other) const {
        return fileIndex == other.fileIndex;
    }
};

struct StationValuePair {
    StationValue st;
    StationValue to;
};

struct TimeCostInfo {
    int totalTime;
    int totalCost;
    JaneZ::String<22> trainID;
    int index;//数组下标
};

struct CompTime {
    bool operator()(const TimeCostInfo& a, const TimeCostInfo& b) const {
        if (a.totalTime != b.totalTime) {
            return a.totalTime > b.totalTime;
        }
        return a.trainID > b.trainID;
    }
};

struct CompCost {
    bool operator()(const TimeCostInfo& a, const TimeCostInfo& b) const {
        if (a.totalCost != b.totalCost) {
            return a.totalCost > b.totalCost;
        }
        return a.trainID > b.trainID;
    }
};

struct TransferInfo {
    int totalTime;
    int totalCost;
    JaneZ::String<22> firstTrain;
    JaneZ::TrainTime initialTime;
    JaneZ::TrainTime midArriveTime;
    JaneZ::String<22> secondTrain;
    JaneZ::TrainTime midLeaveTime;
    JaneZ::TrainTime finalTime;
    JaneZ::String<42> midStationName;
    int firstCost;
    int secondCost;
    int firstEmptySeat;
    int secondEmptySeat;

    TransferInfo():totalTime(1e9),totalCost(1e9){}

    TransferInfo& operator=(const TransferInfo &other) {
        if (this == &other) return *this;

        totalTime = other.totalTime;
        totalCost = other.totalCost;
        firstCost = other.firstCost;
        secondCost = other.secondCost;
        firstEmptySeat = other.firstEmptySeat;
        secondEmptySeat = other.secondEmptySeat;

        firstTrain = other.firstTrain;
        secondTrain = other.secondTrain;
        midStationName = other.midStationName;
        initialTime = other.initialTime;
        midArriveTime = other.midArriveTime;
        midLeaveTime = other.midLeaveTime;
        finalTime = other.finalTime;

        return *this;
    }

    static bool timeCmp(const TransferInfo &A,const TransferInfo &B) {
        if(A.totalTime < B.totalTime) {
            return true;
        }else if(A.totalTime > B.totalTime) {
            return false;
        }
        if(A.totalCost < B.totalCost) {
            return true;
        }else if(A.totalCost > B.totalCost) {
            return false;
        }
        if(A.firstTrain < B.firstTrain) {
            return true;
        }else if(A.firstTrain > B.firstTrain) {
            return false;
        }
        if(A.secondTrain < B.secondTrain) {
            return true;
        }
        return false;
    }

    static bool costCmp(const TransferInfo &A,const TransferInfo &B) {
        if(A.totalCost < B.totalCost) {
            return true;
        }else if(A.totalCost > B.totalCost) {
            return false;
        }
        if(A.totalTime < B.totalTime) {
            return true;
        }else if(A.totalTime > B.totalTime) {
            return false;
        }
        if(A.firstTrain < B.firstTrain) {
            return true;
        }else if(A.firstTrain > B.firstTrain) {
            return false;
        }
        if(A.secondTrain < B.secondTrain) {
            return true;
        }
        return false;
    }
};

class TrainSystem {
    friend class TicketSystem;
private:
    BPT<ull, int> TrainBase;//存储所有火车在TrainFile中的位置，无论发布与否
    MemoryRiver<TrainInfo,1> TrainFile;//存储所有火车信息，无论发布与否
    BPT<ull, StationValue> ReleasedTrainBase;//这个地方Key存什么似乎需要做一些变化
    //ull是对被released的车的每一站做哈希
    int total = 0;
    MemoryRiver<Seats,1> SeatFile;//存储卖出去的座位信息，以每日的一班车为单位（不可大于最大座位数）
    //rmk: 这里尝试维护一个变化信息：卖出去- 有人下车+
    const int MaxDays = 95;

public:
    TrainSystem():TrainBase("TrainBaseIndex","TrainBaseLeaf"),ReleasedTrainBase("ReleasedTrainBaseIndex","ReleasedTrainBaseLeaf") {
        TrainFile.initialise("train.txt");
        SeatFile.initialise("seat.txt");
        TrainFile.get_info(total,1);
    }

    ~TrainSystem() = default;

    bool add_train(JaneZ::String<22> &trainID,
                   int stationNum,
                   int seatNum,
                   sjtu::vector<JaneZ::String<42> > &stations,
                   sjtu::vector<int> &prices,
                   JaneZ::Clock &startTime,
                   sjtu::vector<int> &travelTimes,
                   sjtu::vector<int> &stopoverTimes,
                   JaneZ::Date &saleStartDate,
                   JaneZ::Date &saleEndDate,
                   char type);

    bool delete_train(JaneZ::String<22> &trainID);

    bool release_train(JaneZ::String<22> &trainID);

    bool query_train(JaneZ::String<22> &trainID, JaneZ::Date &date);

    int getSeats(Seats seat,int stIndex,int toIndex,int maxSeatsNUm);

    void query_ticket(JaneZ::String<42> &s, JaneZ::String<42> &t, JaneZ::Date &d, JaneZ::SortType SortWay);

    void printTransfer(JaneZ::String<42> &s, JaneZ::String<42> &t,TransferInfo &current);

    void query_transfer(JaneZ::String<42> &s, JaneZ::String<42> &t, JaneZ::Date &d, JaneZ::SortType SortWay);

    static void clean() {
        // 删除未发布列车数据的索引和叶子文件
        std::remove("TrainBaseIndex");
        std::remove("TrainBaseLeaf");

        // 删除已发布列车数据的索引和叶子文件
        std::remove("ReleasedTrainBaseIndex");
        std::remove("ReleasedTrainBaseLeaf");
    }
};

#endif //TRAIN_H

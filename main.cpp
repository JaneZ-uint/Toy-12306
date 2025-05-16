#include <iostream>
#include <string>

#include "Management/ticket.h"
#include "Management/train.h"
#include "Management/user.h"
#include "Tool/tokenSlicer.h"

int main(){
    std::string op;
    UserSystem user_system;
    TrainSystem train_system;
    TicketSystem ticket_system;
    while (true) {
        std::getline(std::cin,op);
        TokenSlicer knife(op);
        JaneZ::TotalInfo tmp;
        knife.SliceTokens(op,tmp);
        if(tmp.a == JaneZ::opType::add_user) {
            JaneZ::String<22> curUsername(tmp.cur_username);
            JaneZ::String<22> username(tmp.username);
            JaneZ::String<32> password(tmp.password);
            JaneZ::String<22> name(tmp.name);
            JaneZ::String<32> mail(tmp.mail);
            int privilege = tmp.privilege;
            if(user_system.add_user(curUsername,username,password,name,mail,privilege)) {
                std::cout << 0 << '\n';
            }else {
                std::cout << -1 << '\n';
            }
        }else if(tmp.a == JaneZ::opType::login) {
            JaneZ::String<22> username(tmp.username);
            JaneZ::String<32> password(tmp.password);
            if(user_system.login(username,password)) {
                std::cout << 0 << '\n';
            }else {
                std::cout << -1 << '\n';
            }
        }else if(tmp.a == JaneZ::opType::logout) {
            JaneZ::String<22> username(tmp.username);
            if(user_system.logout(username)) {
                std::cout << 0 << '\n';
            }else {
                std::cout << -1 << '\n';
            }
        }else if(tmp.a == JaneZ::opType::query_profile) {
            JaneZ::String<22> curUsername(tmp.cur_username);
            JaneZ::String<22> username(tmp.username);
            UB result = user_system.query_profile(curUsername,username);
            if(result.canBeOperated == false) {
                std::cout << -1 << '\n';
            }else {
                std::cout << result.Info.username << " "<< result.Info.name << " "<< result.Info.mailAddr << " " << result.Info.privilege<< '\n';
            }
        }else if(tmp.a == JaneZ::opType::modify_profile) {
            JaneZ::String<22> curUsername(tmp.cur_username);
            JaneZ::String<22> username(tmp.username);
            JaneZ::String<32> password(tmp.password);
            JaneZ::String<22> name(tmp.name);
            JaneZ::String<32> mail(tmp.mail);
            int privilege = tmp.privilege;
            UB result = user_system.modify_profile(curUsername,username,password,name,mail,privilege);
            if(result.canBeOperated == false) {
                std::cout << -1 << '\n';
            }else {
                std::cout << result.Info.username << " "<< result.Info.name << " "<< result.Info.mailAddr << " " << result.Info.privilege<< '\n';
            }
        }else if(tmp.a == JaneZ::opType::add_train) {
            JaneZ::String<22> trainID(tmp.ID);
            int StationNum = tmp.StationNum;
            int SeatNum = tmp.SeatNum;
            sjtu::vector<JaneZ::String<42>> Stations;
            for(int i = 0;i < StationNum;i ++) {
                JaneZ::String<42> current(tmp.Stations[i]);
                Stations.push_back(current);
            }
            sjtu::vector<int> prices;
            for(int i = 0;i < StationNum - 1;i ++) {
                prices.push_back(tmp.prices[i]);
            }
            JaneZ::Clock startTime = tmp.startTime;
            sjtu::vector<int> travelTimes;
            for(int i = 0;i < StationNum - 1;i ++) {
                travelTimes.push_back(tmp.travelTimes[i]);
            }
            sjtu::vector<int> stopoverTimes;
            for(int i = 0;i < StationNum - 2;i ++) {
                stopoverTimes.push_back(tmp.stopoverTimes[i]);
            }
            JaneZ::Date saleBeginDate = tmp.saleBeginDate;
            JaneZ::Date saleEndDate = tmp.saleEndDate;
            char type = tmp.Type;
            train_system.add_train(trainID,StationNum,SeatNum,Stations,prices,startTime,travelTimes,stopoverTimes,saleBeginDate,saleEndDate,type);
        }else if(tmp.a == JaneZ::opType::delete_train) {
            JaneZ::String<22> trainID(tmp.ID);
            train_system.delete_train(trainID);
        }else if(tmp.a == JaneZ::opType::release_train) {
            JaneZ::String<22> trainID(tmp.ID);
            train_system.release_train(trainID);
        }else if(tmp.a == JaneZ::opType::query_train) {
            JaneZ::String<22> trainID(tmp.ID);
            JaneZ::Date startDate = tmp.BeginDate;
            train_system.query_train(trainID,startDate);
        }else if(tmp.a == JaneZ::opType::query_ticket) {
            JaneZ::String<42> startPlace(tmp.fromStation);
            JaneZ::String<42> endPlace(tmp.toStation);
            JaneZ::Date startDate = tmp.SpecificStationDate;
            JaneZ::SortType sortWay = tmp.SortWay;
            train_system.query_ticket(startPlace,endPlace,startDate,sortWay);
        }else if(tmp.a == JaneZ::opType::query_transfer) {
            JaneZ::String<42> startPlace(tmp.fromStation);
            JaneZ::String<42> endPlace(tmp.toStation);
            JaneZ::Date startDate = tmp.SpecificStationDate;
            JaneZ::SortType sortWay = tmp.SortWay;
            train_system.query_transfer(startPlace,endPlace,startDate,sortWay);
        }else if(tmp.a == JaneZ::opType::buy_ticket) {
            JaneZ::String<22> username(tmp.username);
            JaneZ::String<22> trainID(tmp.ID);
            JaneZ::Date startDate = tmp.SpecificStationDate;
            int TicketNum = tmp.buyNumber;
            JaneZ::String<42> startPlace(tmp.fromStation);
            JaneZ::String<42> endPlace(tmp.toStation);
            bool waitList = tmp.waitList;
            ticket_system.buy_ticket(username,trainID,startDate,TicketNum,startPlace,endPlace,waitList);
        }else if(tmp.a == JaneZ::opType::query_order) {
            JaneZ::String<22> username(tmp.username);
            ticket_system.query_order(username);
        }else if(tmp.a == JaneZ::opType::refund_ticket) {
            JaneZ::String<22> username(tmp.username);
            int nOrder = tmp.nOrder;
            ticket_system.refund_ticket(username,nOrder);
        }else if(tmp.a == JaneZ::opType::clean) {
            UserSystem::clean();
            TrainSystem::clean();
            TicketSystem::clean();
            std::cout << "0" << '\n';
        }else if(tmp.a == JaneZ::opType::exit) {
            std::cout << "bye" << '\n';
            break;
        }
    }
    return 0;
}

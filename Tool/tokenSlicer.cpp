//
// Created by JaneZ on 2025/5/8.
//

#include "tokenSlicer.h"

TokenSlicer::TokenSlicer(std::string &s):op(s) {}

int TokenSlicer::StringToInteger(std::string &s) {
    int result = 0;
    for(int i = 0;i < s.size();i ++) {
        result = 10 * result + (s[i] - '0');
    }
    return result;
}

JaneZ::Date TokenSlicer::StringToDate(std::string &s) {
    JaneZ::Date current;
    std::string tmp;
    tmp += s[0];
    tmp += s[1];
    current.month = StringToInteger(tmp);
    tmp = "";
    tmp += s[3];
    tmp += s[4];
    current.day = StringToInteger(tmp);
    return current;
}

sjtu::vector<std::string> TokenSlicer::StringToVectorString(std::string &s) {
    int length = s.size();
    int i = 0;
    sjtu::vector<std::string> result;
    while(i != length) {
        if(s[i] == '|') {
            ++ i;
        }
        std::string tmp;
        while(s[i] != '|') {
            tmp += s[i];
            ++ i;
        }
        result.push_back(tmp);
    }
    return result;
}

sjtu::vector<int> TokenSlicer::StringToVectorInt(std::string &s) {
    int length = s.size();
    int i = 0;
    sjtu::vector<int> result;
    while(i != length) {
        if(s[i] == '|') {
            ++ i;
        }
        std::string tmp;
        while(s[i] != '|') {
            tmp += s[i];
            ++ i;
        }
        result.push_back(StringToInteger(tmp));
    }
    return result;
}

JaneZ::Clock TokenSlicer::StringToClock(std::string &s) {
    JaneZ::Clock current;
    std::string tmp;
    tmp += s[0];
    tmp += s[1];
    current.hour = StringToInteger(tmp);
    tmp = "";
    tmp += s[3];
    tmp += s[4];
    current.minute = StringToInteger(tmp);
    return current;
}

void TokenSlicer::SliceTokens(std::string &s,JaneZ::TotalInfo &info) {
    int length = s.size();
    int i = 0;
    while(s[i] != ' ') {   //跳过时间戳
        i ++;
    }
    ++ i;
    std::string operation;
    while(s[i] != ' ' || i != length) {
        operation += s[i];
        ++ i;
    }
    if(operation == "add_user") {
        info.a = JaneZ::add_user;
        while(i != length) {
            std::string tmp;
            if(s[i + 2] == 'c') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.cur_username = tmp;
            }else if(s[i + 2] == 'u') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.username = tmp;
            }else if(s[i + 2] == 'p') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.password = tmp;
            }else if(s[i + 2] == 'n') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.name = tmp;
            }else if(s[i + 2] == 'm') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.mail = tmp;
            }else if(s[i + 2] == 'g') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.privilege = StringToInteger(tmp);
            }
        }
    }else if(operation == "login") {
        info.a = JaneZ::login;
        while(i != length) {
            std::string tmp;
            if(s[i + 2] == 'u') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.username = tmp;
            }else if(s[i + 2] == 'p') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.password = tmp;
            }
        }
    }else if(operation == "logout") {
        info.a = JaneZ::logout;
        i += 4;
        std::string tmp;
        while(s[i] != ' ' || i != length) {
            tmp += s[i];
            ++ i;
        }
        info.username = tmp;
    }else if(operation == "query_profile") {
        info.a = JaneZ::query_profile;
        while(i != length) {
            std::string tmp;
            if(s[i + 2] == 'c') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.cur_username = tmp;
            }else if(s[i + 2] == 'u') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.username = tmp;
            }
        }
    }else if(operation == "modify_profile") {
        info.a = JaneZ::modify_profile;
        while(i != length) {
            std::string tmp;
            if(s[i + 2] == 'c') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.cur_username = tmp;
            }else if(s[i + 2] == 'u') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.username = tmp;
            }else if(s[i + 2] == 'p') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.password = tmp;
            }else if(s[i + 2] == 'n') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.name = tmp;
            }else if(s[i + 2] == 'm') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.mail = tmp;
            }else if(s[i + 2] == 'g') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.privilege = StringToInteger(tmp);
            }
        }
    }else if(operation == "add_train") {
        info.a = JaneZ::add_train;
        while(i != length) {
            std::string tmp;
            if(s[i + 2] == 'i') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.ID = tmp;
            }else if(s[i + 2] == 'n') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.StationNum = StringToInteger(tmp);
            }else if(s[i + 2] == 'm') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.SeatNum = StringToInteger(tmp);
            }else if(s[i + 2] == 's') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.Stations = StringToVectorString(tmp);
            }else if(s[i + 2] == 'p') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.prices = StringToVectorInt(tmp);
            }else if(s[i + 2] == 'x') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.startTime = StringToClock(tmp);
            }else if(s[i + 2] == 't') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.travelTimes = StringToVectorInt(tmp);
            }else if(s[i + 2] == 'o') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.stopoverTimes = StringToVectorInt(tmp);
            }else if(s[i + 2] == 'd') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                sjtu::vector<std::string> tmpPair = StringToVectorString(tmp);
                info.saleBeginDate = StringToDate(tmpPair[0]);
                info.saleEndDate = StringToDate(tmpPair[1]);
            }else if(s[i + 2] == 'y') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.Type = tmp[0];
            }
        }
    }else if(operation == "delete_train") {
        info.a = JaneZ::delete_train;
        i += 4;
        std::string tmp;
        while(s[i] != ' ' || i != length) {
            tmp += s[i];
            ++ i;
        }
        info.ID = tmp;
    }else if(operation == "release_train") {
        info.a = JaneZ::release_train;
        i += 4;
        std::string tmp;
        while(s[i] != ' ' || i != length) {
            tmp += s[i];
            ++ i;
        }
        info.ID = tmp;
    }else if(operation == "query_train") {
        info.a = JaneZ::query_train;
        i += 4;
        std::string tmp;
        while(s[i] != ' ' || i != length) {
            tmp += s[i];
            ++ i;
        }
        info.ID = tmp;
    }else if(operation == "query_ticket") {
        info.a = JaneZ::query_ticket;
        info.SortWay = JaneZ::SortType::time;
        while(i != length) {
            std::string tmp;
            if(s[i + 2] == 's') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.fromStation = tmp;
            }else if(s[i + 2] == 't') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.toStation = tmp;
            }else if(s[i + 2] == 'd') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.SpecificStationDate = StringToDate(tmp);
            }else if(s[i + 2] == 'p') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                if(tmp =="time") {
                    info.SortWay = JaneZ::SortType::time;
                }else {
                    info.SortWay = JaneZ::SortType::cost;
                }
            }
        }
    }else if(operation == "query_transfer") {
        info.a = JaneZ::query_transfer;
        info.SortWay = JaneZ::SortType::time;
        while(i != length) {
            std::string tmp;
            if(s[i + 2] == 's') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.fromStation = tmp;
            }else if(s[i + 2] == 't') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.toStation = tmp;
            }else if(s[i + 2] == 'd') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.SpecificStationDate = StringToDate(tmp);
            }else if(s[i + 2] == 'p') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                if(tmp =="time") {
                    info.SortWay = JaneZ::SortType::time;
                }else {
                    info.SortWay = JaneZ::SortType::cost;
                }
            }
        }
    }else if(operation == "buy_ticket") {
        info.a = JaneZ::buy_ticket;
        info.waitList = false;
        while(i != length) {
            std::string tmp;
            if(s[i + 2] == 'u') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.username = tmp;
            }else if(s[i + 2] == 'i') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.ID = tmp;
            }else if(s[i + 2] == 'd') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.SpecificStationDate = StringToDate(tmp);
            }else if(s[i + 2] == 'n') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.buyNumber = StringToInteger(tmp);
            }else if(s[i + 2] == 'f') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.fromStation = tmp;
            }else if(s[i + 2] == 't') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.toStation = tmp;
            }else if(s[i + 2] == 'q') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                if(tmp == "true") {
                    info.waitList = true;
                }else {
                    info.waitList = false;
                }
            }
        }
    }else if(operation == "query_order") {
        info.a = JaneZ::query_order;
        i += 4;
        std::string tmp;
        while(s[i] != ' ' || i != length) {
            tmp += s[i];
            ++ i;
        }
        info.username = tmp;
    }else if(operation == "refund_ticket") {
        info.a = JaneZ::refund_ticket;
        info.nOrder = 1;
        while(i != length) {
            std::string tmp;
            if(s[i + 2] == 'u') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.username = tmp;
            }else if(s[i + 2] == 'n') {
                i += 4;
                while(s[i] != ' ' || i != length) {
                    tmp += s[i];
                    ++ i;
                }
                info.nOrder = StringToInteger(tmp);
            }
        }
    }else if(operation == "clean") {
        info.a = JaneZ::clean;
    }else if(operation == "exit") {
        info.a = JaneZ::exit;
    }
}

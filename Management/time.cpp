//
// Created by JaneZ on 2025/5/5.
//

#include "../Management/time.h"

JaneZ::Clock::Clock() {
    hour = 0;
    minute = 0;
}

JaneZ::Clock::Clock(const JaneZ::Clock& other) {
    hour = other.hour;
    minute = other.minute;
}

JaneZ::Clock::Clock(int h, int m) {
    hour = h;
    minute = m;
}

JaneZ::Clock &JaneZ::Clock::operator=(const Clock &other){
    if(this != &other) {
        hour = other.hour;
        minute = other.minute;
    }
    return *this;
}

bool JaneZ::Clock::operator==(const Clock &other) const {
    return hour == other.hour && minute == other.minute;
}

bool JaneZ::Clock::operator<(const Clock &other) const {
    if(hour < other.hour) {
        return true;
    }else if(hour > other.hour) {
        return false;
    }else {
        if(minute < other.minute) {
            return true;
        }
        return false;
    }
}

bool JaneZ::Clock::operator<=(const Clock &other) const {
    if(*this == other) {
        return true;
    }else if(*this < other) {
        return true;
    }
    return false;
}

bool JaneZ::Clock::operator>(const Clock &other) const {
    if(*this <= other) {
        return false;
    }
    return true;
}

bool JaneZ::Clock::operator>=(const Clock &other) const {
    if(*this < other) {
        return false;
    }
    return true;
}

JaneZ::Date::Date() {
    month = 6;
    day = 1;
}

JaneZ::Date::Date(const Date &other) {
    month = other.month;
    day = other.day;
}

JaneZ::Date::Date(int m, int d) {
    month = m;
    day = d;
}

JaneZ::Date &JaneZ::Date::operator=(const Date &other) {
    if(this != &other) {
        month = other.month;
        day = other.day;
    }
    return *this;
}

bool JaneZ::Date::operator==(const Date &other) const {
    return month == other.month && day == other.day;
}

bool JaneZ::Date::operator<(const Date &other) const {
    if(month < other.month) {
        return true;
    }else if(month > other.month) {
        return false;
    }else {
        if(day < other.day) {
            return true;
        }
        return false;
    }
}

bool JaneZ::Date::operator<=(const Date &other) const {
    if(*this == other) {
        return true;
    }else if(*this < other) {
        return true;
    }
    return false;
}

bool JaneZ::Date::operator>(const Date &other) const {
    if(*this <= other) {
        return false;
    }
    return true;
}

bool JaneZ::Date::operator>=(const Date &other) const {
    if(*this < other) {
        return false;
    }
    return true;
}

JaneZ::Date &JaneZ::Date::operator++(){
    if(day == months[month]) {
        day = 1;
        month ++;
    }else {
        day ++;
    }
    return *this;
}

;
JaneZ::TrainTime::TrainTime(JaneZ::Date& d, JaneZ::Clock& c):date(d),clock(c){}

JaneZ::TrainTime &JaneZ::TrainTime::operator+(int m){
    clock.minute += m;
    clock.hour += clock.minute / 60;
    clock.minute %= 60;
    int days = clock.hour / 24;
    clock.hour %= 24;
    for (int i = 0; i < days; ++i) {
        ++date;
    }
    return *this;
}

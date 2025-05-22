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

JaneZ::Clock JaneZ::Clock::operator+(int m) const {
    int total_minutes = hour * 60 + minute + m;
    int new_hour = (total_minutes / 60) % 24;
    int new_minute = total_minutes % 60;
    Clock result;
    return Clock{new_hour, new_minute};
}

int JaneZ::Clock::toMinutes() const {
    return hour * 60 + minute;
}

int JaneZ::Clock::operator-(const Clock &other) const {
    return this->toMinutes() - other.toMinutes();
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

JaneZ::Date &JaneZ::Date::operator--() {
    if (--day < 1) {
        if (--month < 1) {
            month = 12;
        }
        day = months[month];
    }
    return *this;
}

int JaneZ::Date::operator-(const Date &other) const {
    if (*this == other) {
        return 0;
    }
    const Date &later = (*this > other) ? *this : other;
    const Date &earlier = (*this > other) ? other : *this;
    int days = 0;
    for (int m = earlier.month; m < later.month; ++m) {
        days += months[m];
    }
    days += later.day - earlier.day;
    return days;
}

JaneZ::Date JaneZ::Date::operator+(int days) const {
    Date result = *this;
    result.day += days;
    while (result.day > months[result.month]) {
        result.day -= months[result.month];
        if (++result.month > 12) {
            result.month = 1;
        }
    }
    return result;
}

JaneZ::Date JaneZ::Date::addMinutesToDateTime(const Date &date, const Clock &clock, int minutes) {
    int total_minutes = clock.toMinutes() + minutes;
    int days_to_add = total_minutes / 1440;
    return date + days_to_add;
}

JaneZ::TrainTime::TrainTime(JaneZ::Date& d, JaneZ::Clock& c):date(d),clock(c){}

JaneZ::TrainTime &JaneZ::TrainTime::operator=(const TrainTime &other) {
    if (this != &other) {
        date = other.date;
        clock = other.clock;
    }
    return *this;
}

bool JaneZ::TrainTime::operator==(const TrainTime &other) const {
    return (date == other.date) && (clock == other.clock);
}

bool JaneZ::TrainTime::operator<(const TrainTime &other) const {
    if (date < other.date) return true;
    if (date == other.date) return clock < other.clock;
    return false;
}

bool JaneZ::TrainTime::operator<=(const TrainTime &other) const {
    return (*this < other) || (*this == other);
}

bool JaneZ::TrainTime::operator>(const TrainTime &other) const {
    return !(*this <= other);
}

bool JaneZ::TrainTime::operator>=(const TrainTime &other) const {
    return !(*this < other);
}

JaneZ::TrainTime JaneZ::TrainTime::AddMinutes(const JaneZ::TrainTime &time, int minutes) {
    JaneZ::TrainTime result = time;
    result.clock.minute += minutes;
    result.clock.hour += result.clock.minute / 60;
    result.clock.minute %= 60;
    int days = result.clock.hour / 24;
    result.clock.hour %= 24;
    for (int i = 0; i < days; ++i) {
        ++result.date;
    }
    return result;
}

JaneZ::TrainTime JaneZ::TrainTime::operator+(int m){
    return AddMinutes(*this, m);
}

int JaneZ::TrainTime::operator-(const TrainTime &other) const {
    int daysDiff = date - other.date;
    int minutesDiff = clock - other.clock;
    return daysDiff * 24 * 60 + minutesDiff;
}

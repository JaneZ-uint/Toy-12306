//
// Created by JaneZ on 2025/5/5.
//

#ifndef TIME_H
#define TIME_H
#include <iostream>

namespace JaneZ {
    struct Clock {
        int hour;
        int minute;

        Clock();

        Clock(const Clock& other);

        Clock(int h,int m);

        Clock &operator=(const Clock &other);

        bool operator==(const Clock &other) const;

        bool operator<(const Clock &other) const;

        bool operator<=(const Clock &other) const;

        bool operator>(const Clock &other) const;

        bool operator>=(const Clock &other) const;

        Clock operator+(int m) const;

        int toMinutes() const;

        int operator-(const Clock &other) const;
    };

    struct Date {
        int month;
        int day;
        int months[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

        Date();

        Date(const Date& other);

        Date(int m,int d);

        Date &operator=(const Date &other);

        bool operator==(const Date &other) const;

        bool operator<(const Date &other) const;

        bool operator<=(const Date &other) const;

        bool operator>(const Date &other) const;

        bool operator>=(const Date &other) const;

        Date &operator++();

        Date& operator--();

        int operator-(const Date &other) const;

        Date operator+(int days) const;

        static Date addMinutesToDateTime(const Date& date, const Clock& clock, int minutes);
    };

    struct TrainTime {
        Date date;
        Clock clock;

        TrainTime() = default;

        TrainTime(Date &d,Clock &c);

        TrainTime& operator=(const TrainTime &other);

        bool operator==(const TrainTime &other) const;

        bool operator<(const TrainTime &other) const;

        bool operator<=(const TrainTime &other) const;

        bool operator>(const TrainTime &other) const;

        bool operator>=(const TrainTime &other) const;

        static JaneZ::TrainTime AddMinutes(const JaneZ::TrainTime& time, int minutes);

        TrainTime operator+(int m);

        int operator-(const TrainTime &other) const;
    };

    inline std::ostream& operator<<(std::ostream& os, const Clock& clock) {
        os << (clock.hour < 10 ? "0" : "") << clock.hour << ":" << (clock.minute < 10 ? "0" : "") << clock.minute;
        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const Date& date) {
        os << (date.month < 10 ? "0" : "") << date.month << "-" << (date.day < 10 ? "0" : "") << date.day;
        return os;
    }

    inline std::ostream& operator<<(std::ostream& os, const TrainTime& trainTime) {
        os << trainTime.date << " " << trainTime.clock;
        return os;
    }
}

#endif //TIME_H

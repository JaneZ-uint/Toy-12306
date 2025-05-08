//
// Created by JaneZ on 2025/5/5.
//

#ifndef TIME_H
#define TIME_H

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
    };

    struct TrainTime {
        Date date;
        Clock clock;

        TrainTime(Date &d,Clock &c);

        TrainTime &operator+(int m);
    };
}

#endif //TIME_H

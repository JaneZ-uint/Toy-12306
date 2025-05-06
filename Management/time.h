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

    };

    struct Date {
        int month;
        int day;
        int months[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

        Date();

        Date(const Date& other);

        Date(int m,int d);

    };
}

#endif //TIME_H

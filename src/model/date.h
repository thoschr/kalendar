#ifndef DATE_H
#define DATE_H

#include <time.h>
#include <string>
#include <list>

using namespace std;

class Time { //TODO move to model && rename into Date
private:
    int mday; //month day
    int wday; //week day
    int month;
    int year;
public:
    Time() {}
    Time(int mday, int wday, int month, int year) {
        this->mday = mday;
        this->wday = wday;
        this->month = month;
        this->year = year;
    }
    int getMonthDay() { return mday; }
    int getWeekDay() { return wday; }
    int getMonth() { return month; }
    int getYear() { return year; }
    void setMonthDay(int mday) { this->mday = mday; }
    void setWeekDay(int wday) { this->wday = wday; }
    void setMonth(int month) { this->month = month; }
    void setYear(int year) { this->year = year; }
    Time& operator=(Time t) {
        this->mday = t.getMonthDay();
        this->wday = t.getWeekDay();
        this->month = t.getMonth();
        this->year = t.getYear();
        return *this;
    }
    bool operator==(Time &t) {
        return (this->mday == t.getMonthDay()) &&
               (this->wday == t.getWeekDay()) &&
               (this->month == t.getMonth()) &&
               (this->year == t.getYear());
    }
    string toString() {
        return to_string(this->mday) + "/" + to_string(this->month) + "/" + to_string(this->year) + " Week-day: " + to_string(this->wday);
    }
};

#endif

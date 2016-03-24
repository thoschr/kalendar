#ifndef DATE_H
#define DATE_H

#include <time.h>
#include <string>
#include <list>

using namespace std;

class Date {
private:
    int mday; //month day
    int wday; //week day
    int month;
    int year;
public:
    Date() {}
    Date(int mday, int wday, int month, int year) {
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
    Date& operator=(Date d) {
        this->mday = d.getMonthDay();
        this->wday = d.getWeekDay();
        this->month = d.getMonth();
        this->year = d.getYear();
        return *this;
    }
    bool operator==(Date &d) {
        return (this->mday == d.getMonthDay()) &&
               (this->wday == d.getWeekDay()) &&
               (this->month == d.getMonth()) &&
               (this->year == d.getYear());
    }
    string toString() {
        return to_string(this->mday) + "/" + to_string(this->month) + "/" + to_string(this->year) + " Week-day: " + to_string(this->wday);
    }
};

#endif

#ifndef EVENT_H
#define EVENT_H

#include <ctime>

#include "category.h"

using namespace std;

class Event
{
private:
    unsigned int id;
    string name;
    string description;
    Category *category;
    /* Timestamp */
    time_t start;
    time_t end;

public:
    Event(Event &event) {
        this->name = event.getName();
        this->description = event.getDescription();
        this->category = new Category(*event.getCategory());
        this->id = event.getId();
        this->start = event.getStart();
        this->end = event.getEnd();
    }

    Event(unsigned int id, string name, string description, Category *category, time_t start, time_t end) {
        this->name = name;
        this->description = description;
        this->category = category;
        if (id == 0)
            /* The returned value from the hash function could be bigger than an integer, so be careful with normal integers.
             * I use an unsigned integer to have always a positive number (also with the overflow). */
            this->id = static_cast<unsigned int> (hash<string>()(this->name + this->description));
        else
            this->id = id;
        this->start = start;
        this->end = end;
    }

    ~Event() {
        delete this->category;
    }

    unsigned int getId() { return id; }
    string getName() { return name; }
    string getDescription() { return description; }
    Category *getCategory() { return category; }
    time_t getStart() { return start; }
    time_t getEnd() { return end; }

    bool equals(Event &e) {
        return (this->id == e.getId()) &&
               (this->start == e.getStart()) &&
               (this->end == e.getEnd());
    }

    long hashcode() {
        return this->id + static_cast<long> ((this->start / 1000) + (this->end - this->start));
    }
};

#endif // EVENT_H

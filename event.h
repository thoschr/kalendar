#ifndef EVENT_H
#define EVENT_H

using namespace std;

class Event
{
private:
    int id;
    string *name;
    string *description;
    string *category;
    unsigned long start;
    unsigned long end;

public:
    Event(int id, string *name, string *description, string *category, unsigned long start, unsigned long end) {
        this->id = id;
        this->name = name;
        this->description = description;
        this->category = category;
        this->start = start;
        this->end = end;
    }

    ~Event() {
        delete this->name;
        delete this->description;
        delete this->category;
    }

    int getId() { return id; }
    string* getName() { return name; }
    string* getDescription() { return description; }
    string* getCategory() { return category; }
    unsigned long getStart() { return start; }
    unsigned long getEnd() { return end; }

};

#endif // EVENT_H

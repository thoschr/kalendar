#ifndef EVENT_H
#define EVENT_H

using namespace std;

class Event
{
private:
    unsigned int id;
    string name;
    string description;
    string category;
    unsigned long start;
    unsigned long end;

public:
    Event(unsigned int id, string name, string description, string category, unsigned long start, unsigned long end) {
        this->name = name;
        this->description = description;
        this->category = category;
        if (id == 0)
            /* The returned value from the hash function could be bigger than an integer, so be careful with normal integers.
             * I use an unsigned integer to have always a positive number (also with the overflow). */
            this->id = static_cast<unsigned int> (hash<string>()(this->name + this->description + this->category));
        else
            this->id = id;
        this->start = start;
        this->end = end;
    }

    ~Event() {
    }

    unsigned int getId() { return id; }
    string getName() { return name; }
    string getDescription() { return description; }
    string getCategory() { return category; }
    unsigned long getStart() { return start; }
    unsigned long getEnd() { return end; }

    bool equals(Event &e) {
        return (this->id == e.getId()) &&
               (this->start == e.getStart()) &&
               (this->end == e.getEnd());
    }

    int hashcode() {
        return this->id + static_cast<int> ((this->start / 1000) + (this->end - this->start));
    }
};

#endif // EVENT_H

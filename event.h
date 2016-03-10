#ifndef EVENT_H
#define EVENT_H

class Event
{
private:
    int id;
    QString *name;
    QString *description;
    QString *category;
    unsigned long start;
    unsigned long end;

public:
    Event(int id, QString *name, QString *description, QString *category, unsigned long start, unsigned long end) {
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
    QString* getName() { return name; }
    QString* getDescription() { return description; }
    QString* getCategory() { return category; }
    unsigned long getStart() { return start; }
    unsigned long getEnd() { return end; }

};

#endif // EVENT_H

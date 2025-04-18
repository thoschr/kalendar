#ifndef EVENT_H
#define EVENT_H

#include <ctime>

#include "category.h"

/* A todo is defined as an event with special dates, they are the following:
 * start = end = 29/01/2105 09:40 , the UTC timestamp is: 4262665200
 * Note: this value is incompatible for 32bit systems due to the 2038 year bug (Reference: https://en.wikipedia.org/wiki/Year_2038_problem)
 * The todo is only an high-level concept managed by the logic view, thus the persistence part consider them as normal events. */
//#define BIT32
#ifndef BIT32
// As explained above, this is for 64 bit systems
#define TODO_DATE 4262665200
#else
// 2147437133 is the 18 Jan 2038, the maximum value representable with a 32 bit system
#define TODO_DATE 2147437133
#endif

using namespace std;

struct Rrule
{
  public:
    Rrule() : freq("NONE"), increment(0) {}
    Rrule(std::string rruleline){
      increment = 0;
      if (rruleline.find("DAILY") != std::string::npos){
        freq = "DAILY";
        repetitions = 730;
      }
      else if (rruleline.find("WEEKLY") != std::string::npos){
        freq = "WEEKLY";
        repetitions = 730;
      }
      else if (rruleline.find("MONTHLY") != std::string::npos){
        freq = "MONTHLY";
        repetitions = 730;
      }
      else if (rruleline.find("YEARLY") != std::string::npos){
        freq = "YEARLY";
        repetitions = 5;
        increment = 31536000;
      } 
      else if (rruleline.find("NONE") != std::string::npos)
        freq = "NONE";
      else
        fprintf(stderr, "Error while processing Rrule: %s\n", rruleline.c_str());
    };
    bool isset(){ if (freq == "DAILY" || freq == "WEEKLY" || freq == "MONTHLY" || freq == "YEARLY") return true; else return false; }
    void reset(){ freq = "NONE"; }
    std::string get_freq(){ return freq; }
    int get_repetitions() const { return repetitions; }
    int get_increment() const { return increment; }
  
  private:
    std::string freq;
    int repetitions;
    int increment;

};

// std::ostream& operator<<(std::ostream& os, const Rrule& rrule);
// inline std::ostream& operator<<(std::ostream& os, Rrule const & rrule) {
//   os << "Rrule Frequency: " << rrule.freq.c_str();
//   return os;
// }

class Event
{
private:
    unsigned int id;
    string name;
    string description;
    string place;
    Category *category;
    /* Timestamp */
    time_t start;
    time_t end;
    Rrule rrule;

public:
    Event(Event &event) {
        this->name = event.getName();
        this->description = event.getDescription();
        this->place = event.getPlace();
        this->category = new Category(*event.getCategory());
        this->id = event.getId();
        this->start = event.getStart();
        this->end = event.getEnd();
        this->rrule = event.getRrule();
    }

    Event(unsigned int id, string name, const string &description, const string &place, Category *category, time_t start, time_t end, std::string rrule = "NONE")
        : id(id), name(name), description(description), place(place), category(category), start(start), end(end), rrule(rrule) {
        this->name = name;
        this->description = description;
        this->place = place;
        this->rrule = Rrule(rrule);  
        if (category == NULL) {
            /* An event with a NULL category is inconsistent, it shouldn't exist */
            this->category = NULL;
            fprintf(stderr, "Event %d (%s) received NULL category.\n", id, name.c_str());
        } else
            this->category = new Category(*category);
        this->start = start;
        this->end = end;
        if (id == 0)
            /* The returned value from the hash function could be bigger than an integer, so be careful with normal integers.
             * I use an unsigned integer to have always a positive number (also with the overflow). */
            this->id = static_cast<unsigned int> (hash<string>()(this->name + this->description + this->place)) + (this->category ? this->category->getId() : 0) + static_cast<unsigned int> ((this->start / 1000) + (this->end - this->start));
        else
            this->id = id;
    }

    ~Event() {
        if (this->category != NULL) delete this->category;
    }

    void setInvalid() {
        this->id = 65535;
    }

    bool isInvalid() {
        return this->id == 65535;
    }

    unsigned int getId() { return id; }
    string getName() { return name; }
    string getDescription() { return description; }
    string getPlace() { return place; }
    Category *getCategory() { return category; }
    time_t getStart() { return start; }
    time_t getEnd() { return end; }
    Rrule getRrule() { return rrule; }

    void setStart(time_t start) { this->start = start; }
    void setEnd(time_t end) { this->end = end; }
    void setId(unsigned int id) { this->id = id; }

    bool equals(Event &e) {
        return (this->id == e.getId());
    }

    long hashcode() {
        return this->id;
    }
};

#endif // EVENT_H

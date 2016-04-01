#include "pmanager_test.h"

#include <QDebug>

PManagerTest::PManagerTest()
{
    time_t timestamp = 966038400L; // = 20/12/1999 I think it's better than time(NULL);
    string test("test_string");
    string specialchars("'/--\"#@");
    /* Categories */
    this->valid_default_category = new Category(1, test, test);
    this->valid_category = new Category(0, test, test);
    this->valid_category_2 = new Category(100, test, test);
    this->noname_category = new Category(0, string(""), test);
    this->specialchars_category = new Category(0, specialchars, specialchars);
    /* Events */
    this->valid_event = new Event(0, test, test, new Category(1, test, test), timestamp, timestamp + 100);
    this->valid_event_2 = new Event(100, test, test, new Category(1, test, test), timestamp, timestamp + 1000000); //starts from current month, ends the next month
    /* Invalid Events */
    this->event_with_null_category = new Event(0, test, test, NULL, timestamp, timestamp);
    this->event_with_invalid_category = new Event(0, test, test, new Category(99, test, test), timestamp, timestamp);
    this->noname_event = new Event(1, string(""), test, new Category(1, test, test), timestamp, timestamp + 100);
    this->invalid_time_event = new Event(1, test, test, new Category(1, test, test), timestamp, timestamp - 100);
    this->specialchars_event = new Event(1, specialchars, specialchars, new Category(1, specialchars, specialchars), timestamp, timestamp + 100);
}

PManagerTest::~PManagerTest() {
    delete this->valid_event;
    delete this->valid_event_2;
    delete this->noname_event;
    delete this->invalid_time_event;
    delete this->noname_category;
    delete this->valid_category;
    delete this->specialchars_category;
    delete this->valid_category_2;
    delete this->valid_default_category;
    delete this->specialchars_event;
    delete this->event_with_invalid_category;
    delete this->event_with_null_category;
}

void PManagerTest::test_all() {
    test_remove_all();
    test_add_event();
    test_get_events_of_month();
    test_remove_event();
    test_add_category();
    test_get_categories();
    test_remove_category();
    test_get_category();
    test_edit_event();
    test_get_all_events();
    test_remove_past_events();
    test_edit_category();
}

void PManagerTest::test_remove_all() {
    Test::print("test_remove_all ");
    PManager pm;
    ASSERT (pm.remove_all())
}

void PManagerTest::test_add_event() {
    Test::print("test_add_event ");
    PManager pm;
    ASSERT ((!(pm.add_event(this->noname_event))) &&
           (!(pm.add_event(this->invalid_time_event))) &&
           (!(pm.add_event(this->event_with_invalid_category))) &&
           (!(pm.add_event(this->event_with_null_category))) &&
           (pm.add_event(this->specialchars_event)) &&
           (pm.add_event(this->valid_event)))
    pm.remove_all();
}

void PManagerTest::test_get_events_of_month() {
    Test::print("test_get_events_of_month ");
    bool ret = false, ret2 = false;
    PManager pm;
    pm.add_event(this->valid_event);
    time_t timestamp = this->valid_event->getStart();
    struct tm *current_time = localtime(&timestamp);
    list<Event*> events = pm.get_events_of_month(current_time->tm_mon + 1, current_time->tm_year + 1900); // tm_mon is from 0 to 11, we need to have 1 - 12
    if (!(events.empty())) {
        list<Event*>::iterator it = events.begin();
        ret = this->valid_event->equals(**it); // *it has type Event*
        delete *it;
    }
    pm.add_event(this->valid_event_2);
    timestamp = this->valid_event_2->getEnd();
    current_time = localtime(&timestamp);
    events = pm.get_events_of_month(current_time->tm_mon + 1, current_time->tm_year + 1900);
    if (!(events.empty())) {
        list<Event*>::iterator it = events.begin();
        ret2 = this->valid_event_2->equals(**it); // *it has type Event*
        delete *it;
    }
    ASSERT (ret && ret2)
    pm.remove_all();
}

void PManagerTest::test_remove_event() {
    Test::print("test_remove_event ");
    bool ret;
    PManager pm;
    pm.add_event(this->valid_event);
    pm.add_event(this->valid_event_2);
    ret = pm.remove_event(this->valid_event);
    time_t timestamp = this->valid_event->getStart();
    struct tm *current_time = localtime(&timestamp);
    list<Event*> events = pm.get_events_of_month(current_time->tm_mon + 1, current_time->tm_year + 1900);
    if (events.size() == 1) {
        list<Event*>::iterator it = events.begin();
        ret = ret && this->valid_event_2->equals(**it); // *it has type Event*
        delete *it;
    } else ret = false;
    ASSERT (ret)
    pm.remove_all();
}

void PManagerTest::test_add_category() {
    Test::print("test_add_category ");
    PManager pm;
    ASSERT ((!(pm.add_category(this->noname_category))) && (pm.add_category(this->valid_category)))
    pm.remove_all();
}

void PManagerTest::test_get_categories() {
    Test::print("test_get_categories ");
    bool ret = false;
    PManager pm;
    pm.add_category(this->valid_category);
    vector<Category*> categories = pm.get_categories();
    if (!(categories.empty())) {
        vector<Category*>::iterator it = categories.begin();
        ret = this->valid_category->equals(**it); // *it has type Category*
        delete *it;
    }
    ASSERT (ret)
    pm.remove_all();
}

void PManagerTest::test_remove_category() {
    Test::print("test_remove_category ");
    bool ret;
    PManager pm;
    pm.add_category(this->valid_category);
    pm.add_category(this->valid_category_2);
    ret = pm.remove_category(this->valid_category);
    ret = ret && pm.remove_category(this->valid_default_category); //Delete the default category
    vector<Category*> categories = pm.get_categories();
    if (categories.size() == 1) {
        vector<Category*>::iterator it = categories.begin();
        ret = ret && this->valid_category_2->equals(**it); // *it has type Category*
        delete *it;
    } else ret = false;
    ret = ret && pm.remove_category(this->valid_category_2);
    ret = ret && (pm.get_categories().size() == 0);
    pm.add_category(this->valid_default_category);
    pm.add_event(this->valid_event);
    ret = ret && (!pm.remove_category(this->valid_default_category)); //try to delete the default category, but it's referenced by valid_event, so the function should fails
    categories = pm.get_categories();
    if (categories.size() == 1) {
        vector<Category*>::iterator it = categories.begin();
        ret = ret && this->valid_default_category->equals(**it); // *it has type Category*
        delete *it;
    } else ret = false;
    ASSERT (ret)
    pm.remove_all();
}

void PManagerTest::test_get_category() {
    Test::print("test_get_category ");
    PManager pm;
    pm.add_category(this->valid_category);
    ASSERT ((pm.get_category(this->valid_category->getId()))->equals(*this->valid_category))
    pm.remove_all();
}

void PManagerTest::test_edit_event() {
    Test::print("test_edit_event ");
    PManager pm;
    pm.add_event(this->valid_event);
    ASSERT (pm.edit_event(this->valid_event, this->valid_event_2) &&
            (!pm.edit_event(this->valid_event_2, this->noname_event)))
    pm.remove_all();
}

void PManagerTest::test_get_all_events() {
    Test::print("test_get_all_events ");
    bool ret = false, ret2 = false;
    PManager pm;
    pm.add_event(this->valid_event);
    pm.add_event(this->valid_event_2);
    list<Event*> events = pm.get_all_events();
    if (!(events.empty())) {
        list<Event*>::iterator it = events.begin();
        ret = (this->valid_event->equals(**it) || this->valid_event_2->equals(**it)); // *it has type Event*
        delete *it;
        it++;
        ret2 = (this->valid_event->equals(**it) || this->valid_event_2->equals(**it));
        delete *it;
    }
    ASSERT (ret && ret2)
    pm.remove_all();
}

void PManagerTest::test_remove_past_events() {
    Test::print("test_remove_past_events ");
    bool ret;
    PManager pm;
    pm.add_event(this->valid_event);
    pm.add_event(this->valid_event_2);
    ret = pm.remove_past_events(this->valid_event_2->getEnd()-1);
    list<Event*> events = pm.get_all_events();
    if (events.size() == 1) {
        list<Event*>::iterator it = events.begin();
        ret = ret && this->valid_event_2->equals(**it); // *it has type Event*
        delete *it;
    } else ret = false;
    ASSERT (ret)
    pm.remove_all();
}

void PManagerTest::test_edit_category() {
    Test::print("test_edit_category ");
    PManager pm;
    pm.add_category(this->valid_category);
    ASSERT (!pm.edit_category(this->valid_category, this->noname_category) &&
            (pm.edit_category(this->valid_category, this->valid_category_2)))
    pm.remove_all();
}

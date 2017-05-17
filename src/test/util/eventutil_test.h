#ifndef EVENTUTIL_TEST_H
#define EVENTUTIL_TEST_H

#include "../test.h"
#include "../../util/eventutil.h"

class EventUtilTest
{
private:
    Event *valid_event;
    Event *event_with_null_category;
    string str_valid_event;
    string str_notitle_event;
    string str_event_with_invalid_category;
    string str_event_title_only;

public:
    EventUtilTest();
    ~EventUtilTest();
    void test_all();
    void test_parseString();
};

#endif // EVENTUTIL_TEST_H

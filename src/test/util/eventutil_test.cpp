#include "eventutil_test.h"

EventUtilTest::EventUtilTest()
{
    time_t timestamp = 966038400L;
    string category("Default");
    string category2("category2");
    string title("event title");
    string place("place");
    string description("this is a really complex description. Infact, there are several characters and white-spaces.");
    /* Events */
    this->valid_event = new Event(0, title, description, place, new Category(1, category, category), timestamp, timestamp + 100);
    this->event_with_null_category = new Event(0, title, description, place, NULL, timestamp, timestamp + 100);
    this->str_valid_event = title + "##" + description + "##" + place + "##" + category + "##" + to_string(timestamp) + "##" + to_string(timestamp + 100);
    this->str_event_title_only = title;
    this->str_event_with_invalid_category = title + "##" + description + "##" + place + "##Doesnt_exists##" + to_string(timestamp) + "##" + to_string(timestamp + 100);
    this->str_notitle_event = "##" + description + "##" + place + "##" + category + "##" + to_string(timestamp) + "##" + to_string(timestamp + 100);
}

EventUtilTest::~EventUtilTest() {
    delete this->valid_event;
    delete this->event_with_null_category;
}

void EventUtilTest::test_all() {
    test_parseString();
}

void EventUtilTest::test_parseString() {
    Test::print("test_parseString");
    ASSERT (EventUtil::parseString(this->str_valid_event)->equals(*this->valid_event) &&
            EventUtil::parseString(this->str_event_with_invalid_category)->equals(*this->event_with_null_category) &&
            EventUtil::parseString(this->str_event_title_only) == NULL &&
            EventUtil::parseString(this->str_notitle_event) == NULL)
}

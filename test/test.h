#ifndef TEST_H
#define TEST_H

#include <iostream>
#include "persistence/pmanager_test.h"

using namespace std;

/* Unit testing class */

class Test
{
public:
    Test();
    void test_all();
    void test_persistence();
    static void print_green(string s) { cout << "\033[32m" << s << "\033[39m" << endl; }
    static void print_red(string s) { cout << "\033[31m" << s << "\033[39m" << endl; }
    static void print(string s) { cout << s << endl; }
};

#endif // TEST_H

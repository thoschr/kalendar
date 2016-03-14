#include "test.h"

Test::Test()
{

}

void Test::test_all() {
    test_persistence();
}

void Test::test_persistence() {
    print("\nStarting persistence tests\n");
    PManagerTest pmt;
    pmt.test_all();
    print("\nStarting util tests\n");
    TimeUtilTest tut;
    tut.test_all();
}

#include "test.h"

Test::Test()
{

}

void Test::test_all() {
    test_persistence();
    test_util();
}

void Test::test_persistence() {
    print("\nStarting persistence tests\n");
    PManagerTest pmt;
    pmt.test_all();
}
void Test::test_util() {
    print("\nStarting util tests\n");
    TimeUtilTest tut;
    tut.test_all();
}

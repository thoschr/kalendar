#include "test.h"

Test::Test()
{

}

void Test::test_all() {
    test_persistence();
}

void Test::test_persistence() {
    print("Starting persistence test\n");
    PManagerTest pmt;
    pmt.test_all();
}

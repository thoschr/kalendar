#include "pmanager_test.h"

PManagerTest::PManagerTest()
{

}

void PManagerTest::test_all() {
    test_pmanager_initialization_without_db();
    test_pmanager_initialization_with_db();
}

void PManagerTest::test_pmanager_initialization_with_db() {
    Test::print("test_pmanager_initialization_with_db ");
    //Create db
    //PManagerTest
}

void PManagerTest::test_pmanager_initialization_without_db() {
    Test::print("test_pmanager_initialization_without_db ");
    //Delete db
    //PManager pm;
}

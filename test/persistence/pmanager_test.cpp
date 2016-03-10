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
    string directory = string(getpwuid(getuid())->pw_dir) + string("/" FOLDER_NAME);
    system((string("rm -r ") + directory).c_str());
    PManager pm;
    string path = directory + string("/" DATABASE_NAME);
    ifstream dbfile(path.c_str());
    if (dbfile) {
        Test::print_green("passed\n");
    } else
        Test::print_red("failed\n");
}

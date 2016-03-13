#include "kalendar.h"
#include "persistence/pmanager.h"
#include <QApplication>
#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>
#include "test/test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kalendar window;
    window.show();
    //PManager pm;
    //Test t;
    //t.test_persistence();

    return a.exec();
}

#include "view/monthview.h"
#include "persistence/pmanager.h"
#include <QApplication>
#include <QDebug>
#include <QPushButton>
#include <QHBoxLayout>
#include "test/test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MonthView window;
    window.show();

    //Test t;
    //t.test_util();

    return a.exec();
}

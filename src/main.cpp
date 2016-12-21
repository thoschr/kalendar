#include "view/monthview.h"
#include "persistence/pmanager.h"
#include <QApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <QPushButton>
#include <QHBoxLayout>
#include "test/test.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MonthView window;
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOptions({
            {{"n", "notify"},
                QCoreApplication::translate("main", "Notify the events in the future <day> starting from today"),
                QCoreApplication::translate("main", "notifynextdays")},
        });
    parser.process(a);
    QString notify =  parser.value("notify");
    if (notify == "") {
        window.show();
    } /* Show notifications about the events in the next days */

    //Test t;
    //t.test_persistence();
    //t.test_util();

    return a.exec();
}

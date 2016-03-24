#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T17:50:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kalendar
TEMPLATE = app


SOURCES += main.cpp\
    persistence/pmanager.cpp \
    test/test.cpp \
    test/persistence/pmanager_test.cpp \
    view/monthview.cpp \
    view/qframe_extended.cpp \
    util/dateutil.cpp \
    test/util/dateutil_test.cpp \
    view/eventdialog.cpp

HEADERS  += model/event.h \
    persistence/pmanager.h \
    model/category.h \
    test/test.h \
    test/persistence/pmanager_test.h \
    model/category.h \
    model/event.h \
    view/monthview.h \
    view/qframe_extended.h \
    model/date.h \
    util/dateutil.h \
    test/util/dateutil_test.h \
    view/eventdialog.h

QMAKE_LIBS += -lsqlite3
QMAKE_CXXFLAGS += -std=c++11

FORMS +=

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
    util/timeutil.cpp \
    test/util/timeutil_test.cpp \
    view/monthview.cpp

HEADERS  += model/event.h \
    persistence/pmanager.h \
    model/category.h \
    test/test.h \
    test/persistence/pmanager_test.h \
    model/category.h \
    model/event.h \
    util/timeutil.h \
    test/util/timeutil_test.h \
    view/monthview.h

FORMS    += kalendar.ui \
    view/monthview.ui

QMAKE_LIBS += -lsqlite3
QMAKE_CXXFLAGS += -std=c++11

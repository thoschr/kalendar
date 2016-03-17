#include "monthview.h"
#include "ui_monthview.h"

#include "../util/timeutil.h"

#include <QDebug>

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>

#define CELL_STYLE "QFrame { background-color: #FFFFFF; border: 1px solid #555555; }" \
                   "QLabel { border: none; font-size: 16px; padding: 5px; }" \
                   "QLabel#today { background-color: #FFFF66; }" \
                   "QLabel#header { background-color: #EEEEEE; padding: auto; font-weight: bold; }"


MonthView::MonthView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MonthView)
{
    Time current_time = TimeUtil::get_current_time();
    int tot_days = TimeUtil::get_days_in_month(current_time.getMonth(), current_time.getYear());
    QLabel *label_date = new QLabel(QString(TimeUtil::get_literal_month(current_time.getMonth()).c_str()) + QString(" ") + QString::number(current_time.getYear()));
    QGridLayout *grid_layout = new QGridLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    label_date->setMaximumHeight(20);
    layout->addWidget(label_date);
    //Create 7x7 grid
    int i,j,x,start_wday;
    //First row contains the names of the days of the week
    for (j = 0; j < 7; j++) {
        QFrame *frame = new QFrame;
        QVBoxLayout *vl = new QVBoxLayout;
        frame->setMinimumWidth(150);
        frame->setMinimumHeight(100);
        QLabel *wday_name = new QLabel(TimeUtil::numeric2literal_day_of_week(j+1).c_str());
        wday_name->setObjectName("header");
        vl->addWidget(wday_name);
        vl->setMargin(0);
        frame->setLayout(vl);
        frame->setStyleSheet(CELL_STYLE);
        grid_layout->addWidget(frame, 0, j);
    }
    //weekday of the first day of the current month
    start_wday = current_time.getWeekDay() - (current_time.getMonthDay() % 7) + 1;
    x = 1;
    for (i = 1; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            QFrame *frame = new QFrame;
            QVBoxLayout *vl = new QVBoxLayout;
            if (((i > 1) || (j >= start_wday-1)) && (x <= tot_days)) {
                QLabel *day = new QLabel(QString::number(x));
                if (x == current_time.getMonthDay())
                    day->setObjectName("today");
                else
                    day->setObjectName("day");
                day->setMaximumHeight(25);
                vl->setAlignment(Qt::AlignTop | Qt::AlignRight);
                vl->addWidget(day);
                vl->setMargin(0);
                x++;
            }
            frame->setMinimumWidth(150);
            frame->setMinimumHeight(100);
            frame->setLayout(vl);
            frame->setStyleSheet(CELL_STYLE);
            grid_layout->addWidget(frame, i, j);
        }
    }
    grid_layout->setHorizontalSpacing(0);
    grid_layout->setVerticalSpacing(0);

    layout->addLayout(grid_layout);

     // Set layout in QWidget
     QWidget *window = new QWidget();
     window->setLayout(layout);
     window->setMinimumHeight(700);
     window->setMinimumWidth(1200);

     // Set QWidget as the central layout of the main window
     setCentralWidget(window);
}

MonthView::~MonthView()
{
    delete ui;
}

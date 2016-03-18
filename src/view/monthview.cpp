#include "monthview.h"
#include "ui_monthview.h"

#include "../util/timeutil.h"

#include "qframe_extended.h"

#include <QDebug>

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

#define CELL_STYLE "QFrame { background-color: #FFFFFF; border: 1px solid #555555; }" \
                   "QLabel { border: none; font-size: 16px; padding: 5px; background-color:rgba(0,0,0,0); }" \
                   "QLabel#today { background-color: #FFFF88; }" \
                   "QFrame#day:hover { background-color: #EEEEFF; }" \
                   "QLabel#header { background-color: #EEEEEE; padding: auto; font-weight: bold; }"


void MonthView::on_mouse_press(QFrameExtended *frame) {
    this->selection_start = frame->getTime();
}

void MonthView::on_mouse_release(QFrameExtended *frame) {
    this->selection_end = frame->getTime();
    //TODO: Show the window to add an event with the start and the end already setted
}

MonthView::MonthView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MonthView)
{
    Time current_time = TimeUtil::get_current_time();
    display_days(current_time);
}

MonthView::~MonthView()
{
    delete ui;
    delete this->grid_layout;
}

void MonthView::display_days(Time time) {
    //The current time is needed to highlight the current day
    Time current_time = TimeUtil::get_current_time();
    int tot_days = TimeUtil::get_days_in_month(time.getMonth(), time.getYear());
    QLabel *label_date = new QLabel(QString(TimeUtil::get_literal_month(time.getMonth()).c_str()) + QString(" ") + QString::number(time.getYear()));
    this->grid_layout = new QGridLayout;
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
    //first week day of the current month
    start_wday = time.getWeekDay() - (time.getMonthDay() % 7) + 1;
    x = 1;
    //Next rows contain the days of the selected month
    for (i = 1; i < 7; i++) { //rows
        for (j = 0; j < 7; j++) { //columns
            //Map a Time object to each frame
            QFrameExtended *frame = new QFrameExtended(Time(x, ((start_wday + (x-1)) % 7) + 1, time.getMonth(), time.getYear()));
            QVBoxLayout *vl = new QVBoxLayout;
            //Checks right cells that will contain the days
            if (((i > 1) || (j >= start_wday-1)) && (x <= tot_days)) {
                QLabel *day = new QLabel(QString::number(x));
                //Checks current day
                if ((x == current_time.getMonthDay()) && (time.getMonth() == current_time.getMonth()) && (time.getYear() == current_time.getYear()))
                    day->setObjectName("today");
                frame->setObjectName("day");
                day->setMaximumHeight(25);
                vl->setAlignment(Qt::AlignTop | Qt::AlignRight);
                vl->addWidget(day);
                vl->setMargin(0);
                connect(frame, &QFrameExtended::pressed, this, &MonthView::on_mouse_press);
                connect(frame, &QFrameExtended::released, this, &MonthView::on_mouse_release);
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

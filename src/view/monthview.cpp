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

    QLabel *label_date = new QLabel(QString(TimeUtil::get_literal_month(current_time.getMonth()).c_str()) + QString(" ") + QString::number(current_time.getYear()));
    this->layout = new QVBoxLayout;
    label_date->setMaximumHeight(20);
    //TODO add horizontal layout to do something like: [<]     March 2016     [>]
    //Where < and > are 2 buttons that get the month of the central cell (never empty, it's an hack) and increase or decrease it
    //refreshing the grid using display_days with the new time
    this->layout->addWidget(label_date);

    //Create 7x7 grid
    QGridLayout *grid_layout = new QGridLayout;
    int i,j;
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

    //Next rows contain the days of the selected month
    for (i = 1; i < 7; i++) { //rows
        for (j = 0; j < 7; j++) { //columns
            //Map a Time object to each frame
            QFrameExtended *frame = new QFrameExtended;
            QVBoxLayout *vl = new QVBoxLayout;
            QLabel *day = new QLabel;
            frame->setObjectName("day");
            day->setMaximumHeight(25);
            vl->setAlignment(Qt::AlignTop | Qt::AlignRight);
            vl->addWidget(day);
            vl->setMargin(0);
            connect(frame, &QFrameExtended::pressed, this, &MonthView::on_mouse_press);
            connect(frame, &QFrameExtended::released, this, &MonthView::on_mouse_release);
            frame->setMinimumWidth(150);
            frame->setMinimumHeight(100);
            frame->setLayout(vl);
            frame->setStyleSheet(CELL_STYLE);
            grid_layout->addWidget(frame, i, j);
            this->frames[7*(i-1)+j] = frame;
        }
    }
    grid_layout->setHorizontalSpacing(0);
    grid_layout->setVerticalSpacing(0);

    //Fill the grid with the days of the default month (i.e. the current month)
    display_days(current_time);

    this->layout->addLayout(grid_layout);

     // Set layout in QWidget
     QWidget *window = new QWidget();
     window->setLayout(this->layout);
     window->setMinimumHeight(700);
     window->setMinimumWidth(1200);

     // Set QWidget as the central layout of the main window
     setCentralWidget(window);
}

MonthView::~MonthView()
{
    delete ui; //seems this frees also this->frames
    delete this->layout;
}

void MonthView::display_days(Time time) {
    //The current time is needed to highlight the current day
    Time current_time = TimeUtil::get_current_time();
    int tot_days = TimeUtil::get_days_in_month(time.getMonth(), time.getYear());
    int i,x,start_wday;

    //first week day of the current month
    start_wday = time.getWeekDay() - (time.getMonthDay() % 7) + 1; //TODO: put this function into timeutil

    x = 1;
    for (i = 0; i < 42; i++) {
        //Checks right cells that will contain the days
        if (( i > 6 || //if I'm after the first week or
            (i % 7 >= start_wday-1)) && //if I'm in the first week and I'm in the right days
            (x <= tot_days)) { //and I'm not out of bound
            //I'll insert into the label the number of the day
            this->frames[i]->setTime(Time(x, ((start_wday + (x-1)) % 7) + 1, time.getMonth(), time.getYear()));
            QLabel *day = static_cast<QLabel*> (this->frames[i]->childAt(0,0));
            day->setText(QString::number(x));
            //Checks current day
            if ((x == current_time.getMonthDay()) && (time.getMonth() == current_time.getMonth()) && (time.getYear() == current_time.getYear()))
                day->setObjectName("today");
            x++;
        }
    }
}

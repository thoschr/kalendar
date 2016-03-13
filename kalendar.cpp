#include "kalendar.h"
#include "ui_kalendar.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>

#define CELL_STYLE "QFrame { background-color: #FFFFFF; border: 1px solid #555555; height: 100px; width: 100px; }" \
                   "QLabel { border:none; }"

Kalendar::Kalendar(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Kalendar)
{
    QLabel *label_date = new QLabel("Marzo 2016");
    QGridLayout *grid_layout = new QGridLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label_date);
    //Create 7x7 grid
    int i,j;
    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) {
            QLabel *day = new QLabel("x");
            QFrame *frame = new QFrame;
            QVBoxLayout *vl = new QVBoxLayout;
            vl->addWidget(day);
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

     // Set QWidget as the central layout of the main window
     setCentralWidget(window);
}

Kalendar::~Kalendar()
{
    delete ui;
}
